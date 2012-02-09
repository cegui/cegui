/***********************************************************************
    filename:   CEGUIOpenGLTexture.cpp
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
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
#include <GL/glew.h>
#include "CEGUI/RendererModules/OpenGL3/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/System.h"
#include "CEGUI/ImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGL3Texture::OpenGL3Texture(OpenGL3Renderer& owner, const String& name) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner),
    d_name(name)
{
    generateOpenGLTexture();
}

//----------------------------------------------------------------------------//
OpenGL3Texture::OpenGL3Texture(OpenGL3Renderer& owner, const String& name,
                             const String& filename,
                             const String& resourceGroup) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner),
    d_name(name)
{
    generateOpenGLTexture();
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OpenGL3Texture::OpenGL3Texture(OpenGL3Renderer& owner, const String& name,
                             const Sizef& size) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner),
    d_name(name)
{
    generateOpenGLTexture();
    setTextureSize(size);
}

//----------------------------------------------------------------------------//
OpenGL3Texture::OpenGL3Texture(OpenGL3Renderer& owner, const String& name,
                             GLuint tex, const Sizef& size) :
    d_ogltexture(tex),
    d_size(size),
    d_grabBuffer(0),
    d_dataSize(size),
    d_owner(owner),
    d_name(name)
{
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
OpenGL3Texture::~OpenGL3Texture()
{
    cleanupOpenGLTexture();
}

//----------------------------------------------------------------------------//
const String& OpenGL3Texture::getName() const
{
    return d_name;
}

//----------------------------------------------------------------------------//
const Sizef& OpenGL3Texture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Sizef& OpenGL3Texture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGL3Texture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::loadFromFile(const String& filename,
    const String& resourceGroup)
{
    // Note from PDT:
    // There is somewhat tight coupling here between OpenGL3Texture and the
    // ImageCodec classes - we have intimate knowledge of how they are
    // implemented and that knowledge is relied upon in an unhealthy way; this
    // should be addressed at some stage.

    // load file to memory via resource provider
    RawDataContainer texFile;
    System::getSingleton().getResourceProvider()->
        loadRawDataContainer(filename, texFile, resourceGroup);

    // get and check existence of CEGUI::System (needed to access ImageCodec)
    System* sys = System::getSingletonPtr();
    if (!sys)
        CEGUI_THROW(RendererException("OpenGL3Texture::loadFromFile - "
            "CEGUI::System object has not been created: "
            "unable to access ImageCodec."));

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    System::getSingleton().getResourceProvider()->
        unloadRawDataContainer(texFile);

    if (!res)
        // It's an error
        CEGUI_THROW(RendererException("OpenGL3Texture::loadFromFile - " +
            sys->getImageCodec().getIdentifierString() +
            " failed to load image '" + filename + "'."));
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::loadFromMemory(const void* buffer, const Sizef& buffer_size,
                    PixelFormat pixel_format)
{
    if (!isPixelFormatSupported(pixel_format))
        CEGUI_THROW(InvalidRequestException("OpenGL3Texture::loadFromMemory: "
            "Data was supplied in an unsupported pixel format."));

    GLint comps;
    GLenum format;
    switch (pixel_format)
    {
    case PF_RGB:
        comps = 3;
        format = GL_RGB;
        break;
    case PF_RGBA:
        comps = 4;
        format = GL_RGBA;
        break;
    };

    setTextureSize(buffer_size);
    // store size of original data we are loading
    d_dataSize = buffer_size;
    // update scale values
    updateCachedScaleValues();

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // do the real work of getting the data into the texture
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                    static_cast<GLsizei>(buffer_size.d_width),
                    static_cast<GLsizei>(buffer_size.d_height),
                    format, GL_UNSIGNED_BYTE, buffer);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::setTextureSize(const Sizef& sz)
{
    const Sizef size(d_owner.getAdjustedTextureSize(sz));

    // make sure size is within boundaries
    GLfloat maxSize;
    glGetFloatv(GL_MAX_TEXTURE_SIZE, &maxSize);
    if ((size.d_width > maxSize) || (size.d_height > maxSize))
        CEGUI_THROW(RendererException(
            "OpenGL3Texture::setTextureSize: size too big"));

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // set texture to required size
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 static_cast<GLsizei>(size.d_width),
                 static_cast<GLsizei>(size.d_height),
                 0, GL_RGBA , GL_UNSIGNED_BYTE, 0);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);

    d_dataSize = d_size = size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::grabTexture()
{
    // if texture has already been grabbed, do nothing.
    if (d_grabBuffer)
        return;

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // bind the texture we want to grab
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    // allocate the buffer for storing the image data
    d_grabBuffer = new uint8[static_cast<int>(4*d_size.d_width*d_size.d_height)];
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, d_grabBuffer);
    // delete the texture
    glDeleteTextures(1, &d_ogltexture);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::blitFromMemory(void* sourceData, const Rectf& area)
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // set texture to required size
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    
    glTexSubImage2D(GL_TEXTURE_2D, 0,
        GLint(area.left()), GLint(area.top()),
        GLsizei(area.getWidth()), GLsizei(area.getHeight()),
        GL_RGBA8, GL_UNSIGNED_BYTE, sourceData
    );

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::blitToMemory(void* targetData)
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // set texture to required size
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, targetData);
    
    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::restoreTexture()
{
    if (d_grabBuffer)
    {
        generateOpenGLTexture();

        // save old texture binding
        GLuint old_tex;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

        // bind the texture to restore to
        glBindTexture(GL_TEXTURE_2D, d_ogltexture);

        // reload the saved image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                     static_cast<GLsizei>(d_size.d_width),
                     static_cast<GLsizei>(d_size.d_height),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, d_grabBuffer);

        // restore previous texture binding.
        glBindTexture(GL_TEXTURE_2D, old_tex);

        // free the grabbuffer
        delete [] d_grabBuffer;
        d_grabBuffer = 0;
    }
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::updateCachedScaleValues()
{
    //
    // calculate what to use for x scale
    //
    const float orgW = d_dataSize.d_width;
    const float texW = d_size.d_width;

    // if texture and original data width are the same, scale is based
    // on the original size.
    // if texture is wider (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_x = 1.0f / ((orgW == texW) ? orgW : texW);

    //
    // calculate what to use for y scale
    //
    const float orgH = d_dataSize.d_height;
    const float texH = d_size.d_height;

    // if texture and original data height are the same, scale is based
    // on the original size.
    // if texture is taller (and source data was not stretched), scale
    // is based on the size of the resulting texture.
    d_texelScaling.d_y = 1.0f / ((orgH == texH) ? orgH : texH);
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::generateOpenGLTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    glGenTextures(1, &d_ogltexture);

    // set some parameters for this texture.
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::cleanupOpenGLTexture()
{
    // if the grabbuffer is not empty then free it
    if (d_grabBuffer)
    {
        delete[] d_grabBuffer;
        d_grabBuffer = 0;
    }
    // otherwise delete any OpenGL texture associated with this object.
    else
    {
        glDeleteTextures(1, &d_ogltexture);
        d_ogltexture = 0;
    }
}

//----------------------------------------------------------------------------//
GLuint OpenGL3Texture::getOpenGLTexture() const
{
    return d_ogltexture;
}

//----------------------------------------------------------------------------//
void OpenGL3Texture::setOpenGLTexture(GLuint tex, const Sizef& size)
{
    if (d_ogltexture != tex)
    {
        // cleanup the current state first.
        cleanupOpenGLTexture();

        d_ogltexture = tex;
    }

    d_dataSize = d_size = size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
bool OpenGL3Texture::isPixelFormatSupported(const PixelFormat fmt) const
{
    return fmt == PF_RGBA || fmt == PF_RGB;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
