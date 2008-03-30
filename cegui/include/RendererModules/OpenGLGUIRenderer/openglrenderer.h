/***********************************************************************
    filename: openglrenderer.h
    created: 9/4/2004
    authors:
        Original OpenGLGUIRenderer code: Mark Strom <mwstrom@gmail.com>
        ImageCodec system support: Olivier Delannoy
        Static build support: Jonathan Welch (Kokoro-Sama)
        RenderTarget modifications & updates: Paul D Turner

    purpose: Interface to Renderer implemented via Opengl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _openglrenderer_h_
#define _openglrenderer_h_

#include "CEGUIBase.h"

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef OPENGL_GUIRENDERER_EXPORTS
#       define OPENGL_GUIRENDERER_API __declspec(dllexport)
#   else
#       define OPENGL_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define OPENGL_GUIRENDERER_API
#endif

#if defined(_WIN32) || defined(__WIN32__)
#   include <Windows.h>
#endif

/* XXX Hack for finding headers in Apple's OpenGL framework. */
#if defined( __APPLE__ )
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else /* __APPLE__ */
#include <GL/gl.h>
#include <GL/glu.h>
#endif /* __APPLE__ */
#include <list>
#include <set>

#include "CEGUIRenderer.h"
#include "CEGUITexture.h"

#if defined(_MSC_VER)
# pragma warning(push)
# pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Forward refs
*************************************************************************/
class OpenGLTexture;
class OGLCacheTargetFactory;

/*!
    \brief
    Renderer class to interface with OpenGL
*/
class OPENGL_GUIRENDERER_API OpenGLRenderer : public Renderer
{

public:
    /*!
    \brief
        Constructor for OpenGL Renderer object

    \param codec
        A pointer to a user provided image codec. The renderer does not take
        ownership of the codec object.
    */
    OpenGLRenderer(ImageCodec* codec = 0);

    /*!
    \brief
        Constructor for OpenGL Renderer object

    \param area
        Rect specifying the area to use for the CEGUI primary render target.

    \param codec
        A pointer to a user provided image codec. The renderer does not take
        ownership of the codec object.
    */
    OpenGLRenderer(const Rect& area, ImageCodec* codec  = 0);

    /*!
    \brief
        Destructor for OpenGLRenderer objects
    */
    virtual ~OpenGLRenderer();

    /*!
    \brief
        Find out if a specified OpenGL extension is available.
    */
    static bool isExtensionSupported(const std::string& ext);

    /*!
    \brief
        Check if the extension \a ext is listed in the extensions string
        \a extensions.
    */
    static bool isExtensionInString(const std::string& ext,
                                    const std::string& extensions);

    /*!
    \brief
        Utility function to return the next power of two value.  If \a size is
        not a power of two, return the next power of two up from \a size.
    */
    static uint getNextPowerOfTwo(uint size);

    /*!
    \brief
        Return a pointer to the named OpenGL function, or 0 if such
        a function is not available.
    */
    static void* getGLProcAddr(const std::string& funcion_name);

    // implementation of required interface
    virtual RenderTarget* getPrimaryRenderTarget() const;
    virtual RenderTarget* createCachingRenderTarget();
    virtual void destroyCachingRenderTarget(RenderTarget* target);
    virtual void doRender();
    virtual void clearRenderList();
    virtual Texture* createTexture();
    virtual Texture* createTexture(const String& filename, const String& resourceGroup);
    virtual Texture* createTexture(float size);
    virtual void destroyTexture(Texture* texture);
    virtual void destroyAllTextures();
    virtual uint getMaxTextureSize() const  {return d_maxTextureSize;}
    virtual uint getHorzScreenDPI() const {return 96;}
    virtual uint getVertScreenDPI() const {return 96;}

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

    \return
        Nothing.
    */
    void setDisplaySize(const Size& sz);

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
        created by 'grabTextures'.
    */
    void restoreTextures();

    /*!
    \brief
        Retrieve the image codec used internaly.
    */
    ImageCodec& getImageCodec();

    /*!
    \brief
        Set the image codec to use for loading textures.
    */
    void setImageCodec(const String& codecName);

    /*!
    \brief
        Set the image codec to use from an existing image codec.

        In this case the renderer does not take the ownership of the image codec
        object.

    \param codec
        a pointer to an image codec object.
    */
    void setImageCodec(ImageCodec* codec);

    /*!
    \brief
        Set the name of the default image codec to be used
    */
    static void setDefaultImageCodecName(const String& codecName);

    /*!
    \brief
        Get the name of the default image codec
    */
    static const String& getDefaultImageCodecName();

    /*!
    \brief
        convert colour value to whatever the OpenGL system is expecting.
    */
    static uint32 colourToOGL(const colour& col);

private:
    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    // set the module ID string
    void setModuleIdentifierString();

    // setup image codec
    void setupImageCodec(const String& codecName);

    // cleanup image codec
    void cleanupImageCodec();

    // initialise the cache factory pointer according to capabilities.
    void initialiseCacheFactory();

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    //! List used to track textures.
    std::list<OpenGLTexture*> d_texturelist;
    //! Holds maximum supported texture size (in pixels).
    GLint d_maxTextureSize;
    //! Holds a pointer to the image codec to use.
    ImageCodec* d_imageCodec;
    /*! Holds a pointer to the image codec module. If d_imageCodecModule is 0 we
    are not owner of the image codec object. */
    DynamicModule* d_imageCodecModule;
    //! Holds the name of the default image codec to use.
    static String d_defaultImageCodecName;
    //! Holds pointer to the primary RenderTarget object.
    RenderTarget* d_primaryTarget;
    //! Pointer to a factory that creates RenderTargets that can act as a cache.
    OGLCacheTargetFactory* d_cacheFactory;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
# pragma warning(pop)
#endif

#endif // end of guard _openglrenderer_h_
