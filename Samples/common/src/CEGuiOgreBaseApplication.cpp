/************************************************************************
    filename:   CEGuiOgreBaseApplication.cpp
    created:    9/3/2004
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
// this controls conditional compile of file for MSVC
#include "CEGUIConfig.h"
#ifdef CEGUI_SAMPLES_USE_OGRE

#include "CEGuiOgreBaseApplication.h"
#include "CEGuiSample.h"

#include <OgreKeyEvent.h>


CEGuiOgreBaseApplication::CEGuiOgreBaseApplication() :
        d_ogreRoot(0),
        d_renderer(0),
        d_guiSys(0),
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
        d_camera = d_ogreRoot->getSceneManager(ST_GENERIC)->createCamera("PlayerCam");
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
        d_guiSys   = new CEGUI::System(d_renderer);

        // create frame listener
        d_frameListener= new CEGuiDemoFrameListener(d_window, d_camera);
        d_ogreRoot->addFrameListener(d_frameListener);
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
    if (d_frameListener)
        delete d_frameListener;

    if (d_guiSys)
        delete d_guiSys;

    if (d_renderer)
        delete d_renderer;

    if (d_ogreRoot)
        delete d_ogreRoot;
}

bool CEGuiOgreBaseApplication::execute(CEGuiSample* sampleApp)
{
    // if initialisation failed or was cancelled by user, bail out now.
    if (d_ogreRoot && d_guiSys)
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

    // Load resource paths from config file
    ConfigFile cf;
    cf.load("resources.cfg");

    // Go through all sections & settings in the file
    ConfigFile::SectionIterator seci = cf.getSectionIterator();

    String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        ConfigFile::SettingsMultiMap *settings = seci.getNext();
        ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
    Start of CEGuiDemoFrameListener mehods
*******************************************************************************/
////////////////////////////////////////////////////////////////////////////////
CEGuiDemoFrameListener::CEGuiDemoFrameListener(Ogre::RenderWindow* window, Ogre::Camera* camera, bool useBufferedInputKeys, bool useBufferedInputMouse)
{
    // load and show the default Ogre debug stats overlay
    d_statsOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");
    d_statsOverlay->show();

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
}

CEGuiDemoFrameListener::~CEGuiDemoFrameListener()
{
    delete d_eventProcessor;
}

bool CEGuiDemoFrameListener::frameStarted(const Ogre::FrameEvent& evt)
{
    if(d_window->isClosed() || d_quit)
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
    updateStats();
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

void CEGuiDemoFrameListener::updateStats(void)
{
    using namespace Ogre;

    static String currFps = "Current FPS: ";
    static String avgFps = "Average FPS: ";
    static String bestFps = "Best FPS: ";
    static String worstFps = "Worst FPS: ";
    static String tris = "Triangle Count: ";

    // update stats when necessary
    try
    {
        OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

        const RenderTarget::FrameStats& stats = d_window->getStatistics();

        guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
        guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
        guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
                            +" "+StringConverter::toString(stats.bestFrameTime)+" ms");
        guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
                             +" "+StringConverter::toString(stats.worstFrameTime)+" ms");

        OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

        OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        guiDbg->setCaption(d_window->getDebugText());
    }
    catch(...)
    {
        // ignore
    }
}

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

