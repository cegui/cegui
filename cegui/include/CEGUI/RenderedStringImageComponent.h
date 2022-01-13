/***********************************************************************
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

#include "CEGUI/RenderedStringComponent.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/String.h"
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! String component that draws an image.
class CEGUIEXPORT RenderedStringImageComponent : public RenderedStringComponent
{
public:

    RenderedStringImageComponent() = default;
    RenderedStringImageComponent(const Image* image);

    //! Set the image to be drawn by this component.
    void setImage(const Image* image) { d_image = image; }
    //! return the current set image that will be drawn by this component
    const Image* getImage() const { return d_image; }
    //! Set the colour values used when rendering this component.
    void setColours(const ColourRect& cr) { d_colours = cr; }
    //! Set the colour values used when rendering this component.
    void setColours(const Colour& c) { d_colours.setColours(c); }
    //! return the ColourRect object used when drawing this component.
    const ColourRect& getColours() const { return d_colours; }
    //! set the size for rendering the image (0s mean 'normal' size)
    void setSize(const Sizef& sz) { d_size = sz; }
    //! return the size for rendering the image (0s mean 'normal' size)
    const Sizef& getSize() const { return d_size; }
    //! Sets the vertical image formatting of this image
    void setVerticalImageFormatting(VerticalImageFormatting fmt) { d_verticalImageFormatting = fmt; }
    //! Gets the vertical image formatting of this image
    VerticalImageFormatting getVerticalImageFormatting() const { return d_verticalImageFormatting; }

    // implementation of abstract base interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out,
        const Window* ref_wnd,
        const glm::vec2& position, const ColourRect* mod_colours,
        const Rectf* clip_rect, const float vertical_space,
        const float space_extra) const override;
    Sizef getPixelSize(const Window* ref_wnd) const override;
    bool canSplit() const override { return false; }
    RenderedStringComponentPtr split(const Window* ref_wnd,
                                        float split_point,
                                        bool first_component,
                                        bool& was_word_split) override;
    RenderedStringComponentPtr clone() const override;
    size_t getSpaceCount() const override { return 0; }
    void setSelection(const Window* ref_wnd,
                      const float start, const float end) override;

protected:

    //! pointer to the image drawn by the component.
    const Image* d_image = nullptr;
    //! ColourRect object describing the colours to use when rendering.
    ColourRect d_colours = 0xFFFFFFFF;
    //! target size to render the image at (0s mean natural size)
    Sizef d_size;
    //! The vertical image formatting for the image
    VerticalImageFormatting d_verticalImageFormatting = VerticalImageFormatting::BottomAligned;
    // whether the image is marked as selected.
    bool d_selected = false;
};

}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
