/************************************************************************
	filename: 	CEGUIForwardRefs.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Forward declares all core system classes
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUIForwardRefs_h_
#define _CEGUIForwardRefs_h_

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Forward reference declarations of all core GUI system classes
*************************************************************************/
class String;
class Vector3;
class Size;
class Exception;
class Rect;
class Texture;
class Renderer;
class Image;
class Imageset;
class ImagesetManager;
class MouseCursor;
class Font;
class FontManager;
class EventArgs;
class Event;
class EventSet;
class Property;
class PropertySet;
class Window;
class WindowFactory;
class WindowManager;
class Scheme;
class SchemeManager;
class System;
class FactoryModule;

/*************************************************************************
	Forward reference declarations for GUI element base classes
*************************************************************************/
class ButtonBase;
class RadioButton;
class Checkbox;
class PushButton;
class Titlebar;
class FrameWindow;
class Editbox;
class Listbox;
class Combobox;
class ListHeader;
class MultiColumnList;
class ProgressBar;
class Thumb;
class Scrollbar;
class Slider;
class Static;
class StaticText;
class StaticImage;
class ListboxItem;
class ListboxTextItem;

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIForwardRefs_h_
