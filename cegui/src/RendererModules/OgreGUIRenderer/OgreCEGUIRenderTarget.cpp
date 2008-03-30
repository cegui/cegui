/***********************************************************************
    filename:   OgreCEGUIRenderTarget.cpp
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
#include "OgreCEGUIRenderTarget.h"
#include "OgreCEGUIRenderQueueListener.h"
#include "OgreCEGUITexture.h"
#include <CEGUI/CEGUIColourRect.h>
#include <CEGUI/CEGUIExceptions.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const Ogre::LayerBlendModeEx OgreRenderTarget::COLOUR_BLEND_MODE =
{
    Ogre::LBT_COLOUR,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE
};

//----------------------------------------------------------------------------//
const Ogre::LayerBlendModeEx OgreRenderTarget::ALPHA_BLEND_MODE =
{
    Ogre::LBT_ALPHA,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE
};

//----------------------------------------------------------------------------//
const Ogre::TextureUnitState::UVWAddressingMode
    OgreRenderTarget::TEXTURE_ADDRESS_MODE =
{
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP
};


//----------------------------------------------------------------------------//
OgreRenderTarget::OgreRenderTarget() :
    d_queueEnabled(true),
    d_directRendering(false),
    d_ogreRoot(Ogre::Root::getSingletonPtr()),
    d_ogreRenderSystem(d_ogreRoot->getRenderSystem()),
    d_ogreViewport(0),
    d_texelOffset(d_ogreRenderSystem->getHorizontalTexelOffset(),
                  -d_ogreRenderSystem->getVerticalTexelOffset())
{
    createRenderOpBuffers();
}

//----------------------------------------------------------------------------//
OgreRenderTarget::~OgreRenderTarget()
{
    // cleanup buffers etc
    cleanupRenderOpContext(d_queueRenderOp);
    cleanupRenderOpContext(d_immediateRenderOp);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::queueQuad(const Rect& dest, float z,
    const Texture* tex, const Rect& tex_rect, const ColourRect& cols,
    const QuadSplitMode quad_split_mode)
{
    if (!d_queueEnabled)
    {
        renderQuad(dest, z, tex, tex_rect, cols, quad_split_mode);
        return;
    }

    Quad quad;
    // set up the quad struct with the input data
    _initialise_quad(quad, dest, z,
                     static_cast<const OgreCEGUITexture*>(tex)->getOgreTexture(),
                     tex_rect, quad_split_mode, cols);

    // add quad to list (insertion sorted)
    _queue_quad(quad);

    // will now need to re-put data to vertex buffer
    d_queueRenderOp.buffer_data_valid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::renderQuad(const Rect& dest, float z,
    const Texture* tex, const Rect& tex_rect, const ColourRect& cols,
    const QuadSplitMode quad_split_mode)
{
    Quad quad;
    // set up the quad struct with the input data
    _initialise_quad(quad, dest, z,
                     static_cast<const OgreCEGUITexture*>(tex)->getOgreTexture(),
                     tex_rect, quad_split_mode, cols);

    _render_quad(quad);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::execute(void)
{
    updateRenderOpVBSize(d_queueRenderOp, d_quadlist.size());

    beginRender();
        renderQuadList(d_queueRenderOp, d_quadlist.begin(), d_quadlist.end());
    endRender();
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::clearRenderList(void)
{
    d_quadlist.clear();
    d_queueRenderOp.buffer_data_valid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::beginDirectRendering()
{
    if (!d_directRendering)
    {
        d_directRendering = true;
    }
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::endDirectRendering()
{
    if (d_directRendering)
    {
        updateRenderOpVBSize(d_immediateRenderOp, d_immediateQueue.size());

        // switch to viewport for this target
        d_ogreRenderSystem->_setViewport(d_ogreViewport);
        // render quads added to the 'immediate' queue.
        renderQuadList(d_immediateRenderOp,
                       d_immediateQueue.begin(),
                       d_immediateQueue.end());

        d_immediateQueue.clear();

        d_directRendering = false;
        d_immediateRenderOp.buffer_data_valid = false;
    }
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::enableQueue(bool enable)
{
    d_queueEnabled = enable;
    d_queueEnabled ? endDirectRendering() : beginDirectRendering();
}

//----------------------------------------------------------------------------//
const Rect& OgreRenderTarget::getArea() const
{
    return d_area;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::initialiseRenderState()
{
    using namespace Ogre;

    // set-up matrices
    d_ogreRenderSystem->_setWorldMatrix(Matrix4::IDENTITY);
    d_ogreRenderSystem->_setViewMatrix(Matrix4::IDENTITY);
    d_ogreRenderSystem->_setProjectionMatrix(Matrix4::IDENTITY);

    // initialise render settings
    d_ogreRenderSystem->setLightingEnabled(false);
    d_ogreRenderSystem->_setDepthBufferParams(false, false);
    d_ogreRenderSystem->_setDepthBias(0, 0);
    d_ogreRenderSystem->_setCullingMode(CULL_NONE);
    d_ogreRenderSystem->_setFog(FOG_NONE);
    d_ogreRenderSystem->_setColourBufferWriteEnabled(true, true, true, true);
    d_ogreRenderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);
    d_ogreRenderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
    d_ogreRenderSystem->setShadingType(SO_GOURAUD);
    d_ogreRenderSystem->_setPolygonMode(PM_SOLID);

    // initialise texture settings
    d_ogreRenderSystem->_setTextureCoordCalculation(0, TEXCALC_NONE);
    d_ogreRenderSystem->_setTextureCoordSet(0, 0);
    d_ogreRenderSystem->_setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR,
                                                 FO_POINT);
    d_ogreRenderSystem->_setTextureAddressingMode(0, TEXTURE_ADDRESS_MODE);
    d_ogreRenderSystem->_setTextureMatrix(0, Matrix4::IDENTITY);
    d_ogreRenderSystem->_setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0);
    d_ogreRenderSystem->_setTextureBlendMode(0, COLOUR_BLEND_MODE);
    d_ogreRenderSystem->_setTextureBlendMode(0, ALPHA_BLEND_MODE);
    d_ogreRenderSystem->_disableTextureUnitsFrom(1);

    // enable alpha blending
    d_ogreRenderSystem->_setSceneBlending(SBF_SOURCE_ALPHA,
                                          SBF_ONE_MINUS_SOURCE_ALPHA);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::_queue_quad(const Quad& quad)
{
    d_quadlist.insert(quad);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::_render_quad(const Quad& quad)
{
    d_immediateQueue.push_back(quad);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::_initialise_quad(Quad& quad, const Rect& dest,
    float z, const Ogre::TexturePtr tex, const Rect& tex_rect,
    const QuadSplitMode split, const ColourRect& cols)
{
    // set quad position, flipping y co-ordinates.
    quad.position.d_left    = dest.d_left;
    quad.position.d_right   = dest.d_right;
    quad.position.d_top     = d_area.getHeight() - dest.d_top;
    quad.position.d_bottom  = d_area.getHeight() - dest.d_bottom;
    // apply appropriate texel origin offset
    quad.position.offset(d_texelOffset);

    // convert quad co-ordinates for a -1 to 1 co-ordinate system.
    quad.position.d_left    /= (d_area.getWidth() * 0.5f);
    quad.position.d_right   /= (d_area.getWidth() * 0.5f);
    quad.position.d_top     /= (d_area.getHeight() * 0.5f);
    quad.position.d_bottom  /= (d_area.getHeight() * 0.5f);
    quad.position.offset(Point(-1.0f, -1.0f));

    // set texture details
    quad.texture        = tex;
    quad.texPosition    = tex_rect;

    // convert and set z value.
    quad.z              = -1 + z;

    // covert colours for ogre, note that top / bottom are switched.
    quad.topLeftCol     = colourToOgre(cols.d_bottom_left);
    quad.topRightCol    = colourToOgre(cols.d_bottom_right);
    quad.bottomLeftCol  = colourToOgre(cols.d_top_left);
    quad.bottomRightCol = colourToOgre(cols.d_top_right);

    // set quad split mode
    quad.splitMode = split;
}

//----------------------------------------------------------------------------//
uint32 OgreRenderTarget::colourToOgre(const colour& col) const
{
    Ogre::ColourValue ocv(col.getRed(),
                          col.getGreen(),
                          col.getBlue(),
                          col.getAlpha());

    uint32 final;
    d_ogreRenderSystem->convertColourValue(ocv, &final);

    return final;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::updateRenderOpVBSize(
    RenderOpContext& roc, size_t quad_count)
{
    size_t size = roc.buffer->getNumVertices();
    size_t requestedSize = quad_count * VERTEX_PER_QUAD;

    if (size < requestedSize)
    {
        // Double buffer size until at least the requested size
        while (size < requestedSize)
            size = size * 2;
    }
    else if (requestedSize < size / 2 &&
             roc.underused_framecount >= UNDERUSED_FRAME_THRESHOLD)
    {
        // Resize vertex buffer if it has been to big for too long
        size = size / 2;
        // Reset underused framecount so it takes another
        // UNDERUSED_FRAME_THRESHOLD to half again
        roc.underused_framecount = 0;
    }
    else
        return;

    // Reallocate the buffer
    cleanupRenderOpContext(roc);
    initialiseRenderOpContext(roc, size);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::stuffQuad(const Quad& quad, QuadVertex* buffer)
{
    // setup Vertex 1...
    (*buffer).x = quad.position.d_left;
    (*buffer).y = quad.position.d_bottom;
    (*buffer).z = quad.z;
    (*buffer).diffuse = quad.topLeftCol;
    (*buffer).tu1 = quad.texPosition.d_left;
    (*buffer).tv1 = quad.texPosition.d_bottom;
    ++buffer;

    // setup Vertex 2...
    (*buffer).x = quad.position.d_right;
    (*buffer).z = quad.z;
    (*buffer).tu1 = quad.texPosition.d_right;

    // top-left to bottom-right diagonal
    if (quad.splitMode == TopLeftToBottomRight)
    {
        (*buffer).y = quad.position.d_bottom;
        (*buffer).diffuse = quad.topRightCol;
        (*buffer).tv1 = quad.texPosition.d_bottom;
    }
    // bottom-left to top-right diagonal
    else
    {
        (*buffer).y = quad.position.d_top;
        (*buffer).diffuse = quad.bottomRightCol;
        (*buffer).tv1 = quad.texPosition.d_top;
    }
    ++buffer;

    // setup Vertex 3...
    (*buffer).x = quad.position.d_left;
    (*buffer).y = quad.position.d_top;
    (*buffer).z = quad.z;
    (*buffer).diffuse = quad.bottomLeftCol;
    (*buffer).tu1 = quad.texPosition.d_left;
    (*buffer).tv1 = quad.texPosition.d_top;
    ++buffer;

    // setup Vertex 4...
    (*buffer).x = quad.position.d_right;
    (*buffer).y = quad.position.d_bottom;
    (*buffer).z = quad.z;
    (*buffer).diffuse = quad.topRightCol;
    (*buffer).tu1 = quad.texPosition.d_right;
    (*buffer).tv1 = quad.texPosition.d_bottom;
    ++buffer;

    // setup Vertex 5...
    (*buffer).x = quad.position.d_right;
    (*buffer).y = quad.position.d_top;
    (*buffer).z = quad.z;
    (*buffer).diffuse = quad.bottomRightCol;
    (*buffer).tu1 = quad.texPosition.d_right;
    (*buffer).tv1 = quad.texPosition.d_top;
    ++buffer;

    // setup Vertex 6...
    (*buffer).x = quad.position.d_left;
    (*buffer).z = quad.z;
    (*buffer).tu1 = quad.texPosition.d_left;

    // top-left to bottom-right diagonal
    if (quad.splitMode == TopLeftToBottomRight)
    {
        (*buffer).y = quad.position.d_top;
        (*buffer).diffuse = quad.bottomLeftCol;
        (*buffer).tv1 = quad.texPosition.d_top;
    }
    // bottom-left to top-right diagonal
    else
    {
        (*buffer).y = quad.position.d_bottom;
        (*buffer).diffuse = quad.topLeftCol;
        (*buffer).tv1 = quad.texPosition.d_bottom;
    }
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::initialiseRenderOpContext(RenderOpContext& roc,
    size_t buffSize)
{
    using namespace Ogre;
    // Create and initialise the Ogre specific parts required for rendering.
    roc.render_op.vertexData = new VertexData;
    roc.render_op.vertexData->vertexStart = 0;

    // setup vertex declaration for the vertex format we use
    VertexDeclaration* vd = roc.render_op.vertexData->vertexDeclaration;
    size_t vd_offset = 0;
    vd->addElement(0, vd_offset, VET_FLOAT3, VES_POSITION);
    vd_offset += VertexElement::getTypeSize(VET_FLOAT3);
    vd->addElement(0, vd_offset, VET_COLOUR, VES_DIFFUSE);
    vd_offset += VertexElement::getTypeSize(VET_COLOUR);
    vd->addElement(0, vd_offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);

    // create hardware vertex buffer
    roc.buffer = HardwareBufferManager::getSingleton().createVertexBuffer(
        vd->getVertexSize(0), buffSize,
        HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false);

    // bind vertex buffer
    roc.render_op.vertexData->vertexBufferBinding->setBinding(0, roc.buffer);

    // complete render operation basic initialisation
    roc.render_op.operationType = RenderOperation::OT_TRIANGLE_LIST;
    roc.render_op.useIndexes = false;

    roc.underused_framecount = 0;
    roc.buffer_data_valid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::cleanupRenderOpContext(RenderOpContext& roc)
{
    delete roc.render_op.vertexData;
    roc.render_op.vertexData = 0;
    roc.buffer.setNull();
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::setViewport(Ogre::Viewport* viewport)
{
    d_ogreViewport = viewport;
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::initialiseTargetArea()
{
    // setup area from viewport
    d_area.d_left   = d_ogreViewport->getActualLeft();
    d_area.d_top    = d_ogreViewport->getActualTop();
    d_area.setSize(Size(d_ogreViewport->getActualWidth(),
                        d_ogreViewport->getActualHeight()));
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::createRenderOpBuffers()
{
    // init render ops / buffers
    initialiseRenderOpContext(d_queueRenderOp, VERTEXBUFFER_INITIAL_CAPACITY);
    initialiseRenderOpContext(d_immediateRenderOp, 64);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::beginRender()
{
    // switch to viewport for this target
    d_ogreRenderSystem->_setViewport(d_ogreViewport);
}

//----------------------------------------------------------------------------//
void OgreRenderTarget::endRender()
{
    // nothing done here by default, subclasses may need to do something though,
    // so this mechanism is provided.
}

} // End of  CEGUI namespace section
