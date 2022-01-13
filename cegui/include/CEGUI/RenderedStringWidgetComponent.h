/***********************************************************************
    created:    27/05/2009
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
#ifndef _CEGUIRenderedStringWidgetComponent_h_
#define _CEGUIRenderedStringWidgetComponent_h_

#include "CEGUI/RenderedStringComponent.h"
#include "CEGUI/String.h"

namespace CEGUI
{
//! String component that moves a widget to appear as part of the string.
class CEGUIEXPORT RenderedStringWidgetComponent : public RenderedStringComponent
{
public:

    RenderedStringWidgetComponent() = default;
    RenderedStringWidgetComponent(const String& widget_name);
    RenderedStringWidgetComponent(Window* widget);

    //! Set the window to be controlled by this component.
    void setWindow(const String& widget_name);
    //! Set the window to be controlled by this component.
    void setWindow(Window* widget);
    //! return the window currently controlled by this component
    const Window* getWindow() const { return getEffectiveWindow(nullptr); }
    //! Set the VerticalTextFormatting option for this component.
    void setVerticalTextFormatting(VerticalTextFormatting fmt) { d_verticalTextFormatting = fmt; }
    //! return the current VerticalTextFormatting option.
    VerticalTextFormatting getVerticalTextFormatting() const { return d_verticalTextFormatting; }

    // implementation of abstract base interface
    void createRenderGeometry(std::vector<GeometryBuffer*>& out, const Window* refWnd,
        const glm::vec2& position, const ColourRect* mod_colours,
        const Rectf* clip_rect, const float vertical_space,
        const float space_extra) const override;
    Sizef getPixelSize(const Window* refWnd) const override;
    bool canSplit() const override { return false; }
    RenderedStringComponentPtr split(const Window* refWnd,
                                         float split_point,
                                         bool first_component,
                                         bool& was_word_split) override;
    RenderedStringComponentPtr clone() const override;
    size_t getSpaceCount() const override { return 0; }
    void setSelection(const Window* refWnd,
                      const float start, const float end) override;

protected:

    Window* getEffectiveWindow(const Window* refWnd) const;

    //! pointer to the window controlled by this component.
    mutable Window* d_window = nullptr;
    //! Name of window to manipulate
    String d_windowName;
    //! Vertical formatting to be used for this component.
    VerticalTextFormatting d_verticalTextFormatting = VerticalTextFormatting::BottomAligned;
    //! whether d_window is synched.
    mutable bool d_windowPtrSynched = true;
    // whether the image is marked as selected.
    bool d_selected = false;
};

}

#endif
