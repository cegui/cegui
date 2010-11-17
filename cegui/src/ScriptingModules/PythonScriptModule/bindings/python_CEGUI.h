#include <vector>
#include <string>
#include <map>
#include <iostream>

#include "CEGUI.h"
#include "falagard/CEGUIFalXMLEnumHelper.h"

// First we create a magic namespace to hold all our aliases
namespace pyplusplus
{
	namespace aliases
	{
        typedef ::CEGUI::NamedXMLResourceManager< CEGUI::Font, CEGUI::Font_xmlHandler >    NamedXMLResourceManagerFont;
        typedef ::CEGUI::RefCounted< CEGUI::FormattedRenderedString > RefCountedFormattedRenderedString;
        typedef ::CEGUI::NamedXMLResourceManager< CEGUI::Imageset, CEGUI::Imageset_xmlHandler >  NamedXMLResourceManagerImageset;
        typedef ::CEGUI::NamedXMLResourceManager< CEGUI::Scheme, CEGUI::Scheme_xmlHandler >  NamedXMLResourceManagerScheme;
        
        typedef std::vector<CEGUI::ListboxItem*> ListBoxItemPtrVector;
        // typedef std::vector<CEGUI::Scheme::AliasMapping> AliasMappingVector;
        typedef std::vector<CEGUI::PropertyInitialiser> PropertyInitialiserVector;
        // typedef std::vector<CEGUI::Scheme::FalagardMapping> FalagardMappingVector; 
        // typedef std::vector<CEGUI::Scheme::LoadableUIElement> LoadableUIElementVector;
        // typedef std::vector<CEGUI::Scheme::UIElementFactory> UIElementFactoryVector;
        // typedef std::vector<CEGUI::Scheme::UIModule> UIModuleVector;
        
        /*
        #if ( CEGUI_VERSION_MINOR > 5 )
         typedef std::list<CEGUI::OgreCEGUITexture*> OgreCEGUITextureList;
        #endif
        */
        
        typedef ::CEGUI::Event::Connection Connection;
        typedef ::CEGUI::Event::Subscriber Subscriber;
        typedef ::CEGUI::EventSet::Iterator EventIterator;
        
        
        typedef ::CEGUI::FontManager::FontIterator FontIterator ;
        typedef ::CEGUI::Imageset::ImageIterator ImageIterator;	
        typedef ::CEGUI::ImagesetManager::ImagesetIterator ImagesetIterator;
        typedef ::CEGUI::PropertySet::Iterator PropertyIterator;
        typedef ::CEGUI::SchemeManager::SchemeIterator SchemeIterator;
        typedef ::CEGUI::WindowFactoryManager::WindowFactoryIterator WindowFactoryIterator;
        typedef ::CEGUI::WindowFactoryManager::TypeAliasIterator TypeAliasIterator;
        typedef ::CEGUI::WindowFactoryManager::FalagardMappingIterator FalagardMappingIterator;
        typedef ::CEGUI::WindowManager::WindowIterator WindowIterator;
        
        typedef std::pair<float, float> StdPairFloatFloat;
        typedef std::map<std::string, std::string> StringMap;
        typedef std::vector<std::string> StringVector;
        
        typedef CEGUI::Singleton<CEGUI::AnimationManager> SingletonAnimationManager;   
        typedef CEGUI::Singleton<CEGUI::Logger> SingletonLogger;
        typedef CEGUI::Singleton<CEGUI::FontManager> SingletonFontManager;
        typedef CEGUI::Singleton<CEGUI::GlobalEventSet> SingletonGlobalEventSet;
        typedef CEGUI::Singleton<CEGUI::ImagesetManager> SingletonImagesetManager;
        typedef CEGUI::Singleton<CEGUI::MouseCursor> SingletonMouseCursor;
        typedef CEGUI::Singleton<CEGUI::RenderEffectManager> SingletonRenderEffectManager; 
        typedef CEGUI::Singleton<CEGUI::SchemeManager> SingletonSchemeManager;
        typedef CEGUI::Singleton<CEGUI::System> SingletonSystem;
        typedef CEGUI::Singleton<CEGUI::WidgetLookManager> SingletonWidgetLookManager;
        typedef CEGUI::Singleton<CEGUI::WindowFactoryManager> SingletonWindowFactoryManager;
        typedef CEGUI::Singleton<CEGUI::WindowManager> SingletonWindowManager;
        typedef CEGUI::Singleton<CEGUI::WindowRendererManager> SingletonWindowRendererManager;
        
        typedef unsigned int Group;
        typedef std::map<CEGUI::String, CEGUI::String, CEGUI::String::FastLessCompare> ResourceGroupMap;
        typedef CEGUI::RefCounted<CEGUI::BoundSlot> Connection;
        typedef CEGUI::SubscriberSlot Subscriber;
        typedef std::multimap<Group, Connection> SlotContainer;
        typedef std::map<CEGUI::String, CEGUI::Event*, CEGUI::String::FastLessCompare>	EventMap;
        typedef std::map<CEGUI::String, CEGUI::Font*, CEGUI::String::FastLessCompare> FontRegistry;
        typedef std::vector<CEGUI::Imageset*> ImagesetVector;
        typedef std::pair<CEGUI::Window*,bool> WindowStackEntry; 
        typedef std::vector<WindowStackEntry> WindowStack;
        typedef std::map<CEGUI::String, CEGUI::Property*, CEGUI::String::FastLessCompare>	PropertyRegistry;
        typedef std::map<CEGUI::String, CEGUI::String, CEGUI::String::FastLessCompare>   UserStringMap;
        typedef std::set<CEGUI::String, CEGUI::String::FastLessCompare> BannedXMLPropertySet;
        typedef std::vector<CEGUI::String>	TargetTypeStack;		//!< Type used to implement stack of target type names.
        typedef std::map<CEGUI::String, CEGUI::Window*, CEGUI::String::FastLessCompare>			WindowRegistry;				//!< Type used to implement registry of Window objects
        typedef std::vector<CEGUI::Window*>    WindowVector;   //!< Type to use for a collection of Window pointers.
        typedef std::vector<CEGUI::Property*> PropertyList;
        typedef std::map<CEGUI::String, CEGUI::WindowRendererFactory*, CEGUI::String::FastLessCompare> WR_Registry;
        typedef std::map<CEGUI::String, CEGUI::String, CEGUI::String::FastLessCompare> AttributeMap;
	}
} 

// then we exposed everything needed (and more) to ensure GCCXML makes them visible to Py++
//
namespace python_CEGUI
{
	namespace details
	{
		inline void instantiate()
		{
            sizeof ( ::std::pair<float, float> );
            sizeof ( ::CEGUI::Event::Connection ) ;
            sizeof ( ::CEGUI::Event::Subscriber) ;
            
            sizeof ( ::CEGUI::FontManager::FontIterator);
            sizeof ( ::CEGUI::Imageset::ImageIterator);	
            sizeof ( ::CEGUI::ImagesetManager::ImagesetIterator);
            sizeof ( ::CEGUI::PropertySet::Iterator);
            sizeof ( ::CEGUI::SchemeManager::SchemeIterator);
            sizeof ( ::CEGUI::WindowFactoryManager::WindowFactoryIterator);
            sizeof ( ::CEGUI::WindowFactoryManager::TypeAliasIterator);
            sizeof ( ::CEGUI::WindowFactoryManager::FalagardMappingIterator);
            sizeof ( ::CEGUI::WindowManager::WindowIterator);
            sizeof ( ::CEGUI::EventSet::Iterator );
            sizeof ( ::CEGUI::String::iterator );
            
            sizeof ( ::std::pair<float, float> );
            
            // seems that doing an sizeof ( ::CEGUI::EventSet::Iterator ); doesn't expose the class :(
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Imageset*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Imageset*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Property*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Property*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Scheme*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Scheme*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Window*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Window*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::WindowFactoryManager::FalagardWindowMapping, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::WindowFactoryManager::FalagardWindowMapping> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Event*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Event*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::WindowFactoryManager::AliasTargetStack> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::WindowFactory*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::WindowFactory*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Font*, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Font*> > > > );
            sizeof(  CEGUI::ConstBaseIterator<std::map<CEGUI::String, CEGUI::Image, CEGUI::String::FastLessCompare, std::allocator<std::pair<const CEGUI::String, CEGUI::Image> > > > );

		}
	}
}
