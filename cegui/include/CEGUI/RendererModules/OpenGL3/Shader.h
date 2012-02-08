/***********************************************************************
    filename:   Shader.h
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

#ifndef _CEGUIOpenGL3Shader_h_
#define _CEGUIOpenGL3Shader_h_

#include "CEGUI/Exceptions.h"

#include <string>

using namespace std;

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    class Shader
    {
    public:

        // Loads shaders from files and compiles them.
        // When path is "hello", the files "hello.frag" & "hello.vert"
        // will be loaded.
        Shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
        ~Shader();

        // Bind the shader to the OGL state-machine
        void bind() const;

        // Unbind the shader
        void unbind() const;

        // Query the location of a vertex attribute inside the shader.
        int getAttribLocation(const std::string &name) const;

        // Query the location of a uniform variable inside the shader.
        int getUniformLocation(const std::string &name) const;

        // Define the name of the variable inside the shader which represents the final color for each fragment.
        void bindFragDataLocation(const std::string &name);

        bool isCreatedSuccessfully();


        void link();

    private:
		int compile(int type, const string &source, const string &path);

        void outputShaderLog(int shader);
        void outputProgramLog(int program);

        std::string d_shaderName;
        bool d_createdSucessfully;

        int d_vertexShader;
        int d_fragmentShader;
        int d_geometryShader;
        int d_program;
    };




    
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)
#define checkGLErrors() my_get_errors(AT)



    using namespace std;

    // Query OpenGL errors and print error messages to STDERR.
    void my_get_errors(const char *location);
}

#endif
