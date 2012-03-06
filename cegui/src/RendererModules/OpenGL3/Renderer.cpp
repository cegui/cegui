/***********************************************************************
    filename:   CEGUIOpenGL3Renderer.cpp
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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


#include "CEGUI/RendererModules/OpenGL3/ShaderManager.h"
#include "CEGUI/RendererModules/OpenGL3/Renderer.h"
#include "CEGUI/RendererModules/OpenGL3/Texture.h"
#include "CEGUI/RendererModules/OpenGL3/Shader.h"
#include "CEGUI/RendererModules/OpenGL3/GlmPimpl.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/RendererModules/OpenGL3/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGL3/GeometryBuffer.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/RendererModules/OpenGL3/FBOTextureTarget.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"
#include "CEGUI/RendererModules/OpenGL3/StateChangeWrapper.h"

#include <sstream>
#include <algorithm>

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
//! Function to perform extensions initialisation.
static void initialiseGLExtensions();
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
class OGL3TextureTargetFactory
{
public:
    OGL3TextureTargetFactory() {}
    virtual ~OGL3TextureTargetFactory() {}
    virtual TextureTarget* create(OpenGL3Renderer&) const
        { return 0; }
    virtual void destroy(TextureTarget* target) const
        { delete target; }
};

// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGL3TextureTargetFactory
{
    virtual TextureTarget* create(OpenGL3Renderer& r) const
        { return new T(r); }
};

//----------------------------------------------------------------------------//
String OpenGL3Renderer::d_rendererID(
"CEGUI::OpenGL3Renderer - Official OpenGL 3.2 core based renderer module.");

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::bootstrapSystem()
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException("OpenGL3Renderer::bootstrapSystem: "
            "CEGUI::System object is already initialised."));

    OpenGL3Renderer& renderer(create());
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::bootstrapSystem(const Sizef& display_size)
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException("OpenGL3Renderer::bootstrapSystem: "
            "CEGUI::System object is already initialised."));

    OpenGL3Renderer& renderer(create(display_size));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException("OpenGL3Renderer::destroySystem: "
            "CEGUI::System object is not created or was already destroyed."));

    OpenGL3Renderer* renderer = static_cast<OpenGL3Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::create()
{
    return *new OpenGL3Renderer();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::create(const Sizef& display_size)
{
    return *new OpenGL3Renderer(display_size);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroy(OpenGL3Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::OpenGL3Renderer() :
    d_displayDPI(96, 96),
    d_initExtraStates(false),
    d_activeBlendMode(BM_INVALID),
    d_shaderStandard(0),
    d_viewProjectionMatrix(0),
    d_activeRenderTarget(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
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
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();

    d_defaultTarget = new OpenGL3ViewportTarget(*this);
    d_openGLStateChanger = new OpenGL3StateChangeWrapper(*this);

    d_viewProjectionMatrix = new mat4Pimpl();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::OpenGL3Renderer(const Sizef& display_size) :
    d_displaySize(display_size),
    d_displayDPI(96, 96),
    d_initExtraStates(false),
    d_activeBlendMode(BM_INVALID),
    d_shaderStandard(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    initialiseGLExtensions();
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();


    d_defaultTarget = new OpenGL3ViewportTarget(*this);
    d_openGLStateChanger = new OpenGL3StateChangeWrapper(*this);

    d_viewProjectionMatrix = new mat4Pimpl();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::~OpenGL3Renderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_defaultTarget;
    delete d_textureTargetFactory;
    delete d_openGLStateChanger;
    delete d_shaderManager;
    delete d_viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
RenderTarget& OpenGL3Renderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OpenGL3Renderer::createGeometryBuffer()
{
    OpenGL3GeometryBuffer* b= new OpenGL3GeometryBuffer(*this);
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
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
void OpenGL3Renderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGL3Renderer::createTextureTarget()
{
    TextureTarget* t = d_textureTargetFactory->create(*this);
    d_textureTargets.push_back(t);
    return t;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                              d_textureTargets.end(),
                                              target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        d_textureTargetFactory->destroy(target);
    }
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& OpenGL3Renderer::createTexture(const String& name)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException("OpenGL3Renderer::createTexture: "
            "A texture named '" + name + "' already exists."));

    OpenGL3Texture* tex = new OpenGL3Texture(*this, name);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGL3Renderer::createTexture(const String& name,
                                       const String& filename,
                                       const String& resourceGroup)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException("OpenGL3Renderer::createTexture: "
            "A texture named '" + name + "' already exists."));

    OpenGL3Texture* tex = new OpenGL3Texture(*this, name, filename, resourceGroup);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGL3Renderer::createTexture(const String& name, const Sizef& size)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException("OpenGL3Renderer::createTexture: "
            "A texture named '" + name + "' already exists."));

    OpenGL3Texture* tex = new OpenGL3Texture(*this, name, size);
    d_textures[name] = tex;

    logTextureCreation(name);

    return *tex;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGL3Renderer] Created texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroyTexture(const String& name)
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
void OpenGL3Renderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGL3Renderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& OpenGL3Renderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException("OpenGL3Renderer::getTexture: "
            "No texture named '" + name + "' is available."));

    return *i->second;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::beginRendering()
{
    // do required set-up.  yes, it really is this minimal ;)
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BM_NORMAL, true);

    // if enabled, restores a subset of the GL state back to default values.
    if (d_initExtraStates)
        setupExtraStates();

    d_shaderStandard->bind();

    d_openGLStateChanger->reset();
}


//----------------------------------------------------------------------------//
void OpenGL3Renderer::endRendering()
{
    d_shaderStandard->unbind();
}

//----------------------------------------------------------------------------//
const Sizef& OpenGL3Renderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGL3Renderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint OpenGL3Renderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& OpenGL3Renderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Texture& OpenGL3Renderer::createTexture(const String& name, GLuint tex,
                                       const Sizef& sz)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException("OpenGL3Renderer::createTexture: "
            "A texture named '" + name + "' already exists."));

    OpenGL3Texture* t = new OpenGL3Texture(*this, name, tex, sz);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::setDisplaySize(const Sizef& sz)
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
void OpenGL3Renderer::enableExtraStateSettings(bool setting)
{
    d_initExtraStates = setting;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::setupExtraStates()
{
    glActiveTexture(GL_TEXTURE0);

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::grabTextures()
{
    // perform grab operations for texture targets
    TextureTargetList::iterator target_iterator = d_textureTargets.begin();
    for (; target_iterator != d_textureTargets.end(); ++target_iterator)
        static_cast<OpenGL3TextureTarget*>(*target_iterator)->grabTexture();

    // perform grab on regular textures
    TextureMap::iterator texture_iterator = d_textures.begin();
    for (; texture_iterator != d_textures.end(); ++texture_iterator)
        texture_iterator->second->grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::restoreTextures()
{
    // perform restore on textures
    TextureMap::iterator texture_iterator = d_textures.begin();
    for (; texture_iterator != d_textures.end(); ++texture_iterator)
        texture_iterator->second->restoreTexture();

    // perform restore operations for texture targets
    TextureTargetList::iterator target_iterator = d_textureTargets.begin();
    for (; target_iterator != d_textureTargets.end(); ++target_iterator)
        static_cast<OpenGL3TextureTarget*>(*target_iterator)->restoreTexture();
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseTextureTargetFactory()
{
    //Use OGL core implementation for FBOs
    d_rendererID += "  TextureTarget support enabled via FBO OGL 3.2 core implementation.";
    d_textureTargetFactory = new OGLTemplateTargetFactory<OpenGL3FBOTextureTarget>;
}

//----------------------------------------------------------------------------//
Sizef OpenGL3Renderer::getAdjustedTextureSize(const Sizef& sz) const
{
    Sizef out(sz);

    return out;
}

//----------------------------------------------------------------------------//
float OpenGL3Renderer::getNextPOTSize(const float f)
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
void OpenGL3Renderer::setupRenderingBlendMode(const BlendMode mode,
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
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    }
}


//----------------------------------------------------------------------------//
OpenGL3Shader*& OpenGL3Renderer::getShaderStandard()
{
    return d_shaderStandard;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardPositionLoc()
{
    return d_shaderStandardPosLoc;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardTexCoordLoc()
{
    return d_shaderStandardTexCoordLoc;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardColourLoc()
{
    return d_shaderStandardColourLoc;
}


//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardMatrixUniformLoc()
{
    return d_shaderStandardMatrixLoc;
}

//----------------------------------------------------------------------------//
const mat4Pimpl* OpenGL3Renderer::getViewProjectionMatrix()
{
    return d_viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::setViewProjectionMatrix(const mat4Pimpl* viewProjectionMatrix)
{
    *d_viewProjectionMatrix = *viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
const CEGUI::Rectf& OpenGL3Renderer::getActiveViewPort()
{
    return d_activeRenderTarget->getArea();
}


//----------------------------------------------------------------------------//
void OpenGL3Renderer::setActiveRenderTarget(RenderTarget* renderTarget)
{
    d_activeRenderTarget = renderTarget;
}



//----------------------------------------------------------------------------//
RenderTarget* OpenGL3Renderer::getActiveRenderTarget()
{
    return d_activeRenderTarget;
}

//----------------------------------------------------------------------------//
OpenGL3StateChangeWrapper* OpenGL3Renderer::getOpenGLStateChanger()
{
    return d_openGLStateChanger;
}



//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseOpenGLShaders()
{
    checkGLErrors();
    d_shaderManager = new OpenGL3ShaderManager();
    d_shaderManager->initialiseShaders();
    d_shaderStandard = d_shaderManager->getShader(SHADER_ID_STANDARDSHADER);
    GLuint texLoc = d_shaderStandard->getUniformLocation("texture0");
    d_shaderStandard->bind();
    glUniform1i(texLoc, 0);
    d_shaderStandard->unbind();

    d_shaderStandardPosLoc = d_shaderStandard->getAttribLocation("inPosition");
    d_shaderStandardTexCoordLoc = d_shaderStandard->getAttribLocation("inTexCoord");
    d_shaderStandardColourLoc = d_shaderStandard->getAttribLocation("inColour");

    d_shaderStandardMatrixLoc = d_shaderStandard->getUniformLocation("modelViewPerspMatrix");
}

//----------------------------------------------------------------------------//

void initialiseGLExtensions()
{
    glewExperimental = GL_TRUE;

    GLenum err = glewInit();
    if(err != GLEW_OK)
    {
        std::ostringstream err_string;
        //Problem: glewInit failed, something is seriously wrong.
        err_string << "OpenGL3Renderer failed to initialise the GLEW library. "
            << glewGetErrorString(err);

        CEGUI_THROW(RendererException(err_string.str().c_str()));
    }
    //Clear the useless error glew produces as of version 1.7.0, when using OGL3.2 Core Profile
    glGetError();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
