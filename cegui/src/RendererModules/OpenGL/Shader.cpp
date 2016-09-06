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
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"

#include <sstream>
#include <iostream>

namespace CEGUI
{
//----------------------------------------------------------------------------//
static const size_t LOG_BUFFER_SIZE = 8096;

//----------------------------------------------------------------------------//
OpenGL3Shader::OpenGL3Shader(const std::string& vertex_shader_source,
                             const std::string& fragment_shader_source) :
    d_createdSucessfully(false),
    d_vertexShader(0),
    d_fragmentShader(0),
    d_geometryShader(0),
    d_program(0)
{
    // Compile the shaders

    d_vertexShader = compile(GL_VERTEX_SHADER, vertex_shader_source);
    if (d_vertexShader == 0)
        return;

    checkGLErrors();

    if(fragment_shader_source.length() > 0)
    {
        d_fragmentShader = compile(GL_FRAGMENT_SHADER, fragment_shader_source);

        if (d_fragmentShader == 0)
            return;
    }

    checkGLErrors();

    d_program = glCreateProgram();
}

//----------------------------------------------------------------------------//
OpenGL3Shader::~OpenGL3Shader()
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
void OpenGL3Shader::bind() const
{
    glUseProgram(d_program);
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::unbind() const
{
    glUseProgram(0);
}

//----------------------------------------------------------------------------//
GLuint OpenGL3Shader::getAttribLocation(const std::string &name) const
{
    return glGetAttribLocation(d_program, name.c_str());
}

//----------------------------------------------------------------------------//
GLuint OpenGL3Shader::getUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(d_program, name.c_str());
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::bindFragDataLocation(const std::string &name)
{
    if(d_program > 0)
    {
        glBindFragDataLocation(d_program, 0, name.c_str());
        link();
    }
}

//----------------------------------------------------------------------------//
bool OpenGL3Shader::isCreatedSuccessfully()
{
    return d_createdSucessfully;
}

//----------------------------------------------------------------------------//
GLuint OpenGL3Shader::compile(GLuint type, const std::string &source)
{
    // Create shader object
    checkGLErrors();
    GLuint shader = glCreateShader(type);

    if (shader == 0)
    {
        std::stringstream stringStream;
        stringStream << "Critical Error - Could not create shader object of type:" << type << ".";
        CEGUI_THROW(RendererException(stringStream.str().c_str()));
        return 0;
    }

    checkGLErrors();

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

    checkGLErrors();

    return shader;
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::link()
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

    checkGLErrors();

    if (d_program == 0)
        return;

    d_createdSucessfully = true;
    checkGLErrors();

    if (OpenGLInfo::getSingleton().isUsingDesktopOpengl())
    {
        glBindFragDataLocation(d_program, 0, "out0"); // GL_COLOR_ATTACHMENT0
        glBindFragDataLocation(d_program, 1, "out1"); // GL_COLOR_ATTACHMENT1
        glBindFragDataLocation(d_program, 2, "out2"); // ...
        glBindFragDataLocation(d_program, 3, "out3");
        glBindFragDataLocation(d_program, 4, "out4");
        glBindFragDataLocation(d_program, 5, "out5");
        glBindFragDataLocation(d_program, 6, "out6");
        glBindFragDataLocation(d_program, 7, "out7");
    }
    checkGLErrors();
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::outputProgramLog(GLuint program)
{
    char logBuffer[LOG_BUFFER_SIZE];
    GLsizei length;

    logBuffer[0] = '\0';
    glGetProgramInfoLog(program, LOG_BUFFER_SIZE, &length, logBuffer);

    if (length > 0)
    {
        std::stringstream sstream;
        sstream << "OpenGL3Shader linking has failed.\n" << logBuffer;
        CEGUI_THROW(RendererException(sstream.str().c_str()));
    }
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::outputShaderLog(GLuint shader)
{
    char logBuffer[LOG_BUFFER_SIZE];
    GLsizei length;

    logBuffer[0] = '\0';
    glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, &length, logBuffer);

    if (length > 0)
    {
        std::stringstream ss;
        ss << "OpenGL3Shader compilation has failed.\n" << logBuffer;
          CEGUI_THROW(RendererException(ss.str().c_str()));
    }
}

//----------------------------------------------------------------------------//
void getGLErrors(const char *location)
{
    GLenum error = glGetError();

    if (error != GL_NO_ERROR)
    {
        std::stringstream stringStream;
        stringStream << "OpenGL3Renderer: Notification - OpenGL error at " << location << ": " << std::endl; 

        switch (error)
        {
        case GL_INVALID_ENUM:
            stringStream << "GL_INVALID_ENUM: enum argument out of range." << std::endl;
            break;
        case GL_INVALID_VALUE:
            stringStream << "GL_INVALID_VALUE: Numeric argument out of range." << std::endl;
            break;
        case GL_INVALID_OPERATION:
            stringStream << "GL_INVALID_OPERATION: Operation illegal in current state." << std::endl;
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            stringStream << "GL_INVALID_FRAMEBUFFER_OPERATION: Framebuffer object is not complete." << std::endl;
            break;
        case GL_OUT_OF_MEMORY:
            stringStream << "GL_OUT_OF_MEMORY: Not enough memory left to execute command." << std::endl;
            break;
        default:
            stringStream << "GL_ERROR: Unknown error." << std::endl;
        }

        if (CEGUI::Logger* logger = CEGUI::Logger::getSingletonPtr())
            logger->logEvent(stringStream.str().c_str());
        else
            std::cerr << stringStream.str() << std::endl;
    }
}

//----------------------------------------------------------------------------//

}
