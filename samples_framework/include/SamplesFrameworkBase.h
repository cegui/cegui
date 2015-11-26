/***********************************************************************
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
#ifndef _SamplesFrameworkBase_h_
#define _SamplesFrameworkBase_h_

// forward declarations
class CEGuiBaseApplication;
class CEGuiRendererSelector;

#include "CEGUI/InputEvent.h"
#include "CEGUI/Exceptions.h"

/*!
\brief
    This is a base class that is intended to be used for all sample applications.
    Here we take care of common things such the renderer selection and application
    startup.
*/
class SamplesFrameworkBase
{
public:
    /*!
    \brief
        Constructor.
    */
    SamplesFrameworkBase();


    /*!
    \brief
        Destructor.
    */
    virtual ~SamplesFrameworkBase();

    /*!
    \brief
        Application entry point.

    \return
        code to be returned by the application.
    */
    int run(const CEGUI::String& dataPathPrefixOverride);

    /*!
    \brief
        Initialises the sample system, this includes asking the user for a render to use and
        the subsequent creation of the required systems to support that renderer.

    \return
        false if anything went wrong.
    */
    virtual bool initialise(const CEGUI::String& logFile,
                            const CEGUI::String& dataPathPrefixOverride);
    
    virtual void cleanup();

    /*!
    \brief
        Update function called before rendering
    */
    virtual void update(float passedTime) = 0;

    /*!
    \brief
    Update function for window size changes
    */
    virtual void handleNewWindowSize(float width, float height) = 0;

    void renderSingleFrame(float elapsed);

    /*!
    \brief
    Draw function to draw GUIContexts
    */
    virtual void renderGUIContexts() = 0;

    /*!
    \brief
    Function to inject key down to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectKeyDown(const CEGUI::Key::Scan& ceguiKey) = 0;

    /*!
    \brief
    Function to inject key up to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectKeyUp(const CEGUI::Key::Scan& ceguiKey) = 0;

    /*!
    \brief
    Function to inject characters to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectChar(int character) = 0;

    /*!
    \brief
    Function to inject mouse button down to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectMouseButtonDown(const CEGUI::MouseButton& ceguiMouseButton) = 0;

    /*!
    \brief
    Function to inject mouse button up to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectMouseButtonUp(const CEGUI::MouseButton& ceguiMouseButton) = 0;

    /*!
    \brief
    Function to inject mouse wheel changes to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectMouseWheelChange(float position) = 0;

    /*!
    \brief
    Function to inject the mouse position to GUIContexts
    \return
    true if event was handled.
    */
    virtual bool injectMousePosition(float x, float y) = 0;

    /*!
    \brief
    Function to set the bool defining if the application should quit as soon as possible
    */
    virtual void setQuitting(bool quit);

    /*!
    \brief
    Function returning if the application should quit as soon as possible
    \return
    true if should quit.
    */
    bool isQuitting();

    /*!
    \brief
    Function setting the application window's size
    */
    void setApplicationWindowSize(int width, int height);

    /*!
    \brief
        Output a message to the user in some OS independant way.
    */
    void outputExceptionMessage(const char* message);

protected:

    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGuiRendererSelector*  d_rendererSelector;     //!< Points to the renderer selector object.
    CEGuiBaseApplication*   d_baseApp;            //!< Pointer to the base application object.

    bool                    d_quitting;              //!< Bool defining if application should quit.

    int                     d_appWindowWidth;            //!< Int defining the application window's width.
    int                     d_appWindowHeight;           //!< Int defining the application window's height.
};

#endif  // end of guard _SamplesFrameworkBase_h_
