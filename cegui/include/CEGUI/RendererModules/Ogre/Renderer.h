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
#include "../../Sizef.h"
#include "CEGUI/Config.h"

#include <vector>

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
class HardwareVertexBufferSharedPtr;
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

    /*!
      \brief
      Creates a new renderer that can be used to create a context on a new Ogre window
    */
    static OgreRenderer& registerWindow(OgreRenderer& main_window,
        Ogre::RenderTarget &new_window);

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

    //! Conversion function from Ogre to glm
    static glm::mat4 ogreToGlmMatrix(const Ogre::Matrix4& matrix);

    //! Conversion function from glm to Ogre
    static Ogre::Matrix4 glmToOgreMatrix(const glm::mat4& matrix);

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

    //! \brief Sets the correct BlendMode for rendering a GeometryBuffer
    void bindBlendMode(BlendMode blend);

    
    /*!
    \brief 
        Initialises the states for rendering textured geometry
    */
    void initialiseTextureStates();

    /*!
    \brief 
        Returns a vertex buffer larger than size if any exist

        This function also cleans the pool and discards large buffers that 
        aren't used
    */
    Ogre::HardwareVertexBufferSharedPtr getVertexBuffer(size_t min_size);

    //! \brief Puts a vertex buffer back in to the pool
    void returnVertexBuffer(Ogre::HardwareVertexBufferSharedPtr buffer);

    //! \brief Clears vertex buffer pool
    void clearVertexBufferPool();

    // implement CEGUI::Renderer interface
    virtual void setViewProjectionMatrix(const glm::mat4& viewProjMatrix);
    virtual RenderTarget& getDefaultRenderTarget();
    virtual RefCounted<RenderMaterial> createRenderMaterial(const DefaultShaderType shaderType) const;
    virtual GeometryBuffer& createGeometryBufferColoured(CEGUI::RefCounted<RenderMaterial> renderMaterial);
    virtual GeometryBuffer& createGeometryBufferTextured(CEGUI::RefCounted<RenderMaterial> renderMaterial);

    virtual TextureTarget* createTextureTarget(bool addStencilBuffer);
    virtual void destroyTextureTarget(TextureTarget* target);
    virtual void destroyAllTextureTargets();
    virtual bool isTexCoordSystemFlipped() const;

    virtual Texture& createTexture(const String& name);
    virtual Texture& createTexture(const String& name, const String& filename,
        const String& resourceGroup);
    virtual Texture& createTexture(const String& name, const Sizef& size);
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    Ogre::SceneManager& getDummyScene() const;
#endif
    virtual void destroyTexture(Texture& texture);
    virtual void destroyTexture(const String& name);
    virtual void destroyAllTextures();

    virtual Texture& getTexture(const String& name) const;
    virtual bool isTextureDefined(const String& name) const;

    virtual void beginRendering();
    virtual void endRendering();

    void setDisplaySize(const Sizef& sz);
    const Sizef& getDisplaySize() const;
    unsigned int getMaxTextureSize() const;
    const String& getIdentifierString() const;

    /*!
    \brief
        Returns if this OgreRenderer uses an OpenGL based RenderSystem.

    \return
        True if this OgreRenderer uses an OpenGL based RenderSystem.
    */
    bool usesOpenGL();

    /*!
    \brief
        Returns if this OgreRenderer uses a Direct3D based RenderSystem.

    \return
        True if this OgreRenderer uses a Direct3D based RenderSystem.
    */
    bool usesDirect3D();

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
    //! Helper that switches off shader-usage
    void switchShaderUsageOff();
    //! helper that creates and sets up shaders
    void initialiseShaders();
    //! helper to clean up shaders
    void cleanupShaders();

    //! Deletes count number of largest vertex buffers
    void cleanLargestVertexBufferPool(size_t count);
    //! Pointer to the hidden implementation data
    OgreRenderer_impl* d_pimpl;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIOgreRenderer_h_
