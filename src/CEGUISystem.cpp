/***********************************************************************
	filename: 	CEGUISystem.cpp
	created:	20/2/2004
	author:		Paul D Turner

	purpose:	Implementation of main system object
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUISystem.h"
#include "CEGUIDefaultLogger.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIFontManager.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIWindow.h"
#include "CEGUIImageset.h"
#include "CEGUIExceptions.h"
#include "elements/CEGUIBaseFactories.h"
#include "CEGUIScriptModule.h"
#include "CEGUIConfig_xmlHandler.h"
#include "CEGUIDataContainer.h"
#include "CEGUIResourceProvider.h"
#include "CEGUIGlobalEventSet.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIWindowRendererManager.h"
#include "CEGUIDynamicModule.h"
#include "CEGUIXMLParser.h"
#include <ctime>
#include <clocale>

//This block includes the proper headers when static linking
#if defined(CEGUI_STATIC)
	#ifdef CEGUI_WITH_EXPAT
		#include "../XMLParserModules/expatParser/CEGUIExpatParserModule.h"
	#elif CEGUI_WITH_TINYXML
		#include "../XMLParserModules/TinyXMLParser/CEGUITinyXMLParserModule.h"
	#elif CEGUI_WITH_XERCES
		#include "../XMLParserModules/XercesParser/CEGUIXercesParserModule.h"
	#endif
#endif


#define S_(X) #X
#define STRINGIZE(X) S_(X)

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
	double	elapsed()	{ return static_cast<double>(clock() - d_baseTime) / CLOCKS_PER_SEC; }
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
    Window*         d_target_window;    //!< target window for any events generated.
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
template<> System* Singleton<System>::ms_Singleton	= 0;

// click event generation defaults
const double	System::DefaultSingleClickTimeout	= 0.2;
const double	System::DefaultMultiClickTimeout	= 0.33;
const Size		System::DefaultMultiClickAreaSize(12,12);

// event names
const String System::EventGUISheetChanged( "GUISheetChanged" );
const String System::EventSingleClickTimeoutChanged( "SingleClickTimeoutChanged" );
const String System::EventMultiClickTimeoutChanged( "MultiClickTimeoutChanged" );
const String System::EventMultiClickAreaSizeChanged( "MultiClickAreaSizeChanged" );
const String System::EventDefaultFontChanged( "DefaultFontChanged" );
const String System::EventDefaultMouseCursorChanged( "DefaultMouseCursorChanged" );
const String System::EventMouseMoveScalingChanged( "MouseMoveScalingChanged" );

// Holds name of default XMLParser
String System::d_defaultXMLParserName(STRINGIZE(CEGUI_DEFAULT_XMLPARSER));


/*************************************************************************
    Constructor
*************************************************************************/
System::System(Renderer* renderer,
               ResourceProvider* resourceProvider,
               XMLParser* xmlParser,
               ScriptModule* scriptModule,
               const String& configFile,
               const String& logFile)

: d_renderer(renderer),
  d_resourceProvider(resourceProvider ? resourceProvider : d_renderer->createResourceProvider()),
  d_defaultFont(0),
  d_wndWithMouse(0),
  d_activeSheet(0),
  d_modalTarget(0),
  d_sysKeys(0),
  d_lshift(false),
  d_rshift(false),
  d_lctrl(false),
  d_rctrl(false),
  d_lalt(false),
  d_ralt(false),
  d_click_timeout(DefaultSingleClickTimeout),
  d_dblclick_timeout(DefaultMultiClickTimeout),
  d_dblclick_size(DefaultMultiClickAreaSize),
  d_clickTrackerPimpl(new MouseClickTrackerImpl),
  d_defaultMouseCursor(0),
  d_scriptModule(scriptModule),
  d_mouseScalingFactor(1.0f),
  d_xmlParser(xmlParser),
  d_ourXmlParser(false),
  d_parserModule(0),
  d_defaultTooltip(0),
  d_weOwnTooltip(false)
{
    bool userCreatedLogger = true;

    // Start out by fixing the numeric locale to C (we depend on this behaviour)
    // consider a UVector2 as a property {{0.5,0},{0.5,0}} could become {{0,5,0},{0,5,0}}
    setlocale(LC_NUMERIC, "C");

    // Instantiate logger first (we have no file at this point, but entries will be cached until we do)
    // NOTE: If the user already created a logger prior to calling this constructor,
    // we mark it as so and leave the logger untouched. This allows the user to fully customize
    // the logger as he sees fit without fear of seeing its configuration overwritten by this.
    if (!Logger::getSingletonPtr())
    {
        new DefaultLogger();
        userCreatedLogger = false;
    }

    // Set CEGUI version
    d_strVersion = PropertyHelper::uintToString(CEGUI_VERSION_MAJOR) + "." +
       PropertyHelper::uintToString(CEGUI_VERSION_MINOR) + "." +
       PropertyHelper::uintToString(CEGUI_VERSION_PATCH);

    // handle initialisation and setup of the XML parser
    setupXMLParser();

    // strings we may get from the configuration file.
    String configLogname, configSchemeName, configLayoutName, configInitScript, defaultFontName;

    // now XML is available, read the configuration file (if any)
    if (!configFile.empty())
    {
        // create handler object
        Config_xmlHandler handler;

        // do parsing of xml file
        try
        {
            d_xmlParser->parseXMLFile(handler, configFile, CEGUIConfigSchemaName, "");
        }
        catch(...)
        {
            // cleanup XML stuff
            d_xmlParser->cleanup();
            delete d_xmlParser;

            throw;
        }

        // Set the logging level if the user didn't create a logger beforehand
        if(!userCreatedLogger)
            Logger::getSingleton().setLoggingLevel(handler.getLoggingLevel());

        // get the strings read
        configLogname       = handler.getLogFilename();
        configSchemeName    = handler.getSchemeFilename();
        configLayoutName    = handler.getLayoutFilename();
        defaultFontName     = handler.getDefaultFontName();
        configInitScript    = handler.getInitScriptFilename();
        d_termScriptName    = handler.getTermScriptFilename();

        // set default resource group if it was specified.
        if (!handler.getDefaultResourceGroup().empty())
        {
            d_resourceProvider->setDefaultResourceGroup(handler.getDefaultResourceGroup());
        }
    }

    // Start up the logger if the user didn't create a logger beforehand
    if(!userCreatedLogger)
    {
        // Prefer log filename from config file
        if (!configLogname.empty())
        {
            Logger::getSingleton().setLogFilename(configLogname, false);
        }
        // No log specified in configuration, use default / hard-coded option
        else
        {
            Logger::getSingleton().setLogFilename(logFile, false);
        }
    }

    // beginning main init
    Logger::getSingleton().logEvent("---- Begining CEGUI System initialisation ----");

    // create the core system singleton objects
    createSingletons();

    // add the window factories for the core window types
    addStandardWindowFactories();

    // GUISheet's name was changed, register an alias so both can be used
    WindowFactoryManager::getSingleton().addWindowTypeAlias("DefaultGUISheet", GUISheet::WidgetTypeName);

    // success - we are created!  Log it for prosperity :)
    outputLogHeader();

    // subscribe to hear about display mode changes
    d_rendererCon = d_renderer->subscribeEvent(Renderer::EventDisplaySizeChanged, Event::Subscriber(&CEGUI::System::handleDisplaySizeChange, this));

    // load base scheme
    if (!configSchemeName.empty())
    {
        try
        {
            SchemeManager::getSingleton().loadScheme(configSchemeName, d_resourceProvider->getDefaultResourceGroup());

            // set default font if that was specified also
            if (!defaultFontName.empty())
            {
                setDefaultFont(defaultFontName);
            }

        }
        catch (CEGUI::Exception&) {}  // catch exception and try to continue anyway

    }

    // load initial layout
    if (!configLayoutName.empty())
    {
        try
        {
            setGUISheet(WindowManager::getSingleton().loadWindowLayout(configLayoutName));
        }
        catch (CEGUI::Exception&) {}  // catch exception and try to continue anyway

    }

    // Create script module bindings
    if (d_scriptModule)
    {
        d_scriptModule->createBindings();
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
	Logger::getSingleton().logEvent("---- Begining CEGUI System destruction ----");

	// execute shut-down script
	if (!d_termScriptName.empty())
	{
		try
		{
			executeScriptFile(d_termScriptName);
		}
		catch (...) {}  // catch all exceptions and continue system shutdown

	}

    // unsubscribe from the renderer
    d_rendererCon->disconnect();

    // Cleanup script module bindings
    if (d_scriptModule)
    {
        d_scriptModule->destroyBindings();
    }

    // cleanup XML stuff
    cleanupXMLParser();

    //
	// perform cleanup in correct sequence
	//
	// destroy windows so it's safe to destroy factories
    WindowManager::getSingleton().destroyAllWindows();
    WindowManager::getSingleton().cleanDeadPool();

    // remove factories so it's safe to unload GUI modules
	WindowFactoryManager::getSingleton().removeAllFactories();

	// cleanup singletons
    destroySingletons();

	Logger::getSingleton().logEvent("CEGUI::System singleton destroyed.");
	Logger::getSingleton().logEvent("---- CEGUI System destruction completed ----");
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

		if (d_activeSheet)
		{
			d_activeSheet->render();
		}

		d_gui_redraw = false;
	}

	d_renderer->doRender();

	// draw mouse
	d_renderer->setQueueingEnabled(false);
	MouseCursor::getSingleton().draw();

    // do final destruction on dead-pool windows
    WindowManager::getSingleton().cleanDeadPool();
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
    if (sheet)
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
		setDefaultFont(0);
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
    // the default, default, is for nothing!
    if (image == (const Image*)DefaultMouseCursor)
        image = 0;

    // if mouse cursor is set to the current default we *may* need to
    // update its Image immediately (first, we will investigate further!)
    //
    // NB: The reason we do this check, is to allow code to modify the cursor
    // image directly without a call to this member changing the image back
    // again.  However, 'normal' updates to the cursor when the mouse enters
    // a window will, of course, update the mouse image as expected.
    if (MouseCursor::getSingleton().getImage() == d_defaultMouseCursor)
    {
        // does the window containing the mouse use the default cursor?
        if ((d_wndWithMouse) && (0 == d_wndWithMouse->getMouseCursor(false)))
        {
            // default cursor is active, update the image immediately
            MouseCursor::getSingleton().setImage(image);
        }
    }

    // update our pointer for the default mouse cursor image.
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
    Set the scripting module
*************************************************************************/
void System::setScriptingModule(ScriptModule* scriptModule)
{
    // cleanup old bindings
    if (d_scriptModule)
        d_scriptModule->destroyBindings();
    // set new scriptModule
    d_scriptModule = scriptModule;

    if (d_scriptModule)
    {
        // log the new ScriptModule ID string
        Logger::getSingleton().logEvent("---- Scripting module is now: "
            + d_scriptModule->getIdentifierString() + " ----");

        // create bindings on the new scriptModule
        d_scriptModule->createBindings();
    }
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
void System::executeScriptFile(const String& filename, const String& resourceGroup) const
{
	if (d_scriptModule)
	{
		try
		{
			d_scriptModule->executeScriptFile(filename, resourceGroup);
		}
		catch(...)
		{
			throw GenericException("System::executeScriptFile - An exception was thrown during the execution of the script file.");
		}

	}
	else
	{
		Logger::getSingleton().logEvent("System::executeScriptFile - the script named '" + filename +"' could not be executed as no ScriptModule is available.", Errors);
	}

}


/*************************************************************************
	Execute a scripted global function if possible.  The function should
	not take any parameters and should return an integer.
*************************************************************************/
int	System::executeScriptGlobal(const String& function_name) const
{
	if (d_scriptModule)
	{
		try
		{
			return d_scriptModule->executeScriptGlobal(function_name);
		}
		catch(...)
		{
			throw GenericException("System::executeScriptGlobal - An exception was thrown during execution of the scripted function.");
		}

	}
	else
	{
		Logger::getSingleton().logEvent("System::executeScriptGlobal - the global script function named '" + function_name +"' could not be executed as no ScriptModule is available.", Errors);
	}

	return 0;
}


/*************************************************************************
    If possible, execute script code contained in the given
    CEGUI::String object.
*************************************************************************/
void System::executeScriptString(const String& str) const
{
    if (d_scriptModule)
    {
        try
        {
            d_scriptModule->executeString(str);
        }
        catch(...)
        {
            throw GenericException("System::executeScriptString - An exception was thrown during execution of the script code.");
        }

    }
    else
    {
        Logger::getSingleton().logEvent("System::executeScriptString - the script code could not be executed as no ScriptModule is available.", Errors);
    }
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
	MouseEventArgs ma(0);
	MouseCursor& mouse = MouseCursor::getSingleton();

	ma.moveDelta.d_x = delta_x * d_mouseScalingFactor;
	ma.moveDelta.d_y = delta_y * d_mouseScalingFactor;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = 0;
	ma.clickCount = 0;
	ma.button = NoButton;

	// move the mouse cursor & update position in args.
	mouse.offsetPosition(ma.moveDelta);
	ma.position = mouse.getPosition();

	Window* dest_window = getTargetWindow(ma.position);

    // has window containing mouse changed?
    if (dest_window != d_wndWithMouse)
    {
        // store previous window that contained mouse
        Window* oldWindow = d_wndWithMouse;
        // set the new window that contains the mouse.
        d_wndWithMouse = dest_window;

        // inform previous window the mouse has left it
        if (oldWindow)
        {
            ma.window = oldWindow;
            oldWindow->onMouseLeaves(ma);
        }

        // inform window containing mouse that mouse has entered it
        if (d_wndWithMouse)
        {
            ma.window = d_wndWithMouse;
            ma.handled = false;
            d_wndWithMouse->onMouseEnters(ma);
        }
    }

    // inform appropriate window of the mouse movement event
    if (dest_window)
    {
        // ensure event starts as 'not handled'
        ma.handled = false;

        // loop backwards until event is handled or we run out of windows.
        while ((!ma.handled) && (dest_window != 0))
        {
            ma.window = dest_window;
            dest_window->onMouseMove(ma);
            dest_window = getNextTargetWindow(dest_window);
        }
    }

	return ma.handled;
}


/*************************************************************************
	Method that injects that the mouse is leaves the application window
*************************************************************************/
bool System::injectMouseLeaves(void)
{
	MouseEventArgs ma(0);

	// if there is no window that currently contains the mouse, then
	// there is nowhere to send input
	if (d_wndWithMouse)
	{
		ma.position = MouseCursor::getSingleton().getPosition();
		ma.moveDelta = Vector2(0.0f, 0.0f);
		ma.button = NoButton;
		ma.sysKeys = d_sysKeys;
		ma.wheelChange = 0;
		ma.window = d_wndWithMouse;
		ma.clickCount = 0;

		d_wndWithMouse->onMouseLeaves(ma);
		d_wndWithMouse = 0;
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

	MouseEventArgs ma(0);
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = 0;

    // find the likely destination for generated events.
    Window* dest_window = getTargetWindow(ma.position);
	
    //
	// Handling for multi-click generation
	//
	MouseClickTracker& tkr = d_clickTrackerPimpl->click_trackers[button];

	tkr.d_click_count++;

    // if multi-click requirements are not met
    if ((tkr.d_timer.elapsed() > d_dblclick_timeout) ||
        (!tkr.d_click_area.isPointInRect(ma.position)) ||
        (tkr.d_target_window != dest_window) ||
        (tkr.d_click_count > 3))
    {
        // reset to single down event.
        tkr.d_click_count = 1;

        // build new allowable area for multi-clicks
        tkr.d_click_area.setPosition(ma.position);
        tkr.d_click_area.setSize(d_dblclick_size);
        tkr.d_click_area.offset(Point(-(d_dblclick_size.d_width / 2), -(d_dblclick_size.d_height / 2)));

        // set target window for click events on this tracker
        tkr.d_target_window = dest_window;
    }

	// set click count in the event args
	ma.clickCount = tkr.d_click_count;

	// loop backwards until event is handled or we run out of windows.
	while ((!ma.handled) && (dest_window != 0))
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

		dest_window = getNextTargetWindow(dest_window);
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

	MouseEventArgs ma(0);
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = 0;

    // get the tracker that holds the number of down events seen so far for this button
    MouseClickTracker& tkr = d_clickTrackerPimpl->click_trackers[button];
    // set click count in the event args
    ma.clickCount = tkr.d_click_count;

    Window* const initial_dest_window = getTargetWindow(ma.position);
	Window* dest_window = initial_dest_window;

	// loop backwards until event is handled or we run out of windows.
	while ((!ma.handled) && (dest_window != 0))
	{
		ma.window = dest_window;
		dest_window->onMouseButtonUp(ma);
		dest_window = getNextTargetWindow(dest_window);
	}

	bool wasUpHandled = ma.handled;

    // if requirements for click events are met
    if ((tkr.d_timer.elapsed() <= d_click_timeout) &&
        (tkr.d_click_area.isPointInRect(ma.position)) &&
        (tkr.d_target_window == initial_dest_window))
    {
		ma.handled = false;
        dest_window = initial_dest_window;

		// loop backwards until event is handled or we run out of windows.
		while ((!ma.handled) && (dest_window != 0))
		{
			ma.window = dest_window;
			dest_window->onMouseClicked(ma);
			dest_window = getNextTargetWindow(dest_window);
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

	KeyEventArgs args(0);

	if (d_activeSheet)
	{
		args.scancode = (Key::Scan)key_code;
		args.sysKeys = d_sysKeys;

		Window* dest = getKeyboardTargetWindow();

		// loop backwards until event is handled or we run out of windows.
		while ((dest != 0) && (!args.handled))
		{
			args.window = dest;
			dest->onKeyDown(args);
			dest = getNextTargetWindow(dest);
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

	KeyEventArgs args(0);

	if (d_activeSheet)
	{
		args.scancode = (Key::Scan)key_code;
		args.sysKeys = d_sysKeys;

		Window* dest = getKeyboardTargetWindow();

		// loop backwards until event is handled or we run out of windows.
		while ((dest != 0) && (!args.handled))
		{
			args.window = dest;
			dest->onKeyUp(args);
			dest = getNextTargetWindow(dest);
		}

	}

	return args.handled;
}


/*************************************************************************
	Method that injects a typed character event into the system.
*************************************************************************/
bool System::injectChar(utf32 code_point)
{
	KeyEventArgs args(0);

	if (d_activeSheet)
	{
		args.codepoint = code_point;
		args.sysKeys = d_sysKeys;

		Window* dest = getKeyboardTargetWindow();

		// loop backwards until event is handled or we run out of windows.
		while ((dest != 0) && (!args.handled))
		{
			args.window = dest;
			dest->onCharacter(args);
			dest = getNextTargetWindow(dest);
		}

	}

	return args.handled;
}


/*************************************************************************
	Method that injects a mouse-wheel / scroll-wheel event into the system.
*************************************************************************/
bool System::injectMouseWheelChange(float delta)
{
	MouseEventArgs ma(0);
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = NoButton;
	ma.sysKeys = d_sysKeys;
	ma.wheelChange = delta;
	ma.clickCount = 0;

	Window* dest_window = getTargetWindow(ma.position);

	// loop backwards until event is handled or we run out of windows.
	while ((!ma.handled) && (dest_window != 0))
	{
		ma.window = dest_window;
		dest_window->onMouseWheel(ma);
		dest_window = getNextTargetWindow(dest_window);
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
	if (d_activeSheet)
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
	Window* dest_window = 0;

	// if there is no GUI sheet, then there is nowhere to send input
	if (d_activeSheet)
	{
		dest_window = Window::getCaptureWindow();

		if (!dest_window)
		{
			dest_window = d_activeSheet->getTargetChildAtPosition(pt);

			if (!dest_window)
			{
				dest_window = d_activeSheet;
			}

		}
		else
		{
            if (dest_window->distributesCapturedInputs())
            {
                Window* child_window = dest_window->getTargetChildAtPosition(pt);

                if (child_window)
                {
                    dest_window = child_window;
                }

            }

		}

		// modal target overrules
		if (d_modalTarget != 0 && dest_window != d_modalTarget)
		{
			if (!dest_window->isAncestor(d_modalTarget))
			{
				dest_window = d_modalTarget;
			}

		}

	}

	return dest_window;
}


/*************************************************************************
	Return window that should receive keyboard input
*************************************************************************/
Window* System::getKeyboardTargetWindow(void) const
{
	Window* target = 0;

	if (!d_modalTarget)
	{
		target = d_activeSheet->getActiveChild();
	}
	else
	{
		target = d_modalTarget->getActiveChild();
		if (!target)
		{
			target = d_modalTarget;
		}
	}

	return target;
}


/*************************************************************************
	Return the next window that should receive input in the chain
*************************************************************************/
Window* System::getNextTargetWindow(Window* w) const
{
	// if we have not reached the modal target, return the parent
	if (w != d_modalTarget)
	{
		return w->getParent();
	}

	// otherwise stop now
	return 0;
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
		throw InvalidRequestException("System::mouseButtonToSyskey - the parameter 'btn' is not a valid MouseButton value.");
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

	case Key::LeftAlt:
		d_lalt = direction;

		if (!d_ralt)
		{
			return Alt;
		}
		break;

	case Key::RightAlt:
		d_ralt = direction;

		if (!d_lalt)
		{
			return Alt;
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
    // here we need to inform every window using the default font that
    // it's font has been changed.
    WindowManager::WindowIterator iter = 
        WindowManager::getSingleton().getIterator();

    // Args structure we will re-use for all windows.
    WindowEventArgs args(0);

    while (!iter.isAtEnd())
    {
        Window* wnd = iter.getCurrentValue();

        if (wnd->getFont(false) == 0)
        {
            args.window = wnd;
            wnd->onFontChanged(args);
            // ensure 'handled' state is reset.
            args.handled = false;
        }

        ++iter;
    }

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
	// notify the imageset/font manager of the size change
	Size new_sz = getRenderer()->getSize();
	ImagesetManager::getSingleton().notifyScreenResolution(new_sz);
	FontManager::getSingleton().notifyScreenResolution(new_sz);

	// notify gui sheet / root if size change, event propagation will ensure everything else
	// gets updated as required.
	if (d_activeSheet)
	{
		WindowEventArgs args(0);
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
		d_wndWithMouse = 0;
	}

	if (d_activeSheet == window)
	{
		d_activeSheet = 0;
	}

	if (d_modalTarget == window)
	{
		d_modalTarget = 0;
	}

}

void System::setDefaultTooltip(Tooltip* tooltip)
{
    // destroy current custom tooltip if one exists and we created it
    if (d_defaultTooltip && d_weOwnTooltip)
        WindowManager::getSingleton().destroyWindow(d_defaultTooltip);

    // set new custom tooltip 
    d_weOwnTooltip = false;
    d_defaultTooltip = tooltip;
    d_defaultTooltip->setWritingXMLAllowed(false);
}

void System::setDefaultTooltip(const String& tooltipType)
{
    // destroy current tooltip if one exists and we created it
    if (d_defaultTooltip && d_weOwnTooltip)
        WindowManager::getSingleton().destroyWindow(d_defaultTooltip);

    if (tooltipType.empty())
    {
        d_defaultTooltip = 0;
        d_weOwnTooltip = false;
    }
    else
    {
        try
        {
            d_defaultTooltip = static_cast<Tooltip*>(WindowManager::getSingleton().createWindow(tooltipType, "CEGUI::System::default__auto_tooltip__"));
            d_weOwnTooltip = true;
            d_defaultTooltip->setWritingXMLAllowed(false);
        }
        catch(UnknownObjectException x)
        {
            d_defaultTooltip = 0;
            d_weOwnTooltip = false;
        }
    }
}

void System::outputLogHeader()
{
    Logger::getSingleton().logEvent("CEGUI::System singleton created.");
    Logger::getSingleton().logEvent("---- CEGUI System initialisation completed ----");
    Logger::getSingleton().logEvent("---- Version " + d_strVersion + " ----");
    Logger::getSingleton().logEvent("---- Renderer module is: " + d_renderer->getIdentifierString() + " ----");
    Logger::getSingleton().logEvent("---- XML Parser module is: " + d_xmlParser->getIdentifierString() + " ----");
    Logger::getSingleton().logEvent(d_scriptModule ? "---- Scripting module is: " + d_scriptModule->getIdentifierString() + " ----" : "---- Scripting module is: None ----");
}

void System::addStandardWindowFactories()
{
    // Add factories for types all base elements
    WindowFactoryManager& wfMgr = WindowFactoryManager::getSingleton();
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(GUISheet));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(DragContainer));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ScrolledContainer));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ClippedContainer));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Checkbox));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(PushButton));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(RadioButton));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Combobox));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ComboDropList));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Editbox));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(FrameWindow));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ItemEntry));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Listbox));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ListHeader));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ListHeaderSegment));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Menubar));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(PopupMenu));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(MenuItem));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(MultiColumnList));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(MultiLineEditbox));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ProgressBar));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ScrollablePane));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Scrollbar));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Slider));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Spinner));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(TabButton));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(TabControl));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Thumb));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Titlebar));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Tooltip));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(ItemListbox));
    wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(GroupBox));
	wfMgr.addFactory(&CEGUI_WINDOW_FACTORY(Tree));
}

void System::createSingletons()
{
    // cause creation of other singleton objects
    new ImagesetManager();
    new FontManager();
    new WindowFactoryManager();
    new WindowManager();
    new SchemeManager();
    new MouseCursor();
    new GlobalEventSet();
    new WidgetLookManager();
    new WindowRendererManager();
}

void System::destroySingletons()
{
    delete  SchemeManager::getSingletonPtr();
    delete  WindowManager::getSingletonPtr();
    delete  WindowFactoryManager::getSingletonPtr();
    delete  WidgetLookManager::getSingletonPtr();
    delete  WindowRendererManager::getSingletonPtr();
    delete  FontManager::getSingletonPtr();
    delete  MouseCursor::getSingletonPtr();
    delete  ImagesetManager::getSingletonPtr();
    delete  GlobalEventSet::getSingletonPtr();
}

void System::setupXMLParser()
{
    // handle creation / initialisation of XMLParser
    if (!d_xmlParser)
    {

#if !defined(CEGUI_STATIC)
        // load the dynamic module
        d_parserModule = new DynamicModule(String("CEGUI") + d_defaultXMLParserName);
        // get pointer to parser creation function
        XMLParser* (*createFunc)(void) =
            (XMLParser* (*)(void))d_parserModule->getSymbolAddress("createParser");
        // create the parser object
        d_xmlParser = createFunc();
#else
		//Static Linking Call
		d_xmlParser = createParser();
#endif
        // make sure we know to cleanup afterwards.
        d_ourXmlParser = true;
    }

    // perform initialisation of XML parser.
    d_xmlParser->initialise();
}

void System::cleanupXMLParser()
{
    if (d_xmlParser)
    {
        d_xmlParser->cleanup();

        if (d_ourXmlParser && d_parserModule)
        {
#if !defined(CEGUI_STATIC)
            // get pointer to parser deletion function
            void(*deleteFunc)(XMLParser*) =
                (void(*)(XMLParser*))d_parserModule->getSymbolAddress("destroyParser");
            // cleanup the xml parser object
            deleteFunc(d_xmlParser);
#else
			//Static Linking Call
			destroyParser(d_xmlParser);
#endif
            d_xmlParser = 0;
            // delete the dynamic module for the xml parser
            delete d_parserModule;
            d_parserModule = 0;
        }
    }
}

void System::setDefaultXMLParserName(const String& parserName)
{
#if !defined(CEGUI_STATIC)
	if(d_defaultXMLParserName == parserName)
		return;

	//We do this becuase cleanup and setup aren't static functions
	if(getSingletonPtr())
	{
		System* sys = getSingletonPtr();
		sys->cleanupXMLParser();
		d_defaultXMLParserName = parserName;
		sys->setupXMLParser();
	} // if(getSingletonPtr())
#endif
}

const String System::getDefaultXMLParserName()
{
    return d_defaultXMLParserName;
}


} // End of  CEGUI namespace section
