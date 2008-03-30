/***********************************************************************
    filename:   OgreCEGUIRenderTarget.h
    created:    Sun Mar 9 2008
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
#ifndef _OgreCEGUIRenderTarget_h_
#define _OgreCEGUIRenderTarget_h_

#include <CEGUI/CEGUIRenderTarget.h>
#include "OgreCEGUIRenderer.h"

#include <OgreColourValue.h>
#include <OgreHardwareBufferManager.h>
#include <OgreRenderSystem.h>
#include <OgreTexture.h>
#include <OgreTextureUnitState.h>

// Start of CEGUI namespace section
namespace CEGUI
{
/**
 * Common base class for RenderTarget objects implemented via the Ogre engine.
 */
class OGRE_GUIRENDERER_API OgreRenderTarget : public RenderTarget
{
public:
    /**
     * Destructor for OgreRenderTarget objects
     */
    virtual ~OgreRenderTarget();

    // interface from RenderTarget we are required to implement
    virtual void queueQuad(const Rect& dest, float z, const Texture* tex,
                           const Rect& tex_rect, const ColourRect& cols,
                           const QuadSplitMode quad_split_mode);
    virtual void renderQuad(const Rect& dest, float z, const Texture* tex,
                           const Rect& tex_rect, const ColourRect& cols,
                           const QuadSplitMode quad_split_mode);
    virtual void execute(void);
    virtual void clearRenderList(void);
    virtual void beginDirectRendering();
    virtual void endDirectRendering();
    virtual void enableQueue(bool enable);
    virtual const Rect& getArea() const;

protected:
    /*************************************************************************
        Constants
    *************************************************************************/
    /// number of vertices per quad.
    static const size_t VERTEX_PER_QUAD = 6;
    /// initial capacity of the allocated vertex buffer.
    static const size_t VERTEXBUFFER_INITIAL_CAPACITY = 256;
    /// number of frames to wait before shrinking buffer.
    static const size_t UNDERUSED_FRAME_THRESHOLD = 50000;
    /// Colour blending mode used.
    static const Ogre::LayerBlendModeEx COLOUR_BLEND_MODE;
    /// Alpha blending mode used.
    static const Ogre::LayerBlendModeEx ALPHA_BLEND_MODE;
    /// Texture addressing mode used.
    static const Ogre::TextureUnitState::UVWAddressingMode TEXTURE_ADDRESS_MODE;

    /*************************************************************************
        Internally used structs
    *************************************************************************/
    /// Structure used for vertices passed to Ogre.
    struct QuadVertex {
        /// position for the vertex.
        float x, y, z;
        /// diffuse colour applied to the vertex.
        Ogre::RGBA diffuse;
        /// texture coordinates.
        float tu1, tv1;
    };

    /// Structure used to hold details for a queued quad.
    struct Quad
    {
        /// texture the quad uses.
        Ogre::TexturePtr texture;
        /// pixel position of the quad on the target.
        Rect position;
        /// z order of the quad.
        float z;
        /// area of texture to be applied.
        Rect texPosition;
        /// colour to use for top-left vertex.
        uint32 topLeftCol;
        /// colour to use for top-right vertex.
        uint32 topRightCol;
        /// colour to use for bottom-left vertex.
        uint32 bottomLeftCol;
        /// colour to use for bottom-right vertex.
        uint32 bottomRightCol;
        /// defines how the quad is aplit into triangles.
        QuadSplitMode splitMode;

        /**
         * less than operator.
         * Predicate function used to keep queued quads sorted.
         */
        bool operator<(const Quad& other) const
        {
            // this is intentionally reversed.
            return z > other.z;
        }
    };

    /// Structure used to track details about a render op and vertex buffer
    struct RenderOpContext
    {
        /// Ogre render operation.
        Ogre::RenderOperation render_op;
        /// Ogre hardware vb used with this rendering op.
        Ogre::HardwareVertexBufferSharedPtr buffer;
        /// count of consecutive frames where buffer was 50% empty (or more)
        size_t underused_framecount;
        /// indicates whether vertex data in the hw buffer is still valid.
        bool buffer_data_valid;
    };

    /*************************************************************************
        Implementation member functions
    *************************************************************************/
    /**
     * Protected constructor for OgreRenderTarget objects that should never be
     * instanced as a type.
     */
    OgreRenderTarget();

    /**
     * Initialise the Ogre render system ready for CEGUI rendering
     */
    virtual void initialiseRenderState();

    /**
     * Convert a CEGUI::colour into something we can use with Ogre.
     * @param col CEGUI::colour value.
     * @return unsigned 32 bit value representing a colour that Ogre can use.
     */
    uint32 colourToOgre(const colour& col) const;

    /**
     * render quads defined between the two iterators
     */
    template<typename T>
    void renderQuadList(RenderOpContext& roc, const T& beg, const T& end);

    /**
     * Function that dynamically resizes the hardware vb associated with a
     * given RenderOpContext according to the given quad count and previous use.
     * @param roc RenderOpContext object.
     * @param quad_count number of quads we intend to draw next time around.
     */
    void updateRenderOpVBSize(RenderOpContext& roc, size_t quad_count);

    /**
     * Transfer data from the Quad structure into the vertex buffer.
     * @param quad Quad struct containing the queued information.
     * @param buffer pointer to vertx buffer to receive data.
     */
    void stuffQuad(const Quad& quad, QuadVertex* buffer);

    /**
     * Clean up resources associated with the given RenderOpContext.
     * @param roc RenderOpContext to be cleaned up.
     */
    void cleanupRenderOpContext(RenderOpContext& roc);

    /**
     * Initialise buffers and other objects for the given RenderOpContext and
     * buffer size.
     * @param roc RenderOpContext to be initialised.
     * @param buffSize Initial size to use for the vertex buffer associated with
     * the RenderOpContext.
     */
    void initialiseRenderOpContext(RenderOpContext& roc, size_t buffSize);

    /**
     * Perform any initialisation tasks prior to rendering.
     * @note this is only called for normal queued rendering; it is not called
     * when using direct rendering.
     */
    virtual void beginRender();

    /**
     * Perform any cleanup / finalisation tasks after rendering.
     * @note this is only called for normal queued rendering; it is not called
     * when using direct rendering.
     */
    virtual void endRender();

    /**
     * Set the viewport for this RenderTarget.  Call effectively initialises the
     * d_ogreViewport and d_ogreRenderTarget members.
     * @note This is only really intended to be called at construction time.
     */
    void setViewport(Ogre::Viewport* viewport);

    /**
     * Initialise the d_area member according to values obtained from the
     * Ogre::Viewport associated with this RenderTarget.
     */
    void initialiseTargetArea();

    /**
     * Create buffers and initialise the RenderOpContexts for this RenderTarget.
     */
    void createRenderOpBuffers();

    /*************************************************************************
        Data memebers
    *************************************************************************/
    /// type to use for collection of Quad structures
    typedef std::multiset<Quad> QuadList;
    /// Collection of currently queued quads.
    QuadList d_quadlist;
    //! the 'immediate queue' (allows some batching of non-queued quads).
    std::vector<Quad> d_immediateQueue;
    /// specifies whether quads are being queued or not.
    bool d_queueEnabled;
    /// specifies whether 'direct rendering' mode is in effect
    bool d_directRendering;
    /// Rect describing area for this render target.
    Rect d_area;
    /// pointer to the Ogre root object.
    Ogre::Root* d_ogreRoot;
    /// Pointer to the Ogre render system.
    Ogre::RenderSystem* d_ogreRenderSystem;
    /// Viewport we use to render into d_ogreRenderTarget;
    Ogre::Viewport* d_ogreViewport;
    /// Offset required for correct texel to pixel mapping.
    Point d_texelOffset;
    /// RenderOpContext for the main rendering queue.
    RenderOpContext d_queueRenderOp;
    /// RenderOpContext for the 'immediate' render queue.
    RenderOpContext d_immediateRenderOp;

public:
    /*************************************************************************
        internal helper funcs.  These are public, but should not be used
        externally; since parameter type Quad is protected, you can't anyway.
    *************************************************************************/
    /// add quad to the list of quads to render when execute is called
    void _queue_quad(const Quad& quad);
    /// render quad to the target.  Should be within begin/endDirectRendering.
    void _render_quad(const Quad& quad);
    /// initialise quad struct with details
    void _initialise_quad(Quad& quad, const Rect& dest, float z,
                          const Ogre::TexturePtr tex, const Rect& tex_rect,
                          const QuadSplitMode split, const ColourRect& cols);
};

//----------------------------------------------------------------------------//
template<typename T>
void OgreRenderTarget::renderQuadList(RenderOpContext& roc, const T& beg,
    const T& end)
{
    if(!roc.buffer_data_valid)
    {
        QuadVertex* buffmem =
            static_cast<QuadVertex*>(roc.buffer->lock(
                Ogre::HardwareVertexBuffer::HBL_DISCARD));

        // iterate over each quad in the list
        for (T i = beg; i != end; ++i)
        {
            const Quad& quad = *i;
            stuffQuad(quad, buffmem);
            buffmem += VERTEX_PER_QUAD;
        }

        // ensure we leave the buffer in the unlocked state
        roc.buffer->unlock();
        // mark buffer data as valid
        roc.buffer_data_valid = true;
    }

    // Render the buffer
    size_t pos = 0;
    bool first = true;

    // Iterate over each quad in the list and render it
    T i = beg;
    while(i != end)
    {
        Ogre::TexturePtr tex = i->texture;
        roc.render_op.vertexData->vertexStart = pos;

        for (; i != end; ++i)
        {
            const Quad& quad = *i;

            if (tex != quad.texture)
            {
                // If it has a different texture, render this quad in next
                // operation.  Also need to reset render states.
                first = true;
                break;
            }

            pos += VERTEX_PER_QUAD;
        }

        roc.render_op.vertexData->vertexCount =
            pos - roc.render_op.vertexData->vertexStart;

        // Set texture, and do the render
        d_ogreRenderSystem->_setTexture(0, true, tex);

        if (first)
        {
            initialiseRenderState();
            first = false;
        }

        d_ogreRenderSystem->_render(roc.render_op);
    }

    // Count frames to check if utilization of vertex buffer was below half the
    // capacity for UNDERUSED_FRAME_THRESHOLD frames
    if(pos < roc.buffer->getNumVertices() / 2)
       roc.underused_framecount++;
    else
       roc.underused_framecount = 0;
}

} // End of  CEGUI namespace section

#endif  // end of guard _OgreCEGUIRenderTarget_h_
