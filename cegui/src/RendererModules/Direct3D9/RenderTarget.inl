/***********************************************************************
    created:    Tue Feb 10 2009
    author:     Paul D Turner
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
#include "CEGUI/RendererModules/Direct3D9/RenderTarget.h"
#include "CEGUI/RendererModules/Direct3D9/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/Exceptions.h"
#ifdef _WIN32_WINNT_WIN8
#include <DirectXMath.h>
using namespace DirectX;
CEGUI_STATIC_ASSERT(sizeof(D3DMATRIX) == sizeof(XMFLOAT4X4));
#else
#include <d3dx9.h>
CEGUI_STATIC_ASSERT(sizeof(D3DMATRIX) == sizeof(D3DXMATRIX));
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
template <typename T>
Direct3D9RenderTarget<T>::Direct3D9RenderTarget(Direct3D9Renderer& owner) :
    d_owner(owner),
    d_device(owner.getDevice()),
    d_area(0, 0, 0, 0),
    d_matrixValid(false),
    d_viewDistance(0)
{
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
template <typename T>
const Rectf& Direct3D9RenderTarget<T>::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    D3DVIEWPORT9 vp;
    setupViewport(vp);
    d_device->SetViewport(&vp);

    d_owner.getDevice()->SetTransform(D3DTS_PROJECTION, &d_matrix);
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::deactivate()
{
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::unprojectPoint(const GeometryBuffer& buff,
                                           const Vector2f& p_in,
                                           Vector2f& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const Direct3D9GeometryBuffer& gb =
        static_cast<const Direct3D9GeometryBuffer&>(buff);

    D3DVIEWPORT9 vp;
    setupViewport(vp);

#ifdef DIRECTX_MATH_VERSION
    float viewportX = static_cast<float>(vp.X);
    float viewportY = static_cast<float>(vp.Y);
    float viewportWidth = static_cast<float>(vp.Width);
    float viewportHeight = static_cast<float>(vp.Height);
    float viewportMinZ = vp.MinZ;
    float viewportMaxZ = vp.MaxZ;
    XMMATRIX projection = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&d_matrix));
    XMMATRIX view = XMMatrixIdentity();
    XMMATRIX world = XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(gb.getMatrix()));
    XMFLOAT4 in_vec;
#else
    D3DXVECTOR3 in_vec;
#endif
    in_vec.z = 0.0f;

    // project points to create a plane orientated with GeometryBuffer's data
#ifdef DIRECTX_MATH_VERSION
    XMVECTOR p1;
    XMVECTOR p2;
    XMVECTOR p3;
    in_vec.x = 0;
    in_vec.y = 0;
    p1 = XMVector3Project(XMLoadFloat4(&in_vec), viewportX, viewportY, viewportWidth, viewportHeight, vp.MinZ, vp.MaxZ, projection, view, world);

    in_vec.x = 1;
    in_vec.y = 0;
    p2 = XMVector3Project(XMLoadFloat4(&in_vec), viewportX, viewportY, viewportWidth, viewportHeight, vp.MinZ, vp.MaxZ, projection, view, world);

    in_vec.x = 0;
    in_vec.y = 1;
    p3 = XMVector3Project(XMLoadFloat4(&in_vec), viewportX, viewportY, viewportWidth, viewportHeight, vp.MinZ, vp.MaxZ, projection, view, world);
#else
    D3DXVECTOR3 p1;
    D3DXVECTOR3 p2;
    D3DXVECTOR3 p3;
    in_vec.x = 0;
    in_vec.y = 0;
    D3DXVec3Project(&p1, &in_vec, &vp, static_cast<const D3DXMATRIX*>(&d_matrix), 0, static_cast<const D3DXMATRIX*>(gb.getMatrix()));

    in_vec.x = 1;
    in_vec.y = 0;
    D3DXVec3Project(&p2, &in_vec, &vp, static_cast<const D3DXMATRIX*>(&d_matrix), 0, static_cast<const D3DXMATRIX*>(gb.getMatrix()));

    in_vec.x = 0;
    in_vec.y = 1;
    D3DXVec3Project(&p3, &in_vec, &vp, static_cast<const D3DXMATRIX*>(&d_matrix), 0, static_cast<const D3DXMATRIX*>(gb.getMatrix()));
#endif

    // create plane from projected points
#ifdef DIRECTX_MATH_VERSION
    XMVECTOR surface_plane = XMPlaneFromPoints(p1, p2, p3);
#else
    D3DXPLANE surface_plane;
    D3DXPlaneFromPoints(&surface_plane, &p1, &p2, &p3);
#endif

    // unproject ends of ray
    in_vec.x = vp.Width * 0.5f;
    in_vec.y = vp.Height * 0.5f;
    in_vec.z = -d_viewDistance;
#ifdef DIRECTX_MATH_VERSION
    XMVECTOR t1 = XMVector3Unproject(XMLoadFloat4(&in_vec), viewportX, viewportY, viewportWidth, viewportHeight, vp.MinZ, vp.MaxZ, projection, view, world);
#else
    D3DXVECTOR3 t1;
    D3DXVec3Unproject(&t1, &in_vec, &vp, static_cast<const D3DXMATRIX*>(&d_matrix), 0, static_cast<const D3DXMATRIX*>(gb.getMatrix()));
#endif

    in_vec.x = p_in.d_x;
    in_vec.y = p_in.d_y;
    in_vec.z = 0.0f;
#ifdef DIRECTX_MATH_VERSION
    XMVECTOR t2 = XMVector3Unproject(XMLoadFloat4(&in_vec), viewportX, viewportY, viewportWidth, viewportHeight, vp.MinZ, vp.MaxZ, projection, view, world);
#else
    D3DXVECTOR3 t2;
    D3DXVec3Unproject(&t2, &in_vec, &vp, static_cast<const D3DXMATRIX*>(&d_matrix), 0, static_cast<const D3DXMATRIX*>(gb.getMatrix()));
#endif

    // get intersection of ray and plane
#ifdef DIRECTX_MATH_VERSION
    XMFLOAT3 intersect;
    XMStoreFloat3(&intersect, XMPlaneIntersectLine(surface_plane, t1, t2));
#else
    D3DXVECTOR3 intersect;
    D3DXPlaneIntersectLine(&intersect, &surface_plane, &t1, &t2);
#endif

    p_out.d_x = intersect.x;
    p_out.d_y = intersect.y;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::updateMatrix() const
{
    const float fov = 0.523598776f;
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    d_viewDistance = midx / (aspect * 0.267949192431123f);

#ifdef DIRECTX_MATH_VERSION
    XMVECTOR eye = { midx, midy, -d_viewDistance };
    XMVECTOR at = { midx, midy, 1 };
    XMVECTOR up = { 0, -1, 0 };

    XMMATRIX m;
    m = XMMatrixMultiply(
            XMMatrixLookAtRH(eye, at, up),
            XMMatrixPerspectiveFovRH(fov, aspect,
                                     d_viewDistance * 0.5f,
                                     d_viewDistance * 2.0f));
    XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&d_matrix), m);
#else
    D3DXVECTOR3 eye(midx, midy, -d_viewDistance);
    D3DXVECTOR3 at(midx, midy, 1);
    D3DXVECTOR3 up(0, -1, 0);

    D3DXMATRIX tmp;
    D3DXMatrixMultiply(static_cast<D3DXMATRIX*>(&d_matrix),
        D3DXMatrixLookAtRH(static_cast<D3DXMATRIX*>(&d_matrix), &eye, &at, &up),
        D3DXMatrixPerspectiveFovRH(&tmp, fov, aspect,
                                   d_viewDistance * 0.5f,
                                   d_viewDistance * 2.0f));
#endif

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D9RenderTarget<T>::setupViewport(D3DVIEWPORT9& vp) const
{
    vp.X = static_cast<DWORD>(d_area.left());
    vp.Y = static_cast<DWORD>(d_area.top());
    vp.Width = static_cast<DWORD>(d_area.getWidth());
    vp.Height = static_cast<DWORD>(d_area.getHeight());
    vp.MinZ = 0.0f;
    vp.MaxZ = 1.0f;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
