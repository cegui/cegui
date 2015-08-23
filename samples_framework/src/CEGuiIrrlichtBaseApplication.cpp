/***********************************************************************
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
#ifdef CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE

#include "CEGuiIrrlichtBaseApplication.h"
#include "SamplesFrameworkBase.h"
#include "CEGUI/RendererModules/Irrlicht/ImageCodec.h"
#include "CEGUI/RendererModules/Irrlicht/ResourceProvider.h"
#include "CEGUI/RendererModules/Irrlicht/Renderer.h"
#include "CEGUI/System.h"
#include "CEGUI/GUIContext.h"

#include "CEGUI/RendererModules/Irrlicht/EventPusher.h"

using namespace irr;

//----------------------------------------------------------------------------//
CEGuiIrrlichtBaseApplication::CEGuiIrrlichtBaseApplication() :
d_device(0),
    d_driver(0),
    d_smgr(0),
    d_eventPusher(0),
    d_lastDisplaySize( static_cast<const float>(s_defaultWindowWidth), static_cast<const float>(s_defaultWindowHeight) )
{
    using namespace irr;

    // create a device
    d_device = irr::createDevice(video::EDT_OPENGL,
#if CEGUI_IRR_SDK_VERSION >= 16
        core::dimension2d<u32>(s_defaultWindowWidth, s_defaultWindowHeight),
#else
        core::dimension2d<s32>(s_defaultWindowWidth, s_defaultWindowHeight),
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
    d_device->setResizable(true);

    // get driver and scenemanager
    d_driver = d_device->getVideoDriver();
    d_smgr = d_device->getSceneManager();

    irr::scene::ICameraSceneNode* camera =
        d_smgr->addCameraSceneNode(0, core::vector3df(0, 0, 0),
        core::vector3df(0, 0, 1));
    camera->setFOV(1.56f);
    d_driver->setAmbientLight(video::SColor(255, 255, 255, 255));

    d_lastTime = d_device->getTimer()->getRealTime();

    // create irrlicht renderer, image codec and resource provider.
    CEGUI::IrrlichtRenderer& renderer =
        CEGUI::IrrlichtRenderer::create(*d_device);

    d_renderer = &renderer;
    d_imageCodec = &renderer.createIrrlichtImageCodec(*d_driver);
    d_resourceProvider =
        &renderer.createIrrlichtResourceProvider(*d_device->getFileSystem());
    d_eventPusher = renderer.getEventPusher();
}

//----------------------------------------------------------------------------//
CEGuiIrrlichtBaseApplication::~CEGuiIrrlichtBaseApplication()
{
    if (d_device)
        d_device->drop();
}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::destroyRenderer()
{
    CEGUI::IrrlichtRenderer& renderer =
        *static_cast<CEGUI::IrrlichtRenderer*>(d_renderer);

    renderer.destroyIrrlichtResourceProvider(
        *static_cast<CEGUI::IrrlichtResourceProvider*>(d_resourceProvider));

    renderer.destroyIrrlichtImageCodec(
        *static_cast<CEGUI::IrrlichtImageCodec*>(d_imageCodec));

    CEGUI::IrrlichtRenderer::destroy(renderer);
}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::run()
{
    CEGUI::System& guiSystem = CEGUI::System::getSingleton();

    // draw everything
    while (d_device->run())
    {
        // draw only if the window is active
        if (d_device->isWindowActive())
        {
            checkWindowResize();

            // calculate time elapsed
            const irr::u32 currTime = d_device->getTimer()->getRealTime();
            const float elapsed =
                static_cast<float>(currTime - d_lastTime) / 1000.0f;
            d_lastTime = currTime;

            renderSingleFrame(elapsed);
        }

        // see if we should quit
        if (d_sampleApp->isQuitting())
            d_device->closeDevice();
    }
}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::destroyWindow()
{

}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::beginRendering(const float /*elapsed*/)
{
    d_driver->beginScene(true, true, irr::video::SColor(0, 0, 0, 0));
    d_smgr->drawAll();
}

//----------------------------------------------------------------------------//
void CEGuiIrrlichtBaseApplication::endRendering()
{
    d_driver->endScene();
}

//----------------------------------------------------------------------------//
#if CEGUI_IRR_SDK_VERSION >= 14
bool CEGuiIrrlichtBaseApplication::OnEvent(const irr::SEvent& event)
#else
bool CEGuiIrrlichtBaseApplication::OnEvent(irr::SEvent event)
#endif
{
    return (d_renderer != 0) ?
        processEvent(event) :
    false;
}
//----------------------------------------------------------------------------//
bool CEGuiIrrlichtBaseApplication::processEvent(const irr::SEvent& event)
{
    switch (event.EventType)
    {
    case EET_KEY_INPUT_EVENT :
        if (event.KeyInput.PressedDown)
            return OnKeyDown(event.KeyInput.Key, event.KeyInput.Char, event.KeyInput.Control, event.KeyInput.Shift);
        else
            return OnKeyUp(event.KeyInput.Key, event.KeyInput.Char, event.KeyInput.Control, event.KeyInput.Shift);
        break;

    case EET_MOUSE_INPUT_EVENT :
        return OnMouse(event.MouseInput.X, event.MouseInput.Y, event.MouseInput.Wheel, event.MouseInput.Event);
        break;

    default:
        break;
    }

    return false;
}
//----------------------------------------------------------------------------//
bool CEGuiIrrlichtBaseApplication::OnKeyDown(EKEY_CODE key, wchar_t wch, bool /*ctrl*/, bool /*shift*/)
{
    bool handled = false;
    handled = d_sampleApp->injectKeyDown(d_eventPusher->getKeyCode(key));
    handled = d_sampleApp->injectChar(wch) || handled;
    return true;
}
//----------------------------------------------------------------------------//
bool CEGuiIrrlichtBaseApplication::OnKeyUp(EKEY_CODE key, wchar_t /*wch*/, bool /*ctrl*/, bool /*shift*/)
{
    bool handled = false;
    handled = d_sampleApp->injectKeyUp(d_eventPusher->getKeyCode(key));
    return handled;
}
//----------------------------------------------------------------------------//
bool CEGuiIrrlichtBaseApplication::OnMouse(s32 x, s32 y, f32 w, EMOUSE_INPUT_EVENT e)
{
    using namespace irr;
    bool handled = false;

    switch (e)
    {
        //! Left mouse button was pressed down.
    case EMIE_LMOUSE_PRESSED_DOWN:
        handled = d_sampleApp->injectMouseButtonDown(CEGUI::LeftButton);
        break;
        //! Right mouse button was pressed down.
    case EMIE_RMOUSE_PRESSED_DOWN:
        handled = d_sampleApp->injectMouseButtonDown(CEGUI::RightButton);
        break;
        //! Middle mouse button was pressed down.
    case EMIE_MMOUSE_PRESSED_DOWN:
        handled = d_sampleApp->injectMouseButtonDown(CEGUI::MiddleButton);
        break;
        //! Left mouse button was left up.
    case EMIE_LMOUSE_LEFT_UP:
        handled = d_sampleApp->injectMouseButtonUp(CEGUI::LeftButton);
        break;
        //! Right mouse button was left up.
    case EMIE_RMOUSE_LEFT_UP:
        handled = d_sampleApp->injectMouseButtonUp(CEGUI::RightButton);
        break;
        //! Middle mouse button was left up.
    case EMIE_MMOUSE_LEFT_UP:
        handled = d_sampleApp->injectMouseButtonUp(CEGUI::MiddleButton);
        break;
        //! The mouse cursor changed its position.
    case EMIE_MOUSE_MOVED:
        handled = d_sampleApp->injectMousePosition(
            static_cast<float>(x), static_cast<float>(y));
        break;
        //! The mouse wheel was moved. Use Wheel value in event data to find out
        //! in what direction and how fast.
    case EMIE_MOUSE_WHEEL:
        handled = d_sampleApp->injectMouseWheelChange(w);
        break;
    default:
        break;
    }
    return handled;

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

#endif
