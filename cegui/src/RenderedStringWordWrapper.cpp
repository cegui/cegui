/***********************************************************************
    created:    06/07/2009
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
#include "CEGUI/RenderedStringWordWrapper.h"
#include "CEGUI/RenderedString.h"
#include "CEGUI/LeftAlignedRenderedString.h"
#include "CEGUI/RightAlignedRenderedString.h"
#include "CEGUI/CentredRenderedString.h"
#include "CEGUI/JustifiedRenderedString.h"

// The purpose of this file is to define a specialised RenderedStringWordWrapper
// for Justified formatting so that the last line is handled correctly.

namespace CEGUI
{

//----------------------------------------------------------------------------//
template <>
void RenderedStringWordWrapper<JustifiedRenderedString>::format(const RenderedString& rs, const Window* refWnd, const Sizef& areaSize)
{
    d_renderedString = &rs;

    d_lines.clear();
    d_strings.clear();

    RenderedString lstring;
    RenderedString rstring = d_renderedString->clone();

    for (size_t line = 0; line < rstring.getLineCount(); ++line)
    {
        float rsWidth;
        while ((rsWidth = rstring.getLineExtent(refWnd, line).d_width) > areaSize.d_width)
        {
            d_wasWordSplit |= rstring.split(refWnd, line, areaSize.d_width, lstring);
            d_strings.push_back(std::move(lstring));
            d_lines.push_back(std::make_unique<JustifiedRenderedString>());

            line = 0;
        }
    }

    // last line (which we do not justify)
    d_strings.push_back(std::move(rstring));
    d_lines.push_back(std::make_unique<LeftAlignedRenderedString>());

    // Now format all lines
    for (size_t i = 0; i < d_lines.size(); ++i)
        d_lines[i]->format(d_strings[i], refWnd, areaSize);
}

}
