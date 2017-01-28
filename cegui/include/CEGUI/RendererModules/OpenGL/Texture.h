/***********************************************************************
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
#ifndef _CEGUIOpenGLTexture_h_
#define _CEGUIOpenGLTexture_h_

#include "../../Base.h"
#include "../../Renderer.h"
#include "../../Texture.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! Texture implementation for the OpenGLRenderer.
class OPENGL_GUIRENDERER_API OpenGLTexture : public Texture
{
public:
    //! Basic constructor.
    OpenGLTexture(OpenGLRendererBase& owner, const String& name);
    //! Destructor.
    virtual ~OpenGLTexture();

    //! initialise method that creates a Texture.
    void initialise();
    //! initialise method that creates a Texture from an image file.
    void initialise(const String& filename, const String& resourceGroup);
    //! initialise method that creates a Texture with a given size.
    void initialise(const Sizef& size);
    //! Constructor that wraps an existing GL texture.
    void initialise(GLuint tex, const Sizef& size);

    /*!
    \brief
        set the openGL texture that this Texture is based on to the specified
        texture, with the specified size.
    */
    void setOpenGLTexture(GLuint tex, const Sizef& size);

    /*!
    \brief
        Return the internal OpenGL texture id used by this Texture object.

    \return
        id of the OpenGL texture that this object is using.
    */
    GLuint getOpenGLTexture() const;

    /*!
    \brief
        set the size of the internal texture.

    \param sz
        size for the internal texture, in pixels.

    \note
        Depending upon the hardware capabilities, the actual final size of the
        texture may be larger than what is specified when calling this function.
        The texture will never be smaller than what you request here.  To
        discover the actual size, call getSize.

    \exception RendererException
        thrown if the hardware is unable to support a texture large enough to
        fulfill the requested size.

    \return
        Nothing.
    */
    void setTextureSize(const Sizef& sz);

    /*!
    \brief
        Grab the texture to a local buffer.

        This will destroy the OpenGL texture, and restoreTexture must be called
        before using it again.
    */
    void grabTexture();

    /*!
    \brief
        Restore the texture from the locally buffered copy previously create by
        a call to grabTexture.
    */
    void restoreTexture();

    // implement abstract members from base class.
    const String& getName() const override;
    const Sizef& getSize() const override;
    const Sizef& getOriginalDataSize() const override;
    const glm::vec2& getTexelScaling() const override;
    void loadFromFile(const String& filename, const String& resourceGroup) override;
    void loadFromMemory(const void* buffer, const Sizef& buffer_size,
        PixelFormat pixel_format) override;
    void blitFromMemory(const void* sourceData, const Rectf& area) override;
    void blitToMemory(void* targetData) override = 0;
    bool isPixelFormatSupported(const PixelFormat fmt) const override;

protected:

    //! generate the OpenGL texture and set some initial options.
    void generateOpenGLTexture();

    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();

    //! clean up the GL texture, or the grab buffer if it had been grabbed
    void cleanupOpenGLTexture();

    GLint getTextureFormat() const;

    //! initialise the internal format flags for the given CEGUI::PixelFormat.
    virtual void initInternalPixelFormatFields(const PixelFormat fmt) = 0;

    //! internal texture resize function (does not reset format or other fields)
    virtual void setTextureSize_impl(const Sizef& sz) = 0;

    void loadUncompressedTextureBuffer(const Rectf& dest_area,
                                       const GLvoid* buffer) const;

    void loadCompressedTextureBuffer(const Rectf& dest_area,
                                     const GLvoid* buffer) const;

    virtual GLsizei getCompressedTextureSize(const Sizef& pixel_size) const;

    //! OpenGL method to set glTexEnv which is deprecated in GL 3.2 and GLES 2.0 and above
    virtual void setTextureEnvironment();

    //! The OpenGL texture we're wrapping.
    GLuint d_ogltexture;
    //! Size of the texture.
    Sizef d_size;
    //! cached image data for restoring the texture.
    std::uint8_t* d_grabBuffer;
    //! original size of pixel data loaded into texture
    Sizef d_dataSize;
    //! cached pixel to texel mapping scale values.
    glm::vec2 d_texelScaling;
    //! OpenGLRenderer that created and owns this OpenGLTexture
    OpenGLRendererBase& d_owner;
    //! The name given for this texture.
    const String d_name;
    //! Pixel data format
    GLenum d_pixelDataFormat;
    //! Pixel data type
    GLenum d_pixelDataType;
    //! Whether Texture format is a compressed format
    bool d_isCompressed;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif // end of guard _CEGUIOpenGLTexture_h_
