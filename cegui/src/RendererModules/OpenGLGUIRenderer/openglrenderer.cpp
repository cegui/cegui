/***********************************************************************
	filename: 	openglrenderer.cpp
	created:	9/4/2004
	author:		Mark Strom
				mwstrom@gmail.com

	purpose:	Interface to Renderer implemented via Opengl
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <GL/glew.h>

#include "openglrenderer.h"
#include "opengltexture.h"
#include "CEGUIExceptions.h"
#include "CEGUIEventArgs.h"
#include "CEGUIImageCodec.h"
#include "CEGUIDynamicModule.h"

#include <sstream>

//Include the default codec for static builds
#if defined(CEGUI_STATIC)
#	if defined(CEGUI_CODEC_SILLY)
#		include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
#	elif defined(CEGUI_CODEC_TGA)
#		include "../../ImageCodecModules/TGAImageCodec/CEGUITGAImageCodecModule.h"
#	elif defined(CEGUI_CODEC_CORONA)
#		include "../../ImageCodecModules/CoronaImageCodec/CEGUICoronaImageCodecModule.h"
#	elif defined(CEGUI_CODEC_DEVIL)
#		include "../../ImageCodecModules/DevILImageCodec/CEGUIDevILImageCodecModule.h"
#	elif defined(CEGUI_CODEC_FREEIMAGE)
#		include "../../ImageCodecModules/FreeImageImageCodec/CEGUIFreeImageImageCodecModule.h"
#	else //Make Silly the default
#		include "../../ImageCodecModules/SILLYImageCodec/CEGUISILLYImageCodecModule.h"
#	endif
#endif


#define S_(X) #X
#define STRINGIZE(X) S_(X)


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// The following are some GL extension / version dependant related items.
// This is all done totally internally here; no need for external interface
// to show any of this.
//----------------------------------------------------------------------------//
// we only really need this with MSVC / Windows(?) and by now it should already
// be defined on that platform, so we just define it as empty macro so the
// compile does not break on other systems.
#ifndef APIENTRY
#   define APIENTRY
#endif
//! Function to perform extentsions initialisation.
void initialiseGLExtensions();
//! Pointer to a function to use as glActiveTexture
PFNGLACTIVETEXTUREPROC CEGUI_activeTexture;
//! Pointer to a function to use as glClientActiveTexture
PFNGLCLIENTACTIVETEXTUREPROC CEGUI_clientActiveTexture;
//! Dummy function for if real ones are not present (saves testing each render)
void APIENTRY activeTextureDummy(GLenum) {}
//----------------------------------------------------------------------------//

/*************************************************************************
	Constants definitions
*************************************************************************/
const int OpenGLRenderer::VERTEX_PER_QUAD           = 6;
const int OpenGLRenderer::VERTEX_PER_TRIANGLE       = 3;
const int OpenGLRenderer::VERTEXBUFFER_CAPACITY     = OGLRENDERER_VBUFF_CAPACITY;


/*************************************************************************
	Constructor
*************************************************************************/
OpenGLRenderer::OpenGLRenderer(uint max_quads, ImageCodec*  codec) :
    d_queueing(true),
    d_currTexture(0),
    d_bufferPos(0),
    d_imageCodec(codec),
    d_imageCodecModule(0)
{
	GLint vp[4];

	// initialise renderer size
	glGetIntegerv(GL_VIEWPORT, vp);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);
	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= (float)vp[2];
	d_display_area.d_bottom	= (float)vp[3];

    if (!d_imageCodec)
        setupImageCodec("");

    setModuleIdentifierString();

    initialiseGLExtensions();
}


OpenGLRenderer::OpenGLRenderer(uint max_quads,int width, int height, ImageCodec* codec) :
	d_queueing(true),
	d_currTexture(0),
	d_bufferPos(0),
    d_imageCodec(codec),
    d_imageCodecModule(0)
{
	GLint vp[4];

	// initialise renderer size
	glGetIntegerv(GL_VIEWPORT, vp);
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);
	d_display_area.d_left	= 0;
	d_display_area.d_top	= 0;
	d_display_area.d_right	= static_cast<float>(width);
	d_display_area.d_bottom	= static_cast<float>(height);

    if (!d_imageCodec)
        setupImageCodec("");

    setModuleIdentifierString();

    initialiseGLExtensions();
}


/*************************************************************************
	Destructor
*************************************************************************/
OpenGLRenderer::~OpenGLRenderer(void)
{
	destroyAllTextures();
    cleanupImageCodec();
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
	OpenGLTexture* tex = new OpenGLTexture(this);
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
Texture* OpenGLRenderer::createTexture(float size)
{
	OpenGLTexture* tex = new OpenGLTexture(this);
	try
	{
	    tex->setOGLTextureSize((uint)size);
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
void OpenGLRenderer::destroyTexture(Texture* texture)
{
	if (texture)
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

    CEGUI_activeTexture(GL_TEXTURE0);
    CEGUI_clientActiveTexture(GL_TEXTURE0);

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
	glDisable(GL_FOG);
    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}


void OpenGLRenderer::exitPerFrameStates(void)
{
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	//restore former attributes
	glPopAttrib();
	glPopClientAttrib();
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
uint32 OpenGLRenderer::colourToOGL(const colour& col) const
{
    const argb_t c = col.getARGB();

    // OpenGL wants RGBA

#ifdef __BIG_ENDIAN__
    uint32 cval = (c << 8) | (c >> 24);
#else
    uint32 cval = ((c&0xFF0000)>>16) | (c&0xFF00) | ((c&0xFF)<<16) | (c&0xFF000000);
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
/***********************************************************************
    Get the current ImageCodec object used
************************************************************************/
ImageCodec& OpenGLRenderer::getImageCodec()
{
    return *d_imageCodec;
}
/***********************************************************************
    Set the current ImageCodec object used
************************************************************************/
void OpenGLRenderer::setImageCodec(const String& codecName)
{
    setupImageCodec(codecName);
}
/***********************************************************************
    Set the current ImageCodec object used
************************************************************************/
void OpenGLRenderer::setImageCodec(ImageCodec* codec)
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
void OpenGLRenderer::setupImageCodec(const String& codecName)
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
void OpenGLRenderer::cleanupImageCodec()
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
void OpenGLRenderer::setDefaultImageCodecName(const String& codecName)
{
    d_defaultImageCodecName = codecName;
}

/***********************************************************************
    get the default ImageCodec name to be used
************************************************************************/
const String& OpenGLRenderer::getDefaultImageCodecName()
{
    return d_defaultImageCodecName;

}

/***********************************************************************
    store the name of the default ImageCodec
************************************************************************/
String OpenGLRenderer::d_defaultImageCodecName(STRINGIZE(CEGUI_DEFAULT_IMAGE_CODEC));

//----------------------------------------------------------------------------//
void initialiseGLExtensions()
{
    // initialise GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::ostringstream err_string;
        err_string << "OpenGLRenderer failed to initialise the GLEW library. "
            << glewGetErrorString(err);

        throw RendererException(err_string.str());
    }

    // GL 1.3 has multi-texture support natively
    if (GLEW_VERSION_1_3)
    {
        CEGUI_activeTexture = glActiveTexture;
        CEGUI_clientActiveTexture = glClientActiveTexture;
    }
    // Maybe there is the ARB_multitexture extension version?
    else if (GL_ARB_multitexture)
    {
        CEGUI_activeTexture = glActiveTextureARB;
        CEGUI_clientActiveTexture = glClientActiveTextureARB;
    }
    // assign dummy functions if no multitexture possibilities
    else
    {
        CEGUI_activeTexture = activeTextureDummy;
        CEGUI_clientActiveTexture = activeTextureDummy;
    }
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

