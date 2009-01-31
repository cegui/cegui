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
#include "CEGUIOpenGLRenderTarget.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIOpenGLGeometryBuffer.h"
#include "CEGUIOpenGL.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const double OpenGLRenderTarget::d_yfov_tan = 0.267949192431123;

//----------------------------------------------------------------------------//
OpenGLRenderTarget::OpenGLRenderTarget(OpenGLRenderer& owner) :
    d_owner(owner),
    d_area(0, 0, 0, 0),
    d_matrixValid(false),
    d_depthEnabled(false)
{
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::setArea(const Rect& area)
{
    d_area = area;
    d_matrixValid = false;
}

//----------------------------------------------------------------------------//
const Rect& OpenGLRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::activate()
{
    glViewport(static_cast<GLsizei>(d_area.d_left),
               static_cast<GLsizei>(d_area.d_top),
               static_cast<GLsizei>(d_area.getWidth()),
               static_cast<GLsizei>(d_area.getHeight()));

    // activate depth if needed
    if (d_depthEnabled)
        glEnable(GL_DEPTH_TEST);

    if (!d_matrixValid)
        updateMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(d_matrix);
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::deactivate()
{
    // reset depth buffer state setting
    if (d_depthEnabled)
        glDisable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::unprojectPoint(const GeometryBuffer& buff,
    const Vector2& p_in, Vector2& p_out) const
{
    // With no depth values, you get no unprojected points...
    if (!d_depthEnabled)
    {
        p_out = p_in;
        return;
    }

    if (!d_matrixValid)
        updateMatrix();

    const GLint vp[4] =
        { d_area.d_left, d_area.d_top, d_area.getWidth(), d_area.getHeight() };

    GLfloat ix = p_in.d_x;
    GLfloat iy = vp[3] - p_in.d_y;
    GLfloat iz = readZValue(ix, iy);

    double ox, oy, oz;

    gluUnProject(ix, iy, iz,
                 static_cast<const OpenGLGeometryBuffer&>(buff).getMatrix(),
                 d_matrix,
                 vp,
                 &ox, &oy, &oz);

    p_out.d_x = ox;
    p_out.d_y = oy;
}

//----------------------------------------------------------------------------//
float OpenGLRenderTarget::readZValue(const float x, const float y) const
{
    float z;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &z);
    return z;
}

//----------------------------------------------------------------------------//
void OpenGLRenderTarget::updateMatrix() const
{
    const double w = d_area.getWidth();
    const double h = d_area.getHeight();
    const double aspect = w / h;
    const double midx = w * 0.5;
    const double midy = h * 0.5;
    const double dist = midx / (aspect * d_yfov_tan);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPerspective(30.0, aspect, dist * 0.5, dist+dist);
    // Projection matrix abuse!
    gluLookAt(midx, midy, -dist, midx, midy, 1, 0, -1, 0);
    glGetDoublev(GL_PROJECTION_MATRIX, d_matrix);
    glPopMatrix();

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
bool OpenGLRenderTarget::isDepthBufferEnabled()
{
    return d_depthEnabled;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
