/***********************************************************************
    filename:   ShaderWrapper.cpp
    created:    9th December 2013
    author:     Lukas Meindl
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
    : d_shader(shader)
    , d_device(renderer->getDirect3DDevice())
    , d_deviceContext(renderer->getDirect3DDeviceContext())
    , d_perObjectUniformVarBufferVert(0)
    , d_perObjectUniformVarBufferPixel(0)
{
    createPerObjectBuffer(ST_VERTEX);
    createPerObjectBuffer(ST_PIXEL);




        D3D11_SAMPLER_DESC samplerDescription;
    ZeroMemory( &samplerDescription, sizeof(samplerDescription) );
    samplerDescription.Filter =         D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDescription.AddressU =       D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.AddressV =       D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.AddressW =       D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerDescription.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerDescription.MinLOD =         0;
    samplerDescription.MaxLOD =         D3D11_FLOAT32_MAX;

    HRESULT result = d_device->CreateSamplerState( &samplerDescription, &d_samplerState );
    if(FAILED(result))
        CEGUI_THROW(RendererException("SamplerDescription creation failed"));
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

    if(paramType == SPT_TEXTURE)
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

    d_deviceContext->PSSetSamplers(0, 1, &d_samplerState);

    D3D11_MAPPED_SUBRESOURCE mappedResourceVertShader, mappedResourcePixelShader;
    prepareUniformVariableMapping(mappedResourceVertShader, mappedResourcePixelShader);

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
            CEGUI_THROW(RendererException(errorMessage));
        }

        const Direct3D11ParamDesc& parameterDescription = foundIter->second;

        if(parameterType != parameterDescription.d_shaderParamType)
        {
            std::string errorMessage = std::string("Supplied parameter type doesn't match the shader parameter type for variable \"") + iter->first + "\"";
            CEGUI_THROW(RendererException(errorMessage));
        }

        std::map<std::string, ShaderParameter*>::iterator found_iterator = d_shaderParameterStates.find(iter->first);
        if(found_iterator != d_shaderParameterStates.end())
        {
            ShaderParameter* last_shader_parameter = found_iterator->second;
                            
            if(last_shader_parameter->getType() != parameterDescription.d_shaderParamType)
            {
                std::string errorMessage = std::string("The last shader parameter type doesn't match the shader parameter type for variable \"") + iter->first + "\"";
                CEGUI_THROW(RendererException(errorMessage));
            }

            if(parameter->equal(last_shader_parameter))
            {
                ++iter;
                continue;
            }
            else
                last_shader_parameter->takeOverParameterValue(parameter);
        }



        switch(parameterType)
        {
        case SPT_INT:
            {
/*
                const CEGUI::ShaderParameterInt* parameterInt = static_cast<const CEGUI::ShaderParameterInt*>(parameter);
                glUniform1i(location, parameterInt->d_parameterValue);*/
            }
            break;
        case SPT_FLOAT:
            {
/*
                const CEGUI::ShaderParameterFloat* parameterFloat = static_cast<const CEGUI::ShaderParameterFloat*>(parameter);
                glUniform1f(location, parameterFloat->d_parameterValue);*/
            }
            break;
        case SPT_MATRIX_4X4:
            {
                const CEGUI::ShaderParameterMatrix* parameterMatrix = static_cast<const CEGUI::ShaderParameterMatrix*>(parameter);
                



                // Get the pointer to the mapped resource data
                unsigned char* dataInBytes = 0;
                if (parameterDescription.d_shaderType == ST_VERTEX)
                    dataInBytes = static_cast<unsigned char*>(mappedResourceVertShader.pData);
                else if (parameterDescription.d_shaderType == ST_PIXEL)
                    dataInBytes = static_cast<unsigned char*>(mappedResourcePixelShader.pData);

                // Go to the memory position of our uniform variable
                dataInBytes += parameterDescription.d_boundLocation;
                memcpy(dataInBytes, glm::value_ptr(parameterMatrix->d_parameterValue), parameterDescription.d_boundSize);
            }
            break;
        case SPT_TEXTURE:
            {
                const CEGUI::ShaderParameterTexture* parameterTexture = static_cast<const CEGUI::ShaderParameterTexture*>(parameter);
                const CEGUI::Direct3D11Texture* texture = static_cast<const CEGUI::Direct3D11Texture*>(parameterTexture->d_parameterValue);

                ID3D11ShaderResourceView* shaderResourceView = texture->getDirect3DShaderResourceView();

                if (parameterDescription.d_shaderType == ST_PIXEL)
                    d_deviceContext->PSSetShaderResources(parameterDescription.d_boundLocation, 1, &shaderResourceView);
                else if (parameterDescription.d_shaderType == ST_VERTEX)
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
void* Direct3D11ShaderWrapper::getVertShaderBufferPointer()
{
    ID3D10Blob* vertexShaderBuffer = d_shader.getVertexShaderBuffer();

    return vertexShaderBuffer->GetBufferPointer();
}

//----------------------------------------------------------------------------//
SIZE_T Direct3D11ShaderWrapper::getVertShaderBufferSize()
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
        
        bufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDescription.Usage = D3D11_USAGE_DYNAMIC;
        bufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDescription.MiscFlags = 0;
        bufferDescription.ByteWidth = shaderBufferDesc.Size;
    
        if (shaderType == ST_VERTEX)
            d_device->CreateBuffer(&bufferDescription, nullptr, &d_perObjectUniformVarBufferVert);
        else if (shaderType == ST_PIXEL)
            d_device->CreateBuffer(&bufferDescription, nullptr, &d_perObjectUniformVarBufferPixel);
    }
}

//----------------------------------------------------------------------------//
void Direct3D11ShaderWrapper::prepareUniformVariableMapping(D3D11_MAPPED_SUBRESOURCE& mappedResourceVertShader, D3D11_MAPPED_SUBRESOURCE& mappedResourcePixelShader)
{
    if(d_perObjectUniformVarBufferVert != 0)
    {
        d_deviceContext->VSSetConstantBuffers(0, 1, &d_perObjectUniformVarBufferVert);

        HRESULT result = d_deviceContext->Map(d_perObjectUniformVarBufferVert, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourceVertShader);
        if(FAILED(result))
            CEGUI_THROW(RendererException("Failed to map constant shader buffer.\n"));
    }
    if(d_perObjectUniformVarBufferPixel != 0)
    {
        d_deviceContext->PSSetConstantBuffers(0, 1, &d_perObjectUniformVarBufferPixel);

        HRESULT result = d_deviceContext->Map(d_perObjectUniformVarBufferPixel, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResourcePixelShader);
        if(FAILED(result))
            CEGUI_THROW(RendererException("Failed to map constant shader buffer.\n"));
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

