/***********************************************************************
    created:    21/2/2005
    author:     Paul D Turner
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
#ifndef _CEGUITooltip_h_
#define _CEGUITooltip_h_

#include "../Window.h"
#include "../WindowRenderer.h"

namespace CEGUI
{

//! \brief Base class for Tooltip window renderer objects.
class CEGUIEXPORT TooltipWindowRenderer : public WindowRenderer
{
public:

    TooltipWindowRenderer(const String& name);

    /*!
    \brief
        Return the size of the area that will be occupied by the tooltip text, given
        any current formatting options.

    \return
        Size object describing the size of the rendered tooltip text in pixels.
    */
    virtual Sizef getTextSize() const = 0;
};

/*!
\brief
    Base class for Tooltip widgets.

    The Tooltip class shows a simple pop-up window around the cursor position
    with some text information.  The tool-tip fades in when the user hovers
    with the cursor over a window which has tool-tip text set, and then fades
    out after some pre-set time.

\note
    For Tooltip to work properly, you must specify a default tool-tip widget
    type via System::setTooltip, or by setting a custom tool-tip object for
    your Window(s).  Additionally, you need to ensure that time pulses are
    properly passed to the system via System::injectTimePulse.
*/
class CEGUIEXPORT Tooltip : public Window
{
public:

    static const String WidgetTypeName;                 //!< Window factory name
    static const String EventNamespace;                 //!< Namespace for global events

    /** Event fired when the hover timeout for the tool tip gets changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip whose hover timeout has
        * been changed.
        */
    static const String EventHoverTimeChanged;
    /** Event fired when the display timeout for the tool tip gets changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip whose display timeout has
        * been changed.
        */
    static const String EventDisplayTimeChanged;
    /** Event fired when the fade timeout for the tooltip gets changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip whose fade timeout has
        * been changed.
        */
    static const String EventFadeTimeChanged;
    /** Event fired when the tooltip is about to get activated.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip that is about to become
        * active.
        */
    static const String EventTooltipActive;
    /** Event fired when the tooltip has been deactivated.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip that has become inactive.
        */
    static const String EventTooltipInactive;
    /** Event fired when the tooltip changes target window but stays active.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the Tooltip that has transitioned.
        */
    static const String EventTooltipTransition;

    Tooltip(const String& type, const String& name);

    /*!
    \brief
        Sets the target window for the tooltip.  This used internally to manage tooltips, you
        should not have to call this yourself.

    \param wnd
        Window object that the tooltip should be associated with (for now).
    */
    void setTargetWindow(Window* wnd);

    /*!
    \brief
        return the current target window for this Tooltip.

    \return
        Pointer to the target window for this Tooltip or 0 for none.
    */
    const Window* getTargetWindow() { return d_target; }

    /*!
    \brief
        Resets the timer on the tooltip when in the Active / Inactive states.  This is used internally
        to control the tooltip, it is not normally necessary to call this method yourself.
    */
    void resetTimer() { d_elapsed = 0.f; }

    /*!
    \brief
        Return the number of seconds the cursor should hover stationary
        over the target window before the tooltip gets activated.

    \return
        float value representing a number of seconds.
    */
    float getHoverTime() const { return d_hoverTime; }

    /*!
    \brief
        Set the number of seconds the tooltip should be displayed for before it automatically
        de-activates itself. 0 indicates that the tooltip should never timeout and auto-deactivate.

    \param seconds
        float value representing a number of seconds.
    */
    void setDisplayTime(float seconds);

    /*!
    \brief
        Set the number of seconds the cursor should hover stationary over
        the target window before the tooltip gets activated.

    \param seconds
        float value representing a number of seconds.
    */
    void setHoverTime(float seconds);

    /*!
    \brief
        Return the number of seconds the tooltip should be displayed for before it automatically
        de-activates itself.  0 indicates that the tooltip never timeout and auto-deactivates.

    \return
        float value representing a number of seconds.
    */
    float getDisplayTime() const { return d_displayTime; }

    //! \brief Causes the tooltip to position itself appropriately.
    void positionSelf();

    //! \brief Causes the tooltip to resize itself appropriately.
    void sizeSelf();

    /*!
    \brief
        Return the size of the area that will be occupied by the tooltip text, given
        any current formatting options.

    \return
        Size object describing the size of the rendered tooltip text in pixels.
    */
    Sizef getTextSize() const;

    /*!
    \brief
        Return the size of the area that will be occupied by the tooltip text, given
        any current formatting options.

    \return
        Size object describing the size of the rendered tooltip text in pixels.
    */
    virtual Sizef getTextSize_impl() const { return getRenderedString().getPixelSize(this); }

protected:

    //! \brief Event trigger method called when the hover timeout gets changed.
    virtual void onHoverTimeChanged(WindowEventArgs& e);
    //! \brief Event trigger method called when the display timeout gets changed.
    virtual void onDisplayTimeChanged(WindowEventArgs& e);
    //! \brief Event trigger method called just before the tooltip becomes active.
    virtual void onTooltipActive(WindowEventArgs& e);
    //! \brief Event trigger method called just after the tooltip is deactivated.
    virtual void onTooltipInactive(WindowEventArgs& e);
    //! \brief Event trigger method called just after the tooltip changed target window but remained active.
    virtual void onTooltipTransition(WindowEventArgs& e);

    bool validateWindowRenderer(const WindowRenderer* renderer) const override;
    void updateSelf(float elapsed) override;
    void onHidden(WindowEventArgs& e) override;
    void onCursorEnters(CursorInputEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;

    void switchToInactiveState();
    void switchToActiveState();

    const Window* d_target = nullptr; //!< Current target Window for this Tooltip.
    float d_elapsed = 0.f;            //!< Used to track state change timings
    float d_hoverTime = 0.4f;         //!< tool-tip hover time (seconds cursor must stay stationary before tip shows).
    float d_displayTime = 7.5f;       //!< tool-tip display time (seconds that tip is shown for).
    bool d_active = false;            //!< true if the tooltip is active   
    bool d_inPositionSelf = false;

private:

    void addTooltipProperties();
};

}

#endif
