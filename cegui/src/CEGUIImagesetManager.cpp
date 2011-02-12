/***********************************************************************
    filename:   CEGUIImagesetManager.cpp
    created:    Sat Jul 18 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// singleton instance pointer
template<> ImagesetManager* Singleton<ImagesetManager>::ms_Singleton = 0;

//----------------------------------------------------------------------------//
ImagesetManager::ImagesetManager() :
    NamedXMLResourceManager<Imageset, Imageset_xmlHandler>("Imageset")
{
    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
        "CEGUI::ImagesetManager singleton created " + String(addr_buff));
}

//----------------------------------------------------------------------------//
ImagesetManager::~ImagesetManager()
{
    Logger::getSingleton().logEvent(
        "---- Begining cleanup of Imageset system ----");

    destroyAll();

    char addr_buff[32];
    sprintf(addr_buff, "(%p)", static_cast<void*>(this));
    Logger::getSingleton().logEvent(
       "CEGUI::ImagesetManager singleton destroyed " + String(addr_buff));
}

//----------------------------------------------------------------------------//
Imageset& ImagesetManager::create(const String& name, Texture& texture,
                                  XMLResourceExistsAction action)
{
    Logger::getSingleton().logEvent("Attempting to create Imageset '" + name +
                                    "' with texture only.");

    // create new object ahead of time
    Imageset* object = CEGUI_NEW_AO Imageset(name, texture);
    // return appropriate object instance (deleting any not required)
    return doExistingObjectAction(name, object, action);;
}

//----------------------------------------------------------------------------//
Imageset& ImagesetManager::createFromImageFile(const String& name,
                                               const String& filename,
                                               const String& resourceGroup,
                                               XMLResourceExistsAction action)
{
    Logger::getSingleton().logEvent("Attempting to create Imageset '" + name +
        "' using image file '" + filename + "'.");

    // create new object ahead of time
    Imageset* object = CEGUI_NEW_AO Imageset(name, filename, resourceGroup);
    // return appropriate object instance (deleting any not required)
    return doExistingObjectAction(name, object, action);;
}

//----------------------------------------------------------------------------//
void ImagesetManager::notifyDisplaySizeChanged(const Size& size)
{
    // notify all attached Imageset objects of the change in resolution
    ObjectRegistry::iterator pos = d_objects.begin(), end = d_objects.end();

    for (; pos != end; ++pos)
        pos->second->notifyDisplaySizeChanged(size);
}

//----------------------------------------------------------------------------//
ImagesetManager::ImagesetIterator ImagesetManager::getIterator(void) const
{
    return ImagesetIterator(d_objects.begin(), d_objects.end());
}

//----------------------------------------------------------------------------//
void ImagesetManager::writeImagesetToStream(const String& imageset,
                                            OutStream& out_stream) const
{
    // Create an XMLSerializer which make use of 4 space and UTF-8 encoding
    XMLSerializer xml(out_stream);
    get(imageset).writeXMLToStream(xml);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
