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



/*************************************************************************
	Constructor
*************************************************************************/
System::System(Renderer* renderer) :
	d_renderer(renderer),
	d_activeSheet(NULL),
	d_wndWithMouse(NULL),
	d_gui_redraw(false)
{
	// first thing to do is create logger
	new Logger((utf8*)"CEGUI.log");

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
		throw std::exception(message.c_str());
	}

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

	// destroy factories so it's safe to unload GUI modules
	WindowFactoryManager::getSingleton().removeAllFactories();

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
		d_renderer->setQueueingEnabled(true);
		d_renderer->clearRenderList();

		if (d_activeSheet != NULL)
		{
			d_activeSheet->render();
		}

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
	Method that injects a mouse movement event into the system
*************************************************************************/
void System::injectMouseMove(int delta_x, int delta_y)
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
				d_wndWithMouse->onMouseLeaves(MouseEventArgs(ma));
			}

			d_wndWithMouse = dest_window;
			dest_window->onMouseEnters(MouseEventArgs(ma));
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
	MouseEventArgs ma;
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	// TODO: Add system keys to MouseEventArgs

	Window* dest_window = getTargetWindow(ma.position);

	while ((!ma.handled) && (dest_window != NULL))
	{
		dest_window->onMouseButtonDown(ma);
		dest_window = dest_window->getParent();
	}

}


/*************************************************************************
	Method that injects a mouse button up event into the system.
*************************************************************************/
void System::injectMouseButtonUp(MouseButton button)
{
	MouseEventArgs ma;
	ma.position = MouseCursor::getSingleton().getPosition();
	ma.moveDelta = Vector2(0.0f, 0.0f);
	ma.button = button;
	// TODO: Add system keys to MouseEventArgs

	Window* dest_window = getTargetWindow(ma.position);

	while ((!ma.handled) && (dest_window != NULL))
	{
		dest_window->onMouseButtonUp(ma);
		dest_window = dest_window->getParent();
	}

}


/*************************************************************************
	Method that injects a key down event into the system.
*************************************************************************/
void System::injectKeyDown(uint key_code)
{
}


/*************************************************************************
	Method that injects a key up event into the system.
*************************************************************************/
void System::injectKeyUp(uint key_code)
{
}


/*************************************************************************
	Method that injects a typed character event into the system.	
*************************************************************************/
void System::injectChar(utf32 code_point)
{
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

	}

	return dest_window;
}


} // End of  CEGUI namespace section
