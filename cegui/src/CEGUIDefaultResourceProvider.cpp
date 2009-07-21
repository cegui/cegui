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

#if defined(__WIN32__) || defined(_WIN32)
#   include <io.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <dirent.h>
#   include <fnmatch.h>
#endif

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
        if (directory.length() == 0)
            return;

        #if defined(_WIN32) || defined(__WIN32__)
            // while we rarely use the unportable '\', the user may have
            const String separators("\\/");
        #else
            const String separators("/");
        #endif

        if (String::npos == separators.find(directory[directory.length() - 1]))
            d_resourceGroups[resourceGroup] = directory + '/';
        else
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

size_t DefaultResourceProvider::getResourceGroupFileNames(
    std::vector<String>& out_vec,
    const String& file_pattern,
    const String& resource_group)
{
    // look-up resource group name
    ResourceGroupMap::const_iterator iter =
        d_resourceGroups.find(resource_group.empty() ? d_defaultResourceGroup :
                                                       resource_group);
    // get directory that's set for the resource group
    const String dir_name(iter != d_resourceGroups.end() ? (*iter).second : "./");

    size_t entries = 0;

// Win32 code.
#if defined(__WIN32__) || defined(_WIN32)
    intptr_t f;
    struct _finddata_t fd;
    if ((f = _findfirst((dir_name + file_pattern).c_str(), &fd)) != -1)
    {
        do
        {
            if ((fd.attrib & _A_SUBDIR))
                continue;

            out_vec.push_back(fd.name);
            ++entries;
        } while (_findnext(f, &fd) == 0);

        _findclose(f);
    }
// Everybody else
#else
    DIR* dirp;
    if ((dirp = opendir(dir_name.c_str())))
    {
        struct dirent* dp;
        while ((dp = readdir(dirp)))
        {
            const String filename(dir_name + dp->d_name);
            struct stat s;

            if ((stat(filename.c_str(), &s) == 0) &&
                S_ISREG(s.st_mode) &&
                (fnmatch(file_pattern.c_str(), dp->d_name, 0) == 0))
            {
                out_vec.push_back(dp->d_name);
                ++entries;
            }
        }

        closedir(dirp);
    }
#endif
    return entries;
}

} // End of  CEGUI namespace section
