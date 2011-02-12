/***********************************************************************
    filename:   CEGuiD3D10BaseApplication.h
    created:    Sun May 25 2008
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
#ifndef _CEGuiD3D10BaseApplication_h_
#define _CEGuiD3D10BaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "CEGUIGeometryBuffer.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <windows.h>
#endif

// MSVC auto-linking
#if defined(_MSC_VER)
#   pragma comment(lib, "d3d10.lib")
#   if defined(_DEBUG)
#       if defined(CEGUI_STATIC)
#           pragma comment(lib, "CEGUIDirect3D10Renderer_Static_d.lib")
#       else
#           pragma comment(lib, "CEGUIDirect3D10Renderer_d.lib")
#       endif
#   else
#       if defined(CEGUI_STATIC)
#           pragma comment(lib, "CEGUIDirect3D10Renderer_Static.lib")
#       else
#           pragma comment(lib, "CEGUIDirect3D10Renderer.lib")
#       endif
#   endif
#endif

// forward declare struct that will hold D3D10 specifics
struct CEGuiBaseApplicationImpl;

//! CEGuiBaseApplication implementation for Microsoft Direct3D 10
class CEGuiD3D10BaseApplication : public CEGuiBaseApplication
{
public:
    //!Constructor.
    CEGuiD3D10BaseApplication();

    //! Destructor.
    ~CEGuiD3D10BaseApplication();

protected:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    //! Initialises Direct3D system.
    bool initialiseDirect3D(unsigned int width, unsigned int height,
                            bool windowed);

    //! cleanup D3D objects / interfaces
    void cleanupDirect3D();

    // Implementation of base class abstract methods.
    bool execute_impl(CEGuiSample* sampleApp);
    void cleanup_impl();
    void beginRendering(const float elapsed);
    void endRendering();

    /*************************************************************************
        Data fields
    *************************************************************************/
    //! Pointer to the struct holding D3D specific fields.
    CEGuiBaseApplicationImpl* pimpl;;

    // counter used to track elapsed time
    DWORD d_lastFrameTime;
};

#endif  // end of guard _CEGuiD3D10BaseApplication_h_
