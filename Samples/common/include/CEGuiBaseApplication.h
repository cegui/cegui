/***********************************************************************
    filename:   CEGuiBaseApplication.h
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
#ifndef _CEGuiBaseApplication_h_
#define _CEGuiBaseApplication_h_

/*************************************************************************
    Forward refs
*************************************************************************/
class CEGuiSample;


/*!
\brief
    Base application abstract base class.
 
    The "BaseApplication" family of classes are used to start up and execute a host application for
    CeGui samples in a consistent manner.
*/
class CEGuiBaseApplication
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiBaseApplication() :
        d_quitting(false)
    { }


    /*!
    \brief
        Destructor
    */
    virtual ~CEGuiBaseApplication()
    { }


    /*!
    \brief
        Start the base application

        This will fully initialise the application, finish initialisation of the demo via calls to 'sampleApp', and finally
        control execution of the sample.

    \param sampleApp
        Pointer to the CEGuiSample object that the CEGuiBaseApplication is being invoked for.

    \return
        - true if the application initialised and ran okay (cleanup call is required).
        - false if the application failed to initialise (no cleanup call is required).
    */
    virtual bool execute(CEGuiSample* sampleApp) = 0;


    /*!
    \brief
        Performs any required cleanup of the base application system.
    */
    virtual void cleanup() = 0;


    /*!
    \brief
        Set whether the BaseApplication should clean up and exit.

    \param quit
        - true if the application should clean up and exit.

    \return
        Nothing.
    */
    virtual void setQuitting(bool quit = true)      { d_quitting = quit; }


    /*!
    \brief
        Return whether the app is currently set to quit.

    \return
        - true if the application will terminate at its earliest opportunity.
        - false if the application will keep running.
    */
    virtual bool isQuitting() const     { return d_quitting; }

protected:
    bool    d_quitting;     //!< true when the base app should cleanup and exit.
};

#endif  // end of guard _CEGuiBaseApplication_h_
