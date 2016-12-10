/***********************************************************************
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

#include <sstream>
#include <iostream>

namespace CEGUI
{
//----------------------------------------------------------------------------//
static const size_t LOG_BUFFER_SIZE = 8096;

//----------------------------------------------------------------------------//
OpenGLBaseShader::OpenGLBaseShader(const std::string& vertex_shader_source,
                             const std::string& fragment_shader_source,
                             OpenGLBaseStateChangeWrapper* glStateChanger) :
    d_glStateChanger(glStateChanger),
    d_createdSuccessfully(false),
    d_vertexShader(0),
    d_fragmentShader(0),
    d_geometryShader(0),
    d_program(0)
{
    // Compile the shaders

    d_vertexShader = compile(GL_VERTEX_SHADER, vertex_shader_source);
    if (d_vertexShader == 0)
        return;

    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);

    if(fragment_shader_source.length() > 0)
    {
        d_fragmentShader = compile(GL_FRAGMENT_SHADER, fragment_shader_source);

        if (d_fragmentShader == 0)
            return;
    }

    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);

    d_program = glCreateProgram();
}

//----------------------------------------------------------------------------//
OpenGLBaseShader::~OpenGLBaseShader()
{
    if(d_program != 0)
        glDeleteProgram(d_program);
    if(d_vertexShader != 0)
        glDeleteShader(d_vertexShader);
    if(d_fragmentShader != 0)
        glDeleteShader(d_fragmentShader);
    if(d_geometryShader != 0)
        glDeleteShader(d_geometryShader);
}

//----------------------------------------------------------------------------//
void OpenGLBaseShader::bind() const
{
    d_glStateChanger->useProgram(d_program);
}

//----------------------------------------------------------------------------//
GLint OpenGLBaseShader::getAttribLocation(const std::string &name) const
{
    return glGetAttribLocation(d_program, name.c_str());
}

//----------------------------------------------------------------------------//
GLint OpenGLBaseShader::getUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(d_program, name.c_str());
}

//----------------------------------------------------------------------------//
void OpenGLBaseShader::bindFragDataLocation(const std::string& /*name*/)
{
    if(d_program > 0)
    {
        link();
    }
}

//----------------------------------------------------------------------------//
bool OpenGLBaseShader::isCreatedSuccessfully()
{
    return d_createdSuccessfully;
}

//----------------------------------------------------------------------------//
GLuint OpenGLBaseShader::compile(GLuint type, const std::string &source)
{
    // Create shader object
    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);
    GLuint shader = glCreateShader(type);

    if (shader == 0)
    {
        std::stringstream stringStream;
        stringStream << "Critical Error - Could not create shader object of type:" << type << ".";
        throw RendererException(stringStream.str().c_str());
        return 0;
    }

    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);

    // Define shader source and compile

    const char* src = source.data();
    int len = source.size();

    glShaderSource(shader, 1, &src, &len);

    glCompileShader(shader);

    // Check for errors
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE)
    {
        outputShaderLog(shader);
        return 0;
    }

    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);

    return shader;
}

//----------------------------------------------------------------------------//
void OpenGLBaseShader::link()
{

    // Attach shaders and link
    glAttachShader(d_program, d_vertexShader);

    if(d_geometryShader != 0)
        glAttachShader(d_program, d_geometryShader);

    if(d_fragmentShader !=0)
        glAttachShader(d_program, d_fragmentShader);

    glLinkProgram(d_program);

    // Check for problems

    GLint status;
    glGetProgramiv(d_program, GL_LINK_STATUS, &status);

    if (status != GL_TRUE)
    {
        outputProgramLog(d_program);

        glDeleteProgram(d_program);
        d_program = 0;
    }

    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);

    if (d_program == 0)
        return;

    d_createdSuccessfully = true;
    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);


}

//----------------------------------------------------------------------------//
void OpenGLBaseShader::outputProgramLog(GLuint program)
{
    char logBuffer[LOG_BUFFER_SIZE];
    GLsizei length;

    logBuffer[0] = '\0';
    glGetProgramInfoLog(program, LOG_BUFFER_SIZE, &length, logBuffer);

    if (length > 0)
    {
        std::stringstream sstream;
        sstream << "OpenGLBaseShader linking has failed.\n" << logBuffer;
        throw RendererException(sstream.str().c_str());
    }
}

//----------------------------------------------------------------------------//
void OpenGLBaseShader::outputShaderLog(GLuint shader)
{
    char logBuffer[LOG_BUFFER_SIZE];
    GLsizei length;

    logBuffer[0] = '\0';
    glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, &length, logBuffer);

    if (length > 0)
    {
        std::stringstream ss;
        ss << "OpenGLBaseShader compilation has failed.\n" << logBuffer;
          throw RendererException(ss.str().c_str());
    }
}

//----------------------------------------------------------------------------//

}
