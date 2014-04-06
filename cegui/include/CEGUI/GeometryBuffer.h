/***********************************************************************
    filename:   CEGUIGeometryBuffer.h
    created:    Thu Jan 8 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIGeometryBuffer_h_
#define _CEGUIGeometryBuffer_h_

#include "CEGUI/Base.h"
#include "CEGUI/Renderer.h"
#include "CEGUI/Rect.h"
#include "CEGUI/RefCounted.h"
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/Quaternion.h"

#include "glm/glm.hpp"

#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

class RenderMaterial;

//----------------------------------------------------------------------------//

/*!
\brief
    Enumerated type that contains the valid options to specify a vertex attribute
    of a vertex used in CEGUI
*/
enum VertexAttributeType
{
    //! Position 0 attribute
    VAT_POSITION0,
    //! Colour 0 attribute
    VAT_COLOUR0,
    //! Texture coordinate 0 attribute
    VAT_TEXCOORD0
};

//----------------------------------------------------------------------------//

/*!
\brief
    Enumerated type that contains the valid options for the fill rule mode. The
    fill rule defines how overlaps of the polygon defined by this GeometryBuffer
    should be rendered. For further information see "fill-rule" in the SVG
    standard: http://www.w3.org/TR/SVGTiny12/painting.html#FillRuleProperty
*/
enum PolygonFillRule
{
    //! Draw the polygon normally - without a fill-rule.
    PFR_NONE,
    //! Uses the nonzero rule to determine how the polygon is to be filled.
    PFR_NON_ZERO,
    //! Uses the  evenodd rule to determine how the polygon is to be filled.
    PFR_EVEN_ODD
};

//----------------------------------------------------------------------------//

/*!
\brief
    Abstract class defining the interface for objects that buffer geometry for
    later rendering.
*/
class CEGUIEXPORT GeometryBuffer :
    public AllocatedObject<GeometryBuffer>
{
public:
    virtual ~GeometryBuffer();

    /*!
    \brief
        Draw the geometry buffered within this GeometryBuffer object.
    */
    virtual void draw() const = 0;

    /*!
    \brief
        Set the translation to be applied to the geometry in the buffer when it
        is subsequently rendered.

    \param translation
        Vector3 describing the three axis translation vector to be used.
    */
    virtual void setTranslation(const Vector3f& translation);

    /*!
    \brief
        Set the rotations to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param rotationQuat
        Quaternion describing the rotation to be used.
    */
    virtual void setRotation(const Quaternion& rotationQuat);

    /*!
    \brief
        Set the scaling to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param scale
        Vector3 describing the scale to be used.
    */
    virtual void setScale(const Vector3f& scale);

    /*!
    \brief
        Set the scaling to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param scale
        Vector2 describing the x and y scale to be used.
    */
    void setScale(const Vector2f& scale);

    /*!
    \brief
        Set the pivot point to be used when applying the rotations.

    \param p
        Vector3 describing the location of the pivot point to be used when
        applying the rotation to the geometry.
    */
    void setPivot(const Vector3f& p);

    /*!
    \brief
        Set a custom transformation matrix that will be applied to the
        geometry in the buffer after all the other transformations.

    \param transformation
        3x3 Matrix that describes the transformation.
    */
    void setCustomTransform(const glm::mat4x4& transformation);

    /*!
    \brief
        Set the clipping region to be used when rendering this buffer.
    */
    virtual void setClippingRegion(const Rectf& region) = 0;

    /*!
    \brief
        Sets the fill rule that should be used when rendering the geometry.

    \param fill_rule
        The fill rule that should be used when rendering the geometry.
    */
    void setStencilRenderingActive(PolygonFillRule fill_rule);

    /*!
    \brief
        Sets the number of vertices that should be rendered after the stencil buffer was filled.

    \param vertex_count
        The number of vertices that should be rendered after the stencil buffer was filled.
    */
    void setStencilPostRenderingVertexCount(unsigned int vertex_count);

    /*!
    \brief
        Append the geometry data to the existing data

    \param vertex_data
        Pointer to an array of floats containing the geometry data that
        should be added to the GeometryBuffer.

    \param array_size
        The number of elements in the array.
    */
    virtual void appendGeometry(const float* const vertex_data, uint array_size);


    /*!
    \brief
        Append the geometry data to the existing data

    \param vertex_data
        Vector of floats containing the geometry data that should be added to the
        GeometryBuffer.
    */
    virtual void appendGeometry(const std::vector<float>& vertex_data) = 0;

    /*!
    \brief
        Append a single vertex to the buffer.

    \param vertex
        Vertex object describing the vertex to be added to the GeometryBuffer.
    */
    virtual void appendVertex(const TexturedColouredVertex& vertex);

    /*!
    \brief
        Append a single vertex to the buffer.

    \param vertex
        Vertex object describing the vertex to be added to the GeometryBuffer.
    */
    virtual void appendVertex(const ColouredVertex& vertex);

    /*!
    \brief
        Appends vertices with colour attributes from an std::vector to the GeometryBuffer.
    \param coloured_vertices
        The vector of ColouredVertices.
    */
    void appendGeometry(const std::vector<ColouredVertex>& coloured_vertices);

    /*!
    \brief
        Append a number of vertices with colour attributes from an array to the GeometryBuffer.

    \param vertex_array
        Pointer to an array of Vertex objects that describe the vertices that
        are to be added to the GeometryBuffer.

    \param vertex_count
        The number of Vertex objects from the array \a vertex_array that are to be
        added to the GeometryBuffer.
    */
    virtual void appendGeometry(const ColouredVertex* vertex_array, uint vertex_count);

    /*!
    \brief
        Appends vertices with texture coordinate and colour attributes from an std::vector to 
        the GeometryBuffer.
    \param textured_vertices
        The vector of TexturedColouredVertices.
    */
    void appendGeometry(const std::vector<TexturedColouredVertex>& textured_vertices);

    /*!
    \brief
        Append a number of vertices, with texture coordinate and colour attributes,
        from an array to the GeometryBuffer.

    \param vertex_array
        Pointer to an array of Vertex objects that describe the vertices that
        are to be added to the GeometryBuffer.

    \param vertex_count
        The number of Vertex objects from the array \a vertex_array that are to be
        added to the GeometryBuffer.
    */
    virtual void appendGeometry(const TexturedColouredVertex* vertex_array, uint vertex_count);

    /*!
    \brief
        A helper function that sets a texture parameter of the RenderMaterial of this
        Geometrybuffer.

    \param parameterName
        Name of the parameter as used inside the shader program. The regular CEGUI
        texture-parameter that is used inside CEGUI's materials is called "texture0".

    \param texture
        Pointer to a Texture object that shall be used for subsequently added
        vertices.  This may be 0, in which case texturing will be disabled for
        subsequently added vertices.
    */
    virtual void setTexture(const std::string& parameterName, const Texture* texture);

    /*!
    \brief
        Clear all buffered data and reset the GeometryBuffer to the default
        state. This excludes resettings the vertex attributes.
    */
    virtual void reset();

    /*!
    \brief
        Returns the vertex count of this GeometryBuffer, which is determined based
        on the the used vertex layout and the size of the vertex data

    \return
        The number of vertices that have been appended to this GeometryBuffer.
    */
    virtual uint getVertexCount() const;

    /*!
    \brief
        Returns the total number of floats used by the attributes of the
        current vertex layout.

    \return
        The total number of floats used by the attributes of the current vertex
        layout.
    */
    int getVertexAttributeElementCount() const;


    /*!
    \brief
        Set the RenderEffect to be used by this GeometryBuffer.

    \param effect
        Pointer to the RenderEffect to be used during renderng of the
        GeometryBuffer.  May be 0 to remove a previously added RenderEffect.

    \note
        When adding a RenderEffect, the GeometryBuffer <em>does not</em> take
        ownership of, nor make a copy of, the passed RenderEffect - this means
        you need to be careful not to delete the RenderEffect if it might still
        be in use!
    */
    virtual void setRenderEffect(RenderEffect* effect);

    /*!
    \brief
        Return the RenderEffect object that is assigned to this GeometryBuffer
        or 0 if none.
    */
    virtual RenderEffect* getRenderEffect();

    /*!
    \brief
        Set the blend mode option to use when rendering this GeometryBuffer.

    \note
        The blend mode setting is not a 'state' setting, but is used for \e all
        geometry added to the buffer regardless of when the blend mode is set.

    \param mode
        One of the BlendMode enumerated values indicating the blending mode to
        be used.
    */
    virtual void setBlendMode(const BlendMode mode);

    /*!
    \brief
        Return the blend mode that is set to be used for this GeometryBuffer.

    \return
        One of the BlendMode enumerated values indicating the blending mode
        that will be used when rendering all geometry added to this
        GeometryBuffer object.
    */
    virtual BlendMode getBlendMode() const;

    /*!
    \brief
        Set whether clipping will be active for subsequently added vertices.

    \param active
        - true if vertices added after this call should be clipped to the
          clipping region defined for this GeometryBuffer.
        - false if vertices added after this call should not be clipped
          (other than to the edges of rendering target.
    */
    virtual void setClippingActive(const bool active);

    /*
    \brief
        Return whether clipping will be used for the current batch
        of vertices being defined.

    \return
        - true if vertices subsequently added to the GeometryBuffer will
          be clipped to the clipping region defined for this GeometryBuffer.
        - false if vertices subsequently added will not be clipped (other than
          to the edges of the rendering target).
    */
    virtual bool isClippingActive() const;

    /*
    \brief
        Resets the vertex attributes that were set for the vertices of this
        GeometryBuffer.
    */
    void resetVertexAttributes();

    /*
    \brief
        Adds a vertex attributes to the list of vertex attributes. The vertex
        attributes are used to describe the layout of the verrex data. The
        order in which the attributes are added is the same order in which the
        data has to be aligned for the vertex. This has be done before adding
        actual vertex data to the GeometryBuffer.

    \param attribute
        The attribute that should be added to the list of vertex attributes
        describing the vertices of this GeometryBuffer.
    */
    void addVertexAttribute(VertexAttributeType attribute);

    /*
    \brief
        Returns the RenderMaterial that is currently used by this GeometryBuffer.

    \return
        A reference to the RenderMaterial that is used by this GeometryBuffer.
    */
    RefCounted<RenderMaterial> getRenderMaterial() const;

    /*
    \brief
        Set a new RenderMaterial to be used by this GeometryBuffer.

    \param render_material
        A reference to the RenderMaterial that will be set to be used by this
        GeometryBuffer.
    */
    void setRenderMaterial(RefCounted<RenderMaterial> render_material);

protected:
    GeometryBuffer(RefCounted<RenderMaterial> renderMaterial);

    //! Reference to the RenderMaterial used for this GeometryBuffer
    RefCounted<RenderMaterial>  d_renderMaterial;

    //! type of container used to store the geometry's vertex data
    typedef std::vector<float>  VertexData;
    //! The container in which the vertex data is stored.
    VertexData                  d_vertexData;
    //! The vertex count which is determined based on the used vertex layout
    unsigned int                d_vertexCount;
    /*
    \brief
        A vector of the attributes of the vertices of this GeometryBuffer. The order
        in which they were added to the vector is used to define the alignment of the
        vertex data.
    */
    std::vector<VertexAttributeType> d_vertexAttributes;


    //! translation vector
    Vector3f        d_translation;
    //! rotation quaternion
    Quaternion      d_rotation;
    //! scaling vector
    Vector3f        d_scale;
    //! pivot point for rotation
    Vector3f        d_pivot;
    //! custom transformation matrix
    glm::mat4x4     d_customTransform;
    //! true when there have been no changes to the GeometryBuffer's transformation since it has last been updated.
    mutable bool    d_matrixValid;

    //! The BlendMode to use when rendering this GeometryBuffer.
    BlendMode       d_blendMode;
    //! The fill rule that should be used when rendering the geometry.
    PolygonFillRule d_polygonFillRule;
    //! The amount of vertices that need to be rendered after rendering to the stencil buffer.
    unsigned int    d_postStencilVertexCount;
    //! RenderEffect that will be used by the GeometryBuffer
    RenderEffect*   d_effect;
    //! True if clipping will be active for the current batch
    bool            d_clippingActive;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
