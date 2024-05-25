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
#pragma once

#include "CEGUI/EventArgs.h"
#include "CEGUI/Sizef.h"
#include <type_traits>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    Helper class for modern C++ enum flags.
    Use needShift if your enum values are zero-based indices (0, 1, 2, 3, 4 ...).
    Leave needShift disabled if your enum values are already single bit masks (0x0, 0x1, 0x2, 0x4, 0x8 ...).
*/

template<typename T, bool needShift = false, typename mask_t = std::underlying_type_t<T>
#ifndef SWIG
, class = typename std::enable_if_t<std::is_enum<T>::value>
#endif
>
struct CEGUIEXPORT Flags
{
    static inline constexpr mask_t bit(T flag)
    {
        // TODO: C++17
        if /*constexpr*/ (needShift)
            return (1 << static_cast<mask_t>(flag));
        else
            return static_cast<mask_t>(flag);
    }

    constexpr Flags() = default;
    constexpr Flags(T flag) : d_mask(bit(flag)) {}
    constexpr Flags(mask_t mask) : d_mask(mask) {}
    constexpr Flags(std::initializer_list<T> list)
    {
        d_mask = 0;
        for (T flag : list)
            d_mask |= bit(flag);
    }

    // TODO: C++17, and need to require all Args to be of type T!
    //template<typename... Args>
    //Flags(Args&&... args) : d_mask((args | ...)) {}

    void reset() { d_mask = 0; }
    mask_t getMask() const { return d_mask; }
    bool has(T flag) const { return d_mask & bit(flag); }
    bool hasAny(Flags mask) const { return d_mask & mask.d_mask; }
    bool hasAll(Flags mask) const { return (d_mask & mask.d_mask) == mask.d_mask; }

    operator bool() const { return d_mask != 0; }
    operator mask_t() const { return d_mask; }

    bool operator ==(Flags other) const { return d_mask == other.d_mask; }
    bool operator !=(Flags other) const { return !(*this == other); }
    bool operator &(T flag) const { return has(flag); }
    Flags operator &(Flags other) const { return Flags(d_mask & other.d_mask); }
    Flags operator |(T flag) const { return Flags(d_mask | bit(flag)); }
    Flags operator |(Flags other) const { return Flags(d_mask | other.d_mask); }
    Flags operator +(T flag) const { return Flags(d_mask | bit(flag)); }
    Flags& operator +=(T flag) { d_mask |= bit(flag); return *this; }
    Flags operator -(T flag) const { return Flags(d_mask & ~bit(flag)); }
    Flags& operator -=(T flag) { d_mask &= ~bit(flag); return *this; }
    Flags operator +(Flags other) const { return Flags(d_mask | other.d_mask); }
    Flags& operator +=(Flags other) { d_mask |= other.d_mask; return *this; }
    Flags operator -(Flags other) const { return Flags(d_mask & ~other.d_mask); }
    Flags& operator -=(Flags other) { d_mask &= ~other.d_mask; return *this; }

    mask_t d_mask = 0;
};

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
    Left = 0,
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
using MouseButtons = Flags<MouseButton, true>;

//! \brief Enumeration of keyboard modifier keys
enum class ModifierKey : int
{
    None       = 0x00,
    LeftShift  = 0x01,
    LeftCtrl   = 0x02,
    LeftAlt    = 0x04,
    RightShift = 0x08,
    RightCtrl  = 0x10,
    RightAlt   = 0x20
};

//! \brief Helper class for holding and checking modifier key pressed state
struct CEGUIEXPORT ModifierKeys : public Flags<ModifierKey>
{
    // TODO: C++17
    //static inline constexpr ModifierKeys Shift{ModifierKey::LeftShift | ModifierKey::RightShift}; etc
    // return hasAny(ModifierKeys::Shift);
    static ModifierKeys Shift() { return { ModifierKey::LeftShift, ModifierKey::RightShift }; }
    static ModifierKeys Ctrl() { return { ModifierKey::LeftCtrl, ModifierKey::RightCtrl }; }
    static ModifierKeys Alt() { return { ModifierKey::LeftAlt, ModifierKey::RightAlt }; }

    using Flags<ModifierKey>::Flags; // Inherit all constructors

    bool hasShift() const { return hasAny(Shift()); }
    bool hasCtrl() const { return hasAny(Ctrl()); }
    bool hasAlt() const { return hasAny(Alt()); }
};

//! \brief A flexible rule that can be matched with ModifierKeys state to check for a certain combination
class CEGUIEXPORT ModifierKeyRule
{
public:

    static ModifierKeyRule None() { return ModifierKeyRule().noShift().noCtrl().noAlt(); }
    static ModifierKeyRule OnlyShift() { return ModifierKeyRule().shift().noCtrl().noAlt(); }
    static ModifierKeyRule OnlyCtrl() { return ModifierKeyRule().noShift().ctrl().noAlt(); }
    static ModifierKeyRule OnlyAlt() { return ModifierKeyRule().noShift().noCtrl().alt(); }
    static ModifierKeyRule OnlyCtrlShift() { return ModifierKeyRule().shift().ctrl().noAlt(); }

    ModifierKeyRule& leftShift() { set(ModifierKey::LeftShift, false); return *this; }
    ModifierKeyRule& rightShift() { set(ModifierKey::RightShift, false); return *this; }
    ModifierKeyRule& shift() { set(ModifierKey::LeftShift, false); set(ModifierKey::RightShift, false); return *this; }
    ModifierKeyRule& noLeftShift() { set(ModifierKey::LeftShift, true); return *this; }
    ModifierKeyRule& noRightShift() { set(ModifierKey::RightShift, true); return *this; }
    ModifierKeyRule& noShift() { set(ModifierKey::LeftShift, true); set(ModifierKey::RightShift, true); return *this; }

    ModifierKeyRule& leftCtrl() { set(ModifierKey::LeftCtrl, false); return *this; }
    ModifierKeyRule& rightCtrl() { set(ModifierKey::RightCtrl, false); return *this; }
    ModifierKeyRule& ctrl() { set(ModifierKey::LeftCtrl, false); set(ModifierKey::RightCtrl, false); return *this; }
    ModifierKeyRule& noLeftCtrl() { set(ModifierKey::LeftCtrl, true); return *this; }
    ModifierKeyRule& noRightCtrl() { set(ModifierKey::RightCtrl, true); return *this; }
    ModifierKeyRule& noCtrl() { set(ModifierKey::LeftCtrl, true); set(ModifierKey::RightCtrl, true); return *this; }

    ModifierKeyRule& leftAlt() { set(ModifierKey::LeftAlt, false); return *this; }
    ModifierKeyRule& rightAlt() { set(ModifierKey::RightAlt, false); return *this; }
    ModifierKeyRule& alt() { set(ModifierKey::LeftAlt, false); set(ModifierKey::RightAlt, false); return *this; }
    ModifierKeyRule& noLeftAlt() { set(ModifierKey::LeftAlt, true); return *this; }
    ModifierKeyRule& noRightAlt() { set(ModifierKey::RightAlt, true); return *this; }
    ModifierKeyRule& noAlt() { set(ModifierKey::LeftAlt, true); set(ModifierKey::RightAlt, true); return *this; }

    bool match(ModifierKeys keys) const
    {
        // Set 1 to every bit where the rule was violated - xor to find differences and mask out insignificant ones
        ModifierKeys violations((keys.getMask() ^ d_set.getMask()) & d_map.getMask());

        // Handle the fact that 'key' rule without specifying left or right means 'any', not 'all' like 'noKey' does
        if (violations)
        {
            if (d_set.hasAll(ModifierKeys::Shift()) && !violations.hasAll(ModifierKeys::Shift()))
                violations -= ModifierKeys::Shift();
            if (d_set.hasAll(ModifierKeys::Ctrl()) && !violations.hasAll(ModifierKeys::Ctrl()))
                violations -= ModifierKeys::Ctrl();
            if (d_set.hasAll(ModifierKeys::Alt()) && !violations.hasAll(ModifierKeys::Alt()))
                violations -= ModifierKeys::Alt();
        }

        return !violations;
    }

protected:

    void set(ModifierKey key, bool deny)
    {
        d_map += key;
        if (deny)
            d_set -= key;
        else
            d_set += key;
    }

    ModifierKeys d_map = 0; // ModifierKey bits that need to be checked
    ModifierKeys d_set = 0; // Whether the corresponding key should be pressed or not
};

static inline ModifierKey ModifierFromScanCode(Key::Scan scanCode)
{
    switch (scanCode)
    {
        case Key::Scan::LeftShift: return ModifierKey::LeftShift;
        case Key::Scan::RightShift: return ModifierKey::RightShift;
        case Key::Scan::LeftAlt: return ModifierKey::LeftAlt;
        case Key::Scan::RightAlt: return ModifierKey::RightAlt;
        case Key::Scan::LeftControl: return ModifierKey::LeftCtrl;
        case Key::Scan::RightControl: return ModifierKey::RightCtrl;
        default: return ModifierKey::None;
    }
}

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
    Base class for objects passed to input event handlers concerning cursor input.
*/
class CEGUIEXPORT CursorInputEventArgs : public WindowEventArgs
{
public:

    CursorInputEventArgs(Window* wnd, const glm::vec2& globalPos, MouseButtons buttons = {}, ModifierKeys modifiers = {});

    //! Current cursor position in screen coordinates
    glm::vec2 d_globalPos;
    //! Current cursor position in a rendering surface coordinates
    glm::vec2 d_surfacePos;
    //! Current cursor position in local coordinates of the window
    glm::vec2 d_localPos;
    //! State of mouse buttons at the moment of sending the event. See MouseButton.
    MouseButtons d_buttons;
    //! State of modifier keys at the moment of sending the event. See ModifierKey.
    ModifierKeys d_modifiers;
};

/*!
\brief
    Cursor input event args used for cursor move events.
*/
class CEGUIEXPORT CursorMoveEventArgs : public CursorInputEventArgs
{
public:

    CursorMoveEventArgs(const CursorInputEventArgs& e, const glm::vec2& moveDelta)
        : CursorInputEventArgs(e), d_moveDelta(moveDelta)
    {}

    CursorMoveEventArgs(Window* wnd, const glm::vec2& globalPos, MouseButtons buttons, ModifierKeys modifiers, const glm::vec2& moveDelta)
        : CursorInputEventArgs(wnd, globalPos, buttons, modifiers), d_moveDelta(moveDelta)
    {}

    //! Variation of cursor position from last cursor input
    glm::vec2 d_moveDelta;
};

/*!
\brief
    Cursor input event args used for mouse button state change events.
*/
class CEGUIEXPORT MouseButtonEventArgs : public CursorInputEventArgs
{
public:

    MouseButtonEventArgs(Window* wnd, const glm::vec2& globalPos, MouseButtons buttons, ModifierKeys modifiers, MouseButton button, int clickEventOrder = 0)
        : CursorInputEventArgs(wnd, globalPos, buttons, modifiers), d_button(button), d_generatedClickEventOrder(clickEventOrder)
    {}

    //! Mouse button that triggered this event
    MouseButton d_button = MouseButton::Invalid;
    //! An order of click generated (typically 1 to 3, or 0 for no click event)
    int d_generatedClickEventOrder = 0;
};

/*!
\brief
    Cursor input event args used for scroll events.
*/
class CEGUIEXPORT ScrollEventArgs : public CursorInputEventArgs
{
public:

    ScrollEventArgs(Window* wnd, const glm::vec2& globalPos, MouseButtons buttons, ModifierKeys modifiers, float delta)
        : CursorInputEventArgs(wnd, globalPos, buttons, modifiers), d_delta(delta)
    {}

    //! Amount of the scroll
    float d_delta = 0.f;
};

/*!
\brief
    EventArgs based class used for certain drag/drop notifications
*/
class CEGUIEXPORT DragDropEventArgs : public WindowEventArgs
{
public:
    DragDropEventArgs(Window* wnd, DragContainer* dragDropItem = nullptr) : WindowEventArgs(wnd), d_dragDropItem(dragDropItem) {}
    DragContainer* d_dragDropItem = nullptr; //!< pointer to the DragContainer window being dragged / dropped.
};

/*!
\brief
    EventArgs based class that is used for objects passed to input event handlers
    concerning text input.
*/
class CEGUIEXPORT KeyEventArgs : public WindowEventArgs
{
public:
    KeyEventArgs(Window* wnd, Key::Scan key, ModifierKeys modifiers = {}) : WindowEventArgs(wnd), d_key(key), d_modifiers(modifiers) {}

    //! CEGUI code of the key that triggered this event
    Key::Scan d_key = Key::Scan::Unknown;
    //! State of modifier keys at the moment of sending the event. See ModifierKey.
    ModifierKeys d_modifiers;
};

/*!
\brief
	EventArgs based class that is used for objects passed to input event handlers
	concerning text input.
*/
class CEGUIEXPORT TextEventArgs : public WindowEventArgs
{
public:
	TextEventArgs(Window* wnd, char32_t character = 0) : WindowEventArgs(wnd), d_character(character) {}

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

	Window*	otherWindow = nullptr; //!< Pointer to the other window involved in the activation change.
};

/*!
\brief
    EventArgs based class that is used for notifications regarding the main
    display.
*/
class CEGUIEXPORT DisplayEventArgs : public EventArgs
{
public:
    DisplayEventArgs(const Sizef& sz): size(sz) {}

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
    FontEventArgs(Font* font) : font(font) {}

    //! Pointer to the font object related to the event notification.
    Font* font;
};

struct CEGUIEXPORT KeySemanticMapping
{
    // TODO StringAtom!
    String value;
    Key::Scan scanCode = Key::Scan::Unknown;
    ModifierKeyRule modifiers;

    bool operator <(const KeySemanticMapping& other) const { return value < other.value; }
};

// TODO StringAtom!
struct CEGUIEXPORT KeySemanticMappingComp
{
    bool operator() (const KeySemanticMapping& s, const String& value) const { return s.value < value; }
    bool operator() (const String& value, const KeySemanticMapping& s) const { return value < s.value; }
};

struct CEGUIEXPORT MouseButtonSemanticMapping
{
    // TODO StringAtom!
    String value;
    MouseButton button = MouseButton::Invalid;
    ModifierKeyRule modifiers;
    MouseButtons buttons;
    int clickOrder = 0;

    bool operator <(const MouseButtonSemanticMapping& other) const { return value < other.value; }
};

// TODO StringAtom!
struct CEGUIEXPORT MouseButtonSemanticMappingComp
{
    bool operator() (const MouseButtonSemanticMapping& s, const String& value) const { return s.value < value; }
    bool operator() (const String& value, const MouseButtonSemanticMapping& s) const { return value < s.value; }
};

//! \brief Predefined semantics that can be associated with specific input operations.
namespace SemanticValue
{
    // TODO StringAtom!
    extern const String CEGUIEXPORT GoToPreviousCharacter;
    extern const String CEGUIEXPORT GoToNextCharacter;
    extern const String CEGUIEXPORT GoToPreviousWord;
    extern const String CEGUIEXPORT GoToNextWord;
    extern const String CEGUIEXPORT GoToStartOfLine;
    extern const String CEGUIEXPORT GoToEndOfLine;
    extern const String CEGUIEXPORT GoToStartOfDocument;
    extern const String CEGUIEXPORT GoToEndOfDocument;
    extern const String CEGUIEXPORT GoToNextPage;
    extern const String CEGUIEXPORT GoToPreviousPage;
    extern const String CEGUIEXPORT GoDown;
    extern const String CEGUIEXPORT GoUp;

    extern const String CEGUIEXPORT SelectRange;
    extern const String CEGUIEXPORT SelectCumulative;
    extern const String CEGUIEXPORT SelectWord;
    extern const String CEGUIEXPORT SelectAll;
    extern const String CEGUIEXPORT SelectPreviousCharacter;
    extern const String CEGUIEXPORT SelectNextCharacter;
    extern const String CEGUIEXPORT SelectPreviousWord;
    extern const String CEGUIEXPORT SelectNextWord;
    extern const String CEGUIEXPORT SelectToStartOfLine;
    extern const String CEGUIEXPORT SelectToEndOfLine;
    extern const String CEGUIEXPORT SelectToStartOfDocument;
    extern const String CEGUIEXPORT SelectToEndOfDocument;
    extern const String CEGUIEXPORT SelectToNextPage;
    extern const String CEGUIEXPORT SelectToPreviousPage;
    extern const String CEGUIEXPORT SelectNextPage;
    extern const String CEGUIEXPORT SelectPreviousPage;
    extern const String CEGUIEXPORT SelectUp;
    extern const String CEGUIEXPORT SelectDown;

    extern const String CEGUIEXPORT DeleteNextCharacter;
    extern const String CEGUIEXPORT DeletePreviousCharacter;
    extern const String CEGUIEXPORT Confirm;
    extern const String CEGUIEXPORT Back;
    extern const String CEGUIEXPORT Undo;
    extern const String CEGUIEXPORT Redo;
    extern const String CEGUIEXPORT Cut;
    extern const String CEGUIEXPORT Copy;
    extern const String CEGUIEXPORT Paste;
    extern const String CEGUIEXPORT NavigateToNext;
    extern const String CEGUIEXPORT NavigateToPrevious;
}

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
