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
//----------------------------------------------------------------------------//
template <typename T>
const double OpenGLESRenderTarget<T>::d_yfov_tan = 0.267949192431123;

//----------------------------------------------------------------------------//
template <typename T>
OpenGLESRenderTarget<T>::OpenGLESRenderTarget(OpenGLESRenderer& owner) :
    d_owner(owner),
    d_area(0, 0, 0, 0),
    d_viewDistance(0),
    d_matrixValid(false)
{
    //d_matrix does not need to be initialised here, we have d_matrixValid
    //for(unsigned int i = 0; i < 16;++i)
    //    d_matrix[i]=0.0;
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLESRenderTarget<T>::draw(const GeometryBuffer& buffer)
{
    buffer.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLESRenderTarget<T>::draw(const RenderQueue& queue)
{
    queue.draw();
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLESRenderTarget<T>::setArea(const Rectf& area)
{
    d_area = area;
    d_matrixValid = false;

    RenderTargetEventArgs args(this);
    T::fireEvent(RenderTarget::EventAreaChanged, args);
}

//----------------------------------------------------------------------------//
template <typename T>
const Rectf& OpenGLESRenderTarget<T>::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLESRenderTarget<T>::activate()
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

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLESRenderTarget<T>::deactivate()
{
}

//----------------------------------------------------------------------------//
typedef double GLdouble;
static GLint
gluProject(GLdouble objx, GLdouble objy, GLdouble objz, 
           const GLdouble modelMatrix[16], 
           const GLdouble projMatrix[16],
           const GLint viewport[4],
           GLdouble *winx, GLdouble *winy, GLdouble *winz);
static GLint
gluUnProject(GLdouble winx, GLdouble winy, GLdouble winz,
             const GLdouble modelMatrix[16], 
             const GLdouble projMatrix[16],
             const GLint viewport[4],
             GLdouble *objx, GLdouble *objy, GLdouble *objz);
static void
gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
          GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
          GLdouble upz);
static void
gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);

//----------------------------------------------------------------------------//
template <typename T>
void OpenGLESRenderTarget<T>::unprojectPoint(const GeometryBuffer& buff,
    const Vector2f& p_in, Vector2f& p_out) const
{
    if (!d_matrixValid)
        updateMatrix();

    const OpenGLESGeometryBuffer& gb =
        static_cast<const OpenGLESGeometryBuffer&>(buff);

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

	double gb_matrixd[16], d_matrixd[16];
	for (uint i = 0; i < 16; ++i)
	{
		gb_matrixd[i] = (double)gb.getMatrix()[i];
		d_matrixd[i] = (double)d_matrix[i];
	}

    gluUnProject(in_x, in_y, in_z, gb_matrixd, d_matrixd, vp,
                 &r1_x, &r1_y, &r1_z);
    in_x = p_in.d_x;
    in_y = vp[3] - p_in.d_y;
    in_z = 0.0;
    gluUnProject(in_x, in_y, in_z, gb_matrixd, d_matrixd, vp,
                 &r2_x, &r2_y, &r2_z);

    // project points to orientate them with GeometryBuffer plane
    GLdouble p1_x, p1_y, p1_z;
    GLdouble p2_x, p2_y, p2_z;
    GLdouble p3_x, p3_y, p3_z;
    in_x = 0.0;
    in_y = 0.0;
    gluProject(in_x, in_y, in_z, gb_matrixd, d_matrixd, vp,
               &p1_x, &p1_y, &p1_z);
    in_x = 1.0;
    in_y = 0.0;
    gluProject(in_x, in_y, in_z, gb_matrixd, d_matrixd, vp,
               &p2_x, &p2_y, &p2_z);
    in_x = 0.0;
    in_y = 1.0;
    gluProject(in_x, in_y, in_z, gb_matrixd, d_matrixd, vp,
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
void OpenGLESRenderTarget<T>::updateMatrix() const
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
static void __gluMultMatrixVecd(const GLdouble matrix[16], const GLdouble in[4],
		      GLdouble out[4])
{
    int i;

    for (i=0; i<4; i++) {
	out[i] = 
	    in[0] * matrix[0*4+i] +
	    in[1] * matrix[1*4+i] +
	    in[2] * matrix[2*4+i] +
	    in[3] * matrix[3*4+i];
    }
}

//----------------------------------------------------------------------------//
static int __gluInvertMatrixd(const GLdouble m[16], GLdouble invOut[16])
{
    double inv[16], det;
    int i;

    inv[0] =   m[5]*m[10]*m[15] - m[5]*m[11]*m[14] - m[9]*m[6]*m[15]
             + m[9]*m[7]*m[14] + m[13]*m[6]*m[11] - m[13]*m[7]*m[10];
    inv[4] =  -m[4]*m[10]*m[15] + m[4]*m[11]*m[14] + m[8]*m[6]*m[15]
             - m[8]*m[7]*m[14] - m[12]*m[6]*m[11] + m[12]*m[7]*m[10];
    inv[8] =   m[4]*m[9]*m[15] - m[4]*m[11]*m[13] - m[8]*m[5]*m[15]
             + m[8]*m[7]*m[13] + m[12]*m[5]*m[11] - m[12]*m[7]*m[9];
    inv[12] = -m[4]*m[9]*m[14] + m[4]*m[10]*m[13] + m[8]*m[5]*m[14]
             - m[8]*m[6]*m[13] - m[12]*m[5]*m[10] + m[12]*m[6]*m[9];
    inv[1] =  -m[1]*m[10]*m[15] + m[1]*m[11]*m[14] + m[9]*m[2]*m[15]
             - m[9]*m[3]*m[14] - m[13]*m[2]*m[11] + m[13]*m[3]*m[10];
    inv[5] =   m[0]*m[10]*m[15] - m[0]*m[11]*m[14] - m[8]*m[2]*m[15]
             + m[8]*m[3]*m[14] + m[12]*m[2]*m[11] - m[12]*m[3]*m[10];
    inv[9] =  -m[0]*m[9]*m[15] + m[0]*m[11]*m[13] + m[8]*m[1]*m[15]
             - m[8]*m[3]*m[13] - m[12]*m[1]*m[11] + m[12]*m[3]*m[9];
    inv[13] =  m[0]*m[9]*m[14] - m[0]*m[10]*m[13] - m[8]*m[1]*m[14]
             + m[8]*m[2]*m[13] + m[12]*m[1]*m[10] - m[12]*m[2]*m[9];
    inv[2] =   m[1]*m[6]*m[15] - m[1]*m[7]*m[14] - m[5]*m[2]*m[15]
             + m[5]*m[3]*m[14] + m[13]*m[2]*m[7] - m[13]*m[3]*m[6];
    inv[6] =  -m[0]*m[6]*m[15] + m[0]*m[7]*m[14] + m[4]*m[2]*m[15]
             - m[4]*m[3]*m[14] - m[12]*m[2]*m[7] + m[12]*m[3]*m[6];
    inv[10] =  m[0]*m[5]*m[15] - m[0]*m[7]*m[13] - m[4]*m[1]*m[15]
             + m[4]*m[3]*m[13] + m[12]*m[1]*m[7] - m[12]*m[3]*m[5];
    inv[14] = -m[0]*m[5]*m[14] + m[0]*m[6]*m[13] + m[4]*m[1]*m[14]
             - m[4]*m[2]*m[13] - m[12]*m[1]*m[6] + m[12]*m[2]*m[5];
    inv[3] =  -m[1]*m[6]*m[11] + m[1]*m[7]*m[10] + m[5]*m[2]*m[11]
             - m[5]*m[3]*m[10] - m[9]*m[2]*m[7] + m[9]*m[3]*m[6];
    inv[7] =   m[0]*m[6]*m[11] - m[0]*m[7]*m[10] - m[4]*m[2]*m[11]
             + m[4]*m[3]*m[10] + m[8]*m[2]*m[7] - m[8]*m[3]*m[6];
    inv[11] = -m[0]*m[5]*m[11] + m[0]*m[7]*m[9] + m[4]*m[1]*m[11]
             - m[4]*m[3]*m[9] - m[8]*m[1]*m[7] + m[8]*m[3]*m[5];
    inv[15] =  m[0]*m[5]*m[10] - m[0]*m[6]*m[9] - m[4]*m[1]*m[10]
             + m[4]*m[2]*m[9] + m[8]*m[1]*m[6] - m[8]*m[2]*m[5];

    det = m[0]*inv[0] + m[1]*inv[4] + m[2]*inv[8] + m[3]*inv[12];
    if (det == 0)
        return GL_FALSE;

    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;

    return GL_TRUE;
}

//----------------------------------------------------------------------------//
static void __gluMultMatricesd(const GLdouble a[16], const GLdouble b[16],
				GLdouble r[16])
{
    int i, j;

    for (i = 0; i < 4; i++) {
	for (j = 0; j < 4; j++) {
	    r[i*4+j] = 
		a[i*4+0]*b[0*4+j] +
		a[i*4+1]*b[1*4+j] +
		a[i*4+2]*b[2*4+j] +
		a[i*4+3]*b[3*4+j];
	}
    }
}

//----------------------------------------------------------------------------//
GLint gluProject(GLdouble objx, GLdouble objy, GLdouble objz, 
                 const GLdouble modelMatrix[16], 
                 const GLdouble projMatrix[16],
                 const GLint viewport[4],
                 GLdouble *winx, GLdouble *winy, GLdouble *winz)
{
    double in[4];
    double out[4];

    in[0]=objx;
    in[1]=objy;
    in[2]=objz;
    in[3]=1.0;
    __gluMultMatrixVecd(modelMatrix, in, out);
    __gluMultMatrixVecd(projMatrix, out, in);
    if (in[3] == 0.0) return(GL_FALSE);
    in[0] /= in[3];
    in[1] /= in[3];
    in[2] /= in[3];
    /* Map x, y and z to range 0-1 */
    in[0] = in[0] * 0.5 + 0.5;
    in[1] = in[1] * 0.5 + 0.5;
    in[2] = in[2] * 0.5 + 0.5;

    /* Map x,y to viewport */
    in[0] = in[0] * viewport[2] + viewport[0];
    in[1] = in[1] * viewport[3] + viewport[1];

    *winx=in[0];
    *winy=in[1];
    *winz=in[2];
    return(GL_TRUE);
}

//----------------------------------------------------------------------------//
GLint gluUnProject(GLdouble winx, GLdouble winy, GLdouble winz,
                   const GLdouble modelMatrix[16], 
                   const GLdouble projMatrix[16],
                   const GLint viewport[4],
                   GLdouble *objx, GLdouble *objy, GLdouble *objz)
{
    double finalMatrix[16];
    double in[4];
    double out[4];

    __gluMultMatricesd(modelMatrix, projMatrix, finalMatrix);
    if (!__gluInvertMatrixd(finalMatrix, finalMatrix)) return(GL_FALSE);

    in[0]=winx;
    in[1]=winy;
    in[2]=winz;
    in[3]=1.0;

    /* Map x and y from window coordinates */
    in[0] = (in[0] - viewport[0]) / viewport[2];
    in[1] = (in[1] - viewport[1]) / viewport[3];

    /* Map to range -1 to 1 */
    in[0] = in[0] * 2 - 1;
    in[1] = in[1] * 2 - 1;
    in[2] = in[2] * 2 - 1;

    __gluMultMatrixVecd(finalMatrix, in, out);
    if (out[3] == 0.0) return(GL_FALSE);
    out[0] /= out[3];
    out[1] /= out[3];
    out[2] /= out[3];
    *objx = out[0];
    *objy = out[1];
    *objz = out[2];
    return(GL_TRUE);
}

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
