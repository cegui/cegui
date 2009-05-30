/***********************************************************************
    filename:   CEGUILeftAlignedRenderedString.cpp
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
#include "CEGUILeftAlignedRenderedString.h"
#include "CEGUIRenderedString.h"
#include "CEGUIVector.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
LeftAlignedRenderedString::LeftAlignedRenderedString(
        const RenderedString& string) :
    FormattedRenderedString(string)
{
}

//----------------------------------------------------------------------------//
void LeftAlignedRenderedString::format(const Size& area_size)
{
}

//----------------------------------------------------------------------------//
void LeftAlignedRenderedString::draw(GeometryBuffer& buffer,
                                 const Vector2& position,
                                 const Rect* clip_rect) const
{
    d_renderedString.draw(buffer, position, clip_rect);
}

//----------------------------------------------------------------------------//
size_t LeftAlignedRenderedString::getFormattedLineCount() const
{
    // always one line for basic left aligned formatting.
    return 1;
}

//----------------------------------------------------------------------------//
float LeftAlignedRenderedString::getHorizontalExtent() const
{
    return d_renderedString.getPixelSize().d_width;
}

//----------------------------------------------------------------------------//
float LeftAlignedRenderedString::getVerticalExtent() const
{
    return d_renderedString.getPixelSize().d_height;
}

//----------------------------------------------------------------------------//
    
} // End of  CEGUI namespace section
