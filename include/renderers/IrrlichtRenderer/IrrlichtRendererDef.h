/************************************************************************
filename: 	IrrlichtRendererDef.h
created:	20/7/2004
author:		Thomas Suter

changes: added more complete instructions on patching irrlicht v0.6
a patched version of irrlicht can be found at http://www.irrlichtnx.mmdevel.de/ 
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

/*
 
 Irrlicht v0.6 changes

 In IVideoDriver.h You define the function like this:

 [code]
 // IVideoDriver.h
 / *!
 \brief the part of the texture denoted by sourceRect is used as
 texture for the rectangle denoted by destRect.
 clipRect defines a clipping rectangle against which the destRect is clipped.
 colors is an array of four color values colors[1]
 is the color for the upperleft corner of the destination rectangle.
 the order is counter clock wise.
 useAlphaChannelOfTexture tells the method if rendering
 in opaque mode (false) or blended mode (true) is needed.
 \param texture, the texture used for texturing
 \param destRect, the destination rectangle (screen coordinates)
 \param sourceRect, the source rectangle in texture coordinates ( (0,tex_width), (0, textheight) )
 \param colors, 4 colors of the corners of the destRectangle starting at upperleft, ordered counter clock wise
 \param useAlphaChannelOfTexture, true paint in blend mode, false paint in opaque mode
 * /
virtual void draw2DImage(video::ITexture* texture, const core::rect<s32>& destRect, const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0, video::SColor* colors=0, bool useAlphaChannelOfTexture=false) = 0;
[/code]


Then in CVideoNull.h You do the following

[code]
// CVideoNull.h
virtual void draw2DImage(video::ITexture* texture, const core::rect<s32>& destRectt, const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0, video::SColor* colors=0, bool useAlphaChannelOfTexture=false){ }
[/code]


and finally in the real driver (in my case opengl) you implement the method with the inteded behaviour



Headerfile

[code]
//CVideoOpenGL.h
virtual void draw2DImage(video::ITexture* texture, const core::rect<s32>& destRect,const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,video::SColor* colors=0, bool useAlphaChannelOfTexture=false);
[/code]


Implementation:

[code]
//CVideoOpenGL.cpp
void CVideoOpenGL::draw2DImage(video::ITexture* texture, const core::rect<s32>& destRect,const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect, video::SColor colors[4], bool useAlphaChannelOfTexture)
{
	if (!texture)
		return;

	core::rect<s32> trgRect=destRect;
	core::rect<s32> srcRect=sourceRect;

	const core::dimension2d<s32> targetSurfaceSize=ScreenSize;

	const core::dimension2d<s32> ss = texture->getOriginalSize();
	float ssw=1.0f/ss.Width;
	float ssh=1.0f/ss.Height;

	core::rect<f32> tcoords;
	tcoords.UpperLeftCorner.X = (((f32)srcRect.UpperLeftCorner.X)+0.5f) * ssw ;
	tcoords.UpperLeftCorner.Y = (((f32)srcRect.UpperLeftCorner.Y)+0.5f) * ssh;
	tcoords.LowerRightCorner.X = (((f32)srcRect.UpperLeftCorner.X +0.5f + (f32)srcRect.getWidth())) * ssw;
	tcoords.LowerRightCorner.Y = (((f32)srcRect.UpperLeftCorner.Y +0.5f + (f32)srcRect.getHeight())) * ssh;

	s32 xPlus = -(ScreenSize.Width>>1);
	f32 xFact = 1.0f / (ScreenSize.Width>>1);

	s32 yPlus = ScreenSize.Height-(ScreenSize.Height>>1);
	f32 yFact = 1.0f / (ScreenSize.Height>>1);

	core::rect<float> npos;
	npos.UpperLeftCorner.X = (f32)(trgRect.UpperLeftCorner.X+xPlus+0.5f) * xFact;
	npos.UpperLeftCorner.Y = (f32)(yPlus-trgRect.UpperLeftCorner.Y+0.5f) * yFact;
	npos.LowerRightCorner.X = (f32)(trgRect.LowerRightCorner.X+xPlus+0.5f) * xFact;
	npos.LowerRightCorner.Y = (f32)(yPlus-trgRect.LowerRightCorner.Y+0.5f) * yFact;

	setTexture(0, texture);   

	if (useAlphaChannelOfTexture)
	{
		setRenderStates2DMode(false, true, true);
	}
	else
		setRenderStates2DMode(false, true, false);

	bool bTempColors=false;
	if(colors==NULL)
	{
		colors=new SColor[4];
		for(int i=0;i<4;i++)colors[i]=SColor(0,255,255,255);
		bTempColors=true;
	}

	glBegin(GL_QUADS);

	glColor4ub(colors[0].getRed(), colors[0].getGreen(), colors[0].getBlue(), colors[0].getAlpha());
	glTexCoord2f(tcoords.UpperLeftCorner.X, tcoords.UpperLeftCorner.Y);
	glVertex2f(npos.UpperLeftCorner.X, npos.UpperLeftCorner.Y);

	glColor4ub(colors[3].getRed(), colors[3].getGreen(), colors[3].getBlue(), colors[3].getAlpha());
	glTexCoord2f(tcoords.LowerRightCorner.X, tcoords.UpperLeftCorner.Y);
	glVertex2f(npos.LowerRightCorner.X, npos.UpperLeftCorner.Y);

	glColor4ub(colors[2].getRed(), colors[2].getGreen(), colors[2].getBlue(), colors[2].getAlpha());
	glTexCoord2f(tcoords.LowerRightCorner.X, tcoords.LowerRightCorner.Y);
	glVertex2f(npos.LowerRightCorner.X, npos.LowerRightCorner.Y);

	glColor4ub(colors[1].getRed(), colors[1].getGreen(), colors[1].getBlue(), colors[1].getAlpha());
	glTexCoord2f(tcoords.UpperLeftCorner.X, tcoords.LowerRightCorner.Y);
	glVertex2f(npos.UpperLeftCorner.X, npos.LowerRightCorner.Y);

	glEnd();

	if(bTempColors) delete [] colors;
}
[/code]

You must also make sure that the textures get painted with alpha blending enabled

[code]
// CVideoOpenGL.cpp
void CVideoOpenGL::setRenderStates2DMode(bool alpha, bool texture, bool alphaChannel)
{
	if (CurrentRenderMode != ERM_2D || Transformation3DChanged)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Transformation3DChanged = false;

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_FOG);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_LIGHTING);

		if (MultiTextureExtension)
			glActiveTextureARB(GL_TEXTURE0_ARB);

		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);

		ClampTexture = false;
	}

	if (texture)
	{
		//GL_NEAREST
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (alphaChannel)
		{
			// GL_MODULATE GL_DECAL
			glEnable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_MODULATE);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			//glDisable(GL_BLEND);
			//glEnable(GL_ALPHA_TEST);
			//glAlphaFunc(GL_GREATER, 0);   

		}
		else
		{
			if (alpha)
			{
				glDisable(GL_ALPHA_TEST);
				glEnable(GL_BLEND);
			}
			else
			{
				glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
				glDisable(GL_ALPHA_TEST);
				glDisable(GL_BLEND);
			}
		}

	}
	else
	{
		if (alpha)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glDisable(GL_ALPHA_TEST);
		}
		else
		{
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glDisable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
		}
	}

	CurrentRenderMode = ERM_2D;
}
[/code]

*/
#ifndef IRRLICHTRENDERERDEF_H_INCLUDED
#define IRRLICHTRENDERERDEF_H_INCLUDED

#if defined( __WIN32__ ) || defined( _WIN32 ) || defined (WIN32)
#   ifdef IRRRENDERER_EXPORTS
#       define IRRLICHT_GUIRENDERER_API __declspec(dllexport)
#   else
#       define IRRLICHT_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define IRRLICHT_GUIRENDERER_API
#endif

#endif