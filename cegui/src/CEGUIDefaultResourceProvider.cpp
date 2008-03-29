/***********************************************************************
	filename: 	CEGUIDefaultResourceProvider.cpp
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Implements the Resource Manager common functionality
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
#include "CEGUIDefaultResourceProvider.h"
#include "CEGUIExceptions.h"

#include <fstream>
#include <iostream>

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
    
    void DefaultResourceProvider::unloadRawDataContainer(RawDataContainer& data)
    {
        uint8* ptr = data.getDataPtr();
        delete [] ptr;
        data.setData(0);
        data.setSize(0);
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
        ResourceGroupMap::const_iterator iter =
            d_resourceGroups.find(resourceGroup.empty() ? d_defaultResourceGroup : resourceGroup);

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
