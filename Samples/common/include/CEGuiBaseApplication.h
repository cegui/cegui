/************************************************************************
    filename:   CEGuiBaseApplication.h
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
    CEGuiBaseApplication()
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
};

#endif  // end of guard _CEGuiBaseApplication_h_
