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

#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"

namespace CEGUI
{

OpenGL3StateChangeWrapper::BlendFuncSeperateParams::BlendFuncSeperateParams()
{
    reset();
}

void OpenGL3StateChangeWrapper::BlendFuncSeperateParams::reset()
{
    d_sfactorRGB = -1;
    d_dfactorRGB = -1;
    d_sfactorAlpha = -1;
    d_dfactorAlpha = -1;
}

bool OpenGL3StateChangeWrapper::BlendFuncSeperateParams::equal(GLenum sFactor, GLenum dFactor)
{
    return equal(sFactor, dFactor, sFactor, dFactor);
}

bool OpenGL3StateChangeWrapper::BlendFuncSeperateParams::equal(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    bool equal = (d_sfactorRGB == sfactorRGB) && (d_dfactorRGB == dfactorRGB) && (d_sfactorAlpha == sfactorAlpha) && (d_dfactorAlpha == dfactorAlpha);
    if (!equal)
    {
        d_sfactorRGB = sfactorRGB;
        d_dfactorRGB = dfactorRGB;
        d_sfactorAlpha = sfactorAlpha;
        d_dfactorAlpha = dfactorAlpha;
    }
    return equal;
}

OpenGL3StateChangeWrapper::PortParams::PortParams()
{
    reset();
}
void OpenGL3StateChangeWrapper::PortParams::reset()
{
    d_x = -1;
    d_y = -1;
    d_width = -1;
    d_height = -1;
}

bool OpenGL3StateChangeWrapper::PortParams::equal(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool equal = (d_x == x) && (d_y == y) && (d_width == width) && (d_height == height);
    if (!equal)
    {
        d_x = x;
        d_y = y;
        d_width = width;
        d_height = height;
    }
    return equal;
}

OpenGL3StateChangeWrapper::BindBufferParams::BindBufferParams()
{
    reset();
}
void OpenGL3StateChangeWrapper::BindBufferParams::reset()
{
    d_target = -1;
    d_buffer = -1;
}

bool OpenGL3StateChangeWrapper::BindBufferParams::equal(GLenum target, GLuint buffer)
{
    bool equal = (d_target == target) && (d_buffer == buffer);
    if (!equal)
    {
        d_target = target;
        d_buffer = buffer;
    }
    return equal;
}

OpenGL3StateChangeWrapper::BoundTexture::BoundTexture()
{
    d_target = -1;
    d_texture = -1;
}

void OpenGL3StateChangeWrapper::BoundTexture::bindTexture(GLenum target, GLuint texture)
{
    d_target = target;
    d_texture = texture;
}





//! constructor.
OpenGL3StateChangeWrapper::OpenGL3StateChangeWrapper()
{
    reset();
}
//! destructor
OpenGL3StateChangeWrapper::~OpenGL3StateChangeWrapper()
{
}

void OpenGL3StateChangeWrapper::reset()
{
    d_vertexArrayObject = -1;
    d_shaderProgram = -1;
    d_blendFuncSeperateParams.reset();
    d_viewPortParams.reset();
    d_scissorParams.reset();
    d_bindBufferParams.reset();
    d_activeTexturePosition = -1;
    d_boundTextures.clear();
    d_enabledOpenGLStates.clear();
}

void OpenGL3StateChangeWrapper::bindVertexArray(GLuint vertexArray)
{
    if (vertexArray != d_vertexArrayObject)
    {
        glBindVertexArray(vertexArray);
        d_vertexArrayObject = vertexArray;
    }

}

GLuint OpenGL3StateChangeWrapper::getBoundVertexArray() const
{
    return d_vertexArrayObject;
}

void OpenGL3StateChangeWrapper::useProgram(GLuint program)
{
    if (program != d_shaderProgram)
    {
        glUseProgram(program);
        d_shaderProgram = program;
    }
}

GLuint OpenGL3StateChangeWrapper::getUsedProgram() const
{
    return d_shaderProgram;
}

void OpenGL3StateChangeWrapper::blendFunc(GLenum sfactor, GLenum dfactor)
{
    bool callIsRedundant = d_blendFuncSeperateParams.equal(sfactor, dfactor);
    if (!callIsRedundant)
        glBlendFunc(sfactor, dfactor);
}

OpenGL3StateChangeWrapper::BlendFuncSeperateParams OpenGL3StateChangeWrapper::getBlendFuncParams() const
{
    return d_blendFuncSeperateParams;
}

void OpenGL3StateChangeWrapper::blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    bool callIsRedundant = d_blendFuncSeperateParams.equal(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    if (!callIsRedundant)
        glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void OpenGL3StateChangeWrapper::viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool callIsRedundant = d_viewPortParams.equal(x, y, width, height);
    if(!callIsRedundant)
        glViewport(x, y, width, height);
}

OpenGL3StateChangeWrapper::PortParams OpenGL3StateChangeWrapper::getViewportParams() const
{
    return d_viewPortParams;
}

void OpenGL3StateChangeWrapper::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool callIsRedundant = d_scissorParams.equal(x, y, width, height);
    if (!callIsRedundant)
        glScissor(x, y, width, height);
}

OpenGL3StateChangeWrapper::PortParams OpenGL3StateChangeWrapper::getScissorParams() const
{
    return d_scissorParams;
}

void OpenGL3StateChangeWrapper::bindBuffer(GLenum target, GLuint buffer)
{
    bool callIsRedundant = d_bindBufferParams.equal(target, buffer);
    if (!callIsRedundant)
        glBindBuffer(target, buffer);
}

OpenGL3StateChangeWrapper::BindBufferParams OpenGL3StateChangeWrapper::getBoundBuffer() const
{
    return d_bindBufferParams;
}

void OpenGL3StateChangeWrapper::activeTexture(unsigned int texture_position)
{
    if (d_activeTexturePosition != texture_position)
    {
        while (texture_position >= d_boundTextures.size())
            d_boundTextures.push_back(BoundTexture());

        glActiveTexture(GL_TEXTURE0 + texture_position);
        d_activeTexturePosition = texture_position;
    }
}

unsigned int OpenGL3StateChangeWrapper::getActiveTexture() const
{
    return GL_TEXTURE0 + d_activeTexturePosition;
}

void OpenGL3StateChangeWrapper::bindTexture(GLenum target, GLuint texture)
{
    if (d_activeTexturePosition == -1)
        return;

    BoundTexture& boundTexture = d_boundTextures[d_activeTexturePosition];
    if (boundTexture.d_target != target || boundTexture.d_texture != texture)
    {
        glBindTexture(target, texture);
        boundTexture.bindTexture(target, texture);
    }
}

void OpenGL3StateChangeWrapper::enable(GLenum capability)
{
    std::map<GLenum, bool>::iterator found_iterator = d_enabledOpenGLStates.find(capability);
    if(found_iterator != d_enabledOpenGLStates.end())
    {
        if(found_iterator->second != true)
        {
            glEnable(capability);
            found_iterator->second = true;
        }
    }
    else
    {
        d_enabledOpenGLStates[capability] = true;
        glEnable(capability);
    }
}

void OpenGL3StateChangeWrapper::disable(GLenum capability)
{
    std::map<GLenum, bool>::iterator found_iterator = d_enabledOpenGLStates.find(capability);
    if(found_iterator != d_enabledOpenGLStates.end())
    {
        if(found_iterator->second != false)
        {
            glDisable(capability);
            found_iterator->second = false;
        }
    }
    else
    {
        d_enabledOpenGLStates[capability] = false;
        glDisable(capability);
    }
}

int OpenGL3StateChangeWrapper::isStateEnabled(GLenum capability) const
{
    std::map<GLenum, bool>::const_iterator found_iterator = d_enabledOpenGLStates.find(capability);
    if(found_iterator != d_enabledOpenGLStates.end())
    {
        bool isEnabled = found_iterator->second;
        if(isEnabled)
            return 1;
        else
            return 0;
    }
    else
        return -1;
}


}
