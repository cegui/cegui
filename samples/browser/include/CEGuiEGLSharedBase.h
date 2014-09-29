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
#ifndef _CEGuiGLFWSharedBase_h_
#define _CEGuiGLFWSharedBase_h_

#include "CEGuiBaseApplication.h"
#include "CEGUI/Cursor.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"

#include <EGL/egl.h>

class SampleBrowserBase;

#ifdef __ANDROID__
#   include "AndroidAppHelper.h"
#endif

class CEGuiEGLSharedBase : public CEGuiBaseApplication
{
private:
    //EGL configurations
    ANativeWindow* window_;
    EGLDisplay d_display;
    EGLSurface d_surface;
    EGLContext d_context;
    EGLConfig d_config;

    int32_t d_width;
    int32_t d_height;

    bool d_contextInitialised;
    bool d_contextValid;

    bool InitEGLSurface();
    bool InitEGLContext();

public:
    CEGuiEGLSharedBase();
    virtual ~CEGuiEGLSharedBase();

    void Terminate();
    bool Init( ANativeWindow* window );
    EGLint Swap();
    bool Invalidate();
    void Suspend();
    EGLint Resume( ANativeWindow* window );
    SampleBrowserBase* getSampleApp();
    static CEGuiEGLSharedBase& getSingleton();
    void engine_draw_frame();
protected:
    // override from base class since we use a non-default resource provider.
    void initialiseResourceGroupDirectories();
    // implementation of base class abstract functions.
    void run();
    void destroyWindow();
    void beginRendering(const float elapsed);
    void endRendering();
    void drawFrame();
    static CEGuiEGLSharedBase* d_appInstance;
    bool d_windowSized;
};


#endif  // end of guard _CEGuiGLFWSharedBase_h_

