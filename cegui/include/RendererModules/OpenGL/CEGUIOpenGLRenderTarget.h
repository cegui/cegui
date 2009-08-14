/***********************************************************************
    filename:   CEGUIOpenGLRenderTarget.h
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
#ifndef _CEGUIOpenGLRenderTarget_h_
#define _CEGUIOpenGLRenderTarget_h_

#include "CEGUIOpenGLRenderer.h"
#include "../../CEGUIRenderTarget.h"
#include "../../CEGUIRect.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Intermediate OpenGL implementation of a RenderTarget.
*/
class OPENGL_GUIRENDERER_API OpenGLRenderTarget : public virtual RenderTarget
{
public:
    //! Constructor
    OpenGLRenderTarget(OpenGLRenderer& owner);

    // implement parts of RenderTarget interface
    void draw(const GeometryBuffer& buffer);
    void draw(const RenderQueue& queue);
    void setArea(const Rect& area);
    const Rect& getArea() const;
    void activate();
    void deactivate();
    void unprojectPoint(const GeometryBuffer& buff,
                        const Vector2& p_in, Vector2& p_out) const;

protected:
    //! helper that initialises the cached matrix
    virtual void updateMatrix() const;

    //! OpenGLRenderer that created this object
    OpenGLRenderer& d_owner;
    //! holds defined area for the RenderTarget
    Rect d_area;
    //! tangent of the y FOV half-angle; used to calculate viewing distance.
    static const double d_yfov_tan;
    //! saved copy of projection matrix
    mutable double d_matrix[16];
    //! true if saved matrix is up to date
    mutable bool d_matrixValid;
    //! tracks viewing distance (this is set up at the same time as d_matrix)
    mutable double d_viewDistance;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLRenderTarget_h_
