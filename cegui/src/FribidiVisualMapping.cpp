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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif
#include "CEGUI/Config.h"

#ifdef CEGUI_USE_FRIBIDI

#include "CEGUI/FribidiVisualMapping.h"
#include "CEGUI/Logger.h"
#include <fribidi.h>
#include <memory>

namespace CEGUI
{

//----------------------------------------------------------------------------//
BidiCharType FribidiVisualMapping::getBidiCharType(const char32_t char_to_check) const
{
    switch (fribidi_get_type((FriBidiChar)char_to_check))
    {
        case FRIBIDI_TYPE_RTL: return BidiCharType::RIGHT_TO_LEFT;
        case FRIBIDI_TYPE_LTR: return BidiCharType::LEFT_TO_RIGHT;
        default: return BidiCharType::NEUTRAL;
    }
}

//----------------------------------------------------------------------------//
bool FribidiVisualMapping::reorderFromLogicalToVisual(const String& logical,
                                                       String& visual,
                                                       std::vector<int>& l2v,
                                                       std::vector<int>& v2l) const
{
    if (logical.size() <= 1)
    {
        l2v.resize(logical.size());
        v2l.resize(logical.size());

        visual = logical;

        if (logical.size() == 1)
        {
            l2v[0] = 0;
            v2l[0] = 0;
        }
        return true;
    }

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    const FriBidiChar* inputFribidiStr = reinterpret_cast<const FriBidiChar*>(logical.c_str());
    const size_t stringLength = logical.size();
#else
    std::u32string utf32Str = String::convertUtf8ToUtf32(logical.c_str());
    const FriBidiChar* inputFribidiStr = reinterpret_cast<const FriBidiChar*>(utf32Str.c_str());
    const size_t stringLength = utf32Str.size();
    if (!stringLength)
        return false;
#endif

    std::unique_ptr<FriBidiChar[]> outputFribidiStr(new FriBidiChar[stringLength]);

    l2v.resize(stringLength);
    v2l.resize(stringLength);

    FriBidiCharType input_base_direction = FRIBIDI_TYPE_L;
    fribidi_boolean success =
        fribidi_log2vis(inputFribidiStr,
                        static_cast<FriBidiStrIndex>(stringLength),
                        &input_base_direction,
                        outputFribidiStr.get(),
                        &l2v[0], &v2l[0], nullptr);

    if (success)
    {
        visual = String(reinterpret_cast<const char32_t*>(outputFribidiStr.get()), stringLength);
        return true;
    }

    visual = logical;

    Logger::getSingleton().logEvent(
        "FribidiVisualMapping::reorderFromLogicalToVisual: fribidi_log2vis "
        "call failed on logical string: " + logical, LoggingLevel::Error);

    return false;
}

}

#endif
