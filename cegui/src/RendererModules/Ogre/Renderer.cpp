/***********************************************************************
    filename:   CEGUIOgreRenderer.cpp
    created:    Tue Feb 17 2009
    author:     Paul D Turner
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
#include "CEGUI/RendererModules/Ogre/Renderer.h"
#include "CEGUI/RendererModules/Ogre/GeometryBuffer.h"
#include "CEGUI/RendererModules/Ogre/TextureTarget.h"
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/RendererModules/Ogre/WindowTarget.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/RendererModules/Ogre/ResourceProvider.h"
#include "CEGUI/RendererModules/Ogre/ImageCodec.h"
#include "CEGUI/Logger.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreGpuProgramManager.h>
#include <OgreGpuProgramParams.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// shader source code strings
static Ogre::String S_hlsl_vs_source(
    "uniform float4x4 worldViewProjMatrix;"
    "struct VS_OUT {"
    "   float4 position : POSITION;"
    "   float2 uv : TEXCOORD0;"
    "   float4 colour : COLOR;"
    "};"
    "VS_OUT main(float4 position : POSITION,"
    "            float2 uv : TEXCOORD0,"
    "            float4 colour : COLOR)"
    "{"
    "    VS_OUT o;"
    "    o.uv = uv;"
    "    o.colour = colour;"
    "    o.position = mul(worldViewProjMatrix, position);"
    "    return o;"
    "}"
);

static Ogre::String S_hlsl_ps_source(
    "float4 main(float4 colour : COLOR,"
    "            float2 texCoord : TEXCOORD0,"
    "            uniform sampler2D texture : TEXUNIT0) : COLOR"
    "{"
    "    return tex2D(texture, texCoord) * colour;"
    "}"
);

static Ogre::String S_glsl_vs_source(
    "void main(void)"
    "{"
    "    gl_TexCoord[0] = gl_MultiTexCoord0;"
    "    gl_FrontColor = gl_Color;"
    "    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;"
    "}"
);

static Ogre::String S_glsl_ps_source(
    "uniform sampler2D texture;"
    "void main(void)"
    "{"
    "    gl_FragColor = texture2D(texture, gl_TexCoord[0].st) * gl_Color;"
    "}"
);

//----------------------------------------------------------------------------//
// Internal Ogre::FrameListener based class.  This is how we noew hook into the
// rendering process (as opposed to render queues previously)
static class OgreGUIFrameListener : public Ogre::FrameListener
{
public:
    OgreGUIFrameListener();

    void setCEGUIRenderEnabled(bool enabled);
    bool isCEGUIRenderEnabled() const;

    bool frameRenderingQueued(const Ogre::FrameEvent& evt);

private:
    bool d_enabled;

} S_frameListener;

//----------------------------------------------------------------------------//
//! container type used to hold TextureTargets we create.
typedef std::vector<TextureTarget*> TextureTargetList;
//! container type used to hold GeometryBuffers we create.
typedef std::vector<OgreGeometryBuffer*> GeometryBufferList;
//! container type used to hold Textures we create.
typedef std::map<String, OgreTexture*, StringFastLessCompare
                 CEGUI_MAP_ALLOC(String, OgreTexture*)> TextureMap;

//----------------------------------------------------------------------------//
// Implementation data for the OgreRenderer
struct OgreRenderer_impl
{
    OgreRenderer_impl() :
        d_displayDPI(96, 96),
        // TODO: should be set to correct value
        d_maxTextureSize(2048),
        d_ogreRoot(Ogre::Root::getSingletonPtr()),
        d_previousVP(0),
        d_activeBlendMode(BM_INVALID),
        d_makeFrameControlCalls(true),
        d_useShaders(false),
        d_worldMatrix(Ogre::Matrix4::IDENTITY),
        d_viewMatrix(Ogre::Matrix4::IDENTITY),
        d_projectionMatrix(Ogre::Matrix4::IDENTITY),
        d_worldViewProjMatrix(Ogre::Matrix4::IDENTITY),
        d_combinedMatrixValid(true)
        {}

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! What the renderer considers to be the current display size.
    Sizef d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    Vector2f d_displayDPI;
    //! The default RenderTarget
    OgreWindowTarget* d_defaultTarget;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! Container used to track textures.
    TextureMap d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
    //! OGRE root object ptr
    Ogre::Root* d_ogreRoot;
    //! Pointer to the render system for Ogre.
    Ogre::RenderSystem* d_renderSystem;
    //! Pointer to the previous viewport set in render system.
    Ogre::Viewport* d_previousVP;
    //! Previous projection matrix set on render system.
    Ogre::Matrix4 d_previousProjMatrix;
    //! What we think is the current blend mode to use
    BlendMode d_activeBlendMode;
    //! Whether _beginFrame and _endFrame will be called.
    bool d_makeFrameControlCalls;
    //! Whether shaders will be used for basic rendering
    bool d_useShaders;
    //! whether shaders are glsl or hlsl
    bool d_useGLSL;
    Ogre::HighLevelGpuProgramPtr d_vertexShader;
    Ogre::HighLevelGpuProgramPtr d_pixelShader;
    Ogre::GpuProgramParametersSharedPtr d_vertexShaderParameters;
    Ogre::GpuProgramParametersSharedPtr d_pixelShaderParameters;

    Ogre::Matrix4 d_worldMatrix;
    Ogre::Matrix4 d_viewMatrix;
    Ogre::Matrix4 d_projectionMatrix;
    Ogre::Matrix4 d_worldViewProjMatrix;
    bool d_combinedMatrixValid;
};

//----------------------------------------------------------------------------//
String OgreRenderer_impl::d_rendererID(
"CEGUI::OgreRenderer - Official OGRE based 2nd generation renderer module.");

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OgreRenderer& renderer = create();
    OgreResourceProvider& rp = createOgreResourceProvider();
    OgreImageCodec& ic = createOgreImageCodec();
    System::create(renderer, &rp, static_cast<XMLParser*>(0), &ic);

    return renderer;
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::bootstrapSystem(Ogre::RenderTarget& target,
                                            const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OgreRenderer& renderer = OgreRenderer::create(target);
    OgreResourceProvider& rp = createOgreResourceProvider();
    OgreImageCodec& ic = createOgreImageCodec();
    System::create(renderer, &rp, static_cast<XMLParser*>(0), &ic);

    return renderer;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    OgreRenderer* renderer = static_cast<OgreRenderer*>(sys->getRenderer());
    OgreResourceProvider* rp =
        static_cast<OgreResourceProvider*>(sys->getResourceProvider());

    OgreImageCodec* ic = &static_cast<OgreImageCodec&>(sys->getImageCodec());

    System::destroy();
    destroyOgreImageCodec(*ic);
    destroyOgreResourceProvider(*rp);
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::create(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OgreRenderer;
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::create(Ogre::RenderTarget& target,
                                   const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OgreRenderer(target);
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroy(OgreRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OgreResourceProvider& OgreRenderer::createOgreResourceProvider()
{
    return *new OgreResourceProvider;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyOgreResourceProvider(OgreResourceProvider& rp)
{
    delete &rp;
}

//----------------------------------------------------------------------------//
OgreImageCodec& OgreRenderer::createOgreImageCodec()
{
    return *new OgreImageCodec;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyOgreImageCodec(OgreImageCodec& ic)
{
    delete &ic;
}

//----------------------------------------------------------------------------//
void OgreRenderer::setRenderingEnabled(const bool enabled)
{
    S_frameListener.setCEGUIRenderEnabled(enabled);
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isRenderingEnabled() const
{
    return S_frameListener.isCEGUIRenderEnabled();
}

//----------------------------------------------------------------------------//
RenderTarget& OgreRenderer::getDefaultRenderTarget()
{
    return *d_pimpl->d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OgreRenderer::createGeometryBuffer()
{
    OgreGeometryBuffer* gb =
        new OgreGeometryBuffer(*this, *d_pimpl->d_renderSystem);

    d_pimpl->d_geometryBuffers.push_back(gb);
    return *gb;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator i = std::find(d_pimpl->d_geometryBuffers.begin(),
                                               d_pimpl->d_geometryBuffers.end(),
                                               &buffer);

    if (d_pimpl->d_geometryBuffers.end() != i)
    {
        d_pimpl->d_geometryBuffers.erase(i);
        delete &buffer;
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyAllGeometryBuffers()
{
    while (!d_pimpl->d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_pimpl->d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* OgreRenderer::createTextureTarget()
{
    TextureTarget* tt = new OgreTextureTarget(*this, *d_pimpl->d_renderSystem);
    d_pimpl->d_textureTargets.push_back(tt);
    return tt;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_pimpl->d_textureTargets.begin(),
                                              d_pimpl->d_textureTargets.end(),
                                              target);

    if (d_pimpl->d_textureTargets.end() != i)
    {
        d_pimpl->d_textureTargets.erase(i);
        delete target;
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyAllTextureTargets()
{
    while (!d_pimpl->d_textureTargets.empty())
        destroyTextureTarget(*d_pimpl->d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name)
{
    throwIfNameExists(name);

    OgreTexture* t = new OgreTexture(name);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name, const String& filename,
                                     const String& resourceGroup)
{
    throwIfNameExists(name);

    OgreTexture* t = new OgreTexture(name, filename, resourceGroup);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name, const Sizef& size)
{
    throwIfNameExists(name);

    OgreTexture* t = new OgreTexture(name, size);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name, Ogre::TexturePtr& tex,
                                     bool take_ownership)
{
    throwIfNameExists(name);

    OgreTexture* t = new OgreTexture(name, tex, take_ownership);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
void OgreRenderer::throwIfNameExists(const String& name) const
{
    if (d_pimpl->d_textures.find(name) != d_pimpl->d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "[OgreRenderer] Texture already exists: " + name));
}

//----------------------------------------------------------------------------//
void OgreRenderer::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OgreRenderer] Created texture: " + name);
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyTexture(const String& name)
{
    TextureMap::iterator i = d_pimpl->d_textures.find(name);

    if (d_pimpl->d_textures.end() != i)
    {
        logTextureDestruction(name);
        delete i->second;
        d_pimpl->d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OgreRenderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyAllTextures()
{
    while (!d_pimpl->d_textures.empty())
        destroyTexture(d_pimpl->d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_pimpl->d_textures.find(name);
    
    if (i == d_pimpl->d_textures.end())
        CEGUI_THROW(UnknownObjectException("Texture does not exist: " + name));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isTextureDefined(const String& name) const
{
    return d_pimpl->d_textures.find(name) != d_pimpl->d_textures.end();
}

//----------------------------------------------------------------------------//
void OgreRenderer::beginRendering()
{
    if ( !d_pimpl->d_previousVP ) 
    {
        d_pimpl->d_previousVP = d_pimpl->d_renderSystem->_getViewport();
        if ( d_pimpl->d_previousVP && d_pimpl->d_previousVP->getCamera() )
            d_pimpl->d_previousProjMatrix =
                d_pimpl->d_previousVP->getCamera()->getProjectionMatrixRS();
    }

    //FIXME: ???
    System::getSingleton().getDefaultGUIContext().getRenderTarget().activate();
    initialiseRenderStateSettings();

    if (d_pimpl->d_makeFrameControlCalls)
        d_pimpl->d_renderSystem->_beginFrame();
}

//----------------------------------------------------------------------------//
void OgreRenderer::endRendering()
{
    if (d_pimpl->d_makeFrameControlCalls)
        d_pimpl->d_renderSystem->_endFrame();

    //FIXME: ???
    System::getSingleton().getDefaultGUIContext().getRenderTarget().deactivate();

    if ( d_pimpl->d_previousVP ) 
    {
        d_pimpl->d_renderSystem->_setViewport(d_pimpl->d_previousVP);
    
        if ( d_pimpl->d_previousVP->getCamera() )
        {
            d_pimpl->d_renderSystem->_setProjectionMatrix(
                d_pimpl->d_previousProjMatrix);
            d_pimpl->d_renderSystem->_setViewMatrix(
                d_pimpl->d_previousVP->getCamera()->getViewMatrix());
        }
        d_pimpl->d_previousVP = 0;
        d_pimpl->d_previousProjMatrix = Ogre::Matrix4::IDENTITY;
    }
}

//----------------------------------------------------------------------------//
const Sizef& OgreRenderer::getDisplaySize() const
{
    return d_pimpl->d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& OgreRenderer::getDisplayDPI() const
{
    return d_pimpl->d_displayDPI;
}

//----------------------------------------------------------------------------//
uint OgreRenderer::getMaxTextureSize() const
{
    return d_pimpl->d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& OgreRenderer::getIdentifierString() const
{
    return d_pimpl->d_rendererID;
}

//----------------------------------------------------------------------------//
OgreRenderer::OgreRenderer() :
    d_pimpl(new OgreRenderer_impl())
{
    checkOgreInitialised();

    // get auto created window
    Ogre::RenderWindow* rwnd = d_pimpl->d_ogreRoot->getAutoCreatedWindow();
    if (!rwnd)
        CEGUI_THROW(RendererException(
            "Ogre was not initialised to automatically create a window, you "
            "should therefore be explicitly specifying a Ogre::RenderTarget in "
            "the OgreRenderer::create function."));

    constructor_impl(*rwnd);
}

//----------------------------------------------------------------------------//
OgreRenderer::OgreRenderer(Ogre::RenderTarget& target) :
    d_pimpl(new OgreRenderer_impl())
{
    checkOgreInitialised();

    constructor_impl(target);
}

//----------------------------------------------------------------------------//
OgreRenderer::~OgreRenderer()
{
    d_pimpl->d_ogreRoot->removeFrameListener(&S_frameListener);

    cleanupShaders();

    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_pimpl->d_defaultTarget;

    delete d_pimpl;
}

//----------------------------------------------------------------------------//
void OgreRenderer::checkOgreInitialised()
{
    if (!d_pimpl->d_ogreRoot)
        CEGUI_THROW(RendererException("The Ogre::Root object has not been "
            "created. You must initialise Ogre first!"));

    if (!d_pimpl->d_ogreRoot->isInitialised())
        CEGUI_THROW(RendererException("Ogre has not been initialised. You must "
            "initialise Ogre first!"));
}

//----------------------------------------------------------------------------//
void OgreRenderer::constructor_impl(Ogre::RenderTarget& target)
{
    d_pimpl->d_renderSystem = d_pimpl->d_ogreRoot->getRenderSystem();

    d_pimpl->d_displaySize.d_width  = target.getWidth();
    d_pimpl->d_displaySize.d_height = target.getHeight();

    // create default target & rendering root (surface) that uses it
    d_pimpl->d_defaultTarget =
        new OgreWindowTarget(*this, *d_pimpl->d_renderSystem, target);

#ifdef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
    // default to using shaders when that is the sane thing to do.
    if (!d_pimpl->d_renderSystem->getFixedPipelineEnabled())
        setUsingShaders(true);
#endif

    // hook into the rendering process
    d_pimpl->d_ogreRoot->addFrameListener(&S_frameListener);
}

//----------------------------------------------------------------------------//
void OgreRenderer::initialiseShaders()
{
    d_pimpl->d_useGLSL = Ogre::HighLevelGpuProgramManager::getSingleton().
        isLanguageSupported("glsl");

    // create vertex shader
    d_pimpl->d_vertexShader = Ogre::HighLevelGpuProgramManager::getSingleton().
        createProgram("__cegui_internal_vs__",
               Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
               d_pimpl->d_useGLSL ? "glsl" : "hlsl", Ogre::GPT_VERTEX_PROGRAM);

    d_pimpl->d_vertexShader->setParameter("entry_point", "main");

    if (d_pimpl->d_useGLSL)
        d_pimpl->d_vertexShader->setParameter("target", "arbvp1");
    else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_4_0"))
        d_pimpl->d_vertexShader->setParameter("target", "vs_4_0");
    else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0"))
        d_pimpl->d_vertexShader->setParameter("target", "vs_2_0");
    else
    {
        d_pimpl->d_vertexShader.setNull();
        CEGUI_THROW(RendererException(
            "OgreRenderer::initialiseShaders: No supported syntax - "
            "unable to compile '__cegui_internal_vs__'"));
    }

    d_pimpl->d_vertexShader->setSource(d_pimpl->d_useGLSL ? S_glsl_vs_source :
                                                            S_hlsl_vs_source);
    d_pimpl->d_vertexShader->load();

    // create pixel shader
    d_pimpl->d_pixelShader = Ogre::HighLevelGpuProgramManager::getSingleton().
        createProgram("__cegui_internal_ps__",
               Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
               d_pimpl->d_useGLSL ? "glsl" : "hlsl", Ogre::GPT_FRAGMENT_PROGRAM);

    d_pimpl->d_pixelShader->setParameter("entry_point", "main");

    if (d_pimpl->d_useGLSL)
        d_pimpl->d_pixelShader->setParameter("target", "arbfp1");
    else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_4_0"))
        d_pimpl->d_pixelShader->setParameter("target", "ps_4_0");
    else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0"))
        d_pimpl->d_pixelShader->setParameter("target", "ps_2_0");
    else
    {
        d_pimpl->d_vertexShader.setNull();
        d_pimpl->d_pixelShader.setNull();

        CEGUI_THROW(RendererException(
            "OgreRenderer::initialiseShaders: No supported syntax - "
            "unable to compile '__cegui_internal_ps__'"));
    }

    d_pimpl->d_pixelShader->setSource(d_pimpl->d_useGLSL ? S_glsl_ps_source :
                                                           S_hlsl_ps_source);
    d_pimpl->d_pixelShader->load();

    d_pimpl->d_vertexShaderParameters =
        d_pimpl->d_vertexShader->createParameters();

    d_pimpl->d_pixelShaderParameters =
        d_pimpl->d_pixelShader->createParameters();

}

//----------------------------------------------------------------------------//
void OgreRenderer::cleanupShaders()
{
    d_pimpl->d_pixelShaderParameters.setNull();
    d_pimpl->d_vertexShaderParameters.setNull();
    d_pimpl->d_pixelShader.setNull();
    d_pimpl->d_vertexShader.setNull();
}

//----------------------------------------------------------------------------//
void OgreRenderer::setDisplaySize(const Sizef& sz)
{
    if (sz != d_pimpl->d_displaySize)
    {
        d_pimpl->d_displaySize = sz;

        // FIXME: This is probably not the right thing to do in all cases.
        Rectf area(d_pimpl->d_defaultTarget->getArea());
        area.setSize(sz);
        d_pimpl->d_defaultTarget->setArea(area);
    }

}

//----------------------------------------------------------------------------//
void OgreRenderer::setupRenderingBlendMode(const BlendMode mode,
                                           const bool force)
{
    using namespace Ogre;

    // do nothing if mode appears current (and is not forced)
    if ((d_pimpl->d_activeBlendMode == mode) && !force)
        return;

    d_pimpl->d_activeBlendMode = mode;

    if (d_pimpl->d_activeBlendMode == BM_RTT_PREMULTIPLIED)
        d_pimpl->d_renderSystem->_setSceneBlending(SBF_ONE,
                                                    SBF_ONE_MINUS_SOURCE_ALPHA);
    else
        d_pimpl->d_renderSystem->
            _setSeparateSceneBlending(SBF_SOURCE_ALPHA,
                                      SBF_ONE_MINUS_SOURCE_ALPHA,
                                      SBF_ONE_MINUS_DEST_ALPHA,
                                      SBF_ONE);
}


//----------------------------------------------------------------------------//
void OgreRenderer::setFrameControlExecutionEnabled(const bool enabled)
{
    d_pimpl->d_makeFrameControlCalls = enabled;

    // default rendering requires _beginFrame and _endFrame calls be made,
    // so if we're disabling those we must also disable default rendering.
    if (!d_pimpl->d_makeFrameControlCalls)
        setRenderingEnabled(false);
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isFrameControlExecutionEnabled() const
{
    return d_pimpl->d_makeFrameControlCalls;
}

//----------------------------------------------------------------------------//
void OgreRenderer::initialiseRenderStateSettings()
{
    using namespace Ogre;

    // initialise render settings
    d_pimpl->d_renderSystem->setLightingEnabled(false);
    d_pimpl->d_renderSystem->_setDepthBufferParams(false, false);
    d_pimpl->d_renderSystem->_setDepthBias(0, 0);
    d_pimpl->d_renderSystem->_setCullingMode(CULL_NONE);
    d_pimpl->d_renderSystem->_setFog(FOG_NONE);
    d_pimpl->d_renderSystem->_setColourBufferWriteEnabled(true, true, true, true);
    d_pimpl->d_renderSystem->setShadingType(SO_GOURAUD);
    d_pimpl->d_renderSystem->_setPolygonMode(PM_SOLID);

    bindShaders();

    // set alpha blending to known state
    setupRenderingBlendMode(BM_NORMAL, true);
}

//----------------------------------------------------------------------------//
void OgreRenderer::setDefaultRootRenderTarget(Ogre::RenderTarget& target)
{
    d_pimpl->d_defaultTarget->setOgreRenderTarget(target);
}

//----------------------------------------------------------------------------//
void OgreRenderer::bindShaders()
{
    if (isUsingShaders())
    {
        if (Ogre::GpuProgram* prog = d_pimpl->d_vertexShader->_getBindingDelegate())
            d_pimpl->d_renderSystem->bindGpuProgram(prog);

        if (Ogre::GpuProgram* prog = d_pimpl->d_pixelShader->_getBindingDelegate())
            d_pimpl->d_renderSystem->bindGpuProgram(prog);
    }
    else
    {
        d_pimpl->d_renderSystem->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
        d_pimpl->d_renderSystem->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
    }
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isUsingShaders() const
{
    return d_pimpl->d_useShaders;
}

//----------------------------------------------------------------------------//
void OgreRenderer::setUsingShaders(const bool use_shaders)
{
    if (d_pimpl->d_useShaders == use_shaders)
        return;

    if (use_shaders)
        initialiseShaders();
    else
        cleanupShaders();

    d_pimpl->d_useShaders = use_shaders;
}

//----------------------------------------------------------------------------//
void OgreRenderer::updateShaderParams() const
{
    if (!isUsingShaders())
        return;

    if (d_pimpl->d_useGLSL)
    {
        d_pimpl->d_pixelShaderParameters->
            setNamedConstant("texture", 0);

        d_pimpl->d_renderSystem->
            bindGpuProgramParameters(Ogre::GPT_FRAGMENT_PROGRAM,
                                     d_pimpl->d_pixelShaderParameters,
                                     Ogre::GPV_ALL);
    }
    else
    {
        d_pimpl->d_vertexShaderParameters->
            setNamedConstant("worldViewProjMatrix", getWorldViewProjMatrix());

        d_pimpl->d_renderSystem->
            bindGpuProgramParameters(Ogre::GPT_VERTEX_PROGRAM,
                                        d_pimpl->d_vertexShaderParameters,
                                        Ogre::GPV_ALL);
    }
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreRenderer::getWorldViewProjMatrix() const
{
    if (!d_pimpl->d_combinedMatrixValid)
    {
        Ogre::Matrix4 final_prj(d_pimpl->d_projectionMatrix);

        if (d_pimpl->d_renderSystem->_getViewport()->getTarget()->
            requiresTextureFlipping())
        {
            final_prj[1][0] = -final_prj[1][0];
            final_prj[1][1] = -final_prj[1][1];
            final_prj[1][2] = -final_prj[1][2];
            final_prj[1][3] = -final_prj[1][3];
        }

        d_pimpl->d_worldViewProjMatrix =
            final_prj * d_pimpl->d_viewMatrix * d_pimpl->d_worldMatrix;

        d_pimpl->d_combinedMatrixValid = true;
    }

    return d_pimpl->d_worldViewProjMatrix;
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreRenderer::getWorldMatrix() const
{
    return d_pimpl->d_worldMatrix;
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreRenderer::getViewMatrix() const
{
    return d_pimpl->d_viewMatrix;
}

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreRenderer::getProjectionMatrix() const
{
    return d_pimpl->d_projectionMatrix;
}

//----------------------------------------------------------------------------//
void OgreRenderer::setWorldMatrix(const Ogre::Matrix4& m)
{
    d_pimpl->d_renderSystem->_setWorldMatrix(m);

    d_pimpl->d_worldMatrix = m;
    d_pimpl->d_combinedMatrixValid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderer::setViewMatrix(const Ogre::Matrix4& m)
{
    d_pimpl->d_renderSystem->_setViewMatrix(m);

    d_pimpl->d_viewMatrix = m;
    d_pimpl->d_combinedMatrixValid = false;
}

//----------------------------------------------------------------------------//
void OgreRenderer::setProjectionMatrix(const Ogre::Matrix4& m)
{
    d_pimpl->d_renderSystem->_setProjectionMatrix(m);

    d_pimpl->d_projectionMatrix = m;
    d_pimpl->d_combinedMatrixValid = false;
}

//----------------------------------------------------------------------------//
OgreGUIFrameListener::OgreGUIFrameListener() :
    d_enabled(true)
{
}

//----------------------------------------------------------------------------//
void OgreGUIFrameListener::setCEGUIRenderEnabled(bool enabled)
{
    d_enabled = enabled;
}

//----------------------------------------------------------------------------//
bool OgreGUIFrameListener::isCEGUIRenderEnabled() const
{
    return d_enabled;
}

//----------------------------------------------------------------------------//
bool OgreGUIFrameListener::frameRenderingQueued(const Ogre::FrameEvent&)
{
    if (d_enabled)
        System::getSingleton().renderAllGUIContexts();

    return true;
}

} // End of  CEGUI namespace section
