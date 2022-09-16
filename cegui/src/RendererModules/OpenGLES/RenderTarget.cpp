/***********************************************************************
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
#include "CEGUI/RendererModules/OpenGLES/RenderTarget.h"
#include "CEGUI/RenderQueue.h"
#include "CEGUI/RendererModules/OpenGLES/GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGLES/GLES.h"
#include <cmath>

// Start of CEGUI namespace section
namespace CEGUI
{

const double OpenGLESRenderTarget::d_yfov_tan = 0.267949192431123;


OpenGLESRenderTarget::OpenGLESRenderTarget(OpenGLESRenderer& owner) :
    d_owner(owner),
    d_area(0, 0, 0, 0),
    d_viewDistance(0),
    d_matrixValid(false)
{
    //d_matrix does not need to be initialised here, we have d_matrixValid
    //for(unsigned int i = 0; i < 16;++i)
    //    d_matrix[i]=0.0;
}


void OpenGLESRenderTarget::draw(const GeometryBuffer& buffer)
{
    buffer.draw(drawModeMask);
}


void OpenGLESRenderTarget::draw(const RenderQueue& queue)
{
    queue.draw(drawModeMask);
}


void OpenGLESRenderTarget::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}


const Rectf& OpenGLESRenderTarget::getArea() const
{
    return d_area;
}


void OpenGLESRenderTarget::activate()
{
	glViewport(static_cast<GLsizei>(d_area.left()),
               static_cast<GLsizei>(d_area.top()),
               static_cast<GLsizei>(d_area.getWidth()),
               static_cast<GLsizei>(d_area.getHeight()));

    if (!d_matrixValid)
        updateMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(d_matrix);
}


void OpenGLESRenderTarget::deactivate()
{
}

//----------------------------------------------------------------------------//
typedef double GLdouble;
static void
gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
          GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
          GLdouble upz);
static void
gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);


void OpenGLESRenderTarget::updateMatrix() const
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
	glGetFloatv(GL_PROJECTION_MATRIX, d_matrix);
    glTranslatef(0.375, 0.375, 0);
    glPopMatrix();

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
// The following functions were taken from iGLU
// (http://code.google.com/p/iphone-glu/).
//
// Specifically the file at 'iGLU/libutil/project.c'
//
/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 */

//----------------------------------------------------------------------------//
static void normalize(float v[3])
{
    float r;

    r = sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
    if (r == 0.0) return;

    v[0] /= r;
    v[1] /= r;
    v[2] /= r;
}

//----------------------------------------------------------------------------//
static void cross(float v1[3], float v2[3], float result[3])
{
    result[0] = v1[1]*v2[2] - v1[2]*v2[1];
    result[1] = v1[2]*v2[0] - v1[0]*v2[2];
    result[2] = v1[0]*v2[1] - v1[1]*v2[0];
}

//----------------------------------------------------------------------------//
static void __gluMakeIdentityf(GLfloat m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

//----------------------------------------------------------------------------//
void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
               GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
               GLdouble upz)
{
    float forward[3], side[3], up[3];
    GLfloat m[4][4];

    forward[0] = centerx - eyex;
    forward[1] = centery - eyey;
    forward[2] = centerz - eyez;

    up[0] = upx;
    up[1] = upy;
    up[2] = upz;

    normalize(forward);

    /* Side = forward x up */
    cross(forward, up, side);
    normalize(side);

    /* Recompute up as: up = side x forward */
    cross(side, forward, up);

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = side[0];
    m[1][0] = side[1];
    m[2][0] = side[2];

    m[0][1] = up[0];
    m[1][1] = up[1];
    m[2][1] = up[2];

    m[0][2] = -forward[0];
    m[1][2] = -forward[1];
    m[2][2] = -forward[2];

    glMultMatrixf(&m[0][0]);
    glTranslatef(-eyex, -eyey, -eyez);
}

//----------------------------------------------------------------------------//
#define __glPi 3.14159265358979323846
void gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLfloat m[4][4];
    double sine, cotangent, deltaZ;
    double radians = fovy / 2 * __glPi / 180;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
	return;
    }
    cotangent = cos(radians) / sine;

    __gluMakeIdentityf(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixf(&m[0][0]);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
