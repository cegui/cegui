/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Henri I Hyyryläinen (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Ogre/GeometryBuffer.h"
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/RendererModules/Ogre/ShaderWrapper.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/RenderEffect.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ShaderParameterBindings.h"

#include <OgreRenderSystem.h>
#include <OgreHardwareBufferManager.h>
#include <OgreQuaternion.h>
#include <OgreManualObject.h>
#include <OgreRenderOperation.h>
#include <OgreSceneManager.h>

#ifdef CEGUI_USE_OGRE_HLMS
#include <OgreHlmsSamplerblock.h>
#include <OgreRenderTarget.h>
#include <OgreViewport.h>
#endif

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp"

#define FLOATS_PER_TEXTURED     9
#define FLOATS_PER_COLOURED     7

// Start of CEGUI namespace section
namespace CEGUI
{

OgreGeometryBuffer::OgreGeometryBuffer(OgreRenderer& owner, 
    Ogre::RenderSystem& rs, CEGUI::RefCounted<RenderMaterial> renderMaterial) :
    GeometryBuffer(renderMaterial),
    d_owner(owner),
    d_renderSystem(rs),
    d_matrix(1.0),
    d_expectedData(MT_INVALID),
    d_dataAppended(false),
    d_previousAlphaValue(-1.f)
{

}

//----------------------------------------------------------------------------//
OgreGeometryBuffer::~OgreGeometryBuffer()
{
    cleanUpVertexAttributes();
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::draw() const
{
    if (d_vertexData.empty())
        return;

    if (d_dataAppended)
        syncVertexData();

    if (d_hwBuffer.isNull())
        return;

#ifdef CEGUI_USE_OGRE_HLMS
    //Ogre::Viewport* previousViewport = d_renderSystem._getViewport();
    //Ogre::Viewport* currentViewport = d_owner.getOgreRenderTarget()->getViewport(0);
    
    // If this viewport approach fails we'll probably need to mess with the
    // set hlms macro block in the Renderer
    Ogre::Viewport* currentViewport = d_renderSystem._getViewport();

    Rectf previousClipRect;
    previousClipRect.left(currentViewport->getScissorLeft());
    previousClipRect.top(currentViewport->getScissorTop());
    previousClipRect.right(currentViewport->getScissorWidth());
    previousClipRect.bottom(currentViewport->getScissorHeight());
#endif //CEGUI_USE_OGRE_HLMS
    
    // setup clip region
    if (d_clippingActive)
    {
    #ifdef CEGUI_USE_OGRE_HLMS
        setScissorRects(currentViewport);
    #else
        setScissorRects();
    #endif //CEGUI_USE_OGRE_HLMS
    }

    // Update the matrix
    updateMatrix();

    CEGUI::ShaderParameterBindings* shaderParameterBindings = 
        (*d_renderMaterial).getShaderParamBindings();

    // Set the ModelViewProjection matrix in the bindings
    shaderParameterBindings->setParameter("modelViewProjMatrix", d_matrix);


    if (d_alpha != d_previousAlphaValue)
    {
        d_previousAlphaValue = d_alpha;

        shaderParameterBindings->setParameter("alphaPercentage", 
            d_previousAlphaValue);
    }

    // activate the desired blending mode
    d_owner.bindBlendMode(d_blendMode);

    // get the impl specific shader wrapper so we can set the necessary render ops in the draw passes.
    OgreShaderWrapper *shaderWrapper = static_cast<OgreShaderWrapper*>( const_cast<ShaderWrapper*>( d_renderMaterial->getShaderWrapper() ) );

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        //Prepare for the rendering process according to the used render material

        // Starting with Ogre 2.1 this also activates the PSO
        // which contains all the blend settings etc
        // This is required because the material cannot set shader parameters before
        // the PSO is bound

        shaderWrapper->setRenderOperation( d_renderOp );
        d_renderMaterial->prepareForRendering();

        // draw the geometry
        d_renderSystem._render(d_renderOp);
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();

    // Disable clipping after rendering
    if (d_clippingActive)
    {
    #ifdef CEGUI_USE_OGRE_HLMS
        currentViewport->setScissors(previousClipRect.left(), previousClipRect.top(),
            previousClipRect.right(), previousClipRect.bottom());
        // Restore viewport? d_renderSystem._setViewport(previousViewport);
    #else
        d_renderSystem.setScissorTest(false);
    #endif //CEGUI_USE_OGRE_HLMS
    }

    updateRenderTargetData(d_owner.getActiveRenderTarget());
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::appendGeometry(const float* vertex_data, std::size_t array_size)
{
    GeometryBuffer::appendGeometry(vertex_data, array_size);

    d_dataAppended = true;
}

void OgreGeometryBuffer::syncVertexData() const
{
    if (!d_dataAppended)
        return;

    // Make sure that our vertex buffer is large enough
    size_t current_size;

    if (!d_hwBuffer.isNull() &&
        (current_size = d_hwBuffer->getNumVertices()) < d_vertexCount)
    {
        size_t new_size = current_size;

        // Reallocate the buffer to be large enough
        while(new_size < d_vertexCount)
            new_size *= 2;

        setVertexBuffer(new_size);
    }

    // copy vertex data into the Ogre hardware buffer
    if (d_vertexCount > 0)
    {
        if (d_hwBuffer.isNull())
        {

            setVertexBuffer(d_vertexCount);
        }
    #ifdef CEGUI_USE_OGRE_HLMS
        void* copy_target = d_hwBuffer->lock(
            Ogre::v1::HardwareVertexBuffer::HBL_DISCARD);
    #else
        void* copy_target = d_hwBuffer->lock(
            Ogre::HardwareVertexBuffer::HBL_DISCARD); 
    #endif //CEGUI_USE_OGRE_HLMS

        assert(copy_target && "Ogre vertex buffer is invalid");

        std::memcpy(copy_target, &d_vertexData[0], d_vertexData.size()*
            sizeof(float));

        d_hwBuffer->unlock();
    }

    // Update rendering for the new vertices
    d_renderOp.vertexData->vertexStart = 0;
    d_renderOp.vertexData->vertexCount = d_vertexCount;

    d_dataAppended = false;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::updateMatrix() const
{
    d_matrixValid = false;
    if ( !d_matrixValid || !isRenderTargetDataValid(d_owner.getActiveRenderTarget()) )
    {
        // Apply the view projection matrix to the model matrix and save the result as cached matrix
        d_matrix = d_owner.getViewProjectionMatrix() * getModelMatrix();

        //If necessary: transpose
        const OgreShaderWrapper* ogreShader = static_cast<const OgreShaderWrapper*>(d_renderMaterial->getShaderWrapper());
        if (!ogreShader->getVertexParameters()->getTransposeMatrices())
            d_matrix = glm::transpose(d_matrix);

        d_matrixValid = true;
    }
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::finaliseVertexAttributes(MANUALOBJECT_TYPE type)
{
    d_expectedData = type;
    
#ifdef CEGUI_USE_OGRE_HLMS
    // basic initialisation of render op
    d_renderOp.vertexData = OGRE_NEW Ogre::v1::VertexData();
    d_renderOp.operationType = Ogre::OT_TRIANGLE_LIST;
    d_renderOp.useIndexes = false;

    // Setup our render operation to match the type
    Ogre::v1::VertexDeclaration* vd = d_renderOp.vertexData->vertexDeclaration;
#else
    // basic initialisation of render op
    d_renderOp.vertexData = OGRE_NEW Ogre::VertexData();
    d_renderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    d_renderOp.useIndexes = false;

    // Setup our render operation to match the type
    Ogre::VertexDeclaration* vd = d_renderOp.vertexData->vertexDeclaration;
#endif //CEGUI_USE_OGRE_HLMS

    switch(d_expectedData)
    {
    case MT_COLOURED: 
    {
        vd->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

        vd->addElement(0, sizeof(float)*3, Ogre::VET_COLOUR, 
            Ogre::VES_DIFFUSE);
        break;
    }
    case MT_TEXTURED: 
    {
        vd->addElement(0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION);

        vd->addElement(0, sizeof(float)*3, Ogre::VET_FLOAT4, 
            Ogre::VES_DIFFUSE);

        vd->addElement(0, sizeof(float)*(3+4), Ogre::VET_FLOAT2, 
            Ogre::VES_TEXTURE_COORDINATES);
        break;
    }
    default:
        throw RendererException(
            "Unknown d_expectedData type.");
    }

}

void OgreGeometryBuffer::setVertexBuffer(size_t count) const
{
    // We first check if some other buffer has already allocated a suited buffer
    // for us
    // We use auto here because the return type depends on Ogre version
    auto already_created = d_owner.getVertexBuffer(count);

    if (!already_created.isNull())
    {

        d_hwBuffer = already_created;

    } else {

        // Create a new vertex buffer
    #ifdef CEGUI_USE_OGRE_HLMS
        d_hwBuffer = Ogre::v1::HardwareBufferManager::getSingleton().
            createVertexBuffer(getVertexAttributeElementCount()*sizeof(float), count,
                Ogre::v1::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                false);
    #else
        d_hwBuffer = Ogre::HardwareBufferManager::getSingleton().
            createVertexBuffer(getVertexAttributeElementCount()*sizeof(float), count,
                Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
                false);
    #endif //CEGUI_USE_OGRE_HLMS
    }

    if (d_hwBuffer.isNull())
    {
        throw RendererException("Failed to create Ogre vertex buffer, "
            "probably because the vertex layout is invalid.");
    }

    // bind the vertex buffer for rendering
    d_renderOp.vertexData->vertexBufferBinding->setBinding(0, d_hwBuffer);
}

void OgreGeometryBuffer::cleanUpVertexAttributes()
{
    OGRE_DELETE d_renderOp.vertexData;
    d_renderOp.vertexData = 0;

    // Store the hardware buffer so that other instances can use it later
    // This check should also help prevent there being nullptrs in the pool
    if (d_hwBuffer.get())
        d_owner.returnVertexBuffer(d_hwBuffer);

    d_hwBuffer.setNull();
}

// ------------------------------------ //
#ifdef CEGUI_USE_OGRE_HLMS
void OgreGeometryBuffer::setScissorRects(Ogre::Viewport* current_viewport) const
{
    int actualWidth = current_viewport->getActualWidth();
    int actualHeight = current_viewport->getActualHeight();
    float scissorsLeft = d_preparedClippingRegion.left() / actualWidth;
    float scissorsTop = d_preparedClippingRegion.top() / actualHeight;
    float scissorsWidth = (d_preparedClippingRegion.right() -
        d_preparedClippingRegion.left()) / actualWidth;
    float scissorsHeight = (d_preparedClippingRegion.bottom() -
        d_preparedClippingRegion.top()) / actualHeight;
    
    current_viewport->setScissors(scissorsLeft, scissorsTop, scissorsWidth,
        scissorsHeight);
}

#else
void OgreGeometryBuffer::setScissorRects() const
{
    d_renderSystem.setScissorTest(true,
        static_cast<size_t>(d_preparedClippingRegion.left()), 
        static_cast<size_t>(d_preparedClippingRegion.top()),
        static_cast<size_t>(d_preparedClippingRegion.right()),
        static_cast<size_t>(d_preparedClippingRegion.bottom()));
}
#endif //CEGUI_USE_OGRE_HLMS

// ------------------------------------ //
void OgreGeometryBuffer::reset()
{
    d_vertexData.clear();
    d_clippingActive = true;
}

// ------------------------------------ //
int OgreGeometryBuffer::getVertexAttributeElementCount() const
{
    switch(d_expectedData)
    {
        case MT_COLOURED:
            return FLOATS_PER_COLOURED;
            break;
        case MT_TEXTURED:
            return FLOATS_PER_TEXTURED;
            break;
        default:
            return 0;
            break;
    }
}

} // End of  CEGUI namespace section
