#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "CEGUI/CEGUI.h"
#include "CEGUI/falagard/XMLEnumHelper.h"

namespace boost
{
    /*
	 * Workaround that allows users to inherit CEGUI classes with methods
	 * that deal with strings (almost all classes)
	 *
	 * The situation:
	 * boost python can't recognize boost::ref(const CEGUI::String&) and doesn't
	 * auto convert it to python unicode string. This works for all types that are
	 * registered normally (CEGUI::String is always converted, the class is unknown
	 * in python).
	 *
	 * The solution:
	 * Make an overloaded version of boost::ref that just returns what it gets.
	 *
	 * Is this horrible?
	 * YES!
	 */
    inline const CEGUI::String& ref(const CEGUI::String& v)
    {
        return v;
    }
}

namespace CEGUI
{

class PropertyHelper_wrapper
{
public:
    static float stringToFloat(const String& str)
    {
        return PropertyHelper<float>::fromString(str);
    }
    static unsigned int stringToUint(const String& str)
    {
        return PropertyHelper<uint>::fromString(str);
    }
    static bool stringToBool(const String& str)
    {
        return PropertyHelper<bool>::fromString(str);
    }
    static Sizef stringToSize(const String& str)
    {
        return PropertyHelper<Sizef >::fromString(str);
    }
    static Vector2f stringToVector2(const String& str)
    {
        return PropertyHelper<Vector2f >::fromString(str);
    }
    static Rectf stringToRect(const String& str)
    {
        return PropertyHelper<Rectf >::fromString(str);
    }
    static const Image*	stringToImage(const String& str)
    {
        return PropertyHelper<Image*>::fromString(str);
    }
    static Colour stringToColour(const String& str)
    {
        return PropertyHelper<Colour>::fromString(str);
    }
    static ColourRect stringToColourRect(const String& str)
    {
        return PropertyHelper<ColourRect>::fromString(str);
    }
    static UDim stringToUDim(const String& str)
    {
        return PropertyHelper<UDim>::fromString(str);
    }
    static UVector2 stringToUVector2(const String& str)
    {
        return PropertyHelper<UVector2>::fromString(str);
    }
    static URect stringToURect(const String& str)
    {
        return PropertyHelper<URect>::fromString(str);
    }
    static UBox stringToUBox(const String& str)
    {
        return PropertyHelper<UBox>::fromString(str);
    }

    static String floatToString(float val)
    {
        return PropertyHelper<float>::toString(val);
    }
    static String uintToString(unsigned int val)
    {
        return PropertyHelper<uint>::toString(val);
    }
    static String boolToString(bool val)
    {
        return PropertyHelper<bool>::toString(val);
    }
    static String sizeToString(const Sizef& val)
    {
        return PropertyHelper<Sizef >::toString(val);
    }
    static String vector2ToString(const Vector2f& val)
    {
        return PropertyHelper<Vector2f >::toString(val);
    }
    static String rectToString(const Rectf& val)
    {
        return PropertyHelper<Rectf >::toString(val);
    }
    static String imageToString(const Image* val)
    {
        return PropertyHelper<Image*>::toString(val);
    }
    static String colourToString(const Colour& val)
    {
        return PropertyHelper<Colour>::toString(val);
    }
    static String colourRectToString(const ColourRect& val)
    {
        return PropertyHelper<ColourRect>::toString(val);
    }
    static String udimToString(const UDim& val)
    {
        return PropertyHelper<UDim>::toString(val);
    }
    static String uvector2ToString(const UVector2& val)
    {
        return PropertyHelper<UVector2>::toString(val);
    }
    static String urectToString(const URect& val)
    {
        return PropertyHelper<URect>::toString(val);
    }
    static String uboxToString(const UBox& val)
    {
        return PropertyHelper<UBox>::toString(val);
    }
};


class FalagardXMLHelper_wrapper
{
public:
    static VerticalFormatting stringToVertFormat(const String& str)
    {
        return FalagardXMLHelper<VerticalFormatting>::fromString(str);
    }
    static HorizontalFormatting stringToHorzFormat(const String& str)
    {
        return FalagardXMLHelper<HorizontalFormatting>::fromString(str);
    }
    static VerticalAlignment stringToVertAlignment(const String& str)
    {
        return FalagardXMLHelper<VerticalAlignment>::fromString(str);
    }
    static HorizontalAlignment stringToHorzAlignment(const String& str)
    {
        return FalagardXMLHelper<HorizontalAlignment>::fromString(str);
    }
    static DimensionType stringToDimensionType(const String& str)
    {
        return FalagardXMLHelper<DimensionType>::fromString(str);
    }
    static VerticalTextFormatting stringToVertTextFormat(const String& str)
    {
        return FalagardXMLHelper<VerticalTextFormatting>::fromString(str);
    }
    static HorizontalTextFormatting stringToHorzTextFormat(const String& str)
    {
        return FalagardXMLHelper<HorizontalTextFormatting>::fromString(str);
    }
    static FontMetricType stringToFontMetricType(const String& str)
    {
        return FalagardXMLHelper<FontMetricType>::fromString(str);
    }
    static DimensionOperator stringToDimensionOperator(const String& str)
    {
        return FalagardXMLHelper<DimensionOperator>::fromString(str);
    }
    static FrameImageComponent stringToFrameImageComponent(const String& str)
    {
        return FalagardXMLHelper<FrameImageComponent>::fromString(str);
    }
    static ChildEventAction stringToChildEventAction(const String& str)
    {
        return FalagardXMLHelper<ChildEventAction>::fromString(str);
    }

    static String vertFormatToString(VerticalFormatting val)
    {
        return FalagardXMLHelper<VerticalFormatting>::toString(val);
    }
    static String horzFormatToString(HorizontalFormatting val)
    {
        return FalagardXMLHelper<HorizontalFormatting>::toString(val);
    }
    static String vertAlignmentToString(VerticalAlignment val)
    {
        return FalagardXMLHelper<VerticalAlignment>::toString(val);
    }
    static String horzAlignmentToString(HorizontalAlignment val)
    {
        return FalagardXMLHelper<HorizontalAlignment>::toString(val);
    }
    static String dimensionTypeToString(DimensionType val)
    {
        return FalagardXMLHelper<DimensionType>::toString(val);
    }
    static String vertTextFormatToString(VerticalTextFormatting val)
    {
        return FalagardXMLHelper<VerticalTextFormatting>::toString(val);
    }
    static String horzTextFormatToString(HorizontalTextFormatting val)
    {
        return FalagardXMLHelper<HorizontalTextFormatting>::toString(val);
    }
    static String fontMetricTypeToString(FontMetricType val)
    {
        return FalagardXMLHelper<FontMetricType>::toString(val);
    }
    static String dimensionOperatorToString(DimensionOperator val)
    {
        return FalagardXMLHelper<DimensionOperator>::toString(val);
    }
    static String frameImageComponentToString(FrameImageComponent val)
    {
        return FalagardXMLHelper<FrameImageComponent>::toString(val);
    }
    static String childEventActionToStringn(ChildEventAction val)
    {
        return FalagardXMLHelper<ChildEventAction>::toString(val);
    }
};

}

// First we create a magic namespace to hold all our aliases
namespace pyplusplus
{
    namespace aliases
    {
        typedef ::CEGUI::RefCounted< CEGUI::FormattedRenderedString > RefCountedFormattedRenderedString;

        typedef ::CEGUI::NamedXMLResourceManager<CEGUI::Font, CEGUI::Font_xmlHandler> NamedXMLResourceManagerFont;
        typedef ::CEGUI::NamedXMLResourceManager<CEGUI::Scheme, CEGUI::Scheme_xmlHandler> NamedXMLResourceManagerScheme;

        typedef ::CEGUI::Vector2<float> Vector2;
        typedef ::CEGUI::Vector3<float> Vector3;
        typedef ::CEGUI::Size<float> Size;
        typedef ::CEGUI::Rect<float> Rect;
        typedef ::CEGUI::Vector2<CEGUI::UDim> UVector2;
        typedef ::CEGUI::Rect<CEGUI::UDim> URect;
        typedef ::CEGUI::Size<CEGUI::UDim> USize;

        typedef std::vector<CEGUI::ListboxItem*
            CEGUI_VECTOR_ALLOC(CEGUI::ListboxItem*)> ListBoxItemPtrVector;
        // typedef std::vector<CEGUI::Scheme::AliasMapping> AliasMappingVector;
        typedef std::vector<CEGUI::PropertyInitialiser
            CEGUI_VECTOR_ALLOC(CEGUI::PropertyInitialiser)> PropertyInitialiserVector;
        // typedef std::vector<CEGUI::Scheme::FalagardMapping> FalagardMappingVector; 
        // typedef std::vector<CEGUI::Scheme::LoadableUIElement> LoadableUIElementVector;
        // typedef std::vector<CEGUI::Scheme::UIElementFactory> UIElementFactoryVector;
        // typedef std::vector<CEGUI::Scheme::UIModule> UIModuleVector;

        typedef ::CEGUI::Event::Connection Connection;
        typedef ::CEGUI::Event::Subscriber Subscriber;
        typedef ::CEGUI::EventSet::EventIterator EventIterator;

        typedef ::CEGUI::FontManager::FontIterator FontIterator;
        typedef ::CEGUI::ImageManager::ImageIterator ImageIterator;
        typedef ::CEGUI::PropertySet::PropertyIterator PropertyIterator;
        typedef ::CEGUI::Scheme::LoadableUIElementIterator LoadableUIElementIterator;
        typedef ::CEGUI::SchemeManager::SchemeIterator SchemeIterator;
        typedef ::CEGUI::WindowFactoryManager::WindowFactoryIterator WindowFactoryIterator;
        typedef ::CEGUI::WindowFactoryManager::TypeAliasIterator TypeAliasIterator;
        typedef ::CEGUI::WindowFactoryManager::FalagardMappingIterator FalagardMappingIterator;
        typedef ::CEGUI::WindowManager::WindowIterator WindowIterator;
        typedef ::CEGUI::WidgetLookFeel::PropertyDefinitionIterator PropertyDefinitionIterator;

        typedef std::pair<float, float> StdPairFloatFloat;
        typedef std::pair<CEGUI::String, CEGUI::String> StdPairCEGUIStringString;
        typedef std::pair<CEGUI::Image*, CEGUI::ImageFactory*> StdPairCEGUIImageImageFactory;

        typedef std::map<std::string, std::string
            CEGUI_MAP_ALLOC(std::string, std::string)> StringMap;
        typedef std::vector<std::string
            CEGUI_VECTOR_ALLOC(std::string)> StringVector;

        typedef CEGUI::Singleton<CEGUI::AnimationManager> SingletonAnimationManager;
        typedef CEGUI::Singleton<CEGUI::Logger> SingletonLogger;
        typedef CEGUI::Singleton<CEGUI::FontManager> SingletonFontManager;
        typedef CEGUI::Singleton<CEGUI::GlobalEventSet> SingletonGlobalEventSet;
        typedef CEGUI::Singleton<CEGUI::ImageManager> SingletonImageManager;
        typedef CEGUI::Singleton<CEGUI::MouseCursor> SingletonMouseCursor;
        typedef CEGUI::Singleton<CEGUI::RenderEffectManager> SingletonRenderEffectManager;
        typedef CEGUI::Singleton<CEGUI::SchemeManager> SingletonSchemeManager;
        typedef CEGUI::Singleton<CEGUI::System> SingletonSystem;
        typedef CEGUI::Singleton<CEGUI::WidgetLookManager> SingletonWidgetLookManager;
        typedef CEGUI::Singleton<CEGUI::WindowFactoryManager> SingletonWindowFactoryManager;
        typedef CEGUI::Singleton<CEGUI::WindowManager> SingletonWindowManager;
        typedef CEGUI::Singleton<CEGUI::WindowRendererManager> SingletonWindowRendererManager;

        typedef unsigned int Group;
        typedef std::map<CEGUI::String, CEGUI::String, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::String)> ResourceGroupMap;
        typedef CEGUI::RefCounted<CEGUI::BoundSlot> Connection;
        typedef CEGUI::SubscriberSlot Subscriber;
        typedef std::multimap<Group, Connection
            CEGUI_MULTIMAP_ALLOC(Group, Connection)> SlotContainer;
        typedef std::map<CEGUI::String, CEGUI::Event*, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::String)>	EventMap;
        typedef std::map<CEGUI::String, CEGUI::Font*, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Font*)> FontRegistry;
        typedef std::pair<CEGUI::Window*, bool> WindowStackEntry;
        typedef std::vector<WindowStackEntry
            CEGUI_VECTOR_ALLOC(WindowStackEntry)> WindowStack;
        typedef std::map<CEGUI::String, CEGUI::Property*, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Property*)> PropertyRegistry;
        typedef std::map<CEGUI::String, CEGUI::String, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::String)> UserStringMap;
        typedef std::set<CEGUI::String, CEGUI::StringFastLessCompare> StringSet;
        typedef std::vector<CEGUI::String
            CEGUI_VECTOR_ALLOC(CEGUI::String)> TargetTypeStack;		//!< Type used to implement stack of target type names.
        typedef std::map<CEGUI::String, CEGUI::Window*, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Window*)> WindowRegistry; //!< Type used to implement registry of Window objects
        typedef std::vector<CEGUI::Window*
            CEGUI_VECTOR_ALLOC(CEGUI::Window*)>    WindowVector;   //!< Type to use for a collection of Window pointers.
        typedef std::vector<CEGUI::Property*
            CEGUI_VECTOR_ALLOC(CEGUI::Property*)> PropertyList;
        typedef std::map<CEGUI::String, CEGUI::WindowRendererFactory*, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::WindowRendererFactory*)> WR_Registry;
        typedef std::map<CEGUI::String, CEGUI::String, CEGUI::StringFastLessCompare
            CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::String)> AttributeMap;
    }
}

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//

// this is a gcc-only workaround, don't do this at build time
#ifdef __GCCXML__

namespace python_CEGUI
{
    namespace details
    {
        inline void instantiate()
        {
            sizeof( ::std::pair<float, float>);
            sizeof( ::CEGUI::Event::Connection);
            sizeof( ::CEGUI::Event::Subscriber);

            sizeof( ::CEGUI::FontManager::FontIterator);
            sizeof( ::CEGUI::ImageManager::ImageIterator);
            sizeof( ::CEGUI::PropertySet::PropertyIterator);
            sizeof( ::CEGUI::Scheme::LoadableUIElementIterator);
            sizeof( ::CEGUI::SchemeManager::SchemeIterator);
            sizeof( ::CEGUI::WindowFactoryManager::WindowFactoryIterator);
            sizeof( ::CEGUI::WindowFactoryManager::TypeAliasIterator);
            sizeof( ::CEGUI::WindowFactoryManager::FalagardMappingIterator);
            sizeof( ::CEGUI::WindowManager::WindowIterator);
            sizeof( ::CEGUI::EventSet::EventIterator );
            sizeof( ::CEGUI::String::iterator );
            sizeof( ::CEGUI::Vector2< float > );
            sizeof( ::CEGUI::Vector3< float > );
            sizeof( ::CEGUI::Size< float > );
            sizeof( ::CEGUI::Rect< float > );
            sizeof( ::CEGUI::Vector2< CEGUI::UDim > );
            sizeof( ::CEGUI::Rect< CEGUI::UDim > );
            sizeof( ::CEGUI::Size< CEGUI::UDim > );

            sizeof( ::std::pair<float, float> );
            sizeof( ::std::pair<CEGUI::String, CEGUI::String> );
            sizeof( ::std::pair<CEGUI::Image*, CEGUI::ImageFactory*> );

            // seems that doing an sizeof ( ::CEGUI::EventSet::Iterator ); doesn't expose the class :(
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::Property*, CEGUI::StringFastLessCompare   CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Property*) > > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::Scheme*, CEGUI::StringFastLessCompare     CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Scheme*) > > );
            //sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::Window*, CEGUI::StringFastLessCompare     CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Window*) > > );
            sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::Window*  CEGUI_VECTOR_ALLOC(Window*)> >);
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::WindowFactoryManager::FalagardWindowMapping, CEGUI::StringFastLessCompare CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::WindowFactoryManager::FalagardWindowMapping) > > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::Event*, CEGUI::StringFastLessCompare      CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Event*) > > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::StringFastLessCompare CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack) > > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::WindowFactory*, CEGUI::StringFastLessCompare CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::WindowFactory*) > > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::Font*, CEGUI::StringFastLessCompare       CEGUI_MAP_ALLOC(CEGUI::String, CEGUI::Font*) > > );
            sizeof( CEGUI::ConstVectorIterator<std::multiset<CEGUI::LayerSpecification, std::less<CEGUI::LayerSpecification>  CEGUI_VECTOR_ALLOC(CEGUI::LayerSpecification) > > );
            sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::ImageryComponent  CEGUI_VECTOR_ALLOC(CEGUI::ImageryComponent) > > );
            sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::EventAction  CEGUI_VECTOR_ALLOC(CEGUI::EventAction) > > );
            sizeof( CEGUI::ConstVectorIterator<std::set<CEGUI::String, CEGUI::StringFastLessCompare, std::allocator<CEGUI::String> > > );
            sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::FrameComponent  CEGUI_VECTOR_ALLOC(CEGUI::FrameComponent) > > );
            sizeof( CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, const CEGUI::PropertyInitialiser*> > );
            sizeof( CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, CEGUI::PropertyDefinitionBase*> > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::ImagerySection, CEGUI::StringFastLessCompare  CEGUI_MAP_ALLOC(const CEGUI::String, CEGUI::ImagerySection) > > );
            //sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::PropertyInitialiser  CEGUI_VECTOR_ALLOC(CEGUI::PropertyInitialiser) > > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::NamedArea, CEGUI::StringFastLessCompare  CEGUI_MAP_ALLOC(const CEGUI::String, CEGUI::NamedArea) > > );
            sizeof( CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, const CEGUI::WidgetComponent*> > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::WidgetLookFeel, CEGUI::StringFastLessCompare  CEGUI_MAP_ALLOC(const CEGUI::String, CEGUI::WidgetLookFeel) > > );
            sizeof( CEGUI::ConstVectorIterator<CEGUI::NamedDefinitionCollator<CEGUI::String, const CEGUI::EventLinkDefinition*> > );
            sizeof( CEGUI::ConstMapIterator<std::map<CEGUI::String, CEGUI::StateImagery, CEGUI::StringFastLessCompare  CEGUI_MAP_ALLOC(const CEGUI::String, CEGUI::StateImagery) > > );
            sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::SectionSpecification  CEGUI_VECTOR_ALLOC(CEGUI::SectionSpecification) > > );
            sizeof( CEGUI::ConstVectorIterator<std::vector<CEGUI::TextComponent  CEGUI_VECTOR_ALLOC(CEGUI::TextComponent) > > );
        }
    }
}

#endif

