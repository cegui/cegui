/***********************************************************************
	filename: 	CEGUIForwardRefs.h
	created:	21/2/2004
	author:		Paul D Turner

	purpose:	Forward declares all core system classes
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
    Forward reference declarations of core system classes
*************************************************************************/
class AbsoluteDim;
class BaseDim;
class BoundSlot;
class ClippedContainer;
class ColourRect;
class ComponentArea;
class CoordConverter;
class DefaultLogger;
class DefaultResourceProvider;
class Dimension;
class DynamicModule;
class Event;
class EventSet;
class FactoryModule;
class FalagardComponentBase;
class FalagardXMLHelper;
class Font;
class FontDim;
class FontManager;
class FrameComponent;
class GlobalEventSet;
class Image;
class ImageCodec;
class ImageDim;
class ImageryComponent;
class ImagerySection;
class Imageset;
class ImagesetManager;
class LayerSpecification;
class Logger;
class MouseCursor;
class NamedArea;
class Property;
class PropertyDim;
class PropertyHelper;
class PropertyInitialiser;
class PropertyReceiver;
class PropertySet;
class RawDataContainer;
class Rect;
template<typename T>
class RefCounted;
class RenderCache;
class RenderTarget;
class Renderer;
class ResourceProvider;
class Scheme;
class SchemeManager;
class ScriptModule;
class SectionSpecification;
template <typename T>
class Singleton;
class Size;
class StateImagery;
class String;
class SubscriberSlot;
class System;
class Task;
class TextComponent;
class TextUtils;
class Texture;
class UDim;
class URect;
class UVector2;
class UnifiedDim;
class Vector2;
class Vector3;
class WidgetComponent;
class WidgetDim;
class WidgetFadeTask;
class WidgetLookFeel;
class WidgetLookManager;
class Window;
class WindowFactory;
class WindowFactoryManager;
class WindowManager;
class WindowRenderer;
class WindowRendererFactory;
class WindowRendererManager;
class XMLAttributes;
class XMLHandler;
class XMLParser;
class XMLSerializer;
class colour;

/*************************************************************************
    Forward reference declarations of exception types
*************************************************************************/
class Exception;
class AlreadyExistsException;
class FileIOException;
class GenericException;
class InvalidRequestException;
class MemoryException;
class NullObjectException;
class ObjectInUseException;
class RendererException;
class ScriptException;
class UnknownObjectException;

/*************************************************************************
    Forward reference declarations of event argument class types
*************************************************************************/
class EventArgs;
class ActivationEventArgs;
class DragDropEventArgs;
class HeaderSequenceEventArgs;
class KeyEventArgs;
class MouseCursorEventArgs;
class MouseEventArgs;
class TreeEventArgs;
class UpdateEventArgs;
class WindowEventArgs;

/*************************************************************************
    Forward reference declarations of GUI widget elements
*************************************************************************/
class ButtonBase;
class Checkbox;
class ComboDropList;
class Combobox;
class DragContainer;
class Editbox;
class FrameWindow;
class GUISheet;
class ItemEntry;
class ItemListBase;
class ItemListbox;
class ListHeader;
class ListHeaderSegment;
class Listbox;
class ListboxItem;
class ListboxTextItem;
class MenuBase;
class MenuItem;
class Menubar;
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
    Forward reference declarations of specialised window renderer types
*************************************************************************/
class EditboxWindowRenderer;
class ItemEntryWindowRenderer;
class ItemListBaseWindowRenderer;
class ListHeaderWindowRenderer;
class ListboxWindowRenderer;
class MultiColumnListWindowRenderer;
class MultiLineEditboxWindowRenderer;
class ScrollablePaneWindowRenderer;
class ScrollbarWindowRenderer;
class SliderWindowRenderer;
class TabControlWindowRenderer;
class TooltipWindowRenderer;

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIForwardRefs_h_
