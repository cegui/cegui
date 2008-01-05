/***********************************************************************
    filename:   CEGuiOgreBaseApplication.cpp
    created:    9/3/2004
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
#ifdef CEGUI_SAMPLES_USE_OGRE

#include "CEGuiOgreBaseApplication.h"
#include "CEGuiSample.h"

CEGuiOgreBaseApplication::CEGuiOgreBaseApplication() :
        d_ogreRoot(0),
        d_renderer(0),
        d_initialised(false),
        d_frameListener(0)
{
    using namespace Ogre;

    d_ogreRoot = new Root();

    initialiseResources();

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
        vp->setBackgroundColour(ColourValue(0,0,0));

        // Update the camera aspect ratio to that of the viewport
        d_camera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

        // initialise resources
        ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        // initialise GUI system
        d_renderer = new CEGUI::OgreCEGUIRenderer(d_window, RENDER_QUEUE_OVERLAY, false, 0, sm);
        new CEGUI::System(d_renderer);

        // create frame listener
        d_frameListener= new CEGuiDemoFrameListener(this, d_window, d_camera);
        d_ogreRoot->addFrameListener(d_frameListener);

        d_initialised = true;
    }
    else
    {
        // aborted.  Clean up and set root to 0 so when app attempts to run it knows what happened here.
        delete d_ogreRoot;
        d_ogreRoot = 0;
    }
}

CEGuiOgreBaseApplication::~CEGuiOgreBaseApplication()
{
    delete d_frameListener;
    delete CEGUI::System::getSingletonPtr();
    delete d_renderer;
    delete d_ogreRoot;
}

bool CEGuiOgreBaseApplication::execute(CEGuiSample* sampleApp)
{
    // if initialisation failed or was cancelled by user, bail out now.
    if (d_ogreRoot && d_initialised)
    {
        // perform sample initialisation
        sampleApp->initialiseSample();

        // start rendering via Ogre3D engine.
        try
        {
            d_ogreRoot->startRendering();
        }
        catch(Ogre::Exception&)
        {
            return false;
        }
        catch(CEGUI::Exception&)
        {
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}

void CEGuiOgreBaseApplication::cleanup()
{
    // nothing to do here.
}

void CEGuiOgreBaseApplication::initialiseResources(void)
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

    // add CEGUI sample framework datafile dirs as resource locations
    ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");

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
    #if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
        sprintf(resourcePath, "%s/%s", dataPathPrefix, "XMLRefSchema/");
        ResourceGroupManager::getSingleton().addResourceLocation(resourcePath, "FileSystem", "schemas");
    #endif
#elif !defined(__APPLE__)
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/fonts", "FileSystem", "fonts");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/imagesets", "FileSystem", "imagesets");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/layouts", "FileSystem", "layouts");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/looknfeel", "FileSystem", "looknfeels");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/schemes", "FileSystem", "schemes");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/configs", "FileSystem");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/lua_scripts", "FileSystem", "lua_scripts");
    #if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
        ResourceGroupManager::getSingleton().addResourceLocation("../../XMLRefSchemma", "FileSystem", "schemas");
    #endif
#else
    // Because Ogre/Mac looks in the bundle's Resources folder by default...
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/fonts", "FileSystem", "fonts");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/imagesets", "FileSystem", "imagesets");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/layouts", "FileSystem", "layouts");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/looknfeel", "FileSystem", "looknfeels");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/schemes", "FileSystem", "schemes");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/configs", "FileSystem");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/lua_scripts", "FileSystem", "lua_scripts");
    #if defined(CEGUI_WITH_XERCES) && (CEGUI_DEFAULT_XMLPARSER == XercesParser)
        // FIXME: I think this is wrong, but still it's how Exsortis had in in the OGL renderer.  5/1/08 PDT
        ResourceGroupManager::getSingleton().addResourceLocation("XMLRefSchemma", "FileSystem", "schemas");
    #endif
#endif
}


////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of CEGuiDemoFrameListener mehods
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
CEGuiDemoFrameListener::CEGuiDemoFrameListener(CEGuiBaseApplication* baseApp, Ogre::RenderWindow* window, Ogre::Camera* camera, bool useBufferedInputKeys, bool useBufferedInputMouse)
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

    // create input system
    d_inputManager = OIS::InputManager::createInputSystem(paramList);

    // create buffered keyboard
    if (d_inputManager->numKeyBoards() > 0)
    {
        d_keyboard = static_cast<OIS::Keyboard*>(d_inputManager->createInputObject(OIS::OISKeyboard, true));
        d_keyboard->setEventCallback(this);
    }

    // create buffered mouse
    if (d_inputManager->numMice() > 0)
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

    // we've not quit yet.
    d_quit = false;

    // setup base app ptr
    d_baseApp = baseApp;
}

CEGuiDemoFrameListener::~CEGuiDemoFrameListener()
{
    if (d_inputManager)
    {
        d_inputManager->destroyInputObject(d_mouse);
        d_inputManager->destroyInputObject(d_keyboard);
        OIS::InputManager::destroyInputSystem(d_inputManager);
    }

}

bool CEGuiDemoFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    if(d_window->isClosed() || d_quit || d_baseApp->isQuitting())
    {
        return false;
    }
    else
    {
        // always inject a time pulse to enable widget automation
        CEGUI::System::getSingleton().injectTimePulse(static_cast<float>(evt.timeSinceLastFrame));

        // update input system
        if (d_mouse)
            d_mouse->capture();
        if (d_keyboard)
            d_keyboard->capture();

        return true;
    }
    
}

bool CEGuiDemoFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

bool CEGuiDemoFrameListener::mouseMoved(const OIS::MouseEvent &e)
{

    CEGUI::System& cegui = CEGUI::System::getSingleton();

    cegui.injectMouseMove(e.state.X.rel, e.state.Y.rel);
    cegui.injectMouseWheelChange(e.state.Z.rel * 0.03);

    return true;
}



bool CEGuiDemoFrameListener::keyPressed(const OIS::KeyEvent &e)
{
    // give 'quitting' priority
    if (e.key == OIS::KC_ESCAPE)
    {
        d_quit = true;
         return true;
    }

    // do event injection
    CEGUI::System& cegui = CEGUI::System::getSingleton();

    // key down
    cegui.injectKeyDown(e.key);

    // now character
    cegui.injectChar(e.text);


    return true;
}


bool CEGuiDemoFrameListener::keyReleased(const OIS::KeyEvent &e)
{
    CEGUI::System::getSingleton().injectKeyUp(e.key);
    return true;
}



bool CEGuiDemoFrameListener::mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonDown(convertOISButtonToCegui(id));

    return true;
}


bool CEGuiDemoFrameListener::mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertOISButtonToCegui(id));

    return true;
}

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

#endif



