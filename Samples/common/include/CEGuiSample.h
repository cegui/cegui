/************************************************************************
    filename:   CEGuiSample.h
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
#ifndef _CEGuiSample_h_
#define _CEGuiSample_h_

// forward declarations
class CEGuiBaseApplication;
class CEGuiRendererSelector;


/*!
\brief
    This is a base class that is intended to be used for all sample applications.
    Here we take care of common things such the renderer selection and application
    startup.
*/
class CEGuiSample
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


    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGuiRendererSelector*  d_rendererSelector;     //!< Points to the renderer selector object.
    CEGuiBaseApplication*   d_sampleApp;            //!< Pointer to the base application object.
};

#endif  // end of guard _CEGuiSample_h_
