/***********************************************************************
    created:    Thu Jan 8 2009
    author:     Paul D Turner
                edited by Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/Rectf.h"
#include "CEGUI/RefCounted.h"
#include "CEGUI/Vertex.h"
#include <glm/gtc/quaternion.hpp>
#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class RenderMaterial;
class ShaderParameterTexture;
enum class BlendMode : int;

//----------------------------------------------------------------------------//

/*!
\brief
    Enumerated type that contains the valid options to specify a vertex attribute
    of a vertex used in CEGUI
*/
enum class VertexAttributeType : int
{
    //! Position 0 attribute
    Position0,
    //! Colour 0 
    Colour0,
    //! Texture coordinate 0 attribute
    TexCoord0
};

//----------------------------------------------------------------------------//

/*!
\brief
    Enumerated type that contains the valid options for the fill rule mode. The
    fill rule defines how overlaps of the polygon defined by this GeometryBuffer
    should be rendered. For further information see "fill-rule" in the SVG
    standard: http://www.w3.org/TR/SVGTiny12/painting.html#FillRuleProperty
*/
enum class PolygonFillRule : uint8_t
{
    //! Draw the polygon normally - without a fill-rule.
    NoFilling,
    //! Uses the nonzero rule to determine how the polygon is to be filled.
    NonZero,
    //! Uses the  evenodd rule to determine how the polygon is to be filled.
    EvenOdd
};

//----------------------------------------------------------------------------//

/*!
\brief
    Abstract class defining the interface for objects that buffer geometry for
    later rendering.
*/
class CEGUIEXPORT GeometryBuffer
{
public:

    static constexpr size_t COLORED_VERTEX_FLOAT_COUNT = sizeof(ColouredVertex) / sizeof(float);
    static constexpr size_t TEXTURED_VERTEX_FLOAT_COUNT = sizeof(TexturedColouredVertex) / sizeof(float);

    virtual ~GeometryBuffer();

    void clear();

    /*!
    \brief
        Draw the geometry buffered within this GeometryBuffer object.
    */
    virtual void draw(uint32_t drawModeMask = DrawModeMaskAll) const = 0;

    /*!
    \brief
        Set the translation to be applied to the geometry in the buffer when it
        is subsequently rendered.

    \param translation
        Vector3 describing the three axis translation vector to be used.
    */
    virtual void setTranslation(const glm::vec3& translation);

    //! \brief Returns a translation set for this buffer with a call to setTranslation.
    const glm::vec3& getTranslation() const { return d_translation; }

    /*!
    \brief
        Set the rotations to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param rotationQuat
        Quaternion describing the rotation to be used.
    */
    virtual void setRotation(const glm::quat& rotationQuat);

    /*!
    \brief
        Set the scaling to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param scale
        Vector3 describing the scale to be used.
    */
    virtual void setScale(const glm::vec3& scale);

    /*!
    \brief
        Set the scaling to be applied to the geometry in the buffer when it is
        subsequently rendered.

    \param scale
        Vector2 describing the x and y scale to be used.
    */
    void setScale(const glm::vec2& scale);

    /*!
    \brief
        Set the pivot point to be used when applying the rotations.

    \param p
        Vector3 describing the location of the pivot point to be used when
        applying the rotation to the geometry.
    */
    void setPivot(const glm::vec3& p);

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
        Set the clipping region to be used when rendering this buffer. The
        clipping region for actual rendering will be stored extra after 
        clamping the input region.
    */
    virtual void setClippingRegion(const Rectf& region);

    //! \brief Gets the set clipping region for this buffer.
    const Rectf& getClippingRegion() const { return d_clippingRegion; }

    //! \brief Gets the prepared clipping region to be used when rendering this buffer.
    const Rectf& getPreparedClippingRegion() const { return d_preparedClippingRegion; }

    /*!
    \brief
        Sets the fill rule that should be used when rendering the geometry.

    \param fill_rule
        The fill rule that should be used when rendering the geometry.
    */
    void setStencilRenderingActive(PolygonFillRule fill_rule) { d_polygonFillRule = fill_rule; }

    /*!
    \brief
        Sets the number of vertices that should be rendered after the stencil buffer was filled.

    \param vertex_count
        The number of vertices that should be rendered after the stencil buffer was filled.
    */
    void setStencilPostRenderingVertexCount(unsigned int vertex_count) { d_postStencilVertexCount = vertex_count; }

    /*!
    \brief
        Append the geometry data to the existing data

    \param vertex_data
        Pointer to an array of floats containing the geometry data that
        should be added to the GeometryBuffer.

    \param array_size
        The number of elements in the passed array.
    */
    void appendGeometry(const float* vertex_data, size_t array_size);

    /*!
    \brief
        Append the geometry for the solid colored rectangle to the existing data

    \param rect
        A rectangle to generate vertices for.

    \param colours
        A colour rect applied to the rectangle vertices.
    */
    void appendSolidRect(const Rectf& rect, const ColourRect& colours);

    /*!
    \brief
        Append a single vertex to the buffer.

    \param vertex
        Vertex object describing the vertex to be added to the GeometryBuffer.
    */
    void appendVertex(const TexturedColouredVertex& vertex) { appendGeometry(&vertex, 1); }

    /*!
    \brief
        Append a single vertex to the buffer.

    \param vertex
        Vertex object describing the vertex to be added to the GeometryBuffer.
    */
    void appendVertex(const ColouredVertex& vertex) { appendGeometry(&vertex, 1); }

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
    void appendGeometry(const ColouredVertex* vertexArray, size_t vertexCount)
    {
        const size_t addedFloatCount = COLORED_VERTEX_FLOAT_COUNT * vertexCount;
        appendGeometry(reinterpret_cast<const float*>(vertexArray), addedFloatCount);
    }

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
    void appendGeometry(const TexturedColouredVertex* vertexArray, size_t vertexCount)
    {
        const size_t addedFloatCount = TEXTURED_VERTEX_FLOAT_COUNT * vertexCount;
        appendGeometry(reinterpret_cast<const float*>(vertexArray), addedFloatCount);
    }

    /*!
    \brief
        A helper function that sets a texture parameter of the RenderMaterial of this
        GeometryBuffer.

    \param parameterName
        Name of the parameter as used inside the shader program. The regular CEGUI
        texture-parameter that is used inside CEGUI's materials is called "texture0".

    \param texture
        Pointer to the Texture object that shall be set for this parameter.
    */
    void setTexture(const std::string& parameterName, const Texture* texture);

    /*!
    \brief
        A helper function that gets a texture parameter of the RenderMaterial of this
        GeometryBuffer.

    \param parameterName
        Name of the parameter as used inside the shader program. The regular CEGUI
        texture-parameter that is used inside CEGUI's materials is called "texture0".

    \return
        Pointer to the Texture object if it is found in the RenderMaterial, otherwise
        returns a nullptr.
    */
    const Texture* getTexture(const std::string& parameterName) const;

    //! \brief An optimized way to set the main CEGUI texture ("texture0")
    void setMainTexture(const Texture* texture);

    //! \brief An optimized way to get the main CEGUI texture ("texture0")
    const Texture* getMainTexture() const;

    /*!
    \brief
        Scales the texture coordinates of this geometry buffer by the supplied factor, if the
        texture is matching the texture (if one exists) of this geometry buffer.
    */
    void updateTextureCoordinates(const Texture* texture, float scaleFactor);

    /*!
    \brief
        Clear all buffered data and reset the GeometryBuffer to the default
        state. This excludes resettings the vertex attributes.
    */
    void reset();

    /*!
    \brief
        Returns the vertex count of this GeometryBuffer, which is determined based
        on the the used vertex layout and the size of the vertex data

    \return
        The number of vertices that have been appended to this GeometryBuffer.
    */
    size_t getVertexCount() const { return d_vertexCount; }

    /*!
    \brief
        Returns the total number of floats used by the attributes of the
        current vertex layout.

    \return
        The total number of floats used by the attributes of the current vertex
        layout.
    */
    virtual int getVertexAttributeElementCount() const;

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
    virtual void setRenderEffect(RenderEffect* effect) { d_effect = effect; }

    /*!
    \brief
        Return the RenderEffect object that is assigned to this GeometryBuffer
        or 0 if none.
    */
    virtual RenderEffect* getRenderEffect() { return d_effect; }

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
    virtual void setBlendMode(const BlendMode mode) { d_blendMode = mode; }

    /*!
    \brief
        Return the blend mode that is set to be used for this GeometryBuffer.

    \return
        One of the BlendMode enumerated values indicating the blending mode
        that will be used when rendering all geometry added to this
        GeometryBuffer object.
    */
    virtual BlendMode getBlendMode() const { return d_blendMode; }

    /*!
    \brief
        Set whether clipping will be active for subsequently added vertices.

    \param active
        - true if vertices added after this call should be clipped to the
          clipping region defined for this GeometryBuffer.
        - false if vertices added after this call should not be clipped
          (other than to the edges of rendering target.
    */
    virtual void setClippingActive(bool active) { d_clippingActive = active; }

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
    virtual bool isClippingActive() const { return d_clippingActive; }

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
    RenderMaterial* getRenderMaterial() const;

    /*
    \brief
        Set a new RenderMaterial to be used by this GeometryBuffer.

    \param render_material
        A reference to the RenderMaterial that will be set to be used by this
        GeometryBuffer.
    */
    void setRenderMaterial(RefCounted<RenderMaterial> render_material);

    /*!
    \brief
        Sets the alpha for this window
    \param alpha The new alpha value in the range 0.f-1.f
    */
    void setAlpha(float alpha) { d_alpha = alpha; }

    //! \brief Gets the current alpha value
    float getAlpha() const { return d_alpha; }

    /*!
    \brief 
        Invalidates the local matrix. This should be called whenever anything extraordinary
        that requires to recalculate the matrix has occured
    */
    void invalidateMatrix() { d_matrixValid = false; }

    //TODO DOCU
    const RenderTarget* getLastRenderTarget() const { return d_lastRenderTarget; }

    /*!
    \brief 
        Returns if the data (matrix etc) from the RenderTarget that was last used is still valid or not.

    \return
        True if still valid. False if invalid.
    */
    bool isRenderTargetDataValid(const RenderTarget* activeRenderTarget) const;

    //TODO DOCU
    void updateRenderTargetData(const RenderTarget* activeRenderTarget) const;

    /*
    \brief
        Calculates and returns the model matrix for this GeometryBuffer.

    \return
        The model matrix for this GeometryBuffer.
    */
    glm::mat4 getModelMatrix() const;

    const std::vector<float>& getVertexData() const { return d_vertexData; }

protected:

    GeometryBuffer(RefCounted<RenderMaterial> renderMaterial);

    virtual void onGeometryChanged() {}

    //! Reference to the RenderMaterial used for this GeometryBuffer
    RefCounted<RenderMaterial>  d_renderMaterial;
    //! RenderEffect that will be used by the GeometryBuffer
    RenderEffect* d_effect = nullptr;
    //! The RenderTarget that this GeometryBuffer's matrix was last updated for
    mutable const RenderTarget* d_lastRenderTarget = nullptr;

    //! The container in which the vertex data is stored.
    std::vector<float>          d_vertexData;
    /*
    \brief
        A vector of the attributes of the vertices of this GeometryBuffer. The order
        in which they were added to the vector is used to define the alignment of the
        vertex data.
    */
    std::vector<VertexAttributeType> d_vertexAttributes;

    /*
    \brief  The last clipping region that was set. Negative values in this one are
            not clamped to 0.
    */
    Rectf           d_clippingRegion;
    //! Clipping region clamped to 0, for usage in rendering
    Rectf           d_preparedClippingRegion;
    //! custom transformation matrix
    glm::mat4x4     d_customTransform;
    //! rotation quaternion
    glm::quat       d_rotation;
    //! translation vector
    glm::vec3       d_translation;
    //! scaling vector
    glm::vec3       d_scale;
    //! pivot point for rotation
    glm::vec3       d_pivot;
    //! The alpha value which will be applied to the whole buffer when rendering
    float           d_alpha = 1.f;
    //! The activation number of the RenderTarget that this GeometryBuffer's matrix was last updated for
    mutable unsigned int    d_lastRenderTargetActivationCount = 0;
    //! The vertex count which is determined based on the used vertex layout
    size_t                d_vertexCount = 0;
    //! The amount of vertices that need to be rendered after rendering to the stencil buffer.
    unsigned int    d_postStencilVertexCount = 0;
    //! The BlendMode to use when rendering this GeometryBuffer.
    BlendMode       d_blendMode;
    //! The fill rule that should be used when rendering the geometry.
    PolygonFillRule d_polygonFillRule = PolygonFillRule::NoFilling;
    //! True if clipping will be active for the current batch
    bool            d_clippingActive = false;
    /*
    \brief
        true, when there have been no translations, rotations or other transformations applied to the GeometryBuffer,
        as well as when it is guaranteed that the view projection matrix of the RenderTarget has been unchanged
        since the last update.
    */
    mutable bool    d_matrixValid = false;

};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
