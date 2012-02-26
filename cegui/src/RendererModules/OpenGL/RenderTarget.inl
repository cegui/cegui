/***********************************************************************
    filename:   CEGUIOpenGLRenderTarget.cpp
    created:    Wed Jan 14 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGL/GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
template <typename T>
const double OpenGLRenderTarget<T>::d_yfov_tan = 0.267949192431123;

//----------------------------------------------------------------------------//
template <typename T>
OpenGLRenderTarget<T>::OpenGLRenderTarget(OpenGLRenderer& owner) :
    d_owner(owner),
    d_area(0, 0, 0, 0),
    d_matrixValid(false),
    d_viewDistance(0)
{
    //d_matrix does not need to be initialised here, we have d_matrixValid
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

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(d_matrix);
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

    const OpenGLGeometryBuffer& gb =
        static_cast<const OpenGLGeometryBuffer&>(buff);

    const GLint vp[4] = {
        static_cast<GLint>(d_area.left()),
        static_cast<GLint>(d_area.top()),
        static_cast<GLint>(d_area.getWidth()),
        static_cast<GLint>(d_area.getHeight())
    };

    GLdouble in_x, in_y, in_z = 0.0;

    // unproject the ends of the ray
    GLdouble r1_x, r1_y, r1_z;
    GLdouble r2_x, r2_y, r2_z;
    in_x = vp[2] * 0.5;
    in_y = vp[3] * 0.5;
    in_z = -d_viewDistance;
    gluUnProject(in_x, in_y, in_z, gb.getMatrix(), d_matrix, vp,
                 &r1_x, &r1_y, &r1_z);
    in_x = p_in.d_x;
    in_y = vp[3] - p_in.d_y;
    in_z = 0.0;
    gluUnProject(in_x, in_y, in_z, gb.getMatrix(), d_matrix, vp,
                 &r2_x, &r2_y, &r2_z);

    // project points to orientate them with GeometryBuffer plane
    GLdouble p1_x, p1_y, p1_z;
    GLdouble p2_x, p2_y, p2_z;
    GLdouble p3_x, p3_y, p3_z;
    in_x = 0.0;
    in_y = 0.0;
    gluProject(in_x, in_y, in_z, gb.getMatrix(), d_matrix, vp,
               &p1_x, &p1_y, &p1_z);
    in_x = 1.0;
    in_y = 0.0;
    gluProject(in_x, in_y, in_z, gb.getMatrix(), d_matrix, vp,
               &p2_x, &p2_y, &p2_z);
    in_x = 0.0;
    in_y = 1.0;
    gluProject(in_x, in_y, in_z, gb.getMatrix(), d_matrix, vp,
               &p3_x, &p3_y, &p3_z);

    // calculate vectors for generating the plane
    const double pv1_x = p2_x - p1_x;
    const double pv1_y = p2_y - p1_y;
    const double pv1_z = p2_z - p1_z;
    const double pv2_x = p3_x - p1_x;
    const double pv2_y = p3_y - p1_y;
    const double pv2_z = p3_z - p1_z;
    // given the vectors, calculate the plane normal
    const double pn_x = pv1_y * pv2_z - pv1_z * pv2_y;
    const double pn_y = pv1_z * pv2_x - pv1_x * pv2_z;
    const double pn_z = pv1_x * pv2_y - pv1_y * pv2_x;
    // calculate plane
    const double pn_len = std::sqrt(pn_x * pn_x + pn_y * pn_y + pn_z * pn_z);
    const double pl_a = pn_x / pn_len;
    const double pl_b = pn_y / pn_len;
    const double pl_c = pn_z / pn_len;
    const double pl_d = -(p1_x * pl_a + p1_y * pl_b + p1_z * pl_c);
    // calculate vector of picking ray
    const double rv_x = r1_x - r2_x;
    const double rv_y = r1_y - r2_y;
    const double rv_z = r1_z - r2_z;
    // calculate intersection of ray and plane
    const double pn_dot_r1 = (r1_x * pn_x + r1_y * pn_y + r1_z * pn_z);
    const double pn_dot_rv = (rv_x * pn_x + rv_y * pn_y + rv_z * pn_z);
    const double tmp1 = pn_dot_rv != 0.0 ? (pn_dot_r1 + pl_d) / pn_dot_rv : 0.0;
    const double is_x = r1_x - rv_x * tmp1;
    const double is_y = r1_y - rv_y * tmp1;

    p_out.d_x = static_cast<float>(is_x);
    p_out.d_y = static_cast<float>(is_y);
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLRenderTarget<T>::updateMatrix() const
{
    const double w = d_area.getWidth();
    const double h = d_area.getHeight();
    const double aspect = w / h;
    const double midx = w * 0.5;
    const double midy = h * 0.5;
    d_viewDistance = midx / (aspect * d_yfov_tan);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(30.0, aspect, d_viewDistance * 0.5, d_viewDistance * 2.0);
    // Projection matrix abuse!
    gluLookAt(midx, midy, -d_viewDistance, midx, midy, 1, 0, -1, 0);
    glGetDoublev(GL_PROJECTION_MATRIX, d_matrix);
    glPopMatrix();

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
