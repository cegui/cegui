/************************************************************************
	filename: 	CEGUISystem.cpp
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Implementation of main system object
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/Janitor.hpp"
#include "xercesc/sax2/DefaultHandler.hpp"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> System* Singleton<System>::ms_Singleton	= NULL;

// click event generation defaults
const double	System::DefaultSingleClickTimeout	= 0.2;
const double	System::DefaultMultiClickTimeout	= 0.33;
const Size		System::DefaultMultiClickAreaSize(12,12);


/*************************************************************************
	Constructor
*************************************************************************/
System::System(Renderer* renderer, utf8* logFile) :
	d_renderer(renderer),
	d_gui_redraw(false),
	d_wndWithMouse(NULL),
	d_activeSheet(NULL),
	d_sysKeys(0),
	d_lshift(false),
	d_rshift(false),
	d_lctrl(false),
	d_rctrl(false),
	d_click_timeout(DefaultSingleClickTimeout),
	d_dblclick_timeout(DefaultMultiClickTimeout),
	d_dblclick_size(DefaultMultiClickAreaSize),
	d_defaultMouseCursor(NULL)
{
	// first thing to do is create logger
	new Logger(logFile);

	Logger::getSingleton().logEvent((utf8*)"---- Begining CEGUI System initialisation ----");

	// cause creation of other singleton objects
	new ImagesetManager();
	new FontManager();
	new WindowFactoryManager();
	new WindowManager();
	new SchemeManager();
	new MouseCursor();

	// initialise Xerces-C XML sysetm
	XERCES_CPP_NAMESPACE_USE
	try
	{
		XMLPlatformUtils::Initialize();
	}
	catch(XMLException& exc)
	{
		// cleanup most stuff we done so far:
		delete	MouseCursor::getSingletonPtr();
		delete	SchemeManager::getSingletonPtr();
		delete	WindowManager::getSingletonPtr();
		delete	WindowFactoryManager::getSingletonPtr();
		delete	FontManager::getSingletonPtr();
		delete	ImagesetManager::getSingletonPtr();

		// log a message about the failure
		ArrayJanitor<char> excmsg(XMLString::transcode(exc.getMessage()));
		String message((utf8*)"An exception occurred while initialising the Xerces-C XML system.  Additional information: ");
		message += (utf8*)excmsg.get();
		Logger::getSingleton().logEvent(message, Errors);
		
		// now delete the logger singleton
		delete Logger::getSingletonPtr();

		// throw a std::exception (because it won't try and use logger)
		throw message.c_str();
	}

	// add default GUISheet factory - the only UI element we can create "out of the box".
	WindowFactoryManager::getSingleton().addFactory(new GUISheetFactory);

	// success - we are created!  Log it for prosperity :)
	Logger::getSingleton().logEvent((utf8*)"CEGUI::System singleton created.");
	Logger::getSingleton().logEvent((utf8*)"---- CEGUI System initialisation completed ----");
}

/*************************************************************************
	Destructor
*************************************************************************/
System::~System(void)
{
	Logger::getSingleton().logEvent((utf8*)"---- Begining CEGUI System destruction ----");

	// cleanup XML stuff
	XERCES_CPP_NAMESPACE_USE
	XMLPlatformUtils::Terminate();

	//
	// perform cleanup in correct sequence
	//
	// destroy windows so it's safe to destroy factories
	WindowManager::getSingleton().destroyAllWindows();

	// get pointer to the GUI sheet factory we added
	GUISheetFactory* factory = (GUISheetFactory*)WindowFactoryManager::getSingleton().getFactory((utf8*)"DefaultGUISheet");

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

	Logger::getSingleton().logEvent((utf8*)"CEGUI::System singleton destroyed.");
	Logger::getSingleton().logEvent((utf8*)"---- CEGUI System destruction completed ----");
	delete Logger::getSingletonPtr();
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
	
	// TODO: Add a 'system default font' changed event and fire it here.
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
}


/*************************************************************************
	Set the image to be used as the default mouse cursor.
*************************************************************************/
void System::setDefaultMouseCursor(const String& imageset, const String& image_name)
{
	d_defaultMouseCursor = &ImagesetManager::getSingleton().getImageset(imageset)->getImage(image_name);
}


/*************************************************************************
	Method that injects a mouse movement event into the system
*************************************************************************/
void System::injectMouseMove(float delta_x, float delta_y)
{
	MouseEventArgs ma;
	MouseCursor& mouse = MouseCursor::getSingleton();

	ma.moveDelta.d_x = (float)delta_x;
	ma.moveDelta.d_y = (float)delta_y;

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
				d_wndWithMouse->onMouseLeaves(ma);
			}

			d_wndWithMouse = dest_window;
			dest_window->onMouseEnters(ma);
		}

		while ((!ma.handled) && (dest_window != NULL))
		{
			dest_window->onMouseMove(ma);
			dest_window = dest_window->getParent();
		}

	}

}


/*************************************************************************
	Method that injects a mouse button down event into the system.
*************************************************************************/
void System::injectMouseButtonDown(MouseButton button)
{
	// update system keys
	d_sysKeys |= mouseButtonToSyskey(button);

	MouseEventArgs ma;
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	ma.sysKeys = d_sysKeys;

	Window* dest_window = getTargetWindow(ma.position);

	Window*	event_wnd = dest_window;

	while ((!ma.handled) && (event_wnd != NULL))
	{
		event_wnd->onMouseButtonDown(ma);
		event_wnd = event_wnd->getParent();
	}

	//
	// Handling for multi-click generation
	//
	MouseClickTracker& tkr = d_click_trackers[button];

	tkr.d_click_count++;

	// see if we meet multi-click timing

	if ((tkr.d_timer.elapsed() > d_dblclick_timeout) ||
		(!tkr.d_click_area.isPointInRect(ma.position)) ||
		(tkr.d_click_count > 3))
	{
		// single down event.
		tkr.d_click_count = 1;

		// build allowable area for multi-clicks
		d_click_trackers[button].d_click_area.setPosition(ma.position);
		d_click_trackers[button].d_click_area.setSize(d_dblclick_size);
		d_click_trackers[button].d_click_area.offset(Point(-(d_dblclick_size.d_width / 2), -(d_dblclick_size.d_height / 2)));
	}

	// reuse same event args from earlier
	ma.handled = false;

	// process multi-clicks we have generated so far
	switch (tkr.d_click_count)
	{
	case 2:
		event_wnd = dest_window;

		while ((!ma.handled) && (event_wnd != NULL))
		{
			event_wnd->onMouseDoubleClicked(ma);
			event_wnd = event_wnd->getParent();
		}
		break;

	case 3:
		event_wnd = dest_window;

		while ((!ma.handled) && (event_wnd != NULL))
		{
			event_wnd->onMouseTripleClicked(ma);
			event_wnd = event_wnd->getParent();
		}
		break;
	}

	// reset timer for this button.
	d_click_trackers[button].d_timer.restart();

}


/*************************************************************************
	Method that injects a mouse button up event into the system.
*************************************************************************/
void System::injectMouseButtonUp(MouseButton button)
{
	// update system keys
	d_sysKeys &= ~mouseButtonToSyskey(button);

	MouseEventArgs ma;
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	ma.sysKeys = d_sysKeys;

	Window* dest_window = getTargetWindow(ma.position);

	while ((!ma.handled) && (dest_window != NULL))
	{
		dest_window->onMouseButtonUp(ma);
		dest_window = dest_window->getParent();
	}

	// check timer for 'button' to see if this up event also constitutes a single 'click'
	if (d_click_trackers[button].d_timer.elapsed() <= d_click_timeout)
	{
		ma.handled = false;
		dest_window = getTargetWindow(ma.position);

		while ((!ma.handled) && (dest_window != NULL))
		{
			dest_window->onMouseClicked(ma);
			dest_window = dest_window->getParent();
		}

	}

}


/*************************************************************************
	Method that injects a key down event into the system.
*************************************************************************/
void System::injectKeyDown(uint key_code)
{
	// update system keys
	d_sysKeys |= keyCodeToSyskey((Key::Scan)key_code, true);

	if (d_activeSheet != NULL)
	{
		KeyEventArgs args;
		args.scancode = (Key::Scan)key_code;
		args.sysKeys = d_sysKeys;

		Window* dest = d_activeSheet->getActiveChild();

		while ((dest != NULL) && (!args.handled))
		{
			dest->onKeyDown(args);
			dest = dest->getParent();
		}

	}

}


/*************************************************************************
	Method that injects a key up event into the system.
*************************************************************************/
void System::injectKeyUp(uint key_code)
{
	// update system keys
	d_sysKeys &= ~keyCodeToSyskey((Key::Scan)key_code, false);

	if (d_activeSheet != NULL)
	{
		KeyEventArgs args;
		args.scancode = (Key::Scan)key_code;
		args.sysKeys = d_sysKeys;

		Window* dest = d_activeSheet->getActiveChild();

		while ((dest != NULL) && (!args.handled))
		{
			dest->onKeyUp(args);
			dest = dest->getParent();
		}

	}

}


/*************************************************************************
	Method that injects a typed character event into the system.	
*************************************************************************/
void System::injectChar(utf32 code_point)
{
	if (d_activeSheet != NULL)
	{
		KeyEventArgs args;
		args.codepoint = code_point;
		args.sysKeys = d_sysKeys;

		Window* dest = d_activeSheet->getActiveChild();

		while ((dest != NULL) && (!args.handled))
		{
			dest->onCharacter(args);
			dest = dest->getParent();
		}
		
	}

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

} // End of  CEGUI namespace section
