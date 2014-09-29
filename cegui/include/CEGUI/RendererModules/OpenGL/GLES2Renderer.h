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
#ifndef _CEGUIOpenGLES2Renderer_h_
#define _CEGUIOpenGLES2Renderer_h_

#include "RendererBase.h"

namespace CEGUI
{
    class OpenGL3ShaderWrapper;
    class OpenGL3ShaderManager;
    class OpenGL3StateChangeWrapper;

/*!
\brief
    Renderer class to interface with OpenGL
*/
class OPENGL_GUIRENDERER_API OpenGLES2Renderer : public OpenGLRendererBase
{
public:
    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        The created Renderer will use the current OpenGL viewport as it's
        default surface size.

        This will create and initialise the following objects for you:
        - CEGUI::OpenGLES2Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::OpenGLES2Renderer object that was created.
    */
    static OpenGLES2Renderer& bootstrapSystem(const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        The created Renderer will use the current OpenGL viewport as it's
        default surface size.

        This will create and initialise the following objects for you:
        - CEGUI::OpenGLES2Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param display_size
        Size object describing the initial display resolution.

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::OpenGLES2Renderer object that was created.
    */
    static OpenGLES2Renderer& bootstrapSystem(const Sizef& display_size,
                                            const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::OpenGLES2Renderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an OpenGLES2Renderer object.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.

    \param abi
        This must be set to CEGUI_VERSION_ABI
    */
    static OpenGLES2Renderer& create(const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Create an OpenGLES2Renderer object.

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.

    \param abi
        This must be set to CEGUI_VERSION_ABI
    */
    static OpenGLES2Renderer& create(const Sizef& display_size,
                                   const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Destroy an OpenGLES2Renderer object.

    \param renderer
        The OpenGLES2Renderer object to be destroyed.
    */
    static void destroy(OpenGLES2Renderer& renderer);

    /*!
    \brief
        Helper to get the wrapper used to check for redundant OpenGL state
        changes.

    \return
        The active OpenGL state change wrapper object.
    */
    OpenGL3StateChangeWrapper* getOpenGLStateChanger();

    // base class overrides / abstract function implementations
    void beginRendering();
    virtual Sizef getAdjustedTextureSize(const Sizef& sz);
    void endRendering();
    Sizef getAdjustedTextureSize(const Sizef& sz) const;
    bool isS3TCSupported() const;
    void setupRenderingBlendMode(const BlendMode mode,
                                 const bool force = false);
    RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const;

protected:
    OpenGLGeometryBufferBase* createGeometryBuffer_impl(CEGUI::RefCounted<RenderMaterial> renderMaterial);
    TextureTarget* createTextureTarget_impl();

    void initialiseRendererIDString();

    /*!
    \brief
        Constructor for OpenGL Renderer objects
    */
    OpenGLES2Renderer();

    /*!
    \brief
        Constructor for OpenGL Renderer objects.

    \param display_size
        Size object describing the initial display resolution.
    */
    OpenGLES2Renderer(const Sizef& display_size);

    //! Initialises the ShaderManager and the required OpenGL shaders
    void initialiseOpenGLShaders();
    //! Initialises the OpenGL ShaderWrapper for textured objects
    void initialiseStandardTexturedShaderWrapper();
    //! Initialises the OpenGL ShaderWrapper for coloured objects
    void initialiseStandardColouredShaderWrapper();

    void initialiseGLExtensions();

    /*!
    \brief
        Destructor for OpenGLES2Renderer objects
    */
    virtual ~OpenGLES2Renderer();

    //! initialise OGLES2TextureTargetFactory that will generate TextureTargets
    void initialiseTextureTargetFactory();

    //! init the extra GL states enabled via enableExtraStateSettings
    void setupExtraStates();

    //! Wrapper of the OpenGL shader we will use for textured geometry
    OpenGL3ShaderWrapper* d_shaderWrapperTextured;
    //! Wrapper of the OpenGL shader we will use for solid geometry
    OpenGL3ShaderWrapper* d_shaderWrapperSolid;

    //! The wrapper we use for OpenGL calls, to detect redundant state changes and prevent them
    OpenGL3StateChangeWrapper* d_openGLStateChanger;
    //! The ShaderManager  takes care of the creation of standard OpenGL Shaders and their deletion
    OpenGL3ShaderManager* d_shaderManager;
    //! whether S3TC texture compression is supported by the context
    bool d_s3tcSupported;
    //! pointer to a helper that creates TextureTargets supported by the system.
    OGLTextureTargetFactory* d_textureTargetFactory;
};

}

#endif

