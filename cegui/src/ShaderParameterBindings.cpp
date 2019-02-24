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

#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/System.h"

#include <glm/glm.hpp>

namespace CEGUI
{
//----------------------------------------------------------------------------//
bool ShaderParameterFloat::equal(const ShaderParameter* other_parameter) const
{
    if (this->getType() == other_parameter->getType())
        return (d_parameterValue == static_cast<const ShaderParameterFloat*>(other_parameter)->d_parameterValue);
    else
        return false;
}

//----------------------------------------------------------------------------//
void ShaderParameterFloat::takeOverParameterValue(const ShaderParameter* other_parameter)
{
    if (getType() == other_parameter->getType())
        d_parameterValue = static_cast<const ShaderParameterFloat*>(other_parameter)->d_parameterValue;
}

//----------------------------------------------------------------------------//
bool ShaderParameterInt::equal(const ShaderParameter* other_parameter) const
{
    if (this->getType() == other_parameter->getType())
        return (d_parameterValue == static_cast<const ShaderParameterInt*>(other_parameter)->d_parameterValue);
    else
        return false;
}

//----------------------------------------------------------------------------//
void ShaderParameterInt::takeOverParameterValue(const ShaderParameter* other_parameter)
{
    if (getType() == other_parameter->getType())
        d_parameterValue = static_cast<const ShaderParameterInt*>(other_parameter)->d_parameterValue;
}

//----------------------------------------------------------------------------//
bool ShaderParameterTexture::equal(const ShaderParameter* other_parameter) const
{
    if (this->getType() == other_parameter->getType())
        return (d_parameterValue == static_cast<const ShaderParameterTexture*>(other_parameter)->d_parameterValue);
    else
        return false;
}

//----------------------------------------------------------------------------//
void ShaderParameterTexture::takeOverParameterValue(const ShaderParameter* other_parameter)
{
    if (getType() == other_parameter->getType())
        d_parameterValue = static_cast<const ShaderParameterTexture*>(other_parameter)->d_parameterValue;
}

//----------------------------------------------------------------------------//
bool ShaderParameterMatrix::equal(const ShaderParameter* other_parameter) const
{
    if (this->getType() == other_parameter->getType())
        return (d_parameterValue == static_cast<const ShaderParameterMatrix*>(other_parameter)->d_parameterValue);
    else
        return false;
}

//----------------------------------------------------------------------------//
void ShaderParameterMatrix::takeOverParameterValue(const ShaderParameter* other_parameter)
{
    if (getType() == other_parameter->getType())
        d_parameterValue = static_cast<const ShaderParameterMatrix*>(other_parameter)->d_parameterValue;
}

//----------------------------------------------------------------------------//
ShaderParameterBindings::ShaderParameterBindings()
{
}

//----------------------------------------------------------------------------//
ShaderParameterBindings::~ShaderParameterBindings()
{
    ShaderParameterBindings::ShaderParameterBindingsMap::iterator iter = d_shaderParameterBindings.begin();
    ShaderParameterBindings::ShaderParameterBindingsMap::iterator end = d_shaderParameterBindings.end();

    while (iter != end)
    {
        delete iter->second;
        ++iter;
    }

    d_shaderParameterBindings.clear();
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::removeParameter(const std::string& parameter_name)
{
    ShaderParameterBindingsMap::iterator found_iterator = d_shaderParameterBindings.find(parameter_name);
    if (found_iterator != d_shaderParameterBindings.end())
    {
        delete found_iterator->second;
        d_shaderParameterBindings.erase(found_iterator);
    }
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setNewParameter(const std::string& parameter_name, ShaderParameter* shader_parameter)
{
    std::map<std::string, ShaderParameter*>::iterator found_iterator = d_shaderParameterBindings.find(parameter_name);
    if (found_iterator != d_shaderParameterBindings.end())
    {
        delete found_iterator->second;
        found_iterator->second = shader_parameter;
    }
    else
    {
        std::pair<std::string, ShaderParameter*> shader_params_pair(parameter_name, shader_parameter);
        d_shaderParameterBindings.insert(shader_params_pair);
    }
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setParameter(const std::string& parameter_name, 
    const glm::mat4& matrix)
{
    ShaderParameter* shader_param = getParameter(parameter_name);
    if (shader_param && ( shader_param->getType() == ShaderParamType::Matrix4X4 ) )
        static_cast<ShaderParameterMatrix*>(shader_param)->d_parameterValue = matrix;
    else
        setNewParameter(parameter_name, new ShaderParameterMatrix(matrix));
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setParameter(const std::string& parameter_name, const CEGUI::Texture* texture)
{
    ShaderParameter* shader_param = getParameter(parameter_name);
    if (shader_param && ( shader_param->getType() == ShaderParamType::Texture ) )
        static_cast<ShaderParameterTexture*>(shader_param)->d_parameterValue = texture;
    else
        setNewParameter(parameter_name, new ShaderParameterTexture(texture));
}

//----------------------------------------------------------------------------//
void ShaderParameterBindings::setParameter(const std::string& parameter_name, 
    const float fvalue)
{
    ShaderParameter* shader_param = getParameter(parameter_name);
    if (shader_param && (shader_param->getType() == ShaderParamType::Float))
        static_cast<ShaderParameterFloat*>(shader_param)->d_parameterValue = 
        fvalue;
    else
        setNewParameter(parameter_name, new ShaderParameterFloat(fvalue));
}

//----------------------------------------------------------------------------//
ShaderParameter* ShaderParameterBindings::getParameter(const std::string& parameter_name)
{
    std::map<std::string, ShaderParameter*>::iterator found_iterator = d_shaderParameterBindings.find(parameter_name);
    if (found_iterator != d_shaderParameterBindings.end())
        return found_iterator->second;
    else
        return nullptr;
}


//----------------------------------------------------------------------------//
const ShaderParameterBindings::ShaderParameterBindingsMap& ShaderParameterBindings::getShaderParameterBindings() const
{
    return d_shaderParameterBindings;
}

//----------------------------------------------------------------------------//
}
