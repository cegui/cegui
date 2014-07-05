/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Paul D Turner, Henri I Hyyryläinen
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
#include <OgreFrameListener.h>
#include <OgreViewport.h>
#include <OgreCamera.h>

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
#include <Compositor/OgreCompositorManager2.h>
#include <Compositor/OgreCompositorCommon.h>
#include <Compositor/OgreCompositorWorkspaceDef.h>
#include <Compositor/OgreCompositorWorkspace.h>
#include <Compositor/OgreCompositorNodeDef.h>
#include <Compositor/Pass/PassClear/OgreCompositorPassClear.h>
#include <Compositor/Pass/PassScene/OgreCompositorPassScene.h>
#include <Compositor/OgreCompositorWorkspaceListener.h>
#endif // CEGUI_USE_OGRE_COMPOSITOR2
#include "CEGUI/RendererModules/Ogre/ShaderWrapper.h"


#include "Shaders.inl"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
// The new method will be used
// Internal Ogre::CompositorWorkspaceListener. This is how the renderer gets notified
// of workspaces that need rendering
class OgreGUIRenderQueueListener : public Ogre::CompositorWorkspaceListener
{
public:
    OgreGUIRenderQueueListener(OgreRenderer* owner);

    void setCEGUIRenderEnabled(bool enabled);
    bool isCEGUIRenderEnabled() const;

    virtual void passPreExecute(Ogre::CompositorPass *pass);

private:
    bool d_enabled;
    OgreRenderer* d_owner;

};


#else // Use the old method
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

#endif // CEGUI_USE_OGRE_COMPOSITOR2
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
struct OgreRenderer_impl :
    public AllocatedObject<OgreRenderer_impl>
{
    OgreRenderer_impl() :
        d_displayDPI(96, 96),
        // TODO: should be set to correct value
        d_maxTextureSize(2048),
        d_ogreRoot(Ogre::Root::getSingletonPtr()),
#if !defined(CEGUI_USE_OGRE_COMPOSITOR2)
        d_previousVP(0),
#else
        d_frameListener(0),
        d_dummyScene(0),
        d_dummyCamera(0),
        d_workspace(0),
#endif
        d_activeBlendMode(BM_INVALID),
        d_makeFrameControlCalls(true),
        d_useShaders(false),
        d_worldMatrix(Ogre::Matrix4::IDENTITY),
        d_viewMatrix(Ogre::Matrix4::IDENTITY),
        d_projectionMatrix(Ogre::Matrix4::IDENTITY),
        d_worldViewProjMatrix(),
        d_combinedMatrixValid(true),
        d_useGLSL(false),
        d_useGLSLCore(false),
        d_texturedShaderWrapper(0),
        d_colouredShaderWrapper(0)
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
#if !defined(CEGUI_USE_OGRE_COMPOSITOR2)
    //! Pointer to the previous viewport set in render system.
    Ogre::Viewport* d_previousVP;
    //! Previous projection matrix set on render system.
    Ogre::Matrix4 d_previousProjMatrix;
#else
    //! This is used to get notifications when our scene is rendered
    //! no longer static because it requires a pointer to this
    OgreGUIRenderQueueListener* d_frameListener;

    //! Used to render at the correct time
    Ogre::SceneManager* d_dummyScene;
    //! This might not be needed, but it's here
    Ogre::Camera* d_dummyCamera;

    Ogre::CompositorWorkspace* d_workspace;

    //! Makes all scene names unique
    static int s_createdSceneNumber;

    //! Allows the initialization to remain the same by automatically
    //! initializing the Compositor if it isn't already
    static bool s_compositorResourcesInitialized;

#endif
    //! What we think is the current blend mode to use
    BlendMode d_activeBlendMode;
    //! Whether _beginFrame and _endFrame will be called.
    bool d_makeFrameControlCalls;
    //! Whether shaders will be used for basic rendering
    bool d_useShaders;
    //! Whether shaders are glsl or hlsl
    bool d_useGLSL;
    //! Whether we use the ARB glsl shaders or the OpenGL 3.2 Core shader profile (140 core)
    bool d_useGLSLCore;

    OgreShaderWrapper* d_texturedShaderWrapper;
    OgreShaderWrapper* d_colouredShaderWrapper;

    Ogre::Matrix4 d_worldMatrix;
    Ogre::Matrix4 d_viewMatrix;
    Ogre::Matrix4 d_projectionMatrix;
    Ogre::Matrix4 d_worldViewProjMatrix;
    bool d_combinedMatrixValid;
};

//----------------------------------------------------------------------------//
String OgreRenderer_impl::d_rendererID(
"CEGUI::OgreRenderer - OGRE based 3rd generation renderer module"
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
" with Ogre::Compositor2 enabled"
#endif // CEGUI_USE_OGRE_COMPOSITOR2
".");

int OgreRenderer_impl::s_createdSceneNumber = 0;
bool OgreRenderer_impl::s_compositorResourcesInitialized = false;

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
        "CEGUI::System object is already initialised."));

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    createOgreCompositorResources();
#endif

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

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    createOgreCompositorResources();
#endif

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

    return *CEGUI_NEW_AO OgreRenderer();
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::create(Ogre::RenderTarget& target,
                                   const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *CEGUI_NEW_AO OgreRenderer(target);
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroy(OgreRenderer& renderer)
{
    CEGUI_DELETE_AO &renderer;
}

//----------------------------------------------------------------------------//
OgreResourceProvider& OgreRenderer::createOgreResourceProvider()
{
    return *CEGUI_NEW_AO OgreResourceProvider();
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
void OgreRenderer::createOgreCompositorResources()
{
    // Create all the definitions for the workspaces and nodes

    Ogre::CompositorManager2* manager = Ogre::Root::getSingleton().
        getCompositorManager2();

    // We want this to fail if it isn't initialized
    if (!manager)
        CEGUI_THROW(RendererException(
        "Ogre CompositorManager2 is not initialized, "
        "you must call Ogre::Root::initialiseCompositor() after "
        "creating at least one window."));

    auto templatedworkspace = manager->addWorkspaceDefinition("CEGUI_workspace");

    // Create a node for rendering on top of everything
    auto rendernode = manager->addNodeDefinition("CEGUIRenderNode");

    // Use the render target passed from the workspace for rendering on top of
    // everything
    rendernode->addTextureSourceName("renderwindow", 0,
        Ogre::TextureDefinitionBase::TEXTURE_INPUT);

    rendernode->setNumTargetPass(1);

    // Pass for it
    auto targetpasses = rendernode->addTargetPass("renderwindow");
    targetpasses->setNumPasses(2);

    Ogre::CompositorPassClearDef* clearpass =
        static_cast<Ogre::CompositorPassClearDef*>(targetpasses->
            addPass(Ogre::PASS_CLEAR));

    // Only clear depth and stencil since we are rendering on top
    // of an existing image
    clearpass->mClearBufferFlags = Ogre::FBT_DEPTH | Ogre::FBT_STENCIL;


    // Now the render scene pass during which the render queue listener
    // should render the GUI
    Ogre::CompositorPassSceneDef* scenepass =
        static_cast<Ogre::CompositorPassSceneDef*>(targetpasses->
            addPass(Ogre::PASS_SCENE));

    // Just render the overlay group since it is the only one used
    scenepass->mFirstRQ = Ogre::RENDER_QUEUE_OVERLAY;
    scenepass->mLastRQ = Ogre::RENDER_QUEUE_OVERLAY+1;

    // Connect the main render target to the node
    templatedworkspace->connectOutput("CEGUIRenderNode", 0);
    
    // Resources now created
    OgreRenderer_impl::s_compositorResourcesInitialized = true;

}
#endif

//----------------------------------------------------------------------------//
void OgreRenderer::destroyOgreResourceProvider(OgreResourceProvider& rp)
{
    CEGUI_DELETE_AO &rp;
}

//----------------------------------------------------------------------------//
OgreImageCodec& OgreRenderer::createOgreImageCodec()
{
    return *CEGUI_NEW_AO OgreImageCodec();
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyOgreImageCodec(OgreImageCodec& ic)
{
    CEGUI_DELETE_AO &ic;
}

//----------------------------------------------------------------------------//
void OgreRenderer::setRenderingEnabled(const bool enabled)
{
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    d_pimpl->d_frameListener->setCEGUIRenderEnabled(enabled);
    d_pimpl->d_workspace->setEnabled(false);
#else
    S_frameListener.setCEGUIRenderEnabled(enabled);
#endif // CEGUI_USE_OGRE_COMPOSITOR2
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isRenderingEnabled() const
{
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    return d_pimpl->d_frameListener->isCEGUIRenderEnabled();
#else
    return S_frameListener.isCEGUIRenderEnabled();
#endif // CEGUI_USE_OGRE_COMPOSITOR2
}

//----------------------------------------------------------------------------//
RenderTarget& OgreRenderer::getDefaultRenderTarget()
{
    return *d_pimpl->d_defaultTarget;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    // The find function that was here before would no longer compile for some reason

    // Manually search the container for a matching pointer
    auto end = d_pimpl->d_geometryBuffers.end();
    for (auto iter = d_pimpl->d_geometryBuffers.begin(); iter != end; ++iter)
    {
        if ((*iter) == &buffer)
        {
            d_pimpl->d_geometryBuffers.erase(iter);
            CEGUI_DELETE_AO &buffer;
            return;
        }
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyAllGeometryBuffers()
{
    // Perhaps a faster function for destroying many buffers
    for (size_t i = 0; i < d_pimpl->d_geometryBuffers.size(); i++)
    {
        CEGUI_DELETE_AO d_pimpl->d_geometryBuffers[i];
    }

    d_pimpl->d_geometryBuffers.clear();
}

//----------------------------------------------------------------------------//
TextureTarget* OgreRenderer::createTextureTarget()
{
    TextureTarget* tt = CEGUI_NEW_AO OgreTextureTarget(*this, *d_pimpl->d_renderSystem);
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
        CEGUI_DELETE_AO target;
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

    OgreTexture* t = CEGUI_NEW_AO OgreTexture(name);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name, const String& filename,
                                     const String& resourceGroup)
{
    throwIfNameExists(name);

    OgreTexture* t = CEGUI_NEW_AO OgreTexture(name, filename, resourceGroup);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name, const Sizef& size)
{
    throwIfNameExists(name);

    OgreTexture* t = CEGUI_NEW_AO OgreTexture(name, size);
    d_pimpl->d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& name, Ogre::TexturePtr& tex,
                                     bool take_ownership)
{
    throwIfNameExists(name);

    OgreTexture* t = CEGUI_NEW_AO OgreTexture(name, tex, take_ownership);
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
        CEGUI_DELETE_AO i->second;
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
#if !defined(CEGUI_USE_OGRE_COMPOSITOR2)
    if ( !d_pimpl->d_previousVP )
    {
        d_pimpl->d_previousVP = d_pimpl->d_renderSystem->_getViewport();
        if ( d_pimpl->d_previousVP && d_pimpl->d_previousVP->getCamera() )
            d_pimpl->d_previousProjMatrix =
            d_pimpl->d_previousVP->getCamera()->getProjectionMatrixRS();
    }

    //FIXME: ???
    System::getSingleton().getDefaultGUIContext().getRenderTarget().activate();
#endif // CEGUI_USE_OGRE_COMPOSITOR2

    initialiseRenderStateSettings();

    if (d_pimpl->d_makeFrameControlCalls)
        d_pimpl->d_renderSystem->_beginFrame();
}

//----------------------------------------------------------------------------//
void OgreRenderer::endRendering()
{
    if (d_pimpl->d_makeFrameControlCalls)
        d_pimpl->d_renderSystem->_endFrame();

#if !defined(CEGUI_USE_OGRE_COMPOSITOR2)
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
#endif // CEGUI_USE_OGRE_COMPOSITOR2
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
    d_pimpl(CEGUI_NEW_AO OgreRenderer_impl())
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
    d_pimpl(CEGUI_NEW_AO OgreRenderer_impl())
{
    checkOgreInitialised();

    constructor_impl(target);
}

//----------------------------------------------------------------------------//
OgreRenderer::~OgreRenderer()
{
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    // Remove the listener and then delete the scene
    d_pimpl->d_workspace->setListener(0);

    d_pimpl->d_ogreRoot->destroySceneManager(d_pimpl->d_dummyScene);

    d_pimpl->d_dummyScene = 0;
    d_pimpl->d_dummyCamera = 0;

    // Remove the workspace so the contents aren't rendered anymore
    d_pimpl->d_ogreRoot->getCompositorManager2()->removeWorkspace(
        d_pimpl->d_workspace);
    
    d_pimpl->d_workspace = 0;

#else
    d_pimpl->d_ogreRoot->removeFrameListener(&S_frameListener);
#endif // CEGUI_USE_OGRE_COMPOSITOR2

    cleanupShaders();


    CEGUI_DELETE_AO d_pimpl->d_frameListener;

    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    CEGUI_DELETE_AO d_pimpl->d_defaultTarget;
    CEGUI_DELETE_AO d_pimpl;
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

    // Now properly checks for the openGL version
    if (d_pimpl->d_renderSystem->getName().find("OpenGL") != Ogre::String::npos)
    {
        if (d_pimpl->d_renderSystem->getDriverVersion().major >= 3)
            d_pimpl->d_useGLSLCore = true;

    }


    // create default target & rendering root (surface) that uses it
    d_pimpl->d_defaultTarget =
        CEGUI_NEW_AO OgreWindowTarget(*this, *d_pimpl->d_renderSystem, target);

#if OGRE_VERSION >= 0x10800
    // Check if built with RT Shader System and if it is: Check if fixed function pipeline is enabled
    #if defined RTSHADER_SYSTEM_BUILD_CORE_SHADERS
        bool isFixedFunctionEnabled = d_pimpl->d_renderSystem->getFixedPipelineEnabled();
    #else
        bool isFixedFunctionEnabled = true;
    #endif

    #ifndef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
        if (d_pimpl->d_useGLSLCore)
            CEGUI_THROW(RendererException("RT Shader System not available but trying to render using OpenGL 3.X+ Core."
            "When GLSL shaders are necessary, the RT Shader System component of Ogre is required for rendering CEGUI."));
    #endif

    // Default to using shaders when that is the sane thing to do.
    // We check for use of the OpenGL 3+ Renderer in which case we always wanna (because we have to) use shaders
    if (isFixedFunctionEnabled && !d_pimpl->d_useGLSLCore)
    {
        // This may no longer be supported
        CEGUI_THROW(RendererException("Underlying Ogre render system does not support shaders "
            "which are required for supporting custom shaders in this CEGUI version"));
    }

#endif

    // hook into the rendering process
#ifdef CEGUI_USE_OGRE_COMPOSITOR2

    // Some automatic bootstrapping
    if (!OgreRenderer_impl::s_compositorResourcesInitialized)
    {
        createOgreCompositorResources();
    }

    // Create the dummy scene and camera
    std::stringstream scene_name;
    scene_name << "CEGUI_forWindow_" <<
        OgreRenderer_impl::s_createdSceneNumber++;

    d_pimpl->d_dummyScene = d_pimpl->d_ogreRoot->createSceneManager(
        Ogre::ST_INTERIOR, 1, Ogre::INSTANCING_CULLING_SINGLETHREAD,
        scene_name.str());

    // Unused camera for the scene
    d_pimpl->d_dummyCamera = d_pimpl->d_dummyScene->createCamera(
        "CEGUI_dummy_camera");
    

    // We will get notified when the workspace is drawn
    d_pimpl->d_frameListener = CEGUI_NEW_AO OgreGUIRenderQueueListener(this);


    // Create the workspace for rendering
    Ogre::CompositorManager2* manager = d_pimpl->d_ogreRoot->
        getCompositorManager2();

    // The -1 should guarantee this to be rendered last on top of everything
    d_pimpl->d_workspace = manager->addWorkspace(d_pimpl->d_dummyScene,
        &target, d_pimpl->d_dummyCamera, "CEGUI_workspace", true, -1);

    d_pimpl->d_workspace->setListener(d_pimpl->d_frameListener);


#else
    d_pimpl->d_ogreRoot->addFrameListener(&S_frameListener);
#endif // CEGUI_USE_OGRE_COMPOSITOR2


    initialiseShaders();

}

//----------------------------------------------------------------------------//
void OgreRenderer::initialiseShaders()
{

    Ogre::HighLevelGpuProgramPtr texture_vs;
    Ogre::HighLevelGpuProgramPtr texture_ps;

    Ogre::HighLevelGpuProgramPtr colour_vs;
    Ogre::HighLevelGpuProgramPtr colour_ps;


    d_pimpl->d_useGLSL = Ogre::HighLevelGpuProgramManager::getSingleton().
        isLanguageSupported("glsl");

    Ogre::String shaderLanguage;

    if (d_pimpl->d_useGLSL)
    {
        shaderLanguage = "glsl";
    }
    else
    {
        shaderLanguage = "hlsl";
    }

    // Create vertex shaders
    texture_vs = Ogre::HighLevelGpuProgramManager::getSingleton().
        createProgram("__cegui_internal_texture_vs__",
               Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
               shaderLanguage, Ogre::GPT_VERTEX_PROGRAM);

    colour_vs = Ogre::HighLevelGpuProgramManager::getSingleton().
        createProgram("__cegui_internal_colour_vs__",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        shaderLanguage, Ogre::GPT_VERTEX_PROGRAM);

    // Create pixel shaders
    texture_ps = Ogre::HighLevelGpuProgramManager::getSingleton().
        createProgram("__cegui_internal_texture_ps__",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        shaderLanguage, Ogre::GPT_FRAGMENT_PROGRAM);

    colour_ps = Ogre::HighLevelGpuProgramManager::getSingleton().
        createProgram("__cegui_internal_colour_ps__",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        shaderLanguage, Ogre::GPT_FRAGMENT_PROGRAM);



    // We always enter through the main function
    texture_vs->setParameter("entry_point", "main");
    texture_ps->setParameter("entry_point", "main");
    colour_vs->setParameter("entry_point", "main");
    colour_ps->setParameter("entry_point", "main");

    // TODO: make sure that the legacy OpenGL shaders bind parameters properly

    // If we use GLSL
    if (d_pimpl->d_useGLSL)
    {
        // We check if we want to use a GLSL core shader, which is required for the Ogre OpenGL 3+ Renderer
        if (d_pimpl->d_useGLSLCore)
        {
            texture_vs->setParameter("target", "glsl");
            texture_vs->setSource(VertexShaderTextured_GLSL);

            colour_vs->setParameter("target", "glsl");
            colour_vs->setSource(VertexShaderColoured_GLSL);

            texture_ps->setParameter("target", "glsl");
            texture_ps->setSource(PixelShaderTextured_GLSL);

            colour_ps->setParameter("target", "glsl");
            colour_ps->setSource(PixelShaderColoured_GLSL);
        }
        else // else we use regular GLSL shader, as in the normal Ogre OpenGL Renderer
        {
            texture_vs->setParameter("target", "arbvp1");
            texture_vs->setSource(VertexShaderTextured_GLSL_Compat);

            colour_vs->setParameter("target", "arbvp1");
            colour_vs->setSource(VertexShaderColoured_GLSL_Compat);

            texture_ps->setParameter("target", "arbfp1");
            texture_ps->setSource(PixelShaderTextured_GLSL_Compat);

            colour_ps->setParameter("target", "arbfp1");
            colour_ps->setSource(PixelShaderColoured_GLSL_Compat);
        }

    }
    else // else we use a hlsl shader with an available syntax code
    {
        if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_5_0"))
        {
            texture_vs->setParameter("target", "vs_5_0");
            texture_vs->setSource(VertexShaderTextured_HLSL);

            colour_vs->setParameter("target", "vs_5_0");
            colour_vs->setSource(VertexShaderColoured_HLSL);
        }
        else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("vs_2_0"))
        {
            texture_vs->setParameter("target", "vs_2_0");
            texture_vs->setSource(VertexShaderTextured_HLSL);

            colour_vs->setParameter("target", "vs_2_0");
            colour_vs->setSource(VertexShaderColoured_HLSL);
        }
        else// If no shader was compatible
        {
            CEGUI_THROW(RendererException(
                "OgreRenderer::initialiseShaders: No supported syntax - "
                "unable to compile for vs_5_0 or vs_2_0"));
        }

        if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_5_0"))
        {
            texture_ps->setParameter("target", "ps_5_0");
            texture_ps->setSource(PixelShaderTextured_HLSL);

            colour_ps->setParameter("target", "ps_5_0");
            colour_ps->setSource(PixelShaderColoured_HLSL);
        }
        else if (Ogre::GpuProgramManager::getSingleton().isSyntaxSupported("ps_2_0"))
        {
            texture_ps->setParameter("target", "ps_2_0");
            texture_ps->setSource(PixelShaderTextured_HLSL);

            colour_ps->setParameter("target", "ps_2_0");
            colour_ps->setSource(PixelShaderColoured_HLSL);
        }
        else
        {
            CEGUI_THROW(RendererException(
                "OgreRenderer::initialiseShaders: No supported syntax - "
                "unable to compile for ps_5_0 or ps_2_0"));
        }

    }

    // Load all the shaders after setting the source code
    texture_vs->load();
    texture_ps->load();
    colour_vs->load();
    colour_ps->load();


    d_pimpl->d_texturedShaderWrapper = CEGUI_NEW_AO OgreShaderWrapper(*this, 
        *d_pimpl->d_renderSystem, texture_vs, texture_ps);

    d_pimpl->d_colouredShaderWrapper = CEGUI_NEW_AO OgreShaderWrapper(*this, 
        *d_pimpl->d_renderSystem, colour_vs, colour_ps);
}

void OgreRenderer::cleanupShaders()
{
    CEGUI_DELETE_AO d_pimpl->d_texturedShaderWrapper;
    CEGUI_DELETE_AO d_pimpl->d_colouredShaderWrapper;
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
static const Ogre::LayerBlendModeEx S_colourBlendMode =
{
    Ogre::LBT_COLOUR,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE,
    Ogre::ColourValue(0, 0, 0, 0),
    Ogre::ColourValue(0, 0, 0, 0),
    0, 0, 0
};

//----------------------------------------------------------------------------//
static const Ogre::LayerBlendModeEx S_alphaBlendMode =
{
    Ogre::LBT_ALPHA,
    Ogre::LBX_MODULATE,
    Ogre::LBS_TEXTURE,
    Ogre::LBS_DIFFUSE,
    Ogre::ColourValue(0, 0, 0, 0),
    Ogre::ColourValue(0, 0, 0, 0),
    0, 0, 0
};

//----------------------------------------------------------------------------//
static const Ogre::TextureUnitState::UVWAddressingMode S_textureAddressMode =
{
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP,
    Ogre::TextureUnitState::TAM_CLAMP
};

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
    d_pimpl->d_renderSystem->setScissorTest(false);

    // set alpha blending to known state
    setupRenderingBlendMode(BM_NORMAL, true);

    d_pimpl->d_renderSystem->_setTextureCoordCalculation(0, TEXCALC_NONE);
    d_pimpl->d_renderSystem->_setTextureCoordSet(0, 0);
    d_pimpl->d_renderSystem->_setTextureAddressingMode(0, S_textureAddressMode);
    d_pimpl->d_renderSystem->_setTextureMatrix(0, Matrix4::IDENTITY);
    d_pimpl->d_renderSystem->_setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0, false);
    d_pimpl->d_renderSystem->_setTextureBlendMode(0, S_colourBlendMode);
    d_pimpl->d_renderSystem->_setTextureBlendMode(0, S_alphaBlendMode);
    d_pimpl->d_renderSystem->_disableTextureUnitsFrom(1);
}

//----------------------------------------------------------------------------//
void OgreRenderer::setDefaultRootRenderTarget(Ogre::RenderTarget& target)
{
    d_pimpl->d_defaultTarget->setOgreRenderTarget(target);
}

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
void OgreRenderer::updateWorkspaceRenderTarget(Ogre::RenderTarget& target)
{
    // There seems to be no way to change the target, so we need to recreate it
    Ogre::CompositorManager2* manager = d_pimpl->d_ogreRoot->
        getCompositorManager2();

    d_pimpl->d_ogreRoot->getCompositorManager2()->removeWorkspace(
        d_pimpl->d_workspace);

    // The -1 should guarantee this to be rendered last on top of everything
    d_pimpl->d_workspace = manager->addWorkspace(d_pimpl->d_dummyScene,
        &target, d_pimpl->d_dummyCamera, "CEGUI_workspace", true, -1);
}
#endif // CEGUI_USE_OGRE_COMPOSITOR2

//----------------------------------------------------------------------------//
const Ogre::Matrix4& OgreRenderer::getWorldViewProjMatrix() const{
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

void OgreRenderer::bindBlendMode(BlendMode blend){

    setupRenderingBlendMode(blend, false);
}

RefCounted<RenderMaterial> OgreRenderer::createRenderMaterial(
    const DefaultShaderType shaderType) const
{
    if (shaderType == DS_TEXTURED)
    {
        RefCounted<RenderMaterial> render_material(CEGUI_NEW_AO 
            RenderMaterial(d_pimpl->d_texturedShaderWrapper));

        return render_material;
    }
    else if (shaderType == DS_SOLID)
    {
        RefCounted<RenderMaterial> render_material(CEGUI_NEW_AO 
            RenderMaterial(d_pimpl->d_colouredShaderWrapper));

        return render_material;
    }
    else
    {
        CEGUI_THROW(RendererException("A default shader of this type does not exist."));

        return RefCounted<RenderMaterial>();
    }
}

GeometryBuffer& OgreRenderer::createGeometryBufferColoured(
    CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    OgreGeometryBuffer* geom_buffer = CEGUI_NEW_AO OgreGeometryBuffer(*this, 
        *d_pimpl->d_renderSystem, renderMaterial);

    geom_buffer->addVertexAttribute(VAT_POSITION0);
    geom_buffer->addVertexAttribute(VAT_COLOUR0);
    geom_buffer->finaliseVertexAttributes(
        OgreGeometryBuffer::MANUALOBJECT_TYPE_COLOURED);

    addGeometryBuffer(*geom_buffer);
    return *geom_buffer;
}

GeometryBuffer& OgreRenderer::createGeometryBufferTextured(
    CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    OgreGeometryBuffer* geom_buffer = CEGUI_NEW_AO OgreGeometryBuffer(*this, 
        *d_pimpl->d_renderSystem, renderMaterial);
    
    geom_buffer->addVertexAttribute(VAT_POSITION0);
    geom_buffer->addVertexAttribute(VAT_COLOUR0);
    geom_buffer->addVertexAttribute(VAT_TEXCOORD0);
    geom_buffer->finaliseVertexAttributes(
        OgreGeometryBuffer::MANUALOBJECT_TYPE_TEXTURED);

    addGeometryBuffer(*geom_buffer);
    return *geom_buffer;
}

//----------------------------------------------------------------------------//
void OgreRenderer::convertGLMMatrixToOgreMatrix(const glm::mat4& source, 
    Ogre::Matrix4& target)
{
    static const size_t glm_single_size = 
#if(defined(GLM_PRECISION_HIGHP_FLOAT))
        sizeof(glm::highp_float);
#elif(defined(GLM_PRECISION_MEDIUMP_FLOAT))
        sizeof(glm::mediump_float);
#elif(defined(GLM_PRECISION_LOWP_FLOAT))
        sizeof(glm::lowp_float);
#else
        sizeof(glm::mediump_float);
#endif//GLM_PRECISION

    memcpy_s(&target[0][0], sizeof(Ogre::Real)*4, &source[0][0], 
        glm_single_size*4);

    memcpy_s(&target[1][0], sizeof(Ogre::Real)*4, &source[1][0], 
        glm_single_size*4);

    memcpy_s(&target[2][0], sizeof(Ogre::Real)*4, &source[2][0], 
        glm_single_size*4);

    memcpy_s(&target[3][0], sizeof(Ogre::Real)*4, &source[3][0], 
        glm_single_size*4);
}

void OgreRenderer::convertOgreMatrixToGLMMatrix(const Ogre::Matrix4& source, 
    glm::mat4& target)
{
    // This might be as efficient as it gets
    static const size_t glm_single_size = 
#if(defined(GLM_PRECISION_HIGHP_FLOAT))
        sizeof(glm::highp_float);
#elif(defined(GLM_PRECISION_MEDIUMP_FLOAT))
        sizeof(glm::mediump_float);
#elif(defined(GLM_PRECISION_LOWP_FLOAT))
        sizeof(glm::lowp_float);
#else
        sizeof(glm::mediump_float);
#endif//GLM_PRECISION

    memcpy_s(&target[0][0], glm_single_size*4, &source[0][0], 
        sizeof(Ogre::Real)*4);

    memcpy_s(&target[1][0], glm_single_size*4, &source[1][0], 
        sizeof(Ogre::Real)*4);

    memcpy_s(&target[2][0], glm_single_size*4, &source[2][0], 
        sizeof(Ogre::Real)*4);

    memcpy_s(&target[3][0], glm_single_size*4, &source[3][0], 
        sizeof(Ogre::Real)*4);
}

Ogre::SceneManager& OgreRenderer::getDummyScene() const{
    return *d_pimpl->d_dummyScene;
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
OgreGUIRenderQueueListener::OgreGUIRenderQueueListener(OgreRenderer* owner) :
    d_enabled(true), d_owner(owner)
{

}

//----------------------------------------------------------------------------//
void OgreGUIRenderQueueListener::setCEGUIRenderEnabled(bool enabled)
{
    d_enabled = enabled;
}

//----------------------------------------------------------------------------//
bool OgreGUIRenderQueueListener::isCEGUIRenderEnabled() const
{
    return d_enabled;
}

void OgreGUIRenderQueueListener::passPreExecute(Ogre::CompositorPass *pass)
{
    
    if (d_enabled && pass->getType() == Ogre::PASS_SCENE)
    {
        // We should only render contexts that are on this render target
        System::getSingleton().renderAllGUIContextsOnTarget(d_owner);
    }
}

//----------------------------------------------------------------------------//

#else
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
#endif // CEGUI_USE_OGRE_COMPOSITOR2

} // End of  CEGUI namespace section
