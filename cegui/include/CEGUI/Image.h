/***********************************************************************
    filename:   CEGUIImage.h
    created:    Wed Feb 16 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIImage_h_
#define _CEGUIImage_h_

#include "CEGUI/ChainedXMLHandler.h"
#include "CEGUI/String.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/Rect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Interface for Image.

    In CEGUI, an Image is some object that can render itself into a given
    GeometryBuffer object.  This may be something as simple as a basic textured
    quad, or something more complex.
*/
class CEGUIEXPORT Image :
    public AllocatedObject<Image>,
    public ChainedXMLHandler
{
public:
    virtual ~Image();

    virtual const String& getName() const = 0;

    virtual const Sizef& getRenderedSize() const = 0;
    virtual const Vector2f& getRenderedOffset() const = 0;

    virtual void render(GeometryBuffer& buffer,
                        const Rectf& dest_area,
                        const Rectf* clip_area,
                        const ColourRect& colours) const = 0;

    virtual void notifyDisplaySizeChanged(const Sizef& size) = 0;

    // Standard Image::render overloads
    void render(GeometryBuffer& buffer,
                const Vector2f& position,
                const Rectf* clip_area = 0) const
    {
        const ColourRect colours(0XFFFFFFFF);
        render(buffer, Rectf(position, getRenderedSize()), clip_area, colours);
    }

    void render(GeometryBuffer& buffer,
                const Vector2f& position,
                const Rectf* clip_area,
                const ColourRect& colours) const
    {
        render(buffer, Rectf(position, getRenderedSize()), clip_area, colours);
    }

    void render(GeometryBuffer& buffer,
                const Vector2f& position,
                const Sizef& size,
                const Rectf* clip_area = 0) const
    {
        const ColourRect colours(0XFFFFFFFF);
        render(buffer, Rectf(position, size), clip_area, colours);
    }

    void render(GeometryBuffer& buffer,
                const Vector2f& position,
                const Sizef& size,
                const Rectf* clip_area,
                const ColourRect& colours) const
    {
        render(buffer, Rectf(position, size), clip_area, colours);
    }

protected:
    // implement chained xml handler abstract interface
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIImage_h_

