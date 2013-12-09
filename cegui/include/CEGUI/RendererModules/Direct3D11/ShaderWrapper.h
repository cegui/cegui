/***********************************************************************
    filename:   ShaderWrapper.h
    created:    18th July 2013
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIDirect3D11ShaderWrapper_h_
#define _CEGUIDirect3D11ShaderWrapper_h_

#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "CEGUI/ShaderWrapper.h"
#include <string>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    class ShaderParameterBindings;
    class ShaderParameter;
    class Direct3D11Shader;

class D3D11_GUIRENDERER_API Direct3D11ShaderWrapper : public ShaderWrapper
{
public:
    Direct3D11ShaderWrapper(Direct3D11Shader& shader);

    ~Direct3D11ShaderWrapper();

    //Implementation of ShaderWrapper interface
    void prepareForRendering(const ShaderParameterBindings* shaderParameterBindings);

    //! Adds a shader attribute variable to the list of variables
    void addAttributeVariable(const std::string& attributeName);
    //! Adds a shader uniform variable to the list of variables
    void addUniformVariable(const std::string& uniformName);
    //! Adds a shader attribute variable to the list of variables
    void addTextureUniformVariable(const std::string& uniformName, int textureUnitIndex);

    int getAttributeLocation(const std::string& attributeName);
    int getUniformLocation(const std::string& uniformName);

protected:
    //! The underlying GLSL shader that this class wraps the access to
    Direct3D11Shader& d_shader;
    //! A map of parameter names and the related uniform variable locations
    std::map<std::string, int> d_uniformVariables;
    //! A map of parameter names and the related attribute variable locations
    std::map<std::string, int> d_attributeVariables;
    //! Last states of the set shader parameters
    std::map<std::string, ShaderParameter*> d_shaderParameterStates;
};


}

#endif

