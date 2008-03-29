/***********************************************************************
	filename: 	CEGUIForwardRefs.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Forward declares all core system classes
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
class ColourRect;
class colour;
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
class ScriptWindowHelper;
class System;
class FactoryModule;
class ScriptModule;
class ResourceProvider;
class DefaultResourceProvider;
class DynamicModule;
class PropertyReceiver;
class GlobalEventSet;
class XMLAttributes;
class XMLHandler;
class XMLParser;
class WindowRenderer;
class WindowRendererManager;

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
class ListHeaderSegment;
class ListHeader;
class MultiColumnList;
class ProgressBar;
class Thumb;
class Scrollbar;
class Slider;
class ListboxItem;
class ListboxTextItem;
class ComboDropList;
class MultiLineEditbox;
class Spinner;
class GUISheet;
class DragContainer;
class ScrollablePane;
class ScrolledContainer;
class Tooltip;
class ItemEntry;
class MenuItem;
class ItemListBase;
class MenuBase;
class Menubar;
class PopupMenu;
class ScrolledItemListBase;
class ItemListbox;
class TabButton;
class TabControl;
class Tree;

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIForwardRefs_h_
