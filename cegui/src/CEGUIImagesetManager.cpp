/***********************************************************************
	filename: 	CEGUIImagesetManager.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implements the ImagesetManager class
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
#include "CEGUIImagesetManager.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIImageset.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> ImagesetManager* Singleton<ImagesetManager>::ms_Singleton	= 0;


/*************************************************************************
	Constructor
*************************************************************************/
ImagesetManager::ImagesetManager(void)
{
	Logger::getSingleton().logEvent("CEGUI::ImagesetManager singleton created");
}


/*************************************************************************
	Destructor
*************************************************************************/
ImagesetManager::~ImagesetManager(void)
{
	Logger::getSingleton().logEvent("---- Begining cleanup of Imageset system ----");

	destroyAllImagesets();

	Logger::getSingleton().logEvent("CEGUI::ImagesetManager singleton destroyed");
}


/*************************************************************************
	Create an empty Imageset that has the given name and uses the
	given Texture
*************************************************************************/
Imageset* ImagesetManager::createImageset(const String& name, Texture* texture)
{
	Logger::getSingleton().logEvent("Attempting to create Imageset '" + name +"' with texture only.");

	if (isImagesetPresent(name))
	{
		throw	AlreadyExistsException("ImagesetManager::createImageset - An Imageset object named '" + name + "' already exists.");
	}

	Imageset* temp = new Imageset(name, texture);
	d_imagesets[name] = temp;

	return temp;
}


/*************************************************************************
	Create an Imageset object from the specified file
*************************************************************************/
Imageset* ImagesetManager::createImageset(const String& filename, const String& resourceGroup)
{
	Logger::getSingleton().logEvent("Attempting to create an Imageset from the information specified in file '" + filename + "'.");

	Imageset* temp = new Imageset(filename, resourceGroup);

	String	name = temp->getName();

	if (isImagesetPresent(name))
	{
		delete temp;

		throw	AlreadyExistsException("ImagesetManager::createImageset - An Imageset object named '" + name + "' already exists.");
	}

	d_imagesets[name] = temp;

	return temp;
}


/*************************************************************************
    Create an Imageset object from the specified image file.
*************************************************************************/
Imageset* ImagesetManager::createImagesetFromImageFile(const String& name, const String& filename, const String& resourceGroup)
{
    Logger::getSingleton().logEvent("Attempting to create Imageset '" + name + "' using image file '" + filename + "'.");

    if (isImagesetPresent(name))
    {
        throw	AlreadyExistsException("ImagesetManager::createImageset - An Imageset object named '" + name + "' already exists.");
    }

    Imageset* temp = new Imageset(name, filename, resourceGroup);
    d_imagesets[name] = temp;

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
		String tmpName(name);

		delete pos->second;
		d_imagesets.erase(pos);

		Logger::getSingleton().logEvent("Imageset '" + tmpName +"' has been destroyed.", Informative);
	}

}


/*************************************************************************
	Destroys the given Imageset object
*************************************************************************/
void ImagesetManager::destroyImageset(Imageset* imageset)
{
	if (imageset)
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
		throw UnknownObjectException("ImagesetManager::getImageset - No Imageset named '" + name + "' is present in the system.");
	}

	return pos->second;
}


/*************************************************************************
	Notify the ImagesetManager of the current (usually new) display
	resolution.
*************************************************************************/
void ImagesetManager::notifyScreenResolution(const Size& size)
{
	// notify all attached Imageset objects of the change in resolution
	ImagesetRegistry::iterator pos = d_imagesets.begin(), end = d_imagesets.end();

	for (; pos != end; ++pos)
	{
		pos->second->notifyScreenResolution(size);
	}

}


/*************************************************************************
	Return a ImagesetManager::ImagesetIterator object to iterate over
	the available Imageset objects.
*************************************************************************/
ImagesetManager::ImagesetIterator ImagesetManager::getIterator(void) const
{
	return ImagesetIterator(d_imagesets.begin(), d_imagesets.end());
}


void ImagesetManager::writeImagesetToStream(const String& imageset, OutStream& out_stream) const
{
    const Imageset* iset = getImageset(imageset);
    // Create an XMLSerializer which make use of 4 space and UTF-8 encoding 
    XMLSerializer xml(out_stream);
    iset->writeXMLToStream(xml);
}


} // End of  CEGUI namespace section
