/***********************************************************************
    filename:   CEGUIDirect3D9Texture.h
    created:    Mon Feb 9 2009
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
#ifndef _CEGUIDirect3D9Texture_h_
#define _CEGUIDirect3D9Texture_h_

#include "../../CEGUIBase.h"
#include "../../CEGUIRenderer.h"
#include "../../CEGUITexture.h"
#include "CEGUIDirect3D9Renderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! Texture implementation for the Direct3D9Renderer.
class DIRECT3D9_GUIRENDERER_API Direct3D9Texture : public Texture
{
public:
    /*!
    \brief
        set the D3D9 texture that this Texture is based on to the specified
        texture.
    */
    void setDirect3D9Texture(LPDIRECT3DTEXTURE9 tex);

    /*!
    \brief
        Return the internal D3D9 texture used by this Texture object.

    \return
        Pointer to the D3D9 texture interface that this object is using.
    */
    LPDIRECT3DTEXTURE9 getDirect3D9Texture() const;

    /*!
    \brief
        Sets what the texture should consider as the original data size.

    \note
        This also causes the texel scaling values to be updated.
    */
    void setOriginalDataSize(const Size& sz);

    //! auto called via the Renderer prior to Reset on the Direct3DDevice9.
    void preD3DReset();

    //! auto called via the Renderer after Reset on the Direct3DDevice9.
    void postD3DReset();

    // implement abstract members from base class.
    const Size& getSize() const;
    const Size& getOriginalDataSize() const;
    const Vector2& getTexelScaling() const;
    void loadFromFile(const String& filename, const String& resourceGroup);
    void loadFromMemory(const void* buffer, const Size& buffer_size,
                        PixelFormat pixel_format);
    void saveToMemory(void* buffer);

protected:
    // Friends (to allow construction and destruction)
    friend Texture& Direct3D9Renderer::createTexture(void);
    friend Texture& Direct3D9Renderer::createTexture(const String&, const String&);
    friend Texture& Direct3D9Renderer::createTexture(const Size&);
    friend Texture& Direct3D9Renderer::createTexture(LPDIRECT3DTEXTURE9 tex);
    friend void Direct3D9Renderer::destroyTexture(Texture&);

    //! Basic constructor.
    Direct3D9Texture(Direct3D9Renderer& owner);
    //! Construct texture from an image file.
    Direct3D9Texture(Direct3D9Renderer& owner, const String& filename,
                     const String& resourceGroup);
    //! Construct texture with a given size.
    Direct3D9Texture(Direct3D9Renderer& owner, const Size& sz);
    //! Construct texture that wraps an existing D3D9 texture.
    Direct3D9Texture(Direct3D9Renderer& owner, LPDIRECT3DTEXTURE9 tex);
    //! Destructor.
    virtual ~Direct3D9Texture();

    //! clean up the internal texture.
    void cleanupDirect3D9Texture();
    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();
    //! set d_size to actual texture size (d_dataSize is used if query fails)
    void updateTextureSize();

    //! Direct3D9Renderer object that created and owns this texture.
    Direct3D9Renderer& d_owner;
    //! The D3D9 texture we're wrapping.
    LPDIRECT3DTEXTURE9 d_texture;
    //! Size of the texture.
    Size d_size;
    //! original pixel of size data loaded into texture
    Size d_dataSize;
    //! cached pixel to texel mapping scale values.
    Vector2 d_texelScaling;
    //! holds info about the texture surface before we released it for reset.
    D3DSURFACE_DESC d_savedSurfaceDesc;
    //! true when d_savedSurfaceDesc is valid and texture can be restored.
    bool d_savedSurfaceDescValid;
};

} // End of  CEGUI namespace section


#endif // end of guard _CEGUIDirect3D9Texture_h_
