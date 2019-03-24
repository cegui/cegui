/***********************************************************************
    created:    Sun, 6th April 2014
    author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIDirect3D11Texture_h_
#define _CEGUIDirect3D11Texture_h_

#include "../../Texture.h"
#include "CEGUI/RendererModules/Direct3D11/Renderer.h"
#include "../../Sizef.h"

// d3d forward refs
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! Texture implementation for the Direct3D11Renderer.
class D3D11_GUIRENDERER_API Direct3D11Texture : public Texture
{
public:
    /*!
    \brief
        set the texture that this Texture is based on to the specified
        texture.
    */
    void setDirect3DTexture(ID3D11Texture2D* tex);

    /*!
    \brief
        Return the internal texture used by this Texture object.

    \return
        Pointer to the texture interface that this object is using.
    */
    ID3D11Texture2D* getDirect3DTexture() const;

    /*!
    \brief
        Return the internal shader resource view for the texture.

    \return
        Pointer to the ShaderResourceView interface.
    */
    ID3D11ShaderResourceView* getDirect3DShaderResourceView() const;

    /*!
    \brief
        Sets what the texture should consider as the original data size.

    \note
        This also causes the texel scaling values to be updated.
    */
    void setOriginalDataSize(const Sizef& sz);

    // implement abstract members from base class.
    const String& getName() const;
    const Sizef& getSize() const;
    const Sizef& getOriginalDataSize() const;
    const glm::vec2& getTexelScaling() const;
    void loadFromFile(const String& filename, const String& resourceGroup);
    void loadFromMemory(const void* buffer, const Sizef& buffer_size,
                        PixelFormat pixel_format);
    void blitFromMemory(const void* sourceData, const Rectf& area);
    void blitToMemory(void* targetData);
    bool isPixelFormatSupported(const PixelFormat fmt) const;

protected:
    // Friends to allow Renderer to peform construction and destruction
    friend Texture& Direct3D11Renderer::createTexture(const String&);
    friend Texture& Direct3D11Renderer::createTexture(const String&,
                                                      const String&,
                                                      const String&);
    friend Texture& Direct3D11Renderer::createTexture(const String&,
                                                      const Sizef&);
    friend void Direct3D11Renderer::destroyTexture(Texture&);
    friend void Direct3D11Renderer::destroyTexture(const String&);

    //! Basic constructor.
    Direct3D11Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const String& name);
    //! Construct texture from an image file.
    Direct3D11Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const String& name, const String& filename,
                      const String& resourceGroup);
    //! Construct texture with a given size.
    Direct3D11Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const String& name, const Sizef& sz);
    //! Construct texture that wraps an existing texture.
    Direct3D11Texture(ID3D11Device& device, ID3D11DeviceContext& deviceContext, const String& name, ID3D11Texture2D* tex);
    //! Destructor.
    virtual ~Direct3D11Texture();

    //! clean up the internal texture.
    void cleanupDirect3D11Texture();
    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();
    //! set d_size to actual texture size (d_dataSize is used if query fails)
    void updateTextureSize();
    //! creates shader resource view for the current D3D texture
    void initialiseShaderResourceView();

    //! The D3D Device
    ID3D11Device& d_device;
    //! The D3D DeviceContext
    ID3D11DeviceContext& d_deviceContext;
    //! The D3D 10 texture we're wrapping.
    ID3D11Texture2D* d_texture;
    //! Shader resource view for the texture.
    ID3D11ShaderResourceView* d_resourceView;
    //! Size of the texture.
    Sizef d_size;
    //! original pixel of size data loaded into texture
    Sizef d_dataSize;
    //! cached pixel to texel mapping scale values.
    glm::vec2 d_texelScaling;
    //! The name we were created with.
    const String d_name;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIDirect3D11Texture_h_
