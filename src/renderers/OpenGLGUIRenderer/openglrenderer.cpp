/************************************************************************
	filename: 	openglrenderer.cpp
	created:	9/4/2004
	author:		Mark Strom
				mwstrom@gmail.com

	purpose:	Interface to Renderer implemented via Opengl
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
#include "renderers/OpenGLGUIRenderer/openglrenderer.h"
#include "renderers/OpenGLGUIRenderer/opengltexture.h"
#include "CEGUIExceptions.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants definitions
*************************************************************************/
const int OpenGLRenderer::VERTEX_PER_QUAD			= 6;
const int OpenGLRenderer::VERTEX_PER_TRIANGLE		= 3;
const int OpenGLRenderer::VERTEXBUFFER_CAPACITY		= OGLRENDERER_VBUFF_CAPACITY;


/*************************************************************************
	Constructor
*************************************************************************/
OpenGLRenderer::OpenGLRenderer(uint max_quads) :
	d_queueing(true),
	d_currTexture(0),
	d_bufferPos(0)
{
	GLint vp[4];   

	// initialise renderer size
	glGetIntegerv(GL_VIEWPORT, vp);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);
	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= (float)vp[2];
	d_display_area.d_bottom	= (float)vp[3];

    setModuleIdentifierString();
}


OpenGLRenderer::OpenGLRenderer(uint max_quads,int width, int height) :
	d_queueing(true),
	d_currTexture(0),
	d_bufferPos(0)
{
	GLint vp[4];   

	// initialise renderer size
	glGetIntegerv(GL_VIEWPORT, vp);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);
	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= static_cast<float>(width);
	d_display_area.d_bottom	= static_cast<float>(height);

    setModuleIdentifierString();
}


/*************************************************************************
	Destructor
*************************************************************************/
OpenGLRenderer::~OpenGLRenderer(void)
{
	destroyAllTextures();
}


/*************************************************************************
	add's a quad to the list to be rendered
*************************************************************************/
void OpenGLRenderer::addQuad(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode)
{
	// if not queuing, render directly (as in, right now!)
	if (!d_queueing)
	{
		renderQuadDirect(dest_rect, z, tex, texture_rect, colours, quad_split_mode);
	}
	else
	{
		QuadInfo quad;
		quad.position			= dest_rect;
		quad.position.d_bottom	= d_display_area.d_bottom - dest_rect.d_bottom;
		quad.position.d_top		= d_display_area.d_bottom - dest_rect.d_top;
		quad.z					= z;
		quad.texid				= ((OpenGLTexture*)tex)->getOGLTexid();
		quad.texPosition		= texture_rect;
		quad.topLeftCol		= colourToOGL(colours.d_top_left);
		quad.topRightCol	= colourToOGL(colours.d_top_right);
		quad.bottomLeftCol	= colourToOGL(colours.d_bottom_left);
		quad.bottomRightCol	= colourToOGL(colours.d_bottom_right);

        // set quad split mode
        quad.splitMode = quad_split_mode;

		d_quadlist.insert(quad);
	}

}



/*************************************************************************
	perform final rendering for all queued renderable quads.
*************************************************************************/
void OpenGLRenderer::doRender(void)
{
	d_currTexture = 0;

	initPerFrameStates();
	glInterleavedArrays(GL_T2F_C4UB_V3F , 0, myBuff);

	// iterate over each quad in the list
	for (QuadList::iterator i = d_quadlist.begin(); i != d_quadlist.end(); ++i)
	{
		const QuadInfo& quad = (*i);

		if(d_currTexture != quad.texid)
		{            
			renderVBuffer();           
			glBindTexture(GL_TEXTURE_2D, quad.texid);
			d_currTexture = quad.texid;          
		}

		//vert0       
		myBuff[d_bufferPos].vertex[0]	= quad.position.d_left;
		myBuff[d_bufferPos].vertex[1]	= quad.position.d_top;
		myBuff[d_bufferPos].vertex[2]	= quad.z;
		myBuff[d_bufferPos].color		= quad.topLeftCol;
		myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_left;
		myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_top;         
		++d_bufferPos;

		//vert1
		myBuff[d_bufferPos].vertex[0]	= quad.position.d_left;
		myBuff[d_bufferPos].vertex[1]	= quad.position.d_bottom;
		myBuff[d_bufferPos].vertex[2]	= quad.z;
		myBuff[d_bufferPos].color		= quad.bottomLeftCol;
		myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_left;
		myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_bottom;
		++d_bufferPos;

		//vert2

        // top-left to bottom-right diagonal
        if (quad.splitMode == TopLeftToBottomRight)
        {
            myBuff[d_bufferPos].vertex[0]	= quad.position.d_right;
            myBuff[d_bufferPos].vertex[1]	= quad.position.d_bottom;
            myBuff[d_bufferPos].vertex[2]	= quad.z;
            myBuff[d_bufferPos].color		= quad.bottomRightCol;
            myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_right;
            myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_bottom;         
        }
        // bottom-left to top-right diagonal
        else
        {
            myBuff[d_bufferPos].vertex[0]	= quad.position.d_right;
            myBuff[d_bufferPos].vertex[1]	= quad.position.d_top;
            myBuff[d_bufferPos].vertex[2]	= quad.z;
            myBuff[d_bufferPos].color		= quad.topRightCol;
            myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_right;
            myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_top;         
        }
		++d_bufferPos;

		//vert3
		myBuff[d_bufferPos].vertex[0]	= quad.position.d_right;
		myBuff[d_bufferPos].vertex[1]	= quad.position.d_top;
		myBuff[d_bufferPos].vertex[2]	= quad.z;
		myBuff[d_bufferPos].color		= quad.topRightCol;
		myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_right;
		myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_top;
		++d_bufferPos;

		//vert4

        // top-left to bottom-right diagonal
        if (quad.splitMode == TopLeftToBottomRight)
        {
            myBuff[d_bufferPos].vertex[0]	= quad.position.d_left;
            myBuff[d_bufferPos].vertex[1]	= quad.position.d_top;
            myBuff[d_bufferPos].vertex[2]	= quad.z;
            myBuff[d_bufferPos].color		= quad.topLeftCol;
            myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_left;
            myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_top;         
        }
        // bottom-left to top-right diagonal
        else
        {
            myBuff[d_bufferPos].vertex[0]	= quad.position.d_left;
            myBuff[d_bufferPos].vertex[1]	= quad.position.d_bottom;
            myBuff[d_bufferPos].vertex[2]	= quad.z;
            myBuff[d_bufferPos].color		= quad.bottomLeftCol;
            myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_left;
            myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_bottom;         
        }
		++d_bufferPos;

		//vert 5
		myBuff[d_bufferPos].vertex[0]	= quad.position.d_right;
		myBuff[d_bufferPos].vertex[1]	= quad.position.d_bottom;
		myBuff[d_bufferPos].vertex[2]	= quad.z;
		myBuff[d_bufferPos].color		= quad.bottomRightCol;
		myBuff[d_bufferPos].tex[0]		= quad.texPosition.d_right;
		myBuff[d_bufferPos].tex[1]		= quad.texPosition.d_bottom;         
		++d_bufferPos;

		if(d_bufferPos > (VERTEXBUFFER_CAPACITY - VERTEX_PER_QUAD))
		{          
			renderVBuffer();
		}

	}

	//Render
	renderVBuffer();

	exitPerFrameStates();
}


/*************************************************************************
	clear the queue
*************************************************************************/
void OpenGLRenderer::clearRenderList(void)
{
	d_quadlist.clear();
}


/*************************************************************************
	create an empty texture
*************************************************************************/
Texture* OpenGLRenderer::createTexture(void)
{
	OpenGLTexture* tex = new OpenGLTexture(this);
	d_texturelist.push_back(tex);
	return tex;
}


/*************************************************************************
	Create a new Texture object and load a file into it.
*************************************************************************/
Texture* OpenGLRenderer::createTexture(const String& filename, const String& resourceGroup)
{
	OpenGLTexture* tex = (OpenGLTexture*)createTexture();
	tex->loadFromFile(filename, resourceGroup);

	return tex;
}


/*************************************************************************
	Create a new texture with the given dimensions
*************************************************************************/
Texture* OpenGLRenderer::createTexture(float size)
{
	OpenGLTexture* tex = (OpenGLTexture*)createTexture();
	tex->setOGLTextureSize((uint)size);

	return tex;
}


/*************************************************************************
	Destroy a texture
*************************************************************************/
void OpenGLRenderer::destroyTexture(Texture* texture)
{
	if (texture != NULL)
	{
		OpenGLTexture* tex = (OpenGLTexture*)texture;
		d_texturelist.remove(tex);
		delete tex;
	}

}


/*************************************************************************
	destroy all textures still active
*************************************************************************/
void OpenGLRenderer::destroyAllTextures(void)
{
	while (!d_texturelist.empty())
	{
		destroyTexture(*(d_texturelist.begin()));
	}
}


/*************************************************************************
	setup states etc
*************************************************************************/
void OpenGLRenderer::initPerFrameStates(void)
{
	//save current attributes
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glPolygonMode(GL_FRONT, GL_FILL);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, d_display_area.d_right, 0.0, d_display_area.d_bottom);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();	

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}


void OpenGLRenderer::exitPerFrameStates(void)
{
	glDisable(GL_TEXTURE_2D);

	glPopMatrix(); 
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);

	//restore former attributes
	glPopClientAttrib();
	glPopAttrib();
}


/*************************************************************************
	renders whatever is in the vertex buffer
*************************************************************************/
void OpenGLRenderer::renderVBuffer(void)
{
	// if bufferPos is 0 there is no data in the buffer and nothing to render
	if (d_bufferPos == 0)
	{
		return;
	}

	// render the sprites
	glDrawArrays(GL_TRIANGLES, 0, d_bufferPos);

	// reset buffer position to 0...
	d_bufferPos = 0;
}


/*************************************************************************
	sort quads list according to texture
*************************************************************************/
void OpenGLRenderer::sortQuads(void)
{
	// no need to do anything here.
}


/*************************************************************************
	render a quad directly to the display
*************************************************************************/
void OpenGLRenderer::renderQuadDirect(const Rect& dest_rect, float z, const Texture* tex, const Rect& texture_rect, const ColourRect& colours, QuadSplitMode quad_split_mode)
{
	QuadInfo quad;
	quad.position.d_left	= dest_rect.d_left;
	quad.position.d_right	= dest_rect.d_right;
	quad.position.d_bottom	= d_display_area.d_bottom - dest_rect.d_bottom;
	quad.position.d_top		= d_display_area.d_bottom - dest_rect.d_top;
	quad.texPosition		= texture_rect;

	quad.topLeftCol		= colourToOGL(colours.d_top_left);
	quad.topRightCol	= colourToOGL(colours.d_top_right);
	quad.bottomLeftCol	= colourToOGL(colours.d_bottom_left);
	quad.bottomRightCol	= colourToOGL(colours.d_bottom_right);

	MyQuad myquad[VERTEX_PER_QUAD];

	initPerFrameStates();
	glInterleavedArrays(GL_T2F_C4UB_V3F , 0, myquad);
	glBindTexture(GL_TEXTURE_2D, ((OpenGLTexture*)tex)->getOGLTexid());

	//vert0
	myquad[0].vertex[0] = quad.position.d_left;
	myquad[0].vertex[1] = quad.position.d_top;
	myquad[0].vertex[2] = z;
	myquad[0].color     = quad.topLeftCol;
	myquad[0].tex[0]    = quad.texPosition.d_left;
	myquad[0].tex[1]    = quad.texPosition.d_top;

	//vert1
	myquad[1].vertex[0] = quad.position.d_left;
	myquad[1].vertex[1] = quad.position.d_bottom;
	myquad[1].vertex[2] = z;
	myquad[1].color     = quad.bottomLeftCol;     
	myquad[1].tex[0]    = quad.texPosition.d_left;
	myquad[1].tex[1]    = quad.texPosition.d_bottom;

	//vert2

    // top-left to bottom-right diagonal
    if (quad_split_mode == TopLeftToBottomRight)
    {
        myquad[2].vertex[0] = quad.position.d_right;
        myquad[2].vertex[1] = quad.position.d_bottom;
        myquad[2].vertex[2] = z;
        myquad[2].color     = quad.bottomRightCol;
        myquad[2].tex[0]    = quad.texPosition.d_right;
        myquad[2].tex[1]    = quad.texPosition.d_bottom;
    }
    // bottom-left to top-right diagonal
    else
    {
        myquad[2].vertex[0] = quad.position.d_right;
        myquad[2].vertex[1] = quad.position.d_top;
        myquad[2].vertex[2] = z;
        myquad[2].color     = quad.topRightCol;
        myquad[2].tex[0]    = quad.texPosition.d_right;
        myquad[2].tex[1]    = quad.texPosition.d_top;
    }

	//vert3
	myquad[3].vertex[0] = quad.position.d_right;
	myquad[3].vertex[1] = quad.position.d_top;
	myquad[3].vertex[2] = z;
	myquad[3].color     = quad.topRightCol;      
	myquad[3].tex[0]    = quad.texPosition.d_right;
	myquad[3].tex[1]    = quad.texPosition.d_top;

	//vert4

    // top-left to bottom-right diagonal
    if (quad_split_mode == TopLeftToBottomRight)
    {
        myquad[4].vertex[0] = quad.position.d_left;
        myquad[4].vertex[1] = quad.position.d_top;
        myquad[4].vertex[2] = z;
        myquad[4].color     = quad.topLeftCol;
        myquad[4].tex[0]    = quad.texPosition.d_left;
        myquad[4].tex[1]    = quad.texPosition.d_top;
    }
    // bottom-left to top-right diagonal
    else
    {
        myquad[4].vertex[0] = quad.position.d_left;
        myquad[4].vertex[1] = quad.position.d_bottom;
        myquad[4].vertex[2] = z;
        myquad[4].color     = quad.bottomLeftCol;
        myquad[4].tex[0]    = quad.texPosition.d_left;
        myquad[4].tex[1]    = quad.texPosition.d_bottom;
    }

	//vert5
	myquad[5].vertex[0] = quad.position.d_right;
	myquad[5].vertex[1] = quad.position.d_bottom;
	myquad[5].vertex[2] = z;
	myquad[5].color     = quad.bottomRightCol;
	myquad[5].tex[0]    = quad.texPosition.d_right;
	myquad[5].tex[1]    = quad.texPosition.d_bottom;

	glDrawArrays(GL_TRIANGLES, 0, 6);

	exitPerFrameStates();
}


/*************************************************************************
	convert colour value to whatever the OpenGL system is expecting.
*************************************************************************/
long OpenGLRenderer::colourToOGL(const colour& col) const
{
	ulong cval;
#ifdef __BIG_ENDIAN__
    cval =  (static_cast<ulong>(255 * col.getAlpha()));
    cval |= (static_cast<ulong>(255 * col.getBlue())) << 8;
    cval |= (static_cast<ulong>(255 * col.getGreen())) << 16;
    cval |= (static_cast<ulong>(255 * col.getRed())) << 24;
#else
	cval =	(static_cast<ulong>(255 * col.getAlpha())) << 24;
	cval |=	(static_cast<ulong>(255 * col.getBlue())) << 16;
	cval |=	(static_cast<ulong>(255 * col.getGreen())) << 8;
	cval |= (static_cast<ulong>(255 * col.getRed()));
#endif
	return cval;
}


/*************************************************************************
	Set the size of the display in pixels.	
*************************************************************************/
void OpenGLRenderer::setDisplaySize(const Size& sz)
{
	if (d_display_area.getSize() != sz)
	{
		d_display_area.setSize(sz);

		EventArgs args;
		fireEvent(EventDisplaySizeChanged, args, EventNamespace);
	}

}

void OpenGLRenderer::setModuleIdentifierString()
{
    // set ID string
    d_identifierString = "CEGUI::OpenGLRenderer - Official OpenGL based renderer module for CEGUI";
}


/************************************************************************
    Grabs all loaded textures to local buffers and frees them
*************************************************************************/
void OpenGLRenderer::grabTextures()
{
    typedef std::list<OpenGLTexture*> texlist;
    texlist::iterator i = d_texturelist.begin();
    while (i!=d_texturelist.end())
    {
        (*i)->grabTexture();
        i++;
    }
}


/************************************************************************
    Restores all textures from the previous call to 'grabTextures'
*************************************************************************/
void OpenGLRenderer::restoreTextures()
{
    typedef std::list<OpenGLTexture*> texlist;
    texlist::iterator i = d_texturelist.begin();
    while (i!=d_texturelist.end())
    {
        (*i)->restoreTexture();
        i++;
    }
}

} // End of  CEGUI namespace section

