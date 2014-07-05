/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Paul D Turner, Henri I Hyyryläinen
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
#ifndef _CEGUIOgreRenderer_h_
#define _CEGUIOgreRenderer_h_

#include "../../Renderer.h"
#include "../../Size.h"
#include "../../Vector.h"
#include "CEGUI/Config.h"

#include <vector>
#include <glm/glm.hpp>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef CEGUIOGRERENDERER_EXPORTS
#       define OGRE_GUIRENDERER_API __declspec(dllexport)
#   else
#       define OGRE_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define OGRE_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace Ogre
{
class Root;
class RenderSystem;
class RenderTarget;
class SceneManager;
#if (CEGUI_OGRE_VERSION < ((1 << 16) | (9 << 8) | 0))
class TexturePtr;
#else
template<typename T> class SharedPtr;
class Texture;
typedef SharedPtr<Texture> TexturePtr;
#endif
class Matrix4;
}

#if (CEGUI_OGRE_VERSION >= (2 << 16))
// The new Ogre Compositor2 system has to be used since ViewPorts
// no longer have the required functionality
#define CEGUI_USE_OGRE_COMPOSITOR2
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class OgreGeometryBuffer;
class OgreTexture;
class OgreResourceProvider;
class OgreImageCodec;
class OgreWindowTarget;
struct OgreRenderer_impl;

//! CEGUI::Renderer implementation for the Ogre engine.
class OGRE_GUIRENDERER_API OgreRenderer : public Renderer
{
public:
    /*!
    \brief
        Convenience function that creates all the Ogre specific objects and
        then initialises the CEGUI system with them.

        The created Renderer will use the default Ogre rendering window as the
        default output surface.

        This will create and initialise the following objects for you:
        - CEGUI::OgreRenderer
        - CEGUI::OgreResourceProvider
        - CEGUI::OgreImageCodec
        - CEGUI::System

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::OgreRenderer object that was created.

    \note
        For this to succeed you must have initialised Ogre to auto create the
        rendering window.  If you have not done this, then you'll be wanting to
        use the overload that takes an Ogre::RenderTarget as input.
    */
    static OgreRenderer& bootstrapSystem(const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function that creates all the Ogre specific objects and
        then initialises the CEGUI system with them.

        The create Renderer will use the specified Ogre::RenderTarget as the
        default output surface.

        This will create and initialise the following objects for you:
        - CEGUI::OgreRenderer
        - CEGUI::OgreResourceProvider
        - CEGUI::OgreImageCodec
        - CEGUI::System

    \param target
        Reference to the Ogre::RenderTarget object that the created OgreRenderer
        will use as the default rendering root.

    \param abi
        This must be set to CEGUI_VERSION_ABI

    \return
        Reference to the CEGUI::OgreRenderer object that was created.
    */
    static OgreRenderer& bootstrapSystem(Ogre::RenderTarget& target,
                                         const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::OgreImageCodec
        - CEGUI::OgreResourceProvider
        - CEGUI::OgreRenderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an OgreRenderer object that uses the default Ogre rendering
        window as the default output surface.

    \note
        For this to succeed you must have initialised Ogre to auto create the
        rendering window.  If you have not done this, then you'll be wanting to
        use the overload that takes an Ogre::RenderTarget as input.
    */
    static OgreRenderer& create(const int abi = CEGUI_VERSION_ABI);

    /*!
    \brief
        Create an OgreRenderer object that uses the specified Ogre::RenderTarget
        as the default output surface.
    */
    static OgreRenderer& create(Ogre::RenderTarget& target,
                                const int abi = CEGUI_VERSION_ABI);

    //! destroy an OgreRenderer object.
    static void destroy(OgreRenderer& renderer);

    //! function to create a CEGUI::OgreResourceProvider object
    static OgreResourceProvider& createOgreResourceProvider();

    //! function to destroy a CEGUI::OgreResourceProvider object
    static void destroyOgreResourceProvider(OgreResourceProvider& rp);

    //! function to create a CEGUI::OgreImageCodec object.
    static OgreImageCodec& createOgreImageCodec();

    //! function to destroy a CEGUI::OgreImageCodec object.
    static void destroyOgreImageCodec(OgreImageCodec& ic);

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    //! Function to initialize required Ogre::Compositor2 workspaces
    static void createOgreCompositorResources();

    //! Function to update the workspace render target
    void updateWorkspaceRenderTarget(Ogre::RenderTarget& target);

#endif // CEGUI_USE_OGRE_COMPOSITOR2


    //! set whether CEGUI rendering will occur
    void setRenderingEnabled(const bool enabled);

    //! return whether CEGUI rendering is enabled.
    bool isRenderingEnabled() const;

    /*!
    \brief
        Create a CEGUI::Texture that wraps an existing Ogre texture.

    \param name
        The name for the new texture being created.

    \param tex
        Ogre::TexturePtr for the texture that will be used by the created
        CEGUI::Texture.

    \param take_ownership
        - true if the created Texture will assume ownership of \a tex and
        thus destroy \a tex when the Texture is destroyed.
        - false if ownership of \a tex remains with the client app, and so
        no attempt will be made to destroy \a tex when the Texture is destroyed.
    */
    Texture& createTexture(const String& name, Ogre::TexturePtr& tex,
                           bool take_ownership = false);

    //! set the render states for the specified BlendMode.
    void setupRenderingBlendMode(const BlendMode mode,
                                 const bool force = false);

    /*!
    \brief
        Controls whether rendering done by CEGUI will be wrapped with calls to
        Ogre::RenderSystem::_beginFrame and Ogre::RenderSystem::_endFrame.

        This defaults to enabled and is required when using the default hook
        that automatically calls CEGUI::System::renderGUI via a frame listener.
        If you disable this setting, the automated rendering will also be
        disabled, which is useful when you wish to perform your own calls to the
        CEGUI::System::renderGUI function (and is the sole purpose for this
        setting).

    \param enabled
        - true if _beginFrame and _endFrame should be called.
        - false if _beginFrame and _endFrame should not be called (also disables
          default renderGUI call).
    */
    void setFrameControlExecutionEnabled(const bool enabled);

    /*!
    \brief
        Returns whether rendering done by CEGUI will be wrapped with calls to
        Ogre::RenderSystem::_beginFrame and Ogre::RenderSystem::_endFrame.

        This defaults to enabled and is required when using the default hook
        that automatically calls CEGUI::System::renderGUI via a frame listener.
        If you disable this setting, the automated rendering will also be
        disabled, which is useful when you wish to perform your own calls to the
        CEGUI::System::renderGUI function (and is the sole purpose for this
        setting).

    \return
        - true if _beginFrame and _endFrame will be called.
        - false if _beginFrame and _endFrame will not be called (also means
          default renderGUI call will not be made).
    */
    bool isFrameControlExecutionEnabled() const;

    /*!
    \brief
        Sets all the required render states needed for CEGUI rendering.

        This is a low-level function intended for certain advanced concepts; in
        general it will not be required to call this function directly, since it
        is called automatically by the system when rendering is done.
    */
    void initialiseRenderStateSettings();

    /*!
    \brief
        Sets the Ogre::RenderTarget that should be targetted by the default
        GUIContext.

    \param target
        Reference to the Ogre::RenderTarget object that is to be used as the
        target for output from the default GUIContext.
    */
    void setDefaultRootRenderTarget(Ogre::RenderTarget& target);

    //! Set the current world matrix to the given matrix.
    void setWorldMatrix(const Ogre::Matrix4& m);
    //! Set the current view matrix to the given matrix.
    void setViewMatrix(const Ogre::Matrix4& m);
    //! Set the current projection matrix to the given matrix.
    void setProjectionMatrix(const Ogre::Matrix4& m);
    //! return a const reference to the current world matrix.
    const Ogre::Matrix4& getWorldMatrix() const;
    //! return a const reference to the current view matrix.
    const Ogre::Matrix4& getViewMatrix() const;
    //! return a const reference to the current projection matrix.
    const Ogre::Matrix4& getProjectionMatrix() const;

    /*!
    \brief
        Return a const reference to the final transformation matrix that
        should be used when transforming geometry.

    \note
        The projection used when building this matrix is correctly adjusted
        according to whether the current Ogre::RenderTarget requires textures
        to be flipped (i.e it does the right thing for both D3D and OpenGL).
    */
    const Ogre::Matrix4& getWorldViewProjMatrix() const;


    void bindBlendMode(BlendMode blend);

    // implement CEGUI::Renderer interface
    RenderTarget& getDefaultRenderTarget();

    RefCounted<RenderMaterial> createRenderMaterial(
        const DefaultShaderType shaderType) const;
    GeometryBuffer& createGeometryBufferColoured(
        CEGUI::RefCounted<RenderMaterial> renderMaterial);
    GeometryBuffer& createGeometryBufferTextured(
        CEGUI::RefCounted<RenderMaterial> renderMaterial);

    void destroyGeometryBuffer(const GeometryBuffer& buffer);
    void destroyAllGeometryBuffers();
    TextureTarget* createTextureTarget();
    void destroyTextureTarget(TextureTarget* target);
    void destroyAllTextureTargets();

    Texture& createTexture(const String& name);
    Texture& createTexture(const String& name, const String& filename,
        const String& resourceGroup);
    Texture& createTexture(const String& name, const Sizef& size);


    Ogre::SceneManager& getDummyScene() const;

    void destroyTexture(Texture& texture);
    void destroyTexture(const String& name);
    void destroyAllTextures();

    Texture& getTexture(const String& name) const;
    bool isTextureDefined(const String& name) const;

    void beginRendering();
    void endRendering();

    void setDisplaySize(const Sizef& sz);
    const Sizef& getDisplaySize() const;
    const Vector2f& getDisplayDPI() const;
    uint getMaxTextureSize() const;
    const String& getIdentifierString() const;
    

    //! \brief Converts a glm::mat4 to Ogre::Matrix4
    static void convertGLMMatrixToOgreMatrix(const glm::mat4& source, 
        Ogre::Matrix4& target);

    //! \brief Converts an Ogre::Matrix4 to a glm::mat4
    static void convertOgreMatrixToGLMMatrix(const Ogre::Matrix4& source, 
        glm::mat4& target);


protected:
    //! default constructor.
    OgreRenderer();
    //! constructor takin the Ogre::RenderTarget to use as the default root.
    OgreRenderer(Ogre::RenderTarget& target);
    //! destructor.
    virtual ~OgreRenderer();

    //! checks Ogre initialisation.  throws exceptions if an issue is detected.
    void checkOgreInitialised();
    //! helper to throw exception if name is already used.
    void throwIfNameExists(const String& name) const;
    //! helper to safely log the creation of a named texture
    static void logTextureCreation(const String& name);
    //! helper to safely log the destruction of a named texture
    static void logTextureDestruction(const String& name);

    //! common parts of constructor
    void constructor_impl(Ogre::RenderTarget& target);
    //! helper that creates and sets up shaders
    void initialiseShaders();
    //! helper to clean up shaders
    void cleanupShaders();

    //! Pointer to the hidden implementation data
    OgreRenderer_impl* d_pimpl;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIOgreRenderer_h_
