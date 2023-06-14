/***********************************************************************
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
#include "CEGUI/RendererModules/OpenGL/RenderTarget.h"
#include "CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{

    
OpenGLRenderTarget::OpenGLRenderTarget(OpenGLRendererBase& owner) :
    d_owner(owner)
{
}

OpenGLRenderTarget::~OpenGLRenderTarget()
{
}

void OpenGLRenderTarget::activate()
{
    glViewport(static_cast<GLsizei>(RenderTarget::d_area.left()),
               static_cast<GLsizei>(RenderTarget::d_area.top()),
               static_cast<GLsizei>(RenderTarget::d_area.getWidth()),
               static_cast<GLsizei>(RenderTarget::d_area.getHeight()));

    if (!RenderTarget::d_matrixValid)
        updateMatrix();

    d_owner.setViewProjectionMatrix(RenderTarget::d_matrix);

    RenderTarget::activate();
}


void OpenGLRenderTarget::updateMatrix() const
{
    RenderTarget::updateMatrix( RenderTarget::createViewProjMatrixForOpenGL() );
}

OpenGLRendererBase& OpenGLRenderTarget::getOwner()
{
    return d_owner;
}


} // End of  CEGUI namespace section
