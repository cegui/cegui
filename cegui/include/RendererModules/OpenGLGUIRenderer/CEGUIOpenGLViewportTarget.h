/***********************************************************************
    filename:   CEGUIOpenGLViewportTarget.h
    created:    Tue Feb 12 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIOpenGLViewportTarget_h_
#define _CEGUIOpenGLViewportTarget_h_

#include "CEGUIOpenGLRenderTarget.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    OpenGLViewportTarget - allows rendering to an OpenGL viewport.
*/
class OPENGL_GUIRENDERER_API OpenGLViewportTarget : public OpenGLRenderTarget
{
public:
    /*!
    \brief
        OpenGLViewportTarget default constructor.  Constructs a view port
        target that initially has the same size and position as the current
        OpenGL view port.
    */
    OpenGLViewportTarget();

    /*!
    \brief
        OpenGLViewportTarget constructor.  Constructs a view port target that
        initially has the size and position as detailed in the specified Rect.

    \param area
        Initial area to be used for the viewport.
    */
    OpenGLViewportTarget(const Rect& area);

    /*!
    \brief
        OpenGLViewportTarget destructor.
    */
    virtual ~OpenGLViewportTarget();

    // interface implementation
    virtual void queueToTarget(const Rect& src, const Rect& dst, float z,
                               const ColourRect& cols,
                               const QuadSplitMode quad_split_mode,
                               RenderTarget& target);
    virtual void renderToTarget(const Rect& src, const Rect& dst, float z,
                                const ColourRect& cols,
                                const QuadSplitMode quad_split_mode,
                                RenderTarget& target);
    virtual void setArea(const Rect& area);
    virtual void declareRenderSize(const Size& sz);
    virtual bool isImageryCache() const;
};


} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLViewportTarget_h_
