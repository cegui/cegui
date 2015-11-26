/***********************************************************************
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
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
#include "CEGUI/RendererModules/Null/Renderer.h"
#include "CEGUI/RendererModules/Null/GeometryBuffer.h"
#include "CEGUI/RendererModules/Null/TextureTarget.h"
#include "CEGUI/RendererModules/Null/Texture.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"

#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
String NullRenderer::d_rendererID(
		"CEGUI::NullRenderer - The null renderer.");

//----------------------------------------------------------------------------//
NullRenderer& NullRenderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

	NullRenderer& renderer = create();
	
	DefaultResourceProvider* rp(CEGUI_NEW_AO DefaultResourceProvider()); 

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
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    NullRenderer* renderer = static_cast<NullRenderer*>(sys->getRenderer());
    ResourceProvider* rp = sys->getResourceProvider();

    //ImageCodec* ic = &(sys->getImageCodec());

    System::destroy();
    // ImageCodec used is currently the system default, so we do not destroy
    // it here (since System already did that).
    //CEGUI_DELETE_AO ic;
	CEGUI_DELETE_AO rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
NullRenderer& NullRenderer::create(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *CEGUI_NEW_AO NullRenderer();
}

//----------------------------------------------------------------------------//
void NullRenderer::destroy(NullRenderer& renderer)
{
    CEGUI_DELETE_AO &renderer;
}

//----------------------------------------------------------------------------//
RenderTarget& NullRenderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& NullRenderer::createGeometryBuffer()
{
    NullGeometryBuffer* gb = CEGUI_NEW_AO NullGeometryBuffer();

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
        CEGUI_DELETE_AO &buffer;
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
    TextureTarget* tt = CEGUI_NEW_AO NullTextureTarget(*this);
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
        CEGUI_DELETE_AO target;
    }
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const String& name)
{
    throwIfNameExists(name);

    NullTexture* t = CEGUI_NEW_AO NullTexture(name);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const String& name, const String& filename,
                                     const String& resourceGroup)
{
    throwIfNameExists(name);

    NullTexture* t = CEGUI_NEW_AO NullTexture(name, filename, resourceGroup);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const String& name, const Sizef& size)
{
    throwIfNameExists(name);

    NullTexture* t = CEGUI_NEW_AO NullTexture(name, size);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
void NullRenderer::throwIfNameExists(const String& name) const
{
    if (d_textures.find(name) != d_textures.end())
        CEGUI_THROW(AlreadyExistsException(
            "[NullRenderer] Texture already exists: " + name));
}

//----------------------------------------------------------------------------//
void NullRenderer::logTextureCreation(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[NullRenderer] Created texture: " + name);
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyTexture(const String& name)
{
    TextureMap::iterator i = d_textures.find(name);

    if (d_textures.end() != i)
    {
        logTextureDestruction(name);
        CEGUI_DELETE_AO i->second;
        d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void NullRenderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[NullRenderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void NullRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException(
            "Texture does not exist: " + name));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool NullRenderer::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
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
const Sizef& NullRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& NullRenderer::getDisplayDPI() const
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

    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//
void NullRenderer::constructor_impl()
{
    // create default target & rendering root (surface) that uses it
    d_defaultTarget = CEGUI_NEW_AO NullRenderTarget<>(*this);
}

//----------------------------------------------------------------------------//
void NullRenderer::setDisplaySize(const Sizef& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // FIXME: This is probably not the right thing to do in all cases.
        Rectf area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of template base class
#include "./RenderTarget.inl"

