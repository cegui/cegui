/***********************************************************************
    created:    28/9/2014
    author:     David Reepmeyer
*************************************************************************/
/***************************************************************************

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
***************************************************************************/
#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
# include <unistd.h>
#endif

#include "CEGUISamplesConfig.h"
#include "SampleBrowserBase.h"
#include "CEGUI/CEGUI.h"

#include <stdexcept>
#include <sstream>

#include <CEGUI/RendererModules/OpenGL/GLES2Renderer.h>
#include "CEGuiEGLBaseApplication.h"

//----------------------------------------------------------------------------//
CEGuiEGLBaseApplication* CEGuiEGLBaseApplication::d_appInstance = 0;

//----------------------------------------------------------------------------//
CEGuiEGLBaseApplication::CEGuiEGLBaseApplication() :
    d_display(EGL_NO_DISPLAY),
    d_surface(EGL_NO_SURFACE),
    d_context(EGL_NO_CONTEXT),
    d_width(0),
    d_height(0),
    d_contextInitialised(false),
    d_contextValid(false),
    d_glesVersion(0)
{
    if(d_appInstance)
        throw CEGUI::InvalidRequestException("CEGuiEGLBaseApplication instance already exists!");

    d_appInstance = this;
#ifdef __ANDROID__
    AndroidAppHelper::createWindow();
#endif
    d_renderer = &CEGUI::GLES2Renderer::create();
}

//----------------------------------------------------------------------------//
CEGuiEGLBaseApplication::~CEGuiEGLBaseApplication()
{
    terminate();
}

CEGuiEGLBaseApplication& CEGuiEGLBaseApplication::getSingleton()
{
    return (*d_appInstance);
}


//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::run()
{

    d_sampleApp->initialise();

    d_windowSized = false; //The resize callback is being called immediately after setting it in this version of glfw
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    clock_gettime(CLOCK_MONOTONIC, &start_timespec);

    while(!d_sampleApp->isQuitting())
    {
        drawFrame();

#ifdef __ANDROID__

        if(!AndroidAppHelper::processEvents())
        {
            d_sampleApp->setQuitting(true);
            terminate();
        }

#endif
        if(d_windowSized)
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
SampleBrowserBase* CEGuiEGLBaseApplication::getSampleApp()
{
    return d_sampleApp;
}

//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::initialiseResourceGroupDirectories()
{
    // initialise the required dirs for the DefaultResourceProvider
    CEGUI::DefaultResourceProvider* rp =
        static_cast<CEGUI::DefaultResourceProvider*>
        (CEGUI::System::getSingleton().getResourceProvider());
#ifdef __ANDROID__
    const char* dataPathPrefix = "datafiles";
#else
    const char* dataPathPrefix = getDataPathPrefix().c_str();
#endif
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
void CEGuiEGLBaseApplication::destroyWindow()
{
    terminate();
}

//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::beginRendering(const float /*elapsed*/)
{
    clearFrame();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, d_width, d_height);
}

//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::endRendering()
{
    swap();
}
//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::clearFrame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::drawFrame()
{
    d_appInstance->renderSingleFrame(static_cast<float>(getElapsedTime()));
}

double CEGuiEGLBaseApplication::getElapsedTime()
{
    timespec now_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now_timespec);
    double elapsed = (now_timespec.tv_sec - start_timespec.tv_sec);
    elapsed += ((double)(now_timespec.tv_nsec - start_timespec.tv_nsec)) / ((double) 1000000000L);
    start_timespec = now_timespec;
    return elapsed;
}

//----------------------------------------------------------------------------//
bool CEGuiEGLBaseApplication::init(int openglesVersion)
{
    d_glesVersion = openglesVersion;

    if(d_contextInitialised)
        return true;

    initEGLSurface();
    initEGLContext();

    d_contextInitialised = true;

    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiEGLBaseApplication::initEGLSurface()
{

    d_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(d_display, 0, 0);

    const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
                               EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
                               EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
                               EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE
                             };
    int32_t color_size_ = 8;
    int32_t depth_size_ = 24;

    EGLint num_configs;
    eglChooseConfig(d_display, attribs, &d_config, 1, &num_configs);

    if(!num_configs)
    {
        //Fall back to 16bit depth buffer
        const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
                                   EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
                                   EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
                                   EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 16, EGL_NONE
                                 };
        eglChooseConfig(d_display, attribs, &d_config, 1, &num_configs);
        depth_size_ = 16;
    }

    if(!num_configs)
    {
        return false;
    }

    d_surface = eglCreateWindowSurface(d_display, d_config, d_window, NULL);
    eglQuerySurface(d_display, d_surface, EGL_WIDTH, &d_width);
    eglQuerySurface(d_display, d_surface, EGL_HEIGHT, &d_height);
    EGLint format;
    eglGetConfigAttrib(d_display, d_config, EGL_NATIVE_VISUAL_ID, &format);
#ifdef __ANDROID__
    ANativeWindow_setBuffersGeometry(d_window, 0, 0, format);
#endif
    return true;
}

//----------------------------------------------------------------------------//
bool CEGuiEGLBaseApplication::initEGLContext()
{
    const EGLint d_contextattribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, d_glesVersion, //Request opengl es version (2/3)
        EGL_NONE
    };

    d_context = eglCreateContext(d_display, d_config, NULL, d_contextattribs);

    if(eglMakeCurrent(d_display, d_surface, d_surface, d_context) == EGL_FALSE)
    {
        return false;
    }

    d_contextValid = true;
    return true;
}

//----------------------------------------------------------------------------//
EGLint CEGuiEGLBaseApplication::swap()
{
    bool b = eglSwapBuffers(d_display, d_surface);

    if(!b)
    {
        EGLint err = eglGetError();

        if(err == EGL_BAD_SURFACE)
        {
            //Recreate surface
            initEGLSurface();
            return EGL_SUCCESS; //Still consider glContext is valid
        }
        else if(err == EGL_CONTEXT_LOST || err == EGL_BAD_CONTEXT)
        {
            //Context has been lost!!
            d_contextValid = false;
            terminate();
            initEGLContext();
        }

        return err;
    }

    return EGL_SUCCESS;
}

//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::terminate()
{
    if(d_display != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(d_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if(d_context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(d_display, d_context);
        }

        if(d_surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(d_display, d_surface);
        }

        eglTerminate(d_display);
    }

    d_display = EGL_NO_DISPLAY;
    d_context = EGL_NO_CONTEXT;
    d_surface = EGL_NO_SURFACE;
    d_contextValid = false;

}

//----------------------------------------------------------------------------//
EGLint CEGuiEGLBaseApplication::resume()
{
    if(d_contextInitialised == false)
    {
        init(d_glesVersion);
        return EGL_SUCCESS;
    }

    int32_t original_widhth = d_width;
    int32_t original_height = d_height;

    //Create surface
    d_surface = eglCreateWindowSurface(d_display, d_config, d_window, NULL);
    eglQuerySurface(d_display, d_surface, EGL_WIDTH, &d_width);
    eglQuerySurface(d_display, d_surface, EGL_HEIGHT, &d_height);

    if(eglMakeCurrent(d_display, d_surface, d_surface, d_context) == EGL_TRUE)
        return EGL_SUCCESS;

    EGLint err = eglGetError();

    if(err == EGL_CONTEXT_LOST)
    {
        //Recreate context
        initEGLContext();
    }
    else
    {
        //Recreate surface
        terminate();
        initEGLSurface();
        initEGLContext();
    }

    return err;
}

//----------------------------------------------------------------------------//
#ifdef __ANDROID__
void CEGuiEGLBaseApplication::setNativeWindow(ANativeWindow* window)
{
    d_window = window;
}
#endif

//----------------------------------------------------------------------------//
void CEGuiEGLBaseApplication::suspend()
{
    if(d_surface != EGL_NO_SURFACE)
    {
        eglDestroySurface(d_display, d_surface);
        d_surface = EGL_NO_SURFACE;
    }
}

//----------------------------------------------------------------------------//
bool CEGuiEGLBaseApplication::invalidate()
{
    terminate();

    d_contextInitialised = false;
    return true;
}

//----------------------------------------------------------------------------//

