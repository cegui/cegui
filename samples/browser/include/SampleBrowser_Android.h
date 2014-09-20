/*
 -----------------------------------------------------------------------------
 This source file is part of OGRE
 (Object-oriented Graphics Rendering Engine)
 For the latest info, see http://www.ogre3d.org/

 Copyright (c) 2000-2014 Torus Knot Software Ltd

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 -----------------------------------------------------------------------------
 */

#ifndef __SampleBrowser_Android_H__
#define __SampleBrowser_Android_H__

#include "OIS.h"
#include <android_native_app_glue.h>
#include <android/log.h>
#include <EGL/egl.h>
#include "Ogre.h"
#include "OgrePlatform.h"
#include "ANDROID/OgreAndroidEGLWindow.h"

#include "OgreFileSystemLayer.h"
#include "Android/OgreAPKFileSystemArchive.h"
#include "Android/OgreAPKZipArchive.h"
#include "OgreWindowEventUtilities.h"
#include "OgreBuildSettings.h"

//#include "CEGuiOgreBaseApplication.h"

#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
#   error This header is for use with Android only
#endif

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Ogre", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Ogre", __VA_ARGS__))

namespace OgreBites {
class OgreAndroidBridge;

/*=============================================================================
 | Android input handling
 =============================================================================*/
class AndroidMultiTouch : public OIS::MultiTouch {
    public:
        AndroidMultiTouch(): OIS::MultiTouch ("Ogre", false, 0, 0) {}

        /** @copydoc Object::setBuffered */
        virtual void setBuffered (bool buffered) {}

        /** @copydoc Object::capture */
        virtual void capture() {}

        /** @copydoc Object::queryInterface */
        virtual OIS::Interface* queryInterface (OIS::Interface::IType type) {
            return 0;
        }

        /** @copydoc Object::_initialize */
        virtual void _initialize() {}

        OIS::MultiTouchState &getMultiTouchState (int i) {
            while (i >= mStates.size()) {
                Ogre::RenderWindow* pRenderWnd =
                    static_cast<Ogre::RenderWindow*> (Ogre::Root::getSingleton().getRenderTarget ("OgreWindow"));
                if (pRenderWnd) {
                    OIS::MultiTouchState state;
                    state.width = pRenderWnd->getWidth();
                    state.height = pRenderWnd->getHeight();
                    mStates.push_back (state);
                }
            }
            return mStates[i];
        }
};

class AndroidKeyboard : public OIS::Keyboard {
    public:
        AndroidKeyboard(): OIS::Keyboard ("Ogre", false, 1, 0) {}

        /** @copydoc Object::setBuffered */
        virtual void setBuffered (bool buffered) {}

        /** @copydoc Object::capture */
        virtual void capture() {}

        /** @copydoc Object::queryInterface */
        virtual OIS::Interface* queryInterface (OIS::Interface::IType type) {
            return 0;
        }

        /** @copydoc Object::_initialize */
        virtual void _initialize() {}

        virtual bool isKeyDown (OIS::KeyCode key) const {
            return false;
        }

        virtual const std::string& getAsString (OIS::KeyCode kc) {
            static std::string defstr = "";
            return defstr;
        }

        virtual void copyKeyStates (char keys[256]) const {

        }
};

/*=============================================================================
 | Android input injection
 =============================================================================*/
class AndroidInputInjector {
    private:
        OIS::KeyListener* keyListener;
        OIS::MultiTouchListener* touchListener;
        AndroidMultiTouch* mTouch;
        AndroidKeyboard* mKeyboard;

    public:

        AndroidInputInjector (OIS::KeyListener *keyListener, OIS::MultiTouchListener *touchListener,
                              AndroidMultiTouch* touch, AndroidKeyboard* keyboard)
            : keyListener (keyListener), touchListener (touchListener), mTouch (touch), mKeyboard (keyboard) {}

        void injectKeyEvent (int action, int32_t keyCode) {
            if (keyCode == AKEYCODE_BACK) {
                OIS::KeyEvent evt (mKeyboard, OIS::KC_ESCAPE, 0);
                if (action == 0) {
                    keyListener->keyPressed (evt);
                } else {
                    keyListener->keyReleased (evt);
                }
            }
        }

        void injectTouchEvent (int action, float x, float y, int pointerId = 0) {
            OIS::MultiTouchState &state = mTouch->getMultiTouchState (pointerId);

            switch (action) {
                case 0:
                    state.touchType = OIS::MT_Pressed;
                    break;
                case 1:
                    state.touchType = OIS::MT_Released;
                    break;
                case 2:
                    state.touchType = OIS::MT_Moved;
                    break;
                case 3:
                    state.touchType = OIS::MT_Cancelled;
                    break;
                default:
                    state.touchType = OIS::MT_None;
            }

            if (state.touchType != OIS::MT_None) {
                int last = state.X.abs;
                state.X.abs = (int)x;
                state.X.rel = state.X.abs - last;

                last = state.Y.abs;
                state.Y.abs = (int)y;
                state.Y.rel = state.Y.abs - last;

                state.Z.abs = 0;
                state.Z.rel = 0;

                OIS::MultiTouchEvent evt (mTouch, state);

                switch (state.touchType) {
                    case OIS::MT_Pressed:
                        touchListener->touchPressed (evt);
                        break;
                    case OIS::MT_Released:
                        touchListener->touchReleased (evt);
                        break;
                    case OIS::MT_Moved:
                        touchListener->touchMoved (evt);
                        break;
                    case OIS::MT_Cancelled:
                        touchListener->touchCancelled (evt);
                        break;
                    default:
                        break;
                }
            }
        }
};

/*=============================================================================
 | Ogre Android bridge
 =============================================================================*/
class OgreAndroidBridge {
    public:
        static void init (struct android_app* state) {
            d_state = state;
            d_state->onAppCmd = &OgreAndroidBridge::handleCmd;
            d_state->onInputEvent = &OgreAndroidBridge::handleInput;
        }

        static void shutdown() {
            OGRE_DELETE_T (mFSLayer, FileSystemLayer, Ogre::MEMCATEGORY_GENERAL);
            mRoot = NULL;
            mRenderWnd = NULL;
            mKeyListener = NULL;
            mMultiTouchListener = NULL;

            delete mTouch;
            mTouch = NULL;

            delete mKeyboard;
            mKeyboard = NULL;

            delete mInputInjector;
            mInputInjector = NULL;
        }

        static int32_t handleInput (struct android_app* app, AInputEvent* event) {
            if (mInputInjector) {
                if (AInputEvent_getType (event) == AINPUT_EVENT_TYPE_MOTION) {
                    int action = (int) (AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction (event));

                    if (action == 0) {
                        mInputInjector->injectTouchEvent (2, AMotionEvent_getRawX (event, 0),
                                                          AMotionEvent_getRawY (event, 0));
                    }
                    mInputInjector->injectTouchEvent (action, AMotionEvent_getRawX (event, 0),
                                                      AMotionEvent_getRawY (event, 0));
                } else {
                    mInputInjector->injectKeyEvent (AKeyEvent_getAction (event), AKeyEvent_getKeyCode (event));
                }

                return 1;
            }
            return 0;
        }

        static void handleCmd (struct android_app* app, int32_t cmd) {
            switch (cmd) {
                case APP_CMD_SAVE_STATE:
                    break;
                case APP_CMD_INIT_WINDOW:
                    __android_log_write (ANDROID_LOG_ERROR, "CEGUIDEMO", "-------------- APP_CMD_INIT_WINDOW");
                    if (app->window && mRoot) {
                        AConfiguration* config = AConfiguration_new();
                        AConfiguration_fromAssetManager (config, app->activity->assetManager);

                        if (!mRenderWnd) {
                            __android_log_write (ANDROID_LOG_ERROR, "CEGUIDEMO", "------- create render window");
                            Ogre::NameValuePairList opt;
                            opt["externalWindowHandle"] = Ogre::StringConverter::toString ((int)app->window);
                            opt["androidConfig"] = Ogre::StringConverter::toString ((int)config);
                            mRenderWnd = Ogre::Root::getSingleton().createRenderWindow (
                                             "OgreWindow", 0, 0, false, &opt);
                            androidWindowInitialized = true;
                            __android_log_write (ANDROID_LOG_ERROR, "CEGUIDEMO", "------- render window created");
                        } else {
                            __android_log_write (ANDROID_LOG_ERROR, "CEGUIDEMO", "------- create internal res");
                            static_cast<Ogre::AndroidEGLWindow*> (mRenderWnd)->_createInternalResources (
                                app->window, config);
                            __android_log_write (ANDROID_LOG_ERROR, "CEGUIDEMO", "------- internal res created");
                        }
                        AConfiguration_delete (config);
                    }
                    break;
                case APP_CMD_TERM_WINDOW:
                    if (mRoot && mRenderWnd)
                        static_cast<Ogre::AndroidEGLWindow*> (mRenderWnd)->_destroyInternalResources();
                    break;
                case APP_CMD_GAINED_FOCUS:
                    break;
                case APP_CMD_LOST_FOCUS:
                    break;
                case APP_CMD_CONFIG_CHANGED:
                    break;
            }
        }

        static Ogre::RenderWindow* createWindow (Ogre::Root* root) {
            mRoot = root;
            int ident, events;
            struct android_poll_source* source;
            // must wait for android to initialize the window to continue loading Ogre resources
            while (!androidWindowInitialized) {
                //This will process until the android window is created.  The Ogre Render Window is then created.
                while ((ident = ALooper_pollAll (0, NULL, &events, (void**)&source)) >= 0) {
                    if (source)
                        source->process (d_state, source);
                    if (d_state->destroyRequested != 0) {
                        return NULL;
                    }
                }
            }
            return mRenderWnd;
        }

        static Ogre::DataStreamPtr openAPKFile (const Ogre::String& fileName) {
            Ogre::DataStreamPtr stream;
            AAssetManager* mAssetMgr = d_state->activity->assetManager;
            AAsset* asset = AAssetManager_open (mAssetMgr, fileName.c_str(), AASSET_MODE_BUFFER);
            if (asset) {
                off_t length = AAsset_getLength (asset);
                void* membuf = OGRE_MALLOC (length, Ogre::MEMCATEGORY_GENERAL);
                memcpy (membuf, AAsset_getBuffer (asset), length);
                AAsset_close (asset);
                stream = Ogre::DataStreamPtr (new Ogre::MemoryDataStream (membuf, length, true, true));
            }
            return stream;
        }

        static void initialiseFS() {
            mFSLayer = OGRE_NEW_T (Ogre::FileSystemLayer, Ogre::MEMCATEGORY_GENERAL) (OGRE_VERSION_NAME);
            AAssetManager* assetManager =  d_state->activity->assetManager;
            Ogre::APKFileSystemArchiveFactory* apk = new Ogre::APKFileSystemArchiveFactory (assetManager);
            Ogre::APKZipArchiveFactory* zip = new Ogre::APKZipArchiveFactory (assetManager);
            Ogre::ArchiveManager::getSingleton().addArchiveFactory (apk);
            Ogre::ArchiveManager::getSingleton().addArchiveFactory (zip);
            Ogre::String layerFullName = mFSLayer->getConfigFilePath ("resources.cfg");
            Ogre::DataStreamPtr ptr = openAPKFile (layerFullName);
            Ogre::ConfigFile cf;
            cf.load (ptr);
            Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
            Ogre::String sec, type, arch;
            // go through all specified resource groups
            while (seci.hasMoreElements()) {
                sec = seci.peekNextKey();
                Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
                Ogre::ConfigFile::SettingsMultiMap::iterator i;
                // go through all resource paths
                for (i = settings->begin(); i != settings->end(); i++) {
                    type = i->first;
                    arch = i->second;
                    Ogre::ResourceGroupManager::getSingleton().addResourceLocation (arch, type, sec);
                }
            }
        }

        static bool initialiseOIS (OIS::KeyListener* keyListener, OIS::MultiTouchListener* mMultiTouchListener) {
            if (!mTouch)
                mTouch = new AndroidMultiTouch();
            if (!mKeyboard)
                mKeyboard = new AndroidKeyboard();
            if (!mInputInjector)
                mInputInjector = new AndroidInputInjector (mKeyListener, mMultiTouchListener, mTouch, mKeyboard);
        }
        static void go() {
            int ident, events;
            struct android_poll_source* source;
            unsigned int width, height, colorDepth;
            while (true) {
                while ((ident = ALooper_pollAll (0, NULL, &events, (void**)&source)) >= 0) {
                    if (source != NULL)
                        source->process (d_state, source);
                    if (d_state->destroyRequested != 0)
                        return;
                }
                Ogre::WindowEventUtilities::messagePump();
                if (mRenderWnd != NULL && mRenderWnd->isActive()) {
                    //CEGUI::System* const sys = CEGUI::System::getSingletonPtr();
                    //if (sys)
                     //   sys->notifyDisplaySizeChanged(CEGUI::Sizef(static_cast<float>(mRenderWnd->getWidth()),
                     //       static_cast<float>(mRenderWnd->getHeight())));
                    mRenderWnd->windowMovedOrResized();
                    mRoot->renderOneFrame();
                }
            }
        }

    private:
        static Ogre::FileSystemLayer* mFSLayer; // File system abstraction layer
        static AndroidInputInjector* mInputInjector;
        static OIS::KeyListener* mKeyListener;
        static OIS::MultiTouchListener* mMultiTouchListener;
        static AndroidKeyboard* mKeyboard;
        static AndroidMultiTouch* mTouch;
        static Ogre::RenderWindow* mRenderWnd;
        static Ogre::Root* mRoot;
        static bool androidWindowInitialized;
        static struct android_app* d_state;
};
}

#endif

