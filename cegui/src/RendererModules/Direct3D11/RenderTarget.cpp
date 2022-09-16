/***********************************************************************
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Direct3D11/RenderTarget.h"
#include "CEGUI/RendererModules/Direct3D11/GeometryBuffer.h"

#include <glm/gtc/matrix_transform.hpp>

#include <math.h>

// Start of CEGUI namespace section
namespace CEGUI
{

Direct3D11RenderTarget::Direct3D11RenderTarget(Direct3D11Renderer& owner) :
    d_owner(owner),
    d_device(*d_owner.getDirect3DDevice()),
    d_deviceContext(*d_owner.getDirect3DDeviceContext())
{
}

void Direct3D11RenderTarget::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    D3D11_VIEWPORT vp;
    setupViewport(vp);
    d_deviceContext.RSSetViewports(1, &vp);

    d_owner.setViewProjectionMatrix(RenderTarget::d_matrix);

    RenderTarget::activate();
}

void Direct3D11RenderTarget::updateMatrix() const
{
    RenderTarget::updateMatrix( RenderTarget::createViewProjMatrixForDirect3D() );
}

void Direct3D11RenderTarget::setupViewport(D3D11_VIEWPORT& vp) const
{
    vp.TopLeftX = static_cast<FLOAT>(d_area.left());
    vp.TopLeftY = static_cast<FLOAT>(d_area.top());
    vp.Width = static_cast<FLOAT>(d_area.getWidth());
    vp.Height = static_cast<FLOAT>(d_area.getHeight());
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
}

Direct3D11Renderer& Direct3D11RenderTarget::getOwner()
{
    return d_owner;
}

} // End of  CEGUI namespace section
