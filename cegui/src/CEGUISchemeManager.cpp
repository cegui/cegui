/***********************************************************************
	filename: 	CEGUISchemeManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements SchemeManager class
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
#include "CEGUISchemeManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIScheme.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> SchemeManager* Singleton<SchemeManager>::ms_Singleton	= 0;



/*************************************************************************
	constructor
*************************************************************************/
SchemeManager::SchemeManager(void)
{
	Logger::getSingleton().logEvent("CEGUI::SchemeManager singleton created.");
}


/*************************************************************************
	Destructor
*************************************************************************/
SchemeManager::~SchemeManager(void)
{
	Logger::getSingleton().logEvent("---- Begining cleanup of GUI Scheme system ----");

	unloadAllSchemes();

	Logger::getSingleton().logEvent("CEGUI::SchemeManager singleton destroyed.");
}


/*************************************************************************
	Loads a scheme
*************************************************************************/
Scheme* SchemeManager::loadScheme(const String& scheme_filename, const String& resourceGroup)
{
	Logger::getSingleton().logEvent("Attempting to load Scheme from file '" + scheme_filename + "'.");

	Scheme* tmp = new Scheme(scheme_filename, resourceGroup);
	String name = tmp->getName();
	d_schemes[name] = tmp;
	return tmp;
}


/*************************************************************************
	Un-Loads a scheme
*************************************************************************/
void SchemeManager::unloadScheme(const String& scheme_name)
{
	SchemeRegistry::iterator pos = d_schemes.find(scheme_name);

	if (pos != d_schemes.end())
	{
		String tmpName(scheme_name);

		delete pos->second;
		d_schemes.erase(pos);

		Logger::getSingleton().logEvent("Scheme '" + tmpName + "' has been unloaded.");
	}
	else
	{
		Logger::getSingleton().logEvent((utf8*)"Unable to unload non-existant scheme '" + scheme_name + "'.", Errors);
	}

}


/*************************************************************************
	Returns a pointer to the Scheme object with the specified name.
*************************************************************************/
Scheme* SchemeManager::getScheme(const String& name) const
{
	SchemeRegistry::const_iterator pos = d_schemes.find(name);

	if (pos == d_schemes.end())
	{
		throw UnknownObjectException("SchemeManager::getScheme - A Scheme object with the specified name '" + name +"' does not exist within the system");
	}

	return pos->second;
}


/*************************************************************************
	Return a SchemeManager::SchemeIterator object to iterate over the
	available schemes.
*************************************************************************/
SchemeManager::SchemeIterator SchemeManager::getIterator(void) const
{
	return SchemeIterator(d_schemes.begin(), d_schemes.end());
}


/*************************************************************************
	Unload all schemes currently defined within the system.
*************************************************************************/
void SchemeManager::unloadAllSchemes(void)
{
	// unload all schemes
	while (!d_schemes.empty())
	{
		unloadScheme(d_schemes.begin()->first);
	}

}


} // End of  CEGUI namespace section
