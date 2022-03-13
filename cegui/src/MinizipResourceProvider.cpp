/***********************************************************************
    created:    19/3/2010
    author:     Jeff A. Marr

    purpose:    Implements the MinizipResourceProvider
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/MinizipResourceProvider.h"
#include "CEGUI/DataContainer.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

// exclude code on apple unless feature macro is defined.
#if !defined(__APPLE__) || defined(CEGUI_BUILD_RESOURCE_PROVIDER_MINIZIP)

#include "minizip/unzip.h"

#include <fstream>

#if defined (__WIN32__) || defined(_WIN32)
#   include <shlwapi.h>
#   define FNMATCH(p, s)    PathMatchSpec(s, p)
#   ifdef _MSC_VER
#       pragma comment(lib, "shlwapi.lib")
#   endif
#else
#   include <fnmatch.h>
#   define FNMATCH(p, s)    fnmatch(p, s, FNM_PATHNAME)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// Impl struct: mainly used in order to keep unzip.h out of the public headers.
struct MinizipResourceProvider::Impl
{
    Impl(const bool loadLocal) :
        d_zfile(0),
        d_loadLocal(loadLocal)
    {
    }

    unzFile d_zfile;
    String  d_archive;
    String  d_password;
    bool    d_loadLocal;
};

//----------------------------------------------------------------------------//
// Helper function that matches names against the pattern.
bool nameMatchesPattern(const String& name, const String& pattern)
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    return !FNMATCH(pattern.c_str(), name.c_str());
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    return !FNMATCH(String::convertUtf32ToUtf8(pattern.getString()).c_str(), String::convertUtf32ToUtf8(name.getString()).c_str());
#endif
}

//----------------------------------------------------------------------------//
MinizipResourceProvider::MinizipResourceProvider() :
    d_pimpl(new Impl(true))
{
}

//----------------------------------------------------------------------------//
MinizipResourceProvider::MinizipResourceProvider(const String& archive,
                                                 bool loadLocal) :
    d_pimpl(new Impl(loadLocal))
{
    setArchive(archive);
}

//----------------------------------------------------------------------------//
MinizipResourceProvider::~MinizipResourceProvider()
{
    if (d_pimpl->d_zfile)
        closeArchive();

    delete d_pimpl;
}

//----------------------------------------------------------------------------//
void MinizipResourceProvider::setArchive(const String& archive)
{
    if (d_pimpl->d_zfile)
        closeArchive();

    d_pimpl->d_archive = archive;
    openArchive();
}

//----------------------------------------------------------------------------//
void MinizipResourceProvider::setPassword(const String& password)
{
    d_pimpl->d_password = password;
}

//----------------------------------------------------------------------------//
bool MinizipResourceProvider::doesFileExist(const String& filename)
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    std::ifstream dataFile(filename.c_str(), std::ios::binary | std::ios::ate);
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    std::ifstream dataFile(String::convertUtf32ToUtf8(filename.getString()).c_str(),
                           std::ios::binary | std::ios::ate);
#endif

    if (dataFile)
        return true;

    return false;
}

//----------------------------------------------------------------------------//
void MinizipResourceProvider::openArchive()
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    d_pimpl->d_zfile = unzOpen(d_pimpl->d_archive.c_str());
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    d_pimpl->d_zfile = unzOpen(String::convertUtf32ToUtf8(d_pimpl->d_archive.getString()).c_str());
#endif

    if (d_pimpl->d_zfile == 0)
    {
        throw InvalidRequestException(
            "'" + d_pimpl->d_archive + "' does not exist");
    }
}

//----------------------------------------------------------------------------//
void MinizipResourceProvider::closeArchive()
{
    if (unzClose(d_pimpl->d_zfile) != Z_OK)
    {
        // do not throw an exception as this method is called from the destructor!
        if (CEGUI::Logger::getSingletonPtr())
        {
            CEGUI::Logger::getSingleton().logEvent(
                "MinizipResourceProvider::closeArchive: '" +
                d_pimpl->d_archive + "' error upon closing", LoggingLevel::Error);
        }
    }

    d_pimpl->d_zfile = 0;
}

//----------------------------------------------------------------------------//
void MinizipResourceProvider::loadRawDataContainer(const String& filename,
                                                   RawDataContainer& output,
                                                   const String& resourceGroup)
{
    const String final_filename = getFinalFilename(filename, resourceGroup);

    if (d_pimpl->d_loadLocal && doesFileExist(final_filename))
    {
        DefaultResourceProvider::loadRawDataContainer(filename,
                                                      output,
                                                      resourceGroup);
        return;
    }

    if (d_pimpl->d_zfile == 0)
    {
        throw InvalidRequestException(
            "'" + final_filename + "' cannot be "
            "loaded because the archive has not been set");
    }

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII)
    if (unzLocateFile(d_pimpl->d_zfile, final_filename.c_str(), 0) != UNZ_OK)
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    if (unzLocateFile(d_pimpl->d_zfile, String::convertUtf32ToUtf8(final_filename.getString()).c_str(), 0) != UNZ_OK)
#endif
    {
        throw InvalidRequestException("'" + final_filename +
            "' does not exist");
    }

    unz_file_info file_info;

    if (unzGetCurrentFileInfo(d_pimpl->d_zfile, &file_info,
                              0, 0, 0, 0, 0, 0) != UNZ_OK)
    {
        throw FileIOException("'" + final_filename +
            "' error reading file header");
    }

    if (unzOpenCurrentFilePassword(d_pimpl->d_zfile, (const char*) d_pimpl->d_password.c_str()) != UNZ_OK)
    {
        throw FileIOException("'" + final_filename +
            "' error opening file");
    }

    const auto size = file_info.uncompressed_size;
    std::uint8_t* buffer = new std::uint8_t[size];

    if (unzReadCurrentFile(d_pimpl->d_zfile, buffer, size) < 0)
    {
        throw FileIOException("'" + final_filename +
            "' error reading file");
    }

    if (unzCloseCurrentFile(d_pimpl->d_zfile) != UNZ_OK)
    {
        throw GenericException("'" + final_filename +
            "' error validating file");
    }

    output.setData(buffer);
    output.setSize(size);
}

//----------------------------------------------------------------------------//
size_t MinizipResourceProvider::getResourceGroupFileNames(
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
        iter != d_resourceGroups.end() ? (*iter).second : "");

    size_t entries = 0;

    // get local (non zip) matches if local mode is set.
    if (d_pimpl->d_loadLocal)
        entries += DefaultResourceProvider::getResourceGroupFileNames(
                                        out_vec, file_pattern, resource_group);

    // exit now if no zip file is loaded
    if (!d_pimpl->d_zfile)
        return entries;

    char current_name[1024];
    unz_file_info file_info;

    if (unzGoToFirstFile(d_pimpl->d_zfile) != UNZ_OK)
    {
        Logger::getSingleton().logEvent(
            "MinizipResourceProvider::getResourceGroupFileNames: "
            "unzGoToFirstFile failed, skipping zip file scan.", LoggingLevel::Error);

        return entries;
    }

    do
    {
        if (unzGetCurrentFileInfo(d_pimpl->d_zfile, &file_info,
                                  current_name, 1024, 0, 0, 0, 0) != UNZ_OK)
        {
            Logger::getSingleton().logEvent(
                "MinizipResourceProvider::getResourceGroupFileNames: "
                "unzGetCurrentFileInfo failed, terminating scan.", LoggingLevel::Error);

            return entries;
        }

        // skip this file if it does not match the pattern.
        if (!nameMatchesPattern(current_name, dir_name + file_pattern))
            continue;

        // strip the resource directory name and append the matched file
        out_vec.push_back(String(current_name).substr(dir_name.length()));
        ++entries;
    }
    while (unzGoToNextFile(d_pimpl->d_zfile) == UNZ_OK);

    return entries;
}

//----------------------------------------------------------------------------//
void MinizipResourceProvider::setLoadLocal(bool load)
{
    d_pimpl->d_loadLocal = load;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#endif
