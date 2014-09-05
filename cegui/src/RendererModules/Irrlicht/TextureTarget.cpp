/***********************************************************************
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
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
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "CEGUI/RendererModules/Irrlicht/TextureTarget.h"
#include "CEGUI/RendererModules/Irrlicht/Texture.h"
#include "CEGUI/PropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float IrrlichtTextureTarget::DEFAULT_SIZE = 128.0f;
uint IrrlichtTextureTarget::s_textureNumber = 0;

//----------------------------------------------------------------------------//
IrrlichtTextureTarget::IrrlichtTextureTarget(IrrlichtRenderer& owner,
                                             irr::video::IVideoDriver& driver) :
    IrrlichtRenderTarget<TextureTarget>(owner, driver),
    d_texture(0),
    d_CEGUITexture(static_cast<IrrlichtTexture*>(
        &d_owner.createTexture(generateTextureName())))
{
    // setup area and cause the initial texture to be generated.
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
IrrlichtTextureTarget::~IrrlichtTextureTarget()
{
    cleanupTargetTexture();
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
void IrrlichtTextureTarget::activate()
{
    d_driver.setRenderTarget(d_texture, false, false);
    IrrlichtRenderTarget::activate();
}

//----------------------------------------------------------------------------//
void IrrlichtTextureTarget::deactivate()
{
    IrrlichtRenderTarget::deactivate();
    d_driver.setRenderTarget(0, false, false);
}

//----------------------------------------------------------------------------//
bool IrrlichtTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void IrrlichtTextureTarget::clear()
{
    d_driver.setRenderTarget(d_texture, true, false,
                             irr::video::SColor(0, 0, 0, 0));
    d_driver.setRenderTarget(0, false, false);
}

//----------------------------------------------------------------------------//
Texture& IrrlichtTextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void IrrlichtTextureTarget::declareRenderSize(const Sizef& sz)
{
    const bool realloc =
                !d_texture ||
                static_cast<float>(d_texture->getSize().Width) < sz.d_width ||
                static_cast<float>(d_texture->getSize().Height) < sz.d_height;

    // update area to render into.
    setArea(Rectf(d_area.getPosition(), sz));

    // exit if current texture size is large enough
    if (!realloc)
        return;

    // get adjusted size - to account for device capabilities
    const Sizef final_sz(d_owner.getAdjustedTextureSize(sz));

    cleanupTargetTexture();

    #if CEGUI_IRR_SDK_VERSION >= 16
        const irr::core::dimension2d<irr::u32> irr_sz(
            static_cast<irr::u32>(final_sz.d_width),
            static_cast<irr::u32>(final_sz.d_height));
    #else
        const irr::core::dimension2d<irr::s32> irr_sz(
            static_cast<irr::s32>(final_sz.d_width),
            static_cast<irr::s32>(final_sz.d_height));
    #endif

    d_texture = d_driver.addRenderTargetTexture(
        irr_sz, IrrlichtTexture::getUniqueName().c_str());

    d_CEGUITexture->setIrrlichtTexture(d_texture);
    d_CEGUITexture->setOriginalDataSize(d_area.getSize());

    clear();
}

//----------------------------------------------------------------------------//
bool IrrlichtTextureTarget::isRenderingInverted() const
{
    return false;
}

//----------------------------------------------------------------------------//
void IrrlichtTextureTarget::cleanupTargetTexture()
{
    if (d_texture)
    {
        d_CEGUITexture->setIrrlichtTexture(0);
        d_driver.removeTexture(d_texture);
        d_texture = 0;
    }
}

//----------------------------------------------------------------------------//
String IrrlichtTextureTarget::generateTextureName()
{
    String tmp("_irr_tt_tex_");
    tmp.append(PropertyHelper<uint>::toString(s_textureNumber++));

    return tmp;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of template base class
#include "./RenderTarget.inl"

