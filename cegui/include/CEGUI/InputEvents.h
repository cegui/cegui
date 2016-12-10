/***********************************************************************
    created:    10/7/2013
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Holds the input events which are used by the Input Aggregator
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
#ifndef _CEGUIInputEvents_h_
#define _CEGUIInputEvents_h_

#include "CEGUI/Base.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/SlotFunctorBase.h"
#include "CEGUI/String.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    The default input events used inside CEGUI
*/
enum class InputEventType : int
{
    TextInputEventType              = 0x0001,   //!< Text was inputted.
    SemanticInputEventType          = 0x0002,   //!< An event with a certain semantic

    UserDefinedInputEventType       = 0x5000,   //!< This marks the beginning of user-defined events.
};

/*!
\brief
    The base class for all input events.
*/
class CEGUIEXPORT InputEvent
{
public:
    InputEvent(InputEventType eventType) : d_eventType(eventType) {}
    virtual ~InputEvent() {}

    //!< The type of the input event
    InputEventType d_eventType;
};

/*!
\brief
    Represents the input of a character
*/
class CEGUIEXPORT TextInputEvent : public InputEvent
{
public:
    TextInputEvent() : InputEvent(InputEventType::TextInputEventType) {}

    //! The input character
    char32_t d_character;         
};

/*!
\brief
    Slot template class that creates a functor that calls back via a class
    member function and send a casted input event subclass as the parameter.

\tparam T
    The type of the class the contains the handler

\tparam TInput
    A subclass of InputEvent or InputEvent itself to cast the input event to
    before calling the functor
*/
template<typename T, typename TInput>
class InputEventHandlerSlot : public SlotFunctorBase<InputEvent>
{
public:
    //! Member function slot type.
    typedef bool(T::*MemberFunctionType)(const TInput&);

    InputEventHandlerSlot(MemberFunctionType func, T* obj) :
        d_function(func),
        d_object(obj)
    {}

    bool operator()(const InputEvent& arg) override
    {
        return (d_object->*d_function)(static_cast<const TInput&>(arg));
    }

private:
    MemberFunctionType d_function;
    T* d_object;
};

} // End of  CEGUI namespace section

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIInputEvents_h_
