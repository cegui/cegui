/**
Author: Thomas Suter, 20.7.2004 
Do what ever You like with this source, but don't blame me if it doesn't work
or even destroys Your system. Use at Your own risk.

This renderer needs some modifications in irrlicht:
add a method to the VideoDriver class hierarchie:

// @tsuter START
virtual void draw2DImage(video::ITexture* texture, const core::rect<s32>& destRect,
const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect = 0,
video::SColor* colors=0, bool useAlphaChannelOfTexture=false) = 0;
// @tsuter END

I only have an implementation for CVideoOpenGL.h/CVideoOpenGL.cpp
Clipping is currently disabled

// @tsuter START
void CVideoOpenGL::draw2DImage(video::ITexture* texture, const core::rect<s32>& destRect,
const core::rect<s32>& sourceRect, const core::rect<s32>* clipRect,
video::SColor colors[4], bool useAlphaChannelOfTexture)
{

if (!texture)
return;

//if (sourceRect.UpperLeftCorner.X >= sourceRect.LowerRightCorner.X ||
//	sourceRect.UpperLeftCorner.Y >= sourceRect.LowerRightCorner.Y)
//	return;

core::rect<s32> trgRect=destRect;
core::rect<s32> srcRect=sourceRect;

const core::dimension2d<s32> targetSurfaceSize=ScreenSize;

//if (clipRect)
//{
//	if (targetPos.X < clipRect->UpperLeftCorner.X)
//	{
//		sourceSize.Width += targetPos.X - clipRect->UpperLeftCorner.X;
//		if (sourceSize.Width <= 0)
//			return;

//		sourcePos.X -= targetPos.X - clipRect->UpperLeftCorner.X;
//		targetPos.X = clipRect->UpperLeftCorner.X;
//	}

//	if (targetPos.X + sourceSize.Width > clipRect->LowerRightCorner.X)
//	{
//		sourceSize.Width -= (targetPos.X + sourceSize.Width) - clipRect->LowerRightCorner.X;
//		if (sourceSize.Width <= 0)
//			return;
//	}

//	if (targetPos.Y < clipRect->UpperLeftCorner.Y)
//	{
//		sourceSize.Height += targetPos.Y - clipRect->UpperLeftCorner.Y;
//		if (sourceSize.Height <= 0)
//			return;

//		sourcePos.Y -= targetPos.Y - clipRect->UpperLeftCorner.Y;
//		targetPos.Y = clipRect->UpperLeftCorner.Y;
//	}

//	if (targetPos.Y + sourceSize.Height > clipRect->LowerRightCorner.Y)
//	{
//		sourceSize.Height -= (targetPos.Y + sourceSize.Height) - clipRect->LowerRightCorner.Y;
//		if (sourceSize.Height <= 0)
//			return;
//	}
//}

// clip these coordinates

//if (targetPos.X<0)
//{
//	sourceSize.Width += targetPos.X;
//	if (sourceSize.Width <= 0)
//		return;

//	sourcePos.X -= targetPos.X;
//	targetPos.X = 0;
//}

//if (targetPos.X + sourceSize.Width > targetSurfaceSize.Width)
//{
//	sourceSize.Width -= (targetPos.X + sourceSize.Width) - targetSurfaceSize.Width;
//	if (sourceSize.Width <= 0)
//		return;
//}

//if (targetPos.Y<0)
//{
//	sourceSize.Height += targetPos.Y;
//	if (sourceSize.Height <= 0)
//		return;

//	sourcePos.Y -= targetPos.Y;
//	targetPos.Y = 0;
//}

//if (targetPos.Y + sourceSize.Height > targetSurfaceSize.Height)
//{
//	sourceSize.Height -= (targetPos.Y + sourceSize.Height) - targetSurfaceSize.Height;
//	if (sourceSize.Height <= 0)
//		return;
//}

// ok, we've clipped everything.
// now draw it.

const core::dimension2d<s32> ss = texture->getSize();
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

//core::rect<s32> poss(targetPos, sourceSize);

core::rect<float> npos;
npos.UpperLeftCorner.X = (f32)(trgRect.UpperLeftCorner.X+xPlus+0.5f) * xFact;
npos.UpperLeftCorner.Y = (f32)(yPlus-trgRect.UpperLeftCorner.Y+0.5f) * yFact;
npos.LowerRightCorner.X = (f32)(trgRect.LowerRightCorner.X+xPlus+0.5f) * xFact;
npos.LowerRightCorner.Y = (f32)(yPlus-trgRect.LowerRightCorner.Y+0.5f) * yFact;

//core::rect<f32> npos;
//npos.UpperLeftCorner.X = (f32)trgRect.UpperLeftCorner.X;
//npos.UpperLeftCorner.Y = (f32)trgRect.UpperLeftCorner.Y;
//npos.LowerRightCorner.X = (f32)trgRect.LowerRightCorner.X;
//npos.LowerRightCorner.Y = (f32)trgRect.LowerRightCorner.Y;

setTexture(0, texture);	

if (useAlphaChannelOfTexture)
setRenderStates2DMode(false, true, true);
else
setRenderStates2DMode(false, true, false);

bool bTempColors=false;
if(colors==NULL)
{
colors=new SColor[4];
for(int i=0;i<4;i++)colors[i]=SColor(255,255,255,255);
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
// @tsuter END

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