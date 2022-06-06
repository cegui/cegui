/***********************************************************************
    created:    Wed Aug 5 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#include "CEGUI/Config.h"
#include "CEGUI/text/BidiVisualMapping.h"
#include "CEGUI/Logger.h"

#if defined (CEGUI_USE_FRIBIDI)
#include <fribidi.h>
#include <memory>
#include <cstring>
#elif defined (CEGUI_USE_MINIBIDI)
// https://gitlab.com/arabeyes-dev/adawat/-/blob/master/minibidi/minibidi.c
// TODO: check for modifications against our copy
#include "minibidi.cpp" // include minibidi code directly
#endif

namespace CEGUI
{

//----------------------------------------------------------------------------//
BidiCharType BidiVisualMapping::getBidiCharType(const char32_t codepoint)
{
#if defined (CEGUI_USE_FRIBIDI)
    switch (fribidi_get_bidi_type((FriBidiChar)codepoint))
    {
        case FRIBIDI_TYPE_RTL: return BidiCharType::RIGHT_TO_LEFT;
        case FRIBIDI_TYPE_LTR: return BidiCharType::LEFT_TO_RIGHT;
        default: return BidiCharType::NEUTRAL;
    }
#elif defined (CEGUI_USE_MINIBIDI)
    switch (getType(codepoint))
    {
        case R: return BidiCharType::RIGHT_TO_LEFT;
        case L: return BidiCharType::LEFT_TO_RIGHT;
        default: return BidiCharType::NEUTRAL;
    }
#else
    return BidiCharType::NEUTRAL;
#endif
}

//----------------------------------------------------------------------------//
bool BidiVisualMapping::applyBidi(const String& logical, String& outVisual,
    std::vector<int>& l2v, std::vector<int>& v2l, DefaultParagraphDirection& dir)
{
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    return applyBidi(logical.getString(), outVisual.getString(), l2v, v2l, dir);
#else
    std::u32string utf32Str = String::convertUtf8ToUtf32(logical.c_str());
    if (utf32Str.empty())
        return false;

    if (!applyBidiInplace(utf32Str, l2v, v2l, dir))
        return false;

    outVisual = String::convertUtf32ToUtf8(utf32Str.c_str());

    return true;
#endif
}

//----------------------------------------------------------------------------//
bool BidiVisualMapping::applyBidi(const String& logical, std::u32string& outVisual,
    std::vector<int>& l2v, std::vector<int>& v2l, DefaultParagraphDirection& dir)
{
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    return applyBidi(logical.getString(), outVisual, l2v, v2l, dir);
#else
    outVisual = String::convertUtf8ToUtf32(logical.c_str());
    return !outVisual.empty() && applyBidiInplace(outVisual, l2v, v2l, dir);
#endif
}

//----------------------------------------------------------------------------//
bool BidiVisualMapping::applyBidi(const char32_t* start, size_t length, std::u32string& outVisual,
    std::vector<int>& l2v, std::vector<int>& v2l, DefaultParagraphDirection& dir)
{
    const bool inplace = (start == outVisual.c_str());

    // Forbid aliasing other than exact matching of source and destination pointers
    if (!inplace && start > outVisual.c_str() && start <= (outVisual.c_str() + length))
        return false;

    l2v.resize(length);
    v2l.resize(length);

    if (length <= 1)
    {
        if (!inplace)
            outVisual.assign(start, length);

        if (length)
        {
            l2v[0] = 0;
            v2l[0] = 0;
        }
        return true;
    }

#if defined (CEGUI_USE_FRIBIDI)
    FriBidiChar* outputBuffer;
    std::unique_ptr<FriBidiChar[]> outputFribidiStr;
    if (inplace)
    {
        outputFribidiStr.reset(new FriBidiChar[length]);
        outputBuffer = outputFribidiStr.get();
    }
    else
    {
        outVisual.resize(length);
        outputBuffer = reinterpret_cast<FriBidiChar*>(const_cast<char32_t*>(outVisual.c_str()));
    }

    FriBidiCharType baseDir =
        (dir == DefaultParagraphDirection::RightToLeft) ? FRIBIDI_PAR_RTL :
        (dir == DefaultParagraphDirection::Automatic) ? FRIBIDI_PAR_ON :
        FRIBIDI_PAR_LTR;

    fribidi_boolean success = fribidi_log2vis(
        reinterpret_cast<const FriBidiChar*>(start), static_cast<FriBidiStrIndex>(length),
        &baseDir, outputBuffer, &l2v[0], &v2l[0], nullptr);

    if (!success)
    {
        if (!inplace)
            outVisual.assign(start, length);

        Logger::getSingleton().logEvent(
            "FribidiVisualMapping::reorderFromLogicalToVisual: fribidi_log2vis "
            "call failed on logical string: " + String(start, length), LoggingLevel::Error);

        return false;
    }

    // Get resolved paragraph direction
    dir = (baseDir == FRIBIDI_PAR_RTL) ? DefaultParagraphDirection::RightToLeft :
        (baseDir == FRIBIDI_PAR_ON) ? DefaultParagraphDirection::Automatic :
        DefaultParagraphDirection::LeftToRight;

    if (inplace)
    {
        outVisual.resize(length);
        std::memcpy(const_cast<char32_t*>(outVisual.c_str()), outputBuffer, length);
    }

    return true;
#elif defined (CEGUI_USE_MINIBIDI)
    if (!inplace)
        outVisual.assign(start, length);

    doBidi(reinterpret_cast<unsigned int*>(const_cast<char32_t*>(outVisual.c_str())),
        static_cast<int>(length), true, false, &v2l[0], &l2v[0]);

    return true;
#else
    return false;
#endif
}

}
