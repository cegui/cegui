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
#ifndef _CEGuiD3D9BaseApplication_h_
#define _CEGuiD3D9BaseApplication_h_

#include "CEGuiBaseApplication.h"
#include "CEGUI/GeometryBuffer.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>

// undefine Microsoft macro evilness
#   undef min
#   undef max
#endif

struct CEGuiBaseApplication9Impl;


class CEGuiD3D9BaseApplication : public CEGuiBaseApplication
{
public:
    //! Constructor.
    CEGuiD3D9BaseApplication();

    //! Destructor.
    ~CEGuiD3D9BaseApplication();

    void destroyRenderer();

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

    // Implementation of base class abstract methods.
    void run();
    void destroyWindow();
    void beginRendering(const float elapsed);
    void endRendering();

    /*************************************************************************
        Data fields
    *************************************************************************/
    CEGuiBaseApplication9Impl* pimpl;

    // counter used to track elapsed time
    DWORD d_lastFrameTime;
};

#endif  // end of guard _CEGuiD3D9BaseApplication_h_
