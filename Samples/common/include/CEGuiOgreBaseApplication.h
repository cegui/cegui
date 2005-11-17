/************************************************************************
    filename:   CEGuiOgreBaseApplication.h
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
#ifndef _CEGuiOgreBaseApplication_h_
#define _CEGuiOgreBaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "CEGUI.h"

#include <OgreCEGUIRenderer.h>
#include <Ogre.h>
#include <OgreEventListeners.h>

// Frame listener forward ref (see class below)
class CEGuiDemoFrameListener;


class CEGuiOgreBaseApplication : public CEGuiBaseApplication
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiOgreBaseApplication();


    /*!
    \brief
        Destructor.
    */
    virtual ~CEGuiOgreBaseApplication();


    // Implementation of base class abstract methods.
    bool execute(CEGuiSample* sampleApp);
    void cleanup();

protected:
    void initialiseResources(void);

    /*************************************************************************
        Data Fields
    *************************************************************************/
    Ogre::Root* d_ogreRoot;
    Ogre::Camera* d_camera;
    Ogre::RenderWindow* d_window;
    CEGUI::OgreCEGUIRenderer* d_renderer;
    bool d_initialised;

    CEGuiDemoFrameListener* d_frameListener;
};


/*!
\brief
    Ogre FrameListener class where we deal with input processing and the like.
*/
class CEGuiDemoFrameListener: public Ogre::FrameListener, public Ogre::KeyListener, Ogre::MouseMotionListener, Ogre::MouseListener
{
public:
    // Construction and Destruction
    CEGuiDemoFrameListener(CEGuiBaseApplication* baseApp, Ogre::RenderWindow* win, Ogre::Camera* cam, bool useBufferedInputKeys = false, bool useBufferedInputMouse = false);
    ~CEGuiDemoFrameListener();

    // Processing to be done at start and end of each frame.
    bool frameStarted(const Ogre::FrameEvent& evt);
    bool frameEnded(const Ogre::FrameEvent& evt);

    // Raw input handlers that we care about
    void mouseMoved(Ogre::MouseEvent *e);
    void mouseDragged(Ogre::MouseEvent *e);
    void keyPressed(Ogre::KeyEvent *e);
    void keyReleased(Ogre::KeyEvent *e);
    void mousePressed(Ogre::MouseEvent *e);
    void mouseReleased(Ogre::MouseEvent *e);

    // Raw input handlers that we do not care about
    void keyClicked(Ogre::KeyEvent *e);
    void mouseClicked(Ogre::MouseEvent *e);
    void mouseEntered(Ogre::MouseEvent *e);
    void mouseExited(Ogre::MouseEvent *e);

protected:
    // convert an Ogre mouse button into a CEGUI mouse button
    CEGUI::MouseButton convertOgreButtonToCegui(int ogre_button_id);

    /*************************************************************************
        Data Fields
    *************************************************************************/
    Ogre::Overlay* d_statsOverlay;
    Ogre::EventProcessor* d_eventProcessor;

    Ogre::Camera* d_camera;
    Ogre::RenderWindow* d_window;
    bool d_quit;
    CEGuiBaseApplication* d_baseApp;
};

#endif  // end of guard _CEGuiOgreBaseApplication_h_
