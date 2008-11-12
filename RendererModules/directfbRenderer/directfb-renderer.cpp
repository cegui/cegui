/***********************************************************************
    filename:   directfb-renderer.cpp
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "directfb-renderer.h"
#include "directfb-texture.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"
#include "CEGUIImageCodec.h"
#include "CEGUIDynamicModule.h"

//Include the default codec for static builds
#if defined(CEGUI_STATIC)
#       if defined(CEGUI_CODEC_SILLY)
#               include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
#       elif defined(CEGUI_CODEC_TGA)
#               include "../../ImageCodecModules/TGAImageCodec/CEGUITGAImageCodecModule.h"
#       elif defined(CEGUI_CODEC_CORONA)
#               include "../../ImageCodecModules/CoronaImageCodec/CEGUICoronaImageCodecModule.h"
#       elif defined(CEGUI_CODEC_DEVIL)
#               include "../../ImageCodecModules/DevILImageCodec/CEGUIDevILImageCodecModule.h"
#       elif defined(CEGUI_CODEC_FREEIMAGE)
#               include "../../ImageCodecModules/FreeImageImageCodec/CEGUIFreeImageImageCodecModule.h"
#       else //Make Silly the default
#               include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
#       endif
#endif

#define S_(X) #X
#define STRINGIZE(X) S_(X)


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Constants definitions
*************************************************************************/
/*************************************************************************
    Constructor
*************************************************************************/
DirectfbRenderer::DirectfbRenderer(IDirectFB *device, IDirectFBSurface *surface, ImageCodec*  codec)
{
    int width, height;
    d_imageCodec = codec;
    d_imageCodecModule = 0;
    d_queueing      = true;
    d_maxTextureSize = 4096;
    d_surface = surface;
    d_device = device;

        d_display_area.d_left   = 0;
        d_display_area.d_top    = 0;
    surface->GetSize(surface, &width, &height);
        d_display_area.d_right  = width;
        d_display_area.d_bottom = height;

    if (!d_imageCodec)
        setupImageCodec("");

    d_identifierString = "CEGUI::DirectfbRenderer - Official Directfb based renderer module for CEGUI";
}

/*************************************************************************
    Destructor
*************************************************************************/
DirectfbRenderer::~DirectfbRenderer(void)
{
    destroyAllTextures();
    cleanupImageCodec();
}

/*************************************************************************
    add's a quad to the list to be rendered
*************************************************************************/
void DirectfbRenderer::addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode)
{
    /*
       doesn't support for drawing mode selection
       so 'quad_split_mode' is neglected at the moment
    */

    RenderQuad quad;
    quad.position            = dest_rect;
    quad.z                = z;
    quad.tex            = ((DirectfbTexture*)tex)->getFBTexture();
    quad.texPosition        = texture_rect;
    quad.texPosition.d_top        *= tex->getHeight();
    quad.texPosition.d_bottom    *= tex->getHeight();
    quad.texPosition.d_left        *= tex->getWidth();
    quad.texPosition.d_right    *= tex->getWidth();
    quad.colours            = colours;

    // if not queuing, render directly (as in, right now!)
    if (!d_queueing)
    {
        doRender(&quad);
    }
    else
    {
        d_quadlist.push_back(quad);
    }
}


/*************************************************************************
    perform final rendering for all queued renderable quads.
*************************************************************************/
void DirectfbRenderer::doRender(void)
{
    sortQuads();
    std::vector<RenderQuad>::iterator iter=d_quadlist.begin();
    for(;iter!=d_quadlist.end();++iter) doRender(&(*iter));
}


void DirectfbRenderer::doRender(RenderQuad *quad)
{
    IDirectFBSurface *texture = quad->tex;
    DFBRectangle src_rect, dest_rect;
    u8 r,g,b,a;

    if((quad->colours.d_top_left != quad->colours.d_bottom_left) ||
       (quad->colours.d_bottom_right != quad->colours.d_top_right) ||
       (quad->colours.d_top_left != quad->colours.d_bottom_right)) {
        Logger::getSingleton().logEvent("DirectfbRenderer can only handle same colour for four the vertex, top-left colour valid only");
    }
    r = static_cast<u8>(quad->colours.d_top_left.getRed() * 255);
    g = static_cast<u8>(quad->colours.d_top_left.getGreen() * 255);
    b = static_cast<u8>(quad->colours.d_top_left.getBlue() * 255);
    a = static_cast<u8>(quad->colours.d_top_left.getAlpha() * 255);

    texture->SetColor(texture, r, g, b, a);
    if(r==255 && g==255 &&  b==255 && a==255) {
        d_surface->SetBlittingFlags(d_surface, DSBLIT_BLEND_ALPHACHANNEL);
    } else {
        d_surface->SetBlittingFlags(d_surface, DSBLIT_BLEND_COLORALPHA);
    }
    src_rect.x = static_cast<int>(quad->texPosition.d_left);
    src_rect.w = static_cast<int>(quad->texPosition.d_right - quad->texPosition.d_left);
    src_rect.y = static_cast<int>(quad->texPosition.d_top);
    src_rect.h = static_cast<int>(quad->texPosition.d_bottom - quad->texPosition.d_top);
    dest_rect.x = static_cast<int>(quad->position.d_left);
    dest_rect.w = static_cast<int>(quad->position.d_right - quad->position.d_left);
    dest_rect.y = static_cast<int>(quad->position.d_top);
    dest_rect.h = static_cast<int>(quad->position.d_bottom - quad->position.d_top);
    d_surface->StretchBlit(d_surface, texture, &src_rect, &dest_rect);
}

/*************************************************************************
    clear the queue
*************************************************************************/
void DirectfbRenderer::clearRenderList(void)
{
    d_quadlist.clear();
}


/*************************************************************************
    create an empty texture
*************************************************************************/
Texture* DirectfbRenderer::createTexture(void)
{
    DirectfbTexture* tex = new DirectfbTexture(this);
    d_texturelist.push_back(tex);
    return tex;
}


/*************************************************************************
    Create a new Texture object and load a file into it.
*************************************************************************/
Texture* DirectfbRenderer::createTexture(const String& filename, const String& resourceGroup)
{
    DirectfbTexture* tex = new DirectfbTexture(this);
    try
    {
        tex->loadFromFile(filename, resourceGroup);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_texturelist.push_back(tex);
    return tex;
}


/*************************************************************************
    Create a new texture with the given dimensions
*************************************************************************/
Texture* DirectfbRenderer::createTexture(float size)
{
    DirectfbTexture* tex = new DirectfbTexture(this);
    try
    {
        tex->setFBTextureSize((uint)size);
    }
    catch (RendererException&)
    {
        delete tex;
        throw;
    }
    d_texturelist.push_back(tex);
    return tex;
}


/*************************************************************************
    Destroy a texture
*************************************************************************/
void DirectfbRenderer::destroyTexture(Texture* texture)
{
    if (texture)
    {
        DirectfbTexture* tex = (DirectfbTexture*)texture;
        d_texturelist.remove(tex);
        delete tex;
    }
}


/*************************************************************************
    destroy all textures still active
*************************************************************************/
void DirectfbRenderer::destroyAllTextures(void)
{
    while (!d_texturelist.empty())
    {
        destroyTexture(*(d_texturelist.begin()));
    }
}

/*************************************************************************
 *         sort quads list according to texture
 **************************************************************************/
void DirectfbRenderer::sortQuads(void)
{
    std::stable_sort(d_quadlist.begin(),d_quadlist.end(), quadsorter());
}


/***********************************************************************
    Get the current ImageCodec object used
************************************************************************/
ImageCodec& DirectfbRenderer::getImageCodec()
{
    return *d_imageCodec;
}
/***********************************************************************
    Set the current ImageCodec object used
************************************************************************/
void DirectfbRenderer::setImageCodec(const String& codecName)
{
    setupImageCodec(codecName);
}
/***********************************************************************
    Set the current ImageCodec object used
************************************************************************/
void DirectfbRenderer::setImageCodec(ImageCodec* codec)
{
    if (codec)
    {
        cleanupImageCodec();
        d_imageCodec = codec;
        d_imageCodecModule = 0;
    }
}
/***********************************************************************
    setup the ImageCodec object used
************************************************************************/
void DirectfbRenderer::setupImageCodec(const String& codecName)
{

    // Cleanup the old image codec
    if (d_imageCodec)
        cleanupImageCodec();

    // Test whether we should use the default codec or not
    if (codecName.empty())
                //If we are statically linking the default codec will already be in the system
#if defined(CEGUI_STATIC)
                d_imageCodecModule = 0;
#else
                d_imageCodecModule = new DynamicModule(String("CEGUI") + d_defaultImageCodecName);
#endif
    else
        d_imageCodecModule = new DynamicModule(String("CEGUI") + codecName);

        //Check to make sure we have a module...
        if(d_imageCodecModule)
        {
                // Create the codec object itself
                ImageCodec* (*createFunc)(void) =
                        (ImageCodec* (*)(void))d_imageCodecModule->getSymbolAddress("createImageCodec");
                d_imageCodec = createFunc();
        } // if(d_imageCodecModule)
        else
        {
#if defined(CEGUI_STATIC)
                d_imageCodec = createImageCodec();
#else
                throw InvalidRequestException("Unable to load codec " + codecName);
#endif
        }

}
/***********************************************************************
    cleanup the ImageCodec object used
************************************************************************/
void DirectfbRenderer::cleanupImageCodec()
{
    if (d_imageCodec && d_imageCodecModule)
    {
        void(*deleteFunc)(ImageCodec*) =
            (void(*)(ImageCodec*))d_imageCodecModule->getSymbolAddress("destroyImageCodec");
        deleteFunc(d_imageCodec);
        d_imageCodec = 0;
        delete d_imageCodecModule;
        d_imageCodecModule = 0;
    } // if (d_imageCodec && d_imageCodecModule)
        else
        {
#if defined(CEGUI_STATIC)
                destroyImageCodec(d_imageCodec);
#endif
        }

}
/***********************************************************************
    set the default ImageCodec name
************************************************************************/
void DirectfbRenderer::setDefaultImageCodecName(const String& codecName)
{
    d_defaultImageCodecName = codecName;
}

/***********************************************************************
    get the default ImageCodec name to be used
************************************************************************/
const String& DirectfbRenderer::getDefaultImageCodecName()
{
    return d_defaultImageCodecName;

}

/***********************************************************************
    store the name of the default ImageCodec
************************************************************************/
String DirectfbRenderer::d_defaultImageCodecName(STRINGIZE(CEGUI_DEFAULT_IMAGE_CODEC));



} // End of  CEGUI namespace section


