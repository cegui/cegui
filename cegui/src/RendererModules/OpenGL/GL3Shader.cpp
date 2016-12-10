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
#include "CEGUI/RendererModules/OpenGL/GL3Shader.h"
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/Logger.h"
#include "CEGUI/Exceptions.h"


#include <sstream>
#include <iostream>

namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGL3Shader::OpenGL3Shader(const std::string& vertex_shader_source,
                             const std::string& fragment_shader_source,
                             OpenGLBaseStateChangeWrapper* glStateChanger) :
    OpenGLBaseShader(vertex_shader_source,fragment_shader_source, glStateChanger)
{
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::bindFragDataLocation(const std::string &name)
{
    if(d_program > 0)
    {
        glBindFragDataLocation(d_program, 0, name.c_str() );
        link();
    }
}

//----------------------------------------------------------------------------//
void OpenGL3Shader::link()
{
    OpenGLBaseShader::link();

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
    OpenGLRendererBase::checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);
}

//----------------------------------------------------------------------------//
}

