/***********************************************************************
    filename:   CEGUINullRenderer.h
    created:    Fri Jan 15 2010
    author:     Eugene Marcotte
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
#ifndef _CEGUINullRenderer_h_
#define _CEGUINullRenderer_h_

#include "../../CEGUIRenderer.h"
#include "../../CEGUISize.h"
#include "../../CEGUIVector.h"

#include <vector>
#include <map>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef NULL_GUIRENDERER_EXPORTS
#       define NULL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define NULL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define NULL_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
class NullGeometryBuffer;
class NullTexture;
class NullRenderTarget;

//! CEGUI::Renderer implementation for no particular engine
class NULL_GUIRENDERER_API NullRenderer : public Renderer
{
public:
    /*!
    \brief
        Convenience function that creates all the necessary objects
        then initialises the CEGUI system with them.

        This will create and initialise the following objects for you:
        - CEGUI::NullRenderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \return
        Reference to the CEGUI::NullRenderer object that was created.

    */
    static NullRenderer& bootstrapSystem();
    
    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::NullRenderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an NullRenderer object 
    */
    static NullRenderer& create();

    //! destory an NullRenderer object.
    static void destroy(NullRenderer& renderer);

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
    NullRenderer();
	//! common construction things.
	void constructor_impl();
    //! destructor.
    virtual ~NullRenderer();

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! What the renderer considers to be the current display size.
    Size d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    Vector2 d_displayDPI;
    //! The default rendering root object
    RenderingRoot* d_defaultRoot;
    //! The default RenderTarget (used by d_defaultRoot)
    NullRenderTarget* d_defaultTarget;
    //! container type used to hold TextureTargets we create.
    typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<NullGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::vector<NullTexture*> TextureList;
    //! Container used to track textures.
    TextureList d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUINullRenderer_h_
