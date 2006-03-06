/************************************************************************
    filename:   CEGuiIrrlichtBaseApplication.cpp
    created:    24/9/2004
    author:     Paul D Turner
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

// this controls conditional compile of file for MSVC
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

    rp->setResourceGroupDirectory("schemes", "../datafiles/schemes/");
    rp->setResourceGroupDirectory("imagesets", "../datafiles/imagesets/");
    rp->setResourceGroupDirectory("fonts", "../datafiles/fonts/");
    rp->setResourceGroupDirectory("layouts", "../datafiles/layouts/");
    rp->setResourceGroupDirectory("looknfeels", "../datafiles/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", "../datafiles/lua_scripts/");
#if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
    rp->setResourceGroupDirectory("schemas", "../../XMLRefSchema/");
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

bool CEGuiIrrlichtBaseApplication::OnEvent(irr::SEvent event)
{
    return (d_renderer != 0) ? d_renderer->OnEvent(event) : false;
}

#endif

