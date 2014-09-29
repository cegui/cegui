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
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLConfig config_;

    //Screen parameters
    int32_t screen_width_;
    int32_t screen_height_;
    int32_t color_size_;
    int32_t depth_size_;

    //Flags
    bool gles_initialized_;
    bool egl_context_initialized_;
    bool es3_supported_;
    float gl_version_;
    bool context_valid_;

    void InitGLES();
    bool InitEGLSurface();
    bool InitEGLContext();

public:
    CEGuiEGLSharedBase();
    virtual ~CEGuiEGLSharedBase();

    void Terminate();
    bool Init( ANativeWindow* window );
    SampleBrowserBase* getSampleApp();
    EGLint Swap();
    bool Invalidate();

    void Suspend();
    EGLint Resume( ANativeWindow* window );

    int32_t GetScreenWidth()
    {
        return screen_width_;
    }
    int32_t GetScreenHeight()
    {
        return screen_height_;
    }

    int32_t GetBufferColorSize()
    {
        return color_size_;
    }
    int32_t GetBufferDepthSize()
    {
        return depth_size_;
    }
    float GetGLVersion()
    {
        return gl_version_;
    }
    bool CheckExtension( const char* extension );
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

    /*************************************************************************
    Implementation Methods
    *************************************************************************/
    //static void initEGL();
    //static void createEGLWindow();
    //static void setEGLAppConfiguration();

    //static int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
    //static void engine_handle_cmd(struct android_app* app, int32_t cmd);
    //int engine_init_display();
    //void engine_term_display();
    //void createWindow();

    void drawFrame();

    /* 
    static void GLFWCALL glfwKeyCallback(int key, int action);
    static void GLFWCALL glfwCharCallback(int character, int action);
    static void GLFWCALL glfwMouseButtonCallback(int key, int action);
    static void GLFWCALL glfwMouseWheelCallback(int position);
    static void GLFWCALL glfwMousePosCallback(int x, int y );

    static int GLFWCALL glfwWindowCloseCallback(void);
    static void GLFWCALL glfwWindowResizeCallback(int width, int height);

    static CEGUI::Key::Scan GlfwToCeguiKey(int glfwKey);
    static CEGUI::MouseButton GlfwToCeguiMouseButton(int glfwButton);
    */

    /*************************************************************************
    Data fields
    *************************************************************************/
    static CEGuiEGLSharedBase* d_appInstance;

    //EGLDisplay display;
    //EGLSurface surface;
    //EGLContext context;
    //EGLConfig config;

    //android_app* d_state;

    //int32_t width, height;
    //int32_t color_size;
    //int32_t depth_size;
    //bool context_valid;

    bool d_windowSized;
    //bool d_windowCreated;
    /*
    static double  d_frameTime;
    static int  d_modifiers;

    static int d_newWindowWidth;
    static int d_newWindowHeight;

    static bool d_mouseLeftWindow;
    static bool d_mouseDisableCalled;
    static int d_oldMousePosX;
    static int d_oldMousePosY;
    */
};


#endif  // end of guard _CEGuiGLFWSharedBase_h_

