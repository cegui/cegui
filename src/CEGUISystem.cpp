/************************************************************************
	filename: 	CEGUISystem.cpp
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Implementation of main system object
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGUISystem.h"
#include "CEGUILogger.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIFontManager.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIWindow.h"
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"
#include "elements/CEGUIGUISheet.h"
#include "CEGUIScriptModule.h"
#include "CEGUIConfig_xmlHandler.h"
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/Janitor.hpp"
#include "xercesc/sax2/DefaultHandler.hpp"
#include "xercesc/sax2/SAX2XMLReader.hpp"
#include "xercesc/sax2/XMLReaderFactory.hpp"
#include "xercesc/framework/MemBufInputSource.hpp"
#include "CEGUIDataContainer.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIGlobalEventSet.h"
#include <time.h>


// Start of CEGUI namespace section
namespace CEGUI
{
const String System::EventNamespace("System");

/*!
\brief
	Simple timer class.
*/
class SimpleTimer
{
	clock_t d_baseTime;

public:
	SimpleTimer() : d_baseTime(clock()) {}

	void	restart()	{ d_baseTime = clock(); }
	double	elapsed()	{ return static_cast<double>((clock() - d_baseTime) / CLOCKS_PER_SEC); }
};
	
/*!
\brief
	Implementation structure used in tracking up & down mouse button inputs in order to generate click, double-click,
	and triple-click events.
*/
struct MouseClickTracker
{
	MouseClickTracker(void) : d_click_count(0), d_click_area(0, 0, 0, 0) {}

	SimpleTimer		d_timer;			//!< Timer used to track clicks for this button.
	int				d_click_count;		//!< count of clicks made so far.
	Rect			d_click_area;		//!< area used to detect multi-clicks
};


struct MouseClickTrackerImpl
{
	MouseClickTracker	click_trackers[MouseButtonCount];
};


/*************************************************************************
	Constants definitions
*************************************************************************/
const char	System::CEGUIConfigSchemaName[]		= "CEGUIConfig.xsd";


/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> System* Singleton<System>::ms_Singleton	= NULL;

// click event generation defaults
const double	System::DefaultSingleClickTimeout	= 0.2;
const double	System::DefaultMultiClickTimeout	= 0.33;
const Size		System::DefaultMultiClickAreaSize(12,12);

// event names
const String System::EventGUISheetChanged( (utf8*)"GUISheetChanged" );
const String System::EventSingleClickTimeoutChanged( (utf8*)"SingleClickTimeoutChanged" );
const String System::EventMultiClickTimeoutChanged( (utf8*)"MultiClickTimeoutChanged" );
const String System::EventMultiClickAreaSizeChanged( (utf8*)"MultiClickAreaSizeChanged" );
const String System::EventDefaultFontChanged( (utf8*)"DefaultFontChanged" );
const String System::EventDefaultMouseCursorChanged( (utf8*)"DefaultMouseCursorChanged" );
const String System::EventMouseMoveScalingChanged( (utf8*)"MouseMoveScalingChanged" );


/*************************************************************************
	Constructor
*************************************************************************/
System::System(Renderer* renderer, utf8* logFile) :
	d_clickTrackerPimpl(new MouseClickTrackerImpl)
{
	constructor_impl(renderer, NULL, NULL, (utf8*)"", logFile);
}
/*************************************************************************
	Construct a new System object
*************************************************************************/
System::System(Renderer* renderer, ResourceProvider* resourceProvider, utf8* logFile) :
	d_clickTrackerPimpl(new MouseClickTrackerImpl)
{
	constructor_impl(renderer, resourceProvider, NULL, (utf8*)"", logFile);
}

/*************************************************************************
	Construct a new System object
*************************************************************************/
System::System(Renderer* renderer, ScriptModule* scriptModule, utf8* configFile) :
	d_clickTrackerPimpl(new MouseClickTrackerImpl)
{
	constructor_impl(renderer, NULL, scriptModule, configFile, (utf8*)"CEGUI.log");
}


/*************************************************************************
	Construct a new System object
*************************************************************************/
System::System(Renderer* renderer, ScriptModule* scriptModule, ResourceProvider* resourceProvider, utf8* configFile) :
	d_clickTrackerPimpl(new MouseClickTrackerImpl)
{
	constructor_impl(renderer, resourceProvider, scriptModule, configFile, (utf8*)"CEGUI.log");
}


/*************************************************************************
	Method to do the work of the constructor
*************************************************************************/
void System::constructor_impl(Renderer* renderer, ResourceProvider* resourceProvider, ScriptModule* scriptModule, const String& configFile, const String& logFile)
{
	d_renderer		= renderer;
	d_gui_redraw	= false;
	d_wndWithMouse	= NULL;
	d_activeSheet	= NULL;
	d_sysKeys		= 0;

	d_lshift	= false;
	d_rshift	= false;
	d_lctrl		= false;
	d_rctrl		= false;

	d_click_timeout		= DefaultSingleClickTimeout;
	d_dblclick_timeout	= DefaultMultiClickTimeout;
	d_dblclick_size		= DefaultMultiClickAreaSize;

	d_defaultMouseCursor = NULL;
	d_scriptModule		 = scriptModule;

	d_mouseScalingFactor = 1.0f;

	// add events for Sytem object
	addSystemEvents();

    // if there has been a resource provider supplied use that otherwise create one.
    d_resourceProvider = resourceProvider ? resourceProvider : renderer->createResourceProvider();

	// initialise Xerces-C XML system
	XERCES_CPP_NAMESPACE_USE
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch(XMLException& exc)
	{
		// prepare a message about the failure
		char* excmsg = XMLString::transcode(exc.getMessage());
		String message((utf8*)"An exception occurred while initialising the Xerces-C XML system.  Additional information: ");
		message += (utf8*)excmsg;
		XMLString::release(&excmsg);

		// throw a std::exception (because it won't try and use logger)
		throw message.c_str();
	}

	// strings we may get from the configuration file.
	String configLogname, configSchemeName, configLayoutName, configInitScript, defaultFontName;

	// now XML is available, read the configuration file (if any)
	if (!configFile.empty())
	{
		SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

		// set basic settings we want from parser
		parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
		parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
		parser->setFeature(XMLUni::fgXercesSchema, true);
		parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);


//        InputSourceContainer configSchemaData;
//        System::getSingleton().getResourceProvider()->loadInputSourceContainer(CEGUIConfigSchemaName, configSchemaData);
//        parser->loadGrammar(*(configSchemaData.getDataPtr()), 
//                Grammar::SchemaGrammarType, true);

        RawDataContainer rawSchemaData;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(CEGUIConfigSchemaName, rawSchemaData);
        MemBufInputSource  configSchemaData(rawSchemaData.getDataPtr(), rawSchemaData.getSize(), CEGUIConfigSchemaName, false);
        parser->loadGrammar(configSchemaData, Grammar::SchemaGrammarType, true);

        // enable grammar reuse
        parser->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);

		// setup schema for Scheme data
		XMLCh* pval = XMLString::transcode(CEGUIConfigSchemaName);
		parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);
		XMLString::release(&pval);

		// setup handler object
		Config_xmlHandler handler;
		parser->setContentHandler(&handler);
		parser->setErrorHandler(&handler);

//        InputSourceContainer configData;
//        d_resourceProvider->loadInputSourceContainer(configFile, configData);

        RawDataContainer rawXMLData;
        System::getSingleton().getResourceProvider()->loadRawDataContainer(configFile, rawXMLData);
        MemBufInputSource  configData(rawXMLData.getDataPtr(), rawXMLData.getSize(), configFile.c_str(), false);

		// do parsing of xml file
		try
		{
//            parser->parse(*(configData.getDataPtr()));
            parser->parse(configData);

			// get the strings read
			configLogname		= handler.getLogFilename();
			configSchemeName	= handler.getSchemeFilename();
			configLayoutName	= handler.getLayoutFilename();
			defaultFontName		= handler.getDefaultFontName();
			configInitScript	= handler.getInitScriptFilename();
			d_termScriptName	= handler.getTermScriptFilename();
		}
		catch(const XMLException& exc)
		{
			if (exc.getCode() != XMLExcepts::NoError)
			{
				delete parser;

				char* excmsg = XMLString::transcode(exc.getMessage());
				String message((utf8*)"System::System - An error occurred while parsing configuration file '" + configFile + "'.  Additional information: ");
				message += excmsg;
				XMLString::release(&excmsg);

				// cleanup XML stuff
				XERCES_CPP_NAMESPACE_USE
				XMLPlatformUtils::Terminate();

				// throw a std::exception (because it won't try and use logger)
				throw message.c_str();
			}

		}
		catch(const SAXParseException& exc)
		{
			delete parser;

			char* excmsg = XMLString::transcode(exc.getMessage());
			String message((utf8*)"System::System - An error occurred while parsing configuration file '" + configFile + "'.  Additional information: ");
			message += excmsg;
			XMLString::release(&excmsg);

			// cleanup XML stuff
			XERCES_CPP_NAMESPACE_USE
			XMLPlatformUtils::Terminate();

			// throw a std::exception (because it won't try and use logger)
			throw message.c_str();
		}
		catch(...)
		{
			delete parser;

			// cleanup XML stuff
			XERCES_CPP_NAMESPACE_USE
			XMLPlatformUtils::Terminate();

			// throw a std::exception (because it won't try and use logger)
			throw String((utf8*)"System::System - An unexpected error occurred while parsing configuration file '" + configFile + "'.").c_str();
		}

		// cleanup parser
		delete parser;
	}

	// Start up the logger:
	// prefer log filename from config file
	if (!configLogname.empty())
	{
		new Logger(configLogname);
	}
	// no log specified in configuration, use default / hard-coded option
	else
	{
		new Logger(logFile);
	}
	
	// beginning main init
	Logger::getSingleton().logEvent((utf8*)"---- Begining CEGUI System initialisation ----");

	// cause creation of other singleton objects
	new ImagesetManager();
	new FontManager();
	new WindowFactoryManager();
	new WindowManager();
	new SchemeManager();
	new MouseCursor();
	new GlobalEventSet();

	// add default GUISheet factory - the only UI element we can create "out of the box".
	WindowFactoryManager::getSingleton().addFactory(new GUISheetFactory);

	// GUISheet's name was changed, register an alias so both can be used
	WindowFactoryManager::getSingleton().addWindowTypeAlias((utf8*)"DefaultGUISheet", GUISheet::WidgetTypeName);

	// success - we are created!  Log it for prosperity :)
	Logger::getSingleton().logEvent((utf8*)"CEGUI::System singleton created.");
	Logger::getSingleton().logEvent((utf8*)"---- CEGUI System initialisation completed ----");

	// subscribe to hear about display mode changes
	d_renderer->subscribeEvent(Renderer::EventDisplaySizeChanged, Event::Subscriber(&CEGUI::System::handleDisplaySizeChange, this));

	// load base scheme
	if (!configSchemeName.empty())
	{
		try
		{
			SchemeManager::getSingleton().loadScheme(configSchemeName);

			// set default font if that was specified also
			if (!defaultFontName.empty())
			{
				setDefaultFont(defaultFontName);
			}

		}
		catch (CEGUI::Exception exc) {}  // catch exception and try to continue anyway

	}

	// load initial layout
	if (!configLayoutName.empty())
	{
		try
		{
			setGUISheet(WindowManager::getSingleton().loadWindowLayout(configLayoutName));
		}
		catch (CEGUI::Exception exc) {}  // catch exception and try to continue anyway

	}

	// execute start up script
	if (!configInitScript.empty())
	{
		try
		{
			executeScriptFile(configInitScript);
		}
		catch (...) {}  // catch all exceptions and try to continue anyway

	}

}


/*************************************************************************
	Destructor
*************************************************************************/
System::~System(void)
{
	Logger::getSingleton().logEvent((utf8*)"---- Begining CEGUI System destruction ----");

	// execute shut-down script
	if (!d_termScriptName.empty())
	{
		try
		{
			executeScriptFile(d_termScriptName);
		}
		catch (...) {}  // catch all exceptions and continue system shutdown

	}

	// cleanup XML stuff
	XERCES_CPP_NAMESPACE_USE
	XMLPlatformUtils::Terminate();

	//
	// perform cleanup in correct sequence
	//
	// destroy windows so it's safe to destroy factories
	WindowManager::getSingleton().destroyAllWindows();

	// get pointer to the GUI sheet factory we added
	GUISheetFactory* factory = (GUISheetFactory*)WindowFactoryManager::getSingleton().getFactory(GUISheet::WidgetTypeName);

	// remove factories so it's safe to unload GUI modules
	WindowFactoryManager::getSingleton().removeAllFactories();

	// destroy GUI sheet factory
	delete factory;

	// cleanup singletons
	delete	SchemeManager::getSingletonPtr();
	delete	WindowManager::getSingletonPtr();
	delete	WindowFactoryManager::getSingletonPtr();
	delete	FontManager::getSingletonPtr();
	delete	MouseCursor::getSingletonPtr();
	delete	ImagesetManager::getSingletonPtr();
	delete	GlobalEventSet::getSingletonPtr();

	Logger::getSingleton().logEvent((utf8*)"CEGUI::System singleton destroyed.");
	Logger::getSingleton().logEvent((utf8*)"---- CEGUI System destruction completed ----");
	delete Logger::getSingletonPtr();

	delete d_clickTrackerPimpl;
}


/*************************************************************************
	Render the GUI for this frame
*************************************************************************/
void System::renderGUI(void)
{
	//////////////////////////////////////////////////////////////////////////
	// This makes use of some tricks the Renderer can do so that we do not
	// need to do a full redraw every frame - only when some UI element has
	// changed.
	//
	// Since the mouse is likely to move very often, and in order not to
	// short-circuit the above optimisation, the mouse is not queued, but is
	// drawn directly to the display every frame.
	//////////////////////////////////////////////////////////////////////////
	
	if (d_gui_redraw)
	{
		d_renderer->resetZValue();
		d_renderer->setQueueingEnabled(true);
		d_renderer->clearRenderList();

		if (d_activeSheet != NULL)
		{
			d_activeSheet->render();
		}

		d_gui_redraw = false;
	}

	d_renderer->doRender();

	// draw mouse
	d_renderer->setQueueingEnabled(false);
	MouseCursor::getSingleton().draw();
}


/*************************************************************************
	Set the active GUI sheet (root) window.	
*************************************************************************/
Window* System::setGUISheet(Window* sheet)
{
	Window* old = d_activeSheet;
	d_activeSheet = sheet;

    // Force and update for the area Rects for 'sheet' so they're correct according
    // to the screen size.
    if (sheet != 0)
    {    
        WindowEventArgs sheetargs(0);
        sheet->onParentSized(sheetargs);
    }
    
	// fire event
	WindowEventArgs args(old);
	onGUISheetChanged(args);

	return old;
}


/*************************************************************************
	Set the default font to be used by the system	
*************************************************************************/
void System::setDefaultFont(const String& name)
{
	if (name.empty())
	{
		setDefaultFont(NULL);
	}
	else
	{
		setDefaultFont(FontManager::getSingleton().getFont(name));
	}

}


/*************************************************************************
	Set the default font to be used by the system	
*************************************************************************/
void System::setDefaultFont(Font* font)
{
	d_defaultFont = font;
	
	// fire event
	EventArgs args;
	onDefaultFontChanged(args);
}


/*************************************************************************
	Set the image to be used as the default mouse cursor.
*************************************************************************/
void System::setDefaultMouseCursor(const Image* image)
{
	if (image == (const Image*)DefaultMouseCursor)
	{
		image = NULL;
	}

	d_defaultMouseCursor = image;

	// fire off event.
	EventArgs args;
	onDefaultMouseCursorChanged(args);
}


/*************************************************************************
	Set the image to be used as the default mouse cursor.
*************************************************************************/
void System::setDefaultMouseCursor(const String& imageset, const String& image_name)
{
	setDefaultMouseCursor(&ImagesetManager::getSingleton().getImageset(imageset)->getImage(image_name));
}


/*************************************************************************
	Return a pointer to the ScriptModule being used for scripting within
	the GUI system.
*************************************************************************/
ScriptModule* System::getScriptingModule(void) const
{
	return d_scriptModule;
}

/*************************************************************************
	Return a pointer to the ResourceProvider being used for within the GUI 
    system.
*************************************************************************/
ResourceProvider* System::getResourceProvider(void) const
{
	return d_resourceProvider;
}

/*************************************************************************
	Execute a script file if possible.	
*************************************************************************/
void System::executeScriptFile(const String& filename) const
{
	if (d_scriptModule != NULL)
	{
		try
		{
			d_scriptModule->executeScriptFile(filename);
		}
		catch(...)
		{
			throw GenericException((utf8*)"System::executeScriptFile - An exception was thrown during the execution of the script file.");
		}

	}
	else
	{
		Logger::getSingleton().logEvent((utf8*)"System::executeScriptFile - the script named '" + filename +"' could not be executed as no ScriptModule is available.", Errors);
	}

}


/*************************************************************************
	Execute a scripted global function if possible.  The function should
	not take any parameters and should return an integer.
*************************************************************************/
int	System::executeScriptGloabl(const String& function_name) const
{
	if (d_scriptModule != NULL)
	{
		try
		{
			return d_scriptModule->executeScriptGloabl(function_name);
		}
		catch(...)
		{
			throw GenericException((utf8*)"System::executeScriptGloabl - An exception was thrown during execution of the scripted function.");
		}

	}
	else
	{
		Logger::getSingleton().logEvent((utf8*)"System::executeScriptGloabl - the global script function named '" + function_name +"' could not be executed as no ScriptModule is available.", Errors);
	}

	return 0;
}


/*************************************************************************
	return the current mouse movement scaling factor.	
*************************************************************************/
float System::getMouseMoveScaling(void) const
{
	return d_mouseScalingFactor;
}


/*************************************************************************
	Set the current mouse movement scaling factor	
*************************************************************************/
void System::setMouseMoveScaling(float scaling)
{
	d_mouseScalingFactor = scaling;

	// fire off event.
	EventArgs args;
	onMouseMoveScalingChanged(args);
}


/*************************************************************************
	Method that injects a mouse movement event into the system
*************************************************************************/
bool System::injectMouseMove(float delta_x, float delta_y)
{
	MouseEventArgs ma(NULL);
	MouseCursor& mouse = MouseCursor::getSingleton();

	ma.moveDelta.d_x = delta_x * d_mouseScalingFactor;
	ma.moveDelta.d_y = delta_y * d_mouseScalingFactor;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = 0;

	// move the mouse cursor & update position in args.
	mouse.offsetPosition(ma.moveDelta);
	ma.position = mouse.getPosition();

	Window* dest_window = getTargetWindow(ma.position);

	// if there is no GUI sheet, then there is nowhere to send input
	if (dest_window != NULL)
	{
		if (dest_window != d_wndWithMouse)
		{
			if (d_wndWithMouse != NULL)
			{
				ma.window = d_wndWithMouse;
				d_wndWithMouse->onMouseLeaves(ma);
			}

			d_wndWithMouse = dest_window;
			ma.window = dest_window;
			dest_window->onMouseEnters(ma);
		}

		// ensure event starts as 'not handled'
		ma.handled = false;

		// loop backwards until event is handled or we run out of windows.
		while ((!ma.handled) && (dest_window != NULL))
		{
			ma.window = dest_window;
			dest_window->onMouseMove(ma);
			dest_window = dest_window->getParent();
		}

	}

	return ma.handled;
}


/*************************************************************************
	Method that injects a mouse button down event into the system.
*************************************************************************/
bool System::injectMouseButtonDown(MouseButton button)
{
	// update system keys
	d_sysKeys |= mouseButtonToSyskey(button);

	MouseEventArgs ma(NULL);
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = 0;

	//
	// Handling for multi-click generation
	//
	MouseClickTracker& tkr = d_clickTrackerPimpl->click_trackers[button];

	tkr.d_click_count++;

	// see if we meet multi-click timing
	if ((tkr.d_timer.elapsed() > d_dblclick_timeout) ||
		(!tkr.d_click_area.isPointInRect(ma.position)) ||
		(tkr.d_click_count > 3))
	{
		// single down event.
		tkr.d_click_count = 1;

		// build allowable area for multi-clicks
		tkr.d_click_area.setPosition(ma.position);
		tkr.d_click_area.setSize(d_dblclick_size);
		tkr.d_click_area.offset(Point(-(d_dblclick_size.d_width / 2), -(d_dblclick_size.d_height / 2)));
	}

	Window* dest_window = getTargetWindow(ma.position);

	// loop backwards until event is handled or we run out of windows.
	while ((!ma.handled) && (dest_window != NULL))
	{
		ma.window = dest_window;

        if (dest_window->wantsMultiClickEvents())
        {
            switch (tkr.d_click_count)
            {
            case 1:
                dest_window->onMouseButtonDown(ma);
                break;

            case 2:
                dest_window->onMouseDoubleClicked(ma);
                break;

            case 3:
                dest_window->onMouseTripleClicked(ma);
                break;
            }
        }
        // current target window does not want multi-clicks,
        // so just send a mouse down event instead.
        else
        {
            dest_window->onMouseButtonDown(ma);
        }

		dest_window = dest_window->getParent();
	}

	// reset timer for this tracker.
	tkr.d_timer.restart();

	return ma.handled;
}


/*************************************************************************
	Method that injects a mouse button up event into the system.
*************************************************************************/
bool System::injectMouseButtonUp(MouseButton button)
{
	// update system keys
	d_sysKeys &= ~mouseButtonToSyskey(button);

	MouseEventArgs ma(NULL);
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = 0;

	Window* dest_window = getTargetWindow(ma.position);

	// loop backwards until event is handled or we run out of windows.
	while ((!ma.handled) && (dest_window != NULL))
	{
		ma.window = dest_window;
		dest_window->onMouseButtonUp(ma);
		dest_window = dest_window->getParent();
	}

	bool wasUpHandled = ma.handled;

	// check timer for 'button' to see if this up event also constitutes a single 'click'
	if (d_clickTrackerPimpl->click_trackers[button].d_timer.elapsed() <= d_click_timeout)
	{
		ma.handled = false;
		dest_window = getTargetWindow(ma.position);

		// loop backwards until event is handled or we run out of windows.
		while ((!ma.handled) && (dest_window != NULL))
		{
			ma.window = dest_window;
			dest_window->onMouseClicked(ma);
			dest_window = dest_window->getParent();
		}

	}

	return (ma.handled | wasUpHandled);
}


/*************************************************************************
	Method that injects a key down event into the system.
*************************************************************************/
bool System::injectKeyDown(uint key_code)
{
	// update system keys
	d_sysKeys |= keyCodeToSyskey((Key::Scan)key_code, true);

	KeyEventArgs args(NULL);

	if (d_activeSheet != NULL)
	{
		args.scancode = (Key::Scan)key_code;
		args.sysKeys = d_sysKeys;

		Window* dest = d_activeSheet->getActiveChild();

		// loop backwards until event is handled or we run out of windows.
		while ((dest != NULL) && (!args.handled))
		{
			args.window = dest;
			dest->onKeyDown(args);
			dest = dest->getParent();
		}

	}

	return args.handled;
}


/*************************************************************************
	Method that injects a key up event into the system.
*************************************************************************/
bool System::injectKeyUp(uint key_code)
{
	// update system keys
	d_sysKeys &= ~keyCodeToSyskey((Key::Scan)key_code, false);

	KeyEventArgs args(NULL);

	if (d_activeSheet != NULL)
	{
		args.scancode = (Key::Scan)key_code;
		args.sysKeys = d_sysKeys;

		Window* dest = d_activeSheet->getActiveChild();

		// loop backwards until event is handled or we run out of windows.
		while ((dest != NULL) && (!args.handled))
		{
			args.window = dest;
			dest->onKeyUp(args);
			dest = dest->getParent();
		}

	}

	return args.handled;
}


/*************************************************************************
	Method that injects a typed character event into the system.	
*************************************************************************/
bool System::injectChar(utf32 code_point)
{
	KeyEventArgs args(NULL);

	if (d_activeSheet != NULL)
	{
		args.codepoint = code_point;
		args.sysKeys = d_sysKeys;

		Window* dest = d_activeSheet->getActiveChild();

		// loop backwards until event is handled or we run out of windows.
		while ((dest != NULL) && (!args.handled))
		{
			args.window = dest;
			dest->onCharacter(args);
			dest = dest->getParent();
		}
		
	}

	return args.handled;
}


/*************************************************************************
	Method that injects a mouse-wheel / scroll-wheel event into the system.	
*************************************************************************/
bool System::injectMouseWheelChange(float delta)
{
	MouseEventArgs ma(NULL);
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = NoButton;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = delta;

	Window* dest_window = getTargetWindow(ma.position);

	// loop backwards until event is handled or we run out of windows.
	while ((!ma.handled) && (dest_window != NULL))
	{
		ma.window = dest_window;
		dest_window->onMouseWheel(ma);
		dest_window = dest_window->getParent();
	}

	return ma.handled;
}


/*************************************************************************
	Method that injects a new position for the mouse cursor.
*************************************************************************/
bool System::injectMousePosition(float x_pos, float y_pos)
{
	// set new mouse position
	MouseCursor::getSingleton().setPosition(Point(x_pos, y_pos));

	// do the real work
	return injectMouseMove(0, 0);
}


/*************************************************************************
	Method to inject time pulses into the system.	
*************************************************************************/
bool System::injectTimePulse(float timeElapsed)
{
	if (d_activeSheet != NULL)
	{
		d_activeSheet->update(timeElapsed);
	}

	return true;
}


/*************************************************************************
	Return window that should get mouse inouts when mouse it at 'pt'
*************************************************************************/
Window*	System::getTargetWindow(const Point& pt) const
{
	Window* dest_window = NULL;

	// if there is no GUI sheet, then there is nowhere to send input
	if (d_activeSheet != NULL)
	{
		dest_window = Window::getCaptureWindow();

		if (dest_window == NULL)
		{
			dest_window = d_activeSheet->getChildAtPosition(pt);

			if (dest_window == NULL)
			{
				dest_window = d_activeSheet;
			}

		}
		else
		{
			Window* child_window = dest_window->getChildAtPosition(pt);

			if (child_window != NULL)
			{
				dest_window = child_window;
			}

		}

	}

	return dest_window;
}


/*************************************************************************
	Translate a MouseButton value into the corresponding SystemKey value	
*************************************************************************/
SystemKey System::mouseButtonToSyskey(MouseButton btn) const
{
	switch (btn)
	{
	case LeftButton:
		return LeftMouse;

	case RightButton:
		return RightMouse;

	case MiddleButton:
		return MiddleMouse;

	case X1Button:
		return X1Mouse;

	case X2Button:
		return X2Mouse;

	default:
		throw InvalidRequestException((utf8*)"System::mouseButtonToSyskey - the parameter 'btn' is not a valid MouseButton value.");
	}
}


/*************************************************************************
	Translate a Key::Scan value into the corresponding SystemKey value	
*************************************************************************/
SystemKey System::keyCodeToSyskey(Key::Scan key, bool direction)
{
	switch (key)
	{
	case Key::LeftShift:
		d_lshift = direction;

		if (!d_rshift)
		{
			return Shift;
		}
		break;

	case Key::RightShift:
		d_rshift = direction;

		if (!d_lshift)
		{
			return Shift;
		}
		break;


	case Key::LeftControl:
		d_lctrl = direction;

		if (!d_rctrl)
		{
			return Control;
		}
		break;

	case Key::RightControl:
		d_rctrl = direction;

		if (!d_lctrl)
		{
			return Control;
		}
		break;

    default:
        break;
	}

	// if not a system key or overall state unchanged, return 0.
	return (SystemKey)0;
}


System&	System::getSingleton(void)
{
	return Singleton<System>::getSingleton();
}


System*	System::getSingletonPtr(void)
{
	return Singleton<System>::getSingletonPtr();
}



/*************************************************************************
	Set the timeout to be used for the generation of single-click events.	
*************************************************************************/
void System::setSingleClickTimeout(double timeout)
{
	d_click_timeout = timeout;

	// fire off event.
	EventArgs args;
	onSingleClickTimeoutChanged(args);
}


/*************************************************************************
	Set the timeout to be used for the generation of multi-click events.	
*************************************************************************/
void System::setMultiClickTimeout(double timeout)
{
	d_dblclick_timeout = timeout;

	// fire off event.
	EventArgs args;
	onMultiClickTimeoutChanged(args);
}


/*************************************************************************
	Set the size of the allowable mouse movement tolerance used when
	generating multi-click events.	
*************************************************************************/
void System::setMultiClickToleranceAreaSize(const Size&	sz)
{
	d_dblclick_size = sz;

	// fire off event.
	EventArgs args;
	onMultiClickAreaSizeChanged(args);
}


/*************************************************************************
	add events for the System object	
*************************************************************************/
void System::addSystemEvents(void)
{
	addEvent(EventGUISheetChanged);
	addEvent(EventSingleClickTimeoutChanged);
	addEvent(EventMultiClickTimeoutChanged);
	addEvent(EventMultiClickAreaSizeChanged);
	addEvent(EventDefaultFontChanged);
	addEvent(EventDefaultMouseCursorChanged);
	addEvent(EventMouseMoveScalingChanged);
}


/*************************************************************************
	Handler called when the main system GUI Sheet (or root window) is changed
*************************************************************************/
void System::onGUISheetChanged(WindowEventArgs& e)
{
	fireEvent(EventGUISheetChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the single-click timeout value is changed.
*************************************************************************/
void System::onSingleClickTimeoutChanged(EventArgs& e)
{
	fireEvent(EventSingleClickTimeoutChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the multi-click timeout value is changed.
*************************************************************************/
void System::onMultiClickTimeoutChanged(EventArgs& e)
{
	fireEvent(EventMultiClickTimeoutChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the size of the multi-click tolerance area is
	changed.
*************************************************************************/
void System::onMultiClickAreaSizeChanged(EventArgs& e)
{
	fireEvent(EventMultiClickAreaSizeChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the default system font is changed.	
*************************************************************************/
void System::onDefaultFontChanged(EventArgs& e)
{
	fireEvent(EventDefaultFontChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the default system mouse cursor image is changed.	
*************************************************************************/
void System::onDefaultMouseCursorChanged(EventArgs& e)
{
	fireEvent(EventDefaultMouseCursorChanged, e, EventNamespace);
}


/*************************************************************************
	Handler called when the mouse movement scaling factor is changed.	
*************************************************************************/
void System::onMouseMoveScalingChanged(EventArgs& e)
{
	fireEvent(EventMouseMoveScalingChanged, e, EventNamespace);
}


/*************************************************************************
	Handler method for display size change notifications
*************************************************************************/
bool System::handleDisplaySizeChange(const EventArgs& e)
{
	// notify gui sheet / root if size change, event propagation will ensure everything else
	// gets updated as required.
	if (d_activeSheet != NULL)
	{
		WindowEventArgs args(NULL);
		d_activeSheet->onParentSized(args);
	}

	return true;
}


/*************************************************************************
	Internal method used to inform the System object whenever a window is
	destroyed, so that System can perform any required housekeeping.
*************************************************************************/
void System::notifyWindowDestroyed(const Window* window)
{
	if (d_wndWithMouse == window)
	{
		d_wndWithMouse = NULL;
	}

	if (d_activeSheet == window)
	{
		d_activeSheet = NULL;
	}

}

} // End of  CEGUI namespace section
