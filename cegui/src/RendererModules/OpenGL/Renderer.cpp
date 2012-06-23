/***********************************************************************
    filename:   CEGUIOpenGLRenderer.cpp
    created:    Sun Jan 11 2009
    author:     Paul D Turner
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
#include <GL/glew.h>

#include "CEGUI/RendererModules/OpenGL/Renderer.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/RendererModules/OpenGL/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGL/GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/FBOTextureTarget.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"

#include <sstream>
#include <algorithm>

#if defined(__linux__)  || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
#   include "CEGUI/RendererModules/OpenGL/GLXPBTextureTarget.h"
#elif defined(_WIN32) || defined(__WIN32__)
#   include "CEGUI/RendererModules/OpenGL/WGLPBTextureTarget.h"
#elif defined(__APPLE__)
#   include "CEGUI/RendererModules/OpenGL/ApplePBTextureTarget.h"
#endif

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
static void initialiseGLExtensions();
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
    virtual TextureTarget* create(OpenGLRenderer&) const
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
String OpenGLRenderer::d_rendererID(
"CEGUI::OpenGLRenderer - Official OpenGL based 2nd generation renderer module.");

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::bootstrapSystem(const TextureTargetType tt_type,
                                                const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGLRenderer& renderer(create(tt_type));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::bootstrapSystem(const Sizef& display_size,
                                                const TextureTargetType tt_type,
                                                const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGLRenderer& renderer(create(display_size, tt_type));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::create(const TextureTargetType tt_type,
                                       const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLRenderer(tt_type);
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::create(const Sizef& display_size,
                                       const TextureTargetType tt_type,
                                       const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLRenderer(display_size, tt_type);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroy(OpenGLRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(const TextureTargetType tt_type) :
    d_displayDPI(96, 96),
    d_initExtraStates(false),
    d_activeBlendMode(BM_INVALID)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    // initialise display size
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    d_displaySize = Sizef(static_cast<float>(vp[2]), static_cast<float>(vp[3]));

    initialiseGLExtensions();
    initialiseTextureTargetFactory(tt_type);

    // we _really_ need separate rgb/alpha blend modes, if this support is not
    // available, add a note to the renderer ID string so that this fact is
    // logged.
    if (!GLEW_VERSION_1_4 && !GLEW_EXT_blend_func_separate)
        d_rendererID += "  No glBlendFuncSeparate(EXT) support.";

    d_defaultTarget = new OpenGLViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(const Sizef& display_size,
                               const TextureTargetType tt_type) :
    d_displaySize(display_size),
    d_displayDPI(96, 96),
    d_initExtraStates(false),
    d_activeBlendMode(BM_INVALID)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    initialiseGLExtensions();
    initialiseTextureTargetFactory(tt_type);

    d_defaultTarget = new OpenGLViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLRenderer::~OpenGLRenderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_defaultTarget;
    delete d_textureTargetFactory;
}

//----------------------------------------------------------------------------//
RenderTarget& OpenGLRenderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OpenGLRenderer::createGeometryBuffer()
{
    OpenGLGeometryBuffer* b= new OpenGLGeometryBuffer(*this);
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
Texture& OpenGLRenderer::createTexture(const String& name)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* tex = new OpenGLTexture(*this, name);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::createTexture(const String& name,
                                       const String& filename,
                                       const String& resourceGroup)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* tex = new OpenGLTexture(*this, name, filename, resourceGroup);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::createTexture(const String& name, const Sizef& size)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* tex = new OpenGLTexture(*this, name, size);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGLRenderer] Created texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyTexture(const String& name)
{
    TextureMap::iterator i = d_textures.find(name);

    if (d_textures.end() != i)
    {
        logTextureDestruction(name);
        delete i->second;
        d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGLRenderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& OpenGLRenderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException(
            "No texture named '" + name + "' is available."));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool OpenGLRenderer::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
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

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BM_NORMAL, true);

    // enable arrays that we'll be using in the batches
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_FOG_COORDINATE_ARRAY);
    glDisableClientState(GL_EDGE_FLAG_ARRAY);

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
const Sizef& OpenGLRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGLRenderer::getDisplayDPI() const
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
Texture& OpenGLRenderer::createTexture(const String& name, GLuint tex,
                                       const Sizef& sz)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* t = new OpenGLTexture(*this, name, tex, sz);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setDisplaySize(const Sizef& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // update the default target's area
        Rectf area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
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
    glDisable(GL_ALPHA_TEST);

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
    // perform grab operations for texture targets
    TextureTargetList::iterator target_iterator = d_textureTargets.begin();
    for (; target_iterator != d_textureTargets.end(); ++target_iterator)
        static_cast<OpenGLTextureTarget*>(*target_iterator)->grabTexture();

    // perform grab on regular textures
    TextureMap::iterator texture_iterator = d_textures.begin();
    for (; texture_iterator != d_textures.end(); ++texture_iterator)
        texture_iterator->second->grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::restoreTextures()
{
    // perform restore on textures
    TextureMap::iterator texture_iterator = d_textures.begin();
    for (; texture_iterator != d_textures.end(); ++texture_iterator)
        texture_iterator->second->restoreTexture();

    // perform restore operations for texture targets
    TextureTargetList::iterator target_iterator = d_textureTargets.begin();
    for (; target_iterator != d_textureTargets.end(); ++target_iterator)
        static_cast<OpenGLTextureTarget*>(*target_iterator)->restoreTexture();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::initialiseTextureTargetFactory(
    const TextureTargetType tt_type)
{
    // prefer FBO

    if (((tt_type == TTT_AUTO) || (tt_type == TTT_FBO)) &&
        GLEW_EXT_framebuffer_object)
    {
        d_rendererID += "  TextureTarget support enabled via FBO extension.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLFBOTextureTarget>;
    }

#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
    // on linux (etc), we can try for GLX pbuffer support
    else if (((tt_type == TTT_AUTO) || (tt_type == TTT_PBUFFER)) &&
             GLXEW_VERSION_1_3)
    {
        d_rendererID += "  TextureTarget support enabled via GLX pbuffers.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLGLXPBTextureTarget>;
    }
#elif defined(_WIN32) || defined(__WIN32__)
    // on Windows, we can try for WGL based pbuffer support
    else if (((tt_type == TTT_AUTO) || (tt_type == TTT_PBUFFER)) &&
             WGLEW_ARB_pbuffer)
    {
        d_rendererID += "  TextureTarget support enabled via WGL_ARB_pbuffer.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLWGLPBTextureTarget>;
    }
#elif defined(__APPLE__)
    // on Apple Mac, we can try for Apple's pbuffer support
    else if (((tt_type == TTT_AUTO) || (tt_type == TTT_PBUFFER)) &&
             GLEW_APPLE_pixel_buffer)
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
Sizef OpenGLRenderer::getAdjustedTextureSize(const Sizef& sz) const
{
    Sizef out(sz);

    // if we can't support non power of two sizes, get appropriate POT values.
    if (!GLEW_ARB_texture_non_power_of_two)
    {
        out.d_width = getNextPOTSize(out.d_width);
        out.d_height = getNextPOTSize(out.d_height);
    }

    return out;
}

//----------------------------------------------------------------------------//
float OpenGLRenderer::getNextPOTSize(const float f)
{
    uint size = static_cast<uint>(f);

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

    return static_cast<float>(size);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setupRenderingBlendMode(const BlendMode mode,
                                             const bool force)
{
    // exit if mode is already set up (and update not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;

    if (d_activeBlendMode == BM_RTT_PREMULTIPLIED)
    {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        if (GLEW_VERSION_1_4)
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                GL_ONE_MINUS_DST_ALPHA, GL_ONE);
        else if (GLEW_EXT_blend_func_separate)
            glBlendFuncSeparateEXT(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                   GL_ONE_MINUS_DST_ALPHA, GL_ONE);
        else
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
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

        CEGUI_THROW(RendererException(err_string.str().c_str()));
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

