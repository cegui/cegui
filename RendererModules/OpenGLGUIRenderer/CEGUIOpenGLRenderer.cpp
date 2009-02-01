/***********************************************************************
    filename:   CEGUIOpenGLRenderer.cpp
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include <GL/glew.h>

#include "CEGUIOpenGLRenderer.h"
#include "CEGUIOpenGLTexture.h"
#include "CEGUIExceptions.h"
#include "CEGUIEventArgs.h"
#include "CEGUIImageCodec.h"
#include "CEGUIDynamicModule.h"
#include "CEGUIOpenGLViewportTarget.h"
#include "CEGUIOpenGLGeometryBuffer.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIOpenGLFBOTextureTarget.h"

#include <sstream>
#include <algorithm>

#if defined(__linux__)
#   include "CEGUIOpenGLGLXPBTextureTarget.h"
#elif defined(_WIN32) || defined(__WIN32__)
#   include "CEGUIOpenGLWGLPBTextureTarget.h"
#elif defined(__APPLE__)
#   include "CEGUIOpenGLApplePBTextureTarget.h"
#endif


//Include the default codec for static builds
#if defined(CEGUI_STATIC)
#	if defined(CEGUI_CODEC_SILLY)
#		include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
#	elif defined(CEGUI_CODEC_TGA)
#		include "../../ImageCodecModules/TGAImageCodec/CEGUITGAImageCodecModule.h"
#	elif defined(CEGUI_CODEC_CORONA)
#		include "../../ImageCodecModules/CoronaImageCodec/CEGUICoronaImageCodecModule.h"
#	elif defined(CEGUI_CODEC_DEVIL)
#		include "../../ImageCodecModules/DevILImageCodec/CEGUIDevILImageCodecModule.h"
#	elif defined(CEGUI_CODEC_FREEIMAGE)
#		include "../../ImageCodecModules/FreeImageImageCodec/CEGUIFreeImageImageCodecModule.h"
#	else //Make Silly the default
#		include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
#	endif
#endif


#define S_(X) #X
#define STRINGIZE(X) S_(X)


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// The following are some GL extension / version dependant related items.
// This is all done totally internally here; no need for external interface
// to show any of this.
//----------------------------------------------------------------------------//
// we only really need this with MSVC / Windows(?) and by now it should already
// be defined on that platform, so we just define it as empty macro so the
// compile does not break on other systems.
#ifndef APIENTRY
#   define APIENTRY
#endif
//! Function to perform extentsions initialisation.
void initialiseGLExtensions();
//! Pointer to a function to use as glActiveTexture
PFNGLACTIVETEXTUREPROC CEGUI_activeTexture;
//! Pointer to a function to use as glClientActiveTexture
PFNGLCLIENTACTIVETEXTUREPROC CEGUI_clientActiveTexture;
//! Dummy function for if real ones are not present (saves testing each render)
void APIENTRY activeTextureDummy(GLenum) {}

//----------------------------------------------------------------------------//
//
// Here we have an internal class that allows us to implement a factory template
// for creating / destroying any type of TextureTarget.  The code that detects
// the computer's abilities will generate an appropriate factory for a
// TextureTarget based on what the host system can provide - or use the default
// 'null' factory if no suitable TextureTargets are available.
//
// base factory class - mainly used as a polymorphic interface
class OGLTextureTargetFactory
{
public:
    OGLTextureTargetFactory() {}
    virtual ~OGLTextureTargetFactory() {}
    virtual TextureTarget* create(OpenGLRenderer& r) const
        { return 0; }
    virtual void destory(TextureTarget* target) const
        { delete target; }
};

// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGLTextureTargetFactory
{
    virtual TextureTarget* create(OpenGLRenderer& r) const
        { return new T(r); }
};

//----------------------------------------------------------------------------//
String OpenGLRenderer::d_defaultImageCodecName(STRINGIZE(CEGUI_DEFAULT_IMAGE_CODEC));

//----------------------------------------------------------------------------//
String OpenGLRenderer::d_rendererID(
"CEGUI::OpenGLRenderer - Official OpenGL based 2nd generation renderer module.");

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::create(ImageCodec* codec)
{
    return *new OpenGLRenderer(codec);
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::create(const Size& display_size,
    ImageCodec* codec)
{
    return *new OpenGLRenderer(display_size, codec);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroy(OpenGLRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(ImageCodec* codec) :
    d_displayDPI(96, 96),
    d_initExtraStates(false),
    d_imageCodec(codec),
    d_imageCodecModule(0)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    // initialise display size
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    d_displaySize = Size(static_cast<float>(vp[2]), static_cast<float>(vp[3]));

    if (!d_imageCodec)
        setupImageCodec("");

    initialiseGLExtensions();
    initialiseTextureTargetFactory();

    d_defaultTarget = new OpenGLViewportTarget(*this);
    d_defaultRoot = new RenderingRoot(*d_defaultTarget);
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(const Size& display_size, ImageCodec* codec) :
    d_displaySize(display_size),
    d_displayDPI(96, 96),
    d_initExtraStates(false),
    d_imageCodec(codec),
    d_imageCodecModule(0)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    if (!d_imageCodec)
        setupImageCodec("");

    initialiseGLExtensions();
    initialiseTextureTargetFactory();

    d_defaultTarget = new OpenGLViewportTarget(*this);
    d_defaultRoot = new RenderingRoot(*d_defaultTarget);
}

//----------------------------------------------------------------------------//
OpenGLRenderer::~OpenGLRenderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextures();
    cleanupImageCodec();

    delete d_defaultRoot;
    delete d_defaultTarget;
    delete d_textureTargetFactory;
}

//----------------------------------------------------------------------------//
RenderingRoot& OpenGLRenderer::getDefaultRenderingRoot()
{
    return *d_defaultRoot;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OpenGLRenderer::createGeometryBuffer()
{
    OpenGLGeometryBuffer* b= new OpenGLGeometryBuffer;
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator i = std::find(d_geometryBuffers.begin(),
                                               d_geometryBuffers.end(),
                                               &buffer);

    if (d_geometryBuffers.end() != i)
    {
        d_geometryBuffers.erase(i);
        delete &buffer;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGLRenderer::createTextureTarget()
{
    TextureTarget* t = d_textureTargetFactory->create(*this);
    d_textureTargets.push_back(t);
    return t;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                              d_textureTargets.end(),
                                              target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        d_textureTargetFactory->destory(target);
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::createTexture()
{
    OpenGLTexture* tex = new OpenGLTexture(this);
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::createTexture(const String& filename,
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
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::createTexture(const Size& size)
{
    OpenGLTexture* tex = new OpenGLTexture(this);
    try
    {
        tex->setTextureSize(size);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_textures.push_back(tex);
    return *tex;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyTexture(Texture& texture)
{
    TextureList::iterator i = std::find(d_textures.begin(),
                                        d_textures.end(),
                                        &texture);

    if (d_textures.end() != i)
    {
        d_textures.erase(i);
        delete &static_cast<OpenGLTexture&>(texture);
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(**d_textures.begin());
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::beginRendering()
{
    //save current attributes
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    // save current matrices
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // do required set-up.  yes, it really is this minimal ;)
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_ALWAYS);

    // if enabled, restores a subset of the GL state back to default values.
    if (d_initExtraStates)
        setupExtraStates();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::endRendering()
{
    if (d_initExtraStates)
        cleanupExtraStates();

    // restore former matrices
    // FIXME: If the push ops failed, the following could mess things up!
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    //restore former attributes
    glPopAttrib();
    glPopClientAttrib();
}

//----------------------------------------------------------------------------//
const Size& OpenGLRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2& OpenGLRenderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint OpenGLRenderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& OpenGLRenderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::createTexture(GLuint tex, const Size& sz)
{
    OpenGLTexture* t = new OpenGLTexture(this, tex, sz);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setDisplaySize(const Size& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // update the default target's area
        Rect area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);

        EventArgs args;
        fireEvent(EventDisplaySizeChanged, args, EventNamespace);
    }

}

//----------------------------------------------------------------------------//
void OpenGLRenderer::enableExtraStateSettings(bool setting)
{
    d_initExtraStates = setting;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setupExtraStates()
{
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();

    CEGUI_activeTexture(GL_TEXTURE0);
    CEGUI_clientActiveTexture(GL_TEXTURE0);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::cleanupExtraStates()
{
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::grabTextures()
{
    TextureList::iterator i = d_textures.begin();
    while (i != d_textures.end())
    {
        (*i)->grabTexture();
        i++;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::restoreTextures()
{
    TextureList::iterator i = d_textures.begin();
    while (i != d_textures.end())
    {
        (*i)->restoreTexture();
        i++;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::initialiseTextureTargetFactory()
{
    // prefer FBO
    if (GLEW_EXT_framebuffer_object)
    {
        d_rendererID += "  TextureTarget support enabled via FBO extension.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLFBOTextureTarget>;
    }

#if defined(__linux__)
    // on linux, we can try for GLX pbuffer support
    else if (GLXEW_VERSION_1_3)
    {
        d_rendererID += "  TextureTarget support enabled via GLX pbuffers.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLGLXPBTextureTarget>;
    }
#elif defined(_WIN32) || defined(__WIN32__)
    // on Windows, we can try for WGL based pbuffer support
    else if (WGLEW_ARB_pbuffer)
    {
        d_rendererID += "  TextureTarget support enabled via WGL_ARB_pbuffer.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLWGLPBTextureTarget>;
    }
#elif defined(__APPLE__)
    // on Apple Mac, we can try for Apple's pbuffer support
    else if (GLEW_APPLE_pixel_buffer)
    {
        d_rendererID += "  TextureTarget support enabled via "
                        "GL_APPLE_pixel_buffer.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLApplePBTextureTarget>;
    }
#endif
    // Nothing suitable available, try to carry on without TextureTargets
    else
    {
        d_rendererID += "  TextureTarget support is not available :(";
        d_textureTargetFactory = new OGLTextureTargetFactory;
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
        d_imageCodecModule = new DynamicModule(String("CEGUI") + d_defaultImageCodecName);
#endif
    else
        d_imageCodecModule = new DynamicModule(String("CEGUI") + codecName);

    //Check to make sure we have a module...
    if (d_imageCodecModule)
    {
        // Create the codec object itself
        ImageCodec*(*createFunc)(void) =
            (ImageCodec * (*)(void))d_imageCodecModule->getSymbolAddress("createImageCodec");
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
            (void(*)(ImageCodec*))d_imageCodecModule->getSymbolAddress("destroyImageCodec");
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
void initialiseGLExtensions()
{
    // initialise GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::ostringstream err_string;
        err_string << "OpenGLRenderer failed to initialise the GLEW library. "
        << glewGetErrorString(err);

        throw RendererException(err_string.str());
    }

    // GL 1.3 has multi-texture support natively
    if (GLEW_VERSION_1_3)
    {
        CEGUI_activeTexture = glActiveTexture;
        CEGUI_clientActiveTexture = glClientActiveTexture;
    }
    // Maybe there is the ARB_multitexture extension version?
    else if (GLEW_ARB_multitexture)
    {
        CEGUI_activeTexture = glActiveTextureARB;
        CEGUI_clientActiveTexture = glClientActiveTextureARB;
    }
    // assign dummy functions if no multitexture possibilities
    else
    {
        CEGUI_activeTexture = activeTextureDummy;
        CEGUI_clientActiveTexture = activeTextureDummy;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

