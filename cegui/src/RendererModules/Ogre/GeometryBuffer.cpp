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
#include "CEGUI/Vertex.h"
#include "CEGUI/RenderEffect.h"

#include <OgreRenderSystem.h>
#include <OgreQuaternion.h>
#include <OgreHardwareBufferManager.h>
#include "CEGUI/Exceptions.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"


#define INITIAL_BUFFER_SIZE     64

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
static const Ogre::LayerBlendModeEx S_colourBlendMode =
{
    Ogre::LBT_COLOUR,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE,
    Ogre::ColourValue(0, 0, 0, 0),
    Ogre::ColourValue(0, 0, 0, 0),
    0, 0, 0
};

//----------------------------------------------------------------------------//
static const Ogre::LayerBlendModeEx S_alphaBlendMode =
{
    Ogre::LBT_ALPHA,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE,
    Ogre::ColourValue(0, 0, 0, 0),
    Ogre::ColourValue(0, 0, 0, 0),
    0, 0, 0
};

//----------------------------------------------------------------------------//
static const Ogre::TextureUnitState::UVWAddressingMode S_textureAddressMode =
{
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP
};

//----------------------------------------------------------------------------//
OgreGeometryBuffer::OgreGeometryBuffer(OgreRenderer& owner, 
    Ogre::RenderSystem& rs, CEGUI::RefCounted<RenderMaterial> renderMaterial) :
    GeometryBuffer(renderMaterial),
    d_owner(owner),
    d_renderSystem(rs),
    d_clipRect(0, 0, 0, 0),
    d_matrixValid(false),
    d_sync(false),
    d_vertexDefBytes(0)
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
    if (!d_sync)
        syncHardwareBuffer();

    if (d_vertexData.empty())
        return;

    // setup clip region
    if (d_clippingActive)
        setScissorRects();

    // Update the model matrix if necessary
    if (!d_matrixValid)
        updateMatrix();

    CEGUI::ShaderParameterBindings* shaderParameterBindings = 
        (*d_renderMaterial).getShaderParamBindings();


    // Set the ModelViewProjection matrix in the bindings
    Ogre::Matrix4 omat = d_owner.getWorldViewProjMatrix()*d_matrix;

    glm::mat4 glmmat;

    OgreRenderer::convertOgreMatrixToGLMMatrix(omat, glmmat);

    shaderParameterBindings->setParameter("modelViewPerspMatrix", glmmat);


    d_owner.bindBlendMode(d_blendMode);

    const int pass_count = d_effect ? d_effect->getPassCount() : 1;
    for (int pass = 0; pass < pass_count; ++pass)
    {
        // set up RenderEffect
        if (d_effect)
            d_effect->performPreRenderFunctions(pass);

        initialiseTextureStates();

        //Prepare for the rendering process according to the used render material
        d_renderMaterial->prepareForRendering();


        // draw the geometry
        d_renderOp.vertexData->vertexStart = 0;
        d_renderOp.vertexData->vertexCount = d_vertexCount;
        
        d_renderSystem._render(d_renderOp);
    }

    // clean up RenderEffect
    if (d_effect)
        d_effect->performPostRenderFunctions();
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
void OgreGeometryBuffer::appendGeometry(const std::vector<float>& vertex_data)
{

    d_vertexData.insert(d_vertexData.end(), vertex_data.begin(), 
        vertex_data.end());

    // Calculate the new vertex count
    d_vertexCount = (d_vertexData.size()*sizeof(float))/d_vertexDefBytes;


    d_sync = false;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::updateMatrix() const
{
    // translation to position geometry and offset to pivot point
    Ogre::Matrix4 trans;

    trans.makeTrans(d_translation.d_x + d_pivot.d_x,
        d_translation.d_y + d_pivot.d_y,
        d_translation.d_z + d_pivot.d_z);


    // rotation
    Ogre::Matrix4 rot(Ogre::Quaternion(
        d_rotation.d_w, d_rotation.d_x, d_rotation.d_y, d_rotation.d_z));


    // translation to remove rotation pivot offset
    Ogre::Matrix4 inv_pivot_trans;
    inv_pivot_trans.makeTrans(-d_pivot.d_x, -d_pivot.d_y, -d_pivot.d_z);


    // calculate final matrix
    d_matrix = trans * rot * inv_pivot_trans;


    d_matrixValid = true;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::syncHardwareBuffer() const
{
    // Reallocate h/w buffer as required
    size_t size = d_hwBuffer->getNumVertices();
    const size_t required_size = d_vertexCount;
    if(size < required_size)
    {
        // calculate new size to use
        while(size < required_size)
            size *= 2;

        // Reallocate the buffer
        d_hwBuffer.setNull();
        setVertexBuffer(size);
    }

    // copy vertex data into hw buffer
    if (required_size > 0)
    {
        std::memcpy(d_hwBuffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD),
                    &d_vertexData[0], sizeof(float) * d_vertexData.size());

        d_hwBuffer->unlock();
    }

    d_sync = true;
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreGeometryBuffer::getMatrix() const{
    if (!d_matrixValid)
        updateMatrix();

    return d_matrix;
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::initialiseTextureStates() const
{
    using namespace Ogre;
    d_renderSystem._setTextureCoordCalculation(0, TEXCALC_NONE);
    d_renderSystem._setTextureCoordSet(0, 0);
    d_renderSystem._setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_POINT);
    d_renderSystem._setTextureAddressingMode(0, S_textureAddressMode);
    d_renderSystem._setTextureMatrix(0, Matrix4::IDENTITY);
    d_renderSystem._setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0, false);
    d_renderSystem._setTextureBlendMode(0, S_colourBlendMode);
    d_renderSystem._setTextureBlendMode(0, S_alphaBlendMode);
    d_renderSystem._disableTextureUnitsFrom(1);
}

//----------------------------------------------------------------------------//
void OgreGeometryBuffer::finaliseVertexAttributes()
{
    using namespace Ogre;

    // basic initialization of render op
    d_renderOp.vertexData = OGRE_NEW Ogre::VertexData();
    d_renderOp.operationType = RenderOperation::OT_TRIANGLE_LIST;
    d_renderOp.useIndexes = false;

    // setup vertex declaration for format we will use
    VertexDeclaration* vd = d_renderOp.vertexData->vertexDeclaration;
    size_t vd_offset = 0;


    // Update the format according to our vertex attributes
    const size_t attribute_count = d_vertexAttributes.size();
    for (size_t i = 0; i < attribute_count; ++i)
    {
        switch(d_vertexAttributes.at(i))
        {
        case VAT_POSITION0:
            {
                vd->addElement(0, vd_offset, VET_FLOAT3, VES_POSITION);
                vd_offset += VertexElement::getTypeSize(VET_FLOAT3);
            }
            break;
        case VAT_COLOUR0:
            {
                vd->addElement(0, vd_offset, VET_COLOUR, VES_DIFFUSE);
                vd_offset += VertexElement::getTypeSize(VET_COLOUR);
            }
            break;
        case VAT_TEXCOORD0:
            {
                vd->addElement(0, vd_offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);
                vd_offset += VertexElement::getTypeSize(VET_FLOAT2);
            }
            break;
        default:
            CEGUI_THROW(RendererException(
                "Unknown vertex atribute."));
            break;
        }
    }

    if(vd->getElementCount() == 0)
        CEGUI_THROW(RendererException(
        "The empty vertex layout is invalid because it is empty."));


    d_vertexDefBytes = vd_offset;


    setVertexBuffer(INITIAL_BUFFER_SIZE);
}

void OgreGeometryBuffer::setVertexBuffer(size_t size) const
{
    using namespace Ogre;

    // create hardware vertex buffer
    d_hwBuffer = HardwareBufferManager::getSingleton().createVertexBuffer(
        d_renderOp.vertexData->vertexDeclaration->getVertexSize(0), 
        size,
        HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
        false);

    if (!d_hwBuffer.get())
    {
        CEGUI_THROW(RendererException("Failed to create Ogre vertex buffer, "
            "probably because the vertex layout is invalid."));
    }

    // bind vertex buffer
    d_renderOp.vertexData->vertexBufferBinding->setBinding(0, d_hwBuffer);
}

void OgreGeometryBuffer::cleanUpVertexAttributes()
{
    OGRE_DELETE d_renderOp.vertexData;
    d_renderOp.vertexData = 0;
    d_hwBuffer.setNull();
}

void OgreGeometryBuffer::setScissorRects() const
{
    d_renderSystem.setScissorTest(true, d_clipRect.left(), 
        d_clipRect.top(), d_clipRect.right(), d_clipRect.bottom());
}






} // End of  CEGUI namespace section
