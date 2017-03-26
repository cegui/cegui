/***********************************************************************
    created:    Fri, 4th July 2014
    author:     Henri I Hyyryläinen
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/RendererModules/Ogre/ShaderWrapper.h"
#include "CEGUI/RendererModules/Ogre/Texture.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/Exceptions.h"

#include <glm/gtc/type_ptr.hpp>
#include "OgreRenderSystem.h"
#include "CEGUI/Logger.h"

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
OgreShaderWrapper::OgreShaderWrapper(OgreRenderer& owner,
  Ogre::RenderSystem& rs, Ogre::HighLevelGpuProgramPtr vs,
  Ogre::HighLevelGpuProgramPtr ps)
    : d_owner(owner),
      d_renderSystem(rs),
      d_vertexShader(vs),
      d_pixelShader(ps),
      d_lastMatrix(),
      d_previousAlpha(-1.f)
{
    d_vertexParameters = d_vertexShader->createParameters();
    d_pixelParameters = d_pixelShader->createParameters();

    const Ogre::GpuConstantDefinitionMap& vertex_map = 
        d_vertexShader->getConstantDefinitions().map;

    Ogre::GpuConstantDefinitionMap::const_iterator target = 
        vertex_map.find("modelViewProjMatrix");

    const Ogre::GpuConstantDefinitionMap& pixel_map = 
        d_pixelShader->getConstantDefinitions().map;

    Ogre::GpuConstantDefinitionMap::const_iterator target2 = 
        pixel_map.find("alphaPercentage");

    // We will throw an error if shaders/parameter names are invalid
    if (target == vertex_map.end() || target2 == pixel_map.end())
    {
        throw RendererException("Ogre renderer couldn't find an index for"
            " the shader data.");

        return;
    }

    d_paramTypeToIndex[static_cast<int>(ShaderParamType::Matrix4X4)] =
        target->second.physicalIndex;
    d_paramTypeToIndex[static_cast<int>(ShaderParamType::Float)] =
        target2->second.physicalIndex;
    d_paramTypeToIndex[static_cast<int>(ShaderParamType::Texture)] = 0;
}

//----------------------------------------------------------------------------//
OgreShaderWrapper::~OgreShaderWrapper()
{
    d_pixelParameters.setNull();
    d_vertexParameters.setNull();
    d_vertexShader.setNull();
    d_pixelShader.setNull();
}

//----------------------------------------------------------------------------//
Ogre::GpuProgramParametersSharedPtr OgreShaderWrapper::getVertexParameters() const
{
    return d_vertexParameters;
}

//----------------------------------------------------------------------------//
void OgreShaderWrapper::prepareForRendering(const ShaderParameterBindings* 
    shaderParameterBindings)
{
    Ogre::GpuProgram* vs = d_vertexShader->_getBindingDelegate();
    d_renderSystem.bindGpuProgram(vs);

    Ogre::GpuProgram* ps = d_pixelShader->_getBindingDelegate();
    d_renderSystem.bindGpuProgram(ps);

    const ShaderParameterBindings::ShaderParameterBindingsMap& 
        shader_parameter_bindings = shaderParameterBindings->
            getShaderParameterBindings();

    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator iter = 
        shader_parameter_bindings.begin();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator end = 
        shader_parameter_bindings.end();
    
    for (; iter != end; ++iter)
    {
        const CEGUI::ShaderParameter* parameter = iter->second;
        const ShaderParamType parameterType = parameter->getType();

        std::map<int, size_t>::const_iterator find_iter = d_paramTypeToIndex.
            find(static_cast<int>(parameterType));

        if (find_iter == d_paramTypeToIndex.end())
        {
            std::string errorMessage = std::string("Unknown variable name: \"")+
            iter->first + "\"";
            throw RendererException(errorMessage);
        }

        size_t target_index = find_iter->second;

        switch (parameterType)
        {
        case ShaderParamType::Texture:
        {
            const CEGUI::ShaderParameterTexture* parameterTexture = 
                static_cast<const CEGUI::ShaderParameterTexture*>(parameter);

            const CEGUI::OgreTexture* texture = static_cast<const 
                CEGUI::OgreTexture*>(parameterTexture->d_parameterValue);

            Ogre::TexturePtr actual_texture = texture->getOgreTexture();

            if (actual_texture.isNull())
            {
                throw RendererException("Ogre texture ptr is empty");
            }

            d_renderSystem._setTexture(0, true, actual_texture);
            d_owner.initialiseTextureStates();

            break;
        }
        case ShaderParamType::Matrix4X4:
        {
            // This is the "modelViewProjMatrix"
            const CEGUI::ShaderParameterMatrix* mat = static_cast<const 
                CEGUI::ShaderParameterMatrix*>(parameter);

            if (d_lastMatrix != mat->d_parameterValue)
            {
                d_vertexParameters->_writeRawConstants(target_index, 
                                                       glm::value_ptr(mat->d_parameterValue),
                                                       16);
                d_lastMatrix = mat->d_parameterValue;
            } 
            break;
        }
        case ShaderParamType::Float:
        {
            // This is the alpha value
            const CEGUI::ShaderParameterFloat* new_alpha = static_cast<const 
                CEGUI::ShaderParameterFloat*>(parameter);

            if (d_previousAlpha != new_alpha->d_parameterValue)
            {
                d_previousAlpha = new_alpha->d_parameterValue;

                d_pixelParameters->_writeRawConstants(target_index, 
                    &d_previousAlpha, 1);
            }
            
            break;
        }
        default:
            throw RendererException("Invalid parameter type");
        }
    }

    // Pass the finalized parameters to Ogre
    d_renderSystem.bindGpuProgramParameters(Ogre::GPT_VERTEX_PROGRAM, 
        d_vertexParameters, Ogre::GPV_ALL);
    d_renderSystem.bindGpuProgramParameters(Ogre::GPT_FRAGMENT_PROGRAM, 
        d_pixelParameters, Ogre::GPV_ALL);
}

//----------------------------------------------------------------------------//
}

