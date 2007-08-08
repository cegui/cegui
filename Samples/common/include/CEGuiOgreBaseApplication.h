/***********************************************************************
    filename:   CEGuiOgreBaseApplication.h
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
#ifndef _CEGuiOgreBaseApplication_h_
#define _CEGuiOgreBaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "CEGUI.h"

#include <OgreCEGUIRenderer.h>
#include <Ogre.h>
#include <OIS.h>

#if defined(_WIN32)
#  if defined(_DEBUG)
#      pragma comment(lib, "OgreGUIRenderer_d.lib")
#      pragma comment(lib, "OgreMain_d.lib")
#  else
#      pragma comment(lib, "OgreGUIRenderer.lib")
#      pragma comment(lib, "OgreMain.lib")
#  endif
#endif

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
class CEGuiDemoFrameListener : public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
    // Construction and Destruction
    CEGuiDemoFrameListener(CEGuiBaseApplication* baseApp, Ogre::RenderWindow* win, Ogre::Camera* cam, bool useBufferedInputKeys = false, bool useBufferedInputMouse = false);
    ~CEGuiDemoFrameListener();

    // Processing to be done at start and end of each frame.
    bool frameStarted(const Ogre::FrameEvent& evt);
    bool frameEnded(const Ogre::FrameEvent& evt);

    // Raw input handlers that we care about
    bool mouseMoved(const OIS::MouseEvent &e);
    bool keyPressed(const OIS::KeyEvent &e);
    bool keyReleased(const OIS::KeyEvent &e);
    bool mousePressed(const OIS::MouseEvent &e, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &e, OIS::MouseButtonID id);

protected:
    // convert an OIS mouse button into a CEGUI mouse button
    CEGUI::MouseButton convertOISButtonToCegui(int buttonID);

    /*************************************************************************
        Data Fields
    *************************************************************************/
    Ogre::Overlay* d_statsOverlay;
    OIS::InputManager* d_inputManager;
    OIS::Keyboard* d_keyboard;
    OIS::Mouse* d_mouse;
    Ogre::Camera* d_camera;
    Ogre::RenderWindow* d_window;
    bool d_quit;
    CEGuiBaseApplication* d_baseApp;
};

#endif  // end of guard _CEGuiOgreBaseApplication_h_
