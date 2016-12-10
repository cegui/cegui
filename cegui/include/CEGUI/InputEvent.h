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
        UNKNOWN         =0x00,
        ESCAPE          =0x01,
        ONE             =0x02,
        TWO             =0x03,
        THREE           =0x04,
        FOUR            =0x05,
        FIVE            =0x06,
        SIX             =0x07,
        SEVEN           =0x08,
        EIGHT           =0x09,
        NINE            =0x0A,
        ZERO            =0x0B,
        MINUS           =0x0C,  /* - on main keyboard */
        EQUALS			=0x0D,
        BACKSPACE		=0x0E,  /* backspace */
        TAB				=0x0F,
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
        LEFT_BRACKET    =0x1A,
        RIGHT_BRACKET   =0x1B,
        RETURN			=0x1C,  /* Enter on main keyboard */
        LEFT_CONTROL    =0x1D,
        A               =0x1E,
        S               =0x1F,
        D               =0x20,
        F               =0x21,
        G               =0x22,
        H               =0x23,
        J               =0x24,
        K               =0x25,
        L               =0x26,
        SEMICOLON       =0x27,
        APOSTROPHE		=0x28,
        GRAVE           =0x29,  /* accent grave */
        LEFT_SHIFT      =0x2A,
        BACKSLASH       =0x2B,
        Z               =0x2C,
        X               =0x2D,
        C               =0x2E,
        V               =0x2F,
        B               =0x30,
        N               =0x31,
        M               =0x32,
        COMMA           =0x33,
        PERIOD          =0x34,  /* . on main keyboard */
        SLASH           =0x35,  /* '/' on main keyboard */
        RIGHT_SHIFT     =0x36,
        MULTIPLY        =0x37,  /* * on numeric keypad */
        LEFT_ALT        =0x38,  /* left Alt */
        SPACE           =0x39,
        CAPITAL         =0x3A,
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
        NUM_LOCK        =0x45,
        SCROLL_LOCK     =0x46,  /* Scroll Lock */
        NUMPAD_7        =0x47,
        NUMPAD_8        =0x48,
        NUMPAD_9        =0x49,
        SUBTRACT        =0x4A,  /* - on numeric keypad */
        NUMPAD_4        =0x4B,
        NUMPAD_5        =0x4C,
        NUMPAD_6        =0x4D,
        ADD				=0x4E,  /* + on numeric keypad */
        NUMPAD_1        =0x4F,
        NUMPAD_2        =0x50,
        NUMPAD_3        =0x51,
        NUMPAD_0        =0x52,
        DECIMAL			=0x53,  /* . on numeric keypad */
        OEM_102         =0x56,  /* < > | on UK/Germany keyboards */
        F11             =0x57,
        F12             =0x58,
        F13             =0x64,  /*                     (NEC PC98) */
        F14             =0x65,  /*                     (NEC PC98) */
        F15             =0x66,  /*                     (NEC PC98) */
        KANA            =0x70,  /* (Japanese keyboard)            */
        ABNT_C1         =0x73,  /* / ? on Portugese (Brazilian) keyboards */
        CONVERT         =0x79,  /* (Japanese keyboard)            */
        NO_CONVERT      =0x7B,  /* (Japanese keyboard)            */
        YEN             =0x7D,  /* (Japanese keyboard)            */
        ABNT_C2         =0x7E,  /* Numpad . on Portugese (Brazilian) keyboards */
        NUMPAD_EQUALS   =0x8D,  /* = on numeric keypad (NEC PC98) */
        PREV_TRACK      =0x90,  /* Previous Track (KC_CIRCUMFLEX on Japanese keyboard) */
        AT              =0x91,  /*                     (NEC PC98) */
        COLON           =0x92,  /*                     (NEC PC98) */
        UNDERLINE       =0x93,  /*                     (NEC PC98) */
        KANJI           =0x94,  /* (Japanese keyboard)            */
        STOP            =0x95,  /*                     (NEC PC98) */
        AX              =0x96,  /*                     (Japan AX) */
        UNLABELED       =0x97,  /*                        (J3100) */
        NEXT_TRACK      =0x99,  /* Next Track */
        NUMPAD_ENTER    =0x9C,  /* Enter on numeric keypad */
        RIGHT_CONTROL   =0x9D,
        MUTE            =0xA0,  /* Mute */
        CALCULATOR      =0xA1,  /* Calculator */
        PLAY_PAUSE      =0xA2,  /* Play / Pause */
        MEDIA_STOP      =0xA4,  /* Media Stop */
        VOLUME_DOWN     =0xAE,  /* Volume - */
        VOLUME_UP       =0xB0,  /* Volume + */
        WEB_HOME        =0xB2,  /* Web home */
        NUMPAD_COMMA    =0xB3,  /* , on numeric keypad (NEC PC98) */
        DIVIDE          =0xB5,  /* / on numeric keypad */
        SYS_RQ          =0xB7,
        RIGHT_ALT       =0xB8,  /* right Alt */
        PAUSE           =0xC5,  /* Pause */
        HOME            =0xC7,  /* Home on arrow keypad */
        ARROW_UP        =0xC8,  /* UpArrow on arrow keypad */
        PAGE_UP         =0xC9,  /* PgUp on arrow keypad */
        ARROW_LEFT      =0xCB,  /* LeftArrow on arrow keypad */
        ARROW_RIGHT     =0xCD,  /* RightArrow on arrow keypad */
        END             =0xCF,  /* End on arrow keypad */
        ARROW_DOWN      =0xD0,  /* DownArrow on arrow keypad */
        PAGE_DOWN		=0xD1,  /* PgDn on arrow keypad */
        INSERT          =0xD2,  /* Insert on arrow keypad */
        KEY_DELETE      =0xD3,  /* Delete on arrow keypad */
        LEFT_WINDOWS    =0xDB,  /* Left Windows key */
        RIGHT_WINDOWS   =0xDC,  /* Right Windows key - Correct spelling :) */
        APP_MENU        =0xDD,  /* AppMenu key */
        POWER           =0xDE,  /* System Power */
        SLEEP           =0xDF,  /* System Sleep */
        WAKE			=0xE3,  /* System Wake */
        WEB_SEARCH		=0xE5,  /* Web Search */
        WEB_FAVORITES	=0xE6,  /* Web Favorites */
        WEB_REFRESH		=0xE7,  /* Web Refresh */
        WEB_STOP        =0xE8,  /* Web Stop */
        WEB_FORWARD		=0xE9,  /* Web Forward */
        WEB_BACK        =0xEA,  /* Web Back */
        MY_COMPUTER		=0xEB,  /* My Computer */
        MAIL			=0xEC,  /* Mail */
        MEDIA_SELECT    =0xED   /* Media Select */
    };

};

/*!
\brief
    Enumeration of mouse buttons
*/
enum class MouseButton : int
{
    //! The left mouse button.
    LEFT_BUTTON,
    //! The right mouse button.
    RIGHT_BUTTON,
    //! The middle mouse button.
    MIDDLE_BUTTON,
    //! The first 'extra' mouse button.
    X1_BUTTON,
    //! The second 'extra' mouse button.
    X2_BUTTON,
    //! Value that equals the number of mouse buttons supported by CEGUI.
    COUNT,
    //! Value set for no mouse button.  NB: This is not 0, do not assume!
    NONE
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
    SemanticEventArgs(Window* wnd) : WindowEventArgs(wnd), d_semanticValue(-1) {}

    int d_semanticValue;            //!< The type of the semantic value
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
