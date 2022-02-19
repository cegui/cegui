/***********************************************************************
    created:    25th July 2013
    author:     Lukas Meindl
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/Renderer.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/FontManager.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
Renderer::Renderer(float fontScale)
    : d_fontScale(fontScale)
{
}

//----------------------------------------------------------------------------//
Renderer::~Renderer()
{
    for (auto buffer : d_geometryBuffers)
        delete buffer;

    for (auto& pair : d_geomeryBufferPool)
        for (auto buffer : pair.second)
            delete buffer;
}

//----------------------------------------------------------------------------//
void Renderer::addGeometryBuffer(GeometryBuffer& buffer)
{
    d_geometryBuffers.insert(&buffer);
}

//----------------------------------------------------------------------------//
void Renderer::destroyGeometryBuffer(GeometryBuffer& buffer)
{
    auto it = d_geometryBuffers.find(&buffer);
    if (it == d_geometryBuffers.cend())
        return;

    d_geometryBuffers.erase(it);
    buffer.clear();
    d_geomeryBufferPool[buffer.getRenderMaterial()->getShaderWrapper()].push_back(&buffer);
}

//----------------------------------------------------------------------------//
void Renderer::destroyAllGeometryBuffers()
{
    for (auto buffer : d_geometryBuffers)
    {
        buffer->clear();
        d_geomeryBufferPool[buffer->getRenderMaterial()->getShaderWrapper()].push_back(buffer);
    }

    d_geometryBuffers.clear();
}

//----------------------------------------------------------------------------//
GeometryBuffer& Renderer::createGeometryBufferTextured()
{
    // FIXME: see field comment!
    if (!d_texturedShader)
        if (auto mtl = createRenderMaterial(DefaultShaderType::Textured))
            d_texturedShader = mtl->getShaderWrapper();

    auto it = d_geomeryBufferPool.find(d_texturedShader);
    if (it != d_geomeryBufferPool.cend() && !it->second.empty())
    {
        GeometryBuffer* buffer = it->second.back();
        it->second.pop_back();
        addGeometryBuffer(*buffer);
        return *buffer;
    }

    return createGeometryBufferTextured(createRenderMaterial(DefaultShaderType::Textured));
}

//----------------------------------------------------------------------------//
GeometryBuffer& Renderer::createGeometryBufferColoured()
{
    // FIXME: see field comment!
    if (!d_coloredShader)
        if (auto mtl = createRenderMaterial(DefaultShaderType::Solid))
            d_coloredShader = mtl->getShaderWrapper();

    auto it = d_geomeryBufferPool.find(d_coloredShader);
    if (it != d_geomeryBufferPool.cend() && !it->second.empty())
    {
        GeometryBuffer* buffer = it->second.back();
        it->second.pop_back();
        addGeometryBuffer(*buffer);
        return *buffer;
    }

    return createGeometryBufferColoured(createRenderMaterial(DefaultShaderType::Solid));
}

//----------------------------------------------------------------------------//
void Renderer::invalidateGeomBufferMatrices(const CEGUI::RenderTarget* renderTarget)
{
    for (auto buffer : d_geometryBuffers)
        if (buffer->getLastRenderTarget() == renderTarget)
            buffer->invalidateMatrix();
}

//----------------------------------------------------------------------------//
void Renderer::updateGeometryBufferTexCoords(const Texture* texture, const float scaleFactor)
{
    for (auto buffer : d_geometryBuffers)
        buffer->updateTextureCoordinates(texture, scaleFactor);
}

//----------------------------------------------------------------------------//
void Renderer::setActiveRenderTarget(RenderTarget* renderTarget)
{
    d_activeRenderTarget = renderTarget;
}

//----------------------------------------------------------------------------//
void Renderer::setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix)
{
    d_viewProjectionMatrix = viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
void Renderer::setFontScale(const float fontScale)
{
    d_fontScale = fontScale;
    FontManager::getSingleton().updateAllFonts();
}

}
