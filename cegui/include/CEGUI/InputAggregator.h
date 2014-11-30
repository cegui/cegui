/***********************************************************************
    created:    10/7/2013
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Generates input events from injected input
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIInputAggregator_h_
#define _CEGUIInputAggregator_h_

#include "CEGUI/Base.h"
#include "CEGUI/Event.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/InjectedInputReceiver.h"
#include "CEGUI/InputEventReceiver.h"
#include "CEGUI/SemanticInputEvent.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
struct MouseClickTracker;

//! EventArgs class passed to subscribers for (most) InputAggregator events.
class CEGUIEXPORT InputAggregatorEventArgs : public EventArgs
{
public:
    InputAggregatorEventArgs(InputAggregator* aggregator):
        d_aggregator(aggregator)
    {}

    //! pointer to the InputAggregator that triggered the event.
    InputAggregator* d_aggregator;
};

/*!
\brief
    Aggregates the input from multiple input devices and processes it to generate
    input events which are then fed to the (optional) \ref InputEventReceiver.
*/
class CEGUIEXPORT InputAggregator : public InjectedInputReceiver,
                                    public EventSet
{
public:
    static const float DefaultMouseButtonClickTimeout;
    static const float DefaultMouseButtonMultiClickTimeout;
    static const Sizef DefaultMouseButtonMultiClickTolerance;

    /** Name of Event fired when the mouse click timeout is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseButtonClickTimeoutChanged;
    /** Name of Event fired when the mouse multi-click timeout is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseButtonMultiClickTimeoutChanged;
    /** Name of Event fired when the mouse multi-click movement tolerance area
     * size is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseButtonMultiClickToleranceChanged;
    /** Name of Event fired when the mouse movement scaling factor is changed.
     * Handlers are passed a const reference to a GUIContextEventArgs struct.
     */
    static const String EventMouseMoveScalingFactorChanged;

    InputAggregator(InputEventReceiver* input_receiver);
    virtual ~InputAggregator();

    /*!
    \brief
        Initialises this InputAggregator with some default simple-key mappings
    */
    virtual void initialise();

    /*!
    \brief
        Set whether automatic mouse button click and multi-click (i.e.
        double-click and treble-click) event generation will occur.

    \param enable
        - true to have mouse button click and multi-click events automatically
        generated by the system from the basic button up and down event
        injections.
        - false if no automatic generation of events should occur.  In this
        instance the user may wish to use the additional event injectors to
        manually inform the system of such events.
    */
    void setMouseClickEventGenerationEnabled(const bool enable);

    /*!
    \brief
        Return whether automatic mouse button click and multi-click (i.e.
        double-click and treble-click) event generation is enabled.

    \return
        - true if mouse button click and multi-click events will be
        automatically generated by the system from the basic button up and down
        event injections.
        - false if no automatic generation of events will occur.  In this
        instance the user may wish to use the additional event injectors to
        manually inform the system of such events.
    */
    bool isMouseClickEventGenerationEnabled() const;

    void setMouseButtonClickTimeout(float seconds);
    float getMouseButtonClickTimeout() const;

    void setMouseButtonMultiClickTimeout(float seconds);
    float getMouseButtonMultiClickTimeout() const;

    /*!
    \brief
        Sets the mouse multi-click tolerance size

    \param sz
        The size of the tolerance in percent of the display's size
    */
    void setMouseButtonMultiClickTolerance(const Sizef& sz);

    /*!
    \brief
        Returns the mouse multi-click tolerance size

    \return
        A size structure with the zone's width and height in percent of the
        display's size
    */
    const Sizef& getMouseButtonMultiClickTolerance() const;

    void setMouseMoveScalingFactor(float factor);
    float getMouseMoveScalingFactor() const;

    /*!
    \brief
        Passes input immediately if it is some predefined action
    \return
        True when the input was consumed
    */
    bool injectRawKeyDown(Key::Scan scan_code, bool shift_down, bool alt_down,
        bool ctrl_down) const;
    
    /************************************************************************/
    /* InjectedInputReceiver interface implementation                       */
    /************************************************************************/
    virtual bool injectMouseMove(float delta_x, float delta_y);
    virtual bool injectMouseLeaves();

    virtual bool injectMouseButtonDown(MouseButton button);
    virtual bool injectMouseButtonUp(MouseButton button);

    /*!
      \return
      Always true
    */
    virtual bool injectKeyDown(Key::Scan scan_code);
    virtual bool injectKeyUp(Key::Scan scan_code);

    virtual bool injectChar(String::value_type code_point);
    virtual bool injectMouseWheelChange(float delta);
    virtual bool injectMousePosition(float x_pos, float y_pos);

    virtual bool injectMouseButtonClick(const MouseButton button);
    virtual bool injectMouseButtonDoubleClick(const MouseButton button);
    virtual bool injectMouseButtonTripleClick(const MouseButton button);

    virtual bool injectCopyRequest();
    virtual bool injectCutRequest();
    virtual bool injectPasteRequest();

protected:
    virtual void onMouseButtonClickTimeoutChanged(InputAggregatorEventArgs& args);
    virtual void onMouseButtonMultiClickTimeoutChanged(InputAggregatorEventArgs& args);
    virtual void onMouseButtonMultiClickToleranceChanged(InputAggregatorEventArgs& args);
    virtual void onMouseMoveScalingFactorChanged(InputAggregatorEventArgs& args);

    virtual bool isControlPressed();
    virtual bool isAltPressed();
    virtual bool isShiftPressed();

    void recomputeMultiClickAbsoluteTolerance();
    virtual bool onDisplaySizeChanged(const EventArgs& args);

    Event::Connection d_displaySizeChangedConnection;

    InputEventReceiver* d_inputReceiver;

    //! Timeout used to when detecting a single-click.
    float d_mouseButtonClickTimeout;
    //! Timeout used when detecting multi-click events.
    float d_mouseButtonMultiClickTimeout;
    //! Movement tolerance (percent) used when detecting multi-click events.
    Sizef d_mouseButtonMultiClickTolerance;
    //! Movement tolerance (absolute) used when detecting multi-click events.
    Sizef d_mouseButtonMultiClickAbsoluteTolerance;
    //! should mouse click/multi-click events be automatically generated.
    bool d_generateMouseClickEvents;
    MouseClickTracker* d_mouseClickTrackers;

    //! Scaling factor applied to injected cursor move deltas.
    float d_mouseMovementScalingFactor;

    glm::vec2 d_pointerPosition;
    int d_keyValuesMappings[0xFF]; //!< Mapping from a key to its semantic
    bool d_keysPressed[0xFF];
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIInputEvents_h_
