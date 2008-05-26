/***********************************************************************
	filename: 	irrlichtrenderer.h
	created:	20/7/2004
	author:		Thomas Suter
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUI_IrrlichtRenderer_h_
#define _CEGUI_IrrlichtRenderer_h_

#include "IrrlichtRendererDef.h"
#include "irrlichttexture.h"
#include "IrrlichtResourceProvider.h"

#include "CEGUIRenderer.h"
#include "CEGUIInputEvent.h"

#include <irrlicht.h>

#include <vector>
#include <algorithm>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{

	class EventPusher;

	/*!
	\brief
	class implementing the interface for Renderer objects with
	the irrlicht graphics engine.
	*/
	class IRRLICHT_GUIRENDERER_API IrrlichtRenderer: public Renderer
	{
	public:

		/*! constructor
		\brief create the irrlicht renderer

		\param dev
		pointer to irr::IrrlichtDevice value specifying the irrlicht device

		\param bWithIrrlichtResourceProvicer
		bool specifying wether to use an irrlicht- or defautresourceprovider (default)
		*/

		IrrlichtRenderer(irr::IrrlichtDevice* dev,bool bWithIrrlichtResourceProvicer=false);

		/*!	destructor */
		virtual ~IrrlichtRenderer();


		/*! get an irrlicht resource provider
		\return irrlicht resourceprovider
		*/
		virtual ResourceProvider* createResourceProvider(void);

		/*! forward event to CEGUI system */
		bool OnEvent(const irr::SEvent& event);


		/*************************************************************************
		Abstract interface methods
		*************************************************************************/
		/*!
		\brief
		Add a quad to the rendering queue.  All clipping and other adjustments should have been made prior to calling this.

		\param dest_rect
		Rect object describing the destination area (values are in pixels)

		\param z
		float value specifying the z co-ordinate / z order of the quad

		\param tex
		pointer to the Texture object that holds the imagery to be rendered

		\param texture_rect
		Rect object holding the area of \a tex that is to be rendered (values are in texture co-ordinates).

		\param colours
		ColourRect object describing the colour values that are to be applied when rendering.

		\param quad_split_mode
		One of the QuadSplitMode values specifying the way quads are split into triangles

		\return
		Nothing
		*/
		virtual	void	addQuad(const Rect& dest_rect, float z, const Texture* tex,
			const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode);


		/*!
		\brief
		Perform final rendering for all quads that have been queued for rendering

		The contents of the rendering queue is retained and can be rendered again as required.  If the contents is not required call clearRenderList().

		\return
		Nothing
		*/
		virtual	void	doRender(void);


		/*!
		\brief
		Clears all queued quads from the render queue.

		\return
		Nothing
		*/
		virtual	void	clearRenderList(void);


		/*!
		\brief
		Enable or disable the queueing of quads from this point on.

		This only affects queueing.  If queueing is turned off, any calls to addQuad will cause the quad to be rendered directly.  Note that
		disabling queueing will not cause currently queued quads to be rendered, nor is the queue cleared - at any time the queue can still
		be drawn by calling doRender, and the list can be cleared by calling clearRenderList.  Re-enabling the queue causes subsequent quads
		to be added as if queueing had never been disabled.

		\param setting
		true to enable queueing, or false to disable queueing (see notes above).

		\return
		Nothing
		*/
		virtual void	setQueueingEnabled(bool setting);


		/*!
		\brief
		Creates a 'null' Texture object.

		\return
		a newly created Texture object.  The returned Texture object has no size or imagery associated with it, and is
		generally of little or no use.
		*/
		virtual	Texture*	createTexture(void);


		/*!
		\brief
		Create a Texture object using the given image file.

		\param filename
		String object that specifies the path and filename of the image file to use when creating the texture.

		\param resourceGroup
		Resource group identifier passed to the resource provider.

		\return
		a newly created Texture object.  The initial contents of the texture memory is the requested image file.

		\note
		Textures are always created with a size that is a power of 2.  If the file you specify is of a size that is not
		a power of two, the final size will be rounded up.  Additionally, textures are always square, so the ultimate
		size is governed by the larger of the width and height of the specified file.  You can check the ultimate sizes
		by querying the texture after creation.
		*/
		virtual	Texture*	createTexture(const String& filename, const String& resourceGroup);


		/*!
		\brief
		Create a Texture object with the given pixel dimensions as specified by \a size.  NB: Textures are always square.

		\param size
		float value that specifies the size to use for the width and height when creating the new texture.

		\return
		a newly created Texture object.  The initial contents of the texture memory is undefined / random.

		\note
		Textures are always created with a size that is a power of 2.  If you specify a size that is not a power of two, the final
		size will be rounded up.  So if you specify a size of 1024, the texture will be (1024 x 1024), however, if you specify a size
		of 1025, the texture will be (2048 x 2048).  You can check the ultimate size by querying the texture after creation.
		*/
		virtual	Texture*	createTexture(float size);


		/*!
		\brief
		Destroy the given Texture object.

		\param texture
		pointer to the Texture object to be destroyed

		\return
		Nothing
		*/
		virtual	void		destroyTexture(Texture* texture);


		/*!
		\brief
		Destroy all Texture objects.

		\return
		Nothing
		*/
		virtual void		destroyAllTextures(void);


		/*!
		\brief
		Return whether queueing is enabled.

		\return
		true if queueing is enabled, false if queueing is disabled.
		*/
		virtual bool	isQueueingEnabled(void) const;


		/*!
		\brief
		Return the current width of the display in pixels

		\return
		float value equal to the current width of the display in pixels.
		*/
		virtual float	getWidth(void) const;


		/*!
		\brief
		Return the current height of the display in pixels

		\return
		float value equal to the current height of the display in pixels.
		*/
		virtual float	getHeight(void) const;


		/*!
		\brief
		Return the size of the display in pixels

		\return
		Size object describing the dimensions of the current display.
		*/
		virtual Size	getSize(void) const;


		/*!
		\brief
		Return a Rect describing the screen

		\return
		A Rect object that describes the screen area.  Typically, the top-left values are always 0, and the size of the area described is
		equal to the screen resolution.
		*/
		virtual Rect	getRect(void) const;


		/*!
		\brief
		Return the maximum texture size available

		\return
		Size of the maximum supported texture in pixels (textures are always assumed to be square)
		*/
		virtual	uint	getMaxTextureSize(void) const;


		/*!
		\brief
		Return the horizontal display resolution dpi

		\return
		horizontal resolution of the display in dpi.
		*/
		virtual	uint	getHorzScreenDPI(void) const;


		/*!
		\brief
		Return the vertical display resolution dpi

		\return
		vertical resolution of the display in dpi.
		*/
		virtual	uint	getVertScreenDPI(void) const;

    /*!
    \brief
        Set the size of the display in pixels.

        If your viewport size changes, you can call this function with the new size
        in pixels to update the rendering area.

    \note
        This method will cause the EventDisplaySizeChanged event to fire if the
        display size has changed.

    \param sz
        Size object describing the size of the display.
    */
    void setDisplaySize(const Size& sz);

		private:

			// the irrlicht device
			irr::IrrlichtDevice* device;
			// video driver
			irr::video::IVideoDriver* driver;
			// window width,height
			irr::core::dimension2d<irr::s32> resolution;
			// screen resolution
			irr::core::dimension2d<irr::s32> screensize;

			// is queueing enabled
			bool bQueuingEnabled;
			// is quad queue sorted
			bool bSorted;
			// enable irrlicht resource provider
			bool bWithIrrlichtResourceProvicer;

			// quad structure used for rendering the gui
			struct RenderQuad
			{
				RenderQuad(){};

				RenderQuad(float zVal,
					const irr::core::rect<irr::s32>& target,
					const irr::core::rect<irr::s32>& source,
					ColourRect col,const Texture*t)
					:z(zVal),dst(target),src(source),colours(col){
						tex=(IrrlichtTexture*)t;
					};

				float z;
				irr::core::rect<irr::s32> dst;
				irr::core::rect<irr::s32> src;
				ColourRect colours;
				IrrlichtTexture* tex;
			};

			RenderQuad dummyQuad;

			// std sorting RenderQuad class
			struct quadsorter
				: public std::binary_function<RenderQuad*, RenderQuad*, bool>
			{
				bool operator()(const RenderQuad& _Left, const RenderQuad& _Right) const
				{return (_Left.z > _Right.z);}
			};

			// list ot quads we want to render
			std::vector<RenderQuad> renderlist;

			// list of textures used for rendering
			std::vector<IrrlichtTexture*> textures;

			// sort the quads in the renderlist
			void sortQuads();

			// render the quad
			void doRender(RenderQuad& quad);

			// print some debug output
			void print(RenderQuad& quad);

			// convert cegui colour to irrlicht scolor
			inline irr::video::SColor toIrrlichtColor(CEGUI::ulong cecolor);
			irr::video::SColor colors[4];



			EventPusher* eventpusher;

	};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif
