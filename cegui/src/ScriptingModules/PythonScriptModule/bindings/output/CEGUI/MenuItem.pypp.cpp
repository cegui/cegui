// This file has been generated by Py++.

#include "boost/python.hpp"
#include "python_CEGUI.h"
#include "MenuItem.pypp.hpp"

namespace bp = boost::python;

struct MenuItem_wrapper : CEGUI::MenuItem, bp::wrapper< CEGUI::MenuItem > {

    MenuItem_wrapper(::CEGUI::String const & type, ::CEGUI::String const & name )
    : CEGUI::MenuItem( boost::ref(type), boost::ref(name) )
      , bp::wrapper< CEGUI::MenuItem >(){
        // constructor
    
    }

    virtual void beginInitialisation(  ) {
        if( bp::override func_beginInitialisation = this->get_override( "beginInitialisation" ) )
            func_beginInitialisation(  );
        else{
            this->CEGUI::Window::beginInitialisation(  );
        }
    }
    
    void default_beginInitialisation(  ) {
        CEGUI::Window::beginInitialisation( );
    }

    virtual void cloneChildWidgetsTo( ::CEGUI::Window & target ) const  {
        if( bp::override func_cloneChildWidgetsTo = this->get_override( "cloneChildWidgetsTo" ) )
            func_cloneChildWidgetsTo( boost::ref(target) );
        else{
            this->CEGUI::Window::cloneChildWidgetsTo( boost::ref(target) );
        }
    }
    
    void default_cloneChildWidgetsTo( ::CEGUI::Window & target ) const  {
        CEGUI::Window::cloneChildWidgetsTo( boost::ref(target) );
    }

    virtual void clonePropertiesTo( ::CEGUI::Window & target ) const  {
        if( bp::override func_clonePropertiesTo = this->get_override( "clonePropertiesTo" ) )
            func_clonePropertiesTo( boost::ref(target) );
        else{
            this->CEGUI::Window::clonePropertiesTo( boost::ref(target) );
        }
    }
    
    void default_clonePropertiesTo( ::CEGUI::Window & target ) const  {
        CEGUI::Window::clonePropertiesTo( boost::ref(target) );
    }

    virtual void destroy(  ) {
        if( bp::override func_destroy = this->get_override( "destroy" ) )
            func_destroy(  );
        else{
            this->CEGUI::Window::destroy(  );
        }
    }
    
    void default_destroy(  ) {
        CEGUI::Window::destroy( );
    }

    virtual void endInitialisation(  ) {
        if( bp::override func_endInitialisation = this->get_override( "endInitialisation" ) )
            func_endInitialisation(  );
        else{
            this->CEGUI::Window::endInitialisation(  );
        }
    }
    
    void default_endInitialisation(  ) {
        CEGUI::Window::endInitialisation( );
    }

    virtual void fireEvent( ::CEGUI::String const & name, ::CEGUI::EventArgs & args, ::CEGUI::String const & eventNamespace="" ) {
        if( bp::override func_fireEvent = this->get_override( "fireEvent" ) )
            func_fireEvent( boost::ref(name), boost::ref(args), boost::ref(eventNamespace) );
        else{
            this->CEGUI::EventSet::fireEvent( boost::ref(name), boost::ref(args), boost::ref(eventNamespace) );
        }
    }
    
    void default_fireEvent( ::CEGUI::String const & name, ::CEGUI::EventArgs & args, ::CEGUI::String const & eventNamespace="" ) {
        CEGUI::EventSet::fireEvent( boost::ref(name), boost::ref(args), boost::ref(eventNamespace) );
    }

    virtual void getRenderingContext_impl( ::CEGUI::RenderingContext & ctx ) const  {
        if( bp::override func_getRenderingContext_impl = this->get_override( "getRenderingContext_impl" ) )
            func_getRenderingContext_impl( boost::ref(ctx) );
        else{
            this->CEGUI::Window::getRenderingContext_impl( boost::ref(ctx) );
        }
    }
    
    void default_getRenderingContext_impl( ::CEGUI::RenderingContext & ctx ) const  {
        CEGUI::Window::getRenderingContext_impl( boost::ref(ctx) );
    }

    virtual ::CEGUI::Rect getUnclippedInnerRect_impl(  ) const  {
        if( bp::override func_getUnclippedInnerRect_impl = this->get_override( "getUnclippedInnerRect_impl" ) )
            return func_getUnclippedInnerRect_impl(  );
        else{
            return this->CEGUI::Window::getUnclippedInnerRect_impl(  );
        }
    }
    
    ::CEGUI::Rect default_getUnclippedInnerRect_impl(  ) const  {
        return CEGUI::Window::getUnclippedInnerRect_impl( );
    }

    virtual void initialiseComponents(  ) {
        if( bp::override func_initialiseComponents = this->get_override( "initialiseComponents" ) )
            func_initialiseComponents(  );
        else{
            this->CEGUI::Window::initialiseComponents(  );
        }
    }
    
    void default_initialiseComponents(  ) {
        CEGUI::Window::initialiseComponents( );
    }

    virtual bool isHit( ::CEGUI::Vector2 const & position, bool const allow_disabled=false ) const  {
        if( bp::override func_isHit = this->get_override( "isHit" ) )
            return func_isHit( boost::ref(position), allow_disabled );
        else{
            return this->CEGUI::Window::isHit( boost::ref(position), allow_disabled );
        }
    }
    
    bool default_isHit( ::CEGUI::Vector2 const & position, bool const allow_disabled=false ) const  {
        return CEGUI::Window::isHit( boost::ref(position), allow_disabled );
    }

    virtual void performChildWindowLayout( bool nonclient_sized_hint=false, bool client_sized_hint=false ) {
        if( bp::override func_performChildWindowLayout = this->get_override( "performChildWindowLayout" ) )
            func_performChildWindowLayout( nonclient_sized_hint, client_sized_hint );
        else{
            this->CEGUI::Window::performChildWindowLayout( nonclient_sized_hint, client_sized_hint );
        }
    }
    
    void default_performChildWindowLayout( bool nonclient_sized_hint=false, bool client_sized_hint=false ) {
        CEGUI::Window::performChildWindowLayout( nonclient_sized_hint, client_sized_hint );
    }

    virtual void setLookNFeel( ::CEGUI::String const & look ) {
        if( bp::override func_setLookNFeel = this->get_override( "setLookNFeel" ) )
            func_setLookNFeel( boost::ref(look) );
        else{
            this->CEGUI::Window::setLookNFeel( boost::ref(look) );
        }
    }
    
    void default_setLookNFeel( ::CEGUI::String const & look ) {
        CEGUI::Window::setLookNFeel( boost::ref(look) );
    }

    virtual void setMargin( ::CEGUI::UBox const & margin ) {
        if( bp::override func_setMargin = this->get_override( "setMargin" ) )
            func_setMargin( boost::ref(margin) );
        else{
            this->CEGUI::Window::setMargin( boost::ref(margin) );
        }
    }
    
    void default_setMargin( ::CEGUI::UBox const & margin ) {
        CEGUI::Window::setMargin( boost::ref(margin) );
    }

    virtual ::CEGUI::RefCounted< CEGUI::BoundSlot > subscribeScriptedEvent( ::CEGUI::String const & name, ::CEGUI::String const & subscriber_name ) {
        if( bp::override func_subscribeScriptedEvent = this->get_override( "subscribeScriptedEvent" ) )
            return func_subscribeScriptedEvent( boost::ref(name), boost::ref(subscriber_name) );
        else{
            return this->CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), boost::ref(subscriber_name) );
        }
    }
    
    ::CEGUI::RefCounted< CEGUI::BoundSlot > default_subscribeScriptedEvent( ::CEGUI::String const & name, ::CEGUI::String const & subscriber_name ) {
        return CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), boost::ref(subscriber_name) );
    }

    virtual ::CEGUI::RefCounted< CEGUI::BoundSlot > subscribeScriptedEvent( ::CEGUI::String const & name, unsigned int group, ::CEGUI::String const & subscriber_name ) {
        if( bp::override func_subscribeScriptedEvent = this->get_override( "subscribeScriptedEvent" ) )
            return func_subscribeScriptedEvent( boost::ref(name), group, boost::ref(subscriber_name) );
        else{
            return this->CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), group, boost::ref(subscriber_name) );
        }
    }
    
    ::CEGUI::RefCounted< CEGUI::BoundSlot > default_subscribeScriptedEvent( ::CEGUI::String const & name, unsigned int group, ::CEGUI::String const & subscriber_name ) {
        return CEGUI::EventSet::subscribeScriptedEvent( boost::ref(name), group, boost::ref(subscriber_name) );
    }

    virtual void update( float elapsed ) {
        if( bp::override func_update = this->get_override( "update" ) )
            func_update( elapsed );
        else{
            this->CEGUI::Window::update( elapsed );
        }
    }
    
    void default_update( float elapsed ) {
        CEGUI::Window::update( elapsed );
    }

    virtual void writeXMLToStream( ::CEGUI::XMLSerializer & xml_stream ) const  {
        if( bp::override func_writeXMLToStream = this->get_override( "writeXMLToStream" ) )
            func_writeXMLToStream( boost::ref(xml_stream) );
        else{
            this->CEGUI::Window::writeXMLToStream( boost::ref(xml_stream) );
        }
    }
    
    void default_writeXMLToStream( ::CEGUI::XMLSerializer & xml_stream ) const  {
        CEGUI::Window::writeXMLToStream( boost::ref(xml_stream) );
    }

};

void register_MenuItem_class(){

    { //::CEGUI::MenuItem
        typedef bp::class_< MenuItem_wrapper, bp::bases< CEGUI::ItemEntry >, boost::noncopyable > MenuItem_exposer_t;
        MenuItem_exposer_t MenuItem_exposer = MenuItem_exposer_t( "MenuItem", "*!\n\
        \n\
            Base class for menu items.\n\
        *\n", bp::init< CEGUI::String const &, CEGUI::String const & >(( bp::arg("type"), bp::arg("name") ), "*!\n\
        \n\
            starts the opening timer for the popup, which will open it if the timer is enabled.\n\
        *\n\
        *************************************************************************\n\
            Construction and Destruction\n\
        *************************************************************************\n\
        *!\n\
        \n\
            Constructor for MenuItem objects\n\
        *\n") );
        bp::scope MenuItem_scope( MenuItem_exposer );
        { //::CEGUI::MenuItem::closePopupMenu
        
            typedef void ( ::CEGUI::MenuItem::*closePopupMenu_function_type )( bool ) ;
            
            MenuItem_exposer.def( 
                "closePopupMenu"
                , closePopupMenu_function_type( &::CEGUI::MenuItem::closePopupMenu )
                , ( bp::arg("notify")=(bool)(true) )
                , "*!\n\
                \n\
                    Closes the PopupMenu.\n\
            \n\
                @param notify\n\
                    true if the parent menubar (if any) is to handle the close.\n\
            \n\
                @return\n\
                    Nothing.\n\
                *\n" );
        
        }
        { //::CEGUI::MenuItem::getAutoPopupTimeout
        
            typedef float ( ::CEGUI::MenuItem::*getAutoPopupTimeout_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "getAutoPopupTimeout"
                , getAutoPopupTimeout_function_type( &::CEGUI::MenuItem::getAutoPopupTimeout )
                , "*!\n\
            \n\
                Returns the time, which has to elapse before the popup window is openedclosed if the hovering\
                state changes.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::getPopupMenu
        
            typedef ::CEGUI::PopupMenu * ( ::CEGUI::MenuItem::*getPopupMenu_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "getPopupMenu"
                , getPopupMenu_function_type( &::CEGUI::MenuItem::getPopupMenu )
                , bp::return_value_policy< bp::reference_existing_object >()
                , "*!\n\
                \n\
                    Get the PopupMenu that is currently attached to this MenuItem.\n\
            \n\
                @return\n\
                    A pointer to the currently attached PopupMenu.  Null is there is no PopupMenu attached.\n\
                *\n" );
        
        }
        { //::CEGUI::MenuItem::getPopupOffset
        
            typedef ::CEGUI::UVector2 const & ( ::CEGUI::MenuItem::*getPopupOffset_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "getPopupOffset"
                , getPopupOffset_function_type( &::CEGUI::MenuItem::getPopupOffset )
                , bp::return_value_policy< bp::copy_const_reference >()
                , "*!\n\
            \n\
                Returns the current offset for popup placement.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::hasAutoPopup
        
            typedef bool ( ::CEGUI::MenuItem::*hasAutoPopup_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "hasAutoPopup"
                , hasAutoPopup_function_type( &::CEGUI::MenuItem::hasAutoPopup )
                , "*!\n\
            \n\
                Returns true if the menu item popup is closed or opened automatically if hovering with the\
                mouse.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::isHovering
        
            typedef bool ( ::CEGUI::MenuItem::*isHovering_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "isHovering"
                , isHovering_function_type( &::CEGUI::MenuItem::isHovering )
                , "*************************************************************************\n\
                    Accessor type functions\n\
                *************************************************************************\n\
                *!\n\
                \n\
                    return true if user is hovering over this widget (or it's pushed and user is not over it for\
                    highlight)\n\
            \n\
                @return\n\
                    true if the user is hovering or if the button is pushed and the mouse is not over the\
                    button.  Otherwise return false.\n\
                *\n" );
        
        }
        { //::CEGUI::MenuItem::isOpened
        
            typedef bool ( ::CEGUI::MenuItem::*isOpened_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "isOpened"
                , isOpened_function_type( &::CEGUI::MenuItem::isOpened )
                , "*!\n\
            \n\
                Returns true if the popup menu attached to the menu item is open.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::isPopupClosing
        
            typedef bool ( ::CEGUI::MenuItem::*isPopupClosing_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "isPopupClosing"
                , isPopupClosing_function_type( &::CEGUI::MenuItem::isPopupClosing )
                , "*!\n\
            \n\
                Returns true if the menu item popup is closing or not.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::isPushed
        
            typedef bool ( ::CEGUI::MenuItem::*isPushed_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "isPushed"
                , isPushed_function_type( &::CEGUI::MenuItem::isPushed )
                , "*!\n\
                \n\
                    Return true if the button widget is in the pushed state.\n\
            \n\
                @return\n\
                    true if the button-type widget is pushed, false if the widget is not pushed.\n\
                *\n" );
        
        }
        { //::CEGUI::MenuItem::openPopupMenu
        
            typedef void ( ::CEGUI::MenuItem::*openPopupMenu_function_type )( bool ) ;
            
            MenuItem_exposer.def( 
                "openPopupMenu"
                , openPopupMenu_function_type( &::CEGUI::MenuItem::openPopupMenu )
                , ( bp::arg("notify")=(bool)(true) )
                , "*!\n\
                \n\
                    Opens the PopupMenu.\n\
            \n\
                @param notify\n\
                    true if the parent menu bar or menu popup (if any) is to handle the open.\n\
                *\n" );
        
        }
        { //::CEGUI::MenuItem::setAutoPopupTimeout
        
            typedef void ( ::CEGUI::MenuItem::*setAutoPopupTimeout_function_type )( float ) ;
            
            MenuItem_exposer.def( 
                "setAutoPopupTimeout"
                , setAutoPopupTimeout_function_type( &::CEGUI::MenuItem::setAutoPopupTimeout )
                , ( bp::arg("time") )
                , "*!\n\
            \n\
                Sets the time, which has to elapse before the popup window is openedclosed if the hovering state\
                changes.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::setPopupMenu
        
            typedef void ( ::CEGUI::MenuItem::*setPopupMenu_function_type )( ::CEGUI::PopupMenu * ) ;
            
            MenuItem_exposer.def( 
                "setPopupMenu"
                , setPopupMenu_function_type( &::CEGUI::MenuItem::setPopupMenu )
                , ( bp::arg("popup") )
                , "*************************************************************************\n\
                    Manipulators\n\
                *************************************************************************\n\
                *!\n\
                \n\
                    Set the popup menu for this item.\n\
            \n\
                @param popup\n\
                    popupmenu window to attach to this item\n\
            \n\
                @return\n\
                    Nothing.\n\
                *\n" );
        
        }
        { //::CEGUI::MenuItem::setPopupOffset
        
            typedef void ( ::CEGUI::MenuItem::*setPopupOffset_function_type )( ::CEGUI::UVector2 const & ) ;
            
            MenuItem_exposer.def( 
                "setPopupOffset"
                , setPopupOffset_function_type( &::CEGUI::MenuItem::setPopupOffset )
                , ( bp::arg("popupOffset") )
                , "*!\n\
            \n\
                sets the current offset for popup placement.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::startPopupClosing
        
            typedef void ( ::CEGUI::MenuItem::*startPopupClosing_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "startPopupClosing"
                , startPopupClosing_function_type( &::CEGUI::MenuItem::startPopupClosing )
                , "*!\n\
            \n\
                starts the closing timer for the popup, which will close it if the timer is enabled.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::startPopupOpening
        
            typedef void ( ::CEGUI::MenuItem::*startPopupOpening_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "startPopupOpening"
                , startPopupOpening_function_type( &::CEGUI::MenuItem::startPopupOpening )
                , "*!\n\
            \n\
                starts the opening timer for the popup, which will open it if the timer is enabled.\n\
            *\n" );
        
        }
        { //::CEGUI::MenuItem::togglePopupMenu
        
            typedef bool ( ::CEGUI::MenuItem::*togglePopupMenu_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "togglePopupMenu"
                , togglePopupMenu_function_type( &::CEGUI::MenuItem::togglePopupMenu )
                , "*!\n\
                \n\
                    Toggles the PopupMenu.\n\
            \n\
                @return\n\
                    true if the popup was opened. false if it was closed.\n\
                *\n" );
        
        }
        MenuItem_exposer.add_static_property( "EventClicked"
                        , bp::make_getter( &CEGUI::MenuItem::EventClicked
                                , bp::return_value_policy< bp::return_by_value >() ) );
        { //::CEGUI::Window::beginInitialisation
        
            typedef void ( ::CEGUI::Window::*beginInitialisation_function_type )(  ) ;
            typedef void ( MenuItem_wrapper::*default_beginInitialisation_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "beginInitialisation"
                , beginInitialisation_function_type(&::CEGUI::Window::beginInitialisation)
                , default_beginInitialisation_function_type(&MenuItem_wrapper::default_beginInitialisation) );
        
        }
        { //::CEGUI::Window::cloneChildWidgetsTo
        
            typedef void ( ::CEGUI::Window::*cloneChildWidgetsTo_function_type )( ::CEGUI::Window & ) const;
            typedef void ( MenuItem_wrapper::*default_cloneChildWidgetsTo_function_type )( ::CEGUI::Window & ) const;
            
            MenuItem_exposer.def( 
                "cloneChildWidgetsTo"
                , cloneChildWidgetsTo_function_type(&::CEGUI::Window::cloneChildWidgetsTo)
                , default_cloneChildWidgetsTo_function_type(&MenuItem_wrapper::default_cloneChildWidgetsTo)
                , ( bp::arg("target") ) );
        
        }
        { //::CEGUI::Window::clonePropertiesTo
        
            typedef void ( ::CEGUI::Window::*clonePropertiesTo_function_type )( ::CEGUI::Window & ) const;
            typedef void ( MenuItem_wrapper::*default_clonePropertiesTo_function_type )( ::CEGUI::Window & ) const;
            
            MenuItem_exposer.def( 
                "clonePropertiesTo"
                , clonePropertiesTo_function_type(&::CEGUI::Window::clonePropertiesTo)
                , default_clonePropertiesTo_function_type(&MenuItem_wrapper::default_clonePropertiesTo)
                , ( bp::arg("target") ) );
        
        }
        { //::CEGUI::Window::destroy
        
            typedef void ( ::CEGUI::Window::*destroy_function_type )(  ) ;
            typedef void ( MenuItem_wrapper::*default_destroy_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "destroy"
                , destroy_function_type(&::CEGUI::Window::destroy)
                , default_destroy_function_type(&MenuItem_wrapper::default_destroy) );
        
        }
        { //::CEGUI::Window::endInitialisation
        
            typedef void ( ::CEGUI::Window::*endInitialisation_function_type )(  ) ;
            typedef void ( MenuItem_wrapper::*default_endInitialisation_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "endInitialisation"
                , endInitialisation_function_type(&::CEGUI::Window::endInitialisation)
                , default_endInitialisation_function_type(&MenuItem_wrapper::default_endInitialisation) );
        
        }
        { //::CEGUI::EventSet::fireEvent
        
            typedef void ( ::CEGUI::EventSet::*fireEvent_function_type )( ::CEGUI::String const &,::CEGUI::EventArgs &,::CEGUI::String const & ) ;
            typedef void ( MenuItem_wrapper::*default_fireEvent_function_type )( ::CEGUI::String const &,::CEGUI::EventArgs &,::CEGUI::String const & ) ;
            
            MenuItem_exposer.def( 
                "fireEvent"
                , fireEvent_function_type(&::CEGUI::EventSet::fireEvent)
                , default_fireEvent_function_type(&MenuItem_wrapper::default_fireEvent)
                , ( bp::arg("name"), bp::arg("args"), bp::arg("eventNamespace")="" ) );
        
        }
        { //::CEGUI::Window::getRenderedStringParser
        
            typedef ::CEGUI::RenderedStringParser & ( ::CEGUI::Window::*getRenderedStringParser_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "getRenderedStringParser"
                , getRenderedStringParser_function_type(&::CEGUI::Window::getRenderedStringParser)
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::CEGUI::Window::getRenderingContext_impl
        
            typedef void ( ::CEGUI::Window::*getRenderingContext_impl_function_type )( ::CEGUI::RenderingContext & ) const;
            typedef void ( MenuItem_wrapper::*default_getRenderingContext_impl_function_type )( ::CEGUI::RenderingContext & ) const;
            
            MenuItem_exposer.def( 
                "getRenderingContext_impl"
                , getRenderingContext_impl_function_type(&::CEGUI::Window::getRenderingContext_impl)
                , default_getRenderingContext_impl_function_type(&MenuItem_wrapper::default_getRenderingContext_impl)
                , ( bp::arg("ctx") ) );
        
        }
        { //::CEGUI::Window::getUnclippedInnerRect_impl
        
            typedef ::CEGUI::Rect ( ::CEGUI::Window::*getUnclippedInnerRect_impl_function_type )(  ) const;
            typedef ::CEGUI::Rect ( MenuItem_wrapper::*default_getUnclippedInnerRect_impl_function_type )(  ) const;
            
            MenuItem_exposer.def( 
                "getUnclippedInnerRect_impl"
                , getUnclippedInnerRect_impl_function_type(&::CEGUI::Window::getUnclippedInnerRect_impl)
                , default_getUnclippedInnerRect_impl_function_type(&MenuItem_wrapper::default_getUnclippedInnerRect_impl) );
        
        }
        { //::CEGUI::Window::initialiseComponents
        
            typedef void ( ::CEGUI::Window::*initialiseComponents_function_type )(  ) ;
            typedef void ( MenuItem_wrapper::*default_initialiseComponents_function_type )(  ) ;
            
            MenuItem_exposer.def( 
                "initialiseComponents"
                , initialiseComponents_function_type(&::CEGUI::Window::initialiseComponents)
                , default_initialiseComponents_function_type(&MenuItem_wrapper::default_initialiseComponents) );
        
        }
        { //::CEGUI::Window::isHit
        
            typedef bool ( ::CEGUI::Window::*isHit_function_type )( ::CEGUI::Vector2 const &,bool const ) const;
            typedef bool ( MenuItem_wrapper::*default_isHit_function_type )( ::CEGUI::Vector2 const &,bool const ) const;
            
            MenuItem_exposer.def( 
                "isHit"
                , isHit_function_type(&::CEGUI::Window::isHit)
                , default_isHit_function_type(&MenuItem_wrapper::default_isHit)
                , ( bp::arg("position"), bp::arg("allow_disabled")=(bool const)(false) ) );
        
        }
        { //::CEGUI::Window::performChildWindowLayout
        
            typedef void ( ::CEGUI::Window::*performChildWindowLayout_function_type )( bool,bool ) ;
            typedef void ( MenuItem_wrapper::*default_performChildWindowLayout_function_type )( bool,bool ) ;
            
            MenuItem_exposer.def( 
                "performChildWindowLayout"
                , performChildWindowLayout_function_type(&::CEGUI::Window::performChildWindowLayout)
                , default_performChildWindowLayout_function_type(&MenuItem_wrapper::default_performChildWindowLayout)
                , ( bp::arg("nonclient_sized_hint")=(bool)(false), bp::arg("client_sized_hint")=(bool)(false) ) );
        
        }
        { //::CEGUI::Window::setLookNFeel
        
            typedef void ( ::CEGUI::Window::*setLookNFeel_function_type )( ::CEGUI::String const & ) ;
            typedef void ( MenuItem_wrapper::*default_setLookNFeel_function_type )( ::CEGUI::String const & ) ;
            
            MenuItem_exposer.def( 
                "setLookNFeel"
                , setLookNFeel_function_type(&::CEGUI::Window::setLookNFeel)
                , default_setLookNFeel_function_type(&MenuItem_wrapper::default_setLookNFeel)
                , ( bp::arg("look") ) );
        
        }
        { //::CEGUI::Window::setMargin
        
            typedef void ( ::CEGUI::Window::*setMargin_function_type )( ::CEGUI::UBox const & ) ;
            typedef void ( MenuItem_wrapper::*default_setMargin_function_type )( ::CEGUI::UBox const & ) ;
            
            MenuItem_exposer.def( 
                "setMargin"
                , setMargin_function_type(&::CEGUI::Window::setMargin)
                , default_setMargin_function_type(&MenuItem_wrapper::default_setMargin)
                , ( bp::arg("margin") ) );
        
        }
        { //::CEGUI::EventSet::subscribeScriptedEvent
        
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( ::CEGUI::EventSet::*subscribeScriptedEvent_function_type )( ::CEGUI::String const &,::CEGUI::String const & ) ;
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( MenuItem_wrapper::*default_subscribeScriptedEvent_function_type )( ::CEGUI::String const &,::CEGUI::String const & ) ;
            
            MenuItem_exposer.def( 
                "subscribeScriptedEvent"
                , subscribeScriptedEvent_function_type(&::CEGUI::EventSet::subscribeScriptedEvent)
                , default_subscribeScriptedEvent_function_type(&MenuItem_wrapper::default_subscribeScriptedEvent)
                , ( bp::arg("name"), bp::arg("subscriber_name") ) );
        
        }
        { //::CEGUI::EventSet::subscribeScriptedEvent
        
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( ::CEGUI::EventSet::*subscribeScriptedEvent_function_type )( ::CEGUI::String const &,unsigned int,::CEGUI::String const & ) ;
            typedef ::CEGUI::RefCounted< CEGUI::BoundSlot > ( MenuItem_wrapper::*default_subscribeScriptedEvent_function_type )( ::CEGUI::String const &,unsigned int,::CEGUI::String const & ) ;
            
            MenuItem_exposer.def( 
                "subscribeScriptedEvent"
                , subscribeScriptedEvent_function_type(&::CEGUI::EventSet::subscribeScriptedEvent)
                , default_subscribeScriptedEvent_function_type(&MenuItem_wrapper::default_subscribeScriptedEvent)
                , ( bp::arg("name"), bp::arg("group"), bp::arg("subscriber_name") ) );
        
        }
        { //::CEGUI::Window::update
        
            typedef void ( ::CEGUI::Window::*update_function_type )( float ) ;
            typedef void ( MenuItem_wrapper::*default_update_function_type )( float ) ;
            
            MenuItem_exposer.def( 
                "update"
                , update_function_type(&::CEGUI::Window::update)
                , default_update_function_type(&MenuItem_wrapper::default_update)
                , ( bp::arg("elapsed") ) );
        
        }
        { //::CEGUI::Window::writeXMLToStream
        
            typedef void ( ::CEGUI::Window::*writeXMLToStream_function_type )( ::CEGUI::XMLSerializer & ) const;
            typedef void ( MenuItem_wrapper::*default_writeXMLToStream_function_type )( ::CEGUI::XMLSerializer & ) const;
            
            MenuItem_exposer.def( 
                "writeXMLToStream"
                , writeXMLToStream_function_type(&::CEGUI::Window::writeXMLToStream)
                , default_writeXMLToStream_function_type(&MenuItem_wrapper::default_writeXMLToStream)
                , ( bp::arg("xml_stream") ) );
        
        }
    }

}