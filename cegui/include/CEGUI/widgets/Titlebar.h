/***********************************************************************
    created:    25/4/2004
    author:     Paul D Turner

    purpose:    Interface for a Titlebar Widget
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#pragma once
#include "../Window.h"

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! \brief Class representing the title bar for FrameWindow and its subclasses.
class CEGUIEXPORT Titlebar : public Window
{
public:
    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name

    Titlebar(const String& type, const String& name);

    /*!
    \brief
        Return whether this title bar will respond to dragging.

    \return
        true if the title bar will respond to dragging, false if the title bar will not respond.
    */
    bool isDraggingEnabled() const { return d_dragEnabled; }

    /*!
    \brief
        Set whether this title bar widget will respond to dragging.

    \param setting
        true if the title bar should respond to being dragged, false if it should not respond.
    */
    void setDraggingEnabled(bool setting);

    //! Checks whether the title bar widget is being dragged at the moment
    bool isDragged() const { return d_dragging; }

    //! Gets the point at which the title bar widget is/was being dragged
    const glm::vec2& getDragPoint() const { return d_dragPoint; }

protected:

    void onCursorMove(CursorMoveEventArgs& e) override;
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onMouseButtonUp(MouseButtonEventArgs& e) override;
    void onDoubleClick(MouseButtonEventArgs& e) override;
    void onCaptureLost(WindowEventArgs& e) override;
    void onFontChanged(WindowEventArgs &e) override;

    /*!
    \brief
        Event handler called when the 'draggable' state for the title bar is changed.

        Note that this is for 'internal' use at the moment and as such does not add or
        fire a public Event that can be subscribed to.
    */
    virtual void onDraggingEnabledChanged(WindowEventArgs&) {}

    URect d_oldCursorArea;     //!< Used to backup cursor restraint area.
    glm::vec2 d_dragPoint;     //!< Point at which we are being dragged.
    bool d_dragging = false;   //!< set to true when the window is being dragged.
    bool d_dragEnabled = true; //!< true when dragging for the widget is enabled.

private:

    void addTitlebarProperties();
};

}

#if defined(_MSC_VER)
#    pragma warning(pop)
#endif
