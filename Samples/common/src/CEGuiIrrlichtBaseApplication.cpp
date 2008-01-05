/***********************************************************************
    filename:   CEGuiIrrlichtBaseApplication.cpp
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#ifdef __linux__
# include <unistd.h>
# define DATAPATH_VAR_NAME "CEGUI_SAMPLE_DATAPATH"
#endif

// this controls conditional compile of file for Apple
#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_USE_IRRLICHT

#include "CEGuiIrrlichtBaseApplication.h"
#include "CEGuiSample.h"
#include "CEGUIDefaultResourceProvider.h"

CEGuiIrrlichtBaseApplication::CEGuiIrrlichtBaseApplication() :
        d_device(0),
        d_driver(0),
        d_smgr(0),
        d_renderer(0)
{
    using namespace irr;

    // create a device
    d_device = irr::createDevice(video::EDT_OPENGL, core::dimension2d<s32>(800, 600), 32, false, true, false, this);

    // set flags for texture creation
    d_device->getCursorControl()->setVisible(false);
    d_device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT,true);
    d_device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_ALWAYS_16_BIT,false);
    d_device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS,false);
    d_device->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_OPTIMIZED_FOR_QUALITY,true);

    // get driver and scenemanager
    d_driver = d_device->getVideoDriver();
    d_smgr = d_device->getSceneManager();

    // create a renderer which uses the IrrlichtResourceProvider to load data
    d_renderer= new CEGUI::IrrlichtRenderer(d_device, false);

    // create the gui
    new CEGUI::System(d_renderer);

    // initialise the required dirs for the ResourceProvider
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());


#if defined(__linux__)
    char dataPathPrefix[PATH_MAX];
    char resourcePath[PATH_MAX];

    // get data path from environment var
    char* envDataPath = getenv(DATAPATH_VAR_NAME);

    // set data path prefix / base directory.  This will
    // be either from an environment variable, or from
    // a compiled in default based on original configure
    // options
    if (envDataPath != 0)
        strcpy(dataPathPrefix, envDataPath);
    else
        strcpy(dataPathPrefix, CEGUI_SAMPLE_DATAPATH);

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
    #if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "XMLRefSchema/");
        rp->setResourceGroupDirectory("schemas", resourcePath);
    #endif
#else
    rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
    rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
    rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
    rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
    #if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
        rp->setResourceGroupDirectory("schemas", "../../XMLRefSchema/");
    #endif
#endif

    irr::scene::ICameraSceneNode* camera = d_smgr->addCameraSceneNode(0, core::vector3df(0,0,0), core::vector3df(0,0,1));
    camera->setFOV(1.56f);
    d_driver->setAmbientLight(video::SColor(255,255,255,255));

    d_lastTime = d_device->getTimer()->getRealTime();
}

CEGuiIrrlichtBaseApplication::~CEGuiIrrlichtBaseApplication()
{
    // free the gui system
    delete CEGUI::System::getSingletonPtr();

    if (d_renderer)
    {
        delete d_renderer;
    }

    if (d_device)
    {
        d_device->drop();
    }
}

bool CEGuiIrrlichtBaseApplication::execute(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();

    // draw everything
    while(d_device->run())
    {
        // draw only if the window is active
        if (d_device->isWindowActive())
        {
            // calculate time elapsed
            irr::u32 currTime = d_device->getTimer()->getRealTime();
            // inject time pulse
            CEGUI::System::getSingleton().injectTimePulse(static_cast<float>(currTime - d_lastTime) / 1000.0f);
            d_lastTime = currTime;

            // start rendering
            d_driver->beginScene(true, true, irr::video::SColor(150,50,50,50));
            //draw scene
            d_smgr->drawAll();
            // draw gui
            CEGUI::System::getSingleton().renderGUI();
            d_driver->endScene();
        }

        // see if we should quit
        if (isQuitting())
            d_device->closeDevice();
    }

    return true;
}

void CEGuiIrrlichtBaseApplication::cleanup()
{
    // Nothing to do here.
}

#if CEGUI_IRR_SDK_VERSION >= 14
bool CEGuiIrrlichtBaseApplication::OnEvent(const irr::SEvent& event)
#else
bool CEGuiIrrlichtBaseApplication::OnEvent(irr::SEvent event)
#endif
{
    // cegui samples always quit on escape
    if (event.EventType == irr::EET_KEY_INPUT_EVENT)
    {
        if (event.KeyInput.Key == irr::KEY_ESCAPE)
        {
            setQuitting();
            return true;
        }
    }

    return (d_renderer != 0) ? d_renderer->OnEvent(event) : false;
}

#endif

