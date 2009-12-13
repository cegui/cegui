/***********************************************************************
    filename:   CEGUIOpenGLESTexture.cpp
    created:    Sun Jan 11 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIOpenGLESTexture.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"
#include "CEGUIImageCodec.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_texelScaling(0, 0),
    d_owner(owner)
{
    generateOpenGLESTexture();
}

//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, const String& filename,
                             const String& resourceGroup) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner)
{
    generateOpenGLESTexture();
    loadFromFile(filename, resourceGroup);
}

//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, const Size& size) :
    d_size(0, 0),
    d_grabBuffer(0),
    d_dataSize(0, 0),
    d_owner(owner)
{
    generateOpenGLESTexture();
    setTextureSize(size);
}

//----------------------------------------------------------------------------//
OpenGLESTexture::OpenGLESTexture(OpenGLESRenderer& owner, GLuint tex,
                             const Size& size) :
    d_ogltexture(tex),
    d_size(size),
    d_grabBuffer(0),
    d_dataSize(size),
    d_owner(owner)
{
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
OpenGLESTexture::~OpenGLESTexture()
{
    cleanupOpenGLESTexture();
}

//----------------------------------------------------------------------------//
const Size& OpenGLESTexture::getSize() const
{
    return d_size;
}

//----------------------------------------------------------------------------//
const Size& OpenGLESTexture::getOriginalDataSize() const
{
    return d_dataSize;
}

//----------------------------------------------------------------------------//
const Vector2& OpenGLESTexture::getTexelScaling() const
{
    return d_texelScaling;
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::loadFromFile(const String& filename,
    const String& resourceGroup)
{
    // Note from PDT:
    // There is somewhat tight coupling here between OpenGLESTexture and the
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
        throw RendererException("OpenGLESTexture::loadFromFile - "
                                "CEGUI::System object has not been created: "
                                "unable to access ImageCodec.");

    Texture* res = sys->getImageCodec().load(texFile, this);

    // unload file data buffer
    System::getSingleton().getResourceProvider()->
        unloadRawDataContainer(texFile);

    if (!res)
        // It's an error
        throw RendererException("OpenGLESTexture::loadFromFile - " +
                                sys->getImageCodec().getIdentifierString()+
                                " failed to load image '" + filename + "'.");
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::loadFromMemory(const void* buffer, const Size& buffer_size,
                    PixelFormat pixel_format)
{
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
void OpenGLESTexture::saveToMemory(void* buffer)
{
	assert(0 && "glGetTexImage() is unsupported");

	// save old texture binding
    //GLuint old_tex;
    //glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    //glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);

    // restore previous texture binding.
    //glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::setTextureSize(const Size& sz)
{
    const Size size(d_owner.getAdjustedTextureSize(sz));

    // make sure size is within boundaries
    GLfloat maxSize;
    glGetFloatv(GL_MAX_TEXTURE_SIZE, &maxSize);
    if ((size.d_width > maxSize) || (size.d_height > maxSize))
        throw RendererException("OpenGLESTexture::setTextureSize: size too big");

    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    // set texture to required size
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 static_cast<GLsizei>(size.d_width),
                 static_cast<GLsizei>(size.d_height),
                 0, GL_RGBA , GL_UNSIGNED_BYTE, 0);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);

    d_dataSize = d_size = size;
    updateCachedScaleValues();
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::grabTexture()
{
	assert(0 && "glGetTexImage() is unsupported");    

	// save old texture binding
    //GLuint old_tex;
    //glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    //// bind the texture we want to grab
    //glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    //// allocate the buffer for storing the image data
    //d_grabBuffer = new uint8[static_cast<int>(4*d_size.d_width*d_size.d_height)];
    //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, d_grabBuffer);
    //// delete the texture
    //glDeleteTextures(1, &d_ogltexture);

    //// restore previous texture binding.
    //glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::restoreTexture()
{
    if (d_grabBuffer)
    {
        generateOpenGLESTexture();

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
void OpenGLESTexture::updateCachedScaleValues()
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
void OpenGLESTexture::generateOpenGLESTexture()
{
    // save old texture binding
    GLuint old_tex;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, reinterpret_cast<GLint*>(&old_tex));

    glGenTextures(1, &d_ogltexture);

    // set some parameters for this texture.
    glBindTexture(GL_TEXTURE_2D, d_ogltexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F); // GL_CLAMP_TO_EDGE
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F); // GL_CLAMP_TO_EDGE
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // restore previous texture binding.
    glBindTexture(GL_TEXTURE_2D, old_tex);
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::cleanupOpenGLESTexture()
{
    // if the grabbuffer is not empty then free it
    if (d_grabBuffer)
    {
        delete[] d_grabBuffer;
        d_grabBuffer = 0;
    }
    // otherwise delete any OpenGLES texture associated with this object.
    else
    {
        glDeleteTextures(1, &d_ogltexture);
        d_ogltexture = 0;
    }
}

//----------------------------------------------------------------------------//
GLuint OpenGLESTexture::getOpenGLESTexture() const
{
    return d_ogltexture;
}

//----------------------------------------------------------------------------//
void OpenGLESTexture::setOpenGLESTexture(GLuint tex, const Size& size)
{
    if (d_ogltexture != tex)
    {
        // cleanup the current state first.
        cleanupOpenGLESTexture();

        d_ogltexture = tex;
    }

    d_dataSize = d_size = size;
    updateCachedScaleValues();
}

} // End of  CEGUI namespace section
