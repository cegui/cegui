/***********************************************************************
    filename:   CEGUIOpenGLRenderer.h
    created:    Sun Jan 11 2009
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
#ifndef _CEGUIOpenGLRenderer_h_
#define _CEGUIOpenGLRenderer_h_

#include "../../CEGUIBase.h"
#include "../../CEGUIRenderer.h"
#include "../../CEGUISize.h"
#include "../../CEGUIVector.h"
#include "CEGUIOpenGL.h"
#include <vector>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef OPENGL_GUIRENDERER_EXPORTS
#       define OPENGL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define OPENGL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define OPENGL_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
class OpenGLTexture;
class OpenGLTextureTarget;
class OpenGLGeometryBuffer;
class OGLTextureTargetFactory;

/*!
\brief
    Renderer class to interface with OpenGL
*/
class OPENGL_GUIRENDERER_API OpenGLRenderer : public Renderer
{
public:
    //! Enumeration of valid texture target types.
    enum TextureTargetType
    {
        //! Automatically choose the best type available.
        TTT_AUTO,
        //! Use targets based on frame buffer objects if available, else none.
        TTT_FBO,
        //! Use targets based on pbuffer support if available, else none.
        TTT_PBUFFER,
        //! Disable texture targets.
        TTT_NONE
    };

    /*!
    \brief
        Create an OpenGLRenderer object.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    static OpenGLRenderer& create(const TextureTargetType tt_type = TTT_AUTO);

    /*!
    \brief
        Create an OpenGLRenderer object.

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    static OpenGLRenderer& create(const Size& display_size,
                                  const TextureTargetType tt_type = TTT_AUTO);

    /*!
    \brief
        Destroy an OpenGLRenderer object.

    \param renderer
        The OpenGLRenderer object to be destroyed.
    */
    static void destroy(OpenGLRenderer& renderer);

    // implement Renderer interface
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

    /*!
    \brief
        Create a texture that uses an existing OpenGL texture with the specified
        size.  Note that it is your responsibility to ensure that the OpenGL
        texture is valid and that the specified size is accurate.

    \param sz
        Size object that describes the pixel size of the OpenGL texture
        identified by \a tex.

    \return
        Texture object that wraps the OpenGL texture \a tex, and whose size is
        specified to be \a sz.
    */
    Texture& createTexture(GLuint tex, const Size& sz);

    /*!
    \brief
        Tells the renderer to initialise some extra states beyond what it
        directly needs itself for CEGUI.

        This option is useful in cases where you've made changes to the default
        OpenGL state and do not want to save/restore those between CEGUI
        rendering calls.  Note that this option will not deal with every
        possible state or extension - if you want a state added here, make a
        request and we'll consider it ;)
    */
    void enableExtraStateSettings(bool setting);

    /*!
    \brief
        Grabs all the loaded textures from Texture RAM and stores them in a
        local data buffer.  This function invalidates all textures, and
        restoreTextures must be called before any CEGUI rendering is done for
        predictable results.
    */
    void grabTextures();

    /*!
    \brief
        Restores all the loaded textures from the local data buffers previously
        created by 'grabTextures'
    */
    void restoreTextures();

    /*!
    \brief
        Helper to return a valid texture size according to reported OpenGL
        capabilities.

    \param sz
        Size object containing input size.

    \return
        Size object containing - possibly different - output size.
    */
    Size getAdjustedTextureSize(const Size& sz) const;

    /*!
    \brief
        Utility function that will return \a f if it's a power of two, or the
        next power of two up from \a f if it's not.
    */
    static float getNextPOTSize(const float f);

private:
    /*!
    \brief
        Constructor for OpenGL Renderer objects

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    OpenGLRenderer(const TextureTargetType tt_type);

    /*!
    \brief
        Constructor for OpenGL Renderer objects.

    \param display_size
        Size object describing the initial display resolution.

    \param tt_type
        Specifies one of the TextureTargetType enumerated values indicating the
        desired TextureTarget type to be used.
    */
    OpenGLRenderer(const Size& display_size, const TextureTargetType tt_type);

    /*!
    \brief
        Destructor for OpenGLRenderer objects
    */
    virtual ~OpenGLRenderer();

    //! init the extra GL states enabled via enableExtraStateSettings
    void setupExtraStates();

    //! cleanup the extra GL states enabled via enableExtraStateSettings
    void cleanupExtraStates();

    //! initialise OGLTextureTargetFactory that will generate TextureTargets
    void initialiseTextureTargetFactory(const TextureTargetType tt_type);

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
    typedef std::vector<OpenGLGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::vector<OpenGLTexture*> TextureList;
    //! Container used to track textures.
    TextureList d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
    //! option of whether to initialise extra states that may not be at default
    bool d_initExtraStates;
    //! pointer to a helper that creates TextureTargets supported by the system.
    OGLTextureTargetFactory* d_textureTargetFactory;
  };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUIOpenGLRenderer_h_
