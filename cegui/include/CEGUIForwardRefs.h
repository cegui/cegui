/***********************************************************************
	filename: 	CEGUIForwardRefs.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Forward declares all core system classes
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
    Forward declare majority of core classes
*************************************************************************/
class BasicRenderedStringParser;
class BiDiVisualMapping;
class CentredRenderedString;
class colour;
class ColourRect;
class CoordConverter;
class DefaultLogger;
class DefaultRenderedStringParser;
class DefaultResourceProvider;
class DynamicModule;
class Event;
class EventArgs;
class EventSet;
class Exception;
class FactoryModule;
class Font;
class FontGlyph;
class FontManager;
class FormattedRenderedString;
class GeometryBuffer;
class GlobalEventSet;
class Image;
class ImageCodec;
class ImagerySection;
class Imageset;
class ImagesetManager;
class JustifiedRenderedString;
class LeftAlignedRenderedString;
class Logger;
class MouseCursor;
class Property;
class PropertyHelper;
class PropertyReceiver;
class PropertySet;
class RawDataContainer;
class Rect;
class RegexMatcher;
class RenderedString;
class RenderedStringComponent;
class RenderedStringImageComponent;
class RenderedStringParser;
class RenderedStringTextComponent;
class RenderedStringWidgetComponent;
class Renderer;
class RenderEffect;
struct RenderingContext;
class RenderingRoot;
class RenderingSurface;
class RenderingWindow;
class RenderQueue;
class RenderSystem;
class RenderTarget;
class ResourceEventSet;
class ResourceProvider;
class RightAlignedRenderedString;
class Scheme;
class SchemeManager;
class ScriptFunctor;
class ScriptModule;
class Size;
class String;
class System;
class Texture;
class TextureTarget;
class TextUtils;
class Vector2;
class Vector3;
struct Vertex;
class WidgetLookFeel;
class Window;
class WindowFactory;
class WindowFactoryManager;
class WindowManager;
class WindowRenderer;
class WindowRendererModule;
class WRFactoryRegisterer;
class XMLAttributes;
class XMLHandler;
class XMLParser;

/*************************************************************************
    Forward declare window / widget classes.
*************************************************************************/
class ButtonBase;
class Checkbox;
class ClippedContainer;
class Combobox;
class ComboDropList;
class DragContainer;
class Editbox;
class FrameWindow;
class GUISheet;
class ItemEntry;
class ItemListBase;
class ItemListbox;
class Listbox;
class ListboxItem;
class ListboxTextItem;
class ListHeader;
class ListHeaderSegment;
class Menubar;
class MenuBase;
class MenuItem;
class MultiColumnList;
class MultiLineEditbox;
class PopupMenu;
class ProgressBar;
class PushButton;
class RadioButton;
class ScrollablePane;
class Scrollbar;
class ScrolledContainer;
class ScrolledItemListBase;
class Slider;
class Spinner;
class TabButton;
class TabControl;
class Thumb;
class Titlebar;
class Tooltip;
class Tree;
class TreeItem;

/*************************************************************************
    Forward declare EventArg based classes.
*************************************************************************/
class ActivationEventArgs;
class DisplayEventArgs;
class DragDropEventArgs;
class HeaderSequenceEventArgs;
class KeyEventArgs;
class MouseCursorEventArgs;
class MouseEventArgs;
class RenderQueueEventArgs;
class ResourceEventArgs;
class TreeEventArgs;
class UpdateEventArgs;
class WindowEventArgs;

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIForwardRefs_h_
