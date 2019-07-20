/***********************************************************************
	created:	30/5/2004
	author:		Paul D Turner

	purpose:	Defines interface to input event classes
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
#ifndef _CEGUIInputEvent_h_
#define _CEGUIInputEvent_h_

#include "CEGUI/Base.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/String.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/SemanticInputEvent.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	struct to give scope to scan code enumeration.
*/
struct CEGUIEXPORT Key
{
	// enumeration of scan codes.  Customised from table taken from Ogre, which took them from DirectInput...
	enum class Scan : unsigned char
    {
        Unknown         =0x00,
        Esc             =0x01,
        One             =0x02,
        Two             =0x03,
        Three           =0x04,
        Four            =0x05,
        Five            =0x06,
        Six             =0x07,
        Seven           =0x08,
        Eight           =0x09,
        Nine            =0x0A,
        Zero            =0x0B,
        Minus           =0x0C,  /* - on main keyboard */
        Equals			=0x0D,
        Backspace		=0x0E,  /* backspace */
        Tab				=0x0F,
        Q               =0x10,
        W               =0x11,
        E               =0x12,
        R               =0x13,
        T               =0x14,
        Y               =0x15,
        U               =0x16,
        I               =0x17,
        O               =0x18,
        P               =0x19,
        LeftBracket     =0x1A,
        RightBracket    =0x1B,
        Return			=0x1C,  /* Enter on main keyboard */
        LeftControl     =0x1D,
        A               =0x1E,
        S               =0x1F,
        D               =0x20,
        F               =0x21,
        G               =0x22,
        H               =0x23,
        J               =0x24,
        K               =0x25,
        L               =0x26,
        Semicolon       =0x27,
        Apostrophe		=0x28,
        Grave           =0x29,  /* accent grave */
        LeftShift       =0x2A,
        Backslash       =0x2B,
        Z               =0x2C,
        X               =0x2D,
        C               =0x2E,
        V               =0x2F,
        B               =0x30,
        N               =0x31,
        M               =0x32,
        Comma           =0x33,
        Period          =0x34,  /* . on main keyboard */
        ForwardSlash    =0x35,  /* '/' on main keyboard */
        RightShift      =0x36,
        Multiply        =0x37,  /* * on numeric keypad */
        LeftAlt         =0x38,  /* left Alt */
        Space           =0x39,
        Capital         =0x3A,
        F1              =0x3B,
        F2              =0x3C,
        F3              =0x3D,
        F4              =0x3E,
        F5              =0x3F,
        F6              =0x40,
        F7              =0x41,
        F8              =0x42,
        F9              =0x43,
        F10             =0x44,
        NumLock         =0x45,
        ScrollLock      =0x46,  /* Scroll Lock */
        Numpad_7        =0x47,
        Numpad_8        =0x48,
        Numpad_9        =0x49,
        Subtract        =0x4A,  /* - on numeric keypad */
        Numpad_4        =0x4B,
        Numpad_5        =0x4C,
        Numpad_6        =0x4D,
        Add				=0x4E,  /* + on numeric keypad */
        Numpad_1        =0x4F,
        Numpad_2        =0x50,
        Numpad_3        =0x51,
        Numpad_0        =0x52,
        Decimal			=0x53,  /* . on numeric keypad */
        Oem102          =0x56,  /* < > | on UK/Germany keyboards */
        F11             =0x57,
        F12             =0x58,
        F13             =0x64,  /*                     (NEC PC98) */
        F14             =0x65,  /*                     (NEC PC98) */
        F15             =0x66,  /*                     (NEC PC98) */
        Kana            =0x70,  /* (Japanese keyboard)            */
        AbntC1          =0x73,  /* / ? on Portugese (Brazilian) keyboards */
        Convert         =0x79,  /* (Japanese keyboard)            */
        NoConvert       =0x7B,  /* (Japanese keyboard)            */
        Yen             =0x7D,  /* (Japanese keyboard)            */
        AbntC2          =0x7E,  /* Numpad . on Portugese (Brazilian) keyboards */
        NumpadEquals    =0x8D,  /* = on numeric keypad (NEC PC98) */
        PrevTrack       =0x90,  /* Previous Track (KC_CIRCUMFLEX on Japanese keyboard) */
        At              =0x91,  /*                     (NEC PC98) */
        Colon           =0x92,  /*                     (NEC PC98) */
        Underline       =0x93,  /*                     (NEC PC98) */
        Kanji           =0x94,  /* (Japanese keyboard)            */
        Stop            =0x95,  /*                     (NEC PC98) */
        Ax              =0x96,  /*                     (Japan AX) */
        Unlabeled       =0x97,  /*                        (J3100) */
        NextTrack       =0x99,  /* Next Track */
        NumpadEnter     =0x9C,  /* Enter on numeric keypad */
        RightControl    =0x9D,
        Mute            =0xA0,  /* Mute */
        Calculator      =0xA1,  /* Calculator */
        PlayPause       =0xA2,  /* Play / Pause */
        MediaStop       =0xA4,  /* Media Stop */
        VolumeDown      =0xAE,  /* Volume - */
        VolumeUp        =0xB0,  /* Volume + */
        WebHome         =0xB2,  /* Web home */
        NumpadComma     =0xB3,  /* , on numeric keypad (NEC PC98) */
        Divide          =0xB5,  /* / on numeric keypad */
        SysRq           =0xB7,
        RightAlt        =0xB8,  /* right Alt */
        Pause           =0xC5,  /* Pause */
        Home            =0xC7,  /* Home on arrow keypad */
        ArrowUp         =0xC8,  /* UpArrow on arrow keypad */
        PageUp          =0xC9,  /* PgUp on arrow keypad */
        ArrowLeft       =0xCB,  /* LeftArrow on arrow keypad */
        ArrowRight      =0xCD,  /* RightArrow on arrow keypad */
        End             =0xCF,  /* End on arrow keypad */
        ArrowDown       =0xD0,  /* DownArrow on arrow keypad */
        PageDown		=0xD1,  /* PgDn on arrow keypad */
        Insert          =0xD2,  /* Insert on arrow keypad */
        DeleteKey       =0xD3,  /* Delete on arrow keypad */
        LeftWindows     =0xDB,  /* Left Windows key */
        RightWindows    =0xDC,  /* Right Windows key - Correct spelling :) */
        AppMenu         =0xDD,  /* AppMenu key */
        Power           =0xDE,  /* System Power */
        Sleep           =0xDF,  /* System Sleep */
        Wake			=0xE3,  /* System Wake */
        WebSearch		=0xE5,  /* Web Search */
        WebFavorites	=0xE6,  /* Web Favorites */
        WebRefresh		=0xE7,  /* Web Refresh */
        WebStop         =0xE8,  /* Web Stop */
        WebForward		=0xE9,  /* Web Forward */
        WebBack         =0xEA,  /* Web Back */
        MyComputer		=0xEB,  /* My Computer */
        Mail            =0xEC,  /* Mail */
        MediaSelect     =0xED   /* Media Select */
    };

};

/*!
\brief
    Enumeration of mouse buttons
*/
enum class MouseButton : int
{
    //! The left mouse button.
    Left,
    //! The right mouse button.
    Right,
    //! The middle mouse button.
    Middle,
    //! The first 'extra' mouse button.
    X1,
    //! The second 'extra' mouse button.
    X2,
    //! Value that equals the number of mouse buttons supported by CEGUI.
    Count,
    //! Value set for no mouse button.  NB: This is not 0, do not assume!
    Invalid
};

/*!
\brief
	EventArgs based class that is used for objects passed to handlers triggered for events
	concerning some Window object.
*/
class CEGUIEXPORT WindowEventArgs : public EventArgs
{
public:
	WindowEventArgs(Window* wnd) : window(wnd) {}

	Window*	window;		//!< pointer to a Window object of relevance to the event.
};

/*!
\brief
    Event arguments used by semantic input event handlers
*/
class CEGUIEXPORT SemanticEventArgs : public WindowEventArgs
{
public:
    SemanticEventArgs(Window* wnd) : WindowEventArgs(wnd), d_semanticValue(SemanticValue::NoValue) {}

    SemanticValue d_semanticValue;  //!< The type of the semantic value
    SemanticPayload d_payload;      //!< The payload of the event
};

/*!
\brief
	WindowEventArgs class that is primarily used by lua scripts
*/
class CEGUIEXPORT UpdateEventArgs : public WindowEventArgs
{
public:
    UpdateEventArgs(Window* window, float tslf) :
        WindowEventArgs(window),
        d_timeSinceLastFrame(tslf)
    {}

	float d_timeSinceLastFrame; //!< Time since the last frame update
};

/*!
\brief
    EventArgs based class that is used for objects passed to input event handlers
    concerning cursor input.
*/
class CEGUIEXPORT CursorInputEventArgs : public WindowEventArgs
{
public:
    CursorInputEventArgs(Window* wnd) : WindowEventArgs(wnd) {}

    //!< holds current cursor position.
    glm::vec2 position;
    //!< holds variation of cursor position from last cursor input
    glm::vec2 moveDelta;
    // one of the CursorInputSource enumerated values describing the source causing the event
    CursorInputSource source;
    // holds the amount of the scroll
    float scroll;

    // current state (hold: true/false) of cursors sources. Addressable by members of \ref CursorInputSource
    CursorsState state;
};

/*!
\brief
    EventArgs based class that is used for objects passed to input event handlers
    concerning cursor events.
*/
class CEGUIEXPORT CursorEventArgs : public EventArgs
{
public:
    CursorEventArgs(Cursor* cursor) : d_cursor(cursor) {}

    Cursor* d_cursor;  //!< pointer to a Cursor object of relevance to the event.
    const Image* d_image; //!< pointer to an Image object of relevance to the event.
};

/*!
\brief
	EventArgs based class that is used for objects passed to input event handlers
	concerning text input.
*/
class CEGUIEXPORT TextEventArgs : public WindowEventArgs
{
public:
	TextEventArgs(Window* wnd) : WindowEventArgs(wnd) {}

    //! char32_t codepoint representing the character of the text event.
	char32_t d_character; 
};

/*!
\brief
	EventArgs based class that is used for Activated and Deactivated window events
*/
class CEGUIEXPORT ActivationEventArgs : public WindowEventArgs
{
public:
	ActivationEventArgs(Window* wnd) : WindowEventArgs(wnd) {}

	Window*	otherWindow;	//!< Pointer to the other window involved in the activation change.
};

/*!
\brief
	EventArgs based class used for certain drag/drop notifications
*/
class CEGUIEXPORT DragDropEventArgs : public WindowEventArgs
{
public:
	DragDropEventArgs(Window* wnd) : WindowEventArgs(wnd) {}
	DragContainer*	dragDropItem; //!< pointer to the DragContainer window being dragged / dropped.
};

/*!
\brief
    EventArgs based class that is used for notifications regarding the main
    display.
*/
class CEGUIEXPORT DisplayEventArgs : public EventArgs
{
public:
    DisplayEventArgs(const Sizef& sz):
        size(sz)
    {}

    //! current / new size of the display.
    Sizef size;
};

//! EventArgs based class that is used for notifications regarding resources.
class CEGUIEXPORT ResourceEventArgs : public EventArgs
{
public:
    ResourceEventArgs(const String& type, const String& name) :
       resourceType(type),
       resourceName(name)
    {}

    //! String identifying the resource type this notification is about.
    String resourceType;
    //! String identifying the name of the resource this notification is about.
    String resourceName;
};

//! EventArgs based class that is used for notifications regarding Font objects.
class CEGUIEXPORT FontEventArgs : public EventArgs
{
public:
    FontEventArgs(Font* font) :
       font(font)
    {}

    //! Pointer to the font object related to the event notification.
    Font* font;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIInputEvent_h_
