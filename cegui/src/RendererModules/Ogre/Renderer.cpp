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
#include <OgreHardwareVertexBuffer.h>

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

#include "glm/gtc/type_ptr.hpp"

#include <unordered_map>

#define VERTEXBUFFER_POOL_SIZE_STARTCLEAR           60

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

    virtual ~OgreGUIRenderQueueListener(){}

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
typedef std::unordered_map<String, OgreTexture*> TextureMap;

//----------------------------------------------------------------------------//
// Implementation data for the OgreRenderer
struct OgreRenderer_impl
{
    OgreRenderer_impl() :
        //! TODO: Currently there is no way to do this easily using Ogre
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
        d_activeBlendMode(BlendMode::Invalid),
        d_makeFrameControlCalls(true),
        d_useGLSL(false),
        d_useGLSLES(false),
        d_useHLSL(false),
        d_useGLSLCore(false),
        d_texturedShaderWrapper(0),
        d_colouredShaderWrapper(0)
        {}


    //! String holding the renderer identification text.
    static String d_rendererID;
    //! What the renderer considers to be the current display size.
    Sizef d_displaySize;
    //! The default RenderTarget
    OgreWindowTarget* d_defaultTarget;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! Container used to track textures.
    TextureMap d_textures;
    //! What the renderer thinks the max texture size is.
    unsigned int d_maxTextureSize;
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
    //! Whether shaders are glsl or hlsl
    bool d_useGLSL;
    //! Whether shaders are glsles
    bool d_useGLSLES;
    //! Whether shaders are hlsl 
    bool d_useHLSL;
    //! Whether we use the ARB glsl shaders or the OpenGL 3.2 Core shader profile (140 core)
    bool d_useGLSLCore;

    //! Vector containing vertex buffers that can be reused
    std::vector<Ogre::HardwareVertexBufferSharedPtr> d_vbPool;

    OgreShaderWrapper* d_texturedShaderWrapper;
    OgreShaderWrapper* d_colouredShaderWrapper;
};

//----------------------------------------------------------------------------//
String OgreRenderer_impl::d_rendererID(
"CEGUI::OgreRenderer - OGRE based 3rd generation renderer module"
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
" with Ogre::Compositor2 enabled"
#endif // CEGUI_USE_OGRE_COMPOSITOR2
".");

#if defined(CEGUI_USE_OGRE_COMPOSITOR2)
int OgreRenderer_impl::s_createdSceneNumber = 0;
bool OgreRenderer_impl::s_compositorResourcesInitialized = false;
#endif

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
        "CEGUI::System object is already initialised.");

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
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

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
        throw InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed.");

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

    return *new OgreRenderer();
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
    return *new OgreResourceProvider();
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::registerWindow(OgreRenderer& /*main_window*/,
    Ogre::RenderTarget &new_window)
{
    // Link the second renderer to the first for them to share some resources

    return *new OgreRenderer(new_window);
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
        throw RendererException(
        "Ogre CompositorManager2 is not initialized, "
        "you must call Ogre::Root::initialiseCompositor() after "
        "creating at least one window.");

    Ogre::CompositorWorkspaceDef* templatedworkspace = 
        manager->addWorkspaceDefinition("CEGUI_workspace");

    // Create a node for rendering on top of everything
    Ogre::CompositorNodeDef* rendernode = 
        manager->addNodeDefinition("CEGUIRenderNode");

    // Use the render target passed from the workspace for rendering on top of
    // everything
    rendernode->addTextureSourceName("renderwindow", 0,
        Ogre::TextureDefinitionBase::TEXTURE_INPUT);

    rendernode->setNumTargetPass(1);

    // Pass for it
    Ogre::CompositorTargetDef* targetpasses = 
        rendernode->addTargetPass("renderwindow");
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
    delete &rp;
}

//----------------------------------------------------------------------------//
OgreImageCodec& OgreRenderer::createOgreImageCodec()
{
    return *new OgreImageCodec();
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyOgreImageCodec(OgreImageCodec& ic)
{
    delete &ic;
}

//----------------------------------------------------------------------------//
//! Conversion function from Ogre to glm
glm::mat4 OgreRenderer::ogreToGlmMatrix(const Ogre::Matrix4& matrix)
{
    return glm::mat4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
                     matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
                     matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                     matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
}

//----------------------------------------------------------------------------//
//! Conversion function from glm to Ogre
Ogre::Matrix4 OgreRenderer::glmToOgreMatrix(const glm::mat4& matrix)
{
    return Ogre::Matrix4(matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
                         matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
                         matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                         matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
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
TextureTarget* OgreRenderer::createTextureTarget(bool addStencilBuffer)
{
    TextureTarget* tt = new OgreTextureTarget(*this, *d_pimpl->d_renderSystem, addStencilBuffer);
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
        throw AlreadyExistsException(
            "[OgreRenderer] Texture already exists: " + name);
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
        throw UnknownObjectException("Texture does not exist: " + name);

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
    #endif

    if (d_pimpl->d_makeFrameControlCalls)
        d_pimpl->d_renderSystem->_beginFrame();
}

//----------------------------------------------------------------------------//
void OgreRenderer::endRendering()
{
    if (d_pimpl->d_makeFrameControlCalls)
        d_pimpl->d_renderSystem->_endFrame();

    #if !defined(CEGUI_USE_OGRE_COMPOSITOR2)
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
    #endif    
}

//----------------------------------------------------------------------------//
const Sizef& OgreRenderer::getDisplaySize() const
{
    return d_pimpl->d_displaySize;
}
//----------------------------------------------------------------------------//
unsigned int OgreRenderer::getMaxTextureSize() const
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
        throw RendererException(
            "Ogre was not initialised to automatically create a window, you "
            "should therefore be explicitly specifying a Ogre::RenderTarget in "
            "the OgreRenderer::create function.");

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

#ifdef CEGUI_USE_OGRE_COMPOSITOR2
    delete d_pimpl->d_frameListener;
#endif

    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();
    clearVertexBufferPool();

    delete d_pimpl->d_defaultTarget;
    delete d_pimpl;
}

//----------------------------------------------------------------------------//
void OgreRenderer::checkOgreInitialised()
{
    if (!d_pimpl->d_ogreRoot)
        throw RendererException("The Ogre::Root object has not been "
            "created. You must initialise Ogre first!");

    if (!d_pimpl->d_ogreRoot->isInitialised())
        throw RendererException("Ogre has not been initialised. You must "
            "initialise Ogre first!");
}

//----------------------------------------------------------------------------//
void OgreRenderer::constructor_impl(Ogre::RenderTarget& target)
{
    d_pimpl->d_renderSystem = d_pimpl->d_ogreRoot->getRenderSystem();

    d_pimpl->d_displaySize.d_width  = static_cast<float>(target.getWidth());
    d_pimpl->d_displaySize.d_height = static_cast<float>(target.getHeight());

    //! Checking if OpenGL > 3.2 supported
    if (d_pimpl->d_renderSystem->getName().find("OpenGL 3+") != Ogre::String::npos)
    {
        d_pimpl->d_useGLSLCore = true;
    }

    // create default target & rendering root (surface) that uses it
    d_pimpl->d_defaultTarget =
        new OgreWindowTarget(*this, *d_pimpl->d_renderSystem, target);

#if OGRE_VERSION >= 0x10800
    #ifndef RTSHADER_SYSTEM_BUILD_CORE_SHADERS
        throw RendererException("RT Shader System not available. However CEGUI relies on shaders for rendering. ");
    #endif
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
    d_pimpl->d_frameListener = new OgreGUIRenderQueueListener(this);

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
    d_pimpl->d_useGLSLES = Ogre::HighLevelGpuProgramManager::getSingleton().
        isLanguageSupported("glsles");
    d_pimpl->d_useHLSL = Ogre::HighLevelGpuProgramManager::getSingleton().
        isLanguageSupported("hlsl");

    Ogre::String shaderLanguage;

    if (d_pimpl->d_useGLSL)
    {
        shaderLanguage = "glsl";
    }
    else if (d_pimpl->d_useGLSLES)
    {
        shaderLanguage = "glsles";
    }
    else if (d_pimpl->d_useHLSL)
    {
        shaderLanguage = "hlsl";
    }
    else {
        throw RendererException("Underlying Ogre render system does not support available " 
            "shader languages which should be one of glsl, glsles or hlsl "
            "which are required for supporting custom shaders in this CEGUI version");
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
    else if (d_pimpl->d_useGLSLES)
    {
        texture_vs->setParameter("target", "glsles");
        texture_vs->setSource(VertexShaderTextured_GLSLES1);

        colour_vs->setParameter("target", "glsles");
        colour_vs->setSource(VertexShaderColoured_GLSLES1);

        texture_ps->setParameter("target", "glsles");
        texture_ps->setSource(PixelShaderTextured_GLSLES1);

        colour_ps->setParameter("target", "glsles");
        colour_ps->setSource(PixelShaderColoured_GLSLES1);
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
            throw RendererException(
                "OgreRenderer::initialiseShaders: No supported syntax - "
                "unable to compile for vs_5_0 or vs_2_0");
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
            throw RendererException(
                "OgreRenderer::initialiseShaders: No supported syntax - "
                "unable to compile for ps_5_0 or ps_2_0");
        }

    }

    // Load all the shaders after setting the source code
    texture_vs->load();
    texture_ps->load();
    colour_vs->load();
    colour_ps->load();

    d_pimpl->d_texturedShaderWrapper = new OgreShaderWrapper(*this, 
        *d_pimpl->d_renderSystem, texture_vs, texture_ps);

    d_pimpl->d_colouredShaderWrapper = new OgreShaderWrapper(*this, 
        *d_pimpl->d_renderSystem, colour_vs, colour_ps);
}

void OgreRenderer::cleanupShaders()
{
    delete d_pimpl->d_texturedShaderWrapper;
    delete d_pimpl->d_colouredShaderWrapper;
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

    if (d_pimpl->d_activeBlendMode == BlendMode::RttPremultiplied)
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
    setupRenderingBlendMode(BlendMode::Normal, true);
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
void OgreRenderer::setViewProjectionMatrix(const glm::mat4& viewProjMatrix)
{
    d_pimpl->d_renderSystem->_setProjectionMatrix( OgreRenderer::glmToOgreMatrix(viewProjMatrix) );

    d_viewProjectionMatrix = viewProjMatrix;

    if (d_pimpl->d_renderSystem->_getViewport()->getTarget()->requiresTextureFlipping())
    {
        d_viewProjectionMatrix[0][1] = -d_viewProjectionMatrix[0][1];
        d_viewProjectionMatrix[1][1] = -d_viewProjectionMatrix[1][1];
        d_viewProjectionMatrix[2][1] = -d_viewProjectionMatrix[2][1];
        d_viewProjectionMatrix[3][1] = -d_viewProjectionMatrix[3][1];
    }  
}

//----------------------------------------------------------------------------//
void OgreRenderer::bindBlendMode(BlendMode blend)
{
    setupRenderingBlendMode(blend, false);
}

//----------------------------------------------------------------------------//
RefCounted<RenderMaterial> OgreRenderer::createRenderMaterial(
    const DefaultShaderType shaderType) const
{
    if (shaderType == DefaultShaderType::Textured)
    {
        RefCounted<RenderMaterial> render_material(new 
            RenderMaterial(d_pimpl->d_texturedShaderWrapper));

        return render_material;
    }
    else if (shaderType == DefaultShaderType::Solid)
    {
        RefCounted<RenderMaterial> render_material(new 
            RenderMaterial(d_pimpl->d_colouredShaderWrapper));

        return render_material;
    }
    else
    {
        throw RendererException("A default shader of this type does not exist.");

        return RefCounted<RenderMaterial>();
    }
}

//----------------------------------------------------------------------------//
GeometryBuffer& OgreRenderer::createGeometryBufferColoured(
    CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    OgreGeometryBuffer* geom_buffer = new OgreGeometryBuffer(*this, 
        *d_pimpl->d_renderSystem, renderMaterial);

    geom_buffer->addVertexAttribute(VertexAttributeType::Position0);
    geom_buffer->addVertexAttribute(VertexAttributeType::Colour0);
    geom_buffer->finaliseVertexAttributes(
        OgreGeometryBuffer::MT_COLOURED);

    addGeometryBuffer(*geom_buffer);
    return *geom_buffer;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OgreRenderer::createGeometryBufferTextured(
    CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    OgreGeometryBuffer* geom_buffer = new OgreGeometryBuffer(*this, 
        *d_pimpl->d_renderSystem, renderMaterial);
    
    geom_buffer->addVertexAttribute(VertexAttributeType::Position0);
    geom_buffer->addVertexAttribute(VertexAttributeType::Colour0);
    geom_buffer->addVertexAttribute(VertexAttributeType::TexCoord0);
    geom_buffer->finaliseVertexAttributes(
        OgreGeometryBuffer::MT_TEXTURED);

    addGeometryBuffer(*geom_buffer);
    return *geom_buffer;
}

//----------------------------------------------------------------------------//
#ifdef CEGUI_USE_OGRE_COMPOSITOR2
Ogre::SceneManager& OgreRenderer::getDummyScene() const{
    return *d_pimpl->d_dummyScene;
}
#endif

//----------------------------------------------------------------------------//
Ogre::HardwareVertexBufferSharedPtr OgreRenderer::getVertexBuffer(size_t 
    min_size)
{
    Ogre::HardwareVertexBufferSharedPtr result(0);

    if (d_pimpl->d_vbPool.empty())
        return result;

    size_t best_found = -1;
    size_t best_over = -1;

    // The vector is searched in reverse to find exact matches from the end
    // which will allow popping the last element which is faster then removing
    // the first one

    for (size_t i = d_pimpl->d_vbPool.size(); --i > 0;)
    {
        // It seems that there can be nullptrs in the pool
        Ogre::HardwareVertexBufferSharedPtr current = d_pimpl->d_vbPool[i];

        if (!current.get()){

            d_pimpl->d_vbPool.erase(d_pimpl->d_vbPool.begin()+i);
            continue;
        }
        
        size_t current_over = current->getNumVertices()-min_size;

        // Perfect match stops searching instantly
        if (current_over == 0)
        {

            best_found = i;
            best_over = 0;
            break;
        }

        if (current_over <= best_over)
        {

            best_over = current_over;
            best_found = i;
        }
    }

    // If the smallest buffer is too large then none is found
    // This will also be true if all buffers are too small
    if (best_over > min_size*1.5f || best_found >= d_pimpl->d_vbPool.size())
    {
        // Clear if there are too many buffers
        int over_size = d_pimpl->d_vbPool.size()-
            VERTEXBUFFER_POOL_SIZE_STARTCLEAR;

        if (over_size > 2)
            cleanLargestVertexBufferPool(over_size);

    } else {

        result = d_pimpl->d_vbPool[best_found];

        d_pimpl->d_vbPool.erase(d_pimpl->d_vbPool.begin()+best_found);

        // We want to avoid using too much memory
        // even if matches are always found
        int over_size = d_pimpl->d_vbPool.size()-
            (VERTEXBUFFER_POOL_SIZE_STARTCLEAR*5);

        if (over_size > 5)
            cleanLargestVertexBufferPool(over_size/2);
    }

    return result;
}

void OgreRenderer::returnVertexBuffer(Ogre::HardwareVertexBufferSharedPtr 
    buffer)
{
    d_pimpl->d_vbPool.push_back(buffer);
}

void OgreRenderer::clearVertexBufferPool()
{
    d_pimpl->d_vbPool.clear();
}

bool hardwareBufferSizeLess(const Ogre::HardwareVertexBufferSharedPtr &first,
    const Ogre::HardwareVertexBufferSharedPtr &second)
{
    return first->getNumVertices() < second->getNumVertices();
}

void OgreRenderer::cleanLargestVertexBufferPool(size_t count)
{
    // The easiest way might be to sort the vector and delete the last count
    // elements
    std::sort(d_pimpl->d_vbPool.begin(), d_pimpl->d_vbPool.end(), 
        &hardwareBufferSizeLess);

    // Adjust the count if there aren't enough elements to delete to avoid
    // asserting
    if (count >= d_pimpl->d_vbPool.size())
    {

        d_pimpl->d_vbPool.clear();
    }

    for (size_t i = 0; i < count; i++)
    {

        d_pimpl->d_vbPool.pop_back();
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::initialiseTextureStates()
{
    d_pimpl->d_renderSystem->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
    d_pimpl->d_renderSystem->_setTextureCoordSet(0, 0);
    d_pimpl->d_renderSystem->_setTextureAddressingMode(0, S_textureAddressMode);
    d_pimpl->d_renderSystem->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
    d_pimpl->d_renderSystem->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_NONE);
    d_pimpl->d_renderSystem->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
    d_pimpl->d_renderSystem->_setTextureBlendMode(0, S_colourBlendMode);
    d_pimpl->d_renderSystem->_setTextureBlendMode(0, S_alphaBlendMode);
    d_pimpl->d_renderSystem->_disableTextureUnitsFrom(1);
}

//----------------------------------------------------------------------------//
bool OgreRenderer::usesOpenGL()
{
    return d_pimpl->d_renderSystem->getName().find("OpenGL") != Ogre::String::npos;
}

//----------------------------------------------------------------------------//
bool OgreRenderer::usesDirect3D()
{
    return d_pimpl->d_renderSystem->getName().find("Direct3D") != Ogre::String::npos;
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isTexCoordSystemFlipped() const
{
    return false;
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
