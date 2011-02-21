/***********************************************************************
    filename:   CEGUIDirect3D10RenderTarget.cpp
    created:    Wed May 5 2010
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#define NOMINMAX
#include "CEGUIDirect3D11RenderTarget.h"
#include "CEGUIDirect3D11GeometryBuffer.h"
#include "CEGUIRenderQueue.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
Direct3D11RenderTarget::Direct3D11RenderTarget(Direct3D11Renderer& owner) :
    d_owner(owner),
    d_device(d_owner.getDirect3DDevice()),
    d_area(0, 0, 0, 0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::setArea(const Rect<>& area)
{
    d_area = area;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
const Rect<>& Direct3D11RenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    D3D11_VIEWPORT vp;
    setupViewport(vp);
    d_device.d_context->RSSetViewports(1, &vp);

    d_owner.setProjectionMatrix(d_matrix);
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::deactivate()
{
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::unprojectPoint(const GeometryBuffer& buff,
                                            const Vector2<>& p_in,
                                            Vector2<>& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const Direct3D11GeometryBuffer& gb =
        static_cast<const Direct3D11GeometryBuffer&>(buff);

    D3D11_VIEWPORT vp_;
    setupViewport(vp_);

	//ToDo
	//ms didn't update their math for 11, so use 10
	D3D10_VIEWPORT vp;
	vp.Width=vp_.Width;
	vp.Height=vp_.Height;
	vp.TopLeftX=vp_.TopLeftX;
	vp.TopLeftY=vp_.TopLeftY;
	vp.MinDepth=vp_.MinDepth;
	vp.MaxDepth=vp_.MaxDepth;

    D3DXVECTOR3 in_vec;
    in_vec.z = 0.0f;

    // project points to create a plane orientated with GeometryBuffer's data
    D3DXVECTOR3 p1;
    D3DXVECTOR3 p2;
    D3DXVECTOR3 p3;
    in_vec.x = 0;
    in_vec.y = 0;
    D3DXVec3Project(&p1, &in_vec, &vp, &d_matrix, 0, gb.getMatrix()); 

    in_vec.x = 1;
    in_vec.y = 0;
    D3DXVec3Project(&p2, &in_vec, &vp, &d_matrix, 0, gb.getMatrix()); 

    in_vec.x = 0;
    in_vec.y = 1;
    D3DXVec3Project(&p3, &in_vec, &vp, &d_matrix, 0, gb.getMatrix()); 

    // create plane from projected points
    D3DXPLANE surface_plane;
    D3DXPlaneFromPoints(&surface_plane, &p1, &p2, &p3);

    // unproject ends of ray
    in_vec.x = vp.Width * 0.5f;
    in_vec.y = vp.Height * 0.5f;
    in_vec.z = -d_viewDistance;
    D3DXVECTOR3 t1;
    D3DXVec3Unproject(&t1, &in_vec, &vp, &d_matrix, 0, gb.getMatrix()); 

    in_vec.x = p_in.d_x;
    in_vec.y = p_in.d_y;
    in_vec.z = 0.0f;
    D3DXVECTOR3 t2;
    D3DXVec3Unproject(&t2, &in_vec, &vp, &d_matrix, 0, gb.getMatrix()); 

    // get intersection of ray and plane
    D3DXVECTOR3 intersect;
    D3DXPlaneIntersectLine(&intersect, &surface_plane, &t1, &t2);

    p_out.d_x = intersect.x;
    p_out.d_y = intersect.y;
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::updateMatrix() const
{
    const float fov = 0.523598776f;
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    d_viewDistance = midx / (aspect * 0.267949192431123f);

    D3DXVECTOR3 eye(midx, midy, -d_viewDistance);
    D3DXVECTOR3 at(midx, midy, 1);
    D3DXVECTOR3 up(0, -1, 0);

    D3DXMATRIX tmp;
    D3DXMatrixMultiply(&d_matrix,
        D3DXMatrixLookAtRH(&d_matrix, &eye, &at, &up),
        D3DXMatrixPerspectiveFovRH(&tmp, fov, aspect,
                                   d_viewDistance * 0.5f,
                                   d_viewDistance * 2.0f));

    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
void Direct3D11RenderTarget::setupViewport(D3D11_VIEWPORT& vp) const
{
    vp.TopLeftX = static_cast<FLOAT>(d_area.left());
    vp.TopLeftY = static_cast<FLOAT>(d_area.top());
    vp.Width = static_cast<FLOAT>(d_area.getWidth());
    vp.Height = static_cast<FLOAT>(d_area.getHeight());
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
