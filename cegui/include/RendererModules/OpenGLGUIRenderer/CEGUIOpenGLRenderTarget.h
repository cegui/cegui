/***********************************************************************
    filename:   CEGUIOpenGLRenderTarget.h
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
#ifndef _CEGUIOpenGLRenderTarget_h_
#define _CEGUIOpenGLRenderTarget_h_

#include "CEGUIRenderTarget.h"
#include "openglrenderer.h"
#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    OpenGLRenderTarget - common base class for OpenGL render targets.
*/
class OPENGL_GUIRENDERER_API OpenGLRenderTarget : public RenderTarget
{
public:
    /*!
    \brief
        OpenGLRenderTarget destructor.
    */
    virtual ~OpenGLRenderTarget();

    // implementation of abstract base members
    virtual void queueQuad(const Rect& dest, float z, const Texture* tex,
                           const Rect& tex_rect, const ColourRect& cols,
                           const QuadSplitMode quad_split_mode);
    virtual void renderQuad(const Rect& dest, float z, const Texture* tex,
                            const Rect& tex_rect, const ColourRect& cols,
                            const QuadSplitMode quad_split_mode);
    virtual void clearRenderList(void);
    virtual void execute(void);
    virtual void beginDirectRendering();
    virtual void endDirectRendering();
    virtual void enableQueue(bool enable);
    virtual const Rect& getArea() const;

protected:
    /************************************************************************
        Constants
    ************************************************************************/
    //!< number of vertices per quad
    static const int VERTEX_PER_QUAD = 6;
    //!< number of vertices for a triangle
    static const int VERTEX_PER_TRIANGLE = 3;
    //!< capacity of the allocated vertex buffer
    static const int VERTEX_BUFFER_CAPACITY = 3000;  // 3000 / 6 == 500 quads

    /*************************************************************************
        Structs & classes
    *************************************************************************/
    struct Vertex
    {
        float tex[2];
        uint32 color;
        float vertex[3];
    };

    struct Quad
    {
        GLuint texid;
        Rect position;
        float z;
        Rect texPosition;
        QuadSplitMode quadSplitMode;
        uint32 topLeftCol;
        uint32 topRightCol;
        uint32 bottomLeftCol;
        uint32 bottomRightCol;

        bool operator<(const Quad& other) const
        {
            // this is intentionally reversed.
            return z > other.z;
        }
    };

    /*************************************************************************
        Functions
    *************************************************************************/
    /*!
    \brief
        OpenGLRenderTarget constructor.  This is protected; you do not create
        instances of this class.
    */
    OpenGLRenderTarget();
    // render whatever is in d_vertBuff
    virtual void renderVertexBuffer();
    // initialise render states
    virtual void beginRenderState();
    // restore old state / clean up
    virtual void endRenderState();
    // stuff data from quad into vertex buffer.
    void stuffQuad(const Quad& quad, Vertex* vbuffer);
    // render quads defined between the two iterators
    template<typename T>
    void renderQuadList(const T& beg, const T& end);

    /*************************************************************************
        Data
    *************************************************************************/
    //! Type to use for quad queue collection.
    typedef std::multiset<Quad> QuadList;
    //! Collection of quads queued to this RenderTarget.
    QuadList d_quadlist;
    //! the 'immediate queue' - not immediate at all then :-p
    std::vector<Quad> d_immediateQueue;
    //! Rect describing area for this render target.
    Rect d_area;
    //! Currently bound texture.
    uint d_boundTexture;
    //! vertex buffer used when performing OpenGL rendering.
    Vertex d_vertBuff[VERTEX_BUFFER_CAPACITY];
    //! index into buffer where next vertex should be put.
    int d_bufferIdx;
    //! queuing enabled state
    bool d_queueEnabled;
    //! stores whether 'direct rendering' mode has been entered.
    bool d_directRendering;

public:
    /*************************************************************************
        internal helper funcs.  These are public, but should not be used
        externally; since parameter type Quad is protected, you can't anyway.
    *************************************************************************/
    // add quad to the list of quads to render when execute is called
    void _queue_quad(const Quad& quad);
    // render quad to the target.  Should be within begin/endDirectRendering.
    void _render_quad(const Quad& quad);
    // initialise quad struct with details
    void _initialise_quad(Quad& quad, const Rect& dest, float z,
                          const GLuint tex, const Rect& tex_rect,
                          const QuadSplitMode split, const ColourRect& cols);
};

//----------------------------------------------------------------------------//
template<typename T>
void OpenGLRenderTarget::renderQuadList(const T& beg, const T& end)
{
    glInterleavedArrays(GL_T2F_C4UB_V3F, 0, d_vertBuff);

    // iterate over each quad in the list
    for (T i = beg; i != end; ++i)
    {
        const Quad& quad = *i;

        // if texture is changing, render what we have for this texture
        if (d_boundTexture != quad.texid)
        {
            renderVertexBuffer();
            glBindTexture(GL_TEXTURE_2D, quad.texid);
            d_boundTexture = quad.texid;
        }

        // stuff this quad into the buffer
        stuffQuad(quad, &d_vertBuff[d_bufferIdx]);
        // advance index / counter by number of vertices for a quad.
        d_bufferIdx += VERTEX_PER_QUAD;

        // if buffer is full or will overflow next quad, render it's content
        if (d_bufferIdx > (VERTEX_BUFFER_CAPACITY - VERTEX_PER_QUAD))
            renderVertexBuffer();
    }

    // render anything remaining in the buffer
    renderVertexBuffer();
}

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOpenGLRenderTarget_h_
