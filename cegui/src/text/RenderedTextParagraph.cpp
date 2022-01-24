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
#include "CEGUI/text/RenderedTextParagraph.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateLineHeights(float defaultFontHeight)
{
    uint32_t lineStartGlyphIdx = 0;
    for (auto& line : lines)
    {
        //if (p/line.heightDirty)
        {
            //p/line.heightDirty = false;

            if (lineStartGlyphIdx == line.glyphEndIdx)
            {
                // An empty line uses the height of the previous text glyph
                uint32_t i = lineStartGlyphIdx;
                for (; i > 0; --i)
                {
                    if (!glyphs[i].isEmbeddedObject)
                    {
                        line.extents.d_height = glyphs[i].height;
                        break;
                    }
                }

                if (!i)
                    line.extents.d_height = (!glyphs[i].isEmbeddedObject) ? glyphs[i].height : defaultFontHeight;
            }
            else
            {
                line.extents.d_height = 0.f;
                for (size_t i = lineStartGlyphIdx; i < line.glyphEndIdx; ++i)
                {
                    const float glyphHeight = glyphs[i].height;
                    if (line.extents.d_height < glyphHeight)
                        line.extents.d_height = glyphHeight;
                }
            }
        }

        lineStartGlyphIdx = line.glyphEndIdx;
    }
}

//----------------------------------------------------------------------------//
void RenderedTextParagraph::updateHorizontalFormatting(float areaWidth)
{
    if (linesDirty)
        return;

    for (auto& line : lines)
    {
        const bool isLastLine = (&line == &lines.back());
        const auto lineHorzFmt = (isLastLine && horzFormatting == HorizontalTextFormatting::Justified) ?
            lastJustifiedLineHorzFormatting :
            horzFormatting;

        switch (lineHorzFmt)
        {
            case HorizontalTextFormatting::RightAligned:
                line.horzOffset = areaWidth - line.extents.d_width;
                line.justifySpaceSize = 0.f;
                break;
            case HorizontalTextFormatting::CentreAligned:
                line.horzOffset = (areaWidth - line.extents.d_width) * 0.5f;
                line.justifySpaceSize = 0.f;
                break;
            case HorizontalTextFormatting::Justified:
                line.horzOffset = 0.f;
                if (line.justifyableCount && line.extents.d_width < areaWidth)
                    line.justifySpaceSize = (areaWidth - line.extents.d_width) / line.justifyableCount;
                break;
        }
    }
}

}
