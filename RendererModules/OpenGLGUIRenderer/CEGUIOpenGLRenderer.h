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

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUISize.h"
#include "CEGUIVector.h"
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
class ImageCodec;
class DynamicModule;

class OpenGLTexture;
class RenderTarget;
class RenderingRoot;
class OpenGLTextureTarget;
class OpenGLGeometryBuffer;

/*!
\brief
    Renderer class to interface with OpenGL
*/
class OPENGL_GUIRENDERER_API OpenGLRenderer : public Renderer
{
public:
    /*!
    \brief
        Create an OpenGLRenderer object.

    \param codec
        A pointer to a user provided image codec. The renderer does not take
        ownership of the codec object.
    */

    static OpenGLRenderer& create(ImageCodec* codec = 0);

    /*!
    \brief
        Create an OpenGLRenderer object.

    \param display_size
        Size object describing the initial display resolution.

    \param codec
        A pointer to a user provided image codec. The renderer does not take
        ownership of the codec object.
    */
    static OpenGLRenderer& create(const Size& display_size,
                                  ImageCodec* codec = 0);

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
    Texture& createTexture(const String& filename,
                                   const String& resourceGroup);
    Texture& createTexture(const Size& size);
    void destroyTexture(Texture& texture);
    void destroyAllTextures();
    void beginRendering();
    void endRendering();
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
        Set the size of the display in pixels.

        If your viewport size changes, you can call this function with the new
        size in pixels to update the rendering area.

    \note
        This method will cause the EventDisplaySizeChanged event to fire if the
        display size has changed.

    \param sz
        Size object describing the size of the display.
    */
    void setDisplaySize(const Size& sz);

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
        Retrieve the image codec used internaly
    */
    ImageCodec& getImageCodec();

    /*!
    \brief
        Set the image codec to use for loading textures
    */
    void setImageCodec(const String& codecName);

    /*!
    \brief
        Set the image codec to use from an existing image codec.

        In this case the renderer does not take the ownership of the image codec
        object.

    \param codec
        a pointer to an image codec object 
    */
    void setImageCodec(ImageCodec* codec);

    /*!
    \brief
        Set the name of the default image codec to be used.
    */
    static void setDefaultImageCodecName(const String& codecName);

    /*!
    \brief
        Get the name of the default image codec.
    */
    static const String& getDefaultImageCodecName();

private:
    /*!
    \brief
        Constructor for OpenGL Renderer objects

    \param codec
        A pointer to a user provided image codec. The renderer does not take
        ownership of the codec object.
    */
    OpenGLRenderer(ImageCodec* codec);

    /*!
    \brief
        Constructor for OpenGL Renderer objects.

    \param display_size
        Size object describing the initial display resolution.

    \param codec
        A pointer to a user provided image codec. The renderer does not take
        ownership of the codec object.
    */
    OpenGLRenderer(const Size& display_size, ImageCodec* codec);

    /*!
    \brief
        Destructor for OpenGLRenderer objects
    */
    virtual ~OpenGLRenderer();

    //! init the extra GL states enabled via enableExtraStateSettings
    void setupExtraStates();

    //! cleanup the extra GL states enabled via enableExtraStateSettings
    void cleanupExtraStates();

    //! setup image codec 
    void setupImageCodec(const String& codecName);

    //! cleanup image codec 
    void cleanupImageCodec();

    //! String holding the renderer identification text.
    static const String d_rendererID;
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
    //! Holds a pointer to the image codec to use.
    ImageCodec* d_imageCodec;
    /** Holds a pointer to the image codec module. If d_imageCodecModule is 0 we
     *  are not owner of the image codec object
     */
    DynamicModule* d_imageCodecModule;
    //! Holds the name of the default codec to use.
    static String d_defaultImageCodecName;
  };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif // end of guard _CEGUIOpenGLRenderer_h_
