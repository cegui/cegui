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
#ifndef _CEGUIRenderedStringComponent_h_
#define _CEGUIRenderedStringComponent_h_

#include "CEGUI/Sizef.h"
#include "CEGUI/Rectf.h"
#include "CEGUI/falagard/Enums.h"

#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Base class representing a part of a rendered string.  The 'part' represented
    may be a text string, an image or some other entity.
*/
class CEGUIEXPORT RenderedStringComponent
{
public:
    //! Destructor.
    virtual ~RenderedStringComponent();

    //! Set the VerticalTextFormatting option for this component.
    void setVerticalTextFormatting(VerticalTextFormatting fmt);
    //! return the current VerticalTextFormatting option.
    VerticalTextFormatting getVerticalTextFormatting() const;
    //! set the padding values.
    void setPadding(const Rectf& padding);
    //! set the left padding value.
    void setLeftPadding(const float padding);
    //! set the right padding value.
    void setRightPadding(const float padding);
    //! set the top padding value.
    void setTopPadding(const float padding);
    //! set the Bottom padding value.
    void setBottomPadding(const float padding);
    //! return the current padding value Rect.
    const Rectf& getPadding() const;
    //! return the left padding value.
    float getLeftPadding() const;
    //! return the right padding value.
    float getRightPadding() const;
    //! return the top padding value.
    float getTopPadding() const;
    //! return the bottom padding value.
    float getBottomPadding() const;

    //! draw the component.
    virtual std::vector<GeometryBuffer*> createRenderGeometry(
        const Window* ref_wnd,
        const glm::vec2& position, const ColourRect* mod_colours,
        const Rectf* clip_rect, const float vertical_space,
        const float space_extra) const = 0;

    //! return the pixel size of the rendered component.
    virtual Sizef getPixelSize(const Window* ref_wnd) const = 0;

    //! return whether the component can be split
    virtual bool canSplit() const = 0;

    /*!
    \brief
        split the component as close to split_point as possible, returning a
        new RenderedStringComponent of the same type as '*this' holding the
        left side of the split, and leaving the right side of the split in
        this object.

    \param was_word_split
        return whether any word was split into 2 or more lines, because it
        couldn't fit in a single line.
    
    \exception InvalidRequestException
        thrown if the RenderedStringComponent does not support being split.
    */
    virtual RenderedStringComponent* split(const Window* ref_wnd,
                                           float split_point,
                                           bool first_component,
                                           bool& was_word_split) = 0;

    /*!
    \brief
        split the component as close to split_point as possible, returning a
        new RenderedStringComponent of the same type as '*this' holding the
        left side of the split, and leaving the right side of the split in
        this object.
    
    \exception InvalidRequestException
        thrown if the RenderedStringComponent does not support being split.
    */
    RenderedStringComponent* split(const Window* ref_wnd,
                                   float split_point,
                                   bool first_component);

    //! clone this component.
    virtual RenderedStringComponent* clone() const = 0;

    //! return the total number of spacing characters in the string.
    virtual size_t getSpaceCount() const = 0;

    //! mark some region appropriate given /a start and /a end as selected.
    virtual void setSelection(const Window* ref_wnd,
                              const float start, const float end) = 0;

protected:
    //! Protected constructor.
    RenderedStringComponent();

    //! Rect object holding the padding values for this component.
    Rectf d_padding;
    //! Vertical formatting to be used for this component.
    VerticalTextFormatting d_verticalTextFormatting = VerticalTextFormatting::BottomAligned;
    //! Image to draw for selection
    const Image* d_selectionImage;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUIRenderedStringComponent_h_
