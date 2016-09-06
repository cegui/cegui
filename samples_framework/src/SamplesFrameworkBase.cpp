/***********************************************************************
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "SamplesFrameworkBase.h"

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif
#include "CEGUISamplesConfig.h"

// includes for renderer selector classes
#if defined( __WIN32__ ) || defined( _WIN32 )
#   include "Win32CEGuiRendererSelector.h"
#elif defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__)  || defined(__HAIKU__)
#   ifdef CEGUI_SAMPLES_USE_GTK2
#       include "GTK2CEGuiRendererSelector.h"
#   else
#       include "CLICEGuiRendererSelector.h"
#   endif
#elif defined(__APPLE__)
#   include "MacCEGuiRendererSelector.h"
#endif

#ifdef __ANDROID__

#include <android/log.h>

#   ifdef CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2
#       include "CEGuiAndroidOpenglEs2BaseApplication.h"
#   endif

#else

// includes for application types
#ifdef CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE
#   include "CEGuiOgreBaseApplication.h"
#endif
#ifdef CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE
#   ifdef CEGUI_BUILD_RENDERER_OPENGL
#      include "CEGuiOpenGLBaseApplication.h"
#   endif
#   ifdef CEGUI_BUILD_RENDERER_OPENGL3
#      include "CEGuiOpenGL3BaseApplication.h"
#   endif
#   ifdef CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2
#      include "CEGuiOpenGLES2BaseApplication.h"
#   endif
#endif
#ifdef CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE
#   include "CEGuiIrrlichtBaseApplication.h"
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE
#   include "CEGuiDirectFBBaseApplication.h"
#endif

#endif

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE
#       include "CEGuiD3D9BaseApplication.h"
#   endif
#   ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE
#       include "CEGuiD3D10BaseApplication.h"
#   endif
#   ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE
#       include "CEGuiD3D11BaseApplication.h"
#   endif
#endif

// now we include the base CEGuiBaseApplication just in case someone has managed to
// get this far without any of the renderers.  This ensures the framework will build,
// although there will be no renderers available for selection in the samples.
#include "CEGuiBaseApplication.h"

#include "CEGUI/CEGUI.h"

// Include iostream if not on windows.
#if !defined( __WIN32__ ) && !defined( _WIN32 )
#   include <iostream>
#endif

/*************************************************************************
    Constructor
*************************************************************************/
SamplesFrameworkBase::SamplesFrameworkBase() :
        d_rendererSelector(0),
        d_baseApp(0),
        d_quitting(false),
        d_appWindowWidth(0),
        d_appWindowHeight(0)
{}


/*************************************************************************
    Destructor
*************************************************************************/
SamplesFrameworkBase::~SamplesFrameworkBase()
{
    if (d_baseApp)
    {
        d_baseApp->cleanup();
        delete d_baseApp;
    }

    if (d_rendererSelector)
    {
        delete d_rendererSelector;
    }

}

/*************************************************************************
    Application entry point
*************************************************************************/
int SamplesFrameworkBase::run(const CEGUI::String& dataPathPrefixOverride)
{
    CEGUI_TRY
    {
        if (initialise("CEGUI.log", dataPathPrefixOverride))
            d_baseApp->run();
        cleanup();
    }
    CEGUI_CATCH(const std::exception& exc)
    {
        outputExceptionMessage(exc.what());
    }
    CEGUI_CATCH(const char* exc)
    {
        SamplesFrameworkBase::outputExceptionMessage(exc);
    }
    CEGUI_CATCH(...)
    {
        SamplesFrameworkBase::outputExceptionMessage
          ("Unknown exception was caught!");
    }
    return 0;
}

/*************************************************************************
    Start the SamplesFramework application
*************************************************************************/
bool SamplesFrameworkBase::initialise
  (const CEGUI::String& logFile, const CEGUI::String& dataPathPrefixOverride)
{
    // Setup renderer selection dialog for Win32
#if defined( __WIN32__ ) || defined( _WIN32 )
    d_rendererSelector = new Win32CEGuiRendererSelector;

    // enable renderer types supported for Win32
#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE
    d_rendererSelector->setRendererAvailability(Direct3D9GuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE
    d_rendererSelector->setRendererAvailability(Direct3D10GuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE
    d_rendererSelector->setRendererAvailability(Direct3D11GuiRendererType);
#endif

#elif defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
    // decide which method to use for renderer selection
#   ifdef CEGUI_SAMPLES_USE_GTK2
        d_rendererSelector = new GTK2CEGuiRendererSelector();
#   else
        d_rendererSelector = new CLICEGuiRendererSelector();
#   endif

#elif defined(__APPLE__)
     d_rendererSelector = new MacCEGuiRendererSelector();
#endif

#if defined __ANDROID__

#   if defined CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE
#       if defined CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2
            d_baseApp = new CEGuiAndroidOpenglEs2BaseApplication();
#       endif
#   endif

#else

    // enable available renderer types
#ifdef CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE
    d_rendererSelector->setRendererAvailability(OgreGuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE
    #ifdef CEGUI_BUILD_RENDERER_OPENGL
        d_rendererSelector->setRendererAvailability(OpenGLGuiRendererType);
    #endif
    #ifdef CEGUI_BUILD_RENDERER_OPENGL3
        d_rendererSelector->setRendererAvailability(OpenGL3GuiRendererType);
    #endif
    #ifdef CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2
	d_rendererSelector->setRendererAvailability(OpenglEs2GuiRendererType);
    #endif
#endif
#ifdef CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE
    d_rendererSelector->setRendererAvailability(IrrlichtGuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE
    d_rendererSelector->setRendererAvailability(DirectFBGuiRendererType);
#endif

    // get selection from user
    if (d_rendererSelector->invokeDialog())
    {
        // create appropriate application type based upon users selection
        switch (d_rendererSelector->getSelectedRendererType())
        {
#ifdef CEGUI_SAMPLES_RENDERER_OGRE_ACTIVE
        case OgreGuiRendererType:
            {
                CEGuiOgreBaseApplication* ogreBaseApp = new CEGuiOgreBaseApplication();
                if(!ogreBaseApp->isInitialised())
                    return false;

                d_baseApp = ogreBaseApp;
            }
            break;
#endif
#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D9_ACTIVE
        case Direct3D9GuiRendererType:
            d_baseApp = new CEGuiD3D9BaseApplication();
            break;
#   endif // DX9
#   ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D10_ACTIVE
        case Direct3D10GuiRendererType:
            d_baseApp = new CEGuiD3D10BaseApplication();
            break;
#   endif // DX10
#   ifdef CEGUI_SAMPLES_RENDERER_DIRECT3D11_ACTIVE
        case Direct3D11GuiRendererType:
            d_baseApp = new CEGuiD3D11BaseApplication();
            break;
#   endif // DX11
#endif // Win32
#ifdef CEGUI_SAMPLES_RENDERER_OPENGL_ACTIVE
#   ifdef CEGUI_BUILD_RENDERER_OPENGL
                case OpenGLGuiRendererType:
                    d_baseApp = new CEGuiOpenGLBaseApplication();
                    break;
#   endif
#   ifdef CEGUI_BUILD_RENDERER_OPENGL3
                case OpenGL3GuiRendererType:
                    d_baseApp = new CEGuiOpenGL3BaseApplication();
                    break;
#   endif
#   ifdef CEGUI_SAMPLES_SUPPORT_RENDERER_OPENGLES2
                case OpenglEs2GuiRendererType:
#       if defined __ANDROID__
                    d_baseApp = new CEGuiAndroidOpenglEs2BaseApplication();
#       else
                    d_baseApp = new CEGuiOpenGLES2BaseApplication();
#       endif
                    break;
#   endif
#endif
#ifdef CEGUI_SAMPLES_RENDERER_IRRLICHT_ACTIVE
        case IrrlichtGuiRendererType:
            d_baseApp = new CEGuiIrrlichtBaseApplication();
            break;
#endif
#ifdef CEGUI_SAMPLES_RENDERER_DIRECTFB_ACTIVE
        case DirectFBGuiRendererType:
            d_baseApp = new CEGuiDirectFBBaseApplication();
            break;
#endif

        default:  break;
        }

    }

    delete d_rendererSelector;
    d_rendererSelector = 0;

#endif

    if (!d_baseApp)
        CEGUI_THROW(CEGUI::GenericException("No renderer was selected!"));

    return d_baseApp->init(this, logFile, dataPathPrefixOverride);
}

void SamplesFrameworkBase::cleanup()
{
    if (d_baseApp)
    {
        d_baseApp->cleanup();
        delete d_baseApp;
        d_baseApp = 0;
    }
}

void SamplesFrameworkBase::renderSingleFrame(float elapsed)
{
    d_baseApp->renderSingleFrame(elapsed);
}

/*************************************************************************
    Output a message to the user in some OS independant way.
*************************************************************************/
void SamplesFrameworkBase::outputExceptionMessage(const char* message)
{
#if defined(__WIN32__) || defined(_WIN32)
    MessageBoxA(0, message, "CEGUI - Exception", MB_OK|MB_ICONERROR);
#elif defined(__ANDROID__)
    __android_log_write (ANDROID_LOG_ERROR, "CEGUISampleFramework_NDK",
      "An exception was thrown within the sample framework:");
    __android_log_write (ANDROID_LOG_ERROR, "CEGUISampleFramework_NDK",
      message);
#else
    std::cout << "An exception was thrown within the sample framework:" << std::endl;
    std::cout << message << std::endl;
#endif
}

void SamplesFrameworkBase::setQuitting(bool quit)
{
    d_quitting = quit;
}

bool SamplesFrameworkBase::isQuitting()
{
    return d_quitting;
}

void SamplesFrameworkBase::setApplicationWindowSize(int width, int height)
{
    d_appWindowWidth = width;
    d_appWindowHeight = height;
}
