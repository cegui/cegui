/***********************************************************************
    filename:   OgreCEGUITextureTarget.cpp
    created:    Sat Mar 8 2008
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
#include "OgreCEGUITextureTarget.h"
#include "OgreCEGUIRenderQueueListener.h"
#include <OgreRoot.h>
#include <OgreSceneManager.h>
#include <OgreHardwarePixelBuffer.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
int OgreTextureTarget::s_rttID(0);
int OgreTextureTarget::s_instanceCount(0);
Ogre::SceneManager* OgreTextureTarget::s_ogreSceneManager(0);
Ogre::Camera* OgreTextureTarget::s_ogreCamera(0);

//----------------------------------------------------------------------------//
OgreTextureTarget::OgreTextureTarget()
{
    using namespace Ogre;

    // if this is the first instance, create the shared objects
    if (!s_instanceCount)
    {
        s_ogreSceneManager = d_ogreRoot->
            createSceneManager(ST_GENERIC, "_cegui_rtt_sm");
        s_ogreCamera = s_ogreSceneManager->createCamera("_cegui_rtt_cam");
    }

    discoverTextureCapabilities();

    d_area.setSize(Size(DEFAULT_SIZE, DEFAULT_SIZE));
    initialiseTargetTexture();

    ++s_instanceCount;
}

//----------------------------------------------------------------------------//
OgreTextureTarget::~OgreTextureTarget()
{
    d_texture.setNull();

    --s_instanceCount;

    // clean up shared objects if this was the last instance.
    if (!s_instanceCount)
    {
        s_ogreSceneManager->destroyCamera(s_ogreCamera);
        d_ogreRoot->destroySceneManager(s_ogreSceneManager);
        s_ogreCamera = 0;
        s_ogreSceneManager = 0;
    }
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::queueToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    if (!d_queueEnabled)
    {
        renderToTarget(src, dst, z, cols, quad_split_mode, target);
        return;
    }

    Rect tex_rect;
    initTexRectFromPixRect(src, tex_rect);

    Quad quad;
    // _initialise_quad must be called on the destination to ensure that the
    // geometry is set up in terms of the destination's area and not our own.
    static_cast<OgreRenderTarget&>(target).
        _initialise_quad(quad, dst, z, d_texture, tex_rect,
                         quad_split_mode, cols);

    static_cast<OgreRenderTarget&>(target)._queue_quad(quad);
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::renderToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    Rect tex_rect;
    initTexRectFromPixRect(src, tex_rect);

    Quad quad;
    // _initialise_quad must be called on the destination to ensure that the
    // geometry is set up in terms of the destination's area and not our own.
    static_cast<OgreRenderTarget&>(target).
        _initialise_quad(quad, dst, z, d_texture, tex_rect,
                         quad_split_mode, cols);

    static_cast<OgreRenderTarget&>(target)._render_quad(quad);
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::setArea(const Rect& area)
{
    // area comes from the texture target and should not be changed externally.
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::declareRenderSize(const Size& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    d_area.setSize(sz);
    initialiseTargetTexture();
}

//----------------------------------------------------------------------------//
bool OgreTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::initTexRectFromPixRect(const Rect& pix_rect,
    Rect& tex_rect) const
{
    tex_rect.d_left = pix_rect.d_left / d_area.getWidth();
    tex_rect.d_right = pix_rect.d_right / d_area.getWidth();
    tex_rect.d_top = pix_rect.d_top / d_area.getHeight();
    tex_rect.d_bottom = pix_rect.d_bottom / d_area.getHeight();
}

//----------------------------------------------------------------------------//
Ogre::String OgreTextureTarget::generateTextureName()
{
    Ogre::StringUtil::StrStreamType strstream;
    strstream << "_cegui_rtt_tex_" << s_rttID++;
    return strstream.str();
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::beginRender()
{
    OgreRenderTarget::beginRender();

    // clear the texture
    d_ogreRenderSystem->clearFrameBuffer(Ogre::FBT_COLOUR,
                                         Ogre::ColourValue(0, 0, 0, 0));
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::initialiseTargetTexture()
{
    using namespace Ogre;

    d_texture.setNull();

    Size sz(getAdjustedTextureSize(d_area.getSize()));

    // create the texture to render to
    d_texture = TextureManager::getSingleton().createManual(
        generateTextureName(),
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
        sz.d_width, sz.d_height,
        1, 0, PF_R8G8B8A8, TU_RENDERTARGET);

    // finish construction tasks
    setViewport(d_texture->getBuffer()->
        getRenderTarget()->addViewport(s_ogreCamera));
    initialiseTargetArea();

    // make sure we're the only ones who write to the damned thing!!
    d_ogreViewport->getTarget()->setActive(false);
}

//----------------------------------------------------------------------------//
Size OgreTextureTarget::getAdjustedTextureSize(const Size& sz) const
{
    // add some room to manoeuvre (this maybe a little hackish)
    int new_width = static_cast<int>(sz.d_width) + 128;
    int new_height = static_cast<int>(sz.d_height) + 128;

    // make dimensions power of two if that is required
    if (!d_supportsNPOT)
    {
        new_width = getNextPowerOfTwo(new_width);
        new_height = getNextPowerOfTwo(new_height);
    }

    // clamp sizes to maximum allowed
    if (new_width > d_maxTextureSize)
        new_width = d_maxTextureSize;

    if (new_height > d_maxTextureSize)
        new_height = d_maxTextureSize;

    return Size(static_cast<float>(new_width), static_cast<float>(new_height));
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::discoverTextureCapabilities()
{
    using namespace Ogre;
    RenderSystem* rs = Root::getSingleton().getRenderSystem();
    const RenderSystemCapabilities* caps = rs->getCapabilities();

    d_supportsNPOT = caps->hasCapability(RSC_NON_POWER_OF_2_TEXTURES);

    // TODO: this is obviously a total load of BS (or perhaps a lucky guess)!
    d_maxTextureSize = 2048;
}

//----------------------------------------------------------------------------//
uint OgreTextureTarget::getNextPowerOfTwo(uint size) const
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

} // End of  CEGUI namespace section
