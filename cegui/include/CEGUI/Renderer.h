/***********************************************************************
    created:  20/2/2004
    author:   Paul D Turner

    purpose: Defines interface for abstract Renderer class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIRenderer_h_
#define _CEGUIRenderer_h_

#include "CEGUI/Base.h"
#include "CEGUI/RefCounted.h"
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <set>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


namespace CEGUI
{
class ShaderWrapper;
class RenderMaterial;
class String;
class Sizef;

//----------------------------------------------------------------------------//

/*!
\brief
    Enumerated type that contains the valid options that specify the type of
    blending that is to be performed for subsequent rendering operations.
*/
enum class BlendMode : int
{
    //! Invalid mode indicator.
    Invalid,
    //! Use normal blending mode.
    Normal,
    //! Use blending mode suitable for textures with premultiplied colours.
    RttPremultiplied
};


//----------------------------------------------------------------------------//

/*!
\brief
    Enum for the default shader types that the Renderers have to offer
*/
enum class DefaultShaderType : int
{
    //! A shader for solid, coloured geometry
    Solid,
    //! A shader for textured geometry, used in most CEGUI widgets
    Textured,
    //! Count of types
    Count
};

//----------------------------------------------------------------------------//

/*!
\brief
    Abstract class defining the basic required interface for Renderer objects.

    Objects derived from Renderer are the means by which the GUI system
    interfaces with specific rendering technologies.  To use a rendering system
    or API to draw CEGUI imagery requires that an appropriate Renderer object be
    available.
*/
class CEGUIEXPORT Renderer
{
public:

    Renderer(float fontScale = 1.0f);

    virtual ~Renderer();

    //! This is the DPI value that was assumed up to CEGUI version 0.8.X
    static const int ReferenceDpiValue = 96;

    /*!
    \brief
        Returns the default RenderTarget object.  The default render target is
        is typically one that targets the entire screen (or rendering window).

    \return
        Reference to a RenderTarget object.
    */
    virtual RenderTarget& getDefaultRenderTarget() = 0;

    /*!
    \brief
        Create a GeometryBuffer for textured geometry and return a reference to it.
        You should remove the GeometryBuffer from any RenderQueues and call destroyGeometryBuffer
        when you want to destroy the GeometryBuffer.

    \return
        GeometryBuffer object.
    */
    virtual GeometryBuffer& createGeometryBufferTextured(RefCounted<RenderMaterial> renderMaterial) = 0;

    /*!
    \brief
        Creates a GeometryBuffer for textured geometry with its default RenderMaterial and return a
        reference to it.
        You should remove the GeometryBuffer from any RenderQueues and call destroyGeometryBuffer
        when you want to destroy the GeometryBuffer.

    \return
        GeometryBuffer object.
    */
    GeometryBuffer& createGeometryBufferTextured();

    /*!
    \brief
        Creates a GeometryBuffer for coloured geometry and return a reference to it.
        You should remove the GeometryBuffer from any RenderQueues and call destroyGeometryBuffer
        when you want to destroy the GeometryBuffer.

    \return
        GeometryBuffer object.
    */
    virtual GeometryBuffer& createGeometryBufferColoured(RefCounted<RenderMaterial> renderMaterial) = 0;

    /*!
    \brief
        Creates a GeometryBuffer for coloured geometry with its default RenderMaterial and return a
        reference to it.
        You should remove the GeometryBuffer from any RenderQueues and call destroyGeometryBuffer
        when you want to destroy the GeometryBuffer.

    \return
        GeometryBuffer object.
    */
    GeometryBuffer& createGeometryBufferColoured();

    /*!
    \brief
        Destroys a GeometryBuffer that was returned when calling one of the
        createGeometryBuffer functions. Before destroying any GeometryBuffer
        you should ensure that it has been removed from any RenderQueue that
        was using it.

    \param buffer
        The GeometryBuffer object to be destroyed.
    */
    void destroyGeometryBuffer(GeometryBuffer& buffer);

    /*!
    \brief
        Destroys all GeometryBuffer objects created by this Renderer.
    */
    void destroyAllGeometryBuffers();

    /*!
    \brief
        Create a TextureTarget that can be used to cache imagery; this is a
        RenderTarget that does not lose it's content from one frame to another.

        If the renderer is unable to offer such a thing, 0 should be returned.

    \return
        Pointer to a TextureTarget object that is suitable for caching imagery,
        or 0 if the renderer is unable to offer such a thing.

   \param addStencilBuffer
        A boolean that defines whether a stencil buffer should be attached to the
        TextureTarget or not.
    */
    virtual TextureTarget* createTextureTarget(bool addStencilBuffer) = 0;

    /*!
    \brief
        Function that cleans up TextureTarget objects created with the
        createTextureTarget function.

    \param target
        A pointer to a TextureTarget object that was previously returned from a
        call to createTextureTarget.
    */
    virtual void destroyTextureTarget(TextureTarget* target) = 0;

    /*!
    \brief
        Destroys all TextureTarget objects created by this Renderer.
    */
    virtual void destroyAllTextureTargets() = 0;

    /*!
    \brief
        Creates a 'null' Texture object.

    \param name
        String holding the name for the new texture.  Texture names must be
        unique within the Renderer.

    \return
        A newly created Texture object.  The returned Texture object has no size
        or imagery associated with it.

    \exceptions
        - AlreadyExistsException - thrown if a Texture object named \a name
          already exists within the system.
    */
    virtual Texture& createTexture(const String& name) = 0;

    /*!
    \brief
        Create a Texture object using the given image file.

    \param name
        String holding the name for the new texture.  Texture names must be
        unique within the Renderer.

    \param filename
        String object that specifies the path and filename of the image file to
        use when creating the texture.

    \param resourceGroup
        String objet that specifies the resource group identifier to be passed
        to the resource provider when loading the texture file \a filename.

    \return
        A newly created Texture object.  The initial content of the texture
        memory is the requested image file.

    \note
        Due to possible limitations of the underlying hardware, API or engine,
        the final size of the texture may not match the size of the loaded file.
        You can check the ultimate sizes by querying the Texture object
        after creation.

    \exceptions
        - AlreadyExistsException - thrown if a Texture object named \a name
          already exists within the system.
    */
    virtual Texture& createTexture(const String& name,
                                   const String& filename,
                                   const String& resourceGroup) = 0;

    /*!
    \brief
        Creates a Texture object with the given pixel dimensions as specified by
        \a size.

    \param name
        String holding the name for the new texture.  Texture names must be
        unique within the Renderer.

    \param size
        Size object that describes the desired texture size.

    \return
        A newly created Texture object.  The initial contents of the texture
        memory is undefined.

    \note
        Due to possible limitations of the underlying hardware, API or engine,
        the final size of the texture may not match the requested size.  You can
        check the ultimate sizes by querying the Texture object after creation.

    \exceptions
        - AlreadyExistsException - thrown if a Texture object named \a name
          already exists within the system.
    */
    virtual Texture& createTexture(const String& name, const Sizef& size) = 0;

    /*!
    \brief
        Destroys a Texture object that was previously created by calling the
        createTexture functions.

    \param texture
        Texture object to be destroyed.
    */
    virtual void destroyTexture(Texture& texture) = 0;

    /*!
    \brief
        Destroys a Texture object that was previously created by calling the
        createTexture functions.

    \param name
        String holding the name of the texture to destroy.
    */
    virtual void destroyTexture(const String& name) = 0;

    /*!
    \brief
        Destroys all Texture objects created by this Renderer.
    */
    virtual void destroyAllTextures() = 0;

    /*!
    \brief
        Returns a Texture object that was previously created by calling the
        createTexture functions.

    \param name
        String holding the name of the Texture object to be returned.

    \exceptions
        - UnknownObjectException - thrown if no Texture object named \a name
          exists within the system.
    */
    virtual Texture& getTexture(const String& name) const = 0;

    //! Return whether a texture with the given name exists.
    virtual bool isTextureDefined(const String& name) const = 0;

    /*!
    \brief
        Performs any operations required to put the system into a state ready
        for rendering operations to begin.
    */
    virtual void beginRendering() = 0;

    /*!
    \brief
        Performs any operations required to finalise rendering.
    */
    virtual void endRendering() = 0;

    /*!
    \brief
        Sets the size of the display or host window in pixels for this Renderer
        object.

        This is intended to be called by the System as part of the notification
        process when display size changes are notified to it via the
        System::notifyDisplaySizeChanged function.

    \note
        The Renderer implementation should not use this function other than to
        perform internal state updates on the Renderer and related objects.

    \param size
        Size object describing the dimesions of the current or host window in
        pixels.
    */
    virtual void setDisplaySize(const Sizef& size) = 0;

    /*!
    \brief
        Returns the size of the display or host window in pixels.

    \return
        Size object describing the pixel dimesntions of the current display or
        host window.
    */
    virtual const Sizef& getDisplaySize() const = 0;

    /*!
    \brief
        Returns the pixel size of the maximum supported texture.

    \return
        Size of the maximum supported texture in pixels.
    */
    virtual unsigned int getMaxTextureSize() const = 0;

    /*!
    \brief
        Returns identification string for the renderer module.

    \return
        String object holding text that identifies the Renderer in use.
    */
    virtual const String& getIdentifierString() const = 0;

    /*!
    \brief
        Creates a copy of the specified default shader type.

    \param shaderType
        Specifies the type of CEGUI shader that the RenderMaterial should be based on

    \return
        A copy of the specified default shader type.
    */
    virtual RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const = 0;

    /*!
    \brief
        Marks all matrices of all GeometryBuffers as dirty, so that they will be updated before their next usage.
        This is a special function that will only be used if a RenderTarget has been rendered more than the amount
        of numbers that can be stored in the counter, at which point the counter restarts at 0. This is necessary
        to ensure that no Matrix will be reused although it actually would need updating (for example in the case
        the Buffer was not rendered for exactly the amount of maximum countable times, and is updated again exactly at
        the same count)
    */
    virtual void invalidateGeomBufferMatrices(const CEGUI::RenderTarget* renderTarget);

    /*!
    \brief
        Goes through all geometry buffers and updates their texture
        coordinates if the texture matches the supplied texture.
    */
    void updateGeometryBufferTexCoords(const Texture* texture, const float scaleFactor);

    virtual void uploadBuffers(RenderingSurface& /*surface*/) {}
    virtual void uploadBuffers(const std::vector<GeometryBuffer*>& /*buffers*/) {}

    /*!
    \brief
        Sets the active render target.

    \param renderTarget
        The active RenderTarget.
    */
    virtual void setActiveRenderTarget(RenderTarget* renderTarget);
        
    /*!
    \brief
        Retruns the active render target.

    \return
        The active RenderTarget.
    */
    RenderTarget* getActiveRenderTarget() { return d_activeRenderTarget; }

    /*!
    \brief
        Sets the currently active view projection matrix.

    \param viewProjectionMatrix
        The view projection matrix that should be set as the new active matrix.
    */
    virtual void setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix);

    /*!
    \brief
        Returns the currently active view projection matrix.

    \return
        The currently active view projection matrix.
    */
    const glm::mat4& getViewProjectionMatrix() const { return d_viewProjectionMatrix; }

    /*!
    \brief
        Returns if the texture coordinate system is vertically flipped or not. The original of a
        texture coordinate system is typically located either at the the top-left or the bottom-left.
        CEGUI, Direct3D and most rendering engines assume it to be on the top-left. OpenGL assumes it to
        be at the bottom left.        
 
        This function is intended to be used when generating geometry for rendering the TextureTarget
        onto another surface. It is also intended to be used when trying to use a custom texture (RTT)
        inside CEGUI using the Image class, in order to determine the Image coordinates correctly.

    \return
        - true if flipping is required: the texture coordinate origin is at the bottom left
        - false if flipping is not required: the texture coordinate origin is at the top left
    */
    virtual bool isTexCoordSystemFlipped() const = 0;

    /*!
    \brief
        Gets the Font scale factor to be used when rendering scalable Fonts.

    \return
        The currently set Font scale factor
    */
    float getFontScale() const
    {
        return d_fontScale;
    }

    /*!
    \brief
        Sets the Font scale factor to be used when rendering scalable Fonts.

        This updates all Fonts but will not invalidate the Windows. If you
        want to also invalidate all Windows to be affected by all Fonts,
        you have to do this after setting the new Font scale manually.
    */
    void setFontScale(const float fontScale);

    /*!
    \brief
        Calculates and returns the font scale factor, based on the 
        reference DPI value of 96, which was assumed as fixed DPI up to
        CEGUI 0.8.X. Point sizes are rendered in CEGUI based on this fixed
        DPI. If you want to render point sizes according to a higher DPI,
        use this function together with setFontScale.

    \return
        The result of dpiValue / ReferenceDpiValue

    \see Renderer::setFontScale
    \see System::getSystemDPI
    */
    static float dpiToFontScale(const float dpiValue)
    {
         return dpiValue / static_cast<float>(ReferenceDpiValue);
    }

protected:
    /*!
    \brief
        Adds a created GeometryBuffer, which was returned when calling one of the
        createGeometryBuffer functions, to the list of GeometryBuffers.

    \param buffer
        The GeometryBuffer object to be destroyed.
    */
    void addGeometryBuffer(GeometryBuffer& buffer);

    //! The currently active RenderTarget
    RenderTarget* d_activeRenderTarget = nullptr;

    //! The currently active view projection matrix 
    glm::mat4 d_viewProjectionMatrix;

private:

    // FIXME: extremely stupid way to associate material type to shader pointer.
    // This is only to maintain compatibility with existing renderers. This should be rewritten later!
    const ShaderWrapper* d_coloredShader = nullptr;
    const ShaderWrapper* d_texturedShader = nullptr;

    //! Container used to track active geometry buffers.
    std::set<GeometryBuffer*> d_geometryBuffers;
    //! Pool of reusable geometry buffers
    std::map<const ShaderWrapper*, std::vector<GeometryBuffer*>> d_geomeryBufferPool;
    //! The Font scale factor to be used when rendering Fonts (except Bitmap Fonts).
    float d_fontScale;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
