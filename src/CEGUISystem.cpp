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
#include "xercesc/util/PlatformUtils.hpp"
#include "xercesc/util/Janitor.hpp"

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
	d_renderer(renderer)
{
	// first thing to do is create logger
	new Logger((utf8*)"CEGUI.log");


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
}

/*************************************************************************
	Destructor
*************************************************************************/
System::~System(void)
{
	// cleanup XML stuff
	XERCES_CPP_NAMESPACE_USE
	XMLPlatformUtils::Terminate();

	// cleanup singletons
	delete	MouseCursor::getSingletonPtr();
	delete	SchemeManager::getSingletonPtr();
	delete	WindowManager::getSingletonPtr();
	delete	WindowFactoryManager::getSingletonPtr();
	delete	FontManager::getSingletonPtr();
	delete	ImagesetManager::getSingletonPtr();

	Logger::getSingleton().logEvent((utf8*)"CEGUI::System singleton destroyed.");
	delete Logger::getSingletonPtr();
}

} // End of  CEGUI namespace section
