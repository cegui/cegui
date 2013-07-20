/***********************************************************************
    filename:   InputAggregator.h
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

/*!
\brief
    Aggregates the input from multiple input devices and processes it to generate
    input events which are then fed to the (optional) \ref InputEventReceiver.
*/
class CEGUIEXPORT InputAggregator : public InjectedInputReceiver
{
public:
    InputAggregator(InputEventReceiver* input_receiver);

    virtual bool injectTimePulse(float timeElapsed);

    virtual bool injectMouseMove(float delta_x, float delta_y);
    virtual bool injectMouseLeaves();

    virtual bool injectMouseButtonDown(MouseButton button);
    virtual bool injectMouseButtonUp(MouseButton button);

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

private:
    void initializeSimpleKeyMappings();

    bool isControlPressed();
    bool isAltPressed();
    bool isShiftPressed();

    InputEventReceiver* d_inputReceiver;

    Vector2f d_pointerPosition;
    SemanticValue d_keyValuesMappings[0xFF]; //!< Mapping from a key to its semantic
    bool d_keysPressed[0xFF];
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIInputEvents_h_
