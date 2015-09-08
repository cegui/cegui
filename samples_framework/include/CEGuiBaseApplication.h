/***********************************************************************
    created:    24/9/2004
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGuiBaseApplication_h_
#define _CEGuiBaseApplication_h_

#include "CEGUI/String.h"

// If this looks wanky, it's becase it is!  Behold that this is not as fullblown
// as it could be though.
#ifndef PATH_MAX
#   include <stdlib.h>
#   ifndef PATH_MAX
#       include <limits.h>
#   endif
#   ifndef PATH_MAX
#      ifdef _MAX_PATH
#           define PATH_MAX _MAX_PATH
#       else
#           define PATH_MAX 260
#       endif
#   endif
#endif

/*************************************************************************
    Forward refs
*************************************************************************/
class SamplesFrameworkBase;
namespace CEGUI
{
class Renderer;
class ImageCodec;
class ResourceProvider;
class GeometryBuffer;
class EventArgs;
class GUIContext;
}

/*!
\brief
    Base application abstract base class.

    The "BaseApplication" family of classes are used to start up and run a
    host application for CeGui samples in a consistent manner.
*/
class CEGuiBaseApplication
{
public:
    //! Constructor.
    CEGuiBaseApplication();

    //! Destructor
    virtual ~CEGuiBaseApplication();

    /*!
    \brief
        Initialise the base application.

        This will fully initialise the application, finish initialisation of the
        demo via calls to 'sampleApp', and finally control execution of the
        sample.  This calls calls the virtual run function.

        Classes that override this method must first call the implementation of
        the superclass!

    \param sampleApp
        Pointer to the CEGuiSample object that the CEGuiBaseApplication is being
        invoked for.

    \return
        - true if the application initialised okay (cleanup function will be
          called).
    */
    virtual bool init(SamplesFrameworkBase* sampleApp,
                      const CEGUI::String& logFile,
                      const CEGUI::String& dataPathPrefixOverride);

    /*!
    \brief
        Performs any required cleanup of the base application system.

        Classes that override this method must, lastly, call the implementation
        of the superclass!
    */
    virtual void cleanup();

    virtual void destroyRenderer();

    /*!
    \brief
        Render a single display frame.  This should be called by subclasses to
        perform rendering.

        This function handles all per-frame updates, calls beginRendering, then
        renders the CEGUI output, and finally calls endRendering.

    \param elapsed
        Number of seconds elapsed since last frame.
    */
    void renderSingleFrame(const float elapsed);

    /* Returns if the renderer (DX or OpenGL) was selected and the initialisation
    was subsequently finished */
    bool isInitialised();

    /*!
     \brief
        Return the path prefix to use for datafiles.  The value returned
        is obtained via a environment variable named 'CEGUI_SAMPLE_DATAPATH'
        if the variable is not set, a default will be used depending on the
        build system in use.
        */
    CEGUI::String getDataPathPrefix() const { return d_dataPathPrefix; }

    void initDataPathPrefix(const CEGUI::String &override);

    /*!
    \brief
        Registers the overlay handler for rendering the FPS for a specified GUIContext
    */
    void registerSampleOverlayHandler(CEGUI::GUIContext* gui_context);

    //! The abstract function for running the application.
    virtual void run() {}

protected:
    //! name of env var that holds the path prefix to the data files.
    static const char DATAPATH_VAR_NAME[];

    //! The abstract function for destroying the renderer and the window.
    virtual void destroyWindow() = 0;
    //! Implementation function to perform required pre-render operations.
    virtual void beginRendering(const float elapsed) = 0;
    //! Implementation function to perform required post-render operations.
    virtual void endRendering() = 0;

    /*!
    \brief
        Setup standard sample resource group directory locations.  Default uses
        the CEGUI::DefaultResourceProvider - override if the sample base app
        being implemented uses something else!
    */
    virtual void initialiseResourceGroupDirectories
      (const CEGUI::String& dataPathPrefixOverride);

    //! initialise the standard default resource groups used by the samples.
    virtual void initialiseDefaultResourceGroups();

    //! function that updates the FPS rendering as needed.
    void updateFPS(const float elapsed);
    //! function that updates the logo rotation as needed.
    void updateLogo(const float elapsed);
    //! function that positions the logo in the correct place.
    void positionLogo();
    //! positions the FPS counter at the correct place.
    void positionFPS();
    //! event handler function that draws the logo and FPS overlay elements.
    bool sampleBrowserOverlayHandler(const CEGUI::EventArgs& args);
    //! event handler function that draws the FPS overlay elements.
    bool sampleOverlayHandler(const CEGUI::EventArgs& args);
    //! event handler function called when main view is resized
    bool resizeHandler(const CEGUI::EventArgs& args);

    //! SampleFramework base used in the application
    static SamplesFrameworkBase* d_sampleApp;
    //! The window width the application should get created with at start
    static const int s_defaultWindowWidth = 1280;
    //! The window height the application should get created with at start
    static const int s_defaultWindowHeight = 720;

    //! true when the base app should cleanup and exit.
    bool d_quitting;
    //! Renderer to use.  This MUST be set in the subclass constructor.
    CEGUI::Renderer* d_renderer;
    //! ImageCodec to use.  Set in subclass constructor, may be 0.
    CEGUI::ImageCodec* d_imageCodec;
    //! ResourceProvider to use.  Set in subclass constructor, may be 0.
    CEGUI::ResourceProvider* d_resourceProvider;
    //! GeometryBuffer used for drawing the spinning CEGUI logo
    CEGUI::GeometryBuffer* d_logoGeometry;
    //! GeometryBuffer used for drawing the FPS value.
    CEGUI::GeometryBuffer* d_FPSGeometry;
    //! Fraction of second elapsed (used for counting frames per second).
    float d_FPSElapsed;
    //! Number of frames drawn so far.
    int d_FPSFrames;
    //! Last changed FPS value.
    int d_FPSValue;
    //! whether to spin the logo
    bool d_spinLogo;

private:
    CEGUI::String d_dataPathPrefix;

};

#endif  // end of guard _CEGuiBaseApplication_h_
