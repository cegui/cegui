/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIOpenGL3Renderer_h_
#define _CEGUIOpenGL3Renderer_h_

#include "RendererBase.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
    class OpenGLBaseShaderWrapper;
    class OpenGLBaseShaderManager;
    class OpenGLBaseStateChangeWrapper;

/*!
\brief
    Renderer class to interface with desktop OpenGL version >= 3.2 or OpenGL ES
    version >= 2.

    Note: to use this renderer with OpenGL ES 2.0, the Epoxy OpenGL loading
    library (https://github.com/yaronct/libepoxy, major version 1)
    must first be installed, and CEGUI must be configured with
    "-DCEGUI_BUILD_RENDERER_OPENGL=OFF -DCEGUI_BUILD_RENDERER_OPENGL3=ON
    -DCEGUI_USE_EPOXY=ON -DCEGUI_USE_GLEW=OFF".

    Note: Your OpenGL context must already be initialised when you call this;
    CEGUI will not create the OpenGL context itself. Nothing special has to be
    done to choose between desktop OpenGL and OpenGL ES: the type is
    automatically determined by the type of the current OpenGL context.
*/
class OPENGL_GUIRENDERER_API OpenGL3Renderer : public OpenGLRendererBase
{
public:
    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        The created Renderer will use the current OpenGL viewport as it's
        default surface size.

        This will create and initialise the following objects for you:
        - CEGUI::OpenGL3Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::OpenGL3Renderer object that was created.
    */
    static OpenGL3Renderer& bootstrapSystem(const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        The created Renderer will use the current OpenGL viewport as it's
        default surface size.

        This will create and initialise the following objects for you:
        - CEGUI::OpenGL3Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param display_size
        Size object describing the initial display resolution.

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::OpenGL3Renderer object that was created.
    */
    static OpenGL3Renderer& bootstrapSystem(const Sizef& display_size,
                                            const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::OpenGL3Renderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an OpenGL3Renderer object.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.

    \param abi
        This must be set to CEGUI_VERSION_ABI
    */
    static OpenGL3Renderer& create(const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Create an OpenGL3Renderer object.

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.

    \param abi
        This must be set to CEGUI_VERSION_ABI
    */
    static OpenGL3Renderer& create(const Sizef& display_size,
                                   const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Destroy an OpenGL3Renderer object.

    \param renderer
        The OpenGL3Renderer object to be destroyed.
    */
    static void destroy(OpenGL3Renderer& renderer);

    /*!
    \brief
        Helper to get the wrapper used to check for redundant OpenGL state
        changes.

    \return
        The active OpenGL state change wrapper object.
    */
    OpenGLBaseStateChangeWrapper* getOpenGLStateChanger();

    // base class overrides / abstract function implementations
    void beginRendering() override;
    void endRendering() override;
    Sizef getAdjustedTextureSize(const Sizef& sz) override;
    void setupRenderingBlendMode(const BlendMode mode,
                                 const bool force = false) override;
    RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const override;

#ifdef CEGUI_OPENGL_BIG_BUFFER
    //! OpenGL vao used for the vertices
    GLuint d_verticesSolidVAO = 0;
    GLuint d_verticesTexturedVAO = 0;
    //! OpenGL vbo containing all vertex data
    GLuint d_verticesSolidVBO = 0;
    GLuint d_verticesTexturedVBO = 0;
    //! Size of the vertex data buffer that is currently in use
    GLuint d_verticesSolidVBOSize = 0;
    GLuint d_verticesTexturedVBOSize = 0;
#endif

protected:
    //! Overrides
    void uploadBuffers(RenderingSurface& surface) override;
    void uploadBuffers(const std::vector<GeometryBuffer*>& buffers) override;
    OpenGLGeometryBufferBase* createGeometryBuffer_impl(CEGUI::RefCounted<RenderMaterial> renderMaterial) override;
    TextureTarget* createTextureTarget_impl(bool addStencilBuffer) override;
    //! creates a texture of GL3Texture type
    OpenGLTexture* createTexture_impl(const String& name) override;

    void initialiseRendererIDString();

    /*!
    \brief
        Constructor for OpenGL Renderer objects
    */
    OpenGL3Renderer();

    /*!
    \brief
        Constructor for OpenGL Renderer objects.

    \param display_size
        Size object describing the initial display resolution.
    */
    OpenGL3Renderer(const Sizef& display_size);
    
    void init();

    //! Initialises the ShaderManager and the required OpenGL shaders
    void initialiseOpenGLShaders();
    //! Initialises the OpenGL ShaderWrapper for textured objects
    void initialiseStandardTexturedShaderWrapper();
    //! Initialises the OpenGL ShaderWrapper for coloured objects
    void initialiseStandardColouredShaderWrapper();

    void initialiseStandardTexturedVAO();
    void initialiseStandardColouredVAO();


protected:
    /*!
    \brief
        Destructor for OpenGL3Renderer objects
    */
    virtual ~OpenGL3Renderer();

private:
    //! initialise OGL3TextureTargetFactory that will generate TextureTargets
    void initialiseTextureTargetFactory();

    //! restores all relevant OpenGL States CEGUI touches to their default value
    void restoreChangedStatesToDefaults(bool isAfterRendering);

    void addGeometry(const std::vector<GeometryBuffer*>& buffers);
    void uploadVertexData(std::vector<float>& vertex_data, GLuint vbo_id, GLuint& vbo_max_size);

    //! Wrapper of the OpenGL shader we will use for textured geometry
    OpenGLBaseShaderWrapper* d_shaderWrapperTextured = nullptr;
    //! Wrapper of the OpenGL shader we will use for solid geometry
    OpenGLBaseShaderWrapper* d_shaderWrapperSolid = nullptr;

    //! The wrapper we use for OpenGL calls, to detect redundant state changes and prevent them
    OpenGLBaseStateChangeWrapper* d_openGLStateChanger = nullptr;
    //! Wrapper for creating and handling shaders
    OpenGLBaseShaderManager* d_shaderManager = nullptr;
    //! pointer to a helper that creates TextureTargets supported by the system.
    OGLTextureTargetFactory* d_textureTargetFactory = nullptr;

    std::vector<float> d_vertex_data_solid;
    std::vector<float> d_vertex_data_textured;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

