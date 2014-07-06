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


#define INITIAL_BUFFER_SIZE     24

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
    d_vertexHolder(0),
    d_expectedData(MT_INVALID),
    d_renderOp(0),
    d_dataAppended(false),
    d_firstMOUpdate(true)
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
    {
        setScissorRects();
    }

    // Update the model matrix if necessary
    if (!d_matrixValid)
        updateMatrix();

    CEGUI::ShaderParameterBindings* shaderParameterBindings = 
        (*d_renderMaterial).getShaderParamBindings();


    // Set the ModelViewProjection matrix in the bindings
    Ogre::Matrix4 omat = d_owner.getWorldViewProjMatrix()*d_matrix;

    shaderParameterBindings->setParameter("modelViewPerspMatrix", &omat);


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
        d_renderSystem._render(*d_renderOp);
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
void OgreGeometryBuffer::appendGeometry(const std::vector<float>& vertex_data)
{
    d_vertexData.insert(d_vertexData.end(), vertex_data.begin(), 
        vertex_data.end());

    d_dataAppended = true;

    size_t float_per_element;

    switch(d_expectedData){
    case MT_COLOURED: float_per_element = FLOATS_PER_COLOURED; break;
    case MT_TEXTURED: float_per_element = FLOATS_PER_TEXTURED; break;
    }

    d_vertexCount = d_vertexData.size()/float_per_element;
}

void OgreGeometryBuffer::syncManualObject() const
{
    if (!d_dataAppended)
        return;

    // First update needs to create the section that subsequent calls can update
    if (d_firstMOUpdate)
    {

        // Blank material initially
        d_vertexHolder->begin("BaseWhiteNoLighting", 
            Ogre::RenderOperation::OT_TRIANGLE_LIST);

        d_vertexHolder->estimateVertexCount(d_vertexCount > INITIAL_BUFFER_SIZE 
            ? d_vertexCount: INITIAL_BUFFER_SIZE);

        d_firstMOUpdate = false;
    }
    else
    {
        // Only the first section is ever used
        d_vertexHolder->beginUpdate(0);

        d_vertexHolder->estimateVertexCount(d_vertexCount);

    }

    // Append the data to the manual object
    if (d_expectedData == MT_TEXTURED)
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
    else if (d_expectedData == MT_COLOURED)
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


    Ogre::ManualObject::ManualObjectSection* section = d_vertexHolder->end();

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
void OgreGeometryBuffer::finaliseVertexAttributes(MANUALOBJECT_TYPE type)
{
    d_expectedData = type;

    if (d_expectedData >= MT_INVALID)
    {
        CEGUI_THROW(RendererException(
            "Unknown d_expectedData type."));
    }

    setVertexBuffer();
}

void OgreGeometryBuffer::setVertexBuffer(){
    // create the vertex container
    d_vertexHolder = d_owner.getDummyScene().createManualObject(
        Ogre::SCENE_STATIC);

    if (!d_vertexHolder)
    {
        CEGUI_THROW(RendererException("Failed to create Ogre vertex buffer, "
            "probably because the vertex layout is invalid."));
    }


    d_vertexHolder->setDynamic(true);
}

void OgreGeometryBuffer::cleanUpVertexAttributes()
{
    d_renderOp = 0;

    d_owner.getDummyScene().destroyManualObject(d_vertexHolder);
    d_vertexHolder = 0;
}

// ------------------------------------ //
void OgreGeometryBuffer::setScissorRects() const
{
    d_renderSystem.setScissorTest(true, d_clipRect.left(), 
        d_clipRect.top(), d_clipRect.right(), d_clipRect.bottom());
}

// ------------------------------------ //
void OgreGeometryBuffer::reset()
{
    d_vertexData.clear();
    d_clippingActive = true;
    d_vertexHolder->clear();
    d_firstMOUpdate = true;
    d_renderOp = 0;
}

// ------------------------------------ //
void OgreGeometryBuffer::setTextureStates() const{
    using namespace Ogre;
    // Set texture states
    d_renderSystem._setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_NONE);
}


} // End of  CEGUI namespace section
