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
    d_clipRect(0, 0, 0, 0),
    d_expectedData(MT_INVALID),
    d_dataAppended(false),
    d_matrix(1.0),
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

    // setup clip region
    if (d_clippingActive)
    {
        setScissorRects();
    }

    // Update the matrix
    updateMatrix();

    CEGUI::ShaderParameterBindings* shaderParameterBindings = 
        (*d_renderMaterial).getShaderParamBindings();

    // Set the ModelViewProjection matrix in the bindings
    shaderParameterBindings->setParameter("modelViewPerspMatrix", d_matrix);


    if (d_alpha != d_previousAlphaValue)
    {
        d_previousAlphaValue = d_alpha;

        shaderParameterBindings->setParameter("alphaPercentage", 
            d_previousAlphaValue);
    }

    // activate the desired blending mode
    d_owner.bindBlendMode(d_blendMode);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        setTextureStates();

        //Prepare for the rendering process according to the used render material
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
        d_renderSystem.setScissorTest(false);
    }
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::setClippingRegion(const Rectf& region)
{
    d_clipRect.top(ceguimax(0.0f, region.top()));
    d_clipRect.bottom(ceguimax(0.0f, region.bottom()));
    d_clipRect.left(ceguimax(0.0f, region.left()));
    d_clipRect.right(ceguimax(0.0f, region.right()));
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

        void* copy_target = d_hwBuffer->lock(
            Ogre::HardwareVertexBuffer::HBL_DISCARD);

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
    // translation to position geometry and offset to pivot point
    Ogre::Matrix4 trans;

    trans.makeTrans(d_translation.x + d_pivot.x,
                    d_translation.d_y + d_pivot.d_y,
                    d_translation.d_z + d_pivot.d_z);

    // rotation
    Ogre::Matrix4 rot(Ogre::Quaternion(d_rotation.d_w, d_rotation.d_x, d_rotation.d_y, d_rotation.d_z));

    // translation to remove rotation pivot offset
    Ogre::Matrix4 inv_pivot_trans;
    inv_pivot_trans.makeTrans(-d_pivot.x, -d_pivot.y, -d_pivot.z);

    // calculate final matrix
    Ogre::Matrix4 finalMatrix = trans * rot * inv_pivot_trans;

    // Apply WorldViewProjection Matrix
    finalMatrix = d_owner.getWorldViewProjMatrix() * finalMatrix;

    //Reinterpret as glm matrix
    d_matrix = glm::make_mat4(&finalMatrix[0][0]);

    //If necessary: transpose
    const OgreShaderWrapper* ogreShader = static_cast<const OgreShaderWrapper*>(d_renderMaterial->getShaderWrapper());
    if (ogreShader->getVertexParameters()->getTransposeMatrices())
        d_matrix = glm::transpose(d_matrix); 

    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4 OgreGeometryBuffer::getMatrix() const
{
    updateMatrix();

    return Ogre::Matrix4(&d_matrix[0][0]);
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::finaliseVertexAttributes(MANUALOBJECT_TYPE type)
{
    d_expectedData = type;

    // basic initialisation of render op
    d_renderOp.vertexData = OGRE_NEW Ogre::VertexData();
    d_renderOp.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
    d_renderOp.useIndexes = false;

    // Setup our render operation to match the type
    Ogre::VertexDeclaration* vd = d_renderOp.vertexData->vertexDeclaration;

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
        CEGUI_THROW(RendererException(
            "Unknown d_expectedData type."));
    }

}

void OgreGeometryBuffer::setVertexBuffer(size_t count) const
{
    // We first check if some other buffer has already allocated a suited buffer
    // for us
    Ogre::HardwareVertexBufferSharedPtr already_created = 
        d_owner.getVertexBuffer(count);

    if (!already_created.isNull())
    {

        d_hwBuffer = already_created;

    } else {

        // Create the a new vertex buffer
        d_hwBuffer = Ogre::HardwareBufferManager::getSingleton().
            createVertexBuffer(getVertexAttributeElementCount()*sizeof(float), count,
            Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
            false);
    }

    if (d_hwBuffer.isNull())
    {
        CEGUI_THROW(RendererException("Failed to create Ogre vertex buffer, "
            "probably because the vertex layout is invalid."));
    }

    // bind the vertex buffer for rendering
    d_renderOp.vertexData->vertexBufferBinding->setBinding(0, d_hwBuffer);
}

void OgreGeometryBuffer::cleanUpVertexAttributes()
{
    OGRE_DELETE d_renderOp.vertexData;
    d_renderOp.vertexData = 0;

    // Store the hardware buffer so that other instances can use it later
    d_owner.returnVertexBuffer(d_hwBuffer);

    d_hwBuffer.setNull();
}

// ------------------------------------ //
void OgreGeometryBuffer::setScissorRects() const
{
    d_renderSystem.setScissorTest(true, static_cast<size_t>(d_clipRect.left()), 
        static_cast<size_t>(d_clipRect.top()), static_cast<size_t>(d_clipRect.right()), static_cast<size_t>(d_clipRect.bottom()));
}

// ------------------------------------ //
void OgreGeometryBuffer::reset()
{
    d_vertexData.clear();
    d_clippingActive = true;
}

// ------------------------------------ //
void OgreGeometryBuffer::setTextureStates() const
{
    using namespace Ogre;
    // Set texture states
    d_renderSystem._setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_NONE);
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
