/***********************************************************************
    created:    11/6/2011
    author:     Martin Preisler
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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

#define BOOST_TEST_MODULE CEGUITests

#include <boost/test/unit_test.hpp>
#include <iostream>

#include "CEGUI/RendererModules/Null/Renderer.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/Font.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/falagard/WidgetLookManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// If this looks wanky, it's because it is!  Behold that this is not as full-blown
// as it could be though.
// Copied from samples_framework/include/CEGuiBaseApplication.h
#ifndef PATH_MAX
#   include <stdlib.h>
#   ifndef PATH_MAX
#       include <limits.h>
#   endif
#   ifndef PATH_MAX
#      ifdef _MAX_PATH
#           define PATH_MAX _MAX_PATH
#       else
#           define PATH_MAX 260
#       endif
#   endif
#endif

#ifdef __APPLE__
#   include <Carbon/Carbon.h>
#endif

#define CEGUI_SAMPLE_DATAPATH_VAR "CEGUI_SAMPLE_DATAPATH"
// setup default-default path
#ifndef CEGUI_SAMPLE_DATAPATH
    #define CEGUI_SAMPLE_DATAPATH "../../datafiles"
#endif

/**
\brief This fixture sets CEGUI up with NullRenderer
*/
class CEGUIInstanceFixture
{
public:
    CEGUIInstanceFixture()
    {
        // BOOST_TEST_MESSAGE is not available here
        std::cout << "Bringing CEGUI up using NullRenderer" << std::endl;
        std::cout << "************************************" << std::endl;
        std::cout << std::endl;

        CEGUI::NullRenderer::bootstrapSystem();
        // we don't need stderr, we will deal with exception reports manually
        CEGUI::Exception::setStdErrEnabled(false);

        // FIXME: it sucks that we have to load a scheme to test but that's
        //        how it is at the moment :-(

        // initialise the required dirs for the DefaultResourceProvider
        CEGUI::DefaultResourceProvider* rp =
            static_cast<CEGUI::DefaultResourceProvider*>
                (CEGUI::System::getSingleton().getResourceProvider());

        const char* dataPathPrefix = getDataPathPrefix();
        char resourcePath[PATH_MAX];

        // set the default resource groups to be used
        CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
        CEGUI::Font::setDefaultResourceGroup("fonts");
        CEGUI::Scheme::setDefaultResourceGroup("schemes");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
        CEGUI::WindowManager::setDefaultResourceGroup("layouts");
        CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
        CEGUI::AnimationManager::setDefaultResourceGroup("animations");

        // setup default group for validation schemas
        CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
        if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
            parser->setProperty("SchemaDefaultResourceGroup", "schemas");

        // for each resource type, set a resource group directory
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
        rp->setResourceGroupDirectory("schemes", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
        rp->setResourceGroupDirectory("imagesets", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
        rp->setResourceGroupDirectory("fonts", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
        rp->setResourceGroupDirectory("layouts", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
        rp->setResourceGroupDirectory("looknfeels", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
        rp->setResourceGroupDirectory("lua_scripts", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
        rp->setResourceGroupDirectory("schemas", resourcePath);
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "animations/");
        rp->setResourceGroupDirectory("animations", resourcePath);

        CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    }

    ~CEGUIInstanceFixture()
    {
        // BOOST_TEST_MESSAGE is not available here
        std::cout << std::endl;
        std::cout << "Destroying CEGUI instance" << std::endl;

        CEGUI::NullRenderer::destroySystem();
    }

    //----------------------------------------------------------------------------//
    const char* getDataPathPrefix() const
    {
        static char dataPathPrefix[PATH_MAX];

#ifdef __APPLE__
        CFURLRef datafilesURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
                                                        CFSTR("datafiles"),
                                                        0, 0);
        CFURLGetFileSystemRepresentation(datafilesURL, true,
                                         reinterpret_cast<UInt8*>(dataPathPrefix),
                                         PATH_MAX);
        CFRelease(datafilesURL);
#else
        char* envDataPath = 0;

        // get data path from environment var
        envDataPath = getenv(CEGUI_SAMPLE_DATAPATH_VAR);

        // set data path prefix / base directory.  This will
        // be either from an environment variable, or from
        // a compiled in default based on original configure
        // options
        if (envDataPath != 0)
            strcpy(dataPathPrefix, envDataPath);
        else
            strcpy(dataPathPrefix, CEGUI_SAMPLE_DATAPATH);
#endif

        return dataPathPrefix;
    }

};

BOOST_GLOBAL_FIXTURE( CEGUIInstanceFixture );
