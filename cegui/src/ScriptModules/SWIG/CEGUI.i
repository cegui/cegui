%module(directors="1") CEGUI
%{
#include "CEGUI/CEGUI.h"
#include "CEGUI/Config.h"
using namespace CEGUI;

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wcast-function-type"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-label"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
%}

%include <std_string.i>
%include <typemaps.i>


%include "CEGUI/Base.h"
%include "CEGUI/Singleton.h"
%include "CEGUI/RefCounted.h"

%template(CEGUISingleton_WindowManager) CEGUI::Singleton<CEGUI::WindowManager>;
%template(CEGUISingleton_ImageManager) CEGUI::Singleton<CEGUI::ImageManager>;
%template(CEGUISingleton_AnimationManager) CEGUI::Singleton<CEGUI::AnimationManager>;
%template(CEGUISingleton_GlobalEventSet) CEGUI::Singleton<CEGUI::GlobalEventSet>;
%template(CEGUISingleton_System) CEGUI::Singleton<CEGUI::System>;
%template(CEGUISingleton_Logger) CEGUI::Singleton<CEGUI::Logger>;
%template(CEGUISingleton_RenderEffectManager) CEGUI::Singleton<CEGUI::RenderEffectManager>;
%template(CEGUISingleton_SchemeManager) CEGUI::Singleton<CEGUI::SchemeManager>;
%template(CEGUISingleton_WindowFactoryManager) CEGUI::Singleton<CEGUI::WindowFactoryManager>;
%template(CEGUISingleton_WindowRendererManager) CEGUI::Singleton<CEGUI::WindowRendererManager>;
%template(CEGUISingleton_WidgetLookManager) CEGUI::Singleton<CEGUI::WidgetLookManager>;
%template(CEGUISingleton_FontManager) CEGUI::Singleton<CEGUI::FontManager>;


%ignore CEGUI::ImageManager::getIterator;


// numeric and string
%include "CEGUI/Sizef.h"
%include "CEGUI/USize.h"
%include "CEGUI/Rectf.h"
%include "CEGUI/URect.h"
%include "CEGUI/UVector.h"
%include "CEGUI/String.h"

// string conversion from python
%typemap(in) CEGUI::String & {
	Py_ssize_t  strLen = 0;
	const char* strData = PyUnicode_AsUTF8AndSize( $input, &strLen );
	#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
	$1 = new CEGUI::String( strData );
	#else
	$1 = new CEGUI::String( strData, strLen );
	#endif
}
%typemap(freearg) CEGUI::String & %{
	delete $1;
%}
%typemap(typecheck) CEGUI::String & %{
	$1 = PyUnicode_Check($input) ? 1 : 0;
%}
// string conversion to python
%extend CEGUI::String {
	PyObject* asPythonStr() {
		%#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
		return PyUnicode_DecodeUTF32($self->c_str(), $self->length(), NULL, 0);
		%#else
		return PyUnicode_DecodeUTF8($self->c_str(), $self->length(), NULL);
		%#endif
	}
};
%typemap(out) CEGUI::String & %{
	#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
	$result = PyUnicode_DecodeUTF32($1->c_str(), $1->length(), NULL, 0);
	#else
	$result = PyUnicode_DecodeUTF8($1->c_str(), $1->length(), NULL);
	#endif
%}

// events, poperty
%ignore CEGUI::Event::final;
%ignore CEGUI::final;
%include "CEGUI/Event.h"
%include "CEGUI/EventArgs.h"
%include "CEGUI/EventSet.h"
%include "CEGUI/Property.h"
%include "CEGUI/PropertyHelper.h"
%include "CEGUI/PropertySet.h"

// events subscriber base class as PySubscriber
%feature("director") PySubscriber;
%include "CEGUI/SlotFunctorBase.h"
%include "CEGUI/SubscriberSlot.h"
%inline %{
	#include "CEGUI/SubscriberSlot.h"
	class PySubscriber : CEGUI::SubscriberSlot {
			bool run_proxy(const CEGUI::EventArgs& args) { return run(args); }
		public:
			PySubscriber() : CEGUI::SubscriberSlot( &PySubscriber::run_proxy, this ) {}
			virtual bool run(const CEGUI::EventArgs& args) { return false; }
			virtual ~PySubscriber() {};
	};
%}

/* EXAMPLE OF USAGE in Python:
	mywin = ...
	class Callback(CEGUI.PySubscriber):
		def run(self, n):
			mywin.hide()
			return True; # must return bool
	handleHideInfoWin = Callback() # can't be temporary object !!!
	CEGUI.toFrameWindow(mywin).getCloseButton().subscribeEvent( CEGUI.PushButton.EventClicked.asPythonStr(), handleHideInfoWin )
*/

// xml
%include "CEGUI/XMLHandler.h"
%include "CEGUI/ChainedXMLHandler.h"
%include "CEGUI/XMLAttributes.h"
%include "CEGUI/XMLParser.h"
%include "CEGUI/XMLSerializer.h"

// element and window
%include "CEGUI/Element.h"
%include "CEGUI/Window.h"

// some base classes (must be before derived classes)
%include "CEGUI/Image.h"
%include "CEGUI/Logger.h"
%include "CEGUI/RenderingSurface.h"
%include "CEGUI/ResourceEventSet.h"
%include "CEGUI/ResourceProvider.h"
%include "CEGUI/InjectedInputReceiver.h"

// other clases
%{
#include "CEGUI/text/TextParser.h"
#include "CEGUI/text/RenderedText.h"
#include "CEGUI/text/LegacyTextParser.h"
#include "CEGUI/text/FontGlyph.h"
#include "CEGUI/text/FontSizeUnit.h"
%}
%include "CEGUI/text/TextParser.h"
%include "CEGUI/text/RenderedText.h"
%include "CEGUI/text/LegacyTextParser.h"
%include "CEGUI/text/DefaultParagraphDirection.h"
%include "CEGUI/text/BidiVisualMapping.h"
%include "CEGUI/text/FontGlyph.h"
%include "CEGUI/text/FontSizeUnit.h"
%include "CEGUI/text/TextUtils.h"

%include "CEGUI/Affector.h"
%include "CEGUI/Animation.h"
%include "CEGUI/AnimationInstance.h"
%include "CEGUI/AnimationManager.h"
%include "CEGUI/AspectMode.h"
%include "CEGUI/BitmapImage.h"
%include "CEGUI/BoundSlot.h"
%include "CEGUI/Clipboard.h"
%include "CEGUI/Colour.h"
%include "CEGUI/ColourRect.h"
%include "CEGUI/CoordConverter.h"
%include "CEGUI/DataContainer.h"
%include "CEGUI/DefaultLogger.h"

%include "CEGUI/DefaultResourceProvider.h"
%inline %{
	CEGUI::DefaultResourceProvider* toDefaultResourceProvider(CEGUI::ResourceProvider* p) { return static_cast<CEGUI::DefaultResourceProvider*>(p); }
%}
%include "CEGUI/DynamicModule.h"
%include "CEGUI/FactoryModule.h"
%include "CEGUI/FactoryRegisterer.h"
%include "CEGUI/FontManager.h"
%include "CEGUI/GeometryBuffer.h"
%include "CEGUI/GlobalEventSet.h"
%include "CEGUI/GUIContext.h"
%include "CEGUI/HorizontalAlignment.h"
%include "CEGUI/ImageCodec.h"
%include "CEGUI/ImageManager.h"
%include "CEGUI/InputEvent.h"
%include "CEGUI/KeyFrame.h"
%include "CEGUI/LinkedEvent.h"
%include "CEGUI/RegexMatcher.h"
%include "CEGUI/RenderEffect.h"
%include "CEGUI/RenderEffectManager.h"
%include "CEGUI/Renderer.h"
%include "CEGUI/RenderingContext.h"
%include "CEGUI/RenderingWindow.h"
%include "CEGUI/RenderQueue.h"
%include "CEGUI/RenderTarget.h"
%include "CEGUI/Scheme.h"
%include "CEGUI/SchemeManager.h"
%include "CEGUI/ScriptModule.h"
%include "CEGUI/StringTranscoder.h"
%include "CEGUI/System.h"
%include "CEGUI/Texture.h"
%include "CEGUI/TextureTarget.h"
%include "CEGUI/Version.h"
%include "CEGUI/Vertex.h"
%include "CEGUI/VerticalAlignment.h"
%include "CEGUI/WindowFactory.h"
%include "CEGUI/WindowFactoryManager.h"
%include "CEGUI/WindowManager.h"
%include "CEGUI/WindowRendererManager.h"
%include "CEGUI/WindowRenderer.h"

%include "CEGUI/TplWindowFactory.h"
%include "CEGUI/TplWindowRendererFactory.h"
%include "CEGUI/TplWindowFactoryRegisterer.h"
%include "CEGUI/TplWRFactoryRegisterer.h"

// views
%include "CEGUI/views/GenericItemModel.h"
%template(CEGUIGenericItemModel_StandardItem)CEGUI::GenericItemModel< CEGUI::StandardItem >;
%include "CEGUI/views/StandardItemModel.h"

%include "CEGUI/views/ItemModel.h"
%include "CEGUI/views/ItemView.h"
%include "CEGUI/views/ListView.h"
%include "CEGUI/views/StandardItemModel.h"
%ignore CEGUI::TreeViewItemRenderingState::d_renderedText; // deleted = operator in RenderedText
%include "CEGUI/views/TreeView.h"

// falagard
%include "CEGUI/falagard/WidgetLookManager.h"
%include "CEGUI/falagard/WidgetLookFeel.h"

// widgets
%include "CEGUI/widgets/EditboxBase.h"
%include "CEGUI/widgets/ButtonBase.h"
%include "CEGUI/widgets/ItemListBase.h"
%include "CEGUI/widgets/MenuBase.h"
%include "CEGUI/widgets/ListWidget.h"

%include "CEGUI/widgets/LayoutContainer.h"
%include "CEGUI/widgets/GridLayoutContainer.h"
%include "CEGUI/widgets/HorizontalLayoutContainer.h"
%include "CEGUI/widgets/VerticalLayoutContainer.h"

%include "CEGUI/widgets/PushButton.h"
%include "CEGUI/widgets/ToggleButton.h"
%include "CEGUI/widgets/RadioButton.h"
%include "CEGUI/widgets/Editbox.h"
%include "CEGUI/widgets/DefaultWindow.h"
%include "CEGUI/widgets/FrameWindow.h"

%include "CEGUI/widgets/Combobox.h"
%include "CEGUI/widgets/ComboDropList.h"
%include "CEGUI/widgets/DragContainer.h"
%include "CEGUI/widgets/ItemEntry.h"
%include "CEGUI/widgets/ListboxItem.h"
%include "CEGUI/widgets/ListboxTextItem.h"
%include "CEGUI/widgets/ListHeader.h"
%include "CEGUI/widgets/Menubar.h"
%include "CEGUI/widgets/MenuItem.h"
%include "CEGUI/widgets/MultiColumnList.h"
%include "CEGUI/widgets/MultiLineEditbox.h"
%include "CEGUI/widgets/PopupMenu.h"
%include "CEGUI/widgets/ProgressBar.h"
%include "CEGUI/widgets/ScrollablePane.h"
%include "CEGUI/widgets/Scrollbar.h"
%include "CEGUI/widgets/ScrolledContainer.h"
%include "CEGUI/widgets/Slider.h"
%include "CEGUI/widgets/Spinner.h"
%include "CEGUI/widgets/TabButton.h"
%include "CEGUI/widgets/TabControl.h"
%include "CEGUI/widgets/Thumb.h"
%include "CEGUI/widgets/Titlebar.h"
%include "CEGUI/widgets/Tooltip.h"
%include "CEGUI/widgets/TreeWidget.h"

%define WINDOW_CONVERTER(Type)
%inline %{
	CEGUI::Type* to##Type(CEGUI::Window* p) { return static_cast<CEGUI::Type*>(p); }
%}
%enddef

WINDOW_CONVERTER(EditboxBase)
WINDOW_CONVERTER(ButtonBase)
WINDOW_CONVERTER(ItemListBase)
WINDOW_CONVERTER(MenuBase)
WINDOW_CONVERTER(ListWidget)

WINDOW_CONVERTER(LayoutContainer)
WINDOW_CONVERTER(GridLayoutContainer)
WINDOW_CONVERTER(HorizontalLayoutContainer)
WINDOW_CONVERTER(VerticalLayoutContainer)

WINDOW_CONVERTER(PushButton)
WINDOW_CONVERTER(ToggleButton)
WINDOW_CONVERTER(RadioButton)
WINDOW_CONVERTER(Editbox)
WINDOW_CONVERTER(DefaultWindow)
WINDOW_CONVERTER(FrameWindow)

WINDOW_CONVERTER(Combobox)
WINDOW_CONVERTER(ComboDropList)
WINDOW_CONVERTER(DragContainer)
WINDOW_CONVERTER(ItemEntry)

// ListboxItem is not derived from Window
// ListboxTextItem is derived from ListboxItem
%inline %{
	CEGUI::ListboxTextItem* toListboxTextItem(CEGUI::ListboxItem* p) { return static_cast<CEGUI::ListboxTextItem*>(p); }
%}

WINDOW_CONVERTER(ListHeader)
WINDOW_CONVERTER(Menubar)
WINDOW_CONVERTER(MenuItem)
WINDOW_CONVERTER(MultiColumnList)
WINDOW_CONVERTER(MultiLineEditbox)
WINDOW_CONVERTER(PopupMenu)
WINDOW_CONVERTER(ProgressBar)
WINDOW_CONVERTER(ScrollablePane)
WINDOW_CONVERTER(Scrollbar)
WINDOW_CONVERTER(ScrolledContainer)
WINDOW_CONVERTER(Slider)
WINDOW_CONVERTER(Spinner)
WINDOW_CONVERTER(TabButton)
WINDOW_CONVERTER(TabControl)
WINDOW_CONVERTER(Thumb)
WINDOW_CONVERTER(Titlebar)
WINDOW_CONVERTER(Tooltip)
WINDOW_CONVERTER(TreeWidget)
