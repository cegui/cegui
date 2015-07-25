/***********************************************************************
    created:    Tue May 29 2012
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGuiD3D11BaseApplication_h_
#define _CEGuiD3D11BaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "CEGUI/GeometryBuffer.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif

// forward declare struct that will hold D3D11 specifics
struct CEGuiBaseApplication11Impl;

//! CEGuiBaseApplication implementation for Microsoft Direct3D 11
class CEGuiD3D11BaseApplication : public CEGuiBaseApplication
{
public:
    CEGuiD3D11BaseApplication();
    ~CEGuiD3D11BaseApplication();
    void destroyRenderer();

protected:
    bool initialiseDirect3D(unsigned int width, unsigned int height,
                            bool windowed);
    void cleanupDirect3D();

    // Implementation of base class.
    void run();
    void destroyWindow();
    void beginRendering(const float elapsed);
    void endRendering();

    //! Pointer to the struct holding D3D specific fields.
    CEGuiBaseApplication11Impl* pimpl;
    //! counter used to track elapsed time
    DWORD d_lastFrameTime;
};

#endif

