/***********************************************************************
    created:    7/2/2008
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
#include "CEGUISamplesConfig.h"
#include "CEGuiBaseApplication.h"
#include "SamplesFramework.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/Image.h"
#include "CEGUI/Font.h"
#include "CEGUI/Scheme.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/ScriptModule.h"
#include "CEGUI/XMLParser.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/RenderTarget.h"
#include "CEGUI/AnimationManager.h"
#include "CEGUI/FontManager.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#ifdef __APPLE__
#   include <Carbon/Carbon.h>
#endif

// setup default-default path
#ifndef CEGUI_SAMPLE_DATAPATH
    #define CEGUI_SAMPLE_DATAPATH "../datafiles"
#endif

/***********************************************************************
    Static / Const data
*************************************************************************/
const char CEGuiBaseApplication::DATAPATH_VAR_NAME[] = "CEGUI_SAMPLE_DATAPATH";
SamplesFrameworkBase* CEGuiBaseApplication::d_sampleApp(0);

//----------------------------------------------------------------------------//
CEGuiBaseApplication::CEGuiBaseApplication() :
    d_quitting(false),
    d_renderer(0),
    d_imageCodec(0),
    d_resourceProvider(0),
    d_logoGeometry(0),
    d_FPSGeometry(0),
    d_FPSElapsed(0.0f),
    d_FPSFrames(0),
    d_FPSValue(0),
    d_spinLogo(false)
{
}

//----------------------------------------------------------------------------//
CEGuiBaseApplication::~CEGuiBaseApplication()
{
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::renderSingleFrame(const float elapsed)
{
    CEGUI::System& gui_system(CEGUI::System::getSingleton());

    gui_system.injectTimePulse(elapsed);
    d_sampleApp->update(static_cast<float>(elapsed));

    updateFPS(elapsed);
    updateLogo(elapsed);

    beginRendering(elapsed);

    CEGUI::Renderer* gui_renderer(gui_system.getRenderer());
    gui_renderer->beginRendering();

    d_sampleApp->renderGUIContexts();

    gui_renderer->endRendering();
    CEGUI::WindowManager::getSingleton().cleanDeadPool();

    endRendering();
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::init(SamplesFrameworkBase* sampleApp,
  const CEGUI::String& logFile, const CEGUI::String& dataPathPrefixOverride)
{
    d_sampleApp = sampleApp;

    if (!d_renderer)
        CEGUI_THROW(CEGUI::InvalidRequestException("CEGuiBaseApplication::run: "
            "Base application subclass did not create Renderer!"));

    // start up CEGUI system using objects created in subclass constructor.
    CEGUI::System::create(*d_renderer, d_resourceProvider, 0, d_imageCodec, 0,
                          "", logFile);

    // initialise resource system
    initialiseResourceGroupDirectories(dataPathPrefixOverride);
    initialiseDefaultResourceGroups();

    const CEGUI::Rectf scrn(CEGUI::Vector2f(0, 0), d_renderer->getDisplaySize());

    // setup for FPS value
    d_FPSGeometry = &d_renderer->createGeometryBuffer();
    d_FPSGeometry->setClippingRegion(scrn);
    positionFPS();

    // setup for spinning logo
    d_logoGeometry = &d_renderer->createGeometryBuffer();
    d_logoGeometry->setPivot(CEGUI::Vector3f(91.5f, 44.5f, 0));
    positionLogo();

    // create logo imageset and draw the image (we only ever draw this once)
    CEGUI::ImageManager::getSingleton().addFromImageFile("cegui_logo",
                                                         "logo.png");
    CEGUI::ImageManager::getSingleton().get("cegui_logo").render(
        *d_logoGeometry, CEGUI::Rectf(0, 0, 183, 89), 0, CEGUI::ColourRect(0xFFFFFFFF));

    // clearing this queue actually makes sure it's created(!)
    CEGUI::System::getSingleton().getDefaultGUIContext().clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    CEGUI::System::getSingleton().getDefaultGUIContext().
        subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
            CEGUI::Event::Subscriber(&CEGuiBaseApplication::sampleBrowserOverlayHandler,
                                     this));

    // subscribe handler to reposition logo when window is sized.
    CEGUI::System::getSingleton().subscribeEvent(
        CEGUI::System::EventDisplaySizeChanged,
        CEGUI::Event::Subscriber(&CEGuiBaseApplication::resizeHandler,
        this));

    const CEGUI::Rectf& area(CEGUI::System::getSingleton().getRenderer()->
                             getDefaultRenderTarget().getArea());
    d_sampleApp->setApplicationWindowSize(static_cast<int>(area.getWidth()),
                                          static_cast<int>(area.getHeight()));

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::cleanup()
{
    if (d_renderer)
    {
        CEGUI::ImageManager::getSingleton().destroy("cegui_logo");
        d_renderer->destroyGeometryBuffer(*d_logoGeometry);
        d_renderer->destroyGeometryBuffer(*d_FPSGeometry);
        CEGUI::System::destroy();
        destroyRenderer();
        destroyWindow();
    }
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::destroyRenderer()
{
    CEGUI_DELETE_AO d_renderer;
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::initialiseResourceGroupDirectories
  (const CEGUI::String& dataPathPrefixOverride)
{
    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>
            (CEGUI::System::getSingleton().getResourceProvider());
    initDataPathPrefix(dataPathPrefixOverride);
    CEGUI::String dataPathPrefix(getDataPathPrefix());

    /* for each resource type, set a resource group directory. We cast strings
       to "const CEGUI::utf8*" in order to support general Unicode strings,
       rather than only ASCII strings (even though currently they're all ASCII).
       */
    rp->setResourceGroupDirectory("schemes",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/schemes/"));
    rp->setResourceGroupDirectory("imagesets",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/imagesets/"));
    rp->setResourceGroupDirectory("fonts",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/fonts/"));
    rp->setResourceGroupDirectory("layouts",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/layouts/"));
    rp->setResourceGroupDirectory("looknfeels",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/looknfeel/"));
    rp->setResourceGroupDirectory("lua_scripts",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/lua_scripts/"));
    rp->setResourceGroupDirectory("schemas",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/xml_schemas/"));   
    rp->setResourceGroupDirectory("animations",
      dataPathPrefix +reinterpret_cast<const CEGUI::utf8*>("/animations/"));
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::initialiseDefaultResourceGroups()
{
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
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::initDataPathPrefix(const CEGUI::String &override)
{

    if (override.empty())
    {

#       ifdef __APPLE__

            char c_str[PATH_MAX];
            CFURLRef datafilesURL = CFBundleCopyResourceURL(CFBundleGetMainBundle(),
                                                            CFSTR("datafiles"),
                                                            0, 0);
            CFURLGetFileSystemRepresentation(datafilesURL, true,
                                             reinterpret_cast<UInt8*>(c_str),
                                             PATH_MAX);
            CFRelease(datafilesURL);
            d_dataPathPrefix = c_str;

#       else

            // get data path from environment var
            char* envDataPath = getenv(DATAPATH_VAR_NAME);

            // set data path prefix / base directory.  This will
            // be either from an environment variable, or from
            // a compiled in default based on original configure
            // options
            d_dataPathPrefix = envDataPath ?
                               envDataPath : CEGUI_SAMPLE_DATAPATH;

#       endif

    }
    else
        d_dataPathPrefix = override;
        
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::sampleBrowserOverlayHandler(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
        return false;

    // draw the logo
    d_logoGeometry->draw();
    // draw FPS value
    d_FPSGeometry->draw();

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::sampleOverlayHandler(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
        return false;

    // draw FPS value
    d_FPSGeometry->draw();

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateFPS(const float elapsed)
{
    // another frame
    ++d_FPSFrames;

    if ((d_FPSElapsed += elapsed) >= 1.0f)
    {
        if (d_FPSFrames != d_FPSValue)
        {
            d_FPSValue = d_FPSFrames;

            CEGUI::Font* fnt = &CEGUI::FontManager::getSingleton().get("DejaVuSans-12");
            if (!fnt)
                return;

            // update FPS imagery
            char fps_textbuff[16];
            sprintf(fps_textbuff , "FPS: %d", d_FPSValue);

            d_FPSGeometry->reset();
            fnt->drawText(*d_FPSGeometry, fps_textbuff, CEGUI::Vector2f(0, 0), 0,
                        CEGUI::Colour(0xFFFFFFFF));
        }

        // reset counter state
        d_FPSFrames = 0;

        float modValue = 1.f; 
        d_FPSElapsed = std::modf(d_FPSElapsed, &modValue);
    }
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::updateLogo(const float elapsed)
{
    if (!d_spinLogo)
        return;

    static float rot = 0.0f;
    d_logoGeometry->setRotation(CEGUI::Quaternion::eulerAnglesDegrees(rot, 0, 0));

    rot = fmodf(rot + 180.0f * elapsed, 360.0f);
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::positionLogo()
{
    const CEGUI::Rectf scrn(d_renderer->getDefaultRenderTarget().getArea());

    d_logoGeometry->setClippingRegion(scrn);
    d_logoGeometry->setTranslation(
        CEGUI::Vector3f(10.0f, scrn.getSize().d_height - 89.0f, 0.0f));
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::positionFPS()
{
    const CEGUI::Rectf scrn(d_renderer->getDefaultRenderTarget().getArea());

    d_FPSGeometry->setClippingRegion(scrn);
    d_FPSGeometry->setTranslation(
        CEGUI::Vector3f(scrn.getSize().d_width - 120.0f, 0.0f, 0.0f));
}


//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::resizeHandler(const CEGUI::EventArgs& /*args*/)
{
    // clear FPS geometry and see that it gets recreated in the next frame
    d_FPSGeometry->reset();
    d_FPSValue = 0;

    const CEGUI::Rectf& area(CEGUI::System::getSingleton().getRenderer()->
                             getDefaultRenderTarget().getArea());
    d_sampleApp->handleNewWindowSize(area.getWidth(), area.getHeight());

    positionLogo();
    return true;
}

//----------------------------------------------------------------------------//

void CEGuiBaseApplication::registerSampleOverlayHandler(CEGUI::GUIContext* gui_context)
{
    // clearing this queue actually makes sure it's created(!)
    gui_context->clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    gui_context->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
        CEGUI::Event::Subscriber(&CEGuiBaseApplication::sampleOverlayHandler,
        this));
}


//----------------------------------------------------------------------------//
