/***********************************************************************
    filename:   CEGuiIrrlichtBaseApplication.cpp
    created:    24/9/2004
    author:     Paul D Turner
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
# include <unistd.h>
#endif

// this controls conditional compile of file for Apple
#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_USE_IRRLICHT

#include "CEGuiIrrlichtBaseApplication.h"
#include "CEGuiSample.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIRenderingRoot.h"

//----------------------------------------------------------------------------//
CEGuiIrrlichtBaseApplication::CEGuiIrrlichtBaseApplication() :
    d_device(0),
    d_driver(0),
    d_smgr(0),
    d_renderer(0),
    d_fps_value(0),
    d_lastDisplaySize(800, 600)
{
    using namespace irr;

    // create a device
    d_device = irr::createDevice(video::EDT_OPENGL,
                                 #if CEGUI_IRR_SDK_VERSION >= 16
                                    core::dimension2d<u32>(800, 600),
                                 #else
                                    core::dimension2d<s32>(800, 600),
                                 #endif
                                 32, false, true, false, this);

    // set flags for texture creation
    d_device->getCursorControl()->setVisible(false);
    d_device->getVideoDriver()->
        setTextureCreationFlag(irr::video::ETCF_ALWAYS_32_BIT, true);
    d_device->getVideoDriver()->
        setTextureCreationFlag(irr::video::ETCF_ALWAYS_16_BIT, false);
    d_device->getVideoDriver()->
        setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
    d_device->getVideoDriver()->
        setTextureCreationFlag(irr::video::ETCF_OPTIMIZED_FOR_QUALITY, true);

    // get driver and scenemanager
    d_driver = d_device->getVideoDriver();
    d_smgr = d_device->getSceneManager();

    // bootstrap the CEGUI system.
    d_renderer = &CEGUI::IrrlichtRenderer::bootstrapSystem(*d_device);

    // resource system initialisation
    initialiseResourceGroupDirectories();
    initialiseDefaultResourceGroups();

    irr::scene::ICameraSceneNode* camera =
        d_smgr->addCameraSceneNode(0, core::vector3df(0, 0, 0),
                                   core::vector3df(0, 0, 1));
    camera->setFOV(1.56f);
    d_driver->setAmbientLight(video::SColor(255, 255, 255, 255));

    // setup required to do direct rendering of FPS value
    const CEGUI::Rect scrn(CEGUI::Vector2(0, 0),
                            d_renderer->getDisplaySize());
    d_fps_geometry = &d_renderer->createGeometryBuffer();
    d_fps_geometry->setClippingRegion(scrn);

    // setup for logo
    CEGUI::ImagesetManager::getSingleton().
        createFromImageFile("cegui_logo", "logo.png", "imagesets");
    d_logo_geometry = &d_renderer->createGeometryBuffer();
    d_logo_geometry->setClippingRegion(scrn);
    d_logo_geometry->setPivot(CEGUI::Vector3(50, 34.75f, 0));
    d_logo_geometry->setTranslation(CEGUI::Vector3(10, 520, 0));
    CEGUI::ImagesetManager::getSingleton().get("cegui_logo").
        getImage("full_image").draw(*d_logo_geometry,
                                    CEGUI::Rect(0, 0, 100, 69.5f), 0);

    // clearing this queue actually makes sure it's created(!)
    d_renderer->getDefaultRenderingRoot().clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    d_renderer->getDefaultRenderingRoot().
        subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
            CEGUI::Event::Subscriber(
                &CEGuiIrrlichtBaseApplication::overlayHandler, this));


    d_lastTime = d_device->getTimer()->getRealTime();
}

//----------------------------------------------------------------------------//
CEGuiIrrlichtBaseApplication::~CEGuiIrrlichtBaseApplication()
{
    // free the gui system and related objects
    CEGUI::IrrlichtRenderer::destroySystem();

    if (d_device)
        d_device->drop();
}

//----------------------------------------------------------------------------//
bool CEGuiIrrlichtBaseApplication::execute(CEGuiSample* sampleApp)
{
    sampleApp->initialiseSample();

    CEGUI::System& guiSystem = CEGUI::System::getSingleton();

    // draw everything
    while (d_device->run())
    {
        // draw only if the window is active
        if (d_device->isWindowActive())
        {
            checkWindowResize();

            // calculate time elapsed
            irr::u32 currTime = d_device->getTimer()->getRealTime();
            // inject time pulse
            const float elapsed =
                static_cast<float>(currTime - d_lastTime) / 1000.0f;
            guiSystem.injectTimePulse(elapsed);
            d_lastTime = currTime;

            // update fps text when needed
            int fps = d_driver->getFPS();
            if (fps != d_fps_value)
            {
                sprintf(d_fps_textbuff , "FPS: %d", fps);
                d_fps_value = fps;
            }

            // update logo rotation
            static float rot = 0.0f;
            d_logo_geometry->setRotation(CEGUI::Vector3(rot, 0, 0));
            rot += 180.0f * elapsed;
            if (rot > 360.0f)
                rot -= 360.0f;

            // start rendering
            d_driver->beginScene(true, true, irr::video::SColor(0, 0, 0, 0));
            //draw scene
            d_smgr->drawAll();
            // draw gui
            guiSystem.renderGUI();

            d_driver->endScene();
        }

        // see if we should quit
        if (isQuitting())
            d_device->closeDevice();
    }

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::cleanup()
{
    // Nothing to do here.
}

//----------------------------------------------------------------------------//
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

    return (d_renderer != 0) ? d_renderer->injectEvent(event) : false;
    return false;
}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::checkWindowResize()
{
    #if CEGUI_IRR_SDK_VERSION >= 16
        irr::core::dimension2d<irr::u32> cur_size = d_driver->getScreenSize();
    #else
        irr::core::dimension2d<irr::s32> cur_size = d_driver->getScreenSize();
    #endif

    if ((static_cast<float>(cur_size.Width) != d_lastDisplaySize.d_width) ||
        (static_cast<float>(cur_size.Height) != d_lastDisplaySize.d_height))
    {
        d_lastDisplaySize.d_width = static_cast<float>(cur_size.Width);
        d_lastDisplaySize.d_height = static_cast<float>(cur_size.Height);
        CEGUI::System::getSingleton().
            notifyDisplaySizeChanged(d_lastDisplaySize);
    }
}

//----------------------------------------------------------------------------//
bool CEGuiIrrlichtBaseApplication::overlayHandler(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    if (static_cast<const RenderQueueEventArgs&>(args).queueID != RQ_OVERLAY)
        return false;

    // render FPS:
    Font* fnt = System::getSingleton().getDefaultFont();
    if (fnt)
    {
        d_fps_geometry->reset();
        fnt->drawText(*d_fps_geometry, d_fps_textbuff, Vector2(0, 0), 0,
                      colour(0xFFFFFFFF));
        d_fps_geometry->draw();
    }

    d_logo_geometry->draw();

    return true;
}

#endif
