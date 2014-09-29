/***********************************************************************
created:    12/2/2012
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
//#include "AndroidAppHelper.h"
#include <CEGUI/RendererModules/OpenGL/GLES2Renderer.h>
#endif

//----------------------------------------------------------------------------//
CEGuiEGLSharedBase* CEGuiEGLSharedBase::d_appInstance = 0;
/* 
double  CEGuiEGLSharedBase::d_frameTime = 0;
int CEGuiEGLSharedBase::d_modifiers = 0;
bool CEGuiEGLSharedBase::d_windowSized = false;
int CEGuiEGLSharedBase::d_newWindowWidth = CEGuiEGLSharedBase::s_defaultWindowWidth;
int CEGuiEGLSharedBase::d_newWindowHeight = CEGuiEGLSharedBase::s_defaultWindowWidth;
bool CEGuiEGLSharedBase::d_mouseLeftWindow = false;
bool CEGuiEGLSharedBase::d_mouseDisableCalled = false;
int CEGuiEGLSharedBase::d_oldMousePosX = 0;
int CEGuiEGLSharedBase::d_oldMousePosY = 0;
*/

//----------------------------------------------------------------------------//
CEGuiEGLSharedBase::CEGuiEGLSharedBase() :
                display_( EGL_NO_DISPLAY ),
                surface_( EGL_NO_SURFACE ),
                context_( EGL_NO_CONTEXT ),
                screen_width_( 0 ),
                screen_height_( 0 ),
                es3_supported_( false ),
                egl_context_initialized_( false ),
                gles_initialized_( false )

{
    __android_log_write (ANDROID_LOG_DEBUG, "EGL", "EGL constructor start!!!!");
    if (d_appInstance)
        throw CEGUI::InvalidRequestException(
        "CEGuiEGLSharedBase instance already exists!");
    d_appInstance = this;
    __android_log_write (ANDROID_LOG_DEBUG, "EGL", "EGL constructor create window!!!!");
    //d_state = AndroidAppHelper::getAndroidApp(); 
    //d_state->onAppCmd = AndroidAppHelper::engine_handle_cmd;
    AndroidAppHelper::createWindow();
    //d_state->onInputEvent = AndroidAppHelper::engine_handle_input;
    __android_log_write (ANDROID_LOG_DEBUG, "EGL", "APP construction!!!!");
    d_renderer = &CEGUI::OpenGL3Renderer::create();
    __android_log_write (ANDROID_LOG_DEBUG, "EGL", "APP construction complete!!!!");
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

    __android_log_write (ANDROID_LOG_DEBUG, "EGL", "APP RUN!!!!");
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
        if (d_windowSized) {
            d_windowSized = false;
            //CEGUI::System::getSingleton().notifyDisplaySizeChanged(
            //CEGUI::Sizef(static_cast<float>(d_newWindowWidth),
            //static_cast<float>(d_newWindowHeight)));
        }

    }
    d_sampleApp->deinitialise();
}
void CEGuiEGLSharedBase::engine_draw_frame() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  
    glViewport(0, 0, screen_width_, screen_height_ ); 
    drawFrame();
    Swap();
}

SampleBrowserBase* CEGuiEGLSharedBase::getSampleApp() {
    return d_sampleApp;
}

/*
void CEGuiEGLSharedBase::engine_draw_frame() {
    if (display == NULL) {
        return;
    }

    //glViewport(0, 0, width, height);
    //glClearColor(0,0,0,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    drawFrame();
    //glFlush();
    //__android_log_write (ANDROID_LOG_DEBUG, "EGL", "SWAP BUFFERS BEING CALLED!!!!!!!!!!!!!!!");
    //__android_log_write (ANDROID_LOG_DEBUG, "EGL", "SWAP BUFFERS before");
    //eglSwapBuffers(display, surface);
    //eglWaitGL();
    //eglWaitClient();
    //
    if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
        __android_log_write (ANDROID_LOG_DEBUG, "EGL", "failed to make current");
    }
    
    if (EGL_FALSE == eglWaitGL()) {
        __android_log_write (ANDROID_LOG_DEBUG, "EGL", "eglwait fail");
        //__android_log_write (ANDROID_LOG_DEBUG, "EGL eglWaitGL() failed. [0x%x]\n", eglGetError());
        //LOGE("_egl_end(): eglWaitGL() failed - NO SWAP [0x%x]\n", eglGetError());
    }

    if (EGL_TRUE != eglSwapBuffers(display, surface)) {
        __android_log_write (ANDROID_LOG_DEBUG, "EGL", "SWAP BUFFERS fail");
        //__android_log_write (ANDROID_LOG_DEBUG, "EGL eglSwapBuffers() failed. [0x%x]\n", eglGetError());
        //LOGE("_egl_end(): eglSwapBuffers() failed. [0x%x]\n", eglGetError());
    }

    Swap();
    //eglSwapBuffers(display, surface);
    //__android_log_write (ANDROID_LOG_DEBUG, "EGL", "SWAP BUFFERS after");
}

void CEGuiEGLSharedBase::engine_term_display() {
    if (display != EGL_NO_DISPLAY) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context != EGL_NO_CONTEXT) {
            eglDestroyContext(display, context);
        }
        if (surface != EGL_NO_SURFACE) {
            eglDestroySurface(display, surface);
        }
        eglTerminate(display);
    }
    display = EGL_NO_DISPLAY;
    context = EGL_NO_CONTEXT;
    surface = EGL_NO_SURFACE;
}

int CEGuiEGLSharedBase::engine_init_display() {
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, 0, 0);

    EGLint format;
    EGLint num_configs;

    const EGLint attribs[] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, //Request opengl ES2.0 
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE };
    color_size = 8;
    depth_size = 24;

    eglChooseConfig( display, attribs, &config, 1, &num_configs );

    if( !num_configs )
    {
        //Fall back to 16bit depth buffer
        const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 16, EGL_NONE };
        eglChooseConfig( display, attribs, &config, 1, &num_configs );
        depth_size = 16;
    }

    if( !num_configs )
    {
        __android_log_write (ANDROID_LOG_DEBUG, "EGL", "Unable to retrieve EGL config");
        return -1;
    }

    surface = eglCreateWindowSurface( display, config, d_state->window, NULL );
    eglQuerySurface( display, surface, EGL_WIDTH, &width );
    eglQuerySurface( display, surface, EGL_HEIGHT, &height);
    eglGetConfigAttrib( display, config, EGL_NATIVE_VISUAL_ID, &format );
    ANativeWindow_setBuffersGeometry( d_state->window, 0, 0, format );

    const EGLint context_attribs[] = { 
        EGL_CONTEXT_CLIENT_VERSION, 2, //Request opengl ES2.0
        //EGL_CONTEXT_CLIENT_VERSION, 3, //Request opengl ES3.0
        EGL_NONE };
    context = eglCreateContext( display, config, NULL, context_attribs );

    if( eglMakeCurrent(display, surface, surface, context ) == EGL_FALSE )
    {
        __android_log_write (ANDROID_LOG_DEBUG, "EGL", "Unable to eglMakeCurrent");
        return -1;
    }

    __android_log_write (ANDROID_LOG_DEBUG, "EGL", "APP engine_init_display complete!!!!");
    //context_valid_ = true;
    return true;


    //return 0;
}
  */
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

/*
//----------------------------------------------------------------------------//
int CEGuiEGLSharedBase::glfwWindowCloseCallback(void)
{
    d_sampleApp->setQuitting(true);
    return GL_TRUE;
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::glfwWindowResizeCallback(int w, int h)
{
    // We cache this in order to minimise calls to notifyDisplaySizeChanged,
    // which happens in the main loop whenever d_windowSized is set to true.
    d_windowSized = true;
    d_newWindowWidth = w;
    d_newWindowHeight = h;
}

//----------------------------------------------------------------------------//
CEGUI::Key::Scan CEGuiEGLSharedBase::GlfwToCeguiKey(int glfwKey)
{
    switch(glfwKey)
    {
    case EGL_KEY_ESC       : return CEGUI::Key::Escape;
    case EGL_KEY_F1        : return CEGUI::Key::F1;
    case EGL_KEY_F2        : return CEGUI::Key::F2;
    case EGL_KEY_F3        : return CEGUI::Key::F3;
    case EGL_KEY_F4        : return CEGUI::Key::F4;
    case EGL_KEY_F5        : return CEGUI::Key::F5;
    case EGL_KEY_F6        : return CEGUI::Key::F6;
    case EGL_KEY_F7        : return CEGUI::Key::F7;
    case EGL_KEY_F8        : return CEGUI::Key::F8;
    case EGL_KEY_F9        : return CEGUI::Key::F9;
    case EGL_KEY_F10       : return CEGUI::Key::F10;
    case EGL_KEY_F11       : return CEGUI::Key::F11;
    case EGL_KEY_F12       : return CEGUI::Key::F12;
    case EGL_KEY_F13       : return CEGUI::Key::F13;
    case EGL_KEY_F14       : return CEGUI::Key::F14;
    case EGL_KEY_F15       : return CEGUI::Key::F15;
    case EGL_KEY_UP        : return CEGUI::Key::ArrowUp;
    case EGL_KEY_DOWN      : return CEGUI::Key::ArrowDown;
    case EGL_KEY_LEFT      : return CEGUI::Key::ArrowLeft;
    case EGL_KEY_RIGHT     : return CEGUI::Key::ArrowRight;
    case EGL_KEY_LSHIFT    : return CEGUI::Key::LeftShift;
    case EGL_KEY_RSHIFT    : return CEGUI::Key::RightShift;
    case EGL_KEY_LCTRL     : return CEGUI::Key::LeftControl;
    case EGL_KEY_RCTRL     : return CEGUI::Key::RightControl;
    case EGL_KEY_LALT      : return CEGUI::Key::LeftAlt;
    case EGL_KEY_RALT      : return CEGUI::Key::RightAlt;
    case EGL_KEY_TAB       : return CEGUI::Key::Tab;
    case EGL_KEY_ENTER     : return CEGUI::Key::Return;
    case EGL_KEY_BACKSPACE : return CEGUI::Key::Backspace;
    case EGL_KEY_INSERT    : return CEGUI::Key::Insert;
    case EGL_KEY_DEL       : return CEGUI::Key::Delete;
    case EGL_KEY_PAGEUP    : return CEGUI::Key::PageUp;
    case EGL_KEY_PAGEDOWN  : return CEGUI::Key::PageDown;
    case EGL_KEY_HOME      : return CEGUI::Key::Home;
    case EGL_KEY_END       : return CEGUI::Key::End;
    case EGL_KEY_KP_ENTER  : return CEGUI::Key::NumpadEnter;
    case EGL_KEY_SPACE     : return CEGUI::Key::Space;
    case 'A'                : return CEGUI::Key::A;
    case 'B'                : return CEGUI::Key::B;
    case 'C'                : return CEGUI::Key::C;
    case 'D'                : return CEGUI::Key::D;
    case 'E'                : return CEGUI::Key::E;
    case 'F'                : return CEGUI::Key::F;
    case 'G'                : return CEGUI::Key::G;
    case 'H'                : return CEGUI::Key::H;
    case 'I'                : return CEGUI::Key::I;
    case 'J'                : return CEGUI::Key::J;
    case 'K'                : return CEGUI::Key::K;
    case 'L'                : return CEGUI::Key::L;
    case 'M'                : return CEGUI::Key::M;
    case 'N'                : return CEGUI::Key::N;
    case 'O'                : return CEGUI::Key::O;
    case 'P'                : return CEGUI::Key::P;
    case 'Q'                : return CEGUI::Key::Q;
    case 'R'                : return CEGUI::Key::R;
    case 'S'                : return CEGUI::Key::S;
    case 'T'                : return CEGUI::Key::T;
    case 'U'                : return CEGUI::Key::U;
    case 'V'                : return CEGUI::Key::V;
    case 'W'                : return CEGUI::Key::W;
    case 'X'                : return CEGUI::Key::X;
    case 'Y'                : return CEGUI::Key::Y;
    case 'Z'                : return CEGUI::Key::Z;
    default                 : return CEGUI::Key::Unknown;
    }
}

//----------------------------------------------------------------------------//
CEGUI::MouseButton CEGuiEGLSharedBase::GlfwToCeguiMouseButton(int glfwButton)
{
    switch(glfwButton)
    {
    case EGL_MOUSE_BUTTON_LEFT     : return CEGUI::LeftButton;
    case EGL_MOUSE_BUTTON_RIGHT    : return CEGUI::RightButton;
    case EGL_MOUSE_BUTTON_MIDDLE   : return CEGUI::MiddleButton;
    default                         : return CEGUI::NoButton;
    }
}

//----------------------------------------------------------------------------//
void EGLCALL CEGuiEGLSharedBase::glfwKeyCallback(int key, int action)
{
    CEGUI::Key::Scan ceguiKey = GlfwToCeguiKey(key);

    if(action == EGL_PRESS)
        d_sampleApp->injectKeyDown(ceguiKey);
    else if (action == EGL_RELEASE)
        d_sampleApp->injectKeyUp(ceguiKey);
}

//----------------------------------------------------------------------------//
void EGLCALL CEGuiEGLSharedBase::glfwCharCallback(int character, int action)
{
    if(action == EGL_PRESS)
        d_sampleApp->injectChar(character);
}

//----------------------------------------------------------------------------//
void EGLCALL CEGuiEGLSharedBase::glfwMouseButtonCallback(int key, int action)
{
    CEGUI::MouseButton ceguiMouseButton = GlfwToCeguiMouseButton(key);

    if(action == EGL_PRESS)
        d_sampleApp->injectMouseButtonDown(ceguiMouseButton);
    else if (action == EGL_RELEASE)
        d_sampleApp->injectMouseButtonUp(ceguiMouseButton);
}

//----------------------------------------------------------------------------//
void EGLCALL CEGuiEGLSharedBase::glfwMouseWheelCallback(int position)
{
    static int lastPosition = 0;
    d_sampleApp->injectMouseWheelChange(static_cast<float>(position - lastPosition));
    lastPosition = position;
}

//----------------------------------------------------------------------------//
void EGLCALL CEGuiEGLSharedBase::glfwMousePosCallback(int x, int y)
{
    if (!d_mouseDisableCalled)
    {
        // if cursor didn't leave the window nothing changes
        d_sampleApp->injectMousePosition(static_cast<float>(x), static_cast<float>(y));
    }
    else
    {
        // if the cursor left the window, we need to use the saved position
        // because glfw beams the cursor to the middle of the window if 
        // the cursor is disabled
        d_sampleApp->injectMousePosition(static_cast<float>(d_oldMousePosX), static_cast<float>(d_oldMousePosY));
        glfwSetMousePos(d_oldMousePosX, d_oldMousePosY);
        d_mouseDisableCalled = false;
    }

#ifndef DEBUG
    if (x < 0 || y < 0
        || x > d_newWindowWidth
        || y > d_newWindowHeight)
    {
        // show cursor
        glfwEnable(EGL_MOUSE_CURSOR);

        // move the cursor to the position where it left the window
        glfwSetMousePos(x, y);
        
        // "note down" that the cursor left the window
        d_mouseLeftWindow = true;
    }
    else
    {
        if (d_mouseLeftWindow)
        {
            // get cursor position to restore afterwards
            glfwGetMousePos(&d_oldMousePosX, &d_oldMousePosY);

            // we need to inject the previous cursor position because
            // glfw moves the cursor to the centre of the render 
            // window if it gets disabled. therefore notify the 
            // next MousePosCallback invocation of the "mouse disabled" event.
            d_mouseDisableCalled = true;

            // disable cursor
            glfwDisable(EGL_MOUSE_CURSOR);

            // "note down" that the cursor is back in the render window
            d_mouseLeftWindow = false;
        }
    }
#endif
}
//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::initEGL()
{

}
//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::createEGLWindow()
{
    if (glfwOpenWindow(s_defaultWindowWidth, s_defaultWindowHeight, 8, 8, 8, 0, 24, 0, EGL_WINDOW) != GL_TRUE)
    {
        CEGUI_THROW(CEGUI::RendererException("Failed to open EGL window."));
        glfwTerminate();
    }
}

//----------------------------------------------------------------------------//
void CEGuiEGLSharedBase::setEGLAppConfiguration()
{
    glfwSetWindowTitle("Crazy Eddie's GUI Mk-2 - Sample Application");

    //Deactivate VSYNC
    glfwSwapInterval(0);

    // Disable the mouse position in Non_Debug mode
#ifndef DEBUG
    glfwDisable(EGL_MOUSE_CURSOR);
#endif
    // Clear Errors by EGL, even if Setup is correct.
    glGetError();
}
*/

void CEGuiEGLSharedBase::InitGLES()
{
    if( gles_initialized_ )
        return;
    //
    //Initialize OpenGL ES 3 if available
    //
    const char* versionStr = (const char*) glGetString( GL_VERSION );
    //if( strstr( versionStr, "OpenGL ES 3." ) && gl3stubInit() )
    if( strstr( versionStr, "OpenGL ES 3." ) && true )
    {
        es3_supported_ = true;
        gl_version_ = 3.0f;
    }
    else
    {
        gl_version_ = 2.0f;
    }

    gles_initialized_ = true;
}

bool CEGuiEGLSharedBase::Init( ANativeWindow* window )
{
    if( egl_context_initialized_ )
        return true;

    //
    //Initialize EGL
    //
    window_ = window;
    InitEGLSurface();
    InitEGLContext();
    InitGLES();

    egl_context_initialized_ = true;

    return true;
}

bool CEGuiEGLSharedBase::InitEGLSurface()
{
    display_ = eglGetDisplay( EGL_DEFAULT_DISPLAY );
    eglInitialize( display_, 0, 0 );

    /*
     * Here specify the attributes of the desired configuration.
     * Below, we select an EGLConfig with at least 8 bits per color
     * component compatible with on-screen windows
     */
    const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
            EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 24, EGL_NONE };
    color_size_ = 8;
    depth_size_ = 24;

    EGLint num_configs;
    eglChooseConfig( display_, attribs, &config_, 1, &num_configs );

    if( !num_configs )
    {
        //Fall back to 16bit depth buffer
        const EGLint attribs[] = { EGL_RENDERABLE_TYPE,
                EGL_OPENGL_ES2_BIT, //Request opengl ES2.0
                EGL_SURFACE_TYPE, EGL_WINDOW_BIT, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8,
                EGL_RED_SIZE, 8, EGL_DEPTH_SIZE, 16, EGL_NONE };
        eglChooseConfig( display_, attribs, &config_, 1, &num_configs );
        depth_size_ = 16;
    }

    if( !num_configs )
    {
        LOGW( "Unable to retrieve EGL config" );
        return false;
    }

    surface_ = eglCreateWindowSurface( display_, config_, window_, NULL );
    eglQuerySurface( display_, surface_, EGL_WIDTH, &screen_width_ );
    eglQuerySurface( display_, surface_, EGL_HEIGHT, &screen_height_ );

    /* EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is
     * guaranteed to be accepted by ANativeWindow_setBuffersGeometry().
     * As soon as we picked a EGLConfig, we can safely reconfigure the
     * ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID. */
    EGLint format;
    eglGetConfigAttrib( display_, config_, EGL_NATIVE_VISUAL_ID, &format );
    ANativeWindow_setBuffersGeometry( window_, 0, 0, format );

    return true;
}

bool CEGuiEGLSharedBase::InitEGLContext()
{
    const EGLint context_attribs[] = { 
        //EGL_CONTEXT_CLIENT_VERSION, 2, //Request opengl ES2.0
        EGL_CONTEXT_CLIENT_VERSION, 3, //Request opengl ES2.0
        EGL_NONE };

    LOGW( "trying to make eglCreateContext" );
    context_ = eglCreateContext( display_, config_, NULL, context_attribs );

    LOGW( "trying to make eglMakeCurrent" );
    if( eglMakeCurrent( display_, surface_, surface_, context_ ) == EGL_FALSE )
    {
        LOGW( "Unable to eglMakeCurrent" );
        return false;
    }

    context_valid_ = true;
    return true;
}

EGLint CEGuiEGLSharedBase::Swap()
{
    bool b = eglSwapBuffers( display_, surface_ );
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
            context_valid_ = false;
            Terminate();
            InitEGLContext();
        }
        return err;
    }
    return EGL_SUCCESS;
}

void CEGuiEGLSharedBase::Terminate()
{
    if( display_ != EGL_NO_DISPLAY )
    {
        eglMakeCurrent( display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT );
        if( context_ != EGL_NO_CONTEXT )
        {
            eglDestroyContext( display_, context_ );
        }

        if( surface_ != EGL_NO_SURFACE )
        {
            eglDestroySurface( display_, surface_ );
        }
        eglTerminate( display_ );
    }

    display_ = EGL_NO_DISPLAY;
    context_ = EGL_NO_CONTEXT;
    surface_ = EGL_NO_SURFACE;
    context_valid_ = false;

}

EGLint CEGuiEGLSharedBase::Resume( ANativeWindow* window )
{
    if( egl_context_initialized_ == false )
    {
        Init( window );
        return EGL_SUCCESS;
    }

    int32_t original_widhth = screen_width_;
    int32_t original_height = screen_height_;

    //Create surface
    window_ = window;
    surface_ = eglCreateWindowSurface( display_, config_, window_, NULL );
    eglQuerySurface( display_, surface_, EGL_WIDTH, &screen_width_ );
    eglQuerySurface( display_, surface_, EGL_HEIGHT, &screen_height_ );

    if( screen_width_ != original_widhth || screen_height_ != original_height )
    {
        //Screen resized
        LOGI( "Screen resized" );
    }

    if( eglMakeCurrent( display_, surface_, surface_, context_ ) == EGL_TRUE )
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

void CEGuiEGLSharedBase::Suspend()
{
    if( surface_ != EGL_NO_SURFACE )
    {
        eglDestroySurface( display_, surface_ );
        surface_ = EGL_NO_SURFACE;
    }
}

bool CEGuiEGLSharedBase::Invalidate()
{
    Terminate();

    egl_context_initialized_ = false;
    return true;
}

bool CEGuiEGLSharedBase::CheckExtension( const char* extension )
{
    if( extension == NULL )
        return false;

    std::string extensions = std::string( (char*) glGetString( GL_EXTENSIONS ) );
    std::string str = std::string( extension );
    str.append( " " );

    size_t pos = 0;
    if( extensions.find( extension, pos ) != std::string::npos )
    {
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------//

