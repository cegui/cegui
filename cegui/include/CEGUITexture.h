/***********************************************************************
    filename: CEGUITexture.h
    created:  21/2/2004
    author:   Paul D Turner

    purpose: Defines abstract interface for texture objects.  Texture
             objects are created & destroyed by the Renderer.
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
#ifndef _CEGUITexture_h_
#define _CEGUITexture_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Abstract base class specifying the required interface for Texture objects.

    Texture objects are created via the Renderer.  The actual inner workings of
    any Texture object are dependant upon the Renderer (and underlying API) in
    use.  This base class defines the minimal set of functions that is required
    for the rest of the system to work.  Texture objects are only created
    through the Renderer object's texture creation functions.
*/
class CEGUIEXPORT Texture :
    public AllocatedObject<Texture>
{
public:
    /*!
    \brief
        Enumerated type containing the supported pixel formats that can be
        passed to loadFromMemory
    */
    enum PixelFormat
    {
        //! Each pixel is 3 bytes. RGB in that order.
        PF_RGB,
        //! Each pixel is 4 bytes. RGBA in that order.
        PF_RGBA
    };

    /*!
    \brief
        Destructor for Texture base class.
    */
    virtual ~Texture() {}

    /*!
    \brief
        Returns the current pixel size of the texture.

    \return
        Reference to a Size object that describes the size of the texture in
        pixels.
    */
    virtual const Size<>& getSize() const = 0;

    /*!
    \brief
        Returns the original pixel size of the data loaded into the texture.

    \return
        reference to a Size object that describes the original size, in pixels,
        of the data loaded into the texture.
    */
    virtual const Size<>& getOriginalDataSize() const = 0;

    /*!
    \brief
        Returns pixel to texel scale values that should be used for converting
        pixel values to texture co-ords.

    \return
        Reference to a Vector2 object that describes the scaling values required
        to accurately map pixel positions to texture co-ordinates.
    */
    virtual const Vector2<>& getTexelScaling() const = 0;

    /*!
    \brief
        Loads the specified image file into the texture.  The texture is resized
        as required to hold the image.

    \param filename
        The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier to be passed to the resource provider when
        loading the image file.

    \return
        Nothing.
    */
    virtual void loadFromFile(const String& filename,
                              const String& resourceGroup) = 0;

    /*!
    \brief
        Loads (copies) an image in memory into the texture.  The texture is
        resized as required to hold the image.

    \param buffer
        Pointer to the buffer containing the image data.

    \param buffer_size
        Size of the buffer (in pixels as specified by \a pixelFormat)

    \param pixel_format
        PixelFormat value describing the format contained in \a buffPtr.

    \return
        Nothing.
    */
    virtual void loadFromMemory(const void* buffer,
                                const Size<>& buffer_size,
                                PixelFormat pixel_format) = 0;

    /*!
    \brief
        Performs an area memory blit to the texture

    \param sourceData
        input data, the size must match area described by the given Rect

    \param area
        area where the blit will happen

    \note The pixel format must match current Texture's pixel format!
    */
    virtual void blitFromMemory(void* sourceData, const Rect& area) = 0;

    /*!
    \brief
    	Performs a complete blit from the texture surface to memory

    \param
    	targetData the buffer where the target is stored

    \note
    	You have to (correctly) preallocate the target buffer!
    */
    virtual void blitToMemory(void* targetData) = 0;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUITexture_h_
