/************************************************************************
	filename: 	ogrerenderer.cpp
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Renderer class for Ogre engine
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
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIImage.h"
#include "CEGUIExceptions.h"
#include "CEGUISystem.h"

#include "renderers/OgreGUIRenderer/ogrerenderer.h"
#include "renderers/OgreGUIRenderer/ogretexture.h"
#include "renderers/OgreGUIRenderer/OgreResourceProvider.h"


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
OgreRenderer::OgreRenderer(Ogre::RenderWindow* window, Ogre::RenderQueueGroupID queue_id, bool post_queue, uint max_quads, Ogre::SceneType scene_type)
{
	constructor_impl(window, queue_id, post_queue, max_quads);

	// hook into ogre rendering system
	setTargetSceneManager(scene_type);
}


/*************************************************************************
	Constructor (specifying scene manager)
*************************************************************************/
OgreRenderer::OgreRenderer(Ogre::RenderWindow* window, Ogre::RenderQueueGroupID queue_id, bool post_queue, uint max_quads, Ogre::SceneManager* scene_manager)
{
	constructor_impl(window, queue_id, post_queue, max_quads);

	// hook into ogre rendering system
	setTargetSceneManager(scene_manager);
}


/*************************************************************************
	Destructor
*************************************************************************/
OgreRenderer::~OgreRenderer(void)
{
	setTargetSceneManager(NULL);

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
		d_sorted = false;
		QuadInfo quad;
		
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

		// covert colours for ogre, note that top / bottom are switched.
		quad.topLeftCol		= colourToOgre(colours.d_bottom_left);
		quad.topRightCol	= colourToOgre(colours.d_bottom_right);
		quad.bottomLeftCol	= colourToOgre(colours.d_top_left);
		quad.bottomRightCol	= colourToOgre(colours.d_top_right);

		d_quadlist.insert(quad);
	}
}



/*************************************************************************
perform final rendering for all queued renderable quads.
*************************************************************************/
void OgreRenderer::doRender(void)
{
	if (d_render_sys->_getViewport()->getOverlaysEnabled())
	{
		sortQuads();
		initRenderStates();

		// clear this in case we think it's untouched from last frame
		d_currTexture.setNull();

		bool locked = false;
		QuadVertex*	buffmem;

		// iterate over each quad in the list
		for (QuadList::iterator i = d_quadlist.begin(); i != d_quadlist.end(); ++i)
		{
			const QuadInfo& quad = (*i);
			// flush & set texture if needed
			if (d_currTexture != quad.texture)
			{
				if (locked)
				{
					d_buffer->unlock();
					locked = false;
				}

				// render any remaining quads for current texture
				renderVBuffer();

				// set new texture
				d_render_sys->_setTexture(0, true, quad.texture->getName());
				d_currTexture = quad.texture;
			}

			// lock the vertex buffer if it is not already locked
			if (!locked)
			{
				buffmem = (QuadVertex*)d_buffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);
				locked = true;
			}

			// setup Vertex 1...
			buffmem->x = quad.position.d_left;
			buffmem->y = quad.position.d_bottom;
			buffmem->z = quad.z;
			buffmem->diffuse = quad.topLeftCol;
			buffmem->tu1 = quad.texPosition.d_left;
			buffmem->tv1 = quad.texPosition.d_bottom;
			++buffmem;

			// setup Vertex 2...
			buffmem->x = quad.position.d_right;
			buffmem->y = quad.position.d_bottom;
			buffmem->z = quad.z;
			buffmem->diffuse = quad.topRightCol;
			buffmem->tu1 = quad.texPosition.d_right;
			buffmem->tv1 = quad.texPosition.d_bottom;
			++buffmem;

			// setup Vertex 3...
			buffmem->x = quad.position.d_left;
			buffmem->y = quad.position.d_top;
			buffmem->z = quad.z;
			buffmem->diffuse = quad.bottomLeftCol;
			buffmem->tu1 = quad.texPosition.d_left;
			buffmem->tv1 = quad.texPosition.d_top;
			++buffmem;

			// setup Vertex 4...
			buffmem->x = quad.position.d_right;
			buffmem->y = quad.position.d_bottom;
			buffmem->z = quad.z;
			buffmem->diffuse = quad.topRightCol;
			buffmem->tu1 = quad.texPosition.d_right;
			buffmem->tv1 = quad.texPosition.d_bottom;
			++buffmem;

			// setup Vertex 5...
			buffmem->x = quad.position.d_right;
			buffmem->y = quad.position.d_top;
			buffmem->z = quad.z;
			buffmem->diffuse = quad.bottomRightCol;
			buffmem->tu1 = quad.texPosition.d_right;
			buffmem->tv1 = quad.texPosition.d_top;
			++buffmem;

			// setup Vertex 6...
			buffmem->x = quad.position.d_left;
			buffmem->y = quad.position.d_top;
			buffmem->z = quad.z;
			buffmem->diffuse = quad.bottomLeftCol;
			buffmem->tu1 = quad.texPosition.d_left;
			buffmem->tv1 = quad.texPosition.d_top;
			++buffmem;

			// update position within buffer for next time
			d_bufferPos += VERTEX_PER_QUAD;

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

}


/*************************************************************************
	clear the queue
*************************************************************************/
void OgreRenderer::clearRenderList(void)
{
	d_sorted = true;
	d_quadlist.clear();
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
	d_render_sys->unbindGpuProgram(GPT_FRAGMENT_PROGRAM);
	d_render_sys->unbindGpuProgram(GPT_VERTEX_PROGRAM);
	d_render_sys->setShadingType(SO_GOURAUD);
	d_render_sys->_setRasterisationMode(SDL_SOLID);

	// initialise texture settings
	d_render_sys->_setTextureCoordCalculation(0, TEXCALC_NONE);
	d_render_sys->_setTextureCoordSet(0, 0);
	d_render_sys->_setTextureUnitFiltering(0, FO_LINEAR, FO_LINEAR, FO_POINT);
	d_render_sys->_setTextureAddressingMode(0, TextureUnitState::TAM_CLAMP);
	d_render_sys->_setTextureMatrix(0, Matrix4::IDENTITY);
	d_render_sys->_setAlphaRejectSettings(CMPF_ALWAYS_PASS, 0);
	d_render_sys->_setTextureBlendMode(0, d_colourBlendMode);
	d_render_sys->_setTextureBlendMode(0, d_alphaBlendMode);
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
		d_sorted = true;
	}

}


/*************************************************************************
render a quad directly to the display
*************************************************************************/
void OgreRenderer::renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours)
{
	if (d_render_sys->_getViewport()->getOverlaysEnabled())
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

		// convert colours for ogre, note that top / bottom are switched.
        Ogre::uint32 topLeftCol	= colourToOgre(colours.d_bottom_left);
        Ogre::uint32 topRightCol	= colourToOgre(colours.d_bottom_right);
        Ogre::uint32 bottomLeftCol	= colourToOgre(colours.d_top_left);
        Ogre::uint32 bottomRightCol= colourToOgre(colours.d_top_right);

		//
		// perform rendering...
		//
		initRenderStates();
		d_render_sys->_setTexture(0, true, ((OgreTexture*)tex)->getOgreTexture()->getName());
		QuadVertex*	buffmem = (QuadVertex*)d_buffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);

		// setup Vertex 1...
		buffmem->x	= final_rect.d_left;
		buffmem->y	= final_rect. d_bottom;
		buffmem->z	= z;
		buffmem->diffuse = topLeftCol;
		buffmem->tu1	= texture_rect.d_left;
		buffmem->tv1	= texture_rect.d_bottom;
		++buffmem;

		// setup Vertex 2...
		buffmem->x	= final_rect.d_right;
		buffmem->y	= final_rect.d_bottom;
		buffmem->z	= z;
		buffmem->diffuse = topRightCol;
		buffmem->tu1	= texture_rect.d_right;
		buffmem->tv1	= texture_rect.d_bottom;
		++buffmem;

		// setup Vertex 3...
		buffmem->x	= final_rect.d_left;
		buffmem->y	= final_rect.d_top;
		buffmem->z	= z;
		buffmem->diffuse = bottomLeftCol;
		buffmem->tu1	= texture_rect.d_left;
		buffmem->tv1	= texture_rect.d_top;
		++buffmem;

		// setup Vertex 4...
		buffmem->x	= final_rect.d_right;
		buffmem->y	= final_rect.d_bottom;
		buffmem->z	= z;
		buffmem->diffuse = topRightCol;
		buffmem->tu1	= texture_rect.d_right;
		buffmem->tv1	= texture_rect.d_bottom;
		++buffmem;

		// setup Vertex 5...
		buffmem->x	= final_rect.d_right;
		buffmem->y	= final_rect.d_top;
		buffmem->z	= z;
		buffmem->diffuse = bottomRightCol;
		buffmem->tu1	= texture_rect.d_right;
		buffmem->tv1	= texture_rect.d_top;
		++buffmem;

		// setup Vertex 6...
		buffmem->x	= final_rect.d_left;
		buffmem->y	= final_rect.d_top;
		buffmem->z	= z;
		buffmem->diffuse = bottomLeftCol;
		buffmem->tu1	= texture_rect.d_left;
		buffmem->tv1	= texture_rect.d_top;

		d_buffer->unlock();
		d_bufferPos = VERTEX_PER_QUAD;

		renderVBuffer();
	}

}


/*************************************************************************
	convert ARGB colour value to whatever the Ogre render system is
	expecting.	
*************************************************************************/
Ogre::uint32 OgreRenderer::colourToOgre(const colour& col) const
{
	Ogre::ColourValue cv(col.getRed(), col.getGreen(), col.getBlue(), col.getAlpha());

    Ogre::uint32 final;
	d_render_sys->convertColourValue(cv, &final);

	return final;
}


/*************************************************************************
	Set the scene manager to be used for rendering the GUI.	
*************************************************************************/
void OgreRenderer::setTargetSceneManager(Ogre::SceneType scene_type)
{
	setTargetSceneManager(d_ogre_root->getSceneManager(scene_type));
}


/*************************************************************************
	Set the scene manager to be used for rendering the GUI.	
*************************************************************************/
void OgreRenderer::setTargetSceneManager(Ogre::SceneManager* scene_manager)
{
	// unhook from current scene manager.
	if (d_sceneMngr != NULL)
	{
		d_sceneMngr->removeRenderQueueListener(d_ourlistener);
		d_sceneMngr = NULL;
	}

	// hook new scene manager if that is not NULL
	if (scene_manager != NULL)
	{
		d_sceneMngr = scene_manager;
		d_sceneMngr->addRenderQueueListener(d_ourlistener);
	}

}


/*************************************************************************
	Set the target render queue for GUI rendering.	
*************************************************************************/
void OgreRenderer::setTargetRenderQueue(Ogre::RenderQueueGroupID queue_id, bool post_queue)
{
	d_queue_id		= queue_id;
	d_post_queue	= post_queue;

	if (d_ourlistener != NULL)
	{
		d_ourlistener->setTargetRenderQueue(queue_id);
		d_ourlistener->setPostRenderQueue(post_queue);
	}

}



/*************************************************************************
	perform main work of the constructor
*************************************************************************/
void OgreRenderer::constructor_impl(Ogre::RenderWindow* window, Ogre::RenderQueueGroupID queue_id, bool post_queue, uint max_quads)
{
	using namespace Ogre;

	// initialise the renderer fields
	d_queueing		= true;
	d_queue_id		= queue_id;
	d_currTexture.isNull();
	d_post_queue	= post_queue;
	d_sceneMngr		= NULL;
	d_bufferPos		= 0;
	d_sorted		= true;
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
	d_buffer = HardwareBufferManager::getSingleton().createVertexBuffer(vd->getVertexSize(0), VERTEXBUFFER_CAPACITY, HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE, false);

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

	// initialise required texel offset
	d_texelOffset = Point((float)d_render_sys->getHorizontalTexelOffset(), -(float)d_render_sys->getVerticalTexelOffset());

	// create listener which will handler the rendering side of things for us.
	d_ourlistener = new OgreRQListener(this, queue_id, post_queue);

	// Initialise blending modes to be used.
	d_colourBlendMode.blendType	= Ogre::LBT_COLOUR;
	d_colourBlendMode.source1	= Ogre::LBS_TEXTURE;
	d_colourBlendMode.source2	= Ogre::LBS_DIFFUSE;
	d_colourBlendMode.operation	= Ogre::LBX_MODULATE;

	d_alphaBlendMode.blendType	= Ogre::LBT_ALPHA;
	d_alphaBlendMode.source1	= Ogre::LBS_TEXTURE;
	d_alphaBlendMode.source2	= Ogre::LBS_DIFFUSE;
	d_alphaBlendMode.operation	= Ogre::LBX_MODULATE;
}


/*************************************************************************
	Create a texture from an existing Ogre::Texture object	
*************************************************************************/
Texture* OgreRenderer::createTexture(Ogre::Texture* texture)
{
	OgreTexture* t = (OgreTexture*)createTexture();

	if (texture != NULL)
	{
		t->setOgreTexture(*texture);
	}

	return t;

}

/*************************************************************************
	Create a resource provider object
*************************************************************************/
ResourceProvider* OgreRenderer::createResourceProvider(void)
{
    d_resourceProvider = new OgreResourceProvider();
    return d_resourceProvider;
}

/*************************************************************************
	Callback from Ogre invoked before other stuff in our target queue
	is rendered
*************************************************************************/
void OgreRQListener::renderQueueStarted(Ogre::RenderQueueGroupID id, bool& skipThisQueue)
{
	if ((!d_post_queue) && (d_queue_id == id))
	{
		CEGUI::System::getSingleton().renderGUI();
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
		CEGUI::System::getSingleton().renderGUI();
	}

}

} // End of  CEGUI namespace section
