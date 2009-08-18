/***********************************************************************
    filename:   CEGUIOgreRenderer.h
    created:    Tue Feb 17 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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

#include "../../CEGUIRenderer.h"
#include "../../CEGUISize.h"
#include "../../CEGUIVector.h"

#include <OgreBlendMode.h>
#include <OgreTextureUnitState.h>

#include <vector>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef OGRE_GUIRENDERER_EXPORTS
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
}

// Start of CEGUI namespace section
namespace CEGUI
{
class OgreGeometryBuffer;
class OgreTexture;
class OgreResourceProvider;
class OgreImageCodec;

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

    \return
        Reference to the CEGUI::OgreRenderer object that was created.

    \note
        For this to succeed you must have initialised Ogre to auto create the
        rendering window.  If you have not done this, then you'll be wanting to
        use the overload that takes an Ogre::RenderTarget as input.
    */
    static OgreRenderer& bootstrapSystem();

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

    \return
        Reference to the CEGUI::OgreRenderer object that was created.
    */
    static OgreRenderer& bootstrapSystem(Ogre::RenderTarget& target);

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
    static OgreRenderer& create();

    /*!
    \brief
        Create an OgreRenderer object that uses the specified Ogre::RenderTarget
        as the default output surface.
    */
    static OgreRenderer& create(Ogre::RenderTarget& target);

    //! destory an OgreRenderer object.
    static void destroy(OgreRenderer& renderer);

    //! function to create a CEGUI::OgreResourceProvider object
    static OgreResourceProvider& createOgreResourceProvider();

    //! function to destroy a CEGUI::OgreResourceProvider object
    static void destroyOgreResourceProvider(OgreResourceProvider& rp);

    //! function to create a CEGUI::OgreImageCodec object.
    static OgreImageCodec& createOgreImageCodec();

    //! function to destroy a CEGUI::OgreImageCodec object.
    static void destroyOgreImageCodec(OgreImageCodec& ic);

    //! set whether CEGUI rendering will occur
    void setRenderingEnabled(const bool enabled);

    //! return whether CEGUI rendering is enabled.
    bool isRenderingEnabled() const;

    /*!
    \brief
        Create a CEGUI::Texture that wraps an existing Ogre texture.

    \param tex
        Ogre::TexturePtr for the texture that will be used by the created
        CEGUI::Texture.

    \param take_ownership
        - true if the created Texture will assume ownership of \a tex and
        thus destroy \a tex when the Texture is destroyed.
        - false if ownership of \a tex remains with the client app, and so
        no attempt will be made to destroy \a tex when the Texture is destroyed.
    */
    Texture& createTexture(Ogre::TexturePtr& tex, bool take_ownership = false);

    // implement CEGUI::Renderer interface
    RenderingRoot& getDefaultRenderingRoot();
    GeometryBuffer& createGeometryBuffer();
    void destroyGeometryBuffer(const GeometryBuffer& buffer);
    void destroyAllGeometryBuffers();
    TextureTarget* createTextureTarget();
    void destroyTextureTarget(TextureTarget* target);
    void destroyAllTextureTargets();
    Texture& createTexture();
    Texture& createTexture(const String& filename, const String& resourceGroup);
    Texture& createTexture(const Size& size);
    void destroyTexture(Texture& texture);
    void destroyAllTextures();
    void beginRendering();
    void endRendering();
    void setDisplaySize(const Size& sz);
    const Size& getDisplaySize() const;
    const Vector2& getDisplayDPI() const;
    uint getMaxTextureSize() const;
    const String& getIdentifierString() const;

protected:
    //! default constructor.
    OgreRenderer();
    //! constructor takin the Ogre::RenderTarget to use as the default root.
    OgreRenderer(Ogre::RenderTarget& target);
    //! destructor.
    virtual ~OgreRenderer();

    //! checks Ogre initialisation.  throws exceptions if an issue is detected.
    void checkOgreInitialised();

    //! common parts of constructor
    void constructor_impl(Ogre::RenderTarget& target);

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! What the renderer considers to be the current display size.
    Size d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    Vector2 d_displayDPI;
    //! The default rendering root object
    RenderingRoot* d_defaultRoot;
    //! The default RenderTarget (used by d_defaultRoot)
    RenderTarget* d_defaultTarget;
    //! container type used to hold TextureTargets we create.
    typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<OgreGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::vector<OgreTexture*> TextureList;
    //! Container used to track textures.
    TextureList d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
    //! OGRE root object ptr
    Ogre::Root* d_ogreRoot;
    //! Pointer to the render system for Ogre.
    Ogre::RenderSystem* d_renderSystem;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIOgreRenderer_h_
