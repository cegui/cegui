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

#include "CEGUI/RendererModules/Direct3D11/ShaderWrapper.h"
#include "CEGUI/RendererModules/Direct3D11/Texture.h"
#include "CEGUI/ShaderParameterBindings.h"
#include "CEGUI/Exceptions.h"

#include <glm/gtc/type_ptr.hpp>

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
Direct3D11ShaderWrapper::Direct3D11ShaderWrapper(Direct3D11Shader& shader,
                                                 Direct3D11Renderer* renderer)
    : d_deviceContext(renderer->getDirect3DDeviceContext())
    , d_device(renderer->getDirect3DDevice())
    , d_perObjectUniformVarBufferVert(0)
    , d_perObjectUniformVarBufferPixel(0)
    , d_shader(shader)
{
    createPerObjectBuffer(ShaderType::VERTEX);
    createPerObjectBuffer(ShaderType::PIXEL);
}

//----------------------------------------------------------------------------//
Direct3D11ShaderWrapper::~Direct3D11ShaderWrapper()
{
    if(d_perObjectUniformVarBufferVert != 0)
        d_perObjectUniformVarBufferVert->Release();

    if(d_perObjectUniformVarBufferPixel != 0)
        d_perObjectUniformVarBufferPixel->Release();
}

//----------------------------------------------------------------------------//
void Direct3D11ShaderWrapper::addUniformVariable(const std::string& variableName, ShaderType shaderType, ShaderParamType paramType)
{
    UINT variableBindingLoc = -1;
    UINT size = -1;

    if(paramType == ShaderParamType::Texture)
    {
        D3D11_SHADER_INPUT_BIND_DESC variableDesc = d_shader.getTextureBindingDesc(variableName, shaderType);
        variableBindingLoc = variableDesc.BindPoint;
        size = variableDesc.BindCount;
    }
    else
    {
        D3D11_SHADER_VARIABLE_DESC variableDesc = d_shader.getUniformVariableDescription(variableName, shaderType);
        variableBindingLoc = variableDesc.StartOffset;
        size = variableDesc.Size;
    }

    Direct3D11ParamDesc parameter = {variableBindingLoc, size, shaderType, paramType};

    d_uniformVariables.insert(std::pair<std::string, Direct3D11ParamDesc>(variableName, parameter));
}

//----------------------------------------------------------------------------//
void Direct3D11ShaderWrapper::prepareForRendering(const ShaderParameterBindings* shaderParameterBindings)
{
    d_shader.bind();

    // The pointers to the mapped resource data
    unsigned char* resourceDataVS = 0;
    unsigned char* resourceDataPS = 0;
    prepareUniformVariableMapping(resourceDataVS, resourceDataPS);

    const ShaderParameterBindings::ShaderParameterBindingsMap& shader_parameter_bindings = shaderParameterBindings->getShaderParameterBindings();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator iter = shader_parameter_bindings.begin();
    ShaderParameterBindings::ShaderParameterBindingsMap::const_iterator end = shader_parameter_bindings.end();

    while(iter != end)
    {
        const CEGUI::ShaderParameter* parameter = iter->second;
        const ShaderParamType parameterType = parameter->getType();

        std::map<std::string, Direct3D11ParamDesc>::iterator foundIter = d_uniformVariables.find(iter->first);
        if(foundIter == d_uniformVariables.end())
        {
            std::string errorMessage = std::string("Variable was not found in the set of uniform variables of the shader. Variable name was: \"") + iter->first + "\"";
            throw RendererException(errorMessage);
        }

        const Direct3D11ParamDesc& parameterDescription = foundIter->second;

        if(parameterType != parameterDescription.d_shaderParamType)
        {
            std::string errorMessage = std::string("Supplied parameter type doesn't match the shader parameter type for variable \"") + iter->first + "\"";
            throw RendererException(errorMessage);
        }

        std::map<std::string, ShaderParameter*>::iterator found_iterator = d_shaderParameterStates.find(iter->first);
        if(found_iterator != d_shaderParameterStates.end())
        {
            ShaderParameter* last_shader_parameter = found_iterator->second;
                            
            if(last_shader_parameter->getType() != parameterDescription.d_shaderParamType)
            {
                std::string errorMessage = std::string("The last shader parameter type doesn't match the shader parameter type for variable \"") + iter->first + "\"";
                throw RendererException(errorMessage);
            }

            if(parameter->equal(last_shader_parameter))
            {
                ++iter;
                continue;
            }
            else
                last_shader_parameter->takeOverParameterValue(parameter);
        }

        // Get the data pointer pointing to the memory position of our current uniform variable
        unsigned char* paramDataPointer = 0;
        if (parameterDescription.d_shaderType == ShaderType::VERTEX)
            paramDataPointer = resourceDataVS + parameterDescription.d_boundLocation;
        else if (parameterDescription.d_shaderType == ShaderType::PIXEL)
            paramDataPointer = resourceDataPS + parameterDescription.d_boundLocation;

        switch(parameterType)
        {
        case ShaderParamType::Int:
            {
                const CEGUI::ShaderParameterInt* parameterInt = static_cast<const CEGUI::ShaderParameterInt*>(parameter);

                //Copy the data
                memcpy(paramDataPointer, &(parameterInt->d_parameterValue), parameterDescription.d_boundSize);
            }
            break;
        case ShaderParamType::Float:
            {
                const CEGUI::ShaderParameterFloat* parameterFloat = static_cast<const CEGUI::ShaderParameterFloat*>(parameter);

                //Copy the data
                memcpy(paramDataPointer, &(parameterFloat->d_parameterValue), parameterDescription.d_boundSize);
            }
            break;
        case ShaderParamType::Matrix4X4:
            {
                const CEGUI::ShaderParameterMatrix* parameterMatrix = static_cast<const CEGUI::ShaderParameterMatrix*>(parameter);

                //Copy the data
                memcpy(paramDataPointer, glm::value_ptr(parameterMatrix->d_parameterValue), parameterDescription.d_boundSize);
            }
            break;
        case ShaderParamType::Texture:
            {
                const CEGUI::ShaderParameterTexture* parameterTexture = static_cast<const CEGUI::ShaderParameterTexture*>(parameter);
                const CEGUI::Direct3D11Texture* texture = static_cast<const CEGUI::Direct3D11Texture*>(parameterTexture->d_parameterValue);

                ID3D11ShaderResourceView* shaderResourceView = texture->getDirect3DShaderResourceView();

                if (parameterDescription.d_shaderType == ShaderType::PIXEL)
                    d_deviceContext->PSSetShaderResources(parameterDescription.d_boundLocation, 1, &shaderResourceView);
                else if (parameterDescription.d_shaderType == ShaderType::VERTEX)
                    d_deviceContext->VSSetShaderResources(parameterDescription.d_boundLocation, 1, &shaderResourceView);
            }
            break;
        default:
            break;
        }

        ++iter; 
    }

    finishUniformVariableMapping();
}

//----------------------------------------------------------------------------//
void* Direct3D11ShaderWrapper::getVertShaderBufferPointer() const
{
    ID3D10Blob* vertexShaderBuffer = d_shader.getVertexShaderBuffer();

    return vertexShaderBuffer->GetBufferPointer();
}

//----------------------------------------------------------------------------//
SIZE_T Direct3D11ShaderWrapper::getVertShaderBufferSize() const
{
    ID3D10Blob* vertexShaderBuffer = d_shader.getVertexShaderBuffer();

    return vertexShaderBuffer->GetBufferSize();
}

//----------------------------------------------------------------------------//
void Direct3D11ShaderWrapper::createPerObjectBuffer(ShaderType shaderType)
{
    HRESULT result;

    ID3D11ShaderReflectionConstantBuffer* shaderReflectionConstBuff = d_shader.getShaderReflectionConstBuffer(shaderType);

    D3D11_SHADER_BUFFER_DESC shaderBufferDesc;
    result = shaderReflectionConstBuff->GetDesc(&shaderBufferDesc);
    if (FAILED(result))
        return;

    if(shaderBufferDesc.Size > 0)
    {
        // create the constant buffers
        D3D11_BUFFER_DESC bufferDescription;
        ZeroMemory(&bufferDescription, sizeof(bufferDescription));
        
        bufferDescription.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        bufferDescription.Usage          = D3D11_USAGE_DYNAMIC;
        bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDescription.MiscFlags      = 0;
        bufferDescription.ByteWidth      = shaderBufferDesc.Size;
    
        if (shaderType == ShaderType::VERTEX)
            d_device->CreateBuffer(&bufferDescription, 0, &d_perObjectUniformVarBufferVert);
        else if (shaderType == ShaderType::PIXEL)
            d_device->CreateBuffer(&bufferDescription, 0, &d_perObjectUniformVarBufferPixel);
    }
}

//----------------------------------------------------------------------------//
void Direct3D11ShaderWrapper::prepareUniformVariableMapping(unsigned char*& resourceDataVS, unsigned char*& resourceDataPS)
{
    if(d_perObjectUniformVarBufferVert != 0)
    {
        d_deviceContext->VSSetConstantBuffers(0, 1, &d_perObjectUniformVarBufferVert);

        D3D11_MAPPED_SUBRESOURCE mappedResourceVertShader;
        HRESULT result = d_deviceContext->Map(d_perObjectUniformVarBufferVert, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVertShader);
        if(FAILED(result))
            throw RendererException("Failed to map constant shader buffer.\n");

        resourceDataVS = static_cast<unsigned char*>(mappedResourceVertShader.pData);
    }
    if(d_perObjectUniformVarBufferPixel != 0)
    {
        d_deviceContext->PSSetConstantBuffers(0, 1, &d_perObjectUniformVarBufferPixel);

        D3D11_MAPPED_SUBRESOURCE mappedResourcePixelShader;
        HRESULT result = d_deviceContext->Map(d_perObjectUniformVarBufferPixel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourcePixelShader);
        if(FAILED(result))
            throw RendererException("Failed to map constant shader buffer.\n");

        resourceDataPS = static_cast<unsigned char*>(mappedResourcePixelShader.pData);
    }
}

//----------------------------------------------------------------------------//
void Direct3D11ShaderWrapper::finishUniformVariableMapping()
{
    if(d_perObjectUniformVarBufferVert != 0)
        d_deviceContext->Unmap(d_perObjectUniformVarBufferVert, 0);
    if(d_perObjectUniformVarBufferPixel != 0)
        d_deviceContext->Unmap(d_perObjectUniformVarBufferPixel, 0);
}

//----------------------------------------------------------------------------//
}

