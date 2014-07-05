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

#include "OgreManualObject.h"
#include "OgreRenderOperation.h"
#include "OgreSceneManager.h"


#define INITIAL_BUFFER_SIZE     64

#define FLOATS_PER_TEXTURED     9
#define FLOATS_PER_COLOURED     7

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
    d_vertexHolder(0),
    d_expectedData(MANUALOBJECT_TYPE_INVALID),
    d_renderOp(0),
    d_dataAppended(false)
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
        syncManualObject();

    if (!d_renderOp)
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

    bool found_texture = false;

    // Not sure if this is needed here
    const ShaderParameterBindings::ShaderParameterBindingsMap& 
        shader_parameter_bindings = shaderParameterBindings->
        getShaderParameterBindings();

    auto end = shader_parameter_bindings.end();

    for (auto iter = shader_parameter_bindings.begin(); iter != end; ++iter)
    {
        const CEGUI::ShaderParameter* parameter = iter->second;
        const ShaderParamType parameterType = parameter->getType();


        if (iter->first == "texture0")
        {

            const CEGUI::ShaderParameterTexture* parameterTexture = 
                static_cast<const CEGUI::ShaderParameterTexture*>(parameter);

            const CEGUI::OgreTexture* texture = static_cast<const 
                CEGUI::OgreTexture*>(parameterTexture->d_parameterValue);

            d_vertexHolder->setMaterialName(0, 
                texture->getOgreTexture()->getName());

            found_texture = true;
            break;
        }
    }

    if (!found_texture)
    {
        // Reset the material
        d_vertexHolder->setMaterialName(0, "BaseWhiteNoLighting");
    }

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
        d_renderSystem._render(*d_renderOp);
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

    d_dataAppended = true;

    size_t float_per_element;

    switch(d_expectedData){
    case MANUALOBJECT_TYPE_COLOURED: float_per_element = FLOATS_PER_COLOURED; break;
    case MANUALOBJECT_TYPE_TEXTURED: float_per_element = FLOATS_PER_TEXTURED; break;
    }

    d_vertexCount = d_vertexData.size()/float_per_element;
}

void OgreGeometryBuffer::syncManualObject() const
{
    if (!d_dataAppended)
        return;

    // All the data is in the vector so it's safe to clear
    d_vertexHolder->clear();

    // Blank material initially
    d_vertexHolder->begin("BaseWhiteNoLighting", 
        Ogre::RenderOperation::OT_TRIANGLE_LIST);

    // Append the data to the manual object
    if (d_expectedData == MANUALOBJECT_TYPE_TEXTURED)
    {
        for (size_t i = 0; i < d_vertexData.size(); i += FLOATS_PER_TEXTURED)
        {
            assert(i+FLOATS_PER_TEXTURED <= d_vertexData.size() && 
                "invalid vertex data passed to OgreGeometryBuffer");

            // First the position
            d_vertexHolder->position(d_vertexData[i], d_vertexData[i+1], 
                d_vertexData[i+2]);

            // And then the colour
            d_vertexHolder->colour(d_vertexData[i+3], d_vertexData[i+4], 
                d_vertexData[i+5], d_vertexData[i+6]);


            // And finally texture coordinate
            d_vertexHolder->textureCoord(d_vertexData[i+7], d_vertexData[i+8]);
        }
    }
    else if (d_expectedData == MANUALOBJECT_TYPE_COLOURED)
    {

        for (size_t i = 0; i < d_vertexData.size(); i += FLOATS_PER_COLOURED)
        {
            assert(i+FLOATS_PER_COLOURED <= d_vertexData.size() && 
                "invalid vertex data passed to OgreGeometryBuffer");

            // First the position
            d_vertexHolder->position(d_vertexData[i], d_vertexData[i+1], 
                d_vertexData[i+2]);

            // And then the colour
            d_vertexHolder->colour(d_vertexData[i+3], d_vertexData[i+4], 
                d_vertexData[i+5], d_vertexData[i+6]);
        }
    }


    auto section = d_vertexHolder->end();

    d_renderOp = section->getRenderOperation();

    d_dataAppended = false;
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
void OgreGeometryBuffer::finaliseVertexAttributes(MANUALOBJECT_TYPE type)
{
    d_expectedData = type;

    if (d_expectedData >= MANUALOBJECT_TYPE_INVALID)
    {
        CEGUI_THROW(RendererException(
            "Unknown d_expectedData type."));
    }

    setVertexBuffer(INITIAL_BUFFER_SIZE);
}

void OgreGeometryBuffer::setVertexBuffer(size_t size)
{
    // create the vertex container
    d_vertexHolder = d_owner.getDummyScene().createManualObject(
        Ogre::SCENE_STATIC);

    if (!d_vertexHolder)
    {
        CEGUI_THROW(RendererException("Failed to create Ogre vertex buffer, "
            "probably because the vertex layout is invalid."));
    }


    d_vertexHolder->setDynamic(false);
    d_vertexHolder->estimateVertexCount(size);
}

void OgreGeometryBuffer::cleanUpVertexAttributes()
{
    d_renderOp = 0;

    d_owner.getDummyScene().destroyManualObject(d_vertexHolder);
    d_vertexHolder = 0;
}

void OgreGeometryBuffer::setScissorRects() const
{
    d_renderSystem.setScissorTest(true, d_clipRect.left(), 
        d_clipRect.top(), d_clipRect.right(), d_clipRect.bottom());
}




} // End of  CEGUI namespace section
