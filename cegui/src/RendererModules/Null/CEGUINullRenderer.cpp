/***********************************************************************
    filename:   CEGUINullRenderer.cpp
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
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
#include "CEGUINullRenderer.h"
#include "CEGUINullGeometryBuffer.h"
#include "CEGUINullTextureTarget.h"
#include "CEGUINullTexture.h"
#include "CEGUIImageCodec.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIDefaultResourceProvider.h"

#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String NullRenderer::d_rendererID(
		"CEGUI::NullRenderer - The null renderer.");

//----------------------------------------------------------------------------//
NullRenderer& NullRenderer::bootstrapSystem()
{
    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException("NullRenderer::bootstrapSystem: "
            "CEGUI::System object is already initialised."));

	NullRenderer& renderer = create();
	
	DefaultResourceProvider* rp(new DefaultResourceProvider()); 

	// TODO: Create image codec?
	// NullImageCodec& ic = createNullImageCodec();
    System::create(renderer, rp, static_cast<XMLParser*>(0), 0);

    return renderer;
}

//----------------------------------------------------------------------------//
void NullRenderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException("NullRenderer::destroySystem: "
            "CEGUI::System object is not created or was already destroyed."));

    NullRenderer* renderer = static_cast<NullRenderer*>(sys->getRenderer());
    ResourceProvider* rp = sys->getResourceProvider();

    //ImageCodec* ic = &(sys->getImageCodec());

    System::destroy();
    // ImageCodec used is currently the system default, so we do not destroy
    // it here (since System already did that).
    //delete ic;
	delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
NullRenderer& NullRenderer::create()
{
    return *new NullRenderer;
}

//----------------------------------------------------------------------------//
void NullRenderer::destroy(NullRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
RenderingRoot& NullRenderer::getDefaultRenderingRoot()
{
    return *d_defaultRoot;
}

//----------------------------------------------------------------------------//
GeometryBuffer& NullRenderer::createGeometryBuffer()
{
    NullGeometryBuffer* gb = new NullGeometryBuffer();

    d_geometryBuffers.push_back(gb);
    return *gb;
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
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
void NullRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* NullRenderer::createTextureTarget()
{
    TextureTarget* tt = new NullTextureTarget(*this);
    d_textureTargets.push_back(tt);
    return tt;
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyTextureTarget(TextureTarget* target)
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
void NullRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture()
{
    NullTexture* t = new NullTexture;
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const String& filename,
                                     const String& resourceGroup)
{
    NullTexture* t = new NullTexture(filename, resourceGroup);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const Size<>& size)
{
    NullTexture* t = new NullTexture(size);
    d_textures.push_back(t);
    return *t;
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyTexture(Texture& texture)
{
    TextureList::iterator i = std::find(d_textures.begin(),
                                        d_textures.end(),
                                        &texture);

    if (d_textures.end() != i)
    {
        d_textures.erase(i);
        delete &static_cast<NullTexture&>(texture);
    }
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(**d_textures.begin());
}

//----------------------------------------------------------------------------//
void NullRenderer::beginRendering()
{
}

//----------------------------------------------------------------------------//
void NullRenderer::endRendering()
{
}

//----------------------------------------------------------------------------//
const Size<>& NullRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2<>& NullRenderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint NullRenderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& NullRenderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
NullRenderer::NullRenderer() :
    d_displayDPI(96, 96),
    // TODO: should be set to correct value
    d_maxTextureSize(2048)
{
    constructor_impl();
}

//----------------------------------------------------------------------------//
NullRenderer::~NullRenderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_defaultRoot;
    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//
void NullRenderer::constructor_impl()
{
    // create default target & rendering root (surface) that uses it
    d_defaultTarget = new NullRenderTarget(*this);
    d_defaultRoot = new RenderingRoot(*d_defaultTarget);
}

//----------------------------------------------------------------------------//
void NullRenderer::setDisplaySize(const Size<>& sz)
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

} // End of  CEGUI namespace section
