/***********************************************************************
    filename:   CEGUIRenderedStringWidgetComponent.h
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

#include "CEGUIRenderedStringComponent.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! String component that moves a widget to appear as part of the string.
class CEGUIEXPORT RenderedStringWidgetComponent : public RenderedStringComponent
{
public:
    //! Constructor
    RenderedStringWidgetComponent();
    RenderedStringWidgetComponent(const String& widget_name);
    RenderedStringWidgetComponent(Window* widget);

    //! Set the window to be controlled by this component.
    void setWindow(const String& widget_name);
    //! Set the window to be controlled by this component.
    void setWindow(Window* widget);
    //! return the window currently controlled by this component
    const Window* getWindow() const;

    // implementation of abstract base interface
    void draw(GeometryBuffer& buffer, const Vector2& position,
              const ColourRect* mod_colours, const Rect* clip_rect,
              const float vertical_space, const float space_extra) const;
    Size getPixelSize() const;
    bool canSplit() const;
    RenderedStringWidgetComponent* split(float split_point, bool first_component);
    RenderedStringWidgetComponent* clone() const;
    size_t getSpaceCount() const;

protected:
    //! pointer to the window controlled by this component.
    Window* d_window;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIRenderedStringWidgetComponent_h_
