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
	CEGUI::Key::A            , // A = 0,        ///< The A key
	CEGUI::Key::B            , // B,            ///< The B key
	CEGUI::Key::C            , // C,            ///< The C key
	CEGUI::Key::D            , // D,            ///< The D key
	CEGUI::Key::E            , // E,            ///< The E key
	CEGUI::Key::F            , // F,            ///< The F key
	CEGUI::Key::G            , // G,            ///< The G key
	CEGUI::Key::H            , // H,            ///< The H key
	CEGUI::Key::I            , // I,            ///< The I key
	CEGUI::Key::J            , // J,            ///< The J key
	CEGUI::Key::K            , // K,            ///< The K key
	CEGUI::Key::L            , // L,            ///< The L key
	CEGUI::Key::M            , // M,            ///< The M key
	CEGUI::Key::N            , // N,            ///< The N key
	CEGUI::Key::O            , // O,            ///< The O key
	CEGUI::Key::P            , // P,            ///< The P key
	CEGUI::Key::Q            , // Q,            ///< The Q key
	CEGUI::Key::R            , // R,            ///< The R key
	CEGUI::Key::S            , // S,            ///< The S key
	CEGUI::Key::T            , // T,            ///< The T key
	CEGUI::Key::U            , // U,            ///< The U key
	CEGUI::Key::V            , // V,            ///< The V key
	CEGUI::Key::W            , // W,            ///< The W key
	CEGUI::Key::X            , // X,            ///< The X key
	CEGUI::Key::Y            , // Y,            ///< The Y key
	CEGUI::Key::Z            , // Z,            ///< The Z key
	CEGUI::Key::Zero         , // Num0,         ///< The 0 key
	CEGUI::Key::One          , // Num1,         ///< The 1 key
	CEGUI::Key::Two          , // Num2,         ///< The 2 key
	CEGUI::Key::Three        , // Num3,         ///< The 3 key
	CEGUI::Key::Four         , // Num4,         ///< The 4 key
	CEGUI::Key::Five         , // Num5,         ///< The 5 key
	CEGUI::Key::Six          , // Num6,         ///< The 6 key
	CEGUI::Key::Seven        , // Num7,         ///< The 7 key
	CEGUI::Key::Eight        , // Num8,         ///< The 8 key
	CEGUI::Key::Nine         , // Num9,         ///< The 9 key
	CEGUI::Key::Escape       , // Escape,       ///< The Escape key
	CEGUI::Key::LeftControl  , // LControl,     ///< The left Control key
	CEGUI::Key::LeftShift    , // LShift,       ///< The left Shift key
	CEGUI::Key::LeftAlt      , // LAlt,         ///< The left Alt key
	CEGUI::Key::LeftWindows  , // LSystem,      ///< The left OS specific key: window (Windows and Linux), apple (MacOS X), ...
	CEGUI::Key::RightControl , // RControl,     ///< The right Control key
	CEGUI::Key::RightShift   , // RShift,       ///< The right Shift key
	CEGUI::Key::RightAlt     , // RAlt,         ///< The right Alt key
	CEGUI::Key::RightWindows , // RSystem,      ///< The right OS specific key: window (Windows and Linux), apple (MacOS X), ...
	CEGUI::Key::AppMenu      , // Menu,         ///< The Menu key
	CEGUI::Key::LeftBracket  , // LBracket,     ///< The [ key
	CEGUI::Key::RightBracket , // RBracket,     ///< The ] key
	CEGUI::Key::Semicolon    , // SemiColon,    ///< The ; key
	CEGUI::Key::Comma        , // Comma,        ///< The , key
	CEGUI::Key::Period       , // Period,       ///< The . key
	CEGUI::Key::Apostrophe   , // Quote,        ///< The ' key
	CEGUI::Key::Slash        , // Slash,        ///< The / key
	CEGUI::Key::Backslash    , // BackSlash,    ///< The \ key
	CEGUI::Key::Grave        , // Tilde,        ///< The ~ key
	CEGUI::Key::Equals       , // Equal,        ///< The = key
	CEGUI::Key::Minus        , // Dash,         ///< The - key
	CEGUI::Key::Space        , // Space,        ///< The Space key
	CEGUI::Key::Return       , // Return,       ///< The Return key
	CEGUI::Key::Backspace    , // BackSpace,    ///< The Backspace key
	CEGUI::Key::Tab          , // Tab,          ///< The Tabulation key
	CEGUI::Key::PageUp       , // PageUp,       ///< The Page up key
	CEGUI::Key::PageDown     , // PageDown,     ///< The Page down key
	CEGUI::Key::End          , // End,          ///< The End key
	CEGUI::Key::Home         , // Home,         ///< The Home key
	CEGUI::Key::Insert       , // Insert,       ///< The Insert key
	CEGUI::Key::Delete       , // Delete,       ///< The Delete key
	CEGUI::Key::Add          , // Add,          ///< The + key
	CEGUI::Key::Subtract     , // Subtract,     ///< The - key
	CEGUI::Key::Multiply     , // Multiply,     ///< The * key
	CEGUI::Key::Divide       , // Divide,       ///< The / key
	CEGUI::Key::ArrowLeft    , // Left,         ///< Left arrow
	CEGUI::Key::ArrowRight   , // Right,        ///< Right arrow
	CEGUI::Key::ArrowUp      , // Up,           ///< Up arrow
	CEGUI::Key::ArrowDown    , // Down,         ///< Down arrow
	CEGUI::Key::Numpad0      , // Numpad0,      ///< The numpad 0 key
	CEGUI::Key::Numpad1      , // Numpad1,      ///< The numpad 1 key
	CEGUI::Key::Numpad2      , // Numpad2,      ///< The numpad 2 key
	CEGUI::Key::Numpad3      , // Numpad3,      ///< The numpad 3 key
	CEGUI::Key::Numpad4      , // Numpad4,      ///< The numpad 4 key
	CEGUI::Key::Numpad5      , // Numpad5,      ///< The numpad 5 key
	CEGUI::Key::Numpad6      , // Numpad6,      ///< The numpad 6 key
	CEGUI::Key::Numpad7      , // Numpad7,      ///< The numpad 7 key
	CEGUI::Key::Numpad8      , // Numpad8,      ///< The numpad 8 key
	CEGUI::Key::Numpad9      , // Numpad9,      ///< The numpad 9 key
	CEGUI::Key::F1           , // F1,           ///< The F1 key
	CEGUI::Key::F2           , // F2,           ///< The F2 key
	CEGUI::Key::F3           , // F3,           ///< The F3 key
	CEGUI::Key::F4           , // F4,           ///< The F4 key
	CEGUI::Key::F5           , // F5,           ///< The F5 key
	CEGUI::Key::F6           , // F6,           ///< The F6 key
	CEGUI::Key::F7           , // F7,           ///< The F7 key
	CEGUI::Key::F8           , // F8,           ///< The F8 key
	CEGUI::Key::F9           , // F9,           ///< The F9 key
	CEGUI::Key::F10          , // F10,          ///< The F10 key
	CEGUI::Key::F11          , // F11,          ///< The F11 key
	CEGUI::Key::F12          , // F12,          ///< The F12 key
	CEGUI::Key::F13          , // F13,          ///< The F13 key
	CEGUI::Key::F14          , // F14,          ///< The F14 key
	CEGUI::Key::F15          , // F15,          ///< The F15 key
	CEGUI::Key::Pause          // Pause,        ///< The Pause key
};

#endif /* _SFML_KEYCODES_TO_CEGUI_MAPPINGS_H_ */
