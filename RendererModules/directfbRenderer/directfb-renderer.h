/***********************************************************************
    filename:   directfb-renderer.h
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
#ifndef _directfbrenderer_h_
#define _directfbrenderer_h_

#include "CEGUIBase.h"
#include <directfb.h>

#include <vector>
#include <list>

#include "CEGUIRenderer.h"
#include "CEGUITexture.h"


// Start of CEGUI namespace section
namespace CEGUI
{
  /*************************************************************************
    Forward refs
  *************************************************************************/
  class DirectfbTexture;
  class ImageCodec;
  class DynamicModule;

  /*!
    \brief
    Renderer class to interface with Directfb
  */
  class DirectfbRenderer : public Renderer
  {
  public:
    /*!
      \brief
      Constructor for Directfb Renderer object

      \param window
      A pointer to the Directfb surface

    */
    DirectfbRenderer(IDirectFB *device, IDirectFBSurface *surface, ImageCodec* codec = 0);

    /*!
      \brief
      Destructor for DirectfbRenderer objects
    */
    virtual ~DirectfbRenderer(void);

    // add's a quad to the list to be rendered
    virtual    void    addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode);

    // perform final rendering for all queued renderable quads.
    virtual    void    doRender(void);

    // clear the queue
    virtual    void    clearRenderList(void);


    /*!
      \brief
      Enable or disable the queuing of quads from this point on.

      This only affects queuing.  If queuing is turned off, any calls to addQuad will cause the quad to be rendered directly.  Note that
      disabling queuing will not cause currently queued quads to be rendered, nor is the queue cleared - at any time the queue can still
      be drawn by calling doRender, and the list can be cleared by calling clearRenderList.  Re-enabling the queue causes subsequent quads
      to be added as if queuing had never been disabled.

      \param setting
      true to enable queuing, or false to disable queuing (see notes above).

      \return
      Nothing
    */
    virtual void    setQueueingEnabled(bool setting)        {d_queueing = setting;}


    // create an empty texture
    virtual    Texture*    createTexture(void);

    // create a texture and load it with the specified file.
    virtual    Texture*    createTexture(const String& filename, const String& resourceGroup);

    // create a texture and set it to the specified size
    virtual    Texture*    createTexture(float size);

    // destroy the given texture
    virtual    void        destroyTexture(Texture* texture);

    // destroy all textures still active
    virtual void        destroyAllTextures(void);

    // return ptr to device
    IDirectFB*    getDevice(void) const           {return d_device;}


    /*!
      \brief
      Return whether queuing is enabled.

      \return
      true if queuing is enabled, false if queuing is disabled.
    */
    virtual bool    isQueueingEnabled(void) const    {return d_queueing;}


    /*!
      \brief
      Return the current width of the display in pixels

      \return
      float value equal to the current width of the display in pixels.
    */
    virtual float    getWidth(void) const        {return d_display_area.getWidth();}


    /*!
      \brief
      Return the current height of the display in pixels

      \return
      float value equal to the current height of the display in pixels.
    */
    virtual float    getHeight(void) const        {return d_display_area.getHeight();}


    /*!
      \brief
      Return the size of the display in pixels

      \return
      Size object describing the dimensions of the current display.
    */
    virtual Size    getSize(void) const            {return d_display_area.getSize();}


    /*!
      \brief
      Return a Rect describing the screen

      \return
      A Rect object that describes the screen area.  Typically, the top-left values are always 0, and the size of the area described is
      equal to the screen resolution.
    */
    virtual Rect    getRect(void) const            {return d_display_area;}


    /*!
      \brief
      Return the maximum texture size available

      \return
      Size of the maximum supported texture in pixels (textures are always assumed to be square)
    */
    virtual    uint    getMaxTextureSize(void) const        {return d_maxTextureSize;}


    /*!
      \brief
      Return the horizontal display resolution dpi

      \return
      horizontal resolution of the display in dpi.
    */
    virtual    uint    getHorzScreenDPI(void) const    {return 96;}


    /*!
      \brief
      Return the vertical display resolution dpi

      \return
      vertical resolution of the display in dpi.
    */
    virtual    uint    getVertScreenDPI(void) const    {return 96;}

    /*!
      \brief
      Retrieve the image codec used internaly
    */
    ImageCodec& getImageCodec(void);


    /*!
      \brief
      Set the image codec to use for loading textures
    */
    void setImageCodec(const String& codecName);

    /*!
     \brief
     Set the image codec to use from an existing image codec.

     In this case the renderer does not take the ownership of the
     image codec object.

     \param codec a pointer to an image codec object
     */
    void setImageCodec(ImageCodec* codec);


    /*!
      \brief
      Set the name of the default image codec to be used
    */
    static void setDefaultImageCodecName(const String& codecName);

    /*!
      \brief
      Get the name of the default image codec
    */
    static const String& getDefaultImageCodecName();


  private:
    /************************************************************************
        Implementation Constants
    ************************************************************************/

    /*************************************************************************
        Implementation Structs & classes
    *************************************************************************/
    /*!
      \brief
      structure holding details about a quad to be drawn
    */
    struct RenderQuad
    {
      IDirectFBSurface    *tex;
      Rect        position;
      float        z;
      Rect        texPosition;
      ColourRect    colours;
    };


    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    // render a quad directly to the display
    void     doRender(RenderQuad *quad);
    void     sortQuads(void);

    // setup image codec
    void setupImageCodec(const String& codecName);

    // cleanup image codec
    void cleanupImageCodec();


    /*************************************************************************
        Implementation Data
    *************************************************************************/
    // std sorting RenderQuad class
    struct quadsorter
        : public std::binary_function<RenderQuad*, RenderQuad*, bool>
    {
        bool operator()(const RenderQuad& _Left, const RenderQuad& _Right) const
        {return (_Left.z > _Right.z);}
    };

    // list ot quads we want to render
    std::vector<RenderQuad> d_quadlist;


    IDirectFBSurface    *d_surface;
    IDirectFB        *d_device;
    Rect        d_display_area;
    bool        d_queueing;            //!< setting for queuing control.

    std::list<DirectfbTexture*>    d_texturelist;        //!< List used to track textures.
    uint    d_maxTextureSize;        //!< Holds maximum supported texture size (in pixels).

    ImageCodec* d_imageCodec;           //!< Holds a pointer to the image codec to use.
    DynamicModule* d_imageCodecModule; //!< Holds a pointer to the image codec module. If d_imageCodecModule is 0 we are not owner of the image codec object

    static String d_defaultImageCodecName; //!< Holds the name of the default codec to use

  };

} // End of  CEGUI namespace section

#endif    // end of guard _openglrenderer_h_
