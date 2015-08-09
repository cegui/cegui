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
#include "CEGUI/RenderQueue.h"
#include "CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"

#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
template <typename T>
const double OpenGLRenderTarget<T>::d_yfov_tan = 0.267949192431123;

//----------------------------------------------------------------------------//
template <typename T>
OpenGLRenderTarget<T>::OpenGLRenderTarget(OpenGLRendererBase& owner) :
    d_owner(owner),
    d_area(0, 0, 0, 0),
    d_matrix(0),
    d_matrixValid(false),
    d_viewDistance(0)
{
    d_matrix = new mat4Pimpl();
}

//----------------------------------------------------------------------------//
template <typename T>
OpenGLRenderTarget<T>::~OpenGLRenderTarget()
{
    delete d_matrix;
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
template <typename T>
const Rectf& OpenGLRenderTarget<T>::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::activate()
{
    glViewport(static_cast<GLsizei>(d_area.left()),
               static_cast<GLsizei>(d_area.top()),
               static_cast<GLsizei>(d_area.getWidth()),
               static_cast<GLsizei>(d_area.getHeight()));

    if (!d_matrixValid)
        updateMatrix();

    d_owner.setViewProjectionMatrix(d_matrix);

    d_owner.setActiveRenderTarget(this);
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::deactivate()
{
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::unprojectPoint(const GeometryBuffer& buff,
    const Vector2f& p_in, Vector2f& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const OpenGLGeometryBufferBase& gb =
        static_cast<const OpenGLGeometryBufferBase&>(buff);

    const GLint vp[4] = {
        static_cast<GLint>(d_area.left()),
        static_cast<GLint>(d_area.top()),
        static_cast<GLint>(d_area.getWidth()),
        static_cast<GLint>(d_area.getHeight())
    };

    GLdouble in_x = 0.0, in_y = 0.0, in_z = 0.0;

    glm::ivec4 viewPort = glm::ivec4(vp[0], vp[1], vp[2], vp[3]);
    const glm::mat4& projMatrix = d_matrix->d_matrix;
    const glm::mat4& modelMatrix = gb.getMatrix()->d_matrix;

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
void OpenGLRenderTarget<T>::updateMatrix() const
{
    const float w = d_area.getWidth();
    const float h = d_area.getHeight();

    // We need to check if width or height are zero and act accordingly to prevent running into issues
    // with divisions by zero which would lead to undefined values, as well as faulty clipping planes
    // This is mostly important for avoiding asserts
    const bool widthAndHeightNotZero = ( w != 0.0f ) && ( h != 0.0f);

    const float aspect = widthAndHeightNotZero ? w / h : 1.0f;
    const float midx = widthAndHeightNotZero ? w * 0.5f : 0.5f;
    const float midy = widthAndHeightNotZero ? h * 0.5f : 0.5f;
    d_viewDistance = midx / (aspect * d_yfov_tan);

    glm::vec3 eye = glm::vec3(midx, midy, float(-d_viewDistance));
    glm::vec3 center = glm::vec3(midx, midy, 1);
    glm::vec3 up = glm::vec3(0, -1, 0);

    //Older glm versions use degrees as parameter here by default (Unless radians are forced via GLM_FORCE_RADIANS). Newer versions of glm exlusively use radians.
#if (GLM_VERSION_MAJOR == 0 && GLM_VERSION_MINOR <= 9 && GLM_VERSION_PATCH < 6) && (!defined(GLM_FORCE_RADIANS))
    glm::mat4 projectionMatrix = glm::perspective(30.f, aspect, float(d_viewDistance * 0.5), float(d_viewDistance * 2.0));
#else
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(30.f), aspect, float(d_viewDistance * 0.5), float(d_viewDistance * 2.0));
#endif

    // Projection matrix abuse!
    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
  
    d_matrix->d_matrix = projectionMatrix * viewMatrix;

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
