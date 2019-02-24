/***********************************************************************
    created:    Tue Feb 17 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/Ogre/TextureTarget.h"
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/PropertyHelper.h"

#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderTexture.h>
#include <OgreRenderSystem.h>
#include <OgreViewport.h>


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float OgreTextureTarget::DEFAULT_SIZE = 128.0f;
std::uint32_t OgreTextureTarget::s_textureNumber = 0;

//----------------------------------------------------------------------------//
OgreTextureTarget::OgreTextureTarget(OgreRenderer& owner,
                                     Ogre::RenderSystem& rs,
                                     bool addStencilBuffer) :
    OgreRenderTarget(owner, rs),
    TextureTarget(addStencilBuffer),
    d_CEGUITexture(0)
{
    d_CEGUITexture = static_cast<OgreTexture*>(
        &d_owner.createTexture(generateTextureName()));

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
OgreTextureTarget::~OgreTextureTarget()
{
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
bool OgreTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::clear()
{
    if (!d_viewportValid)
        updateViewport();

    Ogre::Viewport* const saved_vp = d_renderSystem._getViewport();

    d_renderSystem._setViewport(d_viewport);
    d_renderSystem.clearFrameBuffer(Ogre::FBT_COLOUR,
        Ogre::ColourValue(0, 0, 0, 0));

#if OGRE_VERSION < 0x10800
    if (saved_vp)
        d_renderSystem._setViewport(saved_vp);
#else
    d_renderSystem._setViewport(saved_vp);
#endif
}

//----------------------------------------------------------------------------//
Texture& OgreTextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void OgreTextureTarget::declareRenderSize(const Sizef& sz)
{
    // exit if current size is enough
    if ((d_area.getWidth() >= sz.d_width) && (d_area.getHeight() >=sz.d_height))
        return;

    Ogre::TexturePtr rttTex = Ogre::TextureManager::getSingleton().createManual(OgreTexture::getUniqueName(),
                                                                                Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                                Ogre::TEX_TYPE_2D,
                                                                                static_cast<Ogre::uint>(sz.d_width),
                                                                                static_cast<Ogre::uint>(sz.d_height),
                                                                                1,
                                                                                0, 
                                                                                Ogre::PF_A8R8G8B8,
                                                                                Ogre::TU_RENDERTARGET);

    d_renderTarget = rttTex->getBuffer()->getRenderTarget();

    const Rectf init_area(
        glm::vec2(0.0f, 0.0f),
        Sizef(
            static_cast<float>(d_renderTarget->getWidth()),
            static_cast<float>(d_renderTarget->getHeight())
        )
    );

    setArea(init_area);

    // delete viewport and reset ptr so a new one is generated.  This is
    // required because we have changed d_renderTarget so need a new VP also.
    OGRE_DELETE d_viewport;
    d_viewport = 0;

    // because Texture takes ownership, the act of setting the new ogre texture
    // also ensures any previous ogre texture is released.
    d_CEGUITexture->setOgreTexture(rttTex, true);

    clear();
}

//----------------------------------------------------------------------------//
String OgreTextureTarget::generateTextureName()
{
    String tmp("_ogre_tt_tex_");
    tmp.append(PropertyHelper<std::uint32_t>::toString(s_textureNumber++));

    return tmp;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

#ifdef __APPLE__

template class CEGUI::OgreRenderTarget<CEGUI::RenderTarget>;
template class CEGUI::OgreRenderTarget<CEGUI::TextureTarget>;

#endif

