/***********************************************************************
    filename:   CEGUIRenderedStringImageComponent.h
    created:    24/05/2009
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
#ifndef _CEGUIRenderedStringImageComponent_h_
#define _CEGUIRenderedStringImageComponent_h_

#include "CEGUIRenderedStringComponent.h"
#include "CEGUIColourRect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! String component that draws an image.
class CEGUIEXPORT RenderedStringImageComponent : public RenderedStringComponent
{
public:
    //! Constructor
    RenderedStringImageComponent();
    RenderedStringImageComponent(const String& imageset, const String& image);
    RenderedStringImageComponent(const Image* image);

    //! Set the image to be drawn by this component.
    void setImage(const String& imageset, const String& image);
    //! Set the image to be drawn by this component.
    void setImage(const Image* image);
    //! return the current set image that will be drawn by this component
    const Image* getImage() const;
    //! Set the colour values used when rendering this component.
    void setColours(const ColourRect& cr);
    //! Set the colour values used when rendering this component.
    void setColours(const colour& c);
    //! return the ColourRect object used when drawing this component.
    const ColourRect& getColours() const;
    //! set the size for rendering the image (0s mean 'normal' size)
    void setSize(const Size& sz);
    //! return the size for rendering the image (0s mean 'normal' size)
    const Size& getSize() const;

    // implementation of abstract base interface
    void draw(GeometryBuffer& buffer, const Vector2& position,
              const ColourRect* mod_colours, const Rect* clip_rect,
              const float vertical_space, const float space_extra) const;
    Size getPixelSize() const;
    bool canSplit() const;
    RenderedStringImageComponent* split(float split_point, bool first_component);
    RenderedStringImageComponent* clone() const;
    size_t getSpaceCount() const;

protected:
    //! pointer to the image drawn by the component.
    const Image* d_image;
    //! ColourRect object describing the colours to use when rendering.
    ColourRect d_colours;
    //! target size to render the image at (0s mean natural size)
    Size d_size;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIRenderedStringImageComponent_h_
