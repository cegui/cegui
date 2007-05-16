/***********************************************************************
    filename:   CEGuiD3D9BaseApplication.h
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
#ifndef _CEGuiD3D9BaseApplication_h_
#define _CEGuiD3D9BaseApplication_h_

#include "CEGuiBaseApplication.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>

// undefine Microsoft macro evilness
#   undef min
#   undef max
#endif

//Updated for linking of static libs
#if defined(_WIN32)
#  pragma comment(lib, "d3d9.lib")	
#  if defined(_DEBUG)
#	if defined(CEGUI_STATIC)
#	   pragma comment(lib, "DirectX9GUIRenderer_Static_d.lib")
#	else
#      pragma comment(lib, "DirectX9GUIRenderer_d.lib")
#	endif
#  else
#	if defined(CEGUI_STATIC)
#	   pragma comment(lib, "DirectX9GUIRenderer_Static.lib")
#	else
#      pragma comment(lib, "DirectX9GUIRenderer.lib")
#	endif
#  endif
#endif


struct CEGuiBaseApplicationImpl;


class CEGuiD3D9BaseApplication : public CEGuiBaseApplication
{
public:
    /*!
    \brief
        Constructor.
    */
    CEGuiD3D9BaseApplication();


    /*!
    \brief
        Destructor.
    */
    ~CEGuiD3D9BaseApplication();

    // Implementation of base class abstract methods.
    bool execute(CEGuiSample* sampleApp);
    void cleanup();

protected:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    /*!
    \brief
        Initialises Direct3D system.
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
    CEGuiBaseApplicationImpl* pimpl;;

    // FPS stuff
    DWORD   d_lastTime;
    int d_frames;
    int d_FPS;
};

#endif  // end of guard _CEGuiD3D9BaseApplication_h_
