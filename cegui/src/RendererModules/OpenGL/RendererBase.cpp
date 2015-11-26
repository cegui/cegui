/***********************************************************************
    created:    Tue Apr 30 2013
    authors:    Paul D Turner <paul@cegui.org.uk>
                Lukas E Meindl
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/RendererModules/OpenGL/TextureTarget.h"
#include "CEGUI/RendererModules/OpenGL/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGL/GeometryBufferBase.h"
#include "CEGUI/RendererModules/OpenGL/GlmPimpl.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/System.h"
#include "CEGUI/Logger.h"

#include <sstream>
#include <algorithm>

namespace CEGUI
{
//----------------------------------------------------------------------------//
String OpenGLRendererBase::d_rendererID("--- subclass did not set ID: Fix this!");

//----------------------------------------------------------------------------//
OpenGLRendererBase::OpenGLRendererBase() :
    d_viewProjectionMatrix(0),
    d_activeRenderTarget(0)
{
    init();
    initialiseDisplaySizeWithViewportSize();
    d_defaultTarget = CEGUI_NEW_AO OpenGLViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLRendererBase::OpenGLRendererBase(const Sizef& display_size) :
    d_displaySize(display_size),
    d_viewProjectionMatrix(0),
    d_activeRenderTarget(0)
{
    init();
    d_defaultTarget = CEGUI_NEW_AO OpenGLViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLRendererBase::OpenGLRendererBase(bool set_glew_experimental) :
    d_viewProjectionMatrix(0),
    d_activeRenderTarget(0)
{
    init(true, set_glew_experimental);
    initialiseDisplaySizeWithViewportSize();
    d_defaultTarget = CEGUI_NEW_AO OpenGLViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLRendererBase::OpenGLRendererBase(const Sizef& display_size,
                                       bool set_glew_experimental) :
    d_displaySize(display_size),
    d_viewProjectionMatrix(0),
    d_activeRenderTarget(0)
{
    init(true, set_glew_experimental);
    d_defaultTarget = CEGUI_NEW_AO OpenGLViewportTarget(*this);
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::init(bool init_glew, bool set_glew_experimental)
{
    d_displayDPI.d_x = d_displayDPI.d_y = 96;
    d_initExtraStates = false;
    d_activeBlendMode = BM_INVALID;
    d_viewProjectionMatrix = new mat4Pimpl();
#if defined CEGUI_USE_GLEW
    if (init_glew)
    {
        if (set_glew_experimental)
            glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if(err != GLEW_OK)
        {
            std::ostringstream err_string;
            //Problem: glewInit failed, something is seriously wrong.
            err_string << "failed to initialise the GLEW library. "
                << glewGetErrorString(err);

            CEGUI_THROW(RendererException(err_string.str().c_str()));
        }
        //Clear the useless error glew produces as of version 1.7.0, when using OGL3.2 Core Profile
        glGetError();
    }
#else
    CEGUI_UNUSED(init_glew);
    CEGUI_UNUSED(set_glew_experimental);
#endif
    OpenGLInfo::getSingleton().init();
    initialiseMaxTextureSize();
}

//----------------------------------------------------------------------------//
OpenGLRendererBase::~OpenGLRendererBase()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    CEGUI_DELETE_AO d_defaultTarget;
    delete d_viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::initialiseDisplaySizeWithViewportSize()
{
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    d_displaySize = Sizef(static_cast<float>(vp[2]),
                          static_cast<float>(vp[3]));
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::initialiseMaxTextureSize()
{
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;
}

//----------------------------------------------------------------------------//
RenderTarget& OpenGLRendererBase::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OpenGLRendererBase::createGeometryBuffer()
{
    OpenGLGeometryBufferBase* b = createGeometryBuffer_impl();
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator i = std::find(d_geometryBuffers.begin(),
                                               d_geometryBuffers.end(),
                                               &buffer);

    if (d_geometryBuffers.end() != i)
    {
        d_geometryBuffers.erase(i);
        CEGUI_DELETE_AO &buffer;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGLRendererBase::createTextureTarget()
{
    TextureTarget* t = createTextureTarget_impl();

    if (t)
        d_textureTargets.push_back(t);

    return t;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                              d_textureTargets.end(),
                                              target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        CEGUI_DELETE_AO target;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& OpenGLRendererBase::createTexture(const String& name)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* tex = CEGUI_NEW_AO OpenGLTexture(*this, name);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRendererBase::createTexture(const String& name,
                                       const String& filename,
                                       const String& resourceGroup)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* tex = CEGUI_NEW_AO OpenGLTexture(*this, name, filename, resourceGroup);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRendererBase::createTexture(const String& name, const Sizef& size)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* tex = CEGUI_NEW_AO OpenGLTexture(*this, name, size);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGLRenderer] Created texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyTexture(const String& name)
{
    TextureMap::iterator i = d_textures.find(name);

    if (d_textures.end() != i)
    {
        logTextureDestruction(name);
        CEGUI_DELETE_AO i->second;
        d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGLRenderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& OpenGLRendererBase::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException(
            "No texture named '" + name + "' is available."));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool OpenGLRendererBase::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
}

//----------------------------------------------------------------------------//
const Sizef& OpenGLRendererBase::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGLRendererBase::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint OpenGLRendererBase::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& OpenGLRendererBase::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Texture& OpenGLRendererBase::createTexture(const String& name, GLuint tex,
                                       const Sizef& sz)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    OpenGLTexture* t = CEGUI_NEW_AO OpenGLTexture(*this, name, tex, sz);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::setDisplaySize(const Sizef& sz)
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
void OpenGLRendererBase::enableExtraStateSettings(bool setting)
{
    d_initExtraStates = setting;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::grabTextures()
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
void OpenGLRendererBase::restoreTextures()
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
Sizef OpenGLRendererBase::getAdjustedTextureSize(const Sizef& sz) const
{
    Sizef out(sz);

    // if we can't support non power of two sizes, get appropriate POT values.
    if (!OpenGLInfo::getSingleton().isNpotTextureSupported())
    {
        out.d_width = getNextPOTSize(out.d_width);
        out.d_height = getNextPOTSize(out.d_height);
    }

    return out;
}

//----------------------------------------------------------------------------//
float OpenGLRendererBase::getNextPOTSize(const float f)
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
const mat4Pimpl* OpenGLRendererBase::getViewProjectionMatrix()
{
    return d_viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::setViewProjectionMatrix(const mat4Pimpl* viewProjectionMatrix)
{
    *d_viewProjectionMatrix = *viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
const CEGUI::Rectf& OpenGLRendererBase::getActiveViewPort()
{
    return d_activeRenderTarget->getArea();
}

//----------------------------------------------------------------------------//
void OpenGLRendererBase::setActiveRenderTarget(RenderTarget* renderTarget)
{
    d_activeRenderTarget = renderTarget;
}

//----------------------------------------------------------------------------//
RenderTarget* OpenGLRendererBase::getActiveRenderTarget()
{
    return d_activeRenderTarget;
}

//----------------------------------------------------------------------------//

}

