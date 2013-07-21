/***********************************************************************
    filename:   ShaderParameterBindings.cpp
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

#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/System.h"

#include "Glm/glm.hpp"

namespace CEGUI
{
//----------------------------------------------------------------------------//
ShaderParameterBindings::ShaderParameterBindings()
{
}

//----------------------------------------------------------------------------//
ShaderParameterBindings::~ShaderParameterBindings()
{
    ShaderParameterBindingsMap::iterator current = d_shaderParameterBindings.begin();
    ShaderParameterBindingsMap::iterator end = d_shaderParameterBindings.end();

    while (current != end)
    {
        delete current->second;

        current = d_shaderParameterBindings.erase(current);
    }
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::removeParameter(const std::string& parameterName)
{
    ShaderParameterBindingsMap::iterator found_iterator = d_shaderParameterBindings.find(parameterName);
    if (found_iterator != d_shaderParameterBindings.end())
    {
        delete found_iterator->second;
        d_shaderParameterBindings.erase(found_iterator);
    }
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setParameter(const std::string& parameterName, ShaderParameter* shaderParameter)
{
    std::map<std::string, ShaderParameter*>::iterator iter = d_shaderParameterBindings.find(parameterName);
    if(iter != d_shaderParameterBindings.end())
    {
        delete iter->second;
        iter->second = shaderParameter;
    }
    else
    {
        std::pair<std::string, ShaderParameter*> shader_params_pair(parameterName, shaderParameter);
        d_shaderParameterBindings.insert(shader_params_pair);
    }
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setParameter(const std::string& parameterName, const glm::mat4& matrix)
{
    ShaderParameterMatrix* shaderParameterMatrix = new ShaderParameterMatrix(matrix);

    setParameter(parameterName, shaderParameterMatrix);
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setParameter(const std::string& parameterName, const CEGUI::Texture* texture)
{
    ShaderParameterTexture* shaderParameterTexture = new ShaderParameterTexture(texture);

    setParameter(parameterName, shaderParameterTexture);
}

//----------------------------------------------------------------------------//
const ShaderParameterBindings::ShaderParameterBindingsMap& ShaderParameterBindings::getShaderParameterBindings()
{
    return d_shaderParameterBindings;
}

//----------------------------------------------------------------------------//
}
