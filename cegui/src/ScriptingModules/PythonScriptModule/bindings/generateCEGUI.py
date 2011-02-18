#!/usr/bin/env python

#/***********************************************************************
#    filename:   generateCEGUI.py
#    created:    13/8/2010
#    author:     Martin Preisler (with many bits taken from python ogre)
#
#    purpose:    Generates CEGUI binding code
#*************************************************************************/
#/***************************************************************************
# *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
# *
# *   Thanks to Roman Yakovenko for advices and great work on Py++!
# *   Thanks to Andy Miller for his python-ogre CEGUI bindings!
# *
# *   License: generator is GPL3 (python ogre code generators are also GPL)
# *            generated code is MIT as the rest of CEGUI
 
import os

from pyplusplus import function_transformers as ft
from pyplusplus import messages
from pyplusplus.module_builder import call_policies
from pygccxml import declarations

import commonUtils

PACKAGE_NAME = "PyCEGUI"
PACKAGE_VERSION = commonUtils.GLOBAL_PACKAGE_VERSION
MODULE_NAME = PACKAGE_NAME

OUTPUT_DIR = os.path.join(commonUtils.OUTPUT_DIR, "CEGUI")

def filterDeclarations(mb):
    # by default we exclude everything and only include what we WANT in the module
    mb.global_ns.exclude()
    
    # todo: python ogre does this, why?
    mb.global_ns.namespace("std").class_("pair<float, float>").include()
    
    CEGUI_ns = mb.global_ns.namespace("CEGUI")
    
    ### CORE ###
    
    # CEGUIAffector.h
    affector = CEGUI_ns.class_("Affector")
    affector.include()
    
    # CEGUIAnimation.h
    animation = CEGUI_ns.class_("Animation")
    animation.include()
    
    # CEGUIAnimation_xmlHandler.h
    # not interesting for python users
    
    # CEGUIAnimationInstance.h
    animationInstance = CEGUI_ns.class_("AnimationInstance")
    animationInstance.include()
    
    # CEGUIAnimationManager.h
    animationManager = CEGUI_ns.class_("AnimationManager")
    animationManager.include()
    
    # CEGUIBase.h
    # nothing interesting for python users
    # todo: maybe pixel align?

    # CEGUIBasicImage.h
    basicImage = CEGUI_ns.class_("BasicImage")
    basicImage.include()   
    
    # CEGUIBasicInterpolators.h
    # nothing interesting for python users
    
    # CEGUIBasicRenderedStringParser.h
    # todo: is this interesting for python?
    basicRenderedStringParser = CEGUI_ns.class_("BasicRenderedStringParser")
    basicRenderedStringParser.include()
    
    # CEGUIBiDiVisualMapping.h
    CEGUI_ns.enum("BidiCharType").include()
    bidiVisualMapping = CEGUI_ns.class_("BidiVisualMapping")
    bidiVisualMapping.include()
    
    # CEGUIBoundSlot.h
    boundSlot = CEGUI_ns.class_("BoundSlot")
    boundSlot.include()
    # also include ref counted variant
    CEGUI_ns.class_("RefCounted<CEGUI::BoundSlot>").include()
    
    # CEGUICentredRenderedString.h
    # todo: is this interesting for python?
    centredRenderedString = CEGUI_ns.class_("CentredRenderedString")
    centredRenderedString.include()
    
    # CEGUIChainedXMLHandler.h
    #chainedXMLHandler = CEGUI_ns.class_("ChainedXMLHandler")
    #chainedXMLHandler.include()
    
    # CEGUIColour.h
    colour = CEGUI_ns.class_("Colour")
    colour.include()
    
    # CEGUIColourRect.h
    colourRect = CEGUI_ns.class_("ColourRect")
    colourRect.include()
    
    # CEGUIConfig.h
    # nothing interesting for python
    
    # CEGUIConfig_xmlHandler.h
    # nothing interesting for python
    
    # CEGUICoordConverter.h
    coordConverter = CEGUI_ns.class_("CoordConverter")
    coordConverter.include()
    
    # CEGUIDataContainer.h
    rawDataContainer = CEGUI_ns.class_("RawDataContainer")
    rawDataContainer.include()
    rawDataContainer.mem_funs().exclude()
    
    # CEGUIDefaultLogger.h
    defaultLogger = CEGUI_ns.class_("DefaultLogger")
    defaultLogger.include()
    
    # CEGUIDefaultRenderedStringParser.h
    # not interesting for python
    
    # CEGUIDefaultResourceProvider.h
    defaultResourceProvider = CEGUI_ns.class_("DefaultResourceProvider")
    defaultResourceProvider.include()
    
    # CEGUIDynamicModule.h
    # not doable in python
    
    # CEGUIEvent.h
    event = CEGUI_ns.class_("Event")
    event.include()
    
    # CEGUIEventArgs.h
    # handled separately, all classes ending with "EventArgs" are included
    
    # CEGUIEventSet.h
    eventSet = CEGUI_ns.class_("EventSet")
    eventSet.include()
    # this is done via custom code
    eventSet.mem_funs("subscribeEvent").exclude()
    eventSet.noncopyable = True
    eventSet.add_declaration_code(
"""
class EventCallback
{
public:
    EventCallback() : mSubscriber(0) {}

    EventCallback(PyObject* subscriber, const CEGUI::String& method):
        mSubscriber(boost::python::incref(subscriber)),
        mMethod(method)
    {} 
    
    EventCallback(const EventCallback& other):
        mSubscriber(boost::python::incref(other.mSubscriber)),
        mMethod(other.mMethod)
    {}
    
    ~EventCallback()
    {
        boost::python::decref(mSubscriber);
    } 
    
    /*void setsubscriber( PyObject* subscriber )
    {
        mSubscriber = subscriber;
    }*/
    
    bool operator() (const CEGUI::EventArgs &args) const
    {
        if (dynamic_cast<CEGUI::MouseCursorEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::MouseCursorEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::MouseCursorEventArgs&>(args) );
            
        else if (dynamic_cast<CEGUI::KeyEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::KeyEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::KeyEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::ActivationEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::ActivationEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,  
                                        static_cast<const CEGUI::ActivationEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::DragDropEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::DragDropEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,  
                                        static_cast<const CEGUI::DragDropEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::HeaderSequenceEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::HeaderSequenceEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,
                                        static_cast<const CEGUI::HeaderSequenceEventArgs&>(args) );
        
        else if (dynamic_cast<CEGUI::MouseEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::MouseEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::MouseEventArgs&>(args) );

        else if (dynamic_cast<CEGUI::DisplayEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(),
                                        static_cast<const CEGUI::DisplayEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,
                                        static_cast<const CEGUI::DisplayEventArgs&>(args) );

        else if (dynamic_cast<CEGUI::ResourceEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(),
                                        static_cast<const CEGUI::ResourceEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,
                                        static_cast<const CEGUI::ResourceEventArgs&>(args) );

         else if (dynamic_cast<CEGUI::TreeEventArgs *>((CEGUI::EventArgs *)&args))
           if (mMethod.length() > 0 )
               boost::python::call_method<void>(mSubscriber, mMethod.c_str(),
                                        static_cast<const CEGUI::TreeEventArgs&>(args) );
           else
               boost::python::call<void>(mSubscriber, 
                                        static_cast<const CEGUI::TreeEventArgs&>(args) );

        else if (dynamic_cast<CEGUI::WindowEventArgs *>((CEGUI::EventArgs *)&args))
            if (mMethod.length() > 0 )
                boost::python::call_method<void>(mSubscriber, mMethod.c_str(), 
                                        static_cast<const CEGUI::WindowEventArgs&>(args) );
            else
                boost::python::call<void>(mSubscriber,  
                                        static_cast<const CEGUI::WindowEventArgs&>(args) );

        else 
            boost::python::call_method<void>(mSubscriber, mMethod.c_str(), args );
     return true;
    }

    PyObject* mSubscriber;
    CEGUI::String mMethod;
};

class EventConnection
{
public:
    EventConnection():
        mConnection(0),
        mValid(false)
    {}
    
    EventConnection(const CEGUI::Event::Connection &connection) 
    : mConnection(connection), mValid(true) 
    {}
    
    ~EventConnection()
    {
        delete mCallback;
    }   
    bool connected()
    {
        return mValid && mConnection->connected();
    }
    
    void disconnect()
    {
        if (mValid)
        {
            mConnection->disconnect();
        }
    }
    
protected:
    CEGUI::Event::Connection mConnection;
    bool mValid;
    EventCallback* mCallback;
};

EventConnection * EventSet_subscribeEventSet(CEGUI::EventSet *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}

EventConnection * EventSet_subscribeEventPB(CEGUI::PushButton *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventTB(CEGUI::Titlebar *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventFW(CEGUI::FrameWindow *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventDefaultWindow(CEGUI::DefaultWindow *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventComboBox(CEGUI::Combobox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventCheckBox(CEGUI::Checkbox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventSB(CEGUI::Scrollbar *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventLB(CEGUI::Listbox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventEB(CEGUI::Editbox *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventThumb(CEGUI::Thumb *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventSlider(CEGUI::Slider *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventSpinner(CEGUI::Spinner *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeEventRadioButton(CEGUI::RadioButton *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeDragContainer(CEGUI::DragContainer *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeMultiColumnList(CEGUI::MultiColumnList *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}

EventConnection * EventSet_subscribeWindow(CEGUI::Window *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeMouseCursor(CEGUI::MouseCursor *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
EventConnection * EventSet_subscribeSystem(CEGUI::System *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}

EventConnection * EventSet_subscribeTree(CEGUI::Tree *self , CEGUI::String const & name, 
                                                PyObject* subscriber, CEGUI::String const & method="")
{
    EventConnection *connect = new EventConnection(self->subscribeEvent(name, EventCallback(subscriber, method))); 
    return connect; 
}
"""
    )
    eventSet.add_registration_code(
"""  
def( "subscribeEvent", &EventSet_subscribeEventPB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventTB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventLB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventComboBox, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventCheckBox, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventDefaultWindow, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventFW, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventEB, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventThumb, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSlider, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSpinner, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventRadioButton, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeDragContainer, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeMultiColumnList, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeWindow, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeMouseCursor, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeSystem, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeEventSet, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
EventSet_exposer.def( "subscribeEvent", &EventSet_subscribeTree, 
            bp::return_value_policy< bp::reference_existing_object, bp::default_call_policies >());
                        
{ //EventConnection
        typedef bp::class_< EventConnection > EventConnection_exposer_t;
        EventConnection_exposer_t EventConnection_exposer = EventConnection_exposer_t( "EventConnection" );
        bp::scope EventConnection_scope( EventConnection_exposer );
        EventConnection_exposer.def( bp::init< >() );
        { //::CEGUI::EventConnection::connected
        
            typedef bool ( EventConnection::*connected_function_type )(  ) ;
            
            EventConnection_exposer.def( 
                "connected"
                , connected_function_type( &EventConnection::connected ) );
        
        }
        { //::CEGUI::EventConnection::disconnect
        
            typedef void ( EventConnection::*disconnect_function_type )(  ) ;
            
            EventConnection_exposer.def( 
                "disconnect"
                , disconnect_function_type( &EventConnection::disconnect ) );
        
        }
    }
"""
    )
    
    # CEGUIExceptions.h
    # handled separately
    
    # CEGUIFactoryModule.h
    # not doable in python
    
    # CEGUIFont.h
    font = CEGUI_ns.class_("Font", recursive = False)
    font.include()
    
    # CEGUIFont_xmlHandler.h
    # not interesting for python
    
    # CEGUIFontGlyph.h
    fontGlyph = CEGUI_ns.class_("FontGlyph")
    fontGlyph.include()
    
    # CEGUIFontManager.h
    fontManager = CEGUI_ns.class_("FontManager")
    fontManager.include()
    
    # CEGUIFormattedRenderedString.
    # todo: is this interesting for python?
    
    # CEGUIForwardRefs.h
    # hell no
    
    # CEGUIFreeFunctionSlot.h
    # handled differently elsewhere, see EventSet
    
    # CEGUIFreeTypeFont.h
    # not interesting for python
    
    # CEGUIFribidiVisualMapping.h
    # not interesting for python
    
    # CEGUIFunctorCopySlot.h
    # handled differently elsewhere, see EventSet
    
    # CEGUIFunctorPointerSlot.h
    # handled differently elsewhere, see EventSet
    
    # CEGUIFunctorReferenceBinder.h
    # handled differently elsewhere, see EventSet
    
    # CEGUIFunctorReferenceSlot.h
    # handled differently elsewhere, see EventSet
    
    # CEGUIGeometryBuffer.h
    geometryBuffer = CEGUI_ns.class_("GeometryBuffer")
    geometryBuffer.include()
    
    # CEGUIGlobalEventSet.h
    globalEventSet = CEGUI_ns.class_("GlobalEventSet")
    globalEventSet.include()
    
    # CEGUIGUILayout_xmlHandler.h
    # not needed in python
    
    # CEGUIImage.h
    image = CEGUI_ns.class_("Image")
    image.include()
    
    # CEGUIImageCodec.h
    imageCodec = CEGUI_ns.class_("ImageCodec")
    imageCodec.include()
    
    # CEGUIImageManager.h
    imageManager = CEGUI_ns.class_("ImageManager")
    imageManager.include()
    
    # CEGUIInputEvent.h
    key = CEGUI_ns.class_("Key")
    key.include()
    
    mouseButtonEnum = CEGUI_ns.enum("MouseButton")
    mouseButtonEnum.include()
    
    systemKeyEnum = CEGUI_ns.enum("SystemKey")
    systemKeyEnum.include()
    
    # CEGUIInterpolator.h
    interpolator = CEGUI_ns.class_("Interpolator")
    interpolator.include()
    
    # CEGUIIteratorBase.h
    # all iterators are sorted later
    
    # CEGUIJustifiedRenderedString.h
    # not needed in python
    
    # CEGUIKeyFrame.h
    keyFrame = CEGUI_ns.class_("KeyFrame")
    keyFrame.include()
    
    # CEGUILeftAlignedRenderedString.h
    # not needed in python
    
    # !!!
    
    # CEGUILogger.h
    logger = CEGUI_ns.class_("Logger")
    logger.include()
    
    loggingLevelEnum = CEGUI_ns.enum("LoggingLevel")
    loggingLevelEnum.include()
    
    # CEGUIMemberFunctionSlot.h
    # sorted elsewhere, see EventSet
    
    # CEGUIMinibidiVisualMapping.h
    # not needed for python
    
    # CEGUIMinizipResourceProvider.h
    # not needed for python
    
    # CEGUIMouseCursor.h
    mouseCursor = CEGUI_ns.class_("MouseCursor")
    mouseCursor.include()
    mouseCursor.noncopyable = True
    
    mouseCursorImageEnum = CEGUI_ns.enum("MouseCursorImage")
    mouseCursorImageEnum.include()
    
    # CEGUINamedXMLResourceManager.h
    xmlResourceExistsActionEnum = CEGUI_ns.enum("XMLResourceExistsAction")
    xmlResourceExistsActionEnum.include()
    
    resourceEventSet = CEGUI_ns.class_("ResourceEventSet")
    resourceEventSet.include()
    
    # CEGUIPCRERegexMatcher.h
    # not needed in python
    
    # CEGUIPixmapFont.h
    # not needed in python
    
    # CEGUIProperty.h
    propertyReceiver = CEGUI_ns.class_("PropertyReceiver")
    propertyReceiver.include()
    
    property = CEGUI_ns.class_("Property")
    property.include()
    
    # CEGUIPropertyHelper.h
    propertyHelper = CEGUI_ns.class_("PropertyHelper_wrapper")
    propertyHelper.include()
    propertyHelper.rename("PropertyHelper")
    
    # CEGUIPropertySet.h
    propertySet = CEGUI_ns.class_("PropertySet")
    propertySet.include()
    
    # CEGUIQuaternion.h
    quaternion = CEGUI_ns.class_("Quaternion")
    quaternion.include()
    
    # CEGUIRect.h
    rect = CEGUI_ns.class_("Rect")
    rect.include()
    
    # CEGUIRefCounted.h
    # handled elsewhere
    
    # CEGUIRefexMatcher.h
    # not needed in python
    
    # CEGUIRenderedString.h
    renderedString = CEGUI_ns.class_("RenderedString")
    renderedString.include()
    
    # CEGUIRenderedStringComponent.h
    renderedStringComponent = CEGUI_ns.class_("RenderedStringComponent")
    renderedStringComponent.include()
    
    # CEGUIRenderedStringImageComponent.h
    renderedStringImageComponent = CEGUI_ns.class_("RenderedStringImageComponent")
    renderedStringImageComponent.include()
    
    # CEGUIRenderedStringTextComponent.h
    renderedStringTextComponent = CEGUI_ns.class_("RenderedStringTextComponent")
    renderedStringTextComponent.include()

    # CEGUIRenderedStringParser.h
    renderedStringParser = CEGUI_ns.class_("RenderedStringParser")
    renderedStringParser.include()
    
    # CEGUIRenderedStringWidgetComponent.h
    renderedStringWidgetComponent = CEGUI_ns.class_("RenderedStringWidgetComponent")
    renderedStringWidgetComponent.include()
    
    # CEGUIRenderEffect.h
    renderEffect = CEGUI_ns.class_("RenderEffect")
    renderEffect.include()
    
    # CEGUIRenderEffectFactory.h
    renderEffectFactory = CEGUI_ns.class_("RenderEffectFactory")
    renderEffectFactory.include()
    
    # CEGUIRenderEffectManager.h
    renderEffectManager = CEGUI_ns.class_("RenderEffectManager")
    renderEffectManager.include()
    
    # CEGUIRenderer.h
    quadSplitModeEnum = CEGUI_ns.enum("QuadSplitMode")
    quadSplitModeEnum.include()
    
    blendModeEnum = CEGUI_ns.enum("BlendMode")
    blendModeEnum.include()
    
    renderer = CEGUI_ns.class_("Renderer")
    renderer.include()
    
    # CEGUIRenderingContext.h
    renderingContext = CEGUI_ns.class_("RenderingContext")
    renderingContext.include()
    
    # CEGUIRenderingRoot.h
    renderingRoot = CEGUI_ns.class_("RenderingRoot")
    renderingRoot.include()
    
    # CEGUIRenderingSurface.h
    CEGUI_ns.enum("RenderQueueID").include()
    renderingSurface = CEGUI_ns.class_("RenderingSurface")
    renderingSurface.include()
    
    # CEGUIRenderingWindow.h
    renderingWindow = CEGUI_ns.class_("RenderingWindow")
    renderingWindow.include()
    
    # CEGUIRenderQueue.h
    renderQueue = CEGUI_ns.class_("RenderQueue")
    renderQueue.include()
    
    # CEGUIRenderTarget.h
    renderTarget = CEGUI_ns.class_("RenderTarget")
    renderTarget.include()
    
    # CEGUIResourceProvider.h
    resourceProvider = CEGUI_ns.class_("ResourceProvider")
    resourceProvider.include()
    
    # CEGUIRightAlignedRenderedString.h
    # not needed in python
    
    # CEGUIScheme.h
    scheme = CEGUI_ns.class_("Scheme")
    scheme.include()
    
    # CEGUISchemeManager.h
    schemeManager = CEGUI_ns.class_("SchemeManager")
    schemeManager.include()
    
    # CEGUIScriptModule.h
    scriptModule = CEGUI_ns.class_("ScriptModule")
    scriptModule.include()
    
    # CEGUISingleton.h
    # handled separately
    
    # CEGUISize.h
    size = CEGUI_ns.class_("Size<float>")
    size.include()
    
    # CEGUISlotFunctorBase.h
    # not needed in python
    
    # CEGUIString.h
    string = CEGUI_ns.class_("String")
    # we set this as already exposed, since we provide conversion to python's inbuilt objects
    # see the beginning of generateCode
    string.already_exposed = True
    
    # CEGUISubscriberSlot.h
    # todo: probably not needed with current setup, should we exclude this?
    subscriberSlot = CEGUI_ns.class_("SubscriberSlot")
    subscriberSlot.include()
    # exclude the constructor that takes function pointer as argument
    for c in subscriberSlot.constructors():
        if len(c.arguments) > 0:
            c.exclude()

	# CEGUISystem.h
    system = CEGUI_ns.class_("System")
    system.include()
    commonUtils.excludeAllPrivate(system)
    
    # CEGUITexture.h
    texture = CEGUI_ns.class_("Texture")
    texture.include()
    
    # CEGUITextureTarget.h
    textureTarget = CEGUI_ns.class_("TextureTarget")
    textureTarget.include()
    
    # CEGUITextUtils.h
    textUtils = CEGUI_ns.class_("TextUtils")
    textUtils.include()
    
    # CEGUITplWindowFactory.h
    # CEGUITplWindowRendererFactory.h
    # CEGUITplWRFactoryRegisterer.h
    # python doesn't like templates :-)
	
	# CEGUIUDim.h
    udim = CEGUI_ns.class_("UDim")
    udim.include()
	
    urect = CEGUI_ns.class_("URect")
    urect.include()
    
    ubox = CEGUI_ns.class_("UBox")
    ubox.include()
    
    # CEGUIVector.h
    vector2 = CEGUI_ns.class_("Vector2<float>")
    vector2.include()
    
    vector3 = CEGUI_ns.class_("Vector3<float>")
    vector3.include()
    
    uvector2 = CEGUI_ns.class_("Vector2<CEGUI::UDim>")
    uvector2.include()
    
    # CEGUIVersion.h
    # nothing usable for python
    
    # CEGUIVertex.h
    vertex = CEGUI_ns.class_("Vertex")
    vertex.include()
    
    # CEGUIWidgetModule.h
    # not needed in python
    
    # CEGUIWindow.h
    CEGUI_ns.enum("VerticalAlignment").include()
    CEGUI_ns.enum("HorizontalAlignment").include()
    CEGUI_ns.enum("WindowUpdateMode").include()
    
    window = CEGUI_ns.class_("Window")
    window.include()
    # python doesn't like void*
    window.mem_fun("setUserData").exclude()
    window.mem_fun("getUserData").exclude()
    # todo: check that getUserData is really a python object
    window.add_declaration_code(
"""
void
Window_setUserData ( ::CEGUI::Window & me, PyObject * data ) {
    me.setUserData ( data );
    }
    
PyObject *
Window_getUserData ( ::CEGUI::Window & me) {
    void *  data = me.getUserData (  );
    Py_INCREF( (PyObject *) data );     // I'm passing a reference to this object so better inc the ref :)
    return  (PyObject *) data;
    }
"""
    )
    window.add_registration_code("""def ("setUserData", &::Window_setUserData);""")
    window.add_registration_code("""def ("getUserData", &::Window_getUserData);""")
    
    # CEGUIWindowFactory.h
    windowFactory = CEGUI_ns.class_("WindowFactory")
    windowFactory.include()
    
    # CEGUIWindowFactoryManager.h
    windowFactoryManager = CEGUI_ns.class_("WindowFactoryManager")
    windowFactoryManager.include()
    
    # CEGUIWindowManager.h
    windowManager = CEGUI_ns.class_("WindowManager")
    windowManager.include()
    # we do this in custom code below
    windowManager.mem_fun("loadWindowLayout").exclude()
    windowManager.add_declaration_code(
"""
CEGUI::Window*
WindowManager_loadWindowLayout(::CEGUI::WindowManager & me,
    const CEGUI::String& filename, const CEGUI::String& name_prefix = "", const CEGUI::String& resourceGroup = "") {
      return me.loadWindowLayout( filename, name_prefix , resourceGroup);
    }
"""
    )
    windowManager.add_registration_code(
"""
def ("loadWindowLayout", &::WindowManager_loadWindowLayout,\
        ( bp::arg("filename"), bp::arg("name_prefix")="", bp::arg("resourceGroup")="" ), \
        bp::return_value_policy< bp::reference_existing_object,bp::default_call_policies >());
"""   
    )
    
    # CEGUIWindowProperties.h
    # not needed in python
    
    # CEGUIWindowRenderer.h
    windowRenderer = CEGUI_ns.class_("WindowRenderer", recursive = False)
    windowRenderer.include()
    
    windowRendererFactory = CEGUI_ns.class_("WindowRendererFactory")
    windowRendererFactory.include()
    
    # CEGUIWindowRendererManager.h
    windowRendererManager = CEGUI_ns.class_("WindowRendererManager")
    windowRendererManager.include()
    
    # CEGUIWindowRendererModule.h
    # not needed in python
    
    # CEGUIWRFactoryRegisterer.h
    # not needed in python
    
    # CEGUIXMLAttributes.h
    xmlAttributes = CEGUI_ns.class_("XMLAttributes")
    xmlAttributes.include()
    
    # CEGUIXMLHandler.h
    xmlHandler = CEGUI_ns.class_("XMLHandler")
    xmlHandler.include()
    
    # CEGUIXMLParser.h
    xmlParser = CEGUI_ns.class_("XMLParser")
    xmlParser.include()
    
    # CEGUIXMLSerializer.h
    xmlSerializer = CEGUI_ns.class_("XMLSerializer")
    xmlSerializer.include()
    
    # all EventArgs
    for cls in CEGUI_ns.classes():
        if cls.name.endswith("EventArgs"):
            cls.include()
    
    # all Iterator classes
    for cls in CEGUI_ns.classes():
        if not declarations.templates.is_instantiation(cls.name):
            continue
        
        tplName = declarations.templates.name(cls.name)
        if not tplName.endswith("Iterator"):
            continue
        
        # now cls is sure to be "Iterator" class
        cls.include()
        
        # we only allow default parameter-less constructors
        # se lets exclude every constructor that has parameters
        cls.constructors(lambda decl: bool(decl.arguments), allow_empty = True, recursive = False).exclude()
        
        # we have to make aliases for operator++ and operator--, these are not supported by python
        # instead of operator++, we use next(self) and instead of operator--, we use previous(self)
        cls.add_declaration_code(
"""
void Iterator_next(::CEGUI::%s& t)
{
    t++;
}

void Iterator_previous(::CEGUI::%s& t)
{
    t--;
}
""" % (cls.name, cls.name))
        cls.add_registration_code('def("next", &::Iterator_next)')
        cls.add_registration_code('def("previous", &::Iterator_previous)')
    
    ### ELEMENTS ###
    
    # we always skip the Properties files since they are of no use to python users
    
    # elements/CEGUIButtonBase.h
    buttonBase = CEGUI_ns.class_("ButtonBase")
    buttonBase.include()
    
    # elements/CEGUICheckbox.h
    checkbox = CEGUI_ns.class_("Checkbox")
    checkbox.include()
    
    # elements/CEGUICombobox.h
    combobox = CEGUI_ns.class_("Combobox")
    combobox.include()
    
    # elements/CEGUIComboDropList.h
    comboDropList = CEGUI_ns.class_("ComboDropList")
    comboDropList.include()

    # elements/CEGUIDefaultWindow.h
    defaultWindow = CEGUI_ns.class_("DefaultWindow")
    defaultWindow.include()
    
    # elements/CEGUIDragContainer.h
    dragContainer = CEGUI_ns.class_("DragContainer")
    dragContainer.include()
    
    # elements/CEGUIEditbox.h
    editbox = CEGUI_ns.class_("Editbox")
    editbox.include()
    
    # elements/CEGUIFrameWindow.h
    frameWindow = CEGUI_ns.class_("FrameWindow")
    frameWindow.include()
    
    # elements/CEGUIGridLayoutContainer.h
    gridLayoutContainer = CEGUI_ns.class_("GridLayoutContainer")
    gridLayoutContainer.include()
    
    # elements/CEGUIGroupBox.h
    groupBox = CEGUI_ns.class_("GroupBox")
    groupBox.include()
    
    # elements/CEGUIHorizontalLayoutContainer.h
    horizontalLayoutContainer = CEGUI_ns.class_("HorizontalLayoutContainer")
    horizontalLayoutContainer.include()
    
    # elements/CEGUIItemEntry.h
    itemEntry = CEGUI_ns.class_("ItemEntry")
    itemEntry.include()
    
    # elements/CEGUIItemListBase.h
    itemListBase = CEGUI_ns.class_("ItemListBase")
    itemListBase.include()
    # needs function pointer as argument
    itemListBase.mem_fun("setSortCallback").exclude()
    
    # elements/CEGUIItemListbox.h
    itemListbox = CEGUI_ns.class_("ItemListbox")
    itemListbox.include()
    
    # elements/CEGUILayoutContainer.h
    layoutContainer = CEGUI_ns.class_("LayoutContainer")
    layoutContainer.include()
    
    # elements/CEGUIListbox.h
    listbox = CEGUI_ns.class_("Listbox")
    listbox.include()
    
    # elements/CEGUIListboxItem.h
    listboxItem = CEGUI_ns.class_("ListboxItem")
    listboxItem.include()
    
    # elements/CEGUIListboxTextItem.h
    listboxTextItem = CEGUI_ns.class_("ListboxTextItem")
    listboxTextItem.include()
    
    # elements/CEGUIListHeader.h
    listHeader = CEGUI_ns.class_("ListHeader")
    listHeader.include()
    
    # elements/CEGUIListHeaderSegment.h
    listHeaderSegment = CEGUI_ns.class_("ListHeaderSegment")
    listHeaderSegment.include()
    
    # elements/CEGUIMenubar.h
    menubar = CEGUI_ns.class_("Menubar")
    menubar.include()
    
    # elements/CEGUIMenuBase.h
    menuBase = CEGUI_ns.class_("MenuBase")
    menuBase.include()
    
    # elements/CEGUIMenuItem.h
    menuItem = CEGUI_ns.class_("MenuItem")
    menuItem.include()
    
    # elements/CEGUIMultiColumnList.h
    CEGUI_ns.class_("MCLGridRef").include()
    multiColumnList = CEGUI_ns.class_("MultiColumnList")
    multiColumnList.include()
    # internal private class
    multiColumnList.class_("ListRow").exclude()
    
    # elements/CEGUIMultiLineEditbox.h
    multiLineEditbox = CEGUI_ns.class_("MultiLineEditbox")
    multiLineEditbox.include()
    
    # elements/CEGUIPopupMenu.h
    popupMenu = CEGUI_ns.class_("PopupMenu")
    popupMenu.include()
    
    # elements/CEGUIProgressBar.h
    progressBar = CEGUI_ns.class_("ProgressBar")
    progressBar.include()
    
    # elements/CEGUIPushButton.h
    pushButton = CEGUI_ns.class_("PushButton")
    pushButton.include()
    
    # elements/CEGUIRadioButton.h
    radioButton = CEGUI_ns.class_("RadioButton")
    radioButton.include()
    
    # elements/CEGUIScrollablePane.h
    scrollablePane = CEGUI_ns.class_("ScrollablePane")
    scrollablePane.include()
    
    # elements/CEGUIScrollbar.h
    scrollbar = CEGUI_ns.class_("Scrollbar")
    scrollbar.include()
    
    # elements/CEGUIScrolledContainer.h
    scrolledContainer = CEGUI_ns.class_("ScrolledContainer")
    scrolledContainer.include()
    
    # elements/CEGUIScrolledItemListBase.h
    scrolledItemListBase = CEGUI_ns.class_("ScrolledItemListBase")
    scrolledItemListBase.include()
    
    # elements/CEGUISequentialLayoutContainer.h
    sequentialLayoutContainer = CEGUI_ns.class_("SequentialLayoutContainer")
    sequentialLayoutContainer.include()
    
    # elements/CEGUISlider.h
    slider = CEGUI_ns.class_("Slider")
    slider.include()
    
    # elements/CEGUISpinner.h
    spinner = CEGUI_ns.class_("Spinner")
    spinner.include()
    
    # elements/CEGUITabButton.h
    tabButton = CEGUI_ns.class_("TabButton")
    tabButton.include()
    
    # elements/CEGUITabButton.h
    tabControl = CEGUI_ns.class_("TabControl")
    tabControl.include()
    
    # elements/CEGUIThumb.h
    thumb = CEGUI_ns.class_("Thumb")
    thumb.include()
    
    # elements/CEGUITitlebar.h
    titlebar = CEGUI_ns.class_("Titlebar")
    titlebar.include()
    
    # elements/CEGUITooltip.h
    tooltip = CEGUI_ns.class_("Tooltip", recursive = False)
    tooltip.include()
    
    # elements/CEGUITree.h
    tree = CEGUI_ns.class_("Tree")
    tree.include()
    # todo: this doesn't work, needs function transformation
    tree.mem_fun("getNextSelectedItemFromList").exclude()
    
    # elements/CEGUITreeItem.h
    treeItem = CEGUI_ns.class_("TreeItem")
    treeItem.include()
    
    # elements/CEGUIVerticalLayoutContainer.h
    verticalLayoutContainer = CEGUI_ns.class_("VerticalLayoutContainer")
    verticalLayoutContainer.include()
    
    ### FALAGARD ###
    
    # falagard/CEGUIFalComponentBase.h
    falagardComponentBase = CEGUI_ns.class_("FalagardComponentBase")
    falagardComponentBase.include()
    
    # falagard/CEGUIFalDimensions.h
    CEGUI_ns.class_("BaseDim").include()
    CEGUI_ns.class_("AbsoluteDim").include()
    CEGUI_ns.class_("ImageDim").include()
    CEGUI_ns.class_("WidgetDim").include()
    CEGUI_ns.class_("UnifiedDim").include()
    CEGUI_ns.class_("FontDim").include()
    CEGUI_ns.class_("PropertyDim").include()
    CEGUI_ns.class_("Dimension").include()
    CEGUI_ns.class_("ComponentArea").include()
    
    # falagard/CEGUIFalEnums.h
    CEGUI_ns.enum("DimensionType").include()
    CEGUI_ns.enum("VerticalFormatting").include()
    CEGUI_ns.enum("HorizontalFormatting").include()
    CEGUI_ns.enum("VerticalTextFormatting").include()
    CEGUI_ns.enum("HorizontalTextFormatting").include()
    CEGUI_ns.enum("FontMetricType").include()
    CEGUI_ns.enum("DimensionOperator").include()
    CEGUI_ns.enum("FrameImageComponent").include()

    # falagard/CEGUIFalEventLinkDefinition.h
    eventLinkDefinition = CEGUI_ns.class_("EventLinkDefinition")
    eventLinkDefinition.include()
    
    # falagard/CEGUIFalFrameComponent.h
    frameComponent = CEGUI_ns.class_("FrameComponent")
    frameComponent.include()
    
    # falagard/CEGUIFalImageryComponent.h
    imageryComponent = CEGUI_ns.class_("ImageryComponent")
    imageryComponent.include()
    
    # falagard/CEGUIFalImagerySection.h
    imagerySection = CEGUI_ns.class_("ImagerySection")
    imagerySection.include()
    
    # falagard/CEGUIFalLayerSpecification.h
    layerSpecification = CEGUI_ns.class_("LayerSpecification")
    layerSpecification.include()
    
    # falagard/CEGUIFalNamedArea.h
    namedArea = CEGUI_ns.class_("NamedArea")
    namedArea.include()
    
    # falagard/CEGUIFalPropertyDefinition.h
    propertyDefinition = CEGUI_ns.class_("PropertyDefinition")
    propertyDefinition.include()
    
    # falagard/CEGUIFalPropertyDefinitionBase.h
    propertyDefinitionBase = CEGUI_ns.class_("PropertyDefinitionBase")
    propertyDefinitionBase.include()
    
    # falagard/CEGUIFalPropertyInitialiser.h
    propertyInitialiser = CEGUI_ns.class_("PropertyInitialiser")
    propertyInitialiser.include()
    
    # falagard/CEGUIFalPropertyLinkDefinition.h
    propertyLinkDefinition = CEGUI_ns.class_("PropertyLinkDefinition")
    propertyLinkDefinition.include()
    
    # falagard/CEGUIFalSectionSpecification.h
    sectionSpecification = CEGUI_ns.class_("SectionSpecification")
    sectionSpecification.include()
    
    # falagard/CEGUIFalStateImagery.h
    stateImagery = CEGUI_ns.class_("StateImagery")
    stateImagery.include()
    
    # falagard/CEGUIFalTextComponent.h
    textComponent = CEGUI_ns.class_("TextComponent")
    textComponent.include()
    
    # falagard/CEGUIFalWidgetComponent.h
    widgetComponent = CEGUI_ns.class_("WidgetComponent")
    widgetComponent.include()
    
    # falagard/CEGUIFalWidgetLookFeel.h
    widgetLookFeel = CEGUI_ns.class_("WidgetLookFeel")
    widgetLookFeel.include()
    
    # falagard/CEGUIFalWidgetLookManager.h
    widgetLookManager = CEGUI_ns.class_("WidgetLookManager")
    widgetLookManager.include()
    
    # falagard/CEGUIFalXMLEnumHelper.h
    falagardXMLHelper = CEGUI_ns.class_("FalagardXMLHelper")
    falagardXMLHelper.include()
    
    # todo: hack fixes
    # taken from python ogre, causes AttributeError at import if not excluded
    for cls in CEGUI_ns.classes():
        try:
            cls.variable("EventNamespace").exclude()
        except:
            pass
    ## turns out that in SOME classes this also fails registration (Combodroplist for example)
    for cls in CEGUI_ns.classes():
        try:
            cls.variable("WidgetTypeName").exclude()
        except:
            pass
    
    # taken from python ogre        
    ## lets work around a bug in GCCXMl - http://language-binding.net/pygccxml/design.html#patchers
    draws = mb.mem_funs("draw")   # find all the draw functions
    for draw in draws:
        for arg in draw.arguments:  
            if arg.default_value == "0ffffffff":
                arg.default_value = "0xffffffff"
                
    # HACK! These shouldn't be needed exposed!
    for cls in CEGUI_ns.classes():
        if cls.name.startswith("Singleton"):
            cls.include()
            
        if cls.name.startswith("NamedXMLResourceManager"):
            cls.include()
            
    # no need for this function, just use getSingleton
    mb.mem_funs("getSingletonPtr").exclude()

    # hack that should work for now! disallows inheriting and overriding in python
    commonUtils.excludeAllPrivate(CEGUI_ns)
    commonUtils.excludeAllProtected(CEGUI_ns)

def configureExceptions(mb):
    exception = mb.namespace("CEGUI").class_("Exception")
    # todo: do we want the exception or just the message?
    #exception.include()
    exception.translate_exception_to_string("PyExc_RuntimeError",  "exc.getMessage().c_str()")

def generateCode():
    ### disable unnecessary warnings
    # can't pass function pointer
    messages.disable(messages.W1004)
    # operator not supported
    messages.disable(messages.W1014)
    # py++ will create a wrapper
    messages.disable(messages.W1023, messages.W1025, messages.W1026, messages.W1031)
    # static pointer member can't be exported
    messages.disable(messages.W1035)
    # immutable pointer can't be exposed
    messages.disable(messages.W1036)
    # pointer to function can't be exposed
    messages.disable(messages.W1036, messages.W1037)
    # can't be overridden in python
    messages.disable(messages.W1049)
    
    mb = commonUtils.createModuleBuilder("python_CEGUI.h", ["CEGUIBASE_EXPORTS"])
    CEGUI_ns = mb.global_ns.namespace("CEGUI")
    # first thing to do - converters!
    # !!! They have to be registered first, otherwise it will all fail horribly !!!
    commonUtils.addStringConverters(mb)
    commonUtils.addSupportForString(mb)
                            
    filterDeclarations(mb)
    configureExceptions(mb)

    commonUtils.setDefaultCallPolicies(CEGUI_ns)
     
    ## add additional version information to the module to help identify it correctly 
    # todo: this should be done automatically
    commonUtils.addVersionInfo(mb, PACKAGE_NAME, PACKAGE_VERSION)
    
    # Creating code creator. After this step you should not modify/customize declarations.
    mb.build_code_creator(module_name = MODULE_NAME, doc_extractor = commonUtils.createDocumentationExtractor())
    
    commonUtils.writeModule(mb, OUTPUT_DIR)

if __name__ == "__main__":
    generateCode()
