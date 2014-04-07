/***********************************************************************
    filename:   Shader.h
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
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
#ifndef _CEGUIDirect3D11Shader_h_
#define _CEGUIDirect3D11Shader_h_

#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "CEGUI/Exceptions.h"

#include "D3Dcompiler.h"
#include <string>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
/*!
\brief Enumerated type used when specifying the parameters for one of the shaders

 */
enum ShaderType
{
    /**
     * The vertex shader is associated with the parameter
     */
    ST_VERTEX,
    /**
     * The pixel shader is associated with the parameter
     */
    ST_PIXEL
};
//----------------------------------------------------------------------------//

class D3D11_GUIRENDERER_API Direct3D11Shader :
    public AllocatedObject<Direct3D11Shader>
{
public:

    /*!
    \brief
        Creates and loads shader programs from the two strings supplied to it
    */
    Direct3D11Shader(Direct3D11Renderer& owner,
                     const std::string& vertexShaderSource,
                     const std::string& pixelShaderSource);

    ~Direct3D11Shader();

    /*!
    \brief
        Bind the shader
    */
    void bind() const;

    //! Returns the binding location of a texture resource variable in the shader
    D3D11_SHADER_INPUT_BIND_DESC getTextureBindingDesc(const std::string& variableName, ShaderType shaderType);
    //! Returns the binding location of a uniform (constant) variable in the shader
    D3D11_SHADER_VARIABLE_DESC getUniformVariableDescription(const std::string& variableName, ShaderType shaderType);

    ID3D10Blob* getVertexShaderBuffer() const;

    ID3D11ShaderReflectionConstantBuffer* getShaderReflectionConstBuffer(ShaderType shaderType);

private:
    //! Creates the vertex shader from a shader source code string
    void createVertexShader(const std::string& vertexShaderSource);
    //! Creates the pixel shader from a shader source code string
    void createPixelShader(const std::string& pixelShaderSource);

    //! The D3D Device
    ID3D11Device* d_device;
    //! The D3D DeviceContext
    ID3D11DeviceContext* d_deviceContext;

    //! The D3D VertexShader of this shader program
    ID3D11VertexShader* d_vertShader;
    //! The D3D Vertex Shader Buffer
    mutable ID3D10Blob* d_vertexShaderBuffer;
    //! The D3D VertexShader ShaderReflection of this shader program
    ID3D11ShaderReflection* d_vertexShaderReflection;
    //! The D3D PixelShader of this shader program
    ID3D11PixelShader* d_pixelShader;
    //! The D3D Vertex Shader Buffer
    mutable ID3D10Blob* d_pixelShaderBuffer;
    //! The D3D PixelShader ShaderReflection of this shader program
    ID3D11ShaderReflection* d_pixelShaderReflection;
};

}

#endif

