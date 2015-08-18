/***********************************************************************
    created:    Sat Nov 05 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGLES/Renderer.h"
#include "CEGUI/CEGUI.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <GLES/egl.h>
#include <GLES/gl.h>
// Include last to avoid the total fail that is the definition of things
// like None, True and False as macros from affecting the other headers.
#include <X11/Xlib.h>
#include <X11/keysym.h>

//----------------------------------------------------------------------------//
// setup default-default path
#ifndef CEGUI_SAMPLE_DATAPATH
    #define CEGUI_SAMPLE_DATAPATH "../datafiles"
#endif

// environment variable that overrides the location of the datafiles
#define DATAPATH_VAR_NAME "CEGUI_SAMPLE_DATAPATH"

//----------------------------------------------------------------------------//
void initialiseResourceGroupDirectories();
void initialiseDefaultResourceGroups();
const char* getDataPathPrefix();

//----------------------------------------------------------------------------//
int main(int argc, char* argv[])
{
    // Create X11 window.
    Display* dpy = XOpenDisplay(0);
    int scn = DefaultScreen(dpy);
    Window wnd = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                                     50, 50, 480, 320, 1,
                                     BlackPixel(dpy, scn),
                                     WhitePixel(dpy, scn));

    XSelectInput(dpy, wnd, StructureNotifyMask |
                           PointerMotionMask |
                           ButtonPressMask | ButtonReleaseMask |
                           KeyPressMask | KeyReleaseMask);
    XMapWindow(dpy, wnd);

    XEvent evt;
    while (true)
    {
        XNextEvent(dpy, &evt);
        if (evt.type == MapNotify)
            break;
    }

    // EGL setup
    EGLDisplay egldpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    EGLint majVer, minVer;
    eglInitialize(egldpy, &majVer, &minVer);

    EGLint attrs[] =
    {
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES_BIT,
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_NONE
    };

    EGLConfig config;
    EGLint numconfigs;
    eglChooseConfig(egldpy, attrs, &config, 1, &numconfigs);

    EGLSurface surface =
        eglCreateWindowSurface(egldpy, config, (NativeWindowType)wnd, 0);

    EGLContext ctx = eglCreateContext(egldpy, config, 0, 0);
    eglMakeCurrent(egldpy, surface, surface, ctx);

    eglBindAPI(EGL_OPENGL_ES_API);

    // basic gl state setup;
    glViewport(0, 0, 480, 320);
    glClearColor(0.2, 0.2, 0.2, 1);

    // CEGUI setup
    CEGUI::OpenGLESRenderer::bootstrapSystem();
    initialiseResourceGroupDirectories();
    initialiseDefaultResourceGroups();

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    CEGUI::WindowManager& winMgr(CEGUI::WindowManager::getSingleton());
    CEGUI::Window* root = winMgr.createWindow("DefaultWindow", "root");
    CEGUI::Window* fw = root->createChild("TaharezLook/FrameWindow");
    fw->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.25, 0)));
    fw->setSize(CEGUI::USize(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.5, 0)));
    fw->setText("OpenGL ES 1 Test");

    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(root);

    // Main looop
    bool running = true;
    while (running)
    {
        while (XPending(dpy))
        {
            XNextEvent(dpy, &evt);

            switch (evt.type)
            {
            case KeyPress:
            {
                int kspkcr;
                KeySym* ks = XGetKeyboardMapping(dpy, evt.xkey.keycode, 1, &kspkcr);

                if (ks[0] == XK_Escape)
                    running = false;

                break;
            }

            case MotionNotify:
                CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(evt.xmotion.x, evt.xmotion.y);
                break;

            case ButtonPress:
            {
                CEGUI::MouseButton btn;
                if (evt.xbutton.button == Button1)
                    btn = CEGUI::LeftButton;
                else if (evt.xbutton.button == Button2)
                    btn = CEGUI::RightButton;
                else
                    break;

                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(btn);
                break;
            }
                
            case ButtonRelease:
            {
                CEGUI::MouseButton btn;
                if (evt.xbutton.button == Button1)
                    btn = CEGUI::LeftButton;
                else if (evt.xbutton.button == Button2)
                    btn = CEGUI::RightButton;
                else
                    break;

                CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(btn);
                break;
            }
                
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        CEGUI::System::getSingleton().renderAllGUIContexts();

        eglSwapBuffers(egldpy, surface);
    }

    // cleanup
    CEGUI::OpenGLESRenderer::destroySystem();

    eglMakeCurrent(egldpy, 0, 0, 0);
    eglDestroyContext(egldpy, ctx);
    eglDestroySurface(egldpy, surface);
    eglTerminate(dpy);
    eglReleaseThread();

    XCloseDisplay(dpy);

    return 0;
}

//----------------------------------------------------------------------------//
void initialiseResourceGroupDirectories()
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
}

//----------------------------------------------------------------------------//
void initialiseDefaultResourceGroups()
{
    // set the default resource groups to be used
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
    CEGUI::AnimationManager::setDefaultResourceGroup("animations");
    // setup default group for validation schemas
    CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
    if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
        parser->setProperty("SchemaDefaultResourceGroup", "schemas");
}

//----------------------------------------------------------------------------//
const char* getDataPathPrefix()
{
    static char dataPathPrefix[PATH_MAX];

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

    return dataPathPrefix;
}

//----------------------------------------------------------------------------//
