/***********************************************************************
    filename:  openglrenderer.cpp
    created: 9/4/2004
    authors:
        Original OpenGLGUIRenderer code: Mark Strom <mwstrom@gmail.com>
        ImageCodec system support: Olivier Delannoy
        Static build support: Jonathan Welch (Kokoro-Sama)
        RenderTarget modifications & updates: Paul D Turner

    purpose: Interface to Renderer implemented via Opengl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "CEGUIExceptions.h"
#include "CEGUIEventArgs.h"
#include "CEGUIImageCodec.h"
#include "CEGUIDynamicModule.h"
#include "CEGUIcolour.h"

#if defined(__linux__)
#   define GLX_GLXEXT_PROTOTYPES
#   include <GL/glx.h>
#   include "CEGUIOpenGLGLXPBTextureTarget.h"
#elif defined(_WIN32) || defined(__WIN32__)
#   include "CEGUIOpenGLWGLPBTextureTarget.h"
#endif

#include "openglrenderer.h"
#include "opengltexture.h"
#include "CEGUIOpenGLViewportTarget.h"
#include "CEGUIOpenGLFBOTextureTarget.h"

//Include the default codec for static builds
#if defined(CEGUI_STATIC)
# if defined(CEGUI_CODEC_SILLY)
#  include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
# elif defined(CEGUI_CODEC_TGA)
#  include "../../ImageCodecModules/TGAImageCodec/CEGUITGAImageCodecModule.h"
# elif defined(CEGUI_CODEC_CORONA)
#  include "../../ImageCodecModules/CoronaImageCodec/CEGUICoronaImageCodecModule.h"
# elif defined(CEGUI_CODEC_DEVIL)
#  include "../../ImageCodecModules/DevILImageCodec/CEGUIDevILImageCodecModule.h"
# elif defined(CEGUI_CODEC_FREEIMAGE)
#  include "../../ImageCodecModules/FreeImageImageCodec/CEGUIFreeImageImageCodecModule.h"
# else //Make Silly the default
#  include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
# endif
#endif


#define S_(X) #X
#define STRINGIZE(X) S_(X)


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String OpenGLRenderer::d_defaultImageCodecName(STRINGIZE(CEGUI_DEFAULT_IMAGE_CODEC));

//----------------------------------------------------------------------------//
//
// Here we have an internal class that allows us to implement a factory template
// for creating / destroying any type of RenderTarget.  The code that detects
// the computer's abilities will generate an appropriate factory for a
// RenderTarget that does caching - or use the default 'null' factory if no
// suitable RenderTargets are available.
//
// base factory class - mainly used as a polymorphic interface
class OGLCacheTargetFactory
{
public:
    OGLCacheTargetFactory() {}
    virtual ~OGLCacheTargetFactory() {}
    virtual RenderTarget* create() const
        { return 0; }
    virtual void destory(RenderTarget* target) const
        { delete target; }
};

// template specialised class - does the real work
template<typename T>
class OGLTemplateCacheFactory : public OGLCacheTargetFactory
{
    virtual RenderTarget* create() const
        { return new T; }
};

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(ImageCodec*  codec) :
        d_imageCodec(codec),
        d_imageCodecModule(0),
        d_cacheFactory(0)
{
    // create main view port target for the renderer
    d_primaryTarget = new OpenGLViewportTarget;

    // create factory that can generate RenderTargets that cache
    initialiseCacheFactory();

    if (!d_imageCodec)
        setupImageCodec("");

    setModuleIdentifierString();

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(const Rect& area, ImageCodec* codec) :
        d_imageCodec(codec),
        d_imageCodecModule(0)
{
    // create main view port target for the renderer
    d_primaryTarget = new OpenGLViewportTarget(area);

    // create factory that can generate RenderTargets that cache
    initialiseCacheFactory();

    if (!d_imageCodec)
        setupImageCodec("");

    setModuleIdentifierString();

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);
}

//----------------------------------------------------------------------------//
OpenGLRenderer::~OpenGLRenderer()
{
    destroyAllTextures();
    cleanupImageCodec();
    delete d_primaryTarget;
    delete d_cacheFactory;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::doRender()
{
    d_primaryTarget->execute();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::clearRenderList()
{
    d_primaryTarget->clearRenderList();
}

//----------------------------------------------------------------------------//
Texture* OpenGLRenderer::createTexture()
{
    OpenGLTexture* tex = new OpenGLTexture(this);
    d_texturelist.push_back(tex);
    return tex;
}

//----------------------------------------------------------------------------//
Texture* OpenGLRenderer::createTexture(const String& filename,
    const String& resourceGroup)
{
    OpenGLTexture* tex = new OpenGLTexture(this);
    try
    {
        tex->loadFromFile(filename, resourceGroup);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_texturelist.push_back(tex);
    return tex;
}

//----------------------------------------------------------------------------//
Texture* OpenGLRenderer::createTexture(float size)
{
    OpenGLTexture* tex = new OpenGLTexture(this);
    try
    {
        tex->setOGLTextureSize((uint)size);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_texturelist.push_back(tex);
    return tex;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyTexture(Texture* texture)
{
    if (texture)
    {
        OpenGLTexture* tex = (OpenGLTexture*)texture;
        d_texturelist.remove(tex);
        delete tex;
    }

}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyAllTextures()
{
    while (!d_texturelist.empty())
    {
        destroyTexture(*(d_texturelist.begin()));
    }
}

//----------------------------------------------------------------------------//
uint32 OpenGLRenderer::colourToOGL(const colour& col)
{
    const argb_t c = col.getARGB();

    // OpenGL wants RGBA
#ifdef __BIG_ENDIAN__
    uint32 cval = (c << 8) | (c >> 24);
#else
    uint32 cval = ((c & 0xFF0000) >> 16) |
                   (c & 0xFF00) |
                   ((c & 0xFF) << 16) |
                   (c & 0xFF000000);
#endif
    return cval;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setDisplaySize(const Size& sz)
{
    EventArgs args;
    fireEvent(EventDisplaySizeChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setModuleIdentifierString()
{
    // set ID string
    d_identifierString = "CEGUI::OpenGLRenderer - "
                         "Official OpenGL based renderer module for CEGUI";
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::grabTextures()
{
    typedef std::list<OpenGLTexture*> texlist;
    texlist::iterator i = d_texturelist.begin();
    while (i != d_texturelist.end())
    {
        (*i)->grabTexture();
        i++;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::restoreTextures()
{
    typedef std::list<OpenGLTexture*> texlist;
    texlist::iterator i = d_texturelist.begin();
    while (i != d_texturelist.end())
    {
        (*i)->restoreTexture();
        i++;
    }
}

//----------------------------------------------------------------------------//
ImageCodec& OpenGLRenderer::getImageCodec()
{
    return *d_imageCodec;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setImageCodec(const String& codecName)
{
    setupImageCodec(codecName);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setImageCodec(ImageCodec* codec)
{
    if (codec)
    {
        cleanupImageCodec();
        d_imageCodec = codec;
        d_imageCodecModule = 0;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setupImageCodec(const String& codecName)
{

    // Cleanup the old image codec
    if (d_imageCodec)
        cleanupImageCodec();

    // Test whether we should use the default codec or not
    if (codecName.empty())
        //If we are statically linking the default codec will already be in the system
#if defined(CEGUI_STATIC)
        d_imageCodecModule = 0;
#else
        d_imageCodecModule =
            new DynamicModule(String("CEGUI") + d_defaultImageCodecName);
#endif
    else
        d_imageCodecModule = new DynamicModule(String("CEGUI") + codecName);

    //Check to make sure we have a module...
    if (d_imageCodecModule)
    {
        // Create the codec object itself
        ImageCodec*(*createFunc)(void) =
            (ImageCodec * (*)(void))d_imageCodecModule->
                getSymbolAddress("createImageCodec");
        d_imageCodec = createFunc();
    } // if(d_imageCodecModule)
    else
    {
#if defined(CEGUI_STATIC)
        d_imageCodec = createImageCodec();
#else
        throw InvalidRequestException("Unable to load codec " + codecName);
#endif
    }

}

//----------------------------------------------------------------------------//
void OpenGLRenderer::cleanupImageCodec()
{
    if (d_imageCodec && d_imageCodecModule)
    {
        void(*deleteFunc)(ImageCodec*) =
            (void(*)(ImageCodec*))d_imageCodecModule->
                getSymbolAddress("destroyImageCodec");
        deleteFunc(d_imageCodec);
        d_imageCodec = 0;
        delete d_imageCodecModule;
        d_imageCodecModule = 0;
    } // if (d_imageCodec && d_imageCodecModule)
    else
    {
#if defined(CEGUI_STATIC)
        destroyImageCodec(d_imageCodec);
#endif
    }

}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setDefaultImageCodecName(const String& codecName)
{
    d_defaultImageCodecName = codecName;
}

//----------------------------------------------------------------------------//
const String& OpenGLRenderer::getDefaultImageCodecName()
{
    return d_defaultImageCodecName;

}

//----------------------------------------------------------------------------//
RenderTarget* OpenGLRenderer::getPrimaryRenderTarget() const
{
    return d_primaryTarget;
}

//----------------------------------------------------------------------------//
RenderTarget* OpenGLRenderer::createCachingRenderTarget()
{
    if (d_cacheFactory)
        return d_cacheFactory->create();

    return 0;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyCachingRenderTarget(RenderTarget* target)
{
    if (d_cacheFactory)
        d_cacheFactory->destory(target);
    else
        delete target;
}

//----------------------------------------------------------------------------//
bool OpenGLRenderer::isExtensionSupported(const std::string& ext)
{
    // get extensions string.
    const std::string glextns(
        reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));

    return isExtensionInString(ext, glextns);
}

//----------------------------------------------------------------------------//
bool OpenGLRenderer::isExtensionInString(const std::string& ext,
    const std::string& extensions)
{
    using namespace std;
    // search for the extension string we want.
    for (size_t hpos, cpos = 0;
         string::npos != (hpos = extensions.find(ext, cpos));
         cpos = hpos + ext.length())
    {
        // ensure this 'hit' is not a substring of some other extension
        if (hpos == 0 || extensions[hpos - 1] == ' ')
        {
            const char lastchar = extensions[hpos + ext.length()];
            if (lastchar == ' ' || lastchar == '\0')
                return true;
        }
    }

    return false;
}

//----------------------------------------------------------------------------//
uint OpenGLRenderer::getNextPowerOfTwo(uint size)
{
    // if not power of 2
    if ((size & (size - 1)) || !size)
    {
        int log = 0;

        // get integer log of 'size' to base 2
        while (size >>= 1)
            ++log;

        // use log to calculate value to use as size.
        size = (2 << log);
    }

    return size;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::initialiseCacheFactory()
{
    // prefer FBO
    if (isExtensionSupported("GL_EXT_framebuffer_object"))
    {
        d_cacheFactory = new OGLTemplateCacheFactory<OpenGLFBOTextureTarget>;
        return;
    }
#if defined(__linux__)
    // if on linux, we can try for GLX pbuffer support
    int glx_maj, glx_min;
    Display* dpy = glXGetCurrentDisplay();
    if (glXQueryExtension(dpy, 0, 0))
        if (glXQueryVersion(dpy, &glx_maj, &glx_min))
            if (((glx_maj == 1) && (glx_min >= 3)) || (glx_maj > 1))
            {
                d_cacheFactory =
                    new OGLTemplateCacheFactory<OpenGLGLXPBTextureTarget>;
                return;
            }
#elif defined(_WIN32) || defined(__WIN32__)
    PFNWGLGETEXTENSIONSSTRINGARBPROC
        wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)
            wglGetProcAddress("wglGetExtensionsStringARB");

    if (wglGetExtensionsStringARB)
    {
        std::string wglexts = wglGetExtensionsStringARB(wglGetCurrentDC());

        if (OpenGLRenderer::isExtensionInString("WGL_ARB_pbuffer", wglexts))
        {
            d_cacheFactory =
                new OGLTemplateCacheFactory<OpenGLWGLPBTextureTarget>;

            return;
        }
    }
#endif

    // oh well, we tried.  Just carry on without caching ability :(
    d_cacheFactory = new OGLCacheTargetFactory;
}

//----------------------------------------------------------------------------//
void* OpenGLRenderer::getGLProcAddr(const std::string& funcion_name)
{
    #if defined(_WIN32) || defined(__WIN32__)
        return (void*)wglGetProcAddress(funcion_name.c_str());
    #elif defined(__linux__)
        return (void*)glXGetProcAddressARB((const GLubyte*)funcion_name.c_str());
    #elif defined(__APPLE__)
        // FIXME: I think we need a solution similar to what is detailed here:
        // FIXME: http://developer.apple.com/qa/qa2001/qa1188.html
        return 0;
    #endif

}

} // End of  CEGUI namespace section
