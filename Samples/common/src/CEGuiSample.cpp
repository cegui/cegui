/***********************************************************************
    filename:   CEGuiSample.cpp
    created:    24/9/2004
    author:     Paul D Turner
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
#include "CEGuiSample.h"

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif
#include "CEGUISamplesConfig.h"

// includes for renderer selector classes
#if defined( __WIN32__ ) || defined( _WIN32 )
#   include "Win32CEGuiRendererSelector.h"
#elif defined(__linux__)
#   ifdef CEGUI_SAMPLES_USE_GTK2
#       include "GTK2CEGuiRendererSelector.h"
#   else
#       include "CLICEGuiRendererSelector.h"
#   endif
#elif defined(__APPLE__)
#   include "MacCEGuiRendererSelector.h"
#endif

// includes for application types
#ifdef CEGUI_SAMPLES_USE_OGRE
#   include "CEGuiOgreBaseApplication.h"
#endif
#ifdef CEGUI_SAMPLES_USE_OPENGL
#   include "CEGuiOpenGLBaseApplication.h"
#endif
#ifdef CEGUI_SAMPLES_USE_IRRLICHT
#   include "CEGuiIrrlichtBaseApplication.h"
#endif
#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUI_SAMPLES_USE_DIRECTX_8
#       include "CEGuiD3D81BaseApplication.h"
#   endif
#   ifdef CEGUI_SAMPLES_USE_DIRECTX_9
#       include "CEGuiD3D9BaseApplication.h"
#   endif
#endif
// now we include the base CEGuiBaseApplication just in case someone has managed to
// get this far without any of the renderers.  This ensures the framework will build,
// although there will be no renderers available for selection in the samples.
#include "CEGuiBaseApplication.h"

#include "CEGUI.h"

#ifdef CEGUI_WITH_XERCES
#   include "CEGUIXercesParser.h"
#endif

// Include iostream if not on windows.
#if defined( __WIN32__ ) || defined( _WIN32 )
#else
#    include <iostream>
#endif


/*************************************************************************
    Constructor
*************************************************************************/
CEGuiSample::CEGuiSample() :
        d_rendererSelector(0),
        d_sampleApp(0)
{}


/*************************************************************************
    Destructor
*************************************************************************/
CEGuiSample::~CEGuiSample()
{
    if (d_sampleApp)
    {
        d_sampleApp->cleanup();
        delete d_sampleApp;
    }

    if (d_rendererSelector)
    {
        delete d_rendererSelector;
    }

}


/*************************************************************************
    Application entry point
*************************************************************************/
int CEGuiSample::run()
{
    try
    {
        if (initialise())
            cleanup();
    }
    catch (CEGUI::Exception& exc)
    {
        outputExceptionMessage(exc.getMessage().c_str());
    }
    catch (std::exception& exc)
    {
        outputExceptionMessage(exc.what());
    }
    catch(...)
    {
        outputExceptionMessage("Unknown exception was caught!");
    }

    return 0;
}


/*************************************************************************
    Initialise the sample application
*************************************************************************/
bool CEGuiSample::initialise()
{
    // Setup renderer selection dialog for Win32
#if defined( __WIN32__ ) || defined( _WIN32 )
    d_rendererSelector = new Win32CEGuiRendererSelector;

    // enable renderer types supported for Win32
#ifdef CEGUI_SAMPLES_USE_DIRECTX_8
    d_rendererSelector->setRendererAvailability(Direct3D81GuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_USE_DIRECTX_9
    d_rendererSelector->setRendererAvailability(Direct3D9GuiRendererType);
#endif

#elif defined(__linux__)
    // decide which method to use for renderer selection
#   ifdef CEGUI_SAMPLES_USE_GTK2
        d_rendererSelector = new GTK2CEGuiRendererSelector();
#   else
        d_rendererSelector = new CLICEGuiRendererSelector();
#   endif

#elif defined(__APPLE__)
     d_rendererSelector = new MacCEGuiRendererSelector();
#endif

    // enable available renderer types
#ifdef CEGUI_SAMPLES_USE_OGRE
    d_rendererSelector->setRendererAvailability(OgreGuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_USE_OPENGL
    d_rendererSelector->setRendererAvailability(OpenGLGuiRendererType);
#endif
#ifdef CEGUI_SAMPLES_USE_IRRLICHT
    d_rendererSelector->setRendererAvailability(IrrlichtGuiRendererType);
#endif

    // get selection from user
    if (d_rendererSelector->inkokeDialog())
    {
        // create appropriate application type based upon users selection
        switch(d_rendererSelector->getSelectedRendererType())
        {
#ifdef CEGUI_SAMPLES_USE_OGRE
        case OgreGuiRendererType:
            d_sampleApp = new CEGuiOgreBaseApplication();
            break;
#endif
#if defined( __WIN32__ ) || defined( _WIN32 )
#ifdef CEGUI_SAMPLES_USE_DIRECTX_8
        case Direct3D81GuiRendererType:
            d_sampleApp = new CEGuiD3D81BaseApplication();
            break;
#endif
#ifdef CEGUI_SAMPLES_USE_DIRECTX_9
        case Direct3D9GuiRendererType:
            d_sampleApp = new CEGuiD3D9BaseApplication();
            break;
#endif // DX9
#endif // Win32
#ifdef CEGUI_SAMPLES_USE_OPENGL
        case OpenGLGuiRendererType:
            d_sampleApp = new CEGuiOpenGLBaseApplication();
            break;
#endif
#ifdef CEGUI_SAMPLES_USE_IRRLICHT
        case IrrlichtGuiRendererType:
            d_sampleApp = new CEGuiIrrlichtBaseApplication();
            break;
#endif

        default:
            throw CEGUI::GenericException("No renderer was selected!");
            break;
        }

        // set the default resource groups to be used
        CEGUI::Imageset::setDefaultResourceGroup("imagesets");
        CEGUI::Font::setDefaultResourceGroup("fonts");
        CEGUI::Scheme::setDefaultResourceGroup("schemes");
        CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeels");
        CEGUI::WindowManager::setDefaultResourceGroup("layouts");
        CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");
#ifdef CEGUI_WITH_XERCES
        CEGUI::XercesParser::setSchemaDefaultResourceGroup("schemas");
#endif

        // execute the base application (which sets up the demo via 'this' and runs it.
        if (d_sampleApp->execute(this))
        {
            // signal that app initialised and ran
            return true;
        }

        // sample app did not initialise, delete the object.
        delete d_sampleApp;
        d_sampleApp = 0;
    }

    // delete renderer selector object
    delete d_rendererSelector;
    d_rendererSelector = 0;

    // signal app did not initialise and run.
    return false;
}


/*************************************************************************
    Cleanup the sample application.
*************************************************************************/
void CEGuiSample::cleanup()
{
    if (d_sampleApp)
    {
        d_sampleApp->cleanup();
        delete d_sampleApp;
        d_sampleApp = 0;
    }

    if (d_rendererSelector)
    {
        delete d_rendererSelector;
        d_rendererSelector = 0;
    }

}


/*************************************************************************
    Output a message to the user in some OS independant way.
*************************************************************************/
void CEGuiSample::outputExceptionMessage(const char* message) const
{
#if defined(__WIN32__) || defined(_WIN32)
    MessageBoxA(0, message, "CEGUI - Exception", MB_OK|MB_ICONERROR);
#else
    std::cout << "An exception was thrown within the sample framework:" << std::endl;
    std::cout << message << std::endl;
#endif
}
