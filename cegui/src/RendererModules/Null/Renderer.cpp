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
#include "CEGUI/RendererModules/Null/ShaderWrapper.h"
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
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

	NullRenderer& renderer = create();
	
	DefaultResourceProvider* rp(new DefaultResourceProvider()); 

	// TODO: Create image codec?
	// NullImageCodec& ic = createNullImageCodec();
    System::create(renderer, rp, static_cast<XMLParser*>(nullptr), nullptr);

    return renderer;
}

//----------------------------------------------------------------------------//
void NullRenderer::destroySystem()
{
    System* sys = System::getSingletonPtr();
    if (sys == nullptr)
    {
        throw InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed.");
    }

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
NullRenderer& NullRenderer::create(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *(new NullRenderer());
}

//----------------------------------------------------------------------------//
void NullRenderer::destroy(NullRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
RenderTarget& NullRenderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
RefCounted<RenderMaterial> NullRenderer::createRenderMaterial(const DefaultShaderType shaderType) const
{
    if(shaderType == DefaultShaderType::Textured)
    {
        RefCounted<RenderMaterial> render_material(new RenderMaterial(d_shaderWrapperTextured));

        return render_material;
    }
    else if(shaderType == DefaultShaderType::Solid)
    {
        RefCounted<RenderMaterial> render_material(new RenderMaterial(d_shaderWrapperSolid));

        return render_material;
    }
    else
    {
        throw RendererException(
            "A default shader of this type does not exist.");

        return RefCounted<RenderMaterial>();
    }
}

//----------------------------------------------------------------------------//
GeometryBuffer& NullRenderer::createGeometryBufferTextured(RefCounted<RenderMaterial> renderMaterial)
{
    NullGeometryBuffer* geom_buffer = new NullGeometryBuffer(renderMaterial);

    geom_buffer->addVertexAttribute(VertexAttributeType::Position0);
    geom_buffer->addVertexAttribute(VertexAttributeType::Colour0);
    geom_buffer->addVertexAttribute(VertexAttributeType::TexCoord0);

    addGeometryBuffer(*geom_buffer);
    return *geom_buffer;
}

//----------------------------------------------------------------------------//
GeometryBuffer& NullRenderer::createGeometryBufferColoured(RefCounted<RenderMaterial> renderMaterial)
{
    NullGeometryBuffer* geom_buffer = new NullGeometryBuffer(renderMaterial);

    geom_buffer->addVertexAttribute(VertexAttributeType::Position0);
    geom_buffer->addVertexAttribute(VertexAttributeType::Colour0);

    addGeometryBuffer(*geom_buffer);
    return *geom_buffer;
}

//----------------------------------------------------------------------------//
TextureTarget* NullRenderer::createTextureTarget(bool addStencilBuffer)
{
    TextureTarget* tt = new NullTextureTarget(*this, addStencilBuffer);
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
Texture& NullRenderer::createTexture(const String& name)
{
    throwIfNameExists(name);

    NullTexture* t = new NullTexture(name);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const String& name, const String& filename,
                                     const String& resourceGroup)
{
    throwIfNameExists(name);

    NullTexture* t = new NullTexture(name, filename, resourceGroup);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
Texture& NullRenderer::createTexture(const String& name, const Sizef& size)
{
    throwIfNameExists(name);

    NullTexture* t = new NullTexture(name, size);
    d_textures[name] = t;

    logTextureCreation(name);

    return *t;
}

//----------------------------------------------------------------------------//
void NullRenderer::throwIfNameExists(const String& name) const
{
    if (d_textures.find(name) != d_textures.end())
        throw AlreadyExistsException(
            "[NullRenderer] Texture already exists: " + name);
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
        delete i->second;
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
        throw UnknownObjectException(
            "Texture does not exist: " + name);

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
unsigned int NullRenderer::getMaxTextureSize() const
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
    // TODO: should be set to correct value
    d_maxTextureSize(2048)
{
    constructor_impl();
}

//----------------------------------------------------------------------------//
NullRenderer::~NullRenderer()
{
    delete d_shaderWrapperTextured;
    delete d_shaderWrapperSolid;

    destroyAllGeometryBuffers();
    NullRenderer::destroyAllTextureTargets();
    NullRenderer::destroyAllTextures();

    delete d_defaultTarget;
}

//----------------------------------------------------------------------------//
void NullRenderer::constructor_impl()
{
    d_shaderWrapperTextured = new NullShaderWrapper();
    d_shaderWrapperSolid = new NullShaderWrapper();

    // create default target & rendering root (surface) that uses it
    d_defaultTarget = new NullRenderTarget(*this);
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
bool NullRenderer::isTexCoordSystemFlipped() const
{
    return false;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section


