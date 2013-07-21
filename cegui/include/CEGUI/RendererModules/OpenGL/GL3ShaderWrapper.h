/***********************************************************************
    filename:   GL3ShaderWrapper.h
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
#ifndef _CEGUIOpenGL3ShaderWrapper_h_
#define _CEGUIOpenGL3ShaderWrapper_h_

#include "RendererBase.h"

#include "CEGUI/ShaderWrapper.h"
#include <string>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    class OpenGL3Shader;
    class ShaderParameterBindings;

class OPENGL_GUIRENDERER_API OpenGL3ShaderWrapper : public ShaderWrapper
{
public:
    OpenGL3ShaderWrapper(OpenGL3Shader& shader);

    ~OpenGL3ShaderWrapper()
    {}

    //Implementation of ShaderWrapper interface
    void prepareForRendering(ShaderParameterBindings* shaderParameterBindings) const;

    void addAttributeVariable(const std::string& attributeName);
    void addUniformVariable(const std::string& uniformName);
    void addTextureUniformVariable(const std::string& uniformName, GLint textureUnitIndex);

    GLint getAttributeLocation(const std::string& attributeName);
    GLint getUniformLocation(const std::string& uniformName);

protected:
    //! The underlying GLSL shader that this class wraps the access to
    OpenGL3Shader& d_shader;
    //! 
    std::map<std::string, GLint> d_uniformVariables;
    //! 
    std::map<std::string, GLint> d_attributeVariables;


};


}

#endif

