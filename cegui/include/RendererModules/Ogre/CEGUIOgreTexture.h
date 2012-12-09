/***********************************************************************
    filename:   CEGUIOgreTexture.h
    created:    Tue Feb 17 2009
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
#ifndef _CEGUIOgreTexture_h_
#define _CEGUIOgreTexture_h_

#include "../../CEGUITexture.h"
#include "CEGUIOgreRenderer.h"
#include <OgreTexture.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//! Implementation of the CEGUI::Texture class for the Ogre engine.
class OGRE_GUIRENDERER_API OgreTexture : public Texture
{
public:
    //! Set the underlying Ogre texture.
    void setOgreTexture(Ogre::TexturePtr texture, bool take_ownership = false);
    //! Return Ogre::TexturePtr for the underlying Ogre texture.
    Ogre::TexturePtr getOgreTexture() const;

    //! return a Ogre::string containing a unique name.
    static Ogre::String getUniqueName();

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
    friend Texture& OgreRenderer::createTexture();
    friend Texture& OgreRenderer::createTexture(const String&, const String&);
    friend Texture& OgreRenderer::createTexture(const Size&);
    friend Texture& OgreRenderer::createTexture(Ogre::TexturePtr&, bool);
    friend void OgreRenderer::destroyTexture(Texture&);

    //! standard constructor
    OgreTexture();
    //! construct texture via an image file.
    OgreTexture(const String& filename, const String& resourceGroup);
    //! construct texture with a specified initial size.
    OgreTexture(const Size& sz);
    //! construct texture from existing Ogre texture.
    OgreTexture(Ogre::TexturePtr& tex, bool take_ownership);

    //! destructor.
    virtual ~OgreTexture();
    //! release the underlying Ogre texture.
    void freeOgreTexture();
    //! updates cached scale value used to map pixels to texture co-ords.
    void updateCachedScaleValues();

    //! Counter used to provide unique texture names.
    static uint32 d_textureNumber;
    //!< The underlying Ogre texture.
    Ogre::TexturePtr d_texture;
    //! specifies whether d_texture was created externally (not owned by us).
    bool d_isLinked;
    //! Size of the texture.
    Size d_size;
    //! original pixel of size data loaded into texture
    Size d_dataSize;
    //! cached pixel to texel mapping scale values.
    Vector2 d_texelScaling;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIOgreTexture_h_
