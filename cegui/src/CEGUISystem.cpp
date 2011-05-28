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
#include "CEGUIClipboard.h"
#include "CEGUIDefaultLogger.h"
#include "CEGUIImageManager.h"
#include "CEGUIFontManager.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUITplWindowFactory.h"
#include "CEGUIWindowManager.h"
#include "CEGUISchemeManager.h"
#include "CEGUIRenderEffectManager.h"
#include "CEGUIAnimationManager.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
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
#include "CEGUIRenderingRoot.h"
#include "CEGUIRenderingWindow.h"
#include "CEGUIRenderingContext.h"
#include "CEGUIDefaultResourceProvider.h"
#include "CEGUIImageCodec.h"
#include "elements/CEGUIAll.h"
#include <ctime>
#include <clocale>

// declare create / destroy functions used for XMLParser and ImageCodec
// modules as extern when static linking
#if defined(CEGUI_STATIC)
extern "C"
{
// XML Parser
CEGUI::XMLParser* createParser(void);
void destroyParser(CEGUI::XMLParser* parser);
// Image codec
CEGUI::ImageCodec* createImageCodec(void);
void destroyImageCodec(CEGUI::ImageCodec* imageCodec);
}
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
    double d_baseTime;

public:
    static double currentTime(); ///< returns time in seconds

    SimpleTimer() : d_baseTime(currentTime()) {}

    void restart() { d_baseTime = currentTime(); }
    double elapsed() { return currentTime() - d_baseTime; }
};

#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
double SimpleTimer::currentTime()
{
    return timeGetTime() / 1000.0;
}

#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__) || defined(__NetBSD__)  || defined(__HAIKU__)
#include <sys/time.h>
double SimpleTimer::currentTime()
{
    timeval timeStructure;
    gettimeofday(&timeStructure, 0);
    return timeStructure.tv_sec + timeStructure.tv_usec / 1000000.0;
}
#else
#error "SimpleTimer not available for this platform, please implement it"
#endif


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
	Rectf			d_click_area;		//!< area used to detect multi-clicks
    Window*         d_target_window;    //!< target window for any events generated.
};


struct MouseClickTrackerImpl
{
	MouseClickTracker	click_trackers[MouseButtonCount];
};


/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> System* Singleton<System>::ms_Singleton	= 0;

// click event generation defaults
const double	System::DefaultSingleClickTimeout	= 0.0; // was 0.2
const double	System::DefaultMultiClickTimeout	= 0.33;
const Sizef	System::DefaultMultiClickAreaSize(12,12);

// event names
const String System::EventGUISheetChanged( "GUISheetChanged" );
const String System::EventSingleClickTimeoutChanged( "SingleClickTimeoutChanged" );
const String System::EventMultiClickTimeoutChanged( "MultiClickTimeoutChanged" );
const String System::EventMultiClickAreaSizeChanged( "MultiClickAreaSizeChanged" );
const String System::EventDefaultFontChanged( "DefaultFontChanged" );
const String System::EventDefaultMouseCursorChanged( "DefaultMouseCursorChanged" );
const String System::EventMouseMoveScalingChanged( "MouseMoveScalingChanged" );
const String System::EventDisplaySizeChanged( "DisplaySizeChanged" );
const String System::EventRenderedStringParserChanged("RenderedStringParserChanged");

// Holds name of default XMLParser
String System::d_defaultXMLParserName(STRINGIZE(CEGUI_DEFAULT_XMLPARSER));
// Holds name of default ImageCodec
String System::d_defaultImageCodecName(STRINGIZE(CEGUI_DEFAULT_IMAGE_CODEC));


/*************************************************************************
    Constructor
*************************************************************************/
System::System(Renderer& renderer,
               ResourceProvider* resourceProvider,
               XMLParser* xmlParser,
               ImageCodec* imageCodec,
               ScriptModule* scriptModule,
               const String& configFile,
               const String& logFile)

: d_renderer(&renderer),
  d_resourceProvider(resourceProvider),
  d_ourResourceProvider(false),
  d_defaultFont(0),
  d_wndWithMouse(0),
  d_activeSheet(0),
  d_modalTarget(0),
  d_clipboard(CEGUI_NEW_AO Clipboard()),
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
  d_weOwnTooltip(false),
  d_imageCodec(imageCodec),
  d_ourImageCodec(false),
  d_imageCodecModule(0),
  d_ourLogger(Logger::getSingletonPtr() == 0),
  d_customRenderedStringParser(0),
  d_generateMouseClickEvents(true)
{
    // Start out by fixing the numeric locale to C (we depend on this behaviour)
    // consider a UVector2 as a property {{0.5,0},{0.5,0}} could become {{0,5,0},{0,5,0}}
    setlocale(LC_NUMERIC, "C");

    // Instantiate logger first (we have no file at this point, but entries will
    // be cached until we do)
    //
    // NOTE: If the user already created a logger prior to calling this
    // constructor, we mark it as so and leave the logger untouched. This allows
    // the user to fully customize the logger as he sees fit without fear of
    // seeing its configuration overwritten by this.
#ifdef CEGUI_HAS_DEFAULT_LOGGER
    if (d_ourLogger)
        CEGUI_NEW_AO DefaultLogger();
#endif

    Logger& logger(Logger::getSingleton());

    // create default resource provider, unless one was already provided
    if (!d_resourceProvider)
    {
        d_resourceProvider = CEGUI_NEW_AO DefaultResourceProvider();
        d_ourResourceProvider = true;
    }

    // handle initialisation and setup of the XML parser
    setupXMLParser();

    // now XML is available, read the configuration file (if any)
    Config_xmlHandler config;
    if (!configFile.empty())
    {
        CEGUI_TRY
        {
            d_xmlParser->parseXMLFile(config, configFile,
                                      config.CEGUIConfigSchemaName,
                                      "");
        }
        CEGUI_CATCH(...)
        {
            // cleanup XML stuff
            d_xmlParser->cleanup();
            CEGUI_DELETE_AO d_xmlParser;
            CEGUI_RETHROW;
        }
    }

    // Initialise logger if the user didn't create a logger beforehand
    if (d_ourLogger)
        config.initialiseLogger(logFile);

    // if we created the resource provider we know it's DefaultResourceProvider
    // so can auto-initialise the resource group directories via the config
    if (d_ourResourceProvider)
        config.initialiseResourceGroupDirectories();

    // get config to update XML parser if it needs to.
    config.initialiseXMLParser();

    // set up ImageCodec
    config.initialiseImageCodec();
    if (!d_imageCodec)
        setupImageCodec("");

    // initialise any default resource groups specified in the config.
    config.initialiseDefaultResourceGroups();

    initialiseVersionString();
    outputLogHeader();

    // beginning main init
    logger.logEvent("---- Begining CEGUI System initialisation ----");

    // create the core system singleton objects
    createSingletons();

    // add the window factories for the core window types
    addStandardWindowFactories();

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    logger.logEvent("CEGUI::System singleton created. " + String(addr_buff));
    logger.logEvent("---- CEGUI System initialisation completed ----");
    logger.logEvent("");

    // autoload resources specified in config
    config.loadAutoResources();

    // set up defaults
    config.initialiseDefaultFont();
    config.initialiseDefaultMouseCursor();
    config.initialiseDefaulTooltip();

    // scripting available?
    if (d_scriptModule)
    {
        d_scriptModule->createBindings();
        config.executeInitScript();
        d_termScriptName = config.getTerminateScriptName();
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
		CEGUI_TRY
		{
			executeScriptFile(d_termScriptName);
		}
		CEGUI_CATCH (...) {}  // catch all exceptions and continue system shutdown

	}

    cleanupImageCodec();

    // cleanup XML stuff
    cleanupXMLParser();

    //
	// perform cleanup in correct sequence
	//
    // ensure no windows get created during destruction.  NB: I'm allowing the
    // potential exception to escape here so as to make it obvious that client
    // code should really be adjusted to not create windows during cleanup.
    WindowManager::getSingleton().lock();
	// destroy windows so it's safe to destroy factories
    WindowManager::getSingleton().destroyAllWindows();
    WindowManager::getSingleton().cleanDeadPool();

    // remove factories so it's safe to unload GUI modules
	WindowFactoryManager::getSingleton().removeAllFactories();

    // Cleanup script module bindings
    if (d_scriptModule)
        d_scriptModule->destroyBindings();

	// cleanup singletons
    destroySingletons();

    // cleanup resource provider if we own it
    if (d_ourResourceProvider)
        CEGUI_DELETE_AO d_resourceProvider;

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
	Logger::getSingleton().logEvent("CEGUI::System singleton destroyed. " +
       String(addr_buff));
	Logger::getSingleton().logEvent("---- CEGUI System destruction completed ----");

#ifdef CEGUI_HAS_DEFAULT_LOGGER
    // delete the Logger object only if we created it.
    if (d_ourLogger)
        CEGUI_DELETE_AO Logger::getSingletonPtr();
#endif

	delete d_clickTrackerPimpl;
    
    CEGUI_DELETE_AO d_clipboard;
}


/*************************************************************************
	Render the GUI for this frame
*************************************************************************/
void System::renderGUI(void)
{
    d_renderer->beginRendering();

	if (d_gui_redraw)
	{
		if (d_activeSheet)
		{
            RenderingSurface& rs = d_activeSheet->getTargetRenderingSurface();
            rs.clearGeometry();

            if (rs.isRenderingWindow())
                static_cast<RenderingWindow&>(rs).getOwner().clearGeometry();

			d_activeSheet->render();
		}
        // no sheet, so ensure default surface geometry is cleared
        else
            d_renderer->getDefaultRenderingRoot().clearGeometry();

		d_gui_redraw = false;
	}

    d_renderer->getDefaultRenderingRoot().draw();
	MouseCursor::getSingleton().draw();
    d_renderer->endRendering();

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
		setDefaultFont(&FontManager::getSingleton().get(name));
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
    const Image* const default_cursor =
        reinterpret_cast<const Image*>(DefaultMouseCursor);

    // the default, default, is for nothing!
    if (image == default_cursor)
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
        if ((d_wndWithMouse) &&
            (default_cursor == d_wndWithMouse->getMouseCursor(false)))
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
void System::setDefaultMouseCursor(const String& name)
{
	setDefaultMouseCursor(
        &ImageManager::getSingleton().get(name));
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
		CEGUI_TRY
		{
			d_scriptModule->executeScriptFile(filename, resourceGroup);
		}
        // Forward script exceptions with line number and file info
        CEGUI_CATCH(ScriptException& e)
        {
            CEGUI_THROW(e);
        }
		CEGUI_CATCH(...)
		{
			CEGUI_THROW(GenericException("System::executeScriptFile - An exception was thrown during the execution of the script file."));
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
		CEGUI_TRY
		{
			return d_scriptModule->executeScriptGlobal(function_name);
		}
        // Forward script exceptions with line number and file info
        CEGUI_CATCH(ScriptException& e)
        {
            CEGUI_THROW(e);
        }
		CEGUI_CATCH(...)
		{
			CEGUI_THROW(GenericException("System::executeScriptGlobal - An exception was thrown during execution of the scripted function."));
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
        CEGUI_TRY
        {
            d_scriptModule->executeString(str);
        }
        // Forward script exceptions with line number and file info
        CEGUI_CATCH(ScriptException& e)
        {
            CEGUI_THROW(e);
        }
        CEGUI_CATCH(...)
        {
            CEGUI_THROW(GenericException("System::executeScriptString - An exception was thrown during execution of the script code."));
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
    ma.moveDelta.d_x = delta_x * d_mouseScalingFactor;
    ma.moveDelta.d_y = delta_y * d_mouseScalingFactor;

    // no movement means no event
    if ((ma.moveDelta.d_x == 0) && (ma.moveDelta.d_y == 0))
        return false;

    ma.sysKeys = d_sysKeys;
    ma.wheelChange = 0;
    ma.clickCount = 0;
    ma.button = NoButton;

    // move the mouse cursor & update position in args.
    MouseCursor& mouse(MouseCursor::getSingleton());
    mouse.offsetPosition(ma.moveDelta);
    ma.position = mouse.getPosition();

    return mouseMoveInjection_impl(ma);
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
		ma.position = d_wndWithMouse->
          getUnprojectedPosition(MouseCursor::getSingleton().getPosition());
		ma.moveDelta = Vector2f(0.0f, 0.0f);
		ma.button = NoButton;
		ma.sysKeys = d_sysKeys;
		ma.wheelChange = 0;
		ma.window = d_wndWithMouse;
		ma.clickCount = 0;

		d_wndWithMouse->onMouseLeaves(ma);
		d_wndWithMouse = 0;
	}

	return ma.handled != 0;
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
    ma.moveDelta = Vector2f(0.0f, 0.0f);
    ma.button = button;
    ma.sysKeys = d_sysKeys;
    ma.wheelChange = 0;
    ma.window = getTargetWindow(ma.position, false);
    // make mouse position sane for this target window
    if (ma.window)
        ma.position = ma.window->getUnprojectedPosition(ma.position);

    //
    // Handling for multi-click generation
    //
    MouseClickTracker& tkr = d_clickTrackerPimpl->click_trackers[button];

    tkr.d_click_count++;

    // if multi-click requirements are not met
    if (((d_dblclick_timeout > 0) && (tkr.d_timer.elapsed() > d_dblclick_timeout)) ||
        (!tkr.d_click_area.isPointInRect(ma.position)) ||
        (tkr.d_target_window != ma.window) ||
        (tkr.d_click_count > 3))
    {
        // reset to single down event.
        tkr.d_click_count = 1;

        // build new allowable area for multi-clicks
        tkr.d_click_area.setPosition(ma.position);
        tkr.d_click_area.setSize(d_dblclick_size);
        tkr.d_click_area.offset(Vector2f(-(d_dblclick_size.d_width / 2), -(d_dblclick_size.d_height / 2)));

        // set target window for click events on this tracker
        tkr.d_target_window = ma.window;
    }

    // set click count in the event args
    ma.clickCount = tkr.d_click_count;

    if (ma.window)
    {
        if (d_generateMouseClickEvents && ma.window->wantsMultiClickEvents())
        {
            switch (tkr.d_click_count)
            {
            case 1:
                ma.window->onMouseButtonDown(ma);
                break;

            case 2:
                ma.window->onMouseDoubleClicked(ma);
                break;

            case 3:
                ma.window->onMouseTripleClicked(ma);
                break;
            }
        }
        // click generation disabled, or current target window does not want
        // multi-clicks, so just send a mouse down event instead.
        else
        {
            ma.window->onMouseButtonDown(ma);
        }
    }

    // reset timer for this tracker.
    tkr.d_timer.restart();

    return ma.handled != 0;
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
    ma.moveDelta = Vector2f(0.0f, 0.0f);
    ma.button = button;
    ma.sysKeys = d_sysKeys;
    ma.wheelChange = 0;
    ma.window = getTargetWindow(ma.position, false);
    // make mouse position sane for this target window
    if (ma.window)
        ma.position = ma.window->getUnprojectedPosition(ma.position);

    // get the tracker that holds the number of down events seen so far for this button
    MouseClickTracker& tkr = d_clickTrackerPimpl->click_trackers[button];
    // set click count in the event args
    ma.clickCount = tkr.d_click_count;

    // if there is no window, inputs can not be handled.
    if (!ma.window)
        return false;

    // store original window becase we re-use the event args.
    Window* const tgt_wnd = ma.window;

    // send 'up' input to the window
    ma.window->onMouseButtonUp(ma);
    // store whether the 'up' part was handled so we may reuse the EventArgs
    const uint upHandled = ma.handled;

    // restore target window (because Window::on* may have propagated input)
    ma.window = tgt_wnd;

    // send MouseClicked event if the requirements for that were met
    if (d_generateMouseClickEvents &&
        ((d_click_timeout == 0) || (tkr.d_timer.elapsed() <= d_click_timeout)) &&
        (tkr.d_click_area.isPointInRect(ma.position)) &&
        (tkr.d_target_window == ma.window))
    {
        ma.handled = 0;
        ma.window->onMouseClicked(ma);
    }

    return (ma.handled + upHandled) != 0;
}


/*************************************************************************
	Method that injects a key down event into the system.
*************************************************************************/
bool System::injectKeyDown(uint key_code)
{
    // update system keys
    d_sysKeys |= keyCodeToSyskey((Key::Scan)key_code, true);

    KeyEventArgs args(getKeyboardTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.scancode = (Key::Scan)key_code;
    args.sysKeys = d_sysKeys;

    args.window->onKeyDown(args);
    return args.handled != 0;
}


/*************************************************************************
	Method that injects a key up event into the system.
*************************************************************************/
bool System::injectKeyUp(uint key_code)
{
    // update system keys
    d_sysKeys &= ~keyCodeToSyskey((Key::Scan)key_code, false);

    KeyEventArgs args(getKeyboardTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.scancode = (Key::Scan)key_code;
    args.sysKeys = d_sysKeys;

    args.window->onKeyUp(args);
    return args.handled != 0;
}


/*************************************************************************
	Method that injects a typed character event into the system.
*************************************************************************/
bool System::injectChar(String::value_type code_point)
{
    KeyEventArgs args(getKeyboardTargetWindow());

    // if there's no destination window, input can't be handled.
    if (!args.window)
        return false;

    args.codepoint = code_point;
    args.sysKeys = d_sysKeys;

    args.window->onCharacter(args);
    return args.handled != 0;
}


/*************************************************************************
	Method that injects a mouse-wheel / scroll-wheel event into the system.
*************************************************************************/
bool System::injectMouseWheelChange(float delta)
{
    MouseEventArgs ma(0);
    ma.position = MouseCursor::getSingleton().getPosition();
    ma.moveDelta = Vector2f(0.0f, 0.0f);
    ma.button = NoButton;
    ma.sysKeys = d_sysKeys;
    ma.wheelChange = delta;
    ma.clickCount = 0;
    ma.window = getTargetWindow(ma.position, false);
    // make mouse position sane for this target window
    if (ma.window)
        ma.position = ma.window->getUnprojectedPosition(ma.position);

    // if there is no target window, input can not be handled.
    if (!ma.window)
        return false;

    ma.window->onMouseWheel(ma);
    return ma.handled != 0;
}


/*************************************************************************
	Method that injects a new position for the mouse cursor.
*************************************************************************/
bool System::injectMousePosition(float x_pos, float y_pos)
{
    const Vector2f new_position(x_pos, y_pos);
    MouseCursor& mouse(MouseCursor::getSingleton());

    // setup mouse movement event args object.
    MouseEventArgs ma(0);
    ma.moveDelta = new_position - mouse.getPosition();

    // no movement means no event
    if ((ma.moveDelta.d_x == 0) && (ma.moveDelta.d_y == 0))
        return false;

    ma.sysKeys = d_sysKeys;
    ma.wheelChange = 0;
    ma.clickCount = 0;
    ma.button = NoButton;

    // move mouse cursor to new position
    mouse.setPosition(new_position);
    // update position in args (since actual position may be constrained)
    ma.position = mouse.getPosition();

    return mouseMoveInjection_impl(ma);
}


/*************************************************************************
	Method to inject time pulses into the system.
*************************************************************************/
bool System::injectTimePulse(float timeElapsed)
{
    AnimationManager::getSingleton().stepInstances(timeElapsed);

    // if no visible active sheet, input can't be handled
    if (!d_activeSheet || !d_activeSheet->isEffectiveVisible())
        return false;

    // else pass to sheet for distribution.
    d_activeSheet->update(timeElapsed);
    // this input is then /always/ considered handled.
    return true;
}


/*************************************************************************
	Return window that should get mouse inouts when mouse it at 'pt'
*************************************************************************/
Window* System::getTargetWindow(const Vector2f& pt,
                                const bool allow_disabled) const
{
    // if there is no GUI sheet visible, then there is nowhere to send input
    if (!d_activeSheet || !d_activeSheet->isEffectiveVisible())
        return 0;

    Window* dest_window = Window::getCaptureWindow();

    if (!dest_window)
    {
        dest_window = d_activeSheet->
            getTargetChildAtPosition(pt, allow_disabled);

        if (!dest_window)
            dest_window = d_activeSheet;
    }
    else
    {
        if (dest_window->distributesCapturedInputs())
        {
            Window* child_window = dest_window->
                getTargetChildAtPosition(pt, allow_disabled);

            if (child_window)
                dest_window = child_window;
        }
    }

    // modal target overrules
    if (d_modalTarget && dest_window != d_modalTarget)
        if (!dest_window->isAncestor(d_modalTarget))
            dest_window = d_modalTarget;

    return dest_window;
}


/*************************************************************************
	Return window that should receive keyboard input
*************************************************************************/
Window* System::getKeyboardTargetWindow(void) const
{
    // if no active sheet, there is no target widow.
    if (!d_activeSheet || !d_activeSheet->isEffectiveVisible())
        return 0;

    // handle normal non-modal situations
    if (!d_modalTarget)
        return d_activeSheet->getActiveChild();

    // handle possible modal window.
    Window* const target = d_modalTarget->getActiveChild();
    return target ? target : d_modalTarget;
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
		CEGUI_THROW(InvalidRequestException("System::mouseButtonToSyskey - the parameter 'btn' is not a valid MouseButton value."));
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
void System::setMultiClickToleranceAreaSize(const Sizef& sz)
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
            args.handled = 0;
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
void System::notifyDisplaySizeChanged(const Sizef& new_size)
{
    // notify other components of the display size change
    d_renderer->setDisplaySize(new_size);
    ImageManager::getSingleton().notifyDisplaySizeChanged(new_size);
    FontManager::getSingleton().notifyDisplaySizeChanged(new_size);
    MouseCursor::getSingleton().notifyDisplaySizeChanged(new_size);

	// notify gui sheet / root if size change, event propagation will ensure everything else
	// gets updated as required.
    //
    // FIXME: This is no longer correct, the RenderTarget the sheet is using as
    // FIXME: it's parent element may not be the main screen.
	if (d_activeSheet)
	{
		WindowEventArgs args(0);
		d_activeSheet->onParentSized(args);
    }

    invalidateAllWindows();

    // Fire event
    DisplayEventArgs args(new_size);
    fireEvent(EventDisplaySizeChanged, args, EventNamespace);

    Logger::getSingleton().logEvent(
        "Display resize:"
        " w=" + PropertyHelper<float>::toString(new_size.d_width) +
        " h=" + PropertyHelper<float>::toString(new_size.d_height));
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

    if (d_defaultTooltip == window)
    {
        d_defaultTooltip = 0;
        d_weOwnTooltip = false;
    }
}

void System::setDefaultTooltip(Tooltip* tooltip)
{
    destroySystemOwnedDefaultTooltipWindow();

    d_defaultTooltip = tooltip;

    if (d_defaultTooltip)
        d_defaultTooltip->setWritingXMLAllowed(false);
}

void System::setDefaultTooltip(const String& tooltipType)
{
    destroySystemOwnedDefaultTooltipWindow();

    d_defaultTooltipType = tooltipType;
}

//----------------------------------------------------------------------------//
void System::createSystemOwnedDefaultTooltipWindow() const
{
    WindowManager& winmgr(WindowManager::getSingleton());

    if (!winmgr.isLocked())
    {
        d_defaultTooltip = static_cast<Tooltip*>(
            winmgr.createWindow(d_defaultTooltipType,
                                "CEGUI::System::default__auto_tooltip__"));
        d_defaultTooltip->setWritingXMLAllowed(false);
        d_weOwnTooltip = true;
    }
}

//----------------------------------------------------------------------------//
void System::destroySystemOwnedDefaultTooltipWindow()
{
    if (d_defaultTooltip && d_weOwnTooltip)
    {
        WindowManager::getSingleton().destroyWindow(d_defaultTooltip);
        d_defaultTooltip = 0;
    }

    d_weOwnTooltip = false;
}

//----------------------------------------------------------------------------//
Tooltip* System::getDefaultTooltip(void) const
{
    if (!d_defaultTooltip && !d_defaultTooltipType.empty())
        createSystemOwnedDefaultTooltipWindow();

    return d_defaultTooltip;
}

//----------------------------------------------------------------------------//

void System::outputLogHeader()
{
    Logger& l(Logger::getSingleton());
    l.logEvent("");
    l.logEvent("********************************************************************************");
    l.logEvent("* Important:                                                                   *");
    l.logEvent("*     To get support at the CEGUI forums, you must post _at least_ the section *");
    l.logEvent("*     of this log file indicated below.  Failure to do this will result in no  *");
    l.logEvent("*     support being given; please do not waste our time.                       *");
    l.logEvent("********************************************************************************");
    l.logEvent("********************************************************************************");
    l.logEvent("* -------- START OF ESSENTIAL SECTION TO BE POSTED ON THE FORUM       -------- *");
    l.logEvent("********************************************************************************");
    l.logEvent("---- Version " + d_strVersion + " ----");
    l.logEvent("---- Renderer module is: " + d_renderer->getIdentifierString() + " ----");
    l.logEvent("---- XML Parser module is: " + d_xmlParser->getIdentifierString() + " ----");
    l.logEvent("---- Image Codec module is: " + d_imageCodec->getIdentifierString() + " ----");
    l.logEvent(d_scriptModule ? "---- Scripting module is: " + d_scriptModule->getIdentifierString() + " ----" : "---- Scripting module is: None ----");
    l.logEvent("********************************************************************************");
    l.logEvent("* -------- END OF ESSENTIAL SECTION TO BE POSTED ON THE FORUM         -------- *");
    l.logEvent("********************************************************************************");
    l.logEvent("");
}

void System::addStandardWindowFactories()
{
    // Add factories for types all base elements
    WindowFactoryManager::addFactory< TplWindowFactory<DefaultWindow> >();
    WindowFactoryManager::addFactory< TplWindowFactory<DragContainer> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ScrolledContainer> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ClippedContainer> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Checkbox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<PushButton> >();
    WindowFactoryManager::addFactory< TplWindowFactory<RadioButton> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Combobox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ComboDropList> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Editbox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<FrameWindow> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ItemEntry> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Listbox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ListHeader> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ListHeaderSegment> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Menubar> >();
    WindowFactoryManager::addFactory< TplWindowFactory<PopupMenu> >();
    WindowFactoryManager::addFactory< TplWindowFactory<MenuItem> >();
    WindowFactoryManager::addFactory< TplWindowFactory<MultiColumnList> >();
    WindowFactoryManager::addFactory< TplWindowFactory<MultiLineEditbox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ProgressBar> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ScrollablePane> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Scrollbar> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Slider> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Spinner> >();
    WindowFactoryManager::addFactory< TplWindowFactory<TabButton> >();
    WindowFactoryManager::addFactory< TplWindowFactory<TabControl> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Thumb> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Titlebar> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Tooltip> >();
    WindowFactoryManager::addFactory< TplWindowFactory<ItemListbox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<GroupBox> >();
    WindowFactoryManager::addFactory< TplWindowFactory<Tree> >();
    WindowFactoryManager::addFactory< TplWindowFactory<LayoutCell> >();
    WindowFactoryManager::addFactory< TplWindowFactory<HorizontalLayoutContainer> >();
    WindowFactoryManager::addFactory< TplWindowFactory<VerticalLayoutContainer> >();
    WindowFactoryManager::addFactory< TplWindowFactory<GridLayoutContainer> >();
}

void System::createSingletons()
{
    // cause creation of other singleton objects
    CEGUI_NEW_AO ImageManager();
    CEGUI_NEW_AO FontManager();
    CEGUI_NEW_AO WindowFactoryManager();
    CEGUI_NEW_AO WindowManager();
    CEGUI_NEW_AO SchemeManager();
    CEGUI_NEW_AO MouseCursor();
    CEGUI_NEW_AO GlobalEventSet();
    CEGUI_NEW_AO AnimationManager();
    CEGUI_NEW_AO WidgetLookManager();
    CEGUI_NEW_AO WindowRendererManager();
    CEGUI_NEW_AO RenderEffectManager();
}

void System::destroySingletons()
{
    CEGUI_DELETE_AO SchemeManager::getSingletonPtr();
    CEGUI_DELETE_AO WindowManager::getSingletonPtr();
    CEGUI_DELETE_AO WindowFactoryManager::getSingletonPtr();
    CEGUI_DELETE_AO WidgetLookManager::getSingletonPtr();
    CEGUI_DELETE_AO WindowRendererManager::getSingletonPtr();
    CEGUI_DELETE_AO AnimationManager::getSingletonPtr();
    CEGUI_DELETE_AO RenderEffectManager::getSingletonPtr();
    CEGUI_DELETE_AO FontManager::getSingletonPtr();
    CEGUI_DELETE_AO MouseCursor::getSingletonPtr();
    CEGUI_DELETE_AO ImageManager::getSingletonPtr();
    CEGUI_DELETE_AO GlobalEventSet::getSingletonPtr();
}

//----------------------------------------------------------------------------//
void System::setupXMLParser()
{
    // handle creation / initialisation of XMLParser
    if (!d_xmlParser)
    {
#ifndef CEGUI_STATIC
        setXMLParser(d_defaultXMLParserName);
#else
        //Static Linking Call
        d_xmlParser = createParser();
        // make sure we know to cleanup afterwards.
        d_ourXmlParser = true;
		d_xmlParser->initialise();
#endif
    }
    // parser object already set, just initialise it.
    else
        d_xmlParser->initialise();
}

//----------------------------------------------------------------------------//
void System::cleanupXMLParser()
{
    // bail out if no parser
    if (!d_xmlParser)
        return;

    // get parser object to do whatever cleanup it needs to
    d_xmlParser->cleanup();

    // exit if we did not create this parser object
    if (!d_ourXmlParser)
        return;

    // if parser module loaded, destroy the parser object & cleanup module
    if (d_parserModule)
    {
        // get pointer to parser deletion function
        void(*deleteFunc)(XMLParser*) = (void(*)(XMLParser*))d_parserModule->
            getSymbolAddress("destroyParser");
        // cleanup the xml parser object
        deleteFunc(d_xmlParser);

        // delete the dynamic module for the xml parser
        CEGUI_DELETE_AO d_parserModule;
        d_parserModule = 0;
    }
#ifdef CEGUI_STATIC
    else
        //Static Linking Call
        destroyParser(d_xmlParser);
#endif

    d_xmlParser = 0;
}

//----------------------------------------------------------------------------//
void System::setXMLParser(const String& parserName)
{
#ifndef CEGUI_STATIC
    cleanupXMLParser();
    // load dynamic module
    d_parserModule = CEGUI_NEW_AO DynamicModule(String("CEGUI") + parserName);
    // get pointer to parser creation function
    XMLParser* (*createFunc)(void) =
        (XMLParser* (*)(void))d_parserModule->getSymbolAddress("createParser");
    // create the parser object
    d_xmlParser = createFunc();
    // make sure we know to cleanup afterwards.
    d_ourXmlParser = true;
    // perform initialisation of XML parser.
    d_xmlParser->initialise();
#else
    Logger::getSingleton().logEvent(
        "System::setXMLParser(const String& parserName) called from statically "
        "linked CEGUI library - unable to load dynamic module!", Errors);
#endif
}

//----------------------------------------------------------------------------//
void System::setXMLParser(XMLParser* parser)
{
    cleanupXMLParser();
    d_xmlParser = parser;
    d_ourXmlParser = false;
    setupXMLParser();
}

//----------------------------------------------------------------------------//
void System::setDefaultXMLParserName(const String& parserName)
{
    d_defaultXMLParserName = parserName;
}

const String System::getDefaultXMLParserName()
{
    return d_defaultXMLParserName;
}

//----------------------------------------------------------------------------//
bool System::mouseMoveInjection_impl(MouseEventArgs& ma)
{
    updateWindowContainingMouse();

    // input can't be handled if there is no window to handle it.
    if (!d_wndWithMouse)
        return false;

    // make mouse position sane for this target window
    ma.position = d_wndWithMouse->getUnprojectedPosition(ma.position);
    // inform window about the input.
    ma.window = d_wndWithMouse;
    ma.handled = 0;
    ma.window->onMouseMove(ma);

    // return whether window handled the input.
    return ma.handled != 0;
}

//----------------------------------------------------------------------------//
Window* System::getCommonAncestor(Window* w1, Window* w2)
{
    if (!w2)
        return w2;

    if (w1 == w2)
        return w1;

    // make sure w1 is always further up
    if (w1 && w1->isAncestor(w2))
        return w2;

    while (w1)
    {
        if (w2->isAncestor(w1))
            break;

        w1 = w1->getParent();
    }

    return w1;
}

//----------------------------------------------------------------------------//
void System::notifyMouseTransition(Window* top, Window* bottom,
                                   void (Window::*func)(MouseEventArgs&),
                                   MouseEventArgs& args)
{
    if (top == bottom)
        return;
    
    Window* const parent = bottom->getParent();

    if (parent && parent != top)
        notifyMouseTransition(top, parent, func, args);

    args.handled = 0;
    args.window = bottom;

    (bottom->*func)(args);
}

//----------------------------------------------------------------------------//
bool System::updateWindowContainingMouse()
{
    MouseEventArgs ma(0);
    const Vector2f mouse_pos(MouseCursor::getSingleton().getPosition());

    Window* const curr_wnd_with_mouse = getTargetWindow(mouse_pos, true);

    // exit if window containing mouse has not changed.
    if (curr_wnd_with_mouse == d_wndWithMouse)
        return false;

    ma.sysKeys = d_sysKeys;
    ma.wheelChange = 0;
    ma.clickCount = 0;
    ma.button = NoButton;

    Window* oldWindow = d_wndWithMouse;
    d_wndWithMouse = curr_wnd_with_mouse;

    // inform previous window the mouse has left it
    if (oldWindow)
    {
        ma.window = oldWindow;
        ma.position = oldWindow->getUnprojectedPosition(mouse_pos);
        oldWindow->onMouseLeaves(ma);
    }

    // inform window containing mouse that mouse has entered it
    if (d_wndWithMouse)
    {
        ma.handled = 0;
        ma.window = d_wndWithMouse;
        ma.position = d_wndWithMouse->getUnprojectedPosition(mouse_pos);
        d_wndWithMouse->onMouseEnters(ma);
    }

    // do the 'area' version of the events
    Window* root = getCommonAncestor(oldWindow, d_wndWithMouse);

    if (oldWindow)
        notifyMouseTransition(root, oldWindow, &Window::onMouseLeavesArea, ma);

    if (d_wndWithMouse)
        notifyMouseTransition(root, d_wndWithMouse, &Window::onMouseEntersArea, ma);

    return true;
}

//----------------------------------------------------------------------------//
ImageCodec& System::getImageCodec() const
{
    return *d_imageCodec;
}

//----------------------------------------------------------------------------//
void System::setImageCodec(const String& codecName)
{
    setupImageCodec(codecName);
}

//----------------------------------------------------------------------------//
void System::setImageCodec(ImageCodec& codec)
{
    cleanupImageCodec();
    d_imageCodec = &codec;
    d_ourImageCodec = false;
    d_imageCodecModule = 0;
}

//----------------------------------------------------------------------------//
void System::setupImageCodec(const String& codecName)
{
    // Cleanup the old image codec
    cleanupImageCodec();

    #if defined(CEGUI_STATIC)
        // for static build use static createImageCodec to create codec object
        d_imageCodec = createImageCodec();
    #else
        // load the appropriate image codec module
        d_imageCodecModule = codecName.empty() ?
            new DynamicModule(String("CEGUI") + d_defaultImageCodecName) :
            new DynamicModule(String("CEGUI") + codecName);

        // use function from module to create the codec object.
        d_imageCodec = ((ImageCodec*(*)(void))d_imageCodecModule->
            getSymbolAddress("createImageCodec"))();
    #endif

    // make sure we mark this as our own object so we can clean it up later.
    d_ourImageCodec = true;
}

//----------------------------------------------------------------------------//
void System::cleanupImageCodec()
{
    // bail out if no codec, or if we did not create it.
    if (!d_imageCodec || !d_ourImageCodec)
        return;

    if (d_imageCodecModule)
    {
        ((void(*)(ImageCodec*))d_imageCodecModule->
            getSymbolAddress("destroyImageCodec"))(d_imageCodec);

        CEGUI_DELETE_AO d_imageCodecModule;
        d_imageCodecModule = 0;
    }
#if defined(CEGUI_STATIC)
    else
        destroyImageCodec(d_imageCodec);
#endif

    d_imageCodec = 0;
}

//----------------------------------------------------------------------------//
void System::setDefaultImageCodecName(const String& codecName)
{
    d_defaultImageCodecName = codecName;
}

//----------------------------------------------------------------------------//
const String& System::getDefaultImageCodecName()
{
    return d_defaultImageCodecName;
}

//----------------------------------------------------------------------------//
void System::initialiseVersionString()
{
    d_strVersion = PropertyHelper<uint>::toString(CEGUI_VERSION_MAJOR) + "." +
       PropertyHelper<uint>::toString(CEGUI_VERSION_MINOR) + "." +
       PropertyHelper<uint>::toString(CEGUI_VERSION_PATCH);

    d_strVersion += " (Build: " __DATE__;

#if defined(CEGUI_STATIC)
    d_strVersion += " Static";
#endif

#if defined(DEBUG) || defined(_DEBUG)
    d_strVersion += " Debug";
#endif

#if defined(__linux__)
    d_strVersion += " GNU/Linux";
#elif defined (__FreeBSD__)
    d_strVersion += " FreeBSD";
#elif defined (__APPLE__)
    d_strVersion += " Apple Mac";
#elif defined (_WIN32) || defined (__WIN32__)
    d_strVersion += " Microsoft Windows";
#endif

#ifdef __GNUG__
    d_strVersion += " g++ " __VERSION__;

#ifdef _LP64
    d_strVersion += " 64 bit";
#else
    d_strVersion += " 32 bit";
#endif

#elif defined(_MSC_VER)
    d_strVersion += " MSVC++ ";
#if _MSC_VER <= 1200
    d_strVersion += "Dinosaur Edition!";
#elif _MSC_VER == 1300
    d_strVersion += "7.0";
#elif _MSC_VER == 1310
    d_strVersion += "7.1";
#elif _MSC_VER == 1400
    d_strVersion += "8.0";
#elif _MSC_VER == 1500
    d_strVersion += "9.0";
#elif _MSC_VER == 1600
    d_strVersion += "10.0";
#elif _MSC_VER > 1600
    d_strVersion += "Great Scott!";
#endif

#ifdef _WIN64
    d_strVersion += " 64 bit";
#else
    d_strVersion += " 32 bit";
#endif

#endif

    d_strVersion += ")";
}

//----------------------------------------------------------------------------//
System& System::create(Renderer& renderer, ResourceProvider* resourceProvider,
                       XMLParser* xmlParser, ImageCodec* imageCodec,
                       ScriptModule* scriptModule, const String& configFile,
                       const String& logFile)
{
    return *CEGUI_NEW_AO System(renderer, resourceProvider, xmlParser, imageCodec,
                       scriptModule, configFile, logFile);
}

//----------------------------------------------------------------------------//
void System::destroy()
{
    CEGUI_DELETE_AO System::getSingletonPtr();
}

//----------------------------------------------------------------------------//
RenderedStringParser* System::getDefaultCustomRenderedStringParser() const
{
    return d_customRenderedStringParser;
}

//----------------------------------------------------------------------------//
void System::setDefaultCustomRenderedStringParser(RenderedStringParser* parser)
{
    if (parser != d_customRenderedStringParser)
    {
        d_customRenderedStringParser = parser;
        
        // fire event
        EventArgs args;
        fireEvent(EventRenderedStringParserChanged, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//
bool System::isMouseClickEventGenerationEnabled() const
{
    return d_generateMouseClickEvents;
}

//----------------------------------------------------------------------------//
void System::setMouseClickEventGenerationEnabled(const bool enable)
{
    d_generateMouseClickEvents = enable;
}

//----------------------------------------------------------------------------//
bool System::injectMouseButtonClick(const MouseButton button)
{
    MouseEventArgs ma(0);
    ma.position = MouseCursor::getSingleton().getPosition();
    ma.window = getTargetWindow(ma.position, false);

    if (ma.window)
    {
        // initialise remainder of args struct.
        ma.moveDelta = Vector2f(0.0f, 0.0f);
        ma.button = button;
        ma.sysKeys = d_sysKeys;
        ma.wheelChange = 0;
        // make mouse position sane for this target window
        ma.position = ma.window->getUnprojectedPosition(ma.position);
        // tell the window about the event.
        ma.window->onMouseClicked(ma);
    }

    return ma.handled != 0;
}

//----------------------------------------------------------------------------//
bool System::injectMouseButtonDoubleClick(const MouseButton button)
{
    MouseEventArgs ma(0);
    ma.position = MouseCursor::getSingleton().getPosition();
    ma.window = getTargetWindow(ma.position, false);

    if (ma.window && ma.window->wantsMultiClickEvents())
    {
        // initialise remainder of args struct.
        ma.moveDelta = Vector2f(0.0f, 0.0f);
        ma.button = button;
        ma.sysKeys = d_sysKeys;
        ma.wheelChange = 0;
        // make mouse position sane for this target window
        ma.position = ma.window->getUnprojectedPosition(ma.position);
        // tell the window about the event.
        ma.window->onMouseDoubleClicked(ma);
    }

    return ma.handled != 0;
}

//----------------------------------------------------------------------------//
bool System::injectMouseButtonTripleClick(const MouseButton button)
{
    MouseEventArgs ma(0);
    ma.position = MouseCursor::getSingleton().getPosition();
    ma.window = getTargetWindow(ma.position, false);

    if (ma.window && ma.window->wantsMultiClickEvents())
    {
        // initialise remainder of args struct.
        ma.moveDelta = Vector2f(0.0f, 0.0f);
        ma.button = button;
        ma.sysKeys = d_sysKeys;
        ma.wheelChange = 0;
        // make mouse position sane for this target window
        ma.position = ma.window->getUnprojectedPosition(ma.position);
        // tell the window about the event.
        ma.window->onMouseTripleClicked(ma);
    }

    return ma.handled != 0;
}

bool System::injectCopyRequest()
{
    Window* source = getKeyboardTargetWindow();
    return source ? source->performCopy(*d_clipboard) : false;
}

bool System::injectCutRequest()
{
    Window* source = getKeyboardTargetWindow();
    return source ? source->performCut(*d_clipboard) : false;
}

bool System::injectPasteRequest()
{
    Window* source = getKeyboardTargetWindow();
    return source ? source->performPaste(*d_clipboard) : false;
}

//----------------------------------------------------------------------------//
void System::invalidateAllCachedRendering()
{
    invalidateAllWindows();
    MouseCursor::getSingleton().invalidate();
}

//----------------------------------------------------------------------------//
void System::invalidateAllWindows()
{
    WindowManager::WindowIterator wi(
        WindowManager::getSingleton().getIterator());

    for ( ; !wi.isAtEnd(); ++wi)
    {
        Window* const wnd(wi.getCurrentValue());
        // invalidate window itself
        wnd->invalidate();
        // if window has rendering window surface, invalidate it's geometry
        RenderingSurface* rs;
        if ((rs = wnd->getRenderingSurface()) && rs->isRenderingWindow())
            static_cast<RenderingWindow*>(rs)->invalidateGeometry();
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

