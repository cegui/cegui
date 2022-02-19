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
#include "CEGUI/ShaderWrapper.h"

namespace CEGUI
{
const std::string MainTextureParameterName("texture0");

//----------------------------------------------------------------------------//
RenderMaterial::RenderMaterial(ShaderWrapper* shaderWrapper)
    : d_shaderWrapper(shaderWrapper)
{
}

//----------------------------------------------------------------------------//
void RenderMaterial::prepareForRendering() const
{
    d_shaderWrapper->prepareForRendering(&d_shaderParamBindings);
}

//----------------------------------------------------------------------------//
void RenderMaterial::setMainTexture(const Texture* texture)
{
    if (d_mainTextureParam)
    {
        d_mainTextureParam->d_parameterValue = texture;
    }
    else
    {
        d_shaderParamBindings.setParameter(MainTextureParameterName, texture);
        d_mainTextureParam = static_cast<ShaderParameterTexture*>(d_shaderParamBindings.getParameter(MainTextureParameterName));
    }
}

//----------------------------------------------------------------------------//
const Texture* RenderMaterial::getMainTexture() const
{
    return d_mainTextureParam ? d_mainTextureParam->d_parameterValue : nullptr;
}

}
