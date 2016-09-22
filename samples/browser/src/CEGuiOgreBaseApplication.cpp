/***********************************************************************
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
    #include "config.h"
#endif

#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
#include <unistd.h>
#endif

// this controls conditional compile of file for Apple
#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE

#include <OgreWindowEventUtilities.h>
#include "CEGuiOgreBaseApplication.h"
#include "SampleBrowserBase.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/RendererModules/Ogre/ResourceProvider.h"

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorCommon.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClear.h>
#include <Compositor/Pass/PassScene/OgreCompositorPassScene.h>
#include <Compositor/OgreTextureDefinition.h>
#endif //CEGUI_USE_OGRE_COMPOSITOR2

#ifdef __ANDROID__
#   include "AndroidAppHelper.h"
#endif

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
#ifndef OGRE_STATIC_LIB
    d_ogreRoot = new Ogre::Root(pluginsFileName);
#else
#   ifdef __ANDROID__
        d_ogreRoot = new Ogre::Root("", "", "Ogre.log"); //this is needed to avoid looking for plugins.cfg
#   else
        d_ogreRoot = new Ogre::Root();
#   endif
    d_staticPluginLoader = new Ogre::StaticPluginLoader();
    d_staticPluginLoader->load();
#endif

    setupDefaultConfigIfNeeded();

    if (d_ogreRoot->showConfigDialog())
    {
#ifdef __ANDROID__
        d_ogreRoot->setRenderSystem(d_ogreRoot->getAvailableRenderers().at (0));
        d_ogreRoot->initialise (false);
        AndroidAppHelper::createWindow();
        d_window = AndroidAppHelper::getRenderWindow();
        AndroidAppHelper::initialiseFS();
#else
        d_window = d_ogreRoot->initialise(true);
#endif

#if CEGUI_OGRE_VERSION_MAJOR < 2
        // Create the scene manager
        SceneManager* sm = d_ogreRoot->
            createSceneManager(ST_GENERIC,
            "SampleSceneMgr");
#else
        // Create the scene manager
        SceneManager* sm = d_ogreRoot->
            createSceneManager(ST_GENERIC, 2, INSTANCING_CULLING_SINGLETHREAD,
            "SampleSceneMgr");
#endif
        // Create and initialise the camera
        d_camera = sm->createCamera("SampleCam");
        d_camera->setPosition(Vector3(0,0,500));
        d_camera->lookAt(Vector3(0,0,-300));
        d_camera->setNearClipDistance(5);

#ifdef CEGUI_USE_OGRE_COMPOSITOR2

        Ogre::CompositorManager2* manager = d_ogreRoot->getCompositorManager2();

        // The compositor has to be initialized
        if (!manager)
        {
            assert(false && "No function to initialize Ogre::CompositorManager2...");
            manager = d_ogreRoot->getCompositorManager2();
        }

        // Create a full screen workspace that just clears the screen
        

        // Define the workspace first
        Ogre::CompositorWorkspaceDef* templatedworkspace = manager->addWorkspaceDefinition(
            "Sample_workspace");

        // Create a node for rendering on top of everything
        Ogre::CompositorNodeDef* rendernode = manager->addNodeDefinition("SampleCleaner");
        
        rendernode->addTextureSourceName("renderwindow", 0,
            Ogre::TextureDefinitionBase::TEXTURE_INPUT);

        rendernode->setNumTargetPass(1);
        // Pass for it
        Ogre::CompositorTargetDef* targetpasses = rendernode->addTargetPass("renderwindow");
        targetpasses->setNumPasses(2);

        Ogre::CompositorPassClearDef* clearpass =
            static_cast<Ogre::CompositorPassClearDef*>(targetpasses->
            addPass(Ogre::PASS_CLEAR));

        // Only clear depth and stencil since we are rendering on top
        // of an existing image
        clearpass->mClearBufferFlags = Ogre::FBT_COLOUR | Ogre::FBT_DEPTH |
            Ogre::FBT_STENCIL;

        // Set the same colour as in below
        clearpass->mColourValue = ColourValue(0.f, 0.f, 0.f, 0.f);
        // Other clear value defaults should be fine

        // Not sure if the samples want anything in their scenes so every group
        // will be rendered
        Ogre::CompositorPassSceneDef* scenepass =
            static_cast<Ogre::CompositorPassSceneDef*>(targetpasses->
            addPass(Ogre::PASS_SCENE));

        // Just render the overlay group since it is the only one used
        scenepass->mFirstRQ = Ogre::RENDER_QUEUE_BACKGROUND;
        scenepass->mLastRQ = Ogre::RENDER_QUEUE_MAX;

        // Connect the main render target to the node
        templatedworkspace->connectOutput("SampleCleaner", 0);

        // Create the workspace for rendering

        // This needs to be rendered first...
        manager->addWorkspace(sm, d_window, d_camera, "Sample_workspace",
            true, 0);

#else
        // Create a viewport covering whole window
        Viewport* vp = d_window->addViewport(d_camera);
        vp->setBackgroundColour(ColourValue(0.0f, 0.0f, 0.0f, 0.0f));
        // Update the camera aspect ratio to that of the viewport
        d_camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

#endif // CEGUI_USE_OGRE_COMPOSITOR2
        

        // create ogre renderer, image codec and resource provider.
        CEGUI::OgreRenderer& renderer = CEGUI::OgreRenderer::create(*d_window);
        d_renderer = &renderer;
        d_imageCodec = &renderer.createOgreImageCodec();
        d_resourceProvider = &renderer.createOgreResourceProvider();

        // create frame listener
        d_frameListener= new CEGuiDemoFrameListener(this, d_sampleApp, d_window, d_camera);
        d_ogreRoot->addFrameListener(d_frameListener);

#ifdef __ANDROID__
        AndroidAppHelper::initialiseOIS(d_frameListener, d_frameListener);
        d_windowEventListener = new WndEvtListener();
#else
        // add a listener for OS framework window events (for resizing)
        d_windowEventListener = new WndEvtListener(d_frameListener->getOISMouse());
#endif
        WindowEventUtilities::addWindowEventListener(d_window,
                                                     d_windowEventListener);

        
        d_ogreRoot->addFrameListener(this);
        renderer.setRenderingEnabled(false);

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
#ifdef OGRE_STATIC_LIB
    d_staticPluginLoader->unload();
    delete d_staticPluginLoader;
#endif
    delete d_ogreRoot;
    delete d_windowEventListener;
}

//----------------------------------------------------------------------------//
bool CEGuiOgreBaseApplication::init(SampleBrowserBase* sampleApp,
  const CEGUI::String &logFile, const CEGUI::String &dataPathPrefixOverride)
{
    if (!CEGuiBaseApplication::init(sampleApp, logFile, dataPathPrefixOverride))
        return false;

    // if base initialisation failed or app was cancelled by user, bail out now.
    if (!d_ogreRoot || !d_initialised)
        return false;

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::destroyRenderer()
{
    delete d_frameListener;

    CEGUI::OgreRenderer& renderer =
        *static_cast<CEGUI::OgreRenderer*>(d_renderer);
    renderer.destroyOgreResourceProvider(
        *static_cast<CEGUI::OgreResourceProvider*>(d_resourceProvider));
    renderer.destroyOgreImageCodec(
        *static_cast<CEGUI::OgreImageCodec*>(d_imageCodec));
    CEGUI::OgreRenderer::destroy(renderer);
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::destroyWindow()
{

}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::beginRendering(const float /*elapsed*/)
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
    rgm.createResourceGroup("animations");
    rgm.createResourceGroup("imagesets");
    rgm.createResourceGroup("fonts");
    rgm.createResourceGroup("layouts");
    rgm.createResourceGroup("schemes");
    rgm.createResourceGroup("looknfeels");
    rgm.createResourceGroup("lua_scripts");
    rgm.createResourceGroup("schemas");
    rgm.createResourceGroup("samples");

#ifdef __ANDROID__
    const char* type= "APKFileSystem";
#else
    // add CEGUI sample framework datafile dirs as resource locations
    ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");
    const char* type= "FileSystem";
#endif
        
    // for each resource type, set a resource group directory
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("schemes").c_str(), type, "schemes");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("samples").c_str(), type, "samples");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("imagesets").c_str(), type, "imagesets");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("fonts").c_str(), type, "fonts");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("layouts").c_str(), type, "layouts");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("looknfeel").c_str(), type, "looknfeels");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("lua_scripts").c_str(), type, "lua_scripts");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("animations").c_str(), type, "animations");
    ResourceGroupManager::getSingleton().addResourceLocation(getResourcePath("schemas").c_str(), type, "schemas");
#else
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("schemes").getString()).c_str(), type, "schemes");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("samples").getString()).c_str(), type, "samples");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("imagesets").getString()).c_str(), type, "imagesets");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("fonts").getString()).c_str(), type, "fonts");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("layouts").getString()).c_str(), type, "layouts");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("looknfeel").getString()).c_str(), type, "looknfeels");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("lua_scripts").getString()).c_str(), type, "lua_scripts");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("animations").getString()).c_str(), type, "animations");
    ResourceGroupManager::getSingleton().addResourceLocation(CEGUI::String::convertUtf32ToUtf8(getResourcePath("schemas").getString()).c_str(), type, "schemas");
#endif
}

CEGUI::String CEGuiOgreBaseApplication::getResourcePath(CEGUI::String resource) {
#ifdef __ANDROID__
    return "/datafiles/" + resource;
#else 
    return getDataPathPrefix() + "/" + resource + "/";
#endif
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::doFrameUpdate(const float /*elapsed*/)
{
}

//----------------------------------------------------------------------------//
bool CEGuiOgreBaseApplication::frameRenderingQueued(const Ogre::FrameEvent& args)
{
    renderSingleFrame(static_cast<float>(args.timeSinceLastFrame));

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiOgreBaseApplication::isInitialised()
{
    return d_initialised;
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::run()
{
    // start rendering via Ogre3D engine.
    CEGUI_TRY
    {
        #ifdef __ANDROID__
                AndroidAppHelper::go();
        #else
                d_ogreRoot->startRendering();
        #endif
    }
    CEGUI_CATCH(...)
    {}
}

//----------------------------------------------------------------------------//
void CEGuiOgreBaseApplication::setupDefaultConfigIfNeeded()
{
    // Check if the config exists
    bool success = d_ogreRoot->restoreConfig();

    if (!success)
    {
        // If not we set our default values for all renderers if possible
        const Ogre::RenderSystemList& renderSystems = d_ogreRoot->getAvailableRenderers();

        size_t renderSystemCount = renderSystems.size();
        for(size_t i = 0; i < renderSystemCount; ++i)
        {
            Ogre::RenderSystem* currentRenderSys = renderSystems.at(i);
            Ogre::ConfigOptionMap& configOptions = currentRenderSys->getConfigOptions();
            Ogre::ConfigOptionMap::iterator foundConfigIter;

            foundConfigIter = configOptions.find("Full Screen");
            if (foundConfigIter != configOptions.end())
                currentRenderSys->setConfigOption("Full Screen","No");

            foundConfigIter = configOptions.find("Video Mode");
            if (foundConfigIter != configOptions.end())
            {
                Ogre::StringVector::iterator optionsIterCur = foundConfigIter->second.possibleValues.begin();
                Ogre::StringVector::iterator optionsIterEnd = foundConfigIter->second.possibleValues.end();
                while(optionsIterCur != optionsIterEnd)
                {
                    if (optionsIterCur->compare("1280 x 768 @ 32-bit colour") == 0)
                    {
                        currentRenderSys->setConfigOption("Video Mode", *optionsIterCur);
                        break;
                    }
                    ++optionsIterCur;
                }
                if (optionsIterCur == optionsIterEnd)
                {
                    optionsIterCur = foundConfigIter->second.possibleValues.begin();
                    while(optionsIterCur != optionsIterEnd)
                    {
                        if (optionsIterCur->compare(0, 10, "1280 x 768") == 0)
                        {
                            currentRenderSys->setConfigOption("Video Mode", *optionsIterCur);
                            break;
                        }
                        ++optionsIterCur;
                    }
                }
            }
        }
    }
}

//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of CEGuiDemoFrameListener mehods
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
CEGuiDemoFrameListener::CEGuiDemoFrameListener(CEGuiOgreBaseApplication* baseApp, SampleBrowserBase*& sampleApp,
    Ogre::RenderWindow* window, Ogre::Camera* camera)
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

    // Prevent the window from capturing mouse making debugging impossible.
    #ifndef NDEBUG
        #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            paramList.insert(std::make_pair("x11_keyboard_grab", "false"));
            paramList.insert(std::make_pair("x11_mouse_grab", "false"));
            paramList.insert(std::make_pair("x11_mouse_hide", "false"));
            paramList.insert(std::make_pair("XAutoRepeatOn", "true"));
        #elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            paramList.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
            paramList.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
            paramList.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
            paramList.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
        #endif
    #endif

#ifndef __ANDROID__
    // create input system
    d_inputManager = OIS::InputManager::createInputSystem(paramList);

    // create buffered keyboard
#ifdef CEGUI_OLD_OIS_API
    if (d_inputManager->numKeyboards() > 0)
#else
    if (d_inputManager->getNumberOfDevices(OIS::OISKeyboard) > 0)
#endif
    {
#ifndef __ANDROID__
        d_keyboard = static_cast<OIS::Keyboard*>(d_inputManager->createInputObject(OIS::OISKeyboard, true));
        d_keyboard->setEventCallback(this);
#endif
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
#endif

    // store inputs we want to make use of
    d_camera = camera;
    d_window = window;
}

//----------------------------------------------------------------------------//
CEGuiDemoFrameListener::~CEGuiDemoFrameListener()
{
    if (d_inputManager)
    {
#ifndef __ANDROID__
        d_inputManager->destroyInputObject(d_mouse);
        d_inputManager->destroyInputObject(d_keyboard);
        OIS::InputManager::destroyInputSystem(d_inputManager);
#endif
    }
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    if (d_window->isClosed() || d_sampleApp->isQuitting())
        return false;

    static_cast<CEGuiOgreBaseApplication*>(d_baseApp)->
        doFrameUpdate(static_cast<float>(evt.timeSinceLastFrame));

    // update input system
#ifndef __ANDROID__
    if (d_mouse)
        d_mouse->capture();
    if (d_keyboard)
        d_keyboard->capture();
#endif

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::frameEnded(const Ogre::FrameEvent&)
{
    return true;
}

#ifdef __ANDROID__
//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::touchMoved(const OIS::MultiTouchEvent &e)
{   
    float x = static_cast <float>(e.state.X.abs);
    float y = static_cast <float>(e.state.Y.abs);
    d_sampleApp->injectMousePosition(x, y);
    return true;
}

#else
//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::mouseMoved(const OIS::MouseEvent &e)
{
    d_sampleApp->injectMousePosition(static_cast<float>(e.state.X.abs), static_cast<float>(e.state.Y.abs));
    d_sampleApp->injectMouseWheelChange(e.state.Z.rel / 120.0f);

    return true;
}
#endif

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::keyPressed(const OIS::KeyEvent &e)
{
    d_sampleApp->injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
    d_sampleApp->injectChar(e.text);

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::keyReleased(const OIS::KeyEvent& e)
{
    d_sampleApp->injectKeyUp(static_cast<CEGUI::Key::Scan>(e.key));

    return true;
}
#ifdef __ANDROID__
//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::touchPressed(const OIS::MultiTouchEvent &e)
{
    float x = static_cast <float>(e.state.X.abs);
    float y = static_cast <float>(e.state.Y.abs);
    d_sampleApp->injectMousePosition(x, y);
    d_sampleApp->injectMouseButtonDown(CEGUI::LeftButton);
    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::touchReleased(const OIS::MultiTouchEvent &e)
{
   d_sampleApp->injectMouseButtonUp(CEGUI::LeftButton);
    return true;
}

#else
//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::mousePressed
  (const OIS::MouseEvent&, OIS::MouseButtonID id)
{
    d_sampleApp->injectMouseButtonDown(convertOISButtonToCegui(id));
    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiDemoFrameListener::mouseReleased
  (const OIS::MouseEvent&, OIS::MouseButtonID id)
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
OIS::Mouse* CEGuiDemoFrameListener::getOISMouse()
{
    return d_mouse;
}
#endif
//----------------------------------------------------------------------------//

////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of WndEvtListener member functions
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
#ifdef __ANDROID__
WndEvtListener::WndEvtListener() 
#else
WndEvtListener::WndEvtListener(OIS::Mouse* mouse) :
    d_mouse(mouse)
#endif
{
}

//----------------------------------------------------------------------------//
void WndEvtListener::windowResized(Ogre::RenderWindow* rw)
{
    CEGUI::System* const sys = CEGUI::System::getSingletonPtr();
    if (sys)
        sys->notifyDisplaySizeChanged(
        CEGUI::Sizef(static_cast<float>(rw->getWidth()),
        static_cast<float>(rw->getHeight())));
#ifndef __ANDROID__
    const OIS::MouseState& mouseState = d_mouse->getMouseState();
    mouseState.width = rw->getWidth();
    mouseState.height = rw->getHeight();
#endif
}

//----------------------------------------------------------------------------//

#endif
