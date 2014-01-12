/***********************************************************************
    filename:   GL3ShaderWrapper.cpp
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
#include <GL/glew.h>

#include "CEGUI/RendererModules/OpenGL/GL3ShaderWrapper.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/Exceptions.h"

#include <glm/gtc/type_ptr.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
OpenGL3ShaderWrapper::OpenGL3ShaderWrapper(OpenGL3Shader& shader, OpenGL3StateChangeWrapper* stateChangeWrapper)
    : d_shader(shader)
    , d_glStateChangeWrapper(stateChangeWrapper)
{
}

//----------------------------------------------------------------------------//
OpenGL3ShaderWrapper::~OpenGL3ShaderWrapper()
{
}

//----------------------------------------------------------------------------//
void OpenGL3ShaderWrapper::addUniformVariable(const std::string& uniformName)
{
    GLuint variable_location = d_shader.getUniformLocation(uniformName);

    d_uniformVariables.insert(std::pair<std::string, GLuint>(uniformName, variable_location));
}

//----------------------------------------------------------------------------//
void OpenGL3ShaderWrapper::addTextureUniformVariable(const std::string& uniformName, GLint textureUnitIndex)
{
    GLuint variable_location = d_shader.getUniformLocation(uniformName);

    d_uniformVariables.insert(std::pair<std::string, GLint>(uniformName, textureUnitIndex));

    d_shader.bind();
    glUniform1i(variable_location, textureUnitIndex);
}

//----------------------------------------------------------------------------//
void OpenGL3ShaderWrapper::addAttributeVariable(const std::string& attributeName)
{
    GLuint variable_location = d_shader.getAttribLocation(attributeName);

    if(variable_location == -1)
        CEGUI_THROW(RendererException("An attribute with the name \"" + attributeName + "\" does not exist in the shader."));

    d_attributeVariables.insert(std::pair<std::string, GLuint>(attributeName, variable_location));
}

//----------------------------------------------------------------------------//
void OpenGL3ShaderWrapper::prepareForRendering(const ShaderParameterBindings* shaderParameterBindings)
{
    d_shader.bind();

    const ShaderParameterBindings::ShaderParameterBindingsMap& shader_parameter_bindings = shaderParameterBindings->getShaderParameterBindings();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator iter = shader_parameter_bindings.begin();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator end = shader_parameter_bindings.end();

    while(iter != end)
    {
        const CEGUI::ShaderParameter* parameter = iter->second;

        if(parameter->getType() != SPT_TEXTURE)
        {
            std::map<std::string, ShaderParameter*>::iterator found_iterator = d_shaderParameterStates.find(iter->first);
            if(found_iterator != d_shaderParameterStates.end())
            {
                ShaderParameter* last_shader_parameter = found_iterator->second;
                if(parameter->equal(last_shader_parameter))
                {
                    ++iter;
                    continue;
                }
                else
                {
                    if(parameter->getType() == last_shader_parameter->getType())
                        last_shader_parameter->takeOverParameterValue(parameter);
                    else
                    {
                        delete found_iterator->second;
                        found_iterator->second = parameter->clone();
                    }
                }
            }
            else
            {
                d_shaderParameterStates[iter->first] = parameter->clone();
            }
        }

        const GLint& location = d_uniformVariables.find(iter->first)->second;


        CEGUI::ShaderParamType parameter_type = parameter->getType();

        switch(parameter_type)
        {
        case SPT_INT:
            {
                const CEGUI::ShaderParameterInt* parameterInt = static_cast<const CEGUI::ShaderParameterInt*>(parameter);
                glUniform1i(location, parameterInt->d_parameterValue);
            }
            break;
        case SPT_FLOAT:
            {
                const CEGUI::ShaderParameterFloat* parameterFloat = static_cast<const CEGUI::ShaderParameterFloat*>(parameter);
                glUniform1f(location, parameterFloat->d_parameterValue);
            }
            break;
        case SPT_MATRIX:
            {
                const CEGUI::ShaderParameterMatrix* parameterMatrix = static_cast<const CEGUI::ShaderParameterMatrix*>(parameter);
                glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(parameterMatrix->d_parameterValue));
            }
            break;
        case SPT_TEXTURE:
            {
                const CEGUI::ShaderParameterTexture* parameterTexture = static_cast<const CEGUI::ShaderParameterTexture*>(parameter);
                const CEGUI::OpenGLTexture* openglTexture = static_cast<const CEGUI::OpenGLTexture*>(parameterTexture->d_parameterValue);

                d_glStateChangeWrapper->activeTexture(location);
                d_glStateChangeWrapper->bindTexture(GL_TEXTURE_2D, openglTexture->getOpenGLTexture());
            }
            break;
        default:
            break;
        }

        ++iter;
    }
}

//----------------------------------------------------------------------------//
GLint OpenGL3ShaderWrapper::getAttributeLocation(const std::string& attributeName)
{
    std::map<std::string, GLint>::const_iterator iter = d_attributeVariables.find(attributeName);
    if(iter != d_attributeVariables.end())
    {
        return iter->second;
    }
    else
    {
        addAttributeVariable(attributeName);
        iter = d_attributeVariables.find(attributeName);
        return iter->second;
    }
}

//----------------------------------------------------------------------------//
GLint OpenGL3ShaderWrapper::getUniformLocation(const std::string& uniformName)
{
    std::map<std::string, GLint>::const_iterator iter = d_uniformVariables.find(uniformName);
    if(iter != d_uniformVariables.end())
    {
        return iter->second;
    }
    else
    {
        addUniformVariable(uniformName);
        iter = d_uniformVariables.find(uniformName);
        return iter->second;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

