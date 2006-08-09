/***********************************************************************
    filename:   CEGuiOgreBaseApplication.cpp
    created:    9/3/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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

// this controls conditional compile of file for Apple
#include "CEGUISamplesConfig.h"
#ifdef CEGUI_SAMPLES_USE_OGRE

#include "CEGuiOgreBaseApplication.h"
#include "CEGuiSample.h"

#include <OgreKeyEvent.h>


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

        // Create and initialise the camera
        d_camera = d_ogreRoot->getSceneManagerIterator().getNext()->createCamera("PlayerCam");
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
        d_renderer = new CEGUI::OgreCEGUIRenderer(d_window);
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

    // add CEGUI sample framework datafile dirs as resource locations
    ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem");
#ifndef __APPLE__
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/fonts", "FileSystem", "fonts");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/imagesets", "FileSystem", "imagesets");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/layouts", "FileSystem", "layouts");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/looknfeel", "FileSystem", "looknfeels");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/schemes", "FileSystem", "schemes");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/configs", "FileSystem");
    ResourceGroupManager::getSingleton().addResourceLocation("../datafiles/lua_scripts", "FileSystem");
#else
    // Because Ogre/Mac looks in the bundle's Resources folder by default...
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/fonts", "FileSystem", "fonts");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/imagesets", "FileSystem", "imagesets");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/layouts", "FileSystem", "layouts");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/looknfeel", "FileSystem", "looknfeels");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/schemes", "FileSystem", "schemes");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/configs", "FileSystem");
    ResourceGroupManager::getSingleton().addResourceLocation("datafiles/lua_scripts", "FileSystem");
#endif
}


////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of CEGuiDemoFrameListener mehods
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
CEGuiDemoFrameListener::CEGuiDemoFrameListener(CEGuiBaseApplication* baseApp, Ogre::RenderWindow* window, Ogre::Camera* camera, bool useBufferedInputKeys, bool useBufferedInputMouse)
{
    // create and initialise events processor
    d_eventProcessor = new Ogre::EventProcessor();
    d_eventProcessor->initialise(window);
    d_eventProcessor->addKeyListener(this);
    d_eventProcessor->addMouseMotionListener(this);
    d_eventProcessor->addMouseListener(this);
    d_eventProcessor->startProcessingEvents();

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
    delete d_eventProcessor;
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
        return true;
    }
}

bool CEGuiDemoFrameListener::frameEnded(const Ogre::FrameEvent& evt)
{
    return true;
}

void CEGuiDemoFrameListener::mouseMoved(Ogre::MouseEvent *e)
{
    CEGUI::Renderer* rend = CEGUI::System::getSingleton().getRenderer();
    CEGUI::System::getSingleton().injectMouseMove(e->getRelX() * rend->getWidth(), e->getRelY() * rend->getHeight());

    float wheel = e->getRelZ();

    if (wheel != 0)
    {
        CEGUI::System::getSingleton().injectMouseWheelChange(wheel * 10);
    }

    e->consume();
}


void CEGuiDemoFrameListener::mouseDragged(Ogre::MouseEvent *e)
{
    mouseMoved(e);
}


void CEGuiDemoFrameListener::keyPressed(Ogre::KeyEvent *e)
{
    // give 'quitting' priority
    if (e->getKey() == Ogre::KC_ESCAPE)
    {
        d_quit = true;
        e->consume();
        return;
    }

    // do event injection
    CEGUI::System& cegui = CEGUI::System::getSingleton();

    // key down
    cegui.injectKeyDown(e->getKey());

    // now character
    cegui.injectChar(e->getKeyChar());

    e->consume();
}


void CEGuiDemoFrameListener::keyReleased(Ogre::KeyEvent *e)
{
    CEGUI::System::getSingleton().injectKeyUp(e->getKey());
}



void CEGuiDemoFrameListener::mousePressed(Ogre::MouseEvent *e)
{
    CEGUI::System::getSingleton().injectMouseButtonDown(convertOgreButtonToCegui(e->getButtonID()));
    e->consume();
}


void CEGuiDemoFrameListener::mouseReleased(Ogre::MouseEvent *e)
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertOgreButtonToCegui(e->getButtonID()));
    e->consume();
}

void CEGuiDemoFrameListener::keyClicked(Ogre::KeyEvent *e)
{}

void CEGuiDemoFrameListener::mouseClicked(Ogre::MouseEvent *e)
{}

void CEGuiDemoFrameListener::mouseEntered(Ogre::MouseEvent *e)
{}

void CEGuiDemoFrameListener::mouseExited(Ogre::MouseEvent *e)
{}

CEGUI::MouseButton CEGuiDemoFrameListener::convertOgreButtonToCegui(int ogre_button_id)
{
    switch (ogre_button_id)
    {
    case Ogre::MouseEvent::BUTTON0_MASK:
        return CEGUI::LeftButton;
        break;

    case Ogre::MouseEvent::BUTTON1_MASK:
        return CEGUI::RightButton;
        break;

    case Ogre::MouseEvent::BUTTON2_MASK:
        return CEGUI::MiddleButton;
        break;

    case Ogre::MouseEvent::BUTTON3_MASK:
        return CEGUI::X1Button;
        break;

    default:
        return CEGUI::LeftButton;
        break;
    }

}

#endif

