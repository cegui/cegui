/************************************************************************
	filename: 	ogrerenderer.cpp
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Renderer class for Ogre engine
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#include "renderers/OgreGUIRenderer/ogrerenderer.h"
#include "renderers/OgreGUIRenderer/ogretexture.h"

#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"

#include "CEGUIExceptions.h"
#include "CEGUISystem.h"

#include <algorithm>


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants definitions
*************************************************************************/
const int	OgreRenderer::VERTEX_PER_QUAD			= 6;
const int	OgreRenderer::VERTEX_PER_TRIANGLE		= 3;
const int	OgreRenderer::VERTEXBUFFER_CAPACITY		= 4096;


/*************************************************************************
	Constructor
*************************************************************************/
OgreRenderer::OgreRenderer(Ogre::RenderWindow* window, Ogre::RenderQueueGroupID queue_id, bool post_queue, uint max_quads) :
	d_queue_id(queue_id),
	d_post_queue(post_queue),
	d_quadBuffSize(max_quads),
	d_queueing(true),
	d_currTexture(NULL),
	d_bufferPos(0),
	d_sorted(true)
{
	using namespace Ogre;

	d_ogre_root		= Root::getSingletonPtr();
	d_render_sys	= d_ogre_root->getRenderSystem();

	// Create and initialise the Ogre specific parts required for use in rendering later.
	d_render_op.vertexData = new VertexData;
	d_render_op.vertexData->vertexStart = 0;

	// setup vertex declaration for the vertex format we use
	VertexDeclaration* vd = d_render_op.vertexData->vertexDeclaration;
	size_t vd_offset = 0;
	vd->addElement(0, vd_offset, VET_FLOAT3, VES_POSITION);
	vd_offset += VertexElement::getTypeSize(VET_FLOAT3);
	vd->addElement(0, vd_offset, VET_COLOUR, VES_DIFFUSE);
	vd_offset += VertexElement::getTypeSize(VET_COLOUR);
	vd->addElement(0, vd_offset, VET_FLOAT2, VES_TEXTURE_COORDINATES);

	// create hardware vertex buffer
	d_buffer = HardwareBufferManager::getSingleton().createVertexBuffer(vd->getVertexSize(0), VERTEXBUFFER_CAPACITY, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY, false);

	// bind vertex buffer
	d_render_op.vertexData->vertexBufferBinding->setBinding(0, d_buffer);

	// complete render operation basic initialisation
	d_render_op.operationType = RenderOperation::OT_TRIANGLE_LIST;
	d_render_op.useIndexes = false;

	// Discover display settings and setup d_display_area
	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= window->getWidth();
	d_display_area.d_bottom	= window->getHeight();

	// initialise quad buffer
	d_quadBuffPos = 0;
	d_quadBuff = new QuadInfo[max_quads + 1];	// NB: alloc 1 extra QuadInfo to simplify management if we try to overrun
	d_quadList = new QuadInfo*[max_quads];

	// initialise required texel offset
	d_texelOffset = Point((float)d_render_sys->getHorizontalTexelOffset(), (float)d_render_sys->getVerticalTexelOffset());

	// create listener which will handler the rendering side of things for us.
	d_ourlistener = new OgreRQListener(this, queue_id, post_queue);

	// hook into ogre rendering system
	d_ogre_root->getSceneManager(ST_GENERIC)->addRenderQueueListener(d_ourlistener);
}


/*************************************************************************
	Destructor
*************************************************************************/
OgreRenderer::~OgreRenderer(void)
{
	if (d_ourlistener)
	{
		delete d_ourlistener;
	}

	// cleanup vertex data we allocated in constructor
	delete d_render_op.vertexData;

	destroyAllTextures();
}


/*************************************************************************
	add's a quad to the list to be rendered
*************************************************************************/
void OgreRenderer::addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
{
	// if not queueing, render directly (as in, right now!)
	if (!d_queueing)
	{
		renderQuadDirect(dest_rect, z, tex, texture_rect, colours);
	}
	else
	{
		if (d_quadBuffPos >= d_quadBuffSize)
		{
			return;
		}

		d_sorted = false;

		QuadInfo& quad = d_quadBuff[d_quadBuffPos];
		
		// set quad position, flipping y co-ordinates, and applying appropriate texel origin offset
		quad.position.d_left	= dest_rect.d_left;
		quad.position.d_right	= dest_rect.d_right;
		quad.position.d_top		= d_display_area.getHeight() - dest_rect.d_top;
		quad.position.d_bottom	= d_display_area.getHeight() - dest_rect.d_bottom;
		quad.position.offset(d_texelOffset);

		// convert quad co-ordinates for a -1 to 1 co-ordinate system.
		quad.position.d_left	/= (d_display_area.getWidth() * 0.5f);
		quad.position.d_right	/= (d_display_area.getWidth() * 0.5f);
		quad.position.d_top		/= (d_display_area.getHeight() * 0.5f);
		quad.position.d_bottom	/= (d_display_area.getHeight() * 0.5f);
		quad.position.offset(Point(-1.0f, -1.0f));

		quad.z				= -1 + z;
		quad.texture		= ((OgreTexture*)tex)->getOgreTexture();
		quad.texPosition	= texture_rect;
		quad.colours		= colours;

		d_quadList[d_quadBuffPos] = &quad;
		d_quadBuffPos++;
	}
}



/*************************************************************************
	perform final rendering for all queued renderable quads.
*************************************************************************/
void OgreRenderer::doRender(void)
{
	sortQuads();
	initRenderStates();

	// clear this in case we think it's untouched from last frame
	d_currTexture = NULL;

	bool locked = false;
	QuadVertex*	buffmem;
	QuadInfo*	quad;

	// iterate over each quad in the list
	for (int i = 0; i < d_quadBuffPos; ++i)
	{
		quad = d_quadList[i];

		// flush & set texture if needed
		if (d_currTexture != quad->texture)
		{
			if (locked)
			{
				d_buffer->unlock();
				locked = false;
			}

			// render any remaining quads for current texture
			renderVBuffer();

			// set new texture
			d_render_sys->_setTexture(0, true, quad->texture->getName());
			d_currTexture = quad->texture;
		}

		// lock the vertex buffer if it is not already locked
		if (!locked)
		{
			buffmem = (QuadVertex*)d_buffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);
			locked = true;
		}

		// setup Vertex 1...
		(&buffmem[0])->x	= quad->position.d_left;
		(&buffmem[0])->y	= quad->position.d_bottom;
		(&buffmem[0])->z	= quad->z;
		(&buffmem[0])->diffuse = (quad->colours.d_top_left);
		(&buffmem[0])->tu1	= quad->texPosition.d_left;
		(&buffmem[0])->tv1	= quad->texPosition.d_bottom;

		// setup Vertex 2...
		(&buffmem[1])->x	= quad->position.d_right;
		(&buffmem[1])->y	= quad->position.d_bottom;
		(&buffmem[1])->z	= quad->z;
		(&buffmem[1])->diffuse = (quad->colours.d_top_right);
		(&buffmem[1])->tu1	= quad->texPosition.d_right;
		(&buffmem[1])->tv1	= quad->texPosition.d_bottom;

		// setup Vertex 3...
		(&buffmem[2])->x	= quad->position.d_left;
		(&buffmem[2])->y	= quad->position.d_top;
		(&buffmem[2])->z	= quad->z;
		(&buffmem[2])->diffuse = (quad->colours.d_bottom_left);
		(&buffmem[2])->tu1	= quad->texPosition.d_left;
		(&buffmem[2])->tv1	= quad->texPosition.d_top;

		// setup Vertex 4...
		(&buffmem[3])->x	= quad->position.d_right;
		(&buffmem[3])->y	= quad->position.d_bottom;
		(&buffmem[3])->z	= quad->z;
		(&buffmem[3])->diffuse = (quad->colours.d_top_right);
		(&buffmem[3])->tu1	= quad->texPosition.d_right;
		(&buffmem[3])->tv1	= quad->texPosition.d_bottom;

		// setup Vertex 5...
		(&buffmem[4])->x	= quad->position.d_right;
		(&buffmem[4])->y	= quad->position.d_top;
		(&buffmem[4])->z	= quad->z;
		(&buffmem[4])->diffuse = (quad->colours.d_bottom_right);
		(&buffmem[4])->tu1	= quad->texPosition.d_right;
		(&buffmem[4])->tv1	= quad->texPosition.d_top;

		// setup Vertex 6...
		(&buffmem[5])->x	= quad->position.d_left;
		(&buffmem[5])->y	= quad->position.d_top;
		(&buffmem[5])->z	= quad->z;
		(&buffmem[5])->diffuse = (quad->colours.d_bottom_left);
		(&buffmem[5])->tu1	= quad->texPosition.d_left;
		(&buffmem[5])->tv1	= quad->texPosition.d_top;

		// update position within buffer for next time
		d_bufferPos += VERTEX_PER_QUAD;
		buffmem		+= VERTEX_PER_QUAD;

		// if there is not enough room in the buffer for another sprite, render what we have
		if (d_bufferPos >= (VERTEXBUFFER_CAPACITY - VERTEX_PER_QUAD))
		{
			if (locked)
			{
				d_buffer->unlock();
				locked = false;
			}

			renderVBuffer();
		}

	}

	// ensure we leave the buffer in the unlocked state
	if (locked)
	{
		d_buffer->unlock();
		locked = false;
	}

	// send any remaining data to be rendered.
	renderVBuffer();
}


/*************************************************************************
	clear the queue
*************************************************************************/
void OgreRenderer::clearRenderList(void)
{
	d_sorted = true;
	d_quadBuffPos = 0;
}


/*************************************************************************
	create an empty texture
*************************************************************************/
Texture* OgreRenderer::createTexture(void)
{
	OgreTexture* tex = new OgreTexture(this);
	d_texturelist.push_back(tex);
	return tex;
}


/*************************************************************************
	create a texture and load it with the specified file.
*************************************************************************/
Texture* OgreRenderer::createTexture(const String& filename)
{
	OgreTexture* tex = (OgreTexture*)createTexture();
	tex->loadFromFile(filename);

	return tex;
}


/*************************************************************************
	create a texture and set it to the specified size
*************************************************************************/
Texture* OgreRenderer::createTexture(float size)
{
	OgreTexture* tex = (OgreTexture*)createTexture();
	tex->setOgreTextureSize((uint)size);

	return tex;
}


/*************************************************************************
	destroy the given texture
*************************************************************************/
void OgreRenderer::destroyTexture(Texture* texture)
{
	if (texture != NULL)
	{
		OgreTexture* tex = (OgreTexture*)texture;

		d_texturelist.remove(tex);
		delete tex;
	}
}


/*************************************************************************
	destroy all textures still active
*************************************************************************/
void OgreRenderer::destroyAllTextures(void)
{
	while (!d_texturelist.empty())
	{
		destroyTexture(*(d_texturelist.begin()));
	}
}


/*************************************************************************
	setup states etc	
*************************************************************************/
void OgreRenderer::initRenderStates(void)
{
	using namespace Ogre;

	// set-up matrices
	d_render_sys->_setWorldMatrix(Matrix4::IDENTITY);
	d_render_sys->_setViewMatrix(Matrix4::IDENTITY);
	d_render_sys->_setProjectionMatrix(Matrix4::IDENTITY);

	// initialise render settings
	d_render_sys->setLightingEnabled(false);
	d_render_sys->_setDepthBufferParams(false, false);
	d_render_sys->_setCullingMode(CULL_NONE);
	d_render_sys->_setFog(FOG_NONE);
	d_render_sys->_setColourBufferWriteEnabled(true, true, true, true);

	// initialise texture settings
	d_render_sys->_setTextureCoordSet(0, 0);
	d_render_sys->_setTextureUnitFiltering(0, FO_POINT, FO_POINT, FO_POINT);
	d_render_sys->_setTextureAddressingMode(0, TextureUnitState::TAM_CLAMP);
	d_render_sys->_setTextureMatrix(0, Matrix4::IDENTITY);
	d_render_sys->_setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0);
	d_render_sys->_disableTextureUnitsFrom(1);

	// enable alpha blending
	d_render_sys->_setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA);
}


/*************************************************************************
	renders whatever is in the vertex buffer	
*************************************************************************/
void OgreRenderer::renderVBuffer(void)
{
	// we only need to do anything if there is actually quads in the buffer
	if (d_bufferPos != 0)
	{
		// tell render operation how many quads to draw
		d_render_op.vertexData->vertexCount = d_bufferPos;
		d_render_sys->_render(d_render_op);

		// reset buffer
		d_bufferPos = 0;
	}

}

 
/*************************************************************************
	sort quads list according to texture	
*************************************************************************/
void OgreRenderer::sortQuads(void)
{
	if (!d_sorted)
	{
		std::stable_sort(d_quadList, d_quadList + d_quadBuffPos, quadsorter());
		d_sorted = true;
	}

}


/*************************************************************************
	render a quad directly to the display
*************************************************************************/
void OgreRenderer::renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
{
	z = -1 + z;

	Rect final_rect;

	// set quad position, flipping y co-ordinates, and applying appropriate texel origin offset
	final_rect.d_left	= dest_rect.d_left;
	final_rect.d_right	= dest_rect.d_right;
	final_rect.d_top	= d_display_area.getHeight() - dest_rect.d_top;
	final_rect.d_bottom	= d_display_area.getHeight() - dest_rect.d_bottom;
	final_rect.offset(d_texelOffset);

	// convert quad co-ordinates for a -1 to 1 co-ordinate system.
	final_rect.d_left	/= (d_display_area.getWidth() * 0.5f);
	final_rect.d_right	/= (d_display_area.getWidth() * 0.5f);
	final_rect.d_top	/= (d_display_area.getHeight() * 0.5f);
	final_rect.d_bottom	/= (d_display_area.getHeight() * 0.5f);
	final_rect.offset(Point(-1.0f, -1.0f));

	//
	// perform rendering...
	//
	initRenderStates();
	d_render_sys->_setTexture(0, true, ((OgreTexture*)tex)->getOgreTexture()->getName());
	QuadVertex*	buffmem = (QuadVertex*)d_buffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);

	// setup Vertex 1...
	(&buffmem[0])->x	= final_rect.d_left;
	(&buffmem[0])->y	= final_rect. d_bottom;
	(&buffmem[0])->z	= z;
	(&buffmem[0])->diffuse = (colours.d_top_left);
	(&buffmem[0])->tu1	= texture_rect.d_left;
	(&buffmem[0])->tv1	= texture_rect.d_bottom;

	// setup Vertex 2...
	(&buffmem[1])->x	= final_rect.d_right;
	(&buffmem[1])->y	= final_rect.d_bottom;
	(&buffmem[1])->z	= z;
	(&buffmem[1])->diffuse = (colours.d_top_right);
	(&buffmem[1])->tu1	= texture_rect.d_right;
	(&buffmem[1])->tv1	= texture_rect.d_bottom;

	// setup Vertex 3...
	(&buffmem[2])->x	= final_rect.d_left;
	(&buffmem[2])->y	= final_rect.d_top;
	(&buffmem[2])->z	= z;
	(&buffmem[2])->diffuse = (colours.d_bottom_left);
	(&buffmem[2])->tu1	= texture_rect.d_left;
	(&buffmem[2])->tv1	= texture_rect.d_top;

	// setup Vertex 4...
	(&buffmem[3])->x	= final_rect.d_right;
	(&buffmem[3])->y	= final_rect.d_bottom;
	(&buffmem[3])->z	= z;
	(&buffmem[3])->diffuse = (colours.d_top_right);
	(&buffmem[3])->tu1	= texture_rect.d_right;
	(&buffmem[3])->tv1	= texture_rect.d_bottom;

	// setup Vertex 5...
	(&buffmem[4])->x	= final_rect.d_right;
	(&buffmem[4])->y	= final_rect.d_top;
	(&buffmem[4])->z	= z;
	(&buffmem[4])->diffuse = (colours.d_bottom_right);
	(&buffmem[4])->tu1	= texture_rect.d_right;
	(&buffmem[4])->tv1	= texture_rect.d_top;

	// setup Vertex 6...
	(&buffmem[5])->x	= final_rect.d_left;
	(&buffmem[5])->y	= final_rect.d_top;
	(&buffmem[5])->z	= z;
	(&buffmem[5])->diffuse = (colours.d_bottom_left);
	(&buffmem[5])->tu1	= texture_rect.d_left;
	(&buffmem[5])->tv1	= texture_rect.d_top;

	d_buffer->unlock();
	d_bufferPos = VERTEX_PER_QUAD;

	renderVBuffer();
}


/*************************************************************************
	Callback from Ogre invoked before other stuff in our target queue
	is rendered
*************************************************************************/
void OgreRQListener::renderQueueStarted(Ogre::RenderQueueGroupID id, bool& skipThisQueue)
{
	if ((!d_post_queue) && (d_queue_id == id))
	{
		System::getSingleton().renderGUI();
	}

}


/*************************************************************************
Callback from Ogre invoked after other stuff in our target queue
is rendered
*************************************************************************/
void OgreRQListener::renderQueueEnded(Ogre::RenderQueueGroupID id, bool& repeatThisQueue)
{
	if ((d_post_queue) && (d_queue_id == id))
	{
		System::getSingleton().renderGUI();
	}

}


} // End of  CEGUI namespace section
