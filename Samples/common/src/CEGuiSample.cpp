/************************************************************************
    filename:   CEGuiSample.cpp
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "CEGuiSample.h"

#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

// includes for renderer selector classes
#if defined( __WIN32__ ) || defined( _WIN32 )
#   include "Win32CEGuiRendererSelector.h"
#elif defined(linux)
#   ifdef CEGUI_SAMPLES_USE_GTK2
#       include "GTK2CEGuiRendererSelector.h"
#   else
#       include "CLICEGuiRendererSelector.h"
#   endif
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
#   include "CEGuiD3D81BaseApplication.h"
#   include "CEGuiD3D9BaseApplication.h"
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
    if (initialise())
    {
        cleanup();
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
    d_rendererSelector->setRendererAvailability(OgreGuiRendererType);
    d_rendererSelector->setRendererAvailability(Direct3D81GuiRendererType);
    d_rendererSelector->setRendererAvailability(Direct3D9GuiRendererType);
    d_rendererSelector->setRendererAvailability(OpenGLGuiRendererType);

    // Setup renderer selection dialog for linux
#elif defined(linux)
    // decide which method to use for renderer selection
#   ifdef CEGUI_SAMPLES_USE_GTK2
        d_rendererSelector = new GTK2CEGuiRendererSelector();
#   else
        d_rendererSelector = new CLICEGuiRendererSelector();
#   endif

    // enable available renderer types for linux
#   ifdef CEGUI_SAMPLES_USE_OGRE
        d_rendererSelector->setRendererAvailability(OgreGuiRendererType);
#   endif
#   ifdef CEGUI_SAMPLES_USE_OPENGL
        d_rendererSelector->setRendererAvailability(OpenGLGuiRendererType);
#   endif
#   ifdef CEGUI_SAMPLES_USE_IRRLICHT
        d_rendererSelector->setRendererAvailability(IrrlichtGuiRendererType);
#   endif
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
        case Direct3D81GuiRendererType:
            d_sampleApp = new CEGuiD3D81BaseApplication();
            break;

        case Direct3D9GuiRendererType:
            d_sampleApp = new CEGuiD3D9BaseApplication();
            break;
#endif
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
            // TODO: Throw exception or something!
            break;
        }

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
