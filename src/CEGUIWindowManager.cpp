/************************************************************************
	filename: 	CEGUIWindowManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the WindowManager class
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
#include "CEGUIWindowManager.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIWindow.h"
#include "CEGUIExceptions.h"
#include "CEGUIGUILayout_xmlHandler.h"

#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>



// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> WindowManager* Singleton<WindowManager>::ms_Singleton	= NULL;


/*************************************************************************
	Definition of constant data for WindowManager
*************************************************************************/
// Declared in WindowManager
const char	WindowManager::GUILayoutSchemaName[]	= "GUILayout.xsd";


/*************************************************************************
	Destructor
*************************************************************************/
WindowManager::~WindowManager(void)
{
	destroyAllWindows();

	Logger::getSingleton().logEvent((utf8*)"CEGUI::WindowManager singleton destroyed");
}


/*************************************************************************
	Create a new window of the specified type
*************************************************************************/
Window* WindowManager::createWindow(const String& type, const String& name)
{
	if (isWindowPresent(name))
	{
		throw AlreadyExistsException("WindowManager::createWindow - A Window object with the name '" + name +"' already exists within the system.");
	}

	WindowFactory* factory = WindowFactoryManager::getSingleton().getFactory(type);

	Window* newWindow = factory->createWindow(name);
	d_windowRegistry[name] = newWindow;

	Logger::getSingleton().logEvent((utf8*)"Window '" + name +"' of type '" + type + "' has been created.", Informative);

	return newWindow;
}


/*************************************************************************
	Destroy the given window by pointer
*************************************************************************/
void WindowManager::destroyWindow(Window* window)
{
	if (window != NULL)
	{
		// this is done because the name is used for the log after the window is destroyed,
		// if we just did getName() we would get a const ref to the Window's internal name
		// string which is destroyed along with the window so wouldn't exist when the log tried
		// to use it (as I soon discovered).
		String name = window->getName();

		destroyWindow(name);
	}

}


/*************************************************************************
	Destroy the given window by name
*************************************************************************/
void WindowManager::destroyWindow(const String& window)
{
	WindowRegistry::iterator wndpos = d_windowRegistry.find(window);

	if (wndpos != d_windowRegistry.end())
	{
		WindowFactory* factory = WindowFactoryManager::getSingleton().getFactory(wndpos->second->getType());
		factory->destroyWindow(wndpos->second);
		d_windowRegistry.erase(wndpos);

		Logger::getSingleton().logEvent((utf8*)"Window '" + window + "' has been destroyed.", Informative);
	}

}


/*************************************************************************
	Return a pointer to the named window
*************************************************************************/
Window* WindowManager::getWindow(const String& name) const
{
	WindowRegistry::const_iterator pos = d_windowRegistry.find(name);

	if (pos == d_windowRegistry.end())
	{
		throw UnknownObjectException("WindowManager::getWindow - A Window object with the name '" + name +"' does not exist within the system");
	}

	return pos->second;
}


/*************************************************************************
	Return true if a window with the given name is present
*************************************************************************/
bool WindowManager::isWindowPresent(const String& name) const
{
	return (d_windowRegistry.find(name) != d_windowRegistry.end());
}


/*************************************************************************
	Destroy all Window objects
*************************************************************************/
void WindowManager::destroyAllWindows(void)
{
	String window_name;
	while (!d_windowRegistry.empty())
	{
		window_name = d_windowRegistry.begin()->first;
		destroyWindow(window_name);
	}

}


/*************************************************************************
	Creates a set of windows (a Gui layout) from the information in the
	specified XML file.	
*************************************************************************/
Window* WindowManager::loadWindowLayout(const String& filename)
{
	XERCES_CPP_NAMESPACE_USE

	if (filename.empty() || (filename == (utf8*)""))
	{
		throw InvalidRequestException((utf8*)"WindowManager::loadWindowLayout - Filename supplied for gui-layout loading must be valid.");
	}

	// log the fact we are about to load a layout
	Logger::getSingleton().logEvent((utf8*)"---- Beginning loading of GUI layout from '" + filename + "' ----", Informative);

	SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();

	// set basic settings we want from parser
	parser->setFeature(XMLUni::fgSAX2CoreValidation, true);
	parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
	parser->setFeature(XMLUni::fgXercesSchema, true);
	parser->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

	// setup schema for gui-layout data
	XMLCh* pval = XMLString::transcode(GUILayoutSchemaName);
	parser->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation, pval);
	XMLString::release(&pval);

	// setup handler object
	GUILayout_xmlHandler handler;
	parser->setContentHandler(&handler);
	parser->setErrorHandler(&handler);

	// do parse (which uses handler to create actual data)
	try
	{
		parser->parse(filename.c_str());

		// log the completion of loading
		Logger::getSingleton().logEvent((utf8*)"---- Successfully completed loading of GUI layout from '" + filename + "' ----", Standard);
	}
	catch(const XMLException& exc)
	{
		if (exc.getCode() != XMLExcepts::NoError)
		{
			delete parser;

			char* excmsg = XMLString::transcode(exc.getMessage());
			String message((utf8*)"WindowManager::loadWindowLayout - An error occurred while parsing gui-layout file '" + filename + "'.  Additional information: ");
			message += (utf8*)excmsg;
			XMLString::release(&excmsg);

			throw FileIOException(message);
		}

	}
	catch(const SAXParseException& exc)
	{
		delete parser;

		char* excmsg = XMLString::transcode(exc.getMessage());
		String message((utf8*)"WindowManager::loadWindowLayout - An error occurred while parsing gui-layout file '" + filename + "'.  Additional information: ");
		message += (utf8*)excmsg;
		XMLString::release(&excmsg);

		throw FileIOException(message);
	}
	catch(...)
	{
		delete parser;

		throw FileIOException((utf8*)"WindowManager::loadWindowLayout - An unexpected error occurred while parsing gui-layout file '" + filename + "'.");
	}

	// cleanup
	delete parser;

	return handler.getLayoutRootWindow();
}


WindowManager& WindowManager::getSingleton(void)
{
	return Singleton<WindowManager>::getSingleton();
}


WindowManager* WindowManager::getSingletonPtr(void)
{
	return Singleton<WindowManager>::getSingletonPtr();
}


/*************************************************************************
	Return a WindowManager::WindowIterator object to iterate over the
	currently defined Windows.
*************************************************************************/
WindowManager::WindowIterator WindowManager::getIterator(void) const
{
	return WindowIterator(d_windowRegistry.begin(), d_windowRegistry.end());
}
} // End of  CEGUI namespace section
