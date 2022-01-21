/***********************************************************************
    created:    25/05/2009
    author:     Paul Turner
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
#ifndef _CEGUIRenderedStringWordWrapper_h_
#define _CEGUIRenderedStringWordWrapper_h_

#include "CEGUI/FormattedRenderedString.h"
#include <vector>

namespace CEGUI
{
/*!
\brief
    Class that handles wrapping of a rendered string into sub-strings. Each
    sub-string is rendered using the FormattedRenderedString based class 'T'.
*/
template <typename T>
class RenderedStringWordWrapper : public FormattedRenderedString
{
public:

    HorizontalTextFormatting getCorrespondingFormatting() const override;

    void format(const RenderedString& rs, const Window* refWnd, const Sizef& areaSize) override;

    void createRenderGeometry(std::vector<GeometryBuffer*>& out, const Window* refWnd,
        const glm::vec2& position, const ColourRect* modColours, const Rectf* clipRect) const override;

    size_t getFormattedLineCount() const override
    {
        size_t ret = 0;
        for (const auto& line : d_lines)
            ret += line->getFormattedLineCount();
        return ret;
    }

    bool wasWordSplit() const override { return d_wasWordSplit; }

protected:

    // NB: formatter doesn't own a string, so store strings separately
    std::vector<std::unique_ptr<FormattedRenderedString>> d_lines;
    std::vector<RenderedString> d_strings;
    bool d_wasWordSplit = false;
};

//----------------------------------------------------------------------------//
template <> CEGUIEXPORT
inline HorizontalTextFormatting RenderedStringWordWrapper<LeftAlignedRenderedString>::getCorrespondingFormatting() const
{
    return HorizontalTextFormatting::WordWrapLeftAligned;
}
template <> CEGUIEXPORT
inline HorizontalTextFormatting RenderedStringWordWrapper<RightAlignedRenderedString>::getCorrespondingFormatting() const
{
    return HorizontalTextFormatting::WordWrapRightAligned;
}
template <> CEGUIEXPORT
inline HorizontalTextFormatting RenderedStringWordWrapper<CentredRenderedString>::getCorrespondingFormatting() const
{
    return HorizontalTextFormatting::WordWrapCentreAligned;
}
template <> CEGUIEXPORT
inline HorizontalTextFormatting RenderedStringWordWrapper<JustifiedRenderedString>::getCorrespondingFormatting() const
{
    return HorizontalTextFormatting::WordWrapJustified;
}

//----------------------------------------------------------------------------//
template <typename T>
HorizontalTextFormatting RenderedStringWordWrapper<T>::getCorrespondingFormatting() const
{
    //static_assert(false, "Unknown string formatter type");
    return HorizontalTextFormatting::WordWrapLeftAligned;
}

//----------------------------------------------------------------------------//
//! specialised version of format used with Justified text
template <> CEGUIEXPORT
void RenderedStringWordWrapper<JustifiedRenderedString>::format(
    const RenderedString& rs, const Window* refWnd, const Sizef& areaSize);

//----------------------------------------------------------------------------//
template <typename T>
void RenderedStringWordWrapper<T>::format(const RenderedString& rs, const Window* refWnd, const Sizef& areaSize)
{
    d_renderedString = &rs;

    d_strings.clear();
    d_lines.clear();

    d_wasWordSplit = false;

    RenderedString lstring;
    RenderedString rstring = d_renderedString->clone();

    for (size_t line = 0; line < rstring.getLineCount(); ++line)
    {
        float rsWidth;
        while ((rsWidth = rstring.getLineExtent(refWnd, line).d_width) > areaSize.d_width)
        {
            d_wasWordSplit |= rstring.split(refWnd, line, areaSize.d_width, lstring);
            d_strings.push_back(std::move(lstring));
            d_lines.push_back(std::make_unique<T>());

            line = 0;
        }
    }

    // last line.
    d_strings.push_back(std::move(rstring));
    d_lines.push_back(std::make_unique<T>());

    // Now format all lines
    for (size_t i = 0; i < d_lines.size(); ++i)
        d_lines[i]->format(d_strings[i], refWnd, areaSize);

    // Update extent
    d_extent.d_width = 0.f;
    d_extent.d_height = 0.f;
    for (const auto& line : d_lines)
    {
        const auto& lineExtent = line->getExtent();
        if (d_extent.d_width < lineExtent.d_width)
            d_extent.d_width = lineExtent.d_width;
        d_extent.d_height += lineExtent.d_height;
    }
}

//----------------------------------------------------------------------------//
template <typename T>
void RenderedStringWordWrapper<T>::createRenderGeometry(std::vector<GeometryBuffer*>& out,
    const Window* refWnd, const glm::vec2& position, const ColourRect* modColours, const Rectf* clipRect) const
{
    glm::vec2 linePos = position;
    for (const auto& line : d_lines)
    {
        line->createRenderGeometry(out, refWnd, linePos, modColours, clipRect);
        linePos.y += line->getExtent().d_height;
    }
}

}

#endif
