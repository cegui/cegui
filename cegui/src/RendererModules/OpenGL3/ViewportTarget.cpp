/***********************************************************************
    filename:   CEGUIOpenGLViewportTarget.cpp
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
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
#include "CEGUI/RendererModules/OpenGL3/ViewportTarget.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/Exceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGL3ViewportTarget::OpenGL3ViewportTarget(OpenGL3Renderer& owner) :
    OpenGL3RenderTarget(owner)
{
    // viewport area defaults to whatever the current OpenGL viewport is set to
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);

    Rectf init_area(Vector2f(static_cast<float>(vp[0]), static_cast<float>(vp[1])),
                        Sizef(static_cast<float>(vp[2]), static_cast<float>(vp[3])));
    setArea(init_area);
}

//----------------------------------------------------------------------------//
OpenGL3ViewportTarget::OpenGL3ViewportTarget(OpenGL3Renderer& owner,
    const Rectf& area) :
        OpenGL3RenderTarget(owner)
{
    setArea(area);
}

//----------------------------------------------------------------------------//
bool OpenGL3ViewportTarget::isImageryCache() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of base class
#include "./RenderTarget.inl"

