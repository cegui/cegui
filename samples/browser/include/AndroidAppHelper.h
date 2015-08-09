/***********************************************************************
    created:    28/9/2004
    author:     David Reepmeyer
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
#ifndef _AndroidAppHelper_h_
#define _AndroidAppHelper_h_

#ifndef __ANDROID__
error:
only android builds should include this file
#endif

#include "CEGUI/AndroidUtils.h"
#include <android_native_app_glue.h>

#ifdef CEGUI_SAMPLES_ANDROID_GLES2
class CEGuiEGLBaseApplication;
#   include "SampleBrowserBase.h"
#elif defined(CEGUI_SAMPLES_ANDROID_OGRE)
#   include <EGL/egl.h>
#   include "OIS.h" 
#   include "Ogre.h"
#   include "OgrePlatform.h"
#   include "Android/OgreAndroidEGLWindow.h"
#   include "OgreFileSystemLayer.h"
#   include "Android/OgreAPKFileSystemArchive.h"
#   include "Android/OgreAPKZipArchive.h"
#   include "OgreWindowEventUtilities.h"
#   include "OgreBuildSettings.h"
#endif

#ifdef CEGUI_SAMPLES_ANDROID_OGRE
class AndroidMultiTouch : public OIS::MultiTouch
{
public:
    AndroidMultiTouch();
    virtual void setBuffered (bool buffered);
    virtual void capture();
    virtual OIS::Interface* queryInterface (OIS::Interface::IType type);
    virtual void _initialize();
    OIS::MultiTouchState &getMultiTouchState (int i);
};

class AndroidKeyboard : public OIS::Keyboard
{
public:
    AndroidKeyboard();
    virtual void setBuffered (bool buffered);
    virtual void capture();
    virtual OIS::Interface* queryInterface (OIS::Interface::IType type);
    virtual void _initialize();
    virtual bool isKeyDown (OIS::KeyCode key) const;
    virtual const std::string& getAsString (OIS::KeyCode kc);
    virtual void copyKeyStates (char keys[256]) const;
};

class AndroidInputInjector
{
private:
    OIS::KeyListener* keyListener;
    OIS::MultiTouchListener* touchListener;
    AndroidMultiTouch* mTouch;
    AndroidKeyboard* mKeyboard;

public:

    AndroidInputInjector (OIS::KeyListener *keyListener, OIS::MultiTouchListener *touchListener,
                          AndroidMultiTouch* touch, AndroidKeyboard* keyboard);

    void injectKeyEvent (int action, int32_t keyCode);
    void injectTouchEvent (int action, float x, float y, int pointerId = 0);
};

#endif //CEGUI_SAMPLES_ANDROID_OGRE


/*!
\brief
    All static utility class containing helper / common functions used for the Win32 apps
*/
class AndroidAppHelper
{
public:
    static bool createWindow();
    static bool processEvents();
    static int32_t handleInput(struct android_app* app, AInputEvent* event);
    static void handleCmd(struct android_app* app, int32_t cmd);

    //Ogre specific
#ifdef CEGUI_SAMPLES_ANDROID_OGRE
    static void shutdown();
    static void go();
    static Ogre::RenderWindow* getRenderWindow ();
    static void initialiseFS();
    static bool initialiseOIS (OIS::KeyListener* keyListener, OIS::MultiTouchListener* mMultiTouchListener);
#endif


private:
    static bool d_windowCreated;
    static bool isWindowCreated();
    static void setWindowCreated(bool windowCreated);
    
    //OgreSpecific
#ifdef CEGUI_SAMPLES_ANDROID_OGRE
    static Ogre::FileSystemLayer* mFSLayer; // File system abstraction layer
    static AndroidInputInjector* mInputInjector;
    static OIS::KeyListener* mKeyListener;
    static OIS::MultiTouchListener* mMultiTouchListener;
    static AndroidKeyboard* mKeyboard;
    static AndroidMultiTouch* mTouch;
    static Ogre::RenderWindow* mRenderWnd;
#endif

};

#endif  // end of guard _AndroidAppHelper_h_

