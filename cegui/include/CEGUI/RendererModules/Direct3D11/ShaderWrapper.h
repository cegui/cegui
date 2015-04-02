/***********************************************************************
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
#ifndef _CEGUIDirect3D11ShaderWrapper_h_
#define _CEGUIDirect3D11ShaderWrapper_h_

#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "CEGUI/RendererModules/Direct3D11/Shader.h"
#include "CEGUI/ShaderWrapper.h"
#include "CEGUI/ShaderParameterBindings.h"
#include <string>

#include "D3D11.h"

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

//----------------------------------------------------------------------------//
struct Direct3D11ParamDesc
{
    UINT d_boundLocation;
    UINT d_boundSize;
    ShaderType d_shaderType;
    ShaderParamType d_shaderParamType;
};

//----------------------------------------------------------------------------//
class D3D11_GUIRENDERER_API Direct3D11ShaderWrapper : public ShaderWrapper
{
public:
    Direct3D11ShaderWrapper(Direct3D11Shader& shader, Direct3D11Renderer* renderer);

    ~Direct3D11ShaderWrapper();

    //Implementation of ShaderWrapper interface
    void prepareForRendering(const ShaderParameterBindings* shaderParameterBindings);

    //! Adds a shader uniform variable to the list of variables
    void addUniformVariable(const std::string& variableName, ShaderType shaderType, ShaderParamType paramType);

    //! Creates a per-object buffer for a parameter
    void createPerObjectBuffer(ShaderType shaderType);

    //! Returns the vertex shader buffer pointer
    void* getVertShaderBufferPointer() const;
    //! Returns the vertex shader buffer size
    SIZE_T getVertShaderBufferSize() const;

protected:
    //! Prepare the uniform variable mapping
    void prepareUniformVariableMapping(unsigned char*& resourceDataVS, unsigned char*& resourceDataPS);
    //! Finish the uniform variable mapping
    void finishUniformVariableMapping();

    //! The D3D DeviceContext
    ID3D11DeviceContext* d_deviceContext;
    //! The D3D Device
    ID3D11Device* d_device;

    //! The D3DBuffer used to update the uniform variables of the vertex shader
    ID3D11Buffer* d_perObjectUniformVarBufferVert;
    //! The D3DBuffer used to update the uniform variables of the pixel shader
    ID3D11Buffer* d_perObjectUniformVarBufferPixel;

    //! The underlying HLSL shader program, consisting of a vertex and fragment shader, that this class wraps the access to
    Direct3D11Shader& d_shader;

    //! A map of parameter names and the related constant variables
    std::map<std::string, Direct3D11ParamDesc> d_uniformVariables;
    //! Last states of the set shader parameters
    std::map<std::string, ShaderParameter*> d_shaderParameterStates;
};


}

#endif

