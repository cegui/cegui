/************************************************************************
	filename: 	CEGUIImagesetManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the ImagesetManager class
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
#include "CEGUIImagesetManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> ImagesetManager* Singleton<ImagesetManager>::ms_Singleton	= NULL;


/*************************************************************************
	Constructor
*************************************************************************/
ImagesetManager::ImagesetManager(void)
{
	Logger::getSingleton().logEvent((utf8*)"CEGUI::ImagesetManager singleton created");
}


/*************************************************************************
	Destructor
*************************************************************************/
ImagesetManager::~ImagesetManager(void)
{
	destroyAllImagesets();

	Logger::getSingleton().logEvent((utf8*)"CEGUI::ImagesetManager singleton destroyed");
}


/*************************************************************************
	Create an empty Imageset that has the given name and uses the
	given Texture
*************************************************************************/
Imageset* ImagesetManager::createImageset(const String& name, Texture* texture)
{
	if (isImagesetPresent(name))
	{
		throw	AlreadyExistsException("An Imageset object named '" + name + "' already exists.");
	}

	Imageset* temp = new Imageset(name, texture);
	d_imagesets[name] = temp;

	Logger::getSingleton().logEvent((utf8*)"Imageset '" + name +"' has been created with texture only.", Informative);

	return temp;
}


/*************************************************************************
	Create an Imageset object from the specified file
*************************************************************************/
Imageset* ImagesetManager::createImageset(const String& filename)
{
	Imageset* temp = new Imageset(filename);

	String	name = temp->getName();

	if (isImagesetPresent(name))
	{
		delete temp;

		throw	AlreadyExistsException("An Imageset object named '" + name + "' already exists.");
	}

	d_imagesets[name] = temp;

	Logger::getSingleton().logEvent((utf8*)"Imageset '" + name +"' has been created from the data specified in file '" + filename + "'.", Informative);

	return temp;
}


/*************************************************************************
	Destroys the Imageset with the specified name
*************************************************************************/
void ImagesetManager::destroyImageset(const String& name)
{
	ImagesetRegistry::iterator	pos = d_imagesets.find(name);

	if (pos != d_imagesets.end())
	{
		Logger::getSingleton().logEvent((utf8*)"Imageset '" + name +"' has been destroyed.", Informative);

		delete pos->second;
		d_imagesets.erase(pos);
	}

}


/*************************************************************************
	Destroys the given Imageset object
*************************************************************************/
void ImagesetManager::destroyImageset(Imageset* imageset)
{
	if (imageset != NULL)
	{
		destroyImageset(imageset->getName());
	}

}


/*************************************************************************
	Destroy all Imageset objects
*************************************************************************/
void ImagesetManager::destroyAllImagesets(void)
{
	while (!d_imagesets.empty())
	{
		destroyImageset(d_imagesets.begin()->first);
	}
}

/*************************************************************************
	Returns a pointer to the Imageset object with the specified name
*************************************************************************/
Imageset* ImagesetManager::getImageset(const String& name) const
{
	ImagesetRegistry::const_iterator	pos = d_imagesets.find(name);

	if (pos == d_imagesets.end())
	{
		throw UnknownObjectException("No Imageset named '" + name + "' is present in the system.");
	}

	return pos->second;
}

} // End of  CEGUI namespace section
