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
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"

namespace CEGUI
{

OpenGLBaseStateChangeWrapper::BlendFuncSeperateParams::BlendFuncSeperateParams()
{
    reset();
}

void OpenGLBaseStateChangeWrapper::BlendFuncSeperateParams::reset()
{
    d_sfactorRGB = -1;
    d_dfactorRGB = -1;
    d_sfactorAlpha = -1;
    d_dfactorAlpha = -1;
}

bool OpenGLBaseStateChangeWrapper::BlendFuncSeperateParams::equal(GLenum sFactor, GLenum dFactor)
{
    return equal(sFactor, dFactor, sFactor, dFactor);
}

bool OpenGLBaseStateChangeWrapper::BlendFuncSeperateParams::equal(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
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

OpenGLBaseStateChangeWrapper::PortParams::PortParams()
{
    reset();
}
void OpenGLBaseStateChangeWrapper::PortParams::reset()
{
    d_x = -1;
    d_y = -1;
    d_width = -1;
    d_height = -1;
}

bool OpenGLBaseStateChangeWrapper::PortParams::equal(GLint x, GLint y, GLsizei width, GLsizei height)
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

OpenGLBaseStateChangeWrapper::BindBufferParams::BindBufferParams()
{
    reset();
}
void OpenGLBaseStateChangeWrapper::BindBufferParams::reset()
{
    d_target = -1;
    d_buffer = -1;
}

bool OpenGLBaseStateChangeWrapper::BindBufferParams::equal(GLenum target, GLuint buffer)
{
    bool equal = (d_target == target) && (d_buffer == buffer);
    if (!equal)
    {
        d_target = target;
        d_buffer = buffer;
    }
    return equal;
}

OpenGLBaseStateChangeWrapper::BoundTexture::BoundTexture()
{
    d_target = -1;
    d_texture = -1;
}

void OpenGLBaseStateChangeWrapper::BoundTexture::bindTexture(GLenum target, GLuint texture)
{
    d_target = target;
    d_texture = texture;
}





//! constructor.
OpenGLBaseStateChangeWrapper::OpenGLBaseStateChangeWrapper()
{
    reset();
}
//! destructor
OpenGLBaseStateChangeWrapper::~OpenGLBaseStateChangeWrapper()
{
}

void OpenGLBaseStateChangeWrapper::reset()
{
    d_vertexArrayObject = -1;
    d_shaderProgram = -1;
    d_blendFuncSeperateParams.reset();
    d_viewPortParams.reset();
    d_scissorParams.reset();
    d_bindBufferParams.reset();
    d_activeTexturePosition = UINT_MAX;
    d_boundTextures.clear();
    d_enabledOpenGLStates.clear();
}

GLuint OpenGLBaseStateChangeWrapper::getBoundVertexArray() const
{
    return d_vertexArrayObject;
}

void OpenGLBaseStateChangeWrapper::useProgram(GLuint program)
{
    if (program != d_shaderProgram)
    {
        glUseProgram(program);
        d_shaderProgram = program;
    }
}
GLuint OpenGLBaseStateChangeWrapper::getUsedProgram() const
{
    return d_shaderProgram;
}

void OpenGLBaseStateChangeWrapper::blendFunc(GLenum sfactor, GLenum dfactor)
{
    bool callIsRedundant = d_blendFuncSeperateParams.equal(sfactor, dfactor);
    if (!callIsRedundant)
        glBlendFunc(sfactor, dfactor);
}

OpenGLBaseStateChangeWrapper::BlendFuncSeperateParams OpenGLBaseStateChangeWrapper::getBlendFuncParams() const
{
    return d_blendFuncSeperateParams;
}

void OpenGLBaseStateChangeWrapper::blendFuncSeparate(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)
{
    bool callIsRedundant = d_blendFuncSeperateParams.equal(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
    if (!callIsRedundant)
        glBlendFuncSeparate(sfactorRGB, dfactorRGB, sfactorAlpha, dfactorAlpha);
}

void OpenGLBaseStateChangeWrapper::viewport(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool callIsRedundant = d_viewPortParams.equal(x, y, width, height);
    if(!callIsRedundant)
        glViewport(x, y, width, height);
}

OpenGLBaseStateChangeWrapper::PortParams OpenGLBaseStateChangeWrapper::getViewportParams() const
{
    return d_viewPortParams;
}

void OpenGLBaseStateChangeWrapper::scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
    bool callIsRedundant = d_scissorParams.equal(x, y, width, height);
    if (!callIsRedundant)
        glScissor(x, y, width, height);
}

OpenGLBaseStateChangeWrapper::PortParams OpenGLBaseStateChangeWrapper::getScissorParams() const
{
    return d_scissorParams;
}

void OpenGLBaseStateChangeWrapper::bindBuffer(GLenum target, GLuint buffer)
{
    bool callIsRedundant = d_bindBufferParams.equal(target, buffer);
    if (!callIsRedundant)
        glBindBuffer(target, buffer);
}

OpenGLBaseStateChangeWrapper::BindBufferParams OpenGLBaseStateChangeWrapper::getBoundBuffer() const
{
    return d_bindBufferParams;
}

void OpenGLBaseStateChangeWrapper::activeTexture(unsigned int texture_position)
{
    if (d_activeTexturePosition != texture_position)
    {
        while (texture_position >= d_boundTextures.size())
            d_boundTextures.push_back(BoundTexture());

        glActiveTexture(GL_TEXTURE0 + texture_position);
        d_activeTexturePosition = texture_position;
    }
}

unsigned int OpenGLBaseStateChangeWrapper::getActiveTexture() const
{
    return GL_TEXTURE0 + d_activeTexturePosition;
}

void OpenGLBaseStateChangeWrapper::bindTexture(GLenum target, GLuint texture)
{
    if (d_activeTexturePosition == UINT_MAX)
        return;

    BoundTexture& boundTexture = d_boundTextures[d_activeTexturePosition];
    if (boundTexture.d_target != target || boundTexture.d_texture != texture)
    {
        glBindTexture(target, texture);
        boundTexture.bindTexture(target, texture);
    }
}

void OpenGLBaseStateChangeWrapper::enable(GLenum capability)
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

void OpenGLBaseStateChangeWrapper::disable(GLenum capability)
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

int OpenGLBaseStateChangeWrapper::isStateEnabled(GLenum capability) const
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
