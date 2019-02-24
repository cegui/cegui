/***********************************************************************
    created:    8/7/2004
    author:     James '_mental_' O'Sullivan
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Exceptions.h"

#include <stdio.h>

#if defined(__WIN32__) || defined(_WIN32)
#   include "CEGUI/System.h"
#   include <io.h>
#   include <windows.h>
#   include <string>
#elif defined(__ANDROID__)
#   include "CEGUI/AndroidUtils.h" 
#   include <android/asset_manager.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <dirent.h>
#   include <fnmatch.h>
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
void DefaultResourceProvider::loadRawDataContainer(const String& filename,
                                                   RawDataContainer& output,
                                                   const String& resourceGroup)
{
    if (filename.empty())
        throw InvalidRequestException(
            "Filename supplied for data loading must be valid");

    const String final_filename(getFinalFilename(filename, resourceGroup));

#ifdef __ANDROID__
    if (AndroidUtils::getAndroidApp() == 0)
        throw FileIOException("AndroidUtils::android_app has not been set for CEGUI");
    struct android_app* app = AndroidUtils::getAndroidApp();
    if (!app->activity->assetManager)
        throw FileIOException("Android AAssetManager is not valid ");
#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32) 
    AAsset *file = AAssetManager_open(app->activity->assetManager, final_filename.c_str(), AASSET_MODE_UNKNOWN);
#else
    AAsset *file = AAssetManager_open(app->activity->assetManager, String::convertUtf32ToUtf8(final_filename.getString()).c_str(), AASSET_MODE_UNKNOWN);
#endif
   
    if (file == 0)
        throw FileIOException(final_filename + " does not exist");

    size_t size = AAsset_getLength(file);

    unsigned char* const buffer = new unsigned char[size];

    const size_t size_read = AAsset_read(file, buffer, size);
    AAsset_close(file);
#else
#   if defined(__WIN32__) || defined(_WIN32)
    FILE* file = _wfopen(System::getStringTranscoder().stringToStdWString(final_filename).c_str(), L"rb");
#   else
#       if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
        FILE* file = fopen(String::convertUtf32ToUtf8(final_filename.getString()).c_str(), "rb");
#       else
        FILE* file = fopen(final_filename.c_str(), "rb");
#       endif
#   endif
    
    if (file == nullptr)
        throw FileIOException(final_filename + " does not exist");
    
    fseek(file, 0, SEEK_END);
    const size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* const buffer = new unsigned char[size];

    const size_t size_read = fread(buffer, sizeof(char), size, file);
    fclose(file);
#endif

    if (size_read != size)
    {
        delete[] buffer;

        throw FileIOException(
            "A problem occurred while reading file: " + final_filename);
    }

    output.setData(buffer);
    output.setSize(size);
}

//----------------------------------------------------------------------------//
void DefaultResourceProvider::unloadRawDataContainer(RawDataContainer& data)
{
    data.release();
}

//----------------------------------------------------------------------------//
void DefaultResourceProvider::setResourceGroupDirectory(
                                                const String& resourceGroup,
                                                const String& directory)
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

//----------------------------------------------------------------------------//
const String& DefaultResourceProvider::getResourceGroupDirectory(
                                                const String& resourceGroup)
{
    return d_resourceGroups[resourceGroup];
}

//----------------------------------------------------------------------------//
void DefaultResourceProvider::clearResourceGroupDirectory(
                                                const String& resourceGroup)
{
    ResourceGroupMap::iterator iter = d_resourceGroups.find(resourceGroup);

    if (iter != d_resourceGroups.end())
        d_resourceGroups.erase(iter);
}

//----------------------------------------------------------------------------//
String DefaultResourceProvider::getFinalFilename(
                                            const String& filename,
                                            const String& resourceGroup) const
{
    String final_filename;

    // look up resource group directory
    ResourceGroupMap::const_iterator iter =
        d_resourceGroups.find(resourceGroup.empty() ?
            d_defaultResourceGroup :
            resourceGroup);

    // if there was an entry for this group, use it's directory as the
    // first part of the filename
    if (iter != d_resourceGroups.end())
        final_filename = (*iter).second;

    // append the filename part that we were passed
    final_filename += filename;

    // return result
    return final_filename;
}

//----------------------------------------------------------------------------//
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
    const String dir_name(
        iter != d_resourceGroups.end() ? (*iter).second : "./");

    size_t entries = 0;

// Win32 code.
#if defined(__WIN32__) || defined(_WIN32)
    intptr_t f;
    struct _wfinddata_t fd;

    if ((f = _wfindfirst(System::getStringTranscoder().stringToStdWString(
            dir_name + file_pattern).c_str(), &fd)) != -1)
    {
        do
        {
            if ((fd.attrib & _A_SUBDIR))
                continue;

            CEGUI::String currentName = System::getStringTranscoder().stringFromStdWString(fd.name);
            out_vec.push_back(currentName);
            ++entries;
        }
        while (_wfindnext(f, &fd) == 0);

        _findclose(f);
    }
#elif defined(__ANDROID__)
    if (AndroidUtils::getAndroidApp() == 0)
        throw FileIOException("AndroidUtils::android_app has not been set for CEGUI");
    struct android_app* app = AndroidUtils::getAndroidApp();
    AAssetDir* dirp;
#if (CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32) 
    if ((dirp == AAssetManager_openDir(app->activity->assetManager, dir_name.c_str()))) 
#else
    if ((dirp == AAssetManager_openDir(app->activity->assetManager, String::convertUtf32ToUtf8(dir_name.getString()).c_str()))) 
#endif
    {
        const char* filename;
        while ((filename =  AAssetDir_getNextFileName(dirp)))
        {
            out_vec.push_back(filename);
            ++entries;
        }
        AAssetDir_close(dirp);
    }
// Everybody else
#else
    DIR* dirp;

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    dirp = opendir(String::convertUtf32ToUtf8(dir_name.getString()).c_str());
#else
    dirp = opendir(dir_name.c_str());
#endif

    if (dirp)
    {
        struct dirent* dp;

        while ((dp = readdir(dirp)))
        {
            const String filename(dir_name + dp->d_name);
            struct stat s;

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
            if ((stat(String::convertUtf32ToUtf8(filename.getString()).c_str(), &s) == 0) &&
                    S_ISREG(s.st_mode) &&
                    (fnmatch(String::convertUtf32ToUtf8(file_pattern.getString()).c_str(), dp->d_name, 0) == 0))
#else
            if ((stat(filename.c_str(), &s) == 0) &&
                    S_ISREG(s.st_mode) &&
                    (fnmatch(file_pattern.c_str(), dp->d_name, 0) == 0))
#endif
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

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

