/************************************************************************
    filename:   CEGuiD3D81BaseApplication.h
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
#ifndef _CEGuiD3D81BaseApplication_h_
#define _CEGuiD3D81BaseApplication_h_

#include "CEGuiBaseApplication.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>

// undefine Microsoft macro evilness
#   undef min
#   undef max
#endif


struct CEGuiD3D81BaseApplicationImpl;


class CEGuiD3D81BaseApplication : public CEGuiBaseApplication
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiD3D81BaseApplication();


    /*!
    \brief
        Destructor.
    */
    ~CEGuiD3D81BaseApplication();


    // Implementation of base class abstract methods.
    bool execute(CEGuiSample* sampleApp);
    void cleanup();


protected:
    /*************************************************************************
        Implementation methods
    *************************************************************************/
    /*!
    \brief
        Initialise Direct3D system.
    */
    bool initialiseDirect3D(unsigned int width, unsigned int height, unsigned int adapter, bool windowed);

    /*!
    \brief
        Do reset of Direct3D device

    \return
        - true if the device was reset successfully
        - false if the device was not reset.
    */
    bool resetDirect3D(void);


    void updateFPS(void)
    {
        ++d_frames;

        DWORD thisTime = GetTickCount();

        if (thisTime - d_lastTime >= 1000)
        {
            d_FPS = d_frames;
            d_frames = 0;
            d_lastTime = thisTime;
        }

    }


    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGuiD3D81BaseApplicationImpl* pimpl;

    // FPS stuff
    DWORD   d_lastTime;
    int d_frames;
    int d_FPS;
};

#endif  // end of guard _CEGuiD3D81BaseApplication_h_
