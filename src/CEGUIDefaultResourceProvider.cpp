/************************************************************************
	filename: 	CEGUIDefaultResourceProvider.cpp
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Implements the Resource Manager common functionality
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include "CEGUIDefaultResourceProvider.h"
#include "CEGUIExceptions.h"

#include <fstream>

// Start of CEGUI namespace section
namespace CEGUI
{

    void DefaultResourceProvider::loadRawDataContainer(const String& filename, RawDataContainer& output, const String& resourceGroup)
    {
        if (filename.empty())
        {
            throw InvalidRequestException(
                "DefaultResourceProvider::load - Filename supplied for data loading must be valid");
        }

        String final_filename(getFinalFilename(filename, resourceGroup));

        std::ifstream dataFile(final_filename.c_str(), std::ios::binary|std::ios::ate);
        if( dataFile.fail())
        {
            throw InvalidRequestException(
                "DefaultResourceProvider::load - " + filename + " does not exist");
        }
        std::streampos size = dataFile.tellg();
        dataFile.seekg (0, std::ios::beg);

        unsigned char* buffer = new unsigned char [size];

        try {
            dataFile.read(reinterpret_cast<char*>(buffer), size);
        }
        catch(std::ifstream::failure e) {
            delete [] buffer;
            throw GenericException(
                "DefaultResourceProvider::loadRawDataContainer - Problem reading " + filename);
        }

        dataFile.close();

        output.setData(buffer);
        output.setSize(size);
    }

    void DefaultResourceProvider::setResourceGroupDirectory(const String& resourceGroup, const String& directory)
    {
        d_resourceGroups[resourceGroup] = directory;
    }

    const String& DefaultResourceProvider::getResourceGroupDirectory(const String& resourceGroup)
    {
        return d_resourceGroups[resourceGroup];
    }

    void DefaultResourceProvider::clearResourceGroupDirectory(const String& resourceGroup)
    {
        ResourceGroupMap::iterator iter = d_resourceGroups.find(resourceGroup);

        if (iter != d_resourceGroups.end())
            d_resourceGroups.erase(iter);
    }

    String DefaultResourceProvider::getFinalFilename(const String& filename, const String& resourceGroup) const
    {
        String final_filename;

        // look up resource group directory
        ResourceGroupMap::const_iterator iter = d_resourceGroups.find(resourceGroup);

        // if there was an entry for this group, use it's directory as the
        // first part of the filename
        if (iter != d_resourceGroups.end())
            final_filename = (*iter).second;

        // append the filename part that we were passed
        final_filename += filename;

        // return result
        return final_filename;
    }

} // End of  CEGUI namespace section
