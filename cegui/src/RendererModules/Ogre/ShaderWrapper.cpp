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

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
OgreShaderWrapper::OgreShaderWrapper(OgreRenderer& owner, 
    Ogre::RenderSystem& rs, 
    Ogre::HighLevelGpuProgramPtr vs, Ogre::HighLevelGpuProgramPtr ps)
    : d_vertexShader(vs),
    d_pixelShader(ps),
    d_owner(owner),
    d_renderSystem(rs)
{
    d_vertexParameters = d_vertexShader->createParameters();
    d_pixelParameters = d_pixelShader->createParameters();
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
void OgreShaderWrapper::prepareForRendering(const ShaderParameterBindings* shaderParameterBindings)
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

    while(iter != end)
    {
        const CEGUI::ShaderParameter* parameter = iter->second;
        const ShaderParamType parameterType = parameter->getType();


        if (iter->first == "texture0")
        {
            // Texture always goes to the pixel shader

            if (parameterType != SPT_TEXTURE)
            {
                CEGUI_THROW(RendererException("Invalid parameter type"));
            }

            const CEGUI::ShaderParameterTexture* parameterTexture = 
                static_cast<const CEGUI::ShaderParameterTexture*>(parameter);

            const CEGUI::OgreTexture* texture = static_cast<const 
                CEGUI::OgreTexture*>(parameterTexture->d_parameterValue);


            d_renderSystem._setTexture(0, true, texture->getOgreTexture());

        } 
        else if (iter->first == "modelViewPerspMatrix")
        {
            // Transform matrix goes to the vertex shader

            if (parameterType != SPT_MATRIX_4X4)
            {

                CEGUI_THROW(RendererException("Invalid parameter type"));
            }

            const CEGUI::ShaderParameterMatrix* mat = static_cast<const 
                CEGUI::ShaderParameterMatrix*>(parameter);
            
            // The cast is here to prevent VERY long error messages about not
            // matching any signatures if the d_parameterValue's type 
            // changes in the future
            Ogre::Matrix4 backconverted;

            OgreRenderer::convertGLMMatrixToOgreMatrix(mat->d_parameterValue,
                backconverted);

            d_vertexParameters->setNamedConstant("worldViewProjMatrix", 
                backconverted);

        } 
        else
        {
            std::string errorMessage = std::string("Unknown variable name: \"")+
                iter->first + "\"";
            CEGUI_THROW(RendererException(errorMessage));
        }

        // TODO add support for all types
        //SPT_INT
        //SPT_FLOAT
        //SPT_MATRIX_4X4
        //SPT_TEXTURE

        ++iter; 
    }


    // Pass the finalized parameters to Ogre
    d_renderSystem.bindGpuProgramParameters(Ogre::GPT_VERTEX_PROGRAM, 
        d_vertexParameters, Ogre::GPV_ALL);
    d_renderSystem.bindGpuProgramParameters(Ogre::GPT_FRAGMENT_PROGRAM, 
        d_pixelParameters, Ogre::GPV_ALL);
}

//----------------------------------------------------------------------------//
}

