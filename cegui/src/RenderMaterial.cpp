/***********************************************************************
    created:    18th July 2013
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
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/ShaderWrapper.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
RenderMaterial::RenderMaterial(ShaderWrapper* shaderWrapper)
    : d_shaderWrapper(shaderWrapper)
{
    d_shaderParamBindings = new ShaderParameterBindings();
}

//----------------------------------------------------------------------------//
RenderMaterial::~RenderMaterial()
{
    delete d_shaderParamBindings;
}

//----------------------------------------------------------------------------//
ShaderParameterBindings* RenderMaterial::getShaderParamBindings() const
{
    return d_shaderParamBindings;
}

//----------------------------------------------------------------------------//
void RenderMaterial::prepareForRendering() const
{
    d_shaderWrapper->prepareForRendering(d_shaderParamBindings);
}

//----------------------------------------------------------------------------//
const ShaderWrapper* RenderMaterial::getShaderWrapper() const
{
    return d_shaderWrapper;
}

//----------------------------------------------------------------------------//
}

