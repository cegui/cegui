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


void OpenGLRenderTarget::unprojectPoint(const GeometryBuffer& buff,
    const glm::vec2& p_in, glm::vec2& p_out) const
{
    if (!RenderTarget::d_matrixValid)
        updateMatrix();

    const OpenGLGeometryBufferBase& gb =
        static_cast<const OpenGLGeometryBufferBase&>(buff);

    const GLint vp[4] = {
        static_cast<GLint>(RenderTarget::d_area.left()),
        static_cast<GLint>(RenderTarget::d_area.top()),
        static_cast<GLint>(RenderTarget::d_area.getWidth()),
        static_cast<GLint>(RenderTarget::d_area.getHeight())
    };

    GLfloat in_x, in_y = 0.0f, in_z = 0.0f;

    glm::ivec4 viewPort = glm::ivec4(vp[0], vp[1], vp[2], vp[3]);
    const glm::mat4& projMatrix = RenderTarget::d_matrix;
    const glm::mat4& modelMatrix = gb.getModelMatrix();

    // unproject the ends of the ray
    glm::vec3 unprojected1;
    glm::vec3 unprojected2;
    in_x = vp[2] * 0.5f;
    in_y = vp[3] * 0.5f;
    in_z = -RenderTarget::d_viewDistance;
    unprojected1 =  glm::unProject(glm::vec3(in_x, in_y, in_z), modelMatrix, projMatrix, viewPort);
    in_x = p_in.x;
    in_y = vp[3] - p_in.y;
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

    p_out.x = static_cast<float>(is_x);
    p_out.y = static_cast<float>(is_y);
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
