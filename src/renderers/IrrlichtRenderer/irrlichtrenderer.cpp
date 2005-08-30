/************************************************************************
	filename: 	irrlichtrenderer.cpp
	created:	20/7/2004
	author:		Thomas Suter
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "renderers/IrrlichtRenderer/irrlichtrenderer.h"
#include "IrrlichtEventPusher.h"
#include "CEGUIDefaultResourceProvider.h"
#include <sstream>

namespace CEGUI
{
/************************************************************************/
	IrrlichtRenderer::IrrlichtRenderer(irr::IrrlichtDevice* dev, bool bWithIrrlichtResourceProvicer): Renderer(),device(dev)
	{
		this->bWithIrrlichtResourceProvicer=bWithIrrlichtResourceProvicer;
		d_resourceProvider=0;
		driver=device->getVideoDriver();
		resolution=driver->getScreenSize(); // @todo use active viewport!!
		screensize=device->getVideoModeList()->getDesktopResolution();
		bSorted=false;
		bQueuingEnabled=true;
		eventpusher=new EventPusher(device->getCursorControl());

        // set ID string
        d_identifierString = "CEGUI::IrrlichtRenderer - Official Irrlicht based renderer module for CEGUI";
	}
/************************************************************************/
	IrrlichtRenderer::~IrrlichtRenderer()
	{
		delete eventpusher;
	};
/************************************************************************/
	void IrrlichtRenderer::addQuad(const Rect& dest_rect, float z, const Texture* tex, 
		const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode)
	{

		/* 
		irrlicht doesn't support for drawing mode selection 
		so 'quad_split_mode' is neglected at the moment
		*/

		irr::u32 tex_height=tex->getHeight();
		irr::u32 tex_width=tex->getWidth();

		dummyQuad.src.UpperLeftCorner.X=texture_rect.d_left*tex_width;
		dummyQuad.src.UpperLeftCorner.Y=(texture_rect.d_top)*tex_height-1;
		dummyQuad.src.LowerRightCorner.X=texture_rect.d_right*tex_width;
		dummyQuad.src.LowerRightCorner.Y=texture_rect.d_bottom*tex_height-1;

		dummyQuad.dst.UpperLeftCorner.X=dest_rect.d_left;
		dummyQuad.dst.UpperLeftCorner.Y=dest_rect.d_top-1;
		dummyQuad.dst.LowerRightCorner.X=dest_rect.d_right;
		dummyQuad.dst.LowerRightCorner.Y=dest_rect.d_bottom-1;

		dummyQuad.z=z;
		dummyQuad.colours=colours;
		dummyQuad.tex=(IrrlichtTexture*)tex;
		
		if(bQueuingEnabled)
		{
			renderlist.push_back(dummyQuad);
			bSorted=false;
		}
		else
		{
			doRender(dummyQuad);
		}
	}
/************************************************************************/
	void IrrlichtRenderer::print(RenderQuad& quad)
	{
		//char buf[255];
		//irr::video::ITexture* texture=((IrrlichtTexture*)quad.tex)->getTexture();
		//sprintf(buf,"renderquad(dst(%d,%d-%d,%d) src(%d,%d-%d,%d) tex(%x-%d,%d))",
		//	quad.dst.UpperLeftCorner.X,quad.dst.UpperLeftCorner.Y,
		//	quad.dst.LowerRightCorner.X,quad.dst.LowerRightCorner.Y,
		//	quad.src.UpperLeftCorner.X,quad.src.UpperLeftCorner.Y,
		//	quad.src.LowerRightCorner.X,quad.src.LowerRightCorner.Y,
		//	(unsigned long)texture,texture->getSize().Width,texture->getSize().Height);
		//device->getLogger()->log(buf,irr::ELL_INFORMATION);
	}
/************************************************************************/
	void IrrlichtRenderer::sortQuads(void)
	{
		if (!bSorted)
		{
			std::stable_sort(renderlist.begin(),renderlist.end(), quadsorter());
			bSorted = true;
		}
	}
/************************************************************************/
	void IrrlichtRenderer::doRender(void)
	{
		sortQuads();
		std::vector<RenderQuad>::iterator iter=renderlist.begin();
		for(;iter!=renderlist.end();++iter) doRender( (*iter) );
	}
/************************************************************************/
	void IrrlichtRenderer::doRender(RenderQuad& quad)
	{
		irr::video::ITexture* texture=((IrrlichtTexture*)quad.tex)->getTexture();
		colors[0].color=quad.colours.d_top_left.getARGB();
		colors[1].color=quad.colours.d_bottom_left.getARGB();
		colors[2].color=quad.colours.d_bottom_right.getARGB();
		colors[3].color=quad.colours.d_top_right.getARGB();
		driver->draw2DImage(texture,quad.dst,quad.src,0,colors,true);
	}
/************************************************************************/
	irr::video::SColor IrrlichtRenderer::toIrrlichtColor(CEGUI::ulong cecolor)
	{
		return irr::video::SColor(
			((cecolor >> 24) ) , 
			(((cecolor & 0x00FF0000) >> 16) ) ,
			(((cecolor & 0x0000FF00) >> 8) ) ,
			((cecolor & 0x000000FF) )
			);
	}
/************************************************************************/
	void IrrlichtRenderer::clearRenderList(void)
	{
		renderlist.resize(0);
	}
/************************************************************************/
	void IrrlichtRenderer::setQueueingEnabled(bool setting)
	{
		bQueuingEnabled=setting;
	}
/************************************************************************/
	Texture* IrrlichtRenderer::createTexture(void)
	{
		IrrlichtTexture* t=new IrrlichtTexture(this,device);
		textures.push_back(t);
		return t;
	}
/************************************************************************/
	Texture* IrrlichtRenderer::createTexture(const String& filename, const String& resourceGroup)
	{
		IrrlichtTexture* t=(IrrlichtTexture*)createTexture();
		t->loadFromFile(filename, resourceGroup);
		return t;
	}
/************************************************************************/	
	Texture* IrrlichtRenderer::createTexture(float size)
	{
		IrrlichtTexture* t=(IrrlichtTexture*)createTexture();
		return t;
	}
/************************************************************************/
	void IrrlichtRenderer::destroyTexture(Texture* texture)
	{
		std::vector<IrrlichtTexture*>::iterator iter=textures.begin();
		for(;iter!=textures.end();++iter)
		{
			IrrlichtTexture* t=(*iter);
			if(t==texture)
			{
				delete t;
				textures.erase(iter);
				return;
			}
		}
	}
/************************************************************************/
	void IrrlichtRenderer::destroyAllTextures(void)
	{
		std::vector<IrrlichtTexture*>::iterator iter=textures.begin();
		for(;iter!=textures.end();)
		{
			IrrlichtTexture* t=(*iter);
			delete t;
			iter=textures.erase(iter);
		}
	}
/************************************************************************/
	bool IrrlichtRenderer::isQueueingEnabled(void) const
	{
		return bQueuingEnabled;
	}
/************************************************************************/
	float IrrlichtRenderer::getWidth(void) const
	{
		return resolution.Width;
	}
/************************************************************************/
	float IrrlichtRenderer::getHeight(void) const
	{
		return resolution.Height;
	}
/************************************************************************/
	Size IrrlichtRenderer::getSize(void) const
	{
		return Size(resolution.Width,resolution.Height);
	}
/************************************************************************/
	Rect IrrlichtRenderer::getRect(void) const
	{
		return Rect(0,0,resolution.Width,resolution.Height);
	}
/************************************************************************/
	uint IrrlichtRenderer::getMaxTextureSize(void) const
	{
		return 2048;
	}
/************************************************************************/
	uint IrrlichtRenderer::getHorzScreenDPI(void) const
	{
		return 96;
	}
/************************************************************************/
	uint IrrlichtRenderer::getVertScreenDPI(void) const
	{
		return 96;
	}
/************************************************************************/
	/*! get an irrlicht resource provider
	\return irrlicht resourceprovider
	*/
	ResourceProvider* IrrlichtRenderer::createResourceProvider(void)
	{
		if(d_resourceProvider==0)
		{
			if(bWithIrrlichtResourceProvicer)
			{
				d_resourceProvider = new IrrlichtResourceProvider(device->getFileSystem());
			}
			else
			{
				d_resourceProvider = new DefaultResourceProvider();
			}
		}
		return d_resourceProvider;
	}

	/*! forward event to CEGUI system */
	bool IrrlichtRenderer::OnEvent(irr::SEvent& event)
	{
		return eventpusher->OnEvent(event);
	}
}
