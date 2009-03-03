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

// Start of CEGUI namespace section
namespace CEGUI
{
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
RenderingRoot& IrrlichtRenderer::getDefaultRenderingRoot()
{
}

//----------------------------------------------------------------------------//
GeometryBuffer& IrrlichtRenderer::createGeometryBuffer()
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyAllGeometryBuffers()
{
}

//----------------------------------------------------------------------------//
TextureTarget* IrrlichtRenderer::createTextureTarget()
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyTextureTarget(TextureTarget* target)
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyAllTextureTargets()
{
}

//----------------------------------------------------------------------------//
Texture& IrrlichtRenderer::createTexture()
{
}

//----------------------------------------------------------------------------//
Texture& IrrlichtRenderer::createTexture(const String& filename,
                                         const String& resourceGroup)
{
}

//----------------------------------------------------------------------------//
Texture& IrrlichtRenderer::createTexture(const Size& size)
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyTexture(Texture& texture)
{
}

//----------------------------------------------------------------------------//
void IrrlichtRenderer::destroyAllTextures()
{
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
const Size& IrrlichtRenderer::getDisplaySize() const
{
}

//----------------------------------------------------------------------------//
const Vector2& IrrlichtRenderer::getDisplayDPI() const
{
}

//----------------------------------------------------------------------------//
uint IrrlichtRenderer::getMaxTextureSize() const
{
}

//----------------------------------------------------------------------------//
const String& IrrlichtRenderer::getIdentifierString() const
{
}

//----------------------------------------------------------------------------//
IrrlichtRenderer::IrrlichtRenderer(irr::IrrlichtDevice& device)
{
}

//----------------------------------------------------------------------------//
IrrlichtRenderer::~IrrlichtRenderer()
{
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
