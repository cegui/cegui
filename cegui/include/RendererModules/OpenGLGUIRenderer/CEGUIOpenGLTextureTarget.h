/***********************************************************************
    filename:   CEGUIOpenGLTextureTarget.h
    created:    Mon Feb 18 2008
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
#ifndef _CEGUIOpenGLTextureTarget_h_
#define _CEGUIOpenGLTextureTarget_h_

#include "CEGUIOpenGLRenderTarget.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    OpenGLTextureTarget - intermediate class containing things common to
    texture based render targets, that do not belong in OpenGLRenderTarget.
*/
class OPENGL_GUIRENDERER_API OpenGLTextureTarget : public OpenGLRenderTarget
{
public:
    // implementation of parts of the interface
    virtual void queueToTarget(const Rect& src, const Rect& dst, float z,
                               const ColourRect& cols,
                               const QuadSplitMode quad_split_mode,
                               RenderTarget& target);
    virtual void renderToTarget(const Rect& src, const Rect& dst, float z,
                                const ColourRect& cols,
                                const QuadSplitMode quad_split_mode,
                                RenderTarget& target);
    virtual void setArea(const Rect& area);
    virtual bool isImageryCache() const;

protected:
    //! Default dimension texture.
    static const float DEFAULT_SIZE;

    // this class is not currently intended to be used directly
    OpenGLTextureTarget();
    ~OpenGLTextureTarget();

    /*!
    \brief
        Discovers capabilites and stores results.
    */
    void discoverTextureCapabilities();

    /*!
    \brief
        Given the Size \a sz return a Size object containing the sizes
        adjusted as required by the limits of the system.

    \exception InvalidRequestException
        thrown if the adjusted size would be too large according to what GL
        reports as the likely maximum texture size.
    */
    Size getAdjustedTextureSize(const Size& sz) const;

    /*!
    \brief
        Takes the pixel area Rect \a pix_rect and converts its co-ordinates into
        texture co-ordinates based on the dimensions of this RenderTarget and
        stores the results in \a tex_rect.
    */
    void initTexRectFromPixRect(const Rect& pix_rect, Rect& tex_rect) const;

    //! Associated OpenGL texture ID
    GLuint d_texture;
    //! whether textures can be non-power of two
    bool d_supportsNPOT;
    //! whether textures have to be square
    bool d_mustBeSquare;
    //! (approximate) maximum texture size supported.
    GLint  d_maxTextureSize;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLTextureTarget_h_
