/***********************************************************************
    filename:   CEGUIIrrlichtRenderer.cpp
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIIrrlichtRenderer.h"
#include "CEGUIIrrlichtGeometryBuffer.h"
#include "CEGUIIrrlichtWindowTarget.h"
#include "CEGUIIrrlichtTextureTarget.h"
#include "CEGUIIrrlichtTexture.h"
#include "CEGUIIrrlichtResourceProvider.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIIrrlichtEventPusher.h"

#include <irrlicht.h>
#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
String IrrlichtRenderer::d_rendererID("CEGUI::IrrlichtRenderer "
"- Official Irrlicht based 2nd generation renderer module.");

//----------------------------------------------------------------------------//
IrrlichtRenderer& IrrlichtRenderer::create(irr::IrrlichtDevice& device)
{
    return *new IrrlichtRenderer(device);
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroy(IrrlichtRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
IrrlichtResourceProvider&
IrrlichtRenderer::createIrrlichtResourceProvider(irr::io::IFileSystem& fs)
{
    return *new IrrlichtResourceProvider(fs);
}

//----------------------------------------------------------------------------//
void
IrrlichtRenderer::destroyIrrlichtResourceProvider(IrrlichtResourceProvider& rp)
{
    delete &rp;
}

//----------------------------------------------------------------------------//
bool IrrlichtRenderer::injectEvent(const irr::SEvent& event)
{
    return d_eventPusher->OnEvent(event);
}

//----------------------------------------------------------------------------//
RenderingRoot& IrrlichtRenderer::getDefaultRenderingRoot()
{
    return *d_defaultRoot;
}

//----------------------------------------------------------------------------//
GeometryBuffer& IrrlichtRenderer::createGeometryBuffer()
{
    IrrlichtGeometryBuffer* gb =
        new IrrlichtGeometryBuffer(*d_driver);

    d_geometryBuffers.push_back(gb);
    return *gb;
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
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
void IrrlichtRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* IrrlichtRenderer::createTextureTarget()
{
    if (!d_driver->queryFeature(irr::video::EVDF_RENDER_TO_TARGET))
        return 0;

    TextureTarget* tt = new IrrlichtTextureTarget(*this, *d_driver);
    d_textureTargets.push_back(tt);
    return tt;
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyTextureTarget(TextureTarget* target)
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
void IrrlichtRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& IrrlichtRenderer::createTexture()
{
    IrrlichtTexture* t = new IrrlichtTexture(*d_driver);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& IrrlichtRenderer::createTexture(const String& filename,
                                         const String& resourceGroup)
{
    IrrlichtTexture* t = new IrrlichtTexture(*d_driver, filename,
                                             resourceGroup);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& IrrlichtRenderer::createTexture(const Size& size)
{
    IrrlichtTexture* t = new IrrlichtTexture(*d_driver, size);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyTexture(Texture& texture)
{
    TextureList::iterator i = std::find(d_textures.begin(),
                                        d_textures.end(),
                                        &texture);

    if (d_textures.end() != i)
    {
        d_textures.erase(i);
        delete &static_cast<IrrlichtTexture&>(texture);
    }
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(**d_textures.begin());
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::beginRendering()
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::endRendering()
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::setDisplaySize(const Size& sz)
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
const Size& IrrlichtRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2& IrrlichtRenderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint IrrlichtRenderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& IrrlichtRenderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
IrrlichtRenderer::IrrlichtRenderer(irr::IrrlichtDevice& device) :
    d_device(device),
    d_driver(d_device.getVideoDriver()),
    d_displaySize(static_cast<float>(d_driver->getScreenSize().Width),
                  static_cast<float>(d_driver->getScreenSize().Height)),
    d_displayDPI(96, 96),
    d_defaultTarget(new IrrlichtWindowTarget(*this, *d_driver)),
    d_defaultRoot(new RenderingRoot(*d_defaultTarget)),
    d_maxTextureSize(2048),
    d_eventPusher(new IrrlichtEventPusher(d_device.getCursorControl()))
{
}

//----------------------------------------------------------------------------//
IrrlichtRenderer::~IrrlichtRenderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_eventPusher;
    delete d_defaultRoot;
    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
