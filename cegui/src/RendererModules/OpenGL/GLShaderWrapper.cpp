/***********************************************************************
    created:    23rd September 2013
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
#include "CEGUI/RendererModules/OpenGL/GLShaderWrapper.h"
#include "CEGUI/RendererModules/OpenGL/GLRenderer.h"
#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/ShaderParameterBindings.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
OpenGLShaderWrapper::OpenGLShaderWrapper()
{
}

//----------------------------------------------------------------------------//
OpenGLShaderWrapper::~OpenGLShaderWrapper()
{
}

//----------------------------------------------------------------------------//
void OpenGLShaderWrapper::prepareForRendering(const ShaderParameterBindings* shaderParameterBindings)
{
    const ShaderParameterBindings::ShaderParameterBindingsMap& shader_parameter_bindings = shaderParameterBindings->getShaderParameterBindings();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator iter = shader_parameter_bindings.begin();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator end = shader_parameter_bindings.end();

    bool is_textured = false;

    while(iter != end)
    {
        const CEGUI::ShaderParameter* parameter = iter->second;

        CEGUI::ShaderParamType parameter_type = parameter->getType();

        switch(parameter_type)
        {
        case ShaderParamType::Texture:
            {
                const CEGUI::ShaderParameterTexture* parameterTexture = static_cast<const CEGUI::ShaderParameterTexture*>(parameter);
                const CEGUI::OpenGLTexture* openglTexture = static_cast<const CEGUI::OpenGLTexture*>(parameterTexture->d_parameterValue);

                glActiveTexture(GL_TEXTURE0);
                glClientActiveTexture(GL_TEXTURE0);

                glBindTexture(GL_TEXTURE_2D, openglTexture->getOpenGLTexture());

                is_textured = true;
            }
            break;
        default:
            break;
        }

        ++iter;
    }

    if(is_textured)
    {
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
    }
    else
    {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);
    }
}

//----------------------------------------------------------------------------//
}

