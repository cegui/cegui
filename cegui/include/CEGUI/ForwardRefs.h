/***********************************************************************
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
class Affector;
class Animation;
class AnimationInstance;
class AnimationManager;
class BidiVisualMapping;
class Clipboard;
class Colour;
class ColourRect;
class CoordConverter;
class DefaultLogger;
class DefaultResourceProvider;
class DynamicModule;
class Element;
class ElementEventArgs;
class Event;
class EventAction;
class EventArgs;
class EventSet;
class Exception;
class FactoryModule;
class Font;
class FontGlyph;
class FontManager;
class GeometryBuffer;
class GlobalEventSet;
class GUIContext;
class Image;
class ImageCodec;
class ImageManager;
class ImagerySection;
class Interpolator;
class KeyFrame;
class LinkedEvent;
class LinkedEventArgs;
class Logger;
class Cursor;
class NativeClipboardProvider;
class Property;
template<typename T> class PropertyHelper;
class PropertyReceiver;
class PropertySet;
class RawDataContainer;
class Rectf;
class RegexMatcher;
class Renderer;
class RenderEffect;
class RenderEffectManager;
struct RenderingContext;
class RenderingSurface;
class RenderingWindow;
class RenderQueue;
class RenderTarget;
class ResourceEventSet;
class ResourceProvider;
class Scheme;
class SchemeManager;
class ScriptFunctor;
class ScriptModule;
class Sizef;
class SVGImage;
class String;
class StringTranscoder;
class System;
class Texture;
class TextureTarget;
class TextUtils;
class UBox;
class UDim;
class UVector2;
class URect;
class USize;
struct TexturedColouredVertex;
struct ColouredVertex;
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
class XMLSerializer;

/*************************************************************************
    Forward declare window / widget classes.
*************************************************************************/
class ButtonBase;
class Combobox;
class ComboDropList;
class DefaultWindow;
class DragContainer;
class Editbox;
class FrameWindow;
class GridLayoutContainer;
class HorizontalLayoutContainer;
class ItemEntry;
class ItemListBase;
class LayoutContainer;
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
class Slider;
class Spinner;
class TabButton;
class TabControl;
class Thumb;
class Titlebar;
class ToggleButton;
class Tooltip;
class VerticalLayoutContainer;

/*************************************************************************
    Forward declare model-view classes.
*************************************************************************/
class ItemModel;
class ListView;
class ListWidget;
class ModelIndex;
class StandardItemModel;
class StandardItem;
class TreeView;
class TreeWidget;

/*************************************************************************
    Forward declare EventArg based classes.
*************************************************************************/
class ActivationEventArgs;
class DisplayEventArgs;
class DragDropEventArgs;
class FontEventArgs;
class GUIContextEventArgs;
class HeaderSequenceEventArgs;
class ItemViewEventArgs;
class ModelEventArgs;
class RegexMatchStateArgs;
class RenderQueueEventArgs;
class RenderTargetEventArgs;
class ResourceEventArgs;
class UpdateEventArgs;
class WindowEventArgs;

/*************************************************************************
    Forward declare SVG based classes.
*************************************************************************/
class SVGData;
class SVGDataManager;
class SVGTesselator;

class SVGBasicShape;
class SVGCircle;
class SVGLine;
class SVGPolyline;
class SVGRect;

class SVGPaint;
class SVGPaintStyle;

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIForwardRefs_h_

