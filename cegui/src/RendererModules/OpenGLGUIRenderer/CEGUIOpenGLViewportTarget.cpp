/***********************************************************************
    filename:   CEGUIOpenGLViewportTarget.cpp
    created:    Tue Feb 12 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIOpenGLViewportTarget.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLViewportTarget::OpenGLViewportTarget()
{
    // viewport area defaults to whatever the current viewport is set to
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    d_area.d_left   = static_cast<float>(vp[0]);
    d_area.d_top    = static_cast<float>(vp[1]);
    d_area.d_right  = static_cast<float>(vp[2]);
    d_area.d_bottom = static_cast<float>(vp[3]);
}

//----------------------------------------------------------------------------//
OpenGLViewportTarget::OpenGLViewportTarget(const Rect& area)
{
    d_area = area;
}

//----------------------------------------------------------------------------//
OpenGLViewportTarget::~OpenGLViewportTarget()
{
}

//----------------------------------------------------------------------------//
void OpenGLViewportTarget::queueToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    throw InvalidRequestException("OpenGLViewportTarget::queueToTarget "
        "function is currently unimplemented.");
}

//----------------------------------------------------------------------------//
void OpenGLViewportTarget::renderToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    throw InvalidRequestException("OpenGLViewportTarget::renderToTarget "
        "function is currently unimplemented.");
}

//----------------------------------------------------------------------------//
void OpenGLViewportTarget::setArea(const Rect& area)
{
    d_area = area;
}

//----------------------------------------------------------------------------//
void OpenGLViewportTarget::declareRenderSize(const Size& sz)
{
    // do nothing for this
}

//----------------------------------------------------------------------------//
bool OpenGLViewportTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
