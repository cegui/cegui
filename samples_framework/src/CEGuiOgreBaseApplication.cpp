/***********************************************************************
    filename:   CEGuiOgreBaseApplication.cpp
    created:    9/3/2004
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
#ifdef CEGUI_SAMPLES_USE_OGRE

#include <OgreWindowEventUtilities.h>
#include "CEGuiOgreBaseApplication.h"
#include "SamplesFrameworkBase.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/RendererModules/Ogre/ResourceProvider.h"

//----------------------------------------------------------------------------//
CEGuiOgreBaseApplication::CEGuiOgreBaseApplication() :
    d_ogreRoot(0),
    d_initialised(false),
    d_frameListener(0),
    d_windowEventListener(0)
{
    using namespace Ogre;
    
#ifdef DEBUG
    Ogre::String pluginsFileName = "plugins_d.cfg";
#else
    Ogre::String pluginsFileName = "plugins.cfg";
#endif

    d_ogreRoot = new Root(pluginsFileName);

    if (d_ogreRoot->showConfigDialog())
    {
        // initialise system according to user options.
        d_window = d_ogreRoot->initialise(true);

        // Create the scene manager
        SceneManager* sm = d_ogreRoot->
            createSceneManager(ST_GENERIC, "SampleSceneMgr");
        // Create and initialise the camera
        d_camera = sm->createCamera("SampleCam");
        d_camera->setPosition(Vector3(0,0,500));
        d_camera->lookAt(Vector3(0,0,-300));
        d_camera->setNearClipDistance(5);

        // Create a viewport covering whole window
        Viewport* vp = d_window->addViewport(d_camera);
        vp->setBackgroundColour(ColourValue(1.f, 1.f, 1.f, 1.f));
        // Update the camera aspect ratio to that of the viewport
        d_camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        // create ogre renderer, image codec and resource provider.
        CEGUI::OgreRenderer& renderer = CEGUI::OgreRenderer::create();
        d_renderer = &renderer;
        d_imageCodec = &renderer.createOgreImageCodec();
        d_resourceProvider = &renderer.createOgreResourceProvider();

        // create frame listener
        d_frameListener= new CEGuiDemoFrameListener(this, d_sampleApp, d_window, d_camera);
        d_ogreRoot->addFrameListener(d_frameListener);

        // add a listener for OS framework window events (for resizing)
        d_windowEventListener = new WndEvtListener(&renderer);
        WindowEventUtilities::addWindowEventListener(d_window,
                                                     d_windowEventListener);

        d_initialised = true;
    }
    else
    {
        // aborted.  Clean up and set root to 0 so when app attempts to run it
        // knows what happened here.
        delete d_ogreRoot;
        d_ogreRoot = 0;
    }
}

//----------------------------------------------------------------------------//
CEGuiOgreBaseApplication::~CEGuiOgreBaseApplication()
{
    delete d_frameListener;

    CEGUI::OgreRenderer& renderer =
        *static_cast<CEGUI::OgreRenderer*>(d_renderer);
    renderer.destroyOgreResourceProvider(
        *static_cast<CEGUI::OgreResourceProvider*>(d_resourceProvider));
    renderer.destroyOgreImageCodec(
        *static_cast<CEGUI::OgreImageCodec*>(d_imageCodec));
    CEGUI::OgreRenderer::destroy(renderer);

    delete d_ogreRoot;
    delete d_windowEventListener;
}

//----------------------------------------------------------------------------//
bool CEGuiOgreBaseApplication::execute_impl()
{
    // if base initialisation failed or app was cancelled by user, bail out now.
    if (!d_ogreRoot || !d_initialised)
        return false;

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    // perform sample initialisation
    d_sampleApp->initialise();

    // start rendering via Ogre3D engine.
    CEGUI_TRY
    {
        d_ogreRoot->startRendering();
    }
    CEGUI_CATCH(...)
    {}

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::cleanup_impl()
{
    // nothing to do here.
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::beginRendering(const float elapsed)
{
    // this is nover called under Ogre, since we're not in control of the
    // rendering process.
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::endRendering()
{
    // this is nover called under Ogre, since we're not in control of the
    // rendering process.
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::initialiseResourceGroupDirectories()
{
    using namespace Ogre;
    ResourceGroupManager& rgm = ResourceGroupManager::getSingleton();

    // add resource groups that we use
    rgm.createResourceGroup("imagesets");
    rgm.createResourceGroup("fonts");
    rgm.createResourceGroup("layouts");
    rgm.createResourceGroup("schemes");
    rgm.createResourceGroup("looknfeels");
    rgm.createResourceGroup("lua_scripts");
    rgm.createResourceGroup("schemas");
    rgm.createResourceGroup("samples");

    // add CEGUI sample framework datafile dirs as resource locations
    ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");

    const char* dataPathPrefix = getDataPathPrefix();
    char resourcePath[PATH_MAX];

    // for each resource type, set a resource group directory
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "schemes");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "imagesets");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "fonts");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "layouts");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "looknfeels");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "lua_scripts");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "schemas");
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "samples/");
    ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "samples");
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::doFrameUpdate(const float elapsed)
{
    CEGUI::System& gui_system(CEGUI::System::getSingleton());

    gui_system.injectTimePulse(elapsed);
    d_sampleApp->update(static_cast<float>(elapsed));

    updateFPS(elapsed);
    updateLogo(elapsed);
}

//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of CEGuiDemoFrameListener mehods
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
CEGuiDemoFrameListener::CEGuiDemoFrameListener(CEGuiOgreBaseApplication* baseApp, SamplesFrameworkBase*& sampleApp,
    Ogre::RenderWindow* window, Ogre::Camera* camera, bool useBufferedInputKeys, bool useBufferedInputMouse)
    : d_baseApp(baseApp),
    d_sampleApp(sampleApp)
{
    // OIS setup
    OIS::ParamList paramList;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    // get window handle
    window->getCustomAttribute("WINDOW", &windowHnd);

    // fill param list
    windowHndStr << (unsigned int)windowHnd;
    paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX && defined (DEBUG)
    paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), "false"));
    paramList.insert(std::make_pair(std::string("x11_mouse_grab"), "false"));
    paramList.insert(std::make_pair(std::string("x11_mouse_hide"), "false"));
#endif

    // create input system
    d_inputManager = OIS::InputManager::createInputSystem(paramList);

    // create buffered keyboard
#ifdef CEGUI_OLD_OIS_API
    if (d_inputManager->numKeyboards() > 0)
#else
    if (d_inputManager->getNumberOfDevices(OIS::OISKeyboard) > 0)
#endif
    {
        d_keyboard = static_cast<OIS::Keyboard*>(d_inputManager->createInputObject(OIS::OISKeyboard, true));
        d_keyboard->setEventCallback(this);
    }

    // create buffered mouse
#ifdef CEGUI_OLD_OIS_API
    if (d_inputManager->numMice() > 0)
#else
    if (d_inputManager->getNumberOfDevices(OIS::OISMouse) > 0)
#endif
    {
        d_mouse = static_cast<OIS::Mouse*>(d_inputManager->createInputObject(OIS::OISMouse, true));
        d_mouse->setEventCallback(this);

        unsigned int width, height, depth;
        int left, top;

        window->getMetrics(width, height, depth, left, top);
        const OIS::MouseState& mouseState = d_mouse->getMouseState();
        mouseState.width = width;
        mouseState.height = height;
    }

    // store inputs we want to make use of
    d_camera = camera;
    d_window = window;
}

//----------------------------------------------------------------------------//
CEGuiDemoFrameListener::~CEGuiDemoFrameListener()
{
    if (d_inputManager)
    {
        d_inputManager->destroyInputObject(d_mouse);
        d_inputManager->destroyInputObject(d_keyboard);
        OIS::InputManager::destroyInputSystem(d_inputManager);
    }
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    if(d_window->isClosed() || d_sampleApp->isQuitting())
        return false;

    static_cast<CEGuiOgreBaseApplication*>(d_baseApp)->
        doFrameUpdate(static_cast<float>(evt.timeSinceLastFrame));

    // update input system
    if (d_mouse)
        d_mouse->capture();
    if (d_keyboard)
        d_keyboard->capture();

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::mouseMoved(const OIS::MouseEvent &e)
{
    d_sampleApp->injectMousePosition(e.state.X.abs, e.state.Y.abs);
    d_sampleApp->injectMouseWheelChange(e.state.Z.rel / 120.0f);

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::keyPressed(const OIS::KeyEvent &e)
{
    d_sampleApp->injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
    d_sampleApp->injectChar(e.text);

    return true;
}


//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::keyReleased(const OIS::KeyEvent &e)
{
    d_sampleApp->injectKeyUp(static_cast<CEGUI::Key::Scan>(e.key));

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    d_sampleApp->injectMouseButtonDown(convertOISButtonToCegui(id));

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   d_sampleApp->injectMouseButtonUp(convertOISButtonToCegui(id));

    return true;
}

//----------------------------------------------------------------------------//
CEGUI::MouseButton CEGuiDemoFrameListener::convertOISButtonToCegui(int buttonID)
{
   using namespace OIS;

   switch (buttonID)
    {
   case OIS::MB_Left:
        return CEGUI::LeftButton;
   case OIS::MB_Right:
        return CEGUI::RightButton;
   case OIS::MB_Middle:
        return CEGUI::MiddleButton;
    default:
        return CEGUI::LeftButton;
    }
}

//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of WndEvtListener member functions
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
WndEvtListener::WndEvtListener(CEGUI::OgreRenderer* renderer) :
    d_renderer(renderer)
{}

//----------------------------------------------------------------------------//
void WndEvtListener::windowResized(Ogre::RenderWindow* rw)
{
    CEGUI::System* const sys = CEGUI::System::getSingletonPtr();
    if (sys)
        sys->notifyDisplaySizeChanged(
            CEGUI::Sizef(static_cast<float>(rw->getWidth()),
                          static_cast<float>(rw->getHeight())));
}

//----------------------------------------------------------------------------//

#endif



