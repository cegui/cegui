/***********************************************************************
    filename:   RenderTarget.inl
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
#include "CEGUI/RenderQueue.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
template <typename T>
const double Direct3D11RenderTarget<T>::d_yfov_tan = 0.267949192431123;

//----------------------------------------------------------------------------//
template <typename T>
Direct3D11RenderTarget<T>::Direct3D11RenderTarget(Direct3D11Renderer& owner) :
    d_owner(owner),
    d_device(*d_owner.getDirect3DDevice()),
    d_deviceContext(*d_owner.getDirect3DDeviceContext()),
    d_area(0, 0, 0, 0),
    d_viewDistance(0),
    d_matrixValid(false)
{
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
template <typename T>
const Rectf& Direct3D11RenderTarget<T>::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::activate()
{
    if (!d_matrixValid)
        updateMatrix();

    D3D11_VIEWPORT vp;
    setupViewport(vp);
    d_deviceContext.RSSetViewports(1, &vp);

    d_owner.setViewProjectionMatrix(d_projViewMatrix);
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::deactivate()
{
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::unprojectPoint(const GeometryBuffer& buff,
    const Vector2f& p_in, Vector2f& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    
    const Direct3D11GeometryBuffer& gb =
        static_cast<const Direct3D11GeometryBuffer&>(buff);

    const int vp[4] = {
        static_cast<int>(d_area.left()),
        static_cast<int>(d_area.top()),
        static_cast<int>(d_area.getWidth()),
        static_cast<int>(d_area.getHeight())
    };

    double in_x, in_y, in_z;

    glm::ivec4 viewPort = glm::ivec4(vp[0], vp[1], vp[2], vp[3]);
    const glm::mat4& projMatrix = d_projViewMatrix;
    const glm::mat4& modelMatrix = gb.getMatrix();

    // unproject the ends of the ray
    glm::vec3 unprojected1;
    glm::vec3 unprojected2;
    in_x = vp[2] * 0.5;
    in_y = vp[3] * 0.5;
    in_z = -d_viewDistance;
    unprojected1 =  glm::unProject(glm::vec3(in_x, in_y, in_z), modelMatrix, projMatrix, viewPort);
    in_x = p_in.d_x;
    in_y = vp[3] - p_in.d_y;
    in_z = 0.0;
    unprojected2 = glm::unProject(glm::vec3(in_x, in_y, in_z), modelMatrix, projMatrix, viewPort);

    // project points to orientate them with GeometryBuffer plane
    glm::vec3 projected1;
    glm::vec3 projected2;
    glm::vec3 projected3;
    in_x = 0.0;
    in_y = 0.0;
    projected1 = glm::project(glm::vec3(in_x, in_y, in_z), modelMatrix, projMatrix, viewPort);
    in_x = 1.0;
    in_y = 0.0;
    projected2 = glm::project(glm::vec3(in_x, in_y, in_z), modelMatrix, projMatrix, viewPort);
    in_x = 0.0;
    in_y = 1.0;
    projected3 = glm::project(glm::vec3(in_x, in_y, in_z), modelMatrix, projMatrix, viewPort);

    // calculate vectors for generating the plane
    const glm::vec3 pv1 = projected2 - projected1;
    const glm::vec3 pv2 = projected3 - projected1;
    // given the vectors, calculate the plane normal
    const glm::vec3 planeNormal = glm::cross(pv1, pv2);
    // calculate plane
    const glm::vec3 planeNormalNormalized = glm::normalize(planeNormal);
    const double pl_d = - glm::dot(projected1, planeNormalNormalized);
    // calculate vector of picking ray
    const glm::vec3 rv = unprojected1 - unprojected2;
    // calculate intersection of ray and plane
    const double pn_dot_r1 = glm::dot(unprojected1, planeNormal);
    const double pn_dot_rv = glm::dot(rv, planeNormal);
    const double tmp1 = pn_dot_rv != 0.0 ? (pn_dot_r1 + pl_d) / pn_dot_rv : 0.0;
    const double is_x = unprojected1.x - rv.x * tmp1;
    const double is_y = unprojected1.y - rv.y * tmp1;

    p_out.d_x = static_cast<float>(is_x);
    p_out.d_y = static_cast<float>(is_y);

    p_out = p_in; // CrazyEddie wanted this
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::updateMatrix() const
{
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();
    const float aspect = w / h;
    const float midx = w * 0.5f;
    const float midy = h * 0.5f;
    d_viewDistance = midx / (aspect * d_yfov_tan);

    glm::vec3 eye = glm::vec3(midx, midy, float(-d_viewDistance));
    glm::vec3 center = glm::vec3(midx, midy, 1);
    glm::vec3 up = glm::vec3(0, -1, 0);

    const float fovy = 30.f;
    const float zNear = static_cast<float>(d_viewDistance * 0.5f);
    const float zFar = static_cast<float>(d_viewDistance * 2.0f);
    const float f = 1.0f / std::tan(fovy * glm::pi<float>() * 0.5f / 180.0f);
    const float Q = zFar / (zNear - zFar);

    // We need to have a projection matrix with its depth in clip space ranging from 0 to 1 for nearclip to farclip.
    // The regular OpenGL projection matrix would work too, but we would lose 1 bit of depth precision, which this matrix should fix:
    float projectionMatrixFloat[16] =
    {
        f/aspect,           0.0f,               0.0f,           0.0f,
        0.0f,               f,                  0.0f,           0.0f,
        0.0f,               0.0f,               Q,              -1.0f,
        0.0f,               0.0f,               Q * zNear,      0.0f
    };

    glm::mat4 projectionMatrix = glm::make_mat4(projectionMatrixFloat);

    // Projection matrix abuse!
    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
  
    d_projViewMatrix = projectionMatrix * viewMatrix;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
template <typename T>
void Direct3D11RenderTarget<T>::setupViewport(D3D11_VIEWPORT& vp) const
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
