/*
 * Maps SFML2 keyboard codes to CEGUI key codes.
 * Useful if you're porting a game that uses CEGUI.
 *
 * (C) 2016 Georger Araujo, do whatever you want with this.
 *
 * No warranty is offered or implied; use this code at your own risk.
 *
 * Released to the Public Domain, do whatever you want with this: use it,
 * modify it, copy it into your own code, ...
 * No need to mention me or this "license" in your code or docs, even though
 * it would be appreciated, of course.
 */

#ifndef _SFML_KEYCODES_TO_CEGUI_MAPPINGS_H_
#define _SFML_KEYCODES_TO_CEGUI_MAPPINGS_H_

// See http://cegui.org.uk/wiki/SFML_to_CEGUI_keytable

// maps SFML2 keyboard codes to CEGUI key codes
static int sfKeyToCEGUIKey[sf::Keyboard::KeyCount] = {
	(int)CEGUI::Key::Scan::A            , // A = 0,        ///< The A key
	(int)CEGUI::Key::Scan::B            , // B,            ///< The B key
	(int)CEGUI::Key::Scan::C            , // C,            ///< The C key
	(int)CEGUI::Key::Scan::D            , // D,            ///< The D key
	(int)CEGUI::Key::Scan::E            , // E,            ///< The E key
	(int)CEGUI::Key::Scan::F            , // F,            ///< The F key
	(int)CEGUI::Key::Scan::G            , // G,            ///< The G key
	(int)CEGUI::Key::Scan::H            , // H,            ///< The H key
	(int)CEGUI::Key::Scan::I            , // I,            ///< The I key
	(int)CEGUI::Key::Scan::J            , // J,            ///< The J key
	(int)CEGUI::Key::Scan::K            , // K,            ///< The K key
	(int)CEGUI::Key::Scan::L            , // L,            ///< The L key
	(int)CEGUI::Key::Scan::M            , // M,            ///< The M key
	(int)CEGUI::Key::Scan::N            , // N,            ///< The N key
	(int)CEGUI::Key::Scan::O            , // O,            ///< The O key
	(int)CEGUI::Key::Scan::P            , // P,            ///< The P key
	(int)CEGUI::Key::Scan::Q            , // Q,            ///< The Q key
	(int)CEGUI::Key::Scan::R            , // R,            ///< The R key
	(int)CEGUI::Key::Scan::S            , // S,            ///< The S key
	(int)CEGUI::Key::Scan::T            , // T,            ///< The T key
	(int)CEGUI::Key::Scan::U            , // U,            ///< The U key
	(int)CEGUI::Key::Scan::V            , // V,            ///< The V key
	(int)CEGUI::Key::Scan::W            , // W,            ///< The W key
	(int)CEGUI::Key::Scan::X            , // X,            ///< The X key
	(int)CEGUI::Key::Scan::Y            , // Y,            ///< The Y key
	(int)CEGUI::Key::Scan::Z            , // Z,            ///< The Z key
	(int)CEGUI::Key::Scan::Zero         , // Num0,         ///< The 0 key
	(int)CEGUI::Key::Scan::One          , // Num1,         ///< The 1 key
	(int)CEGUI::Key::Scan::Two          , // Num2,         ///< The 2 key
	(int)CEGUI::Key::Scan::Three        , // Num3,         ///< The 3 key
	(int)CEGUI::Key::Scan::Four         , // Num4,         ///< The 4 key
	(int)CEGUI::Key::Scan::Five         , // Num5,         ///< The 5 key
	(int)CEGUI::Key::Scan::Six          , // Num6,         ///< The 6 key
	(int)CEGUI::Key::Scan::Seven        , // Num7,         ///< The 7 key
	(int)CEGUI::Key::Scan::Eight        , // Num8,         ///< The 8 key
	(int)CEGUI::Key::Scan::Nine         , // Num9,         ///< The 9 key
	(int)CEGUI::Key::Scan::Esc          , // Escape,       ///< The Escape key
	(int)CEGUI::Key::Scan::LeftControl  , // LControl,     ///< The left Control key
	(int)CEGUI::Key::Scan::LeftShift    , // LShift,       ///< The left Shift key
	(int)CEGUI::Key::Scan::LeftAlt      , // LAlt,         ///< The left Alt key
	(int)CEGUI::Key::Scan::LeftWindows  , // LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
	(int)CEGUI::Key::Scan::RightControl , // RControl,     ///< The right Control key
	(int)CEGUI::Key::Scan::RightShift   , // RShift,       ///< The right Shift key
	(int)CEGUI::Key::Scan::RightAlt     , // RAlt,         ///< The right Alt key
	(int)CEGUI::Key::Scan::RightWindows , // RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
	(int)CEGUI::Key::Scan::AppMenu      , // Menu,         ///< The Menu key
	(int)CEGUI::Key::Scan::LeftBracket  , // LBracket,     ///< The [ key
	(int)CEGUI::Key::Scan::RightBracket , // RBracket,     ///< The ] key
	(int)CEGUI::Key::Scan::Semicolon    , // SemiColon,    ///< The ; key
	(int)CEGUI::Key::Scan::Comma        , // Comma,        ///< The , key
	(int)CEGUI::Key::Scan::Period       , // Period,       ///< The . key
	(int)CEGUI::Key::Scan::Apostrophe   , // Quote,        ///< The ' key
	(int)CEGUI::Key::Scan::ForwardSlash , // Slash,        ///< The / key
	(int)CEGUI::Key::Scan::Backslash    , // BackSlash,    ///< The \ key
	(int)CEGUI::Key::Scan::Grave        , // Tilde,        ///< The ~ key
	(int)CEGUI::Key::Scan::Equals       , // Equal,        ///< The = key
	(int)CEGUI::Key::Scan::Minus        , // Dash,         ///< The - key
	(int)CEGUI::Key::Scan::Space        , // Space,        ///< The Space key
	(int)CEGUI::Key::Scan::Return       , // Return,       ///< The Return key
	(int)CEGUI::Key::Scan::Backspace    , // BackSpace,    ///< The Backspace key
	(int)CEGUI::Key::Scan::Tab          , // Tab,          ///< The Tabulation key
	(int)CEGUI::Key::Scan::PageUp       , // PageUp,       ///< The Page up key
	(int)CEGUI::Key::Scan::PageDown     , // PageDown,     ///< The Page down key
	(int)CEGUI::Key::Scan::End          , // End,          ///< The End key
	(int)CEGUI::Key::Scan::Home         , // Home,         ///< The Home key
	(int)CEGUI::Key::Scan::Insert       , // Insert,       ///< The Insert key
	(int)CEGUI::Key::Scan::DeleteKey    , // Delete,       ///< The Delete key
	(int)CEGUI::Key::Scan::Add          , // Add,          ///< The + key
	(int)CEGUI::Key::Scan::Subtract     , // Subtract,     ///< The - key
	(int)CEGUI::Key::Scan::Multiply     , // Multiply,     ///< The * key
	(int)CEGUI::Key::Scan::Divide       , // Divide,       ///< The / key
	(int)CEGUI::Key::Scan::ArrowLeft    , // Left,         ///< Left arrow
	(int)CEGUI::Key::Scan::ArrowRight   , // Right,        ///< Right arrow
	(int)CEGUI::Key::Scan::ArrowUp      , // Up,           ///< Up arrow
	(int)CEGUI::Key::Scan::ArrowDown    , // Down,         ///< Down arrow
	(int)CEGUI::Key::Scan::Numpad_0     , // Numpad0,      ///< The numpad 0 key
	(int)CEGUI::Key::Scan::Numpad_1     , // Numpad1,      ///< The numpad 1 key
	(int)CEGUI::Key::Scan::Numpad_2     , // Numpad2,      ///< The numpad 2 key
	(int)CEGUI::Key::Scan::Numpad_3     , // Numpad3,      ///< The numpad 3 key
	(int)CEGUI::Key::Scan::Numpad_4     , // Numpad4,      ///< The numpad 4 key
	(int)CEGUI::Key::Scan::Numpad_5     , // Numpad5,      ///< The numpad 5 key
	(int)CEGUI::Key::Scan::Numpad_6     , // Numpad6,      ///< The numpad 6 key
	(int)CEGUI::Key::Scan::Numpad_7     , // Numpad7,      ///< The numpad 7 key
	(int)CEGUI::Key::Scan::Numpad_8     , // Numpad8,      ///< The numpad 8 key
	(int)CEGUI::Key::Scan::Numpad_9     , // Numpad9,      ///< The numpad 9 key
	(int)CEGUI::Key::Scan::F1           , // F1,           ///< The F1 key
	(int)CEGUI::Key::Scan::F2           , // F2,           ///< The F2 key
	(int)CEGUI::Key::Scan::F3           , // F3,           ///< The F3 key
	(int)CEGUI::Key::Scan::F4           , // F4,           ///< The F4 key
	(int)CEGUI::Key::Scan::F5           , // F5,           ///< The F5 key
	(int)CEGUI::Key::Scan::F6           , // F6,           ///< The F6 key
	(int)CEGUI::Key::Scan::F7           , // F7,           ///< The F7 key
	(int)CEGUI::Key::Scan::F8           , // F8,           ///< The F8 key
	(int)CEGUI::Key::Scan::F9           , // F9,           ///< The F9 key
	(int)CEGUI::Key::Scan::F10          , // F10,          ///< The F10 key
	(int)CEGUI::Key::Scan::F11          , // F11,          ///< The F11 key
	(int)CEGUI::Key::Scan::F12          , // F12,          ///< The F12 key
	(int)CEGUI::Key::Scan::F13          , // F13,          ///< The F13 key
	(int)CEGUI::Key::Scan::F14          , // F14,          ///< The F14 key
	(int)CEGUI::Key::Scan::F15          , // F15,          ///< The F15 key
	(int)CEGUI::Key::Scan::Pause          // Pause,        ///< The Pause key
};

#endif /* _SFML_KEYCODES_TO_CEGUI_MAPPINGS_H_ */
