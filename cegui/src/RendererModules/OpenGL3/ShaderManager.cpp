/***********************************************************************
    filename:   OpenGL3ShaderManager.cpp
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl
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

#include "CEGUI/RendererModules/OpenGL3/ShaderManager.h"

#include "CEGUI/RendererModules/OpenGL3/Shader.h"

#include "CEGUI/RendererModules/OpenGL3/StandardShaderVert.h"
#include "CEGUI/RendererModules/OpenGL3/StandardShaderFrag.h"

#include <iostream>

namespace CEGUI
{
    OpenGL3ShaderManager::OpenGL3ShaderManager()
    {
        d_shadersInitialised = false;
    }

    OpenGL3ShaderManager::~OpenGL3ShaderManager()
    {
        deinitialiseShaders();
        d_shadersInitialised = false;
    }

    OpenGL3Shader* OpenGL3ShaderManager::getShader(unsigned int id)
    {
        if(d_shaders.find(id) != d_shaders.end())
            return d_shaders[id];
        else
            return 0;
    }

    void OpenGL3ShaderManager::loadShader(unsigned int id, std::string vertexShader, std::string fragmentShader)
    {
        if(d_shaders.find(id) == d_shaders.end())
        {   
            d_shaders[id] = new OpenGL3Shader(vertexShader, fragmentShader);
            d_shaders[id]->link();
        }
    }

    bool OpenGL3ShaderManager::initialiseShaders()
    {
        if(!d_shadersInitialised)
        {
            loadShader(SHADER_ID_STANDARDSHADER, StandardShaderVert, StandardShaderFrag);


            if(!getShader(SHADER_ID_STANDARDSHADER)->isCreatedSuccessfully())
            {   
                //For CEGUI Log << "\n--- One or multiple shader programs weren't created successfully. Exiting program now. ---" << endl;
                return false;
            }

            std::cout << "Finished loading shaders.\n" << std::endl;
            return true;
        }
        return true;
    }

    void OpenGL3ShaderManager::deinitialiseShaders()
    {
        for(shaderContainerType::iterator iter = d_shaders.begin(); iter != d_shaders.end(); ++iter)
        {
            delete iter->second;
        }
        d_shaders.clear();
    }

}