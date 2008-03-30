/***********************************************************************
    filename:   OgreCEGUIRenderer.cpp
    created:    Mon Mar 3 2008
    author:     Paul D Turner
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
#include "OgreCEGUIRenderer.h"
#include "OgreCEGUITexture.h"
#include "OgreCEGUIResourceProvider.h"
#include "OgreCEGUIOgreRenderTargetTarget.h"
#include "OgreCEGUITextureTarget.h"
#include "OgreCEGUIRenderQueueListener.h"
#include <CEGUI/CEGUIRenderTarget.h>
#include <CEGUI/CEGUIEventArgs.h>
#include <OgreSceneManager.h>
#include <OgreRoot.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OgreCEGUIRenderer::OgreCEGUIRenderer(Ogre::RenderTarget* target,
    Ogre::SceneManager* scene_manager, Ogre::uint8 queue_id, bool post_queue):
        d_ogreSceneManager(0)

{
    using namespace Ogre;

    d_identifierString = "CEGUI::OgreRenderer - "
        "Official Ogre based renderer module for CEGUI (2008 version)";

    d_primaryTarget = new OgreRenderTargetTarget(target);

    // create render queue listener used to trigger redraws.
    d_renderQueueListener = new CEGUIRQListener(queue_id, post_queue);

    // if no scene manager is specified, use the first one we come across.
    if (!scene_manager)
    {
        SceneManagerEnumerator::SceneManagerIterator iter =
            Root::getSingleton().getSceneManagerIterator();

        if (iter.hasMoreElements())
            scene_manager = iter.getNext();
    }

    setTriggerSceneManager(scene_manager);
}


//----------------------------------------------------------------------------//
OgreCEGUIRenderer::~OgreCEGUIRenderer()
{
    destroyAllTextures();
    setTriggerSceneManager(0);
    delete d_renderQueueListener;
    delete d_primaryTarget;
}

//----------------------------------------------------------------------------//
RenderTarget* OgreCEGUIRenderer::getPrimaryRenderTarget() const
{
    return d_primaryTarget;
}

//----------------------------------------------------------------------------//
RenderTarget* OgreCEGUIRenderer::createCachingRenderTarget()
{
    // TODO: This should check if such a thing is supported before blindly
    // TODO: returning objects that might well crash the thing!
    return new OgreTextureTarget;
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::destroyCachingRenderTarget(RenderTarget* target)
{
    delete target;
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::doRender()
{
    d_primaryTarget->execute();
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::clearRenderList()
{
    d_primaryTarget->clearRenderList();
}

//----------------------------------------------------------------------------//
Texture* OgreCEGUIRenderer::createTexture()
{
    OgreCEGUITexture* tex = new OgreCEGUITexture(this);
    d_textureList.push_back(tex);
    return tex;
}

//----------------------------------------------------------------------------//
Texture* OgreCEGUIRenderer::createTexture(const String& filename,
    const String& resourceGroup)
{
    OgreCEGUITexture* tex = (OgreCEGUITexture*)createTexture();
    tex->loadFromFile(filename, resourceGroup);
    return tex;
}

//----------------------------------------------------------------------------//
Texture* OgreCEGUIRenderer::createTexture(float size)
{
    OgreCEGUITexture* tex = (OgreCEGUITexture*)createTexture();
    tex->setOgreTextureSize(static_cast<uint>(size));
    return tex;
}

//----------------------------------------------------------------------------//
Texture* OgreCEGUIRenderer::createTexture(Ogre::TexturePtr& texture)
{
    OgreCEGUITexture* t = static_cast<OgreCEGUITexture*>(createTexture());

    if (!texture.isNull())
        t->setOgreTexture(texture);

    return t;
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::destroyTexture(Texture* texture)
{
    if (!texture)
        return;

    OgreCEGUITexture* tex = (OgreCEGUITexture*)texture;
    d_textureList.remove(tex);
    delete tex;
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::destroyAllTextures()
{
    while (!d_textureList.empty())
        destroyTexture(*(d_textureList.begin()));
}

//----------------------------------------------------------------------------//
uint OgreCEGUIRenderer::getMaxTextureSize() const
{
    // TODO: should return something a little closer to reality.
    return 2048;
}

//----------------------------------------------------------------------------//
uint OgreCEGUIRenderer::getHorzScreenDPI() const
{
    return 96;
}

//----------------------------------------------------------------------------//
uint OgreCEGUIRenderer::getVertScreenDPI() const
{
    return 96;
}

//----------------------------------------------------------------------------//
ResourceProvider* OgreCEGUIRenderer::createResourceProvider()
{
    d_resourceProvider = new OgreCEGUIResourceProvider();
    return d_resourceProvider;
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::setTriggerSceneManager(
    Ogre::SceneManager* scene_manager)
{
    // if currently associated with a SceneManager, unhook listener
    if (d_ogreSceneManager)
    {
        d_ogreSceneManager->removeRenderQueueListener(d_renderQueueListener);
        d_ogreSceneManager = 0;
    }

    // if new SceneManager pointer is valid, hook listener into it
    if (scene_manager)
    {
        d_ogreSceneManager = scene_manager;
        d_ogreSceneManager->addRenderQueueListener(d_renderQueueListener);
    }
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::setTriggerRenderQueue(Ogre::uint8 queue_id,
    bool post_queue)
{
    d_renderQueueListener->setTargetRenderQueue(queue_id);
    d_renderQueueListener->setPostRenderQueue(post_queue);
}

//----------------------------------------------------------------------------//
void OgreCEGUIRenderer::setDisplaySize(const Size& sz)
{
    if (d_displaySize != sz)
    {
        d_displaySize = sz;

        EventArgs args;
        fireEvent(EventDisplaySizeChanged, args, EventNamespace);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
