/************************************************************************
	filename: 	OgreResourceProvider.cpp
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Implements the Resource Provider common functionality
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
#include "renderers/OgreGUIRenderer/OgreResourceProvider.h"

#include "CEGUIExceptions.h"
#include "OgreArchiveManager.h"

#include <xercesc/framework/MemBufInputSource.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{
    void OgreResourceProvider::loadInputSourceContainer(const String& filename, InputSourceContainer& output)
    {
        Ogre::DataChunk input;
        if( !Ogre::ArchiveManager::getSingleton()._findResourceData(
            filename.c_str(), input ) )
        {
            throw InvalidRequestException((utf8*)
                "Scheme::Scheme - Filename supplied for Scheme loading must be valid");
        }

        XERCES_CPP_NAMESPACE_USE
        InputSource* mInputSource = new MemBufInputSource(input.getPtr(), input.getSize(), filename.c_str(), true);

        output.setData(mInputSource);
    }

    void OgreResourceProvider::loadRawDataContainer(const String& filename, RawDataContainer& output)
    {
        Ogre::DataChunk input;
        if( !Ogre::ArchiveManager::getSingleton()._findResourceData(
            filename.c_str(), input ) )
        {
            throw InvalidRequestException((utf8*)
                "Scheme::Scheme - Filename supplied for Scheme loading must be valid");
        }

        output.setData(input.getPtr());
        output.setSize(input.getSize());
    }
} // End of  CEGUI namespace section
