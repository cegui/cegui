/***********************************************************************
    created:    25/9/2014
    author:     David Reepmeyer
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGuiEGLBaseApplication_h_
#define _CEGuiEGLBaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "CEGUI/Cursor.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include <EGL/egl.h>
#include <ctime>
#include <time.h>

class SampleBrowserBase;

#ifdef __ANDROID__
#   include "AndroidAppHelper.h"
#endif

class CEGuiEGLBaseApplication : public CEGuiBaseApplication
{
private:
    //EGL configurations
#ifdef __ANDROID__
    ANativeWindow* d_window;
#elif defined(__unix__)
    Display* x_display;
    Window win;
    NativeWindowType d_window;
#endif
    EGLDisplay d_display;
    EGLSurface d_surface;
    EGLContext d_context;
    EGLConfig d_config;

    int32_t d_width;
    int32_t d_height;
    int d_glesVersion;

    bool d_contextInitialised;
    bool d_contextValid;

    bool initEGLSurface();
    bool initEGLContext();
    timespec start_timespec;

public:
    CEGuiEGLBaseApplication();
    virtual ~CEGuiEGLBaseApplication();

#ifdef __ANDROID__
    void setNativeWindow(ANativeWindow* window);
#endif

    void terminate();
    bool init(int openglesVersion);
    EGLint swap();
    bool invalidate();
    void suspend();
    EGLint resume();
    SampleBrowserBase* getSampleApp();
    static CEGuiEGLBaseApplication& getSingleton();
    void clearFrame();
protected:
    // override from base class since 
    void initialiseResourceGroupDirectories();
    void run();
    virtual void destroyWindow();
    void beginRendering(const float elapsed);
    void endRendering();
    void drawFrame();
    double getElapsedTime();
    static CEGuiEGLBaseApplication* d_appInstance;
    bool d_windowSized;
};

#endif  //end of guard _CEGuiEGLBaseApplication_h_

