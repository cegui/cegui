/***********************************************************************
    created:    Tue Mar 10 2009
    author:     Paul D Turner (parts based on code by Keith Mok)
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
#include "CEGUI/RendererModules/DirectFB/Renderer.h"
#include "CEGUI/RendererModules/DirectFB/Texture.h"
#include "CEGUI/RendererModules/DirectFB/GeometryBuffer.h"
#include "CEGUI/RendererModules/DirectFB/RenderTarget.h"
#include "CEGUI/TextureTarget.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/Logger.h"
#include "CEGUI/System.h"

#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String DirectFBRenderer::d_rendererID(
"CEGUI::DirectFBRenderer - Official DirectFB based 2nd generation renderer "
"module.");

//----------------------------------------------------------------------------//
DirectFBRenderer& DirectFBRenderer::create(IDirectFB& directfb,
                                           IDirectFBSurface& surface,
                                           const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new DirectFBRenderer(directfb, surface);
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::destroy(DirectFBRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
IDirectFBSurface& DirectFBRenderer::getTargetSurface() const
{
    return *d_targetSurface;
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::setTargetSurface(IDirectFBSurface& surface)
{
    d_targetSurface = &surface;
}

//----------------------------------------------------------------------------//
RenderTarget& DirectFBRenderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& DirectFBRenderer::createGeometryBuffer()
{
    DirectFBGeometryBuffer* b = new DirectFBGeometryBuffer(*this);
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
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
void DirectFBRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* DirectFBRenderer::createTextureTarget()
{
    // TODO:
    return 0;
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::destroyTextureTarget(TextureTarget* target)
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
void DirectFBRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& DirectFBRenderer::createTexture(const CEGUI::String& name)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    DirectFBTexture* tex = new DirectFBTexture(d_directfb, name);
    d_textures[name] = tex;

    logTextureCreation(tex);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& DirectFBRenderer::createTexture(const CEGUI::String& name,
                                         const String& filename,
                                         const String& resourceGroup)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    DirectFBTexture* tex = new DirectFBTexture(d_directfb, name,
                                               filename, resourceGroup);
    d_textures[name] = tex;

    logTextureCreation(tex);

    return *tex;
}

//----------------------------------------------------------------------------//
Texture& DirectFBRenderer::createTexture(const CEGUI::String& name,
                                         const Sizef& size)
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "A texture named '" + name + "' already exists."));

    DirectFBTexture* tex = new DirectFBTexture(d_directfb, name, size);
    d_textures[name] = tex;

    logTextureCreation(tex);

    return *tex;
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::logTextureCreation(DirectFBTexture* texture)
{
    if (Logger* logger = Logger::getSingletonPtr())
    {
        char addr_buff[32];
        sprintf(addr_buff, " (%p)", static_cast<void*>(texture));

        logger->logEvent("[DirectFBRenderer] Created texture: " +
                         texture->getName() + addr_buff);
    }
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::destroyTexture(const CEGUI::String& name)
{
    TextureMap::iterator i = d_textures.find(name);

    if (d_textures.end() != i)
    {
        logTextureDestruction(i->second);
        delete i->second;
        d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::logTextureDestruction(DirectFBTexture* texture)
{
    if (Logger* logger = Logger::getSingletonPtr())
    {
        char addr_buff[32];
        sprintf(addr_buff, " (%p)", static_cast<void*>(texture));

        logger->logEvent("[DirectFBRenderer] Destroyed texture: " +
                         texture->getName() + addr_buff);
    }
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& DirectFBRenderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException(
            "No texture named '" + name + "' is available."));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool DirectFBRenderer::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::beginRendering()
{
    // TODO:
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::endRendering()
{
    // TODO:
}

//----------------------------------------------------------------------------//
void DirectFBRenderer::setDisplaySize(const Sizef& sz)
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
const Sizef& DirectFBRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& DirectFBRenderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint DirectFBRenderer::getMaxTextureSize() const
{
    return 2048;
}

//----------------------------------------------------------------------------//
const String& DirectFBRenderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
DirectFBRenderer::DirectFBRenderer(IDirectFB& directfb,
                                   IDirectFBSurface& surface) :
    d_directfb(directfb),
    d_rootSurface(surface),
    d_targetSurface(&d_rootSurface),
    d_displayDPI(96, 96),
    d_defaultTarget(new DirectFBRenderTarget(*this, d_rootSurface))
{
    int w, h;
    d_rootSurface.GetSize(&d_rootSurface, &w, &h);
    d_displaySize.d_width = static_cast<float>(w);
    d_displaySize.d_height = static_cast<float>(h);
}

//----------------------------------------------------------------------------//
DirectFBRenderer::~DirectFBRenderer()
{
    destroyAllTextureTargets();
    destroyAllTextures();
    destroyAllGeometryBuffers();

    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
