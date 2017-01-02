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
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/FontManager.h"

namespace CEGUI
{

Renderer::Renderer(const float fontScale):
    d_activeRenderTarget(nullptr),
    d_fontScale(fontScale)
{}

//----------------------------------------------------------------------------//
void Renderer::addGeometryBuffer(GeometryBuffer& buffer) 
{
    d_geometryBuffers.insert(&buffer);
}

//----------------------------------------------------------------------------//
void Renderer::destroyGeometryBuffer(GeometryBuffer& buffer)
{
    GeometryBufferSet::const_iterator findIter = d_geometryBuffers.find(&buffer);

    if (findIter != d_geometryBuffers.end())
    {
        d_geometryBuffers.erase(findIter);
        delete &buffer;
    }
}

//----------------------------------------------------------------------------//
void Renderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(*(*d_geometryBuffers.begin()));
}

//----------------------------------------------------------------------------//
GeometryBuffer& Renderer::createGeometryBufferTextured()
{
    GeometryBuffer& geometry_buffer = createGeometryBufferTextured(createRenderMaterial(DefaultShaderType::Textured));

    return geometry_buffer;
}

//----------------------------------------------------------------------------//
GeometryBuffer& Renderer::createGeometryBufferColoured()
{
    GeometryBuffer& geometry_buffer = createGeometryBufferColoured(createRenderMaterial(DefaultShaderType::Solid));

    return geometry_buffer;
}

//----------------------------------------------------------------------------//
void Renderer::invalidateGeomBufferMatrices(const CEGUI::RenderTarget* renderTarget)
{
    GeometryBufferSet::iterator currentIter = d_geometryBuffers.begin();
    GeometryBufferSet::iterator iterEnd = d_geometryBuffers.end();

    for(; currentIter != iterEnd; ++currentIter)
    {
        GeometryBuffer* geomBuffer = *currentIter;
        if(geomBuffer->getLastRenderTarget() == renderTarget)
            geomBuffer->invalidateMatrix();
    }
}

//----------------------------------------------------------------------------//
void Renderer::setActiveRenderTarget(RenderTarget* renderTarget)
{
    d_activeRenderTarget = renderTarget;
}
        

//----------------------------------------------------------------------------//
RenderTarget* Renderer::getActiveRenderTarget()
{
    return d_activeRenderTarget;
}

//----------------------------------------------------------------------------//
void Renderer::setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix)
{
    d_viewProjectionMatrix = viewProjectionMatrix;
}

//----------------------------------------------------------------------------//
const glm::mat4& Renderer::getViewProjectionMatrix() const
{
    return d_viewProjectionMatrix;
}

void Renderer::setFontScale(const float fontScale)
{
    d_fontScale = fontScale;

    FontManager::getSingleton().updateAllFonts();
}

void Renderer::updateGeometryBufferTexCoords(const Texture* texture, const float scaleFactor)
{
    for(auto& curGeomBuffer : d_geometryBuffers)
    {
        curGeomBuffer->updateTextureCoordinates(texture, scaleFactor);
    }
}

}
