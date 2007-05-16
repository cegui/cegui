/***********************************************************************
    filename:   CEGuiSample.h
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
#ifndef _CEGuiSample_h_
#define _CEGuiSample_h_

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined (CEGUI_STATIC)
#   ifdef CEGUISAMPLE_EXPORTS
#       define CEGUISAMPLE_API __declspec(dllexport)
#   else
#       define CEGUISAMPLE_API __declspec(dllimport)
#   endif
#else
#       define CEGUISAMPLE_API
#endif


// forward declarations
class CEGuiBaseApplication;
class CEGuiRendererSelector;


/*!
\brief
    This is a base class that is intended to be used for all sample applications.
    Here we take care of common things such the renderer selection and application
    startup.
*/
class CEGUISAMPLE_API CEGuiSample
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiSample();


    /*!
    \brief
        Destructor.
    */
    virtual ~CEGuiSample();


    /*!
    \brief
        Application entry point.

    \return
        code to be returned by the application.
    */
    int run();


    /*!
    \brief
        Sample specific initialisation goes here.  This method is called by the application base object created
        as part of the initialise call.

    \return
        false if something went wrong.
    */
    virtual bool initialiseSample()  = 0;


    /*!
    \brief
        Cleans up resources allocated in the initialiseSample call.
    */
    virtual void cleanupSample() = 0;


protected:
    /*!
    \brief
        Initialises the sample system, this includes asking the user for a render to use and
        the subsequent creation of the required systems to support that renderer.

    \return
        false if anything went wrong.
    */
    virtual bool initialise();


    /*!
    \brief
        Cleans up all resources allocated by the initialise call.
    */
    virtual void cleanup();


    /*!
    \brief
        Output a message to the user in some OS independant way.
    */
    void outputExceptionMessage(const char* message) const;


    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGuiRendererSelector*  d_rendererSelector;     //!< Points to the renderer selector object.
    CEGuiBaseApplication*   d_sampleApp;            //!< Pointer to the base application object.
};

#endif  // end of guard _CEGuiSample_h_
