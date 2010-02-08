/***********************************************************************
    filename:   CEGUINullTexture.h
    created:    Fri Jan 15 2010
    author:     Eugene Marcotte
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUINullTexture_h_
#define _CEGUINullTexture_h_

#include "../../CEGUITexture.h"
#include "CEGUINullRenderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! Implementation of the CEGUI::Texture class for no particular engine.
class NULL_GUIRENDERER_API NullTexture : public Texture
{
public:
    // implement CEGUI::Texture interface
    const Size& getSize() const;
    const Size& getOriginalDataSize() const;
    const Vector2& getTexelScaling() const;
    void loadFromFile(const String& filename, const String& resourceGroup);
    void loadFromMemory(const void* buffer, const Size& buffer_size,
                        PixelFormat pixel_format);
    void saveToMemory(void* buffer);

protected:
    // we all need a little help from out friends ;)
    friend Texture& NullRenderer::createTexture();
    friend Texture& NullRenderer::createTexture(const String&, const String&);
    friend Texture& NullRenderer::createTexture(const Size&);
    friend void NullRenderer::destroyTexture(Texture&);

    //! standard constructor
    NullTexture();
    //! construct texture via an image file.
    NullTexture(const String& filename, const String& resourceGroup);
    //! construct texture with a specified initial size.
    NullTexture(const Size& sz);

    //! destructor.
    virtual ~NullTexture();
    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();

    //! Counter used to provide unique texture names.
    static uint32 d_textureNumber;
    //! Size of the texture.
    Size d_size;
    //! original pixel of size data loaded into texture
    Size d_dataSize;
    //! cached pixel to texel mapping scale values.
    Vector2 d_texelScaling;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUINullTexture_h_
