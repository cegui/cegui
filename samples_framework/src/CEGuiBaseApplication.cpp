/***********************************************************************
    filename:   CEGuiBaseApplication.cpp
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
    WindowManager::getSingleton().cleanDeadPool();

    endRendering();
}

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::execute(SamplesFrameworkBase* sampleApp)
{
    d_sampleApp = sampleApp;

    if (!d_renderer)
        throw CEGUI::InvalidRequestException("CEGuiBaseApplication::execute: "
            "Base application subclass did not create Renderer!");

    // start up CEGUI system using objects created in subclass constructor.
    CEGUI::System::create(*d_renderer, d_resourceProvider, 0, d_imageCodec);

    // initialise resource system
    initialiseResourceGroupDirectories();
    initialiseDefaultResourceGroups();

    const CEGUI::Rectf scrn(CEGUI::Vector2f(0, 0), d_renderer->getDisplaySize());

    // setup for FPS value
    d_FPSGeometry = &d_renderer->createGeometryBuffer();
    d_FPSGeometry->setClippingRegion(scrn);

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
            CEGUI::Event::Subscriber(&CEGuiBaseApplication::overlayHandler,
                                     this));

    // subscribe handler to reposition logo when window is sized.
    CEGUI::System::getSingleton().subscribeEvent(
        CEGUI::System::EventDisplaySizeChanged,
        CEGUI::Event::Subscriber(&CEGuiBaseApplication::resizeHandler,
        this));

    const Rectf& area(CEGUI::System::getSingleton().getRenderer()->getDefaultRenderTarget().getArea());
    d_sampleApp->setApplicationWindowSize(static_cast<int>(area.getWidth()), static_cast<int>(area.getHeight()));

    return execute_impl();
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::cleanup()
{
    cleanup_impl();

    CEGUI::ImageManager::getSingleton().destroy("cegui_logo");
    d_renderer->destroyGeometryBuffer(*d_logoGeometry);
    d_renderer->destroyGeometryBuffer(*d_FPSGeometry);
    CEGUI::System::destroy();
}

//----------------------------------------------------------------------------//
void CEGuiBaseApplication::initialiseResourceGroupDirectories()
{
    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>
            (CEGUI::System::getSingleton().getResourceProvider());

    const char* dataPathPrefix = getDataPathPrefix();
    char resourcePath[PATH_MAX];

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
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "samples/");
    rp->setResourceGroupDirectory("samples", resourcePath); 
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

    SamplesFramework::setDefaultResourceGroup("samples");

    // setup default group for validation schemas
    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");
}

//----------------------------------------------------------------------------//
const char* CEGuiBaseApplication::getDataPathPrefix() const
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
    envDataPath = getenv(DATAPATH_VAR_NAME);

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

//----------------------------------------------------------------------------//
bool CEGuiBaseApplication::overlayHandler(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    if (static_cast<const RenderQueueEventArgs&>(args).queueID != RQ_OVERLAY)
        return false;

    // draw the logo
    d_logoGeometry->draw();
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

            CEGUI::Font* fnt = CEGUI::System::getSingleton().getDefaultGUIContext().getDefaultFont();
            if (!fnt)
                return;

            // update FPS imagery
            char fps_textbuff[16];
            sprintf(fps_textbuff , "FPS: %d", d_FPSValue);

            d_FPSGeometry->reset();
            fnt->drawText(*d_FPSGeometry, fps_textbuff, CEGUI::Vector2f(0, 0), 0,
                        CEGUI::Colour(0xFF000000));
        }

        // reset counter state
        d_FPSFrames = 0;
        d_FPSElapsed = 0.0f;
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
bool CEGuiBaseApplication::resizeHandler(const CEGUI::EventArgs& /*args*/)
{
    // clear FPS geometry and see that it gets recreated in the next frame
    d_FPSGeometry->reset();
    d_FPSValue = 0;

    const Rectf& area(CEGUI::System::getSingleton().getRenderer()->getDefaultRenderTarget().getArea());
    d_sampleApp->handleNewWindowSize(area.getWidth(), area.getHeight());

    positionLogo();
    return true;
}

//----------------------------------------------------------------------------//
