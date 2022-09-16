/***********************************************************************
    created:    Sat Feb 18 2012
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/RenderTarget.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RenderQueue.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#if (GLM_VERSION_MAJOR == 0 && GLM_VERSION_MINOR <= 9 && GLM_VERSION_PATCH <= 3)
    #include <glm/gtx/constants.hpp>
#else
    #include <glm/gtc/constants.hpp>
#endif
#include <glm/gtc/type_ptr.hpp>

namespace CEGUI
{
//----------------------------------------------------------------------------//

const String RenderTarget::EventNamespace("RenderTarget");
const String RenderTarget::EventAreaChanged("AreaChanged");

//----------------------------------------------------------------------------//
RenderTarget::RenderTarget():
    d_activationCounter(0),
    d_area(0, 0, 0, 0),
    d_matrixValid(false),
    d_matrix(1.0f),
    d_viewDistance(0),
    d_fovY(glm::radians(30.0f))
{
    // Call the setter function to ensure that the half-angle tangens value is set correctly
    setFovY(d_fovY);
}

//----------------------------------------------------------------------------//
RenderTarget::~RenderTarget()
{}

//----------------------------------------------------------------------------//
void RenderTarget::activate()
{
    Renderer& owner = getOwner();

    owner.setActiveRenderTarget(this);

    ++d_activationCounter;

    if(d_activationCounter == 0)
        owner.invalidateGeomBufferMatrices(this);
}

//----------------------------------------------------------------------------//
void RenderTarget::deactivate()
{
}

//----------------------------------------------------------------------------//
void RenderTarget::unprojectPoint(const GeometryBuffer& buff, const glm::vec2& p_in, glm::vec2& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const glm::mat4 modelMatrix = buff.getModelMatrix();
    const glm::ivec4 viewPort = glm::ivec4(
        static_cast<int>(d_area.left()),
        static_cast<int>(d_area.top()),
        static_cast<int>(d_area.getWidth()),
        static_cast<int>(d_area.getHeight()));

    // unproject the ends of the ray
    const glm::vec3 unprojected1 = glm::unProject(glm::vec3(d_area.getWidth() * 0.5f, d_area.getHeight() * 0.5f, -d_viewDistance), modelMatrix, d_matrix, viewPort);
    const glm::vec3 unprojected2 = glm::unProject(glm::vec3(p_in.x, d_area.getHeight() - p_in.y, 0.f), modelMatrix, d_matrix, viewPort);

    // project points to orientate them with GeometryBuffer plane
    const glm::vec3 projected1 = glm::project(glm::vec3(0.f, 0.f, 0.f), modelMatrix, d_matrix, viewPort);
    const glm::vec3 projected2 = glm::project(glm::vec3(1.f, 0.f, 0.f), modelMatrix, d_matrix, viewPort);
    const glm::vec3 projected3 = glm::project(glm::vec3(0.f, 1.f, 0.f), modelMatrix, d_matrix, viewPort);

    // calculate vectors for generating the plane
    const glm::vec3 pv1 = projected2 - projected1;
    const glm::vec3 pv2 = projected3 - projected1;
    // given the vectors, calculate the plane normal
    const glm::vec3 planeNormal = glm::cross(pv1, pv2);
    // calculate plane
    const auto pl_d = -glm::dot(projected1, glm::normalize(planeNormal));
    // calculate vector of picking ray
    const glm::vec3 rv = unprojected1 - unprojected2;
    // calculate intersection of ray and plane
    const auto pn_dot_r1 = glm::dot(unprojected1, planeNormal);
    const auto pn_dot_rv = glm::dot(rv, planeNormal);
    const auto tmp1 = (pn_dot_rv != 0.f) ? (pn_dot_r1 + pl_d) / pn_dot_rv : 0.f;

    p_out.x = static_cast<float>(unprojected1.x - rv.x * tmp1);
    p_out.y = static_cast<float>(unprojected1.y - rv.y * tmp1);
}

//----------------------------------------------------------------------------//
void RenderTarget::draw(const GeometryBuffer& buffer,
    std::uint32_t drawModeMask)
{
    buffer.draw(drawModeMask);
}

//----------------------------------------------------------------------------//
void RenderTarget::draw(const RenderQueue& queue,
    std::uint32_t drawModeMask)
{
    queue.draw(drawModeMask);
}

//----------------------------------------------------------------------------//
unsigned int RenderTarget::getActivationCounter() const
{
    return d_activationCounter;
}

//----------------------------------------------------------------------------//
void RenderTarget::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    fireEvent(RenderTarget::EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
const Rectf& RenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
glm::mat4 RenderTarget::createViewProjMatrixForOpenGL() const
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
    RenderTarget::d_viewDistance = midx / (aspect * RenderTarget::d_fovY_halftan);

    glm::vec3 eye = glm::vec3(midx, midy, -RenderTarget::d_viewDistance);
    glm::vec3 center = glm::vec3(midx, midy, 1);
    glm::vec3 up = glm::vec3(0, -1, 0);

    // Creating the perspective projection matrix based on an assumed vertical FOV angle

    // Older glm versions use degrees (Unless radians are forced via GLM_FORCE_RADIANS). Newer versions of glm exlusively use radians.
    // We need to convert the angle for older versions
#if (GLM_VERSION_MAJOR == 0 && GLM_VERSION_MINOR <= 9 && GLM_VERSION_PATCH < 6) && (!defined(GLM_FORCE_RADIANS))
    glm::mat4 projectionMatrix = glm::perspective(glm::degrees(d_fovY), aspect, float(d_viewDistance * 0.5), float(d_viewDistance * 2.0));
#else
    glm::mat4 projectionMatrix = glm::perspective(d_fovY, aspect, float(d_viewDistance * 0.5), float(d_viewDistance * 2.0));
#endif
   
    // Creating the view matrix based on the eye, center and up vectors targeting the middle of the screen
    glm::mat4 viewMatrix = glm::lookAt(eye, center, up);
  
    return projectionMatrix * viewMatrix;
}

//----------------------------------------------------------------------------//
glm::mat4 RenderTarget::createViewProjMatrixForDirect3D() const
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
    RenderTarget::d_viewDistance = midx / (aspect * RenderTarget::d_fovY_halftan);

    glm::vec3 eye = glm::vec3(midx, midy, -d_viewDistance);
    glm::vec3 center = glm::vec3(midx, midy, 1);
    glm::vec3 up = glm::vec3(0, -1, 0);


    // We need to have a projection matrix with its depth in clip space ranging from 0 to 1 for nearclip to farclip.
    // The regular OpenGL projection matrix would work too, but we would lose 1 bit of depth precision, which the following
    // manually filled matrix should fix:
    const float fovy = 30.f;
    const float zNear = RenderTarget::d_viewDistance * 0.5f;
    const float zFar = RenderTarget::d_viewDistance * 2.0f;
    const float f = 1.0f / std::tan(fovy * glm::pi<float>() * 0.5f / 180.0f);
    const float Q = zFar / (zNear - zFar);

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

    return projectionMatrix * viewMatrix;
}

//----------------------------------------------------------------------------//
void RenderTarget::updateMatrix(const glm::mat4& matrix) const
{
    d_matrix = matrix;

    d_matrixValid = true;
    //! This will trigger the RenderTarget to notify all of its GeometryBuffers to regenerate their matrices
    d_activationCounter = -1;
}

//----------------------------------------------------------------------------//
float RenderTarget::getFovY() const
{
    return d_fovY;
}

//----------------------------------------------------------------------------//
void RenderTarget::setFovY(const float fovY)
{
    d_fovY = fovY;

    d_fovY_halftan = std::tan(fovY * 0.5f);
}

}
