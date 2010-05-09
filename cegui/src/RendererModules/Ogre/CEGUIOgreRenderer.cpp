/***********************************************************************
    filename:   CEGUIOgreRenderer.cpp
    created:    Tue Feb 17 2009
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
#include "CEGUIOgreRenderer.h"
#include "CEGUIOgreGeometryBuffer.h"
#include "CEGUIOgreTextureTarget.h"
#include "CEGUIOgreTexture.h"
#include "CEGUIOgreWindowTarget.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIOgreResourceProvider.h"
#include "CEGUIOgreImageCodec.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>

// Start of CEGUI namespace section
namespace CEGUI
{
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
String OgreRenderer::d_rendererID(
"CEGUI::OgreRenderer - Official OGRE based 2nd generation renderer module.");

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::bootstrapSystem()
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException("OgreRenderer::bootstrapSystem: "
            "CEGUI::System object is already initialised."));

    OgreRenderer& renderer = create();
    OgreResourceProvider& rp = createOgreResourceProvider();
    OgreImageCodec& ic = createOgreImageCodec();
    System::create(renderer, &rp, static_cast<XMLParser*>(0), &ic);

    return renderer;
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::bootstrapSystem(Ogre::RenderTarget& target)
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException("OgreRenderer::bootstrapSystem: "
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
        CEGUI_THROW(InvalidRequestException("OgreRenderer::destroySystem: "
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
OgreRenderer& OgreRenderer::create()
{
    return *new OgreRenderer;
}

//----------------------------------------------------------------------------//
OgreRenderer& OgreRenderer::create(Ogre::RenderTarget& target)
{
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
RenderingRoot& OgreRenderer::getDefaultRenderingRoot()
{
    return *d_defaultRoot;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OgreRenderer::createGeometryBuffer()
{
    OgreGeometryBuffer* gb =
        new OgreGeometryBuffer(*this, *d_renderSystem);

    d_geometryBuffers.push_back(gb);
    return *gb;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
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
void OgreRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* OgreRenderer::createTextureTarget()
{
    TextureTarget* tt = new OgreTextureTarget(*this, *d_renderSystem);
    d_textureTargets.push_back(tt);
    return tt;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                              d_textureTargets.end(),
                                              target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        delete target;
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture()
{
    OgreTexture* t = new OgreTexture;
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const String& filename,
                                     const String& resourceGroup)
{
    OgreTexture* t = new OgreTexture(filename, resourceGroup);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(const Size& size)
{
    OgreTexture* t = new OgreTexture(size);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& OgreRenderer::createTexture(Ogre::TexturePtr& tex, bool take_ownership)
{
    OgreTexture* t = new OgreTexture(tex, take_ownership);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyTexture(Texture& texture)
{
    TextureList::iterator i = std::find(d_textures.begin(),
                                        d_textures.end(),
                                        &texture);

    if (d_textures.end() != i)
    {
        d_textures.erase(i);
        delete &static_cast<OgreTexture&>(texture);
    }
}

//----------------------------------------------------------------------------//
void OgreRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(**d_textures.begin());
}

//----------------------------------------------------------------------------//
void OgreRenderer::beginRendering()
{
    initialiseRenderStateSettings();

    if (d_makeFrameControlCalls)
        d_renderSystem->_beginFrame();
}

//----------------------------------------------------------------------------//
void OgreRenderer::endRendering()
{
    if (d_makeFrameControlCalls)
        d_renderSystem->_endFrame();
}

//----------------------------------------------------------------------------//
const Size& OgreRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2& OgreRenderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint OgreRenderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& OgreRenderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
OgreRenderer::OgreRenderer() :
    d_displayDPI(96, 96),
    // TODO: should be set to correct value
    d_maxTextureSize(2048),
    d_ogreRoot(Ogre::Root::getSingletonPtr()),
    d_activeBlendMode(BM_INVALID),
    d_makeFrameControlCalls(true)
{
    checkOgreInitialised();

    // get auto created window
    Ogre::RenderWindow* rwnd = d_ogreRoot->getAutoCreatedWindow();
    if (!rwnd)
        CEGUI_THROW(RendererException(
            "Ogre was not initialised to automatically create a window, you "
            "should therefore be explicitly specifying a Ogre::RenderTarget in "
            "the OgreRenderer::create function."));

    constructor_impl(*rwnd);
}

//----------------------------------------------------------------------------//
OgreRenderer::OgreRenderer(Ogre::RenderTarget& target) :
    d_displayDPI(96, 96),
    // TODO: should be set to correct value
    d_maxTextureSize(2048),
    d_ogreRoot(Ogre::Root::getSingletonPtr()),
    d_activeBlendMode(BM_INVALID),
    d_makeFrameControlCalls(true)
{
    checkOgreInitialised();

    constructor_impl(target);
}

//----------------------------------------------------------------------------//
OgreRenderer::~OgreRenderer()
{
    d_ogreRoot->removeFrameListener(&S_frameListener);

    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_defaultRoot;
    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//
void OgreRenderer::checkOgreInitialised()
{
    if (!d_ogreRoot)
        CEGUI_THROW(RendererException("The Ogre::Root object has not been "
            "created. You must initialise Ogre first!"));

    if (!d_ogreRoot->isInitialised())
        CEGUI_THROW(RendererException("Ogre has not been initialised. You must "
            "initialise Ogre first!"));
}

//----------------------------------------------------------------------------//
void OgreRenderer::constructor_impl(Ogre::RenderTarget& target)
{
    d_renderSystem = d_ogreRoot->getRenderSystem();

    d_displaySize.d_width  = target.getWidth();
    d_displaySize.d_height = target.getHeight();

    // create default target & rendering root (surface) that uses it
    d_defaultTarget = new OgreWindowTarget(*this, *d_renderSystem, target);
    d_defaultRoot = new RenderingRoot(*d_defaultTarget);

    // hook into the rendering process
    d_ogreRoot->addFrameListener(&S_frameListener);
}

//----------------------------------------------------------------------------//
void OgreRenderer::setDisplaySize(const Size& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // FIXME: This is probably not the right thing to do in all cases.
        Rect area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
    }

}

//----------------------------------------------------------------------------//
void OgreRenderer::setupRenderingBlendMode(const BlendMode mode,
                                           const bool force)
{
    using namespace Ogre;

    // do nothing if mode appears current (and is not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;

    if (d_activeBlendMode == BM_RTT_PREMULTIPLIED)
        d_renderSystem->_setSceneBlending(SBF_ONE, SBF_ONE_MINUS_SOURCE_ALPHA);
    else
        d_renderSystem->_setSeparateSceneBlending(SBF_SOURCE_ALPHA,
                                                  SBF_ONE_MINUS_SOURCE_ALPHA,
                                                  SBF_ONE_MINUS_DEST_ALPHA,
                                                  SBF_ONE);
}


//----------------------------------------------------------------------------//
void OgreRenderer::setFrameControlExecutionEnabled(const bool enabled)
{
    d_makeFrameControlCalls = enabled;

    // default rendering requires _beginFrame and _endFrame calls be made,
    // so if we're disabling those we must also disable default rendering.
    if (!d_makeFrameControlCalls)
        setRenderingEnabled(false);
}

//----------------------------------------------------------------------------//
bool OgreRenderer::isFrameControlExecutionEnabled() const
{
    return d_makeFrameControlCalls;
}

//----------------------------------------------------------------------------//
void OgreRenderer::initialiseRenderStateSettings()
{
    using namespace Ogre;

    // initialise render settings
    d_renderSystem->setLightingEnabled(false);
    d_renderSystem->_setDepthBufferParams(false, false);
    d_renderSystem->_setDepthBias(0, 0);
    d_renderSystem->_setCullingMode(CULL_NONE);
    d_renderSystem->_setFog(FOG_NONE);
    d_renderSystem->_setColourBufferWriteEnabled(true, true, true, true);
    d_renderSystem->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);
    d_renderSystem->unbindGpuProgram(GPT_VERTEX_PROGRAM);
    d_renderSystem->setShadingType(SO_GOURAUD);
    d_renderSystem->_setPolygonMode(PM_SOLID);

    // set alpha blending to known state
    setupRenderingBlendMode(BM_NORMAL, true);
}

//----------------------------------------------------------------------------//
void OgreRenderer::setDefaultRootRenderTarget(Ogre::RenderTarget& target)
{
    d_defaultTarget->setOgreRenderTarget(target);
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
        System::getSingleton().renderGUI();

    return true;
}

} // End of  CEGUI namespace section
