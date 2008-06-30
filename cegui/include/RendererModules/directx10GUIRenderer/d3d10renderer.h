/***********************************************************************
    filename:   d3d10renderer.h
    created:    17/7/2004
    author:     Paul D Turner with D3D 9 Updates by Magnus Österlind
    D3D10 Port by Rajko Stojadinovic

    purpose:    Interface for DirectX 10.0 Renderer class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
/*************************************************************************
    This file contains code that is specific to Win32 and DirectX
*************************************************************************/
#ifndef _d3d10renderer_h_
#define _d3d10renderer_h_

#include "CEGUIBase.h"
#include "CEGUIRenderer.h"
#include "CEGUITexture.h"

// Little hack to allow D3D10 to compile on a non-Vista. Of course, actually using this
// renderer on XP is out of the question. These fixes are not needed in VC9 anymore.
#if _MSC_VER <= 1400 // VC8 and lower
	
	// One fix for VC8
	#define WINAPI_INLINE WINAPI

	#if _MSC_VER < 1400 // VC7.1 and lower

		#define __out /**/
		#define __in /**/
		#define __inout /**/

		#define __in_opt /**/
		#define __out_opt /**/
		#define __inout_opt /**/

		#define __in_ecount(THING) /**/
		#define __in_ecount_opt(THING) /**/
		#define __out_ecount(THING) /**/
		#define __out_ecount_opt(THING) /**/
		#define __inout_ecount(THING) /**/
		#define __inout_ecount_opt(THING) /**/

		#define __in_bcount_opt(THING) /**/
		#define __out_bcount_opt(THING) /**/
		#define __inout_bcount_opt(THING) /**/
	
	#endif // < 1400

#endif // <= 1400

#include <d3d10.h>
#include <list>
#include <set>

typedef ID3D10Buffer        IDirect3DIndexBuffer;
typedef ID3D10Buffer        IDirect3DVertexBuffer;
typedef D3D10_VIEWPORT      D3DVIEWPORT;
typedef ID3D10VertexShader 	IDirect3DVertexShader;
typedef ID3D10PixelShader 	IDirect3DPixelShader;
typedef ID3D10Device*       LPDIRECT3DDEVICE;
typedef ID3D10Texture2D*    LPDIRECT3DTEXTURE2D;
typedef ID3D10Buffer*       LPDIRECT3DBUFFER;

#if !defined(CEGUI_STATIC)
	#ifdef DIRECTX10_GUIRENDERER_EXPORTS
	#define DIRECTX10_GUIRENDERER_API __declspec(dllexport)
	#else
	#define DIRECTX10_GUIRENDERER_API __declspec(dllimport)
	#endif
#else
	#define DIRECTX10_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
    Forward refs
*************************************************************************/
class DirectX10Texture;

/*!
\brief
    Renderer class to interface with Microsoft DirectX 10.0
*/
class DIRECTX10_GUIRENDERER_API DirectX10Renderer : public Renderer
{
public:
    /*!
    \brief
        Constructor for Direct3D 10.0 Renderer object

    \param device
        Pointer to the IDirect3DDevice10 interface object that will be used for
        all rendering

    \param max_quads
        Obsolete.  Set to 0.
    */
    DirectX10Renderer(LPDIRECT3DDEVICE device, uint max_quads);

    /*!
    \brief
        Destructor for DirectX10Renderer objects
    */
    virtual ~DirectX10Renderer(void);

    // add a quad to the list to be rendered
    virtual void addQuad(const Rect& dest_rect, float z, const Texture* tex,
                         const Rect& texture_rect, const ColourRect& colours,
                         QuadSplitMode quad_split_mode);

    // perform final rendering for all queued renderable quads.
    virtual void doRender(void);

    // clear the queue
    virtual void clearRenderList(void);

    /*!
    \brief
        Enable or disable the queueing of quads from this point on.

        This only affects queueing.  If queueing is turned off, any calls to
        addQuad will cause the quad to be rendered directly.  Note that
        disabling queueing will not cause currently queued quads to be rendered,
        nor is the queue cleared - at any time the queue can still be drawn by
        calling doRender, and the list can be cleared by calling
        clearRenderList.  Re-enabling the queue causes subsequent quads to be
        added as if queueing had never been disabled.

    \param setting
        - true to enable queueing
        - false to disable queueing (see notes above).

    \return
        Nothing
    */
    virtual void setQueueingEnabled(bool setting)
        { d_queueing = setting; }

    // create an empty texture
    virtual Texture* createTexture(void);

    // create a texture and load it with the specified file.
    virtual Texture* createTexture(const String& filename,
                                   const String& resourceGroup);

    // create a texture and set it to the specified size
    virtual Texture* createTexture(float size);

    // destroy the given texture
    virtual void destroyTexture(Texture* texture);

    // destroy all textures still active
    virtual void destroyAllTextures(void);

    //! return a pointer to the D3D device.
    LPDIRECT3DDEVICE getDevice(void) const
        { return d_device; }

    /*!
    \brief
        Return whether queueing is enabled.

    \return
        - true if queueing is enabled.
        - false if queueing is disabled.
    */
    virtual bool isQueueingEnabled(void) const
        { return d_queueing; }

    /*!
    \brief
        Return the current width of the display in pixels.

    \return
        float value equal to the current width of the display in pixels.
    */
    virtual float getWidth(void) const
        { return d_display_area.getWidth(); }

    /*!
    \brief
        Return the current height of the display in pixels.

    \return
        float value equal to the current height of the display in pixels.
    */
    virtual float getHeight(void) const
        { return d_display_area.getHeight(); }

    /*!
    \brief
        Return the size of the display in pixels.

    \return
        Size object describing the dimensions of the current display.
    */
    virtual Size getSize(void) const
        { return d_display_area.getSize(); }


    /*!
    \brief
        Return a Rect describing the screen.

    \return
        A Rect object that describes the screen area.  Typically, the top-left
        values are always 0, and the size of the area described is equal to the
        screen resolution.
    */
    virtual Rect getRect(void) const
        { return d_display_area; }

    /*!
    \brief
        Return the maximum texture size available

    \return
        Size of the maximum supported texture in pixels
        (textures are always assumed to be square)
    */
    virtual uint getMaxTextureSize(void) const
        { return d_maxTextureSize; }

    /*!
    \brief
        Return the horizontal display resolution dpi.

    \return
        horizontal resolution of the display in dpi.
    */
    virtual uint getHorzScreenDPI(void) const
        { return 96; }

    /*!
    \brief
        Return the vertical display resolution dpi.

    \return
        vertical resolution of the display in dpi.
    */
    virtual uint getVertScreenDPI(void) const
        { return 96; }

    /*!
    \brief
        Set the size of the display in pixels.

        You do not have to call this method under normal operation as the system
        will automatically extract the size from the current view port.

    \note
        This method will cause the EventDisplaySizeChanged event to fire if the
        display size has changed.

    \param sz
        Size object describing the size of the display.

    \return
        Nothing.
    */
    void setDisplaySize(const Size& sz);

private:
    /************************************************************************
        Implementation Constants
    ************************************************************************/
    //! number of vertices per quad.
    static const int VERTEX_PER_QUAD;
    //! number of vertices for a triangle.
    static const int VERTEX_PER_TRIANGLE;
    //! capacity of the allocated vertex buffer.
    static const int VERTEXBUFFER_CAPACITY;

    /*************************************************************************
        Implementation Structs & classes
    *************************************************************************/
    //! FVF structure used for all vertices.
    struct QuadVertex {
        //! The pre-transformed position for the vertex.
        FLOAT x, y, z;
        //! colour of the vertex.
        DWORD diffuse;
        //! texture coordinates.
        float tu1, tv1;
    };

    //! structure holding details about a quad to be drawn.
    struct QuadInfo
    {
        LPDIRECT3DTEXTURE2D texture;
        ID3D10ShaderResourceView* resview;
        Rect position;
        float z;
        Rect texPosition;
        ulong topLeftCol;
        ulong topRightCol;
        ulong bottomLeftCol;
        ulong bottomRightCol;
        QuadSplitMode splitMode;

        bool operator<(const QuadInfo& other) const
        {
            // this is intentionally reversed.
            return z > other.z;
        }
    };

    /*************************************************************************
        Implementation Methods
    *************************************************************************/
    //! renders whatever is in the vertex buffer
    void renderVBuffer(ID3D10ShaderResourceView *tex);

    //! render a quad directly to the display
    void renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex,
                          const Rect& texture_rect, const ColourRect& colours,
                          QuadSplitMode quad_split_mode);

    //! return size of device view port (if possible)
    Size getViewportSize(void);

    //! method to do work of constructor
    void constructor_impl(LPDIRECT3DDEVICE device, const Size& display_size);

    HRESULT CreateVertexBuffer();

    /*************************************************************************
        Implementation Data
    *************************************************************************/
    //! Rect describing the current display area used by the renderer.
    Rect d_display_area;
    //! Typedef for the container where queued quads are kept.
    typedef std::multiset<QuadInfo> QuadList;
    //! Container to hold the queued quads.
    QuadList d_quadlist;
    //! setting for queueing control.
    bool d_queueing;
    //! Base Direct3DDevice10 interface that we use for rendering.
    LPDIRECT3DDEVICE d_device;
    //! currently set texture.
    LPDIRECT3DTEXTURE2D d_currTexture;
    ID3D10ShaderResourceView* d_currSRView;
    //! vertex buffer to queue sprite rendering.
    LPDIRECT3DBUFFER d_buffer;
    //! index into buffer where next vertex should be put.
    int d_bufferPos;
    ID3D10InputLayout* d_InputLayout;
    // D3DX effect interface.
    ID3D10Effect* d_Effect;
    ID3D10EffectTechnique* d_Technique;
    ID3D10EffectShaderResourceVariable* d_DiffuseTexVariable;
    ID3D10EffectScalarVariable* d_WindowHeight;
    ID3D10EffectScalarVariable* d_WindowWidth;
    //! List used to track textures.
    std::list<DirectX10Texture*> d_texturelist;
    //! Holds maximum supported texture size (in pixels).
    uint d_maxTextureSize;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _d3d10renderer_h_
