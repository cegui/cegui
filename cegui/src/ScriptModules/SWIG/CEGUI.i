%module(directors="1") CEGUI
%{
#include "CEGUI/CEGUI.h"
using namespace CEGUI;
%}

%include <std_string.i>
%include <typemaps.i>


%include "CEGUI/Base.h"
%include "CEGUI/Singleton.h"

%template(CEGUISingleton_WindowManager) CEGUI::Singleton<CEGUI::WindowManager>;
%template(CEGUISingleton_ImageManager) CEGUI::Singleton<CEGUI::ImageManager>;
%template(CEGUISingleton_AnimationManager) CEGUI::Singleton<CEGUI::AnimationManager>;
%template(CEGUISingleton_GlobalEventSet) CEGUI::Singleton<CEGUI::GlobalEventSet>;
%template(CEGUISingleton_System) CEGUI::Singleton<CEGUI::System>;


%ignore CEGUI::ImageManager::getIterator;


// numeric and string
%include "CEGUI/Sizef.h"
%include "CEGUI/USize.h"
%include "CEGUI/Rectf.h"
%include "CEGUI/URect.h"
%include "CEGUI/UVector.h"
%include "CEGUI/Interpolator.h"
%include "CEGUI/Quaternion.h"
%include "CEGUI/String.h"

// string conversion from python
%typemap(in) CEGUI::String & {
	Py_ssize_t  strLen = 0;
	const char* strData = PyUnicode_AsUTF8AndSize( $input, &strLen );
	$1 = new CEGUI::String( strData, strLen );
}
%typemap(freearg) CEGUI::String & %{
	delete $1;
%}
%typemap(typecheck) CEGUI::String & %{
	$1 = PyUnicode_Check($input) ? 1 : 0;
%}
// string conversion to python
%typemap(out) CEGUI::String & %{
	$result = PyUnicode_DecodeUTF8($1->c_str(), strlen($1->c_str()), NULL);
%}

// events, poperty
%include "CEGUI/Event.h"
%include "CEGUI/EventArgs.h"
%include "CEGUI/EventSet.h"
%include "CEGUI/Property.h"
%include "CEGUI/PropertyHelper.h"
%include "CEGUI/PropertySet.h"

// element and window
%include "CEGUI/Element.h"
%include "CEGUI/NamedElement.h"
%include "CEGUI/Window.h"

// some base classes
%include "CEGUI/Image.h"
%include "CEGUI/FormattedRenderedString.h"
%include "CEGUI/InputEventReceiver.h"
%include "CEGUI/Logger.h"
%include "CEGUI/RenderedStringParser.h"
%include "CEGUI/RenderingSurface.h"

// other clases
%include "CEGUI/Affector.h"
%include "CEGUI/Animation.h"
%include "CEGUI/AnimationInstance.h"
%include "CEGUI/AnimationManager.h"
%include "CEGUI/AspectMode.h"
%include "CEGUI/BitmapImage.h"
%include "CEGUI/BasicRenderedStringParser.h"
%include "CEGUI/BidiVisualMapping.h"
%include "CEGUI/BoundSlot.h"
%include "CEGUI/CentredRenderedString.h"
%include "CEGUI/Clipboard.h"
%include "CEGUI/Colour.h"
%include "CEGUI/ColourRect.h"
%include "CEGUI/CoordConverter.h"
%include "CEGUI/DataContainer.h"
%include "CEGUI/DefaultLogger.h"
%include "CEGUI/DefaultRenderedStringParser.h"
%include "CEGUI/DefaultResourceProvider.h"
%include "CEGUI/DynamicModule.h"
%include "CEGUI/FactoryModule.h"
%include "CEGUI/FactoryRegisterer.h"
%include "CEGUI/FontGlyph.h"
%include "CEGUI/GeometryBuffer.h"
%include "CEGUI/GlobalEventSet.h"
%include "CEGUI/GUIContext.h"
%include "CEGUI/HorizontalAlignment.h"
%include "CEGUI/ImageCodec.h"
%include "CEGUI/ImageManager.h"
%include "CEGUI/InputEvent.h"
%include "CEGUI/InputEvents.h"
%include "CEGUI/InputAggregator.h"
%include "CEGUI/JustifiedRenderedString.h"
%include "CEGUI/KeyFrame.h"
%include "CEGUI/LeftAlignedRenderedString.h"
%include "CEGUI/LinkedEvent.h"
%include "CEGUI/Cursor.h"
%include "CEGUI/RegexMatcher.h"
%include "CEGUI/RenderedString.h"
%include "CEGUI/RenderedStringImageComponent.h"
%include "CEGUI/RenderedStringTextComponent.h"
%include "CEGUI/RenderedStringWidgetComponent.h"
%include "CEGUI/RenderedStringWordWrapper.h"
%include "CEGUI/RenderEffect.h"
%include "CEGUI/RenderEffectManager.h"
%include "CEGUI/Renderer.h"
%include "CEGUI/RenderingContext.h"
%include "CEGUI/RenderingWindow.h"
%include "CEGUI/RenderQueue.h"
%include "CEGUI/RenderTarget.h"
%include "CEGUI/RightAlignedRenderedString.h"
%include "CEGUI/Scheme.h"
%include "CEGUI/SchemeManager.h"
%include "CEGUI/ScriptModule.h"
%include "CEGUI/SemanticInputEvent.h"
%include "CEGUI/SimpleTimer.h"
%include "CEGUI/StringTranscoder.h"
%include "CEGUI/System.h"
%include "CEGUI/Texture.h"
%include "CEGUI/TextureTarget.h"
%include "CEGUI/TextUtils.h"
%include "CEGUI/Version.h"
%include "CEGUI/Vertex.h"
%include "CEGUI/VerticalAlignment.h"
%include "CEGUI/WindowFactory.h"
%include "CEGUI/WindowFactoryManager.h"
%include "CEGUI/WindowManager.h"
%include "CEGUI/WindowRendererManager.h"
%include "CEGUI/XMLAttributes.h"
%include "CEGUI/XMLHandler.h"
%include "CEGUI/XMLParser.h"
%include "CEGUI/XMLSerializer.h"

// views, falagard and widgets
%include "CEGUI/views/All.h"
%include "CEGUI/falagard/WidgetLookManager.h"
%include "CEGUI/widgets/All.h"

/*
%include "CEGUI/Font.h"
%include "CEGUI/FontManager.h"
%include "CEGUI/Exceptions.h"
%include "CEGUI/TplInterpolators.h"
%include "CEGUI/TplWindowFactory.h"
%include "CEGUI/TplWindowRendererFactory.h"
%include "CEGUI/TplWindowFactoryRegisterer.h"
%include "CEGUI/TplWRFactoryRegisterer.h"
*/
