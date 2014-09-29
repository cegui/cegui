/***********************************************************************
created:    28/9/2014
author:     David Reepmeyer
*************************************************************************/
/***************************************************************************
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
#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
# include <unistd.h>
#endif

#include "CEGUISamplesConfig.h"
#include "CEGuiEGLSharedBase.h"
#include "SampleBrowserBase.h"
#include "CEGUI/CEGUI.h"

#include <stdexcept>
#include <sstream>

#ifdef __ANDROID__
#include <android/log.h>
#include <CEGUI/RendererModules/OpenGL/GLES2Renderer.h>
#endif

//----------------------------------------------------------------------------//
CEGuiEGLSharedBase* CEGuiEGLSharedBase::d_appInstance = 0;

//----------------------------------------------------------------------------//
CEGuiEGLSharedBase::CEGuiEGLSharedBase() :
    d_display( EGL_NO_DISPLAY ),
    d_surface( EGL_NO_SURFACE ),
    d_context( EGL_NO_CONTEXT ),
    d_width( 0 ),
    d_height( 0 ),
    d_contextInitialised( false ),
    d_contextValid( false )
{
    if (d_appInstance)
        throw CEGUI::InvalidRequestException("CEGuiEGLSharedBase instance already exists!");
    d_appInstance = this;
    AndroidAppHelper::createWindow();
    d_renderer = &CEGUI::OpenGLES2Renderer::create();
}

//----------------------------------------------------------------------------//
CEGuiEGLSharedBase::~CEGuiEGLSharedBase()
{
    Terminate();
}

CEGuiEGLSharedBase& CEGuiEGLSharedBase::getSingleton()
{
    return (*d_appInstance);
}


//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::run()
{

    d_sampleApp->initialise();

    d_windowSized = false; //The resize callback is being called immediately after setting it in this version of glfw
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    while (!d_sampleApp->isQuitting())
    {
        engine_draw_frame();
#ifdef __ANDROID__
        if (!AndroidAppHelper::processEvents())
        {
            d_sampleApp->setQuitting(true);
            Terminate();
        }
#endif
        if (d_windowSized)
        {
            d_windowSized = false;
            //CEGUI::System::getSingleton().notifyDisplaySizeChanged(
            //CEGUI::Sizef(static_cast<float>(d_newWindowWidth),
            //static_cast<float>(d_newWindowHeight)));
        }

    }
    d_sampleApp->deinitialise();
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::engine_draw_frame()
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, d_width, d_height );
    drawFrame();
    Swap();
}

//----------------------------------------------------------------------------//
SampleBrowserBase* CEGuiEGLSharedBase::getSampleApp()
{
    return d_sampleApp;
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::initialiseResourceGroupDirectories()
{
    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());

    //const char* dataPathPrefix = getDataPathPrefix();
    const char* dataPathPrefix = "datafiles";
    char resourcePath[PATH_MAX];

    // for each resource type, set a resource group directory
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "schemes");
    rp->setResourceGroupDirectory("schemes", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "imagesets");
    rp->setResourceGroupDirectory("imagesets", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "fonts");
    rp->setResourceGroupDirectory("fonts", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "layouts");
    rp->setResourceGroupDirectory("layouts", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "looknfeel");
    rp->setResourceGroupDirectory("looknfeels", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "lua_scripts");
    rp->setResourceGroupDirectory("lua_scripts", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "xml_schemas");
    rp->setResourceGroupDirectory("schemas", resourcePath);
    sprintf(resourcePath, "%s/%s", dataPathPrefix, "animations");
    rp->setResourceGroupDirectory("animations", resourcePath);
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::destroyWindow()
{
//    glfwTerminate();
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::beginRendering(const float /*elapsed*/)
{
    //glClear(GL_COLOR_BUFFER_BIT);
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::endRendering()
{

}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::drawFrame()
{
    // calculate time elapsed since last frame
    //double time_now = glfwGetTime();
    //const double elapsed = time_now - d_frameTime;
    //d_frameTime = time_now;
    const double elapsed = 0.1f;
    d_appInstance->renderSingleFrame(static_cast<float>(elapsed));
}

//----------------------------------------------------------------------------//
bool CEGuiEGLSharedBase::Init( ANativeWindow* window )
{
    if( d_contextInitialised )
        return true;

    window_ = window;
    InitEGLSurface();
    InitEGLContext();

    d_contextInitialised = true;

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiEGLSharedBase::InitEGLSurface()
{
    d_display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    eglInitialize( d_display, 0, 0 );

    const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
                               EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
                               EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
                               EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE
                             };
    int32_t color_size_ = 8;
    int32_t depth_size_ = 24;

    EGLint num_configs;
    eglChooseConfig( d_display, attribs, &d_config, 1, &num_configs );

    if( !num_configs )
    {
        //Fall back to 16bit depth buffer
        const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
                                   EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
                                   EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
                                   EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 16, EGL_NONE
                                 };
        eglChooseConfig( d_display, attribs, &d_config, 1, &num_configs );
        depth_size_ = 16;
    }

    if( !num_configs )
    {
        LOGW( "Unable to retrieve EGL config" );
        return false;
    }

    d_surface = eglCreateWindowSurface( d_display, d_config, window_, NULL );
    eglQuerySurface( d_display, d_surface, EGL_WIDTH, &d_width );
    eglQuerySurface( d_display, d_surface, EGL_HEIGHT, &d_height );
    EGLint format;
    eglGetConfigAttrib( d_display, d_config, EGL_NATIVE_VISUAL_ID, &format );
    ANativeWindow_setBuffersGeometry( window_, 0, 0, format );
    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiEGLSharedBase::InitEGLContext()
{
    const EGLint d_contextattribs[] =
    {
        //EGL_CONTEXT_CLIENT_VERSION, 2, //Request opengl ES2
        EGL_CONTEXT_CLIENT_VERSION, 3, //Request opengl ES3
        EGL_NONE
    };

    d_context = eglCreateContext( d_display, d_config, NULL, d_contextattribs );

    if( eglMakeCurrent( d_display, d_surface, d_surface, d_context ) == EGL_FALSE )
    {
        LOGW( "Unable to eglMakeCurrent" );
        return false;
    }

    d_contextValid = true;
    return true;
}

//----------------------------------------------------------------------------//
EGLint CEGuiEGLSharedBase::Swap()
{
    bool b = eglSwapBuffers( d_display, d_surface );
    if( !b )
    {
        LOGW( "eglSwapBuffers Fails" );
        EGLint err = eglGetError();
        if( err == EGL_BAD_SURFACE )
        {
            //Recreate surface
            InitEGLSurface();
            return EGL_SUCCESS; //Still consider glContext is valid
        }
        else if( err == EGL_CONTEXT_LOST || err == EGL_BAD_CONTEXT )
        {
            //Context has been lost!!
            d_contextValid = false;
            Terminate();
            InitEGLContext();
        }
        return err;
    }
    return EGL_SUCCESS;
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::Terminate()
{
    if( d_display != EGL_NO_DISPLAY )
    {
        eglMakeCurrent( d_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
        if( d_context != EGL_NO_CONTEXT )
        {
            eglDestroyContext( d_display, d_context );
        }

        if( d_surface != EGL_NO_SURFACE )
        {
            eglDestroySurface( d_display, d_surface );
        }
        eglTerminate( d_display );
    }

    d_display = EGL_NO_DISPLAY;
    d_context = EGL_NO_CONTEXT;
    d_surface = EGL_NO_SURFACE;
    d_contextValid = false;

}

//----------------------------------------------------------------------------//
EGLint CEGuiEGLSharedBase::Resume( ANativeWindow* window )
{
    if( d_contextInitialised == false )
    {
        Init( window );
        return EGL_SUCCESS;
    }

    int32_t original_widhth = d_width;
    int32_t original_height = d_height;

    //Create surface
    window_ = window;
    d_surface = eglCreateWindowSurface( d_display, d_config, window_, NULL );
    eglQuerySurface( d_display, d_surface, EGL_WIDTH, &d_width );
    eglQuerySurface( d_display, d_surface, EGL_HEIGHT, &d_height );

    if( d_width != original_widhth || d_height != original_height )
    {
        //Screen resized
        LOGI( "Screen resized" );
    }

    if( eglMakeCurrent( d_display, d_surface, d_surface, d_context ) == EGL_TRUE )
        return EGL_SUCCESS;

    EGLint err = eglGetError();
    LOGW( "Unable to eglMakeCurrent %d", err );

    if( err == EGL_CONTEXT_LOST )
    {
        //Recreate context
        LOGI( "Re-creating egl context" );
        InitEGLContext();
    }
    else
    {
        //Recreate surface
        Terminate();
        InitEGLSurface();
        InitEGLContext();
    }

    return err;
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::Suspend()
{
    if( d_surface != EGL_NO_SURFACE )
    {
        eglDestroySurface( d_display, d_surface );
        d_surface = EGL_NO_SURFACE;
    }
}

//----------------------------------------------------------------------------//
bool CEGuiEGLSharedBase::Invalidate()
{
    Terminate();

    d_contextInitialised = false;
    return true;
}

//----------------------------------------------------------------------------//


