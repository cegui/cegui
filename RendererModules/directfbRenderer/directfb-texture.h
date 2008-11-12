/***********************************************************************
    filename:   directfb-texture.cpp
    author:     Keith Mok
*************************************************************************/
/***************************************************************************
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
#ifndef _directfbtexture_h_
#define _directfbtexture_h_

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"
#include "directfb-renderer.h"

#include <list>

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Texture class that is created by DirectfbRenderer objects
*/
class DirectfbTexture : public Texture
{
private:
    /*************************************************************************
        Friends (to allow construction and destruction)
    *************************************************************************/
    friend    Texture* DirectfbRenderer::createTexture(void);
    friend    Texture* DirectfbRenderer::createTexture(const String& filename, const String& resourceGroup);
    friend    Texture* DirectfbRenderer::createTexture(float size);
    friend    void     DirectfbRenderer::destroyTexture(Texture* texture);


    /*************************************************************************
        Construction & Destruction (by Renderer object only)
    *************************************************************************/
    DirectfbTexture(Renderer* owner);
    virtual ~DirectfbTexture(void);

public:
    /*!
    \brief
        Returns the current pixel width of the texture

    \return
        ushort value that is the current width of the texture in pixels
    */
    virtual    ushort    getWidth(void) const        {return d_width;}

    /*!
    \brief
        Returns the current pixel height of the texture

    \return
        ushort value that is the current height of the texture in pixels
    */
    virtual    ushort    getHeight(void) const        {return d_height;}

    /*!
    \brief
        Loads the specified image file into the texture.  The texture is resized as required to hold the image.

    \param filename
        The filename of the image file that is to be loaded into the texture

    \param resourceGroup
        Resource group identifier passed to the resource provider.

    \return
        Nothing.
    */
    virtual void    loadFromFile(const String& filename, const String& resourceGroup);


    /*!
    \brief
        Loads (copies) an image in memory into the texture.  The texture is resized as required to hold the image.

    \param buffPtr
        Pointer to the buffer containing the image data

    \param buffWidth
        Width of the buffer (in pixels as specified by \a pixelFormat )

    \param buffHeight
        Height of the buffer (in pixels as specified by \a pixelFormat )

    \param pixelFormat
        PixelFormat value describing the format contained in \a buffPtr

    \return
        Nothing.
    */
    virtual void    loadFromMemory(const void* buffPtr, uint buffWidth, uint buffHeight, PixelFormat pixelFormat);


    /*!
    \brief
        Return a pointer to the internal Directfb surface object

    \return
        Pointer to the Directfb surface object currently being used by this Texture object
    */
    IDirectFBSurface*    getFBTexture(void) const {return d_fbtexture;}


    /*!
    \brief
            set the size of the internal texture.

    \param size
            pixel size of the new internal texture.  This will be rounded up to a power of 2.

    \return
            Nothing.
    */
    void        setFBTextureSize(uint size);


private:
    /*************************************************************************
        Implementation Data
    *************************************************************************/
    IDirectFBSurface*         d_fbtexture;        //!< The 'real' texture.
    ushort        d_width;            //!< cached width of the texture
    ushort        d_height;            //!< cached height of the texture
    /*************************************************************************
        Implementation Functions
    *************************************************************************/
        // safely free directfb texture (can be called multiple times with no ill effect)
        void    freeFBTexture(void);
};

} // End of  CEGUI namespace section


#endif    // end of guard _directfbtexture_h_
