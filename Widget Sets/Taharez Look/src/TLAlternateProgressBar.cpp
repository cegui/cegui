/************************************************************************
	filename: 	TLAlternateProgressBar.h
	created:	23/5/2004
	author:		Paul D Turner
	
	purpose:	Implementation of the alternate Taharez orogress bar
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
#include "TLAlternateProgressBar.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constants
*************************************************************************/
// Progress bar image names
const utf8	TLAlternateProgressBar::ImagesetName[]				= "TaharezImagery";
const utf8	TLAlternateProgressBar::ContainerLeftImageName[]	= "AltProgressLeft";
const utf8	TLAlternateProgressBar::ContainerMiddleImageName[]	= "AltProgressMiddle";
const utf8	TLAlternateProgressBar::ContainerRightImageName[]	= "AltProgressRight";
const utf8	TLAlternateProgressBar::QuartersMarkImageName[]		= "AltProgressQuarter";
const utf8	TLAlternateProgressBar::HalvesMarkImageName[]		= "AltProgressHalf";
const utf8	TLAlternateProgressBar::Light1ImageName[]			= "AltProgressLight1";
const utf8	TLAlternateProgressBar::Light2ImageName[]			= "AltProgressLight2";
const utf8	TLAlternateProgressBar::Light3ImageName[]			= "AltProgressLight3";
const utf8	TLAlternateProgressBar::Light4ImageName[]			= "AltProgressLight4";
const utf8	TLAlternateProgressBar::Light5ImageName[]			= "AltProgressLight5";
const utf8	TLAlternateProgressBar::Light6ImageName[]			= "AltProgressLight6";
const utf8	TLAlternateProgressBar::Light7ImageName[]			= "AltProgressLight7";
const utf8	TLAlternateProgressBar::Light8ImageName[]			= "AltProgressLight8";
const utf8	TLAlternateProgressBar::Light9ImageName[]			= "AltProgressLight9";
const utf8	TLAlternateProgressBar::Light10ImageName[]			= "AltProgressLight10";

// some offsets
const float	TLAlternateProgressBar::FirstLightPaddingRatio		= 0.0f;


/*************************************************************************
	Constructor for Taharez alternate progress bar objects
*************************************************************************/
TLAlternateProgressBar::TLAlternateProgressBar(const String& type, const String& name) :
	ProgressBar(type, name),
	d_lightCount(0),
	d_lightsSize(0),
	d_lights(NULL)
{
	// cache images
	Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);

	d_left			= &iset->getImage(ContainerLeftImageName);
	d_right			= &iset->getImage(ContainerRightImageName);
	d_middle		= &iset->getImage(ContainerMiddleImageName);
	d_quarterMark	= &iset->getImage(QuartersMarkImageName);
	d_halvesMark	= &iset->getImage(HalvesMarkImageName);

	d_lightImages[0] = &iset->getImage(Light1ImageName);
	d_lightImages[1] = &iset->getImage(Light2ImageName);
	d_lightImages[2] = &iset->getImage(Light3ImageName);
	d_lightImages[3] = &iset->getImage(Light4ImageName);
	d_lightImages[4] = &iset->getImage(Light5ImageName);
	d_lightImages[5] = &iset->getImage(Light6ImageName);
	d_lightImages[6] = &iset->getImage(Light7ImageName);
	d_lightImages[7] = &iset->getImage(Light8ImageName);
	d_lightImages[8] = &iset->getImage(Light9ImageName);
	d_lightImages[9] = &iset->getImage(Light10ImageName);

	setupLightsInfo();
}


/*************************************************************************
	Destructor for Taharez alternate progress bar objects
*************************************************************************/
TLAlternateProgressBar::~TLAlternateProgressBar(void)
{
	delete[] d_lights;
}


/*************************************************************************
	function to initialise data relating to lights that should be rendered.	
*************************************************************************/
void TLAlternateProgressBar::setupLightsInfo(void)
{
	//
	// work out how many lights we are going to have
	//
	float lightWidth = d_lightImages[0]->getWidth();
	float availWidth = d_abs_area.getWidth() - ((d_left->getWidth() * FirstLightPaddingRatio) * 2);

	d_lightCount = (int)(availWidth / (lightWidth + 1));
	d_lightSpacing = lightWidth + ((availWidth - (d_lightCount * lightWidth)) / d_lightCount);

	// Allocate a bigger array if needed
	if (d_lightsSize < d_lightCount) {
		delete[] d_lights;
		d_lights = new int[d_lightCount];
		d_lightsSize = d_lightCount;
	}

	// initialise d_lights with appropriate indeces into d_lightImages array.
	for (int lc = 0; lc < d_lightCount; ++lc)
	{
		d_lights[lc] = ((lc * TLAPB_LightImageCount) / d_lightCount);
	}

}


/*************************************************************************
	Perform rendering for this widget
*************************************************************************/
void TLAlternateProgressBar::drawSelf(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	// get the destination screen rect for this window
	Rect absrect(getUnclippedPixelRect());

	// calculate colours to use.
	colour alpha_comp = ((colour)(getEffectiveAlpha() * 255.0f) << 24);
	colour colval = alpha_comp | 0xFFFFFF;
	ColourRect colours(colval, colval, colval, colval);

	//
	// Rendering of the container
	//
	float leftWidth	 = d_left->getWidth();
	float rightWidth = d_right->getWidth();
	float midWidth	 = absrect.getWidth() - leftWidth - rightWidth;

	float containerHeight = absrect.getHeight() - d_halvesMark->getHeight() + 2; // TODO: Magic number removal
	float marksY = absrect.d_bottom - d_halvesMark->getHeight();

	Vector3 pos(absrect.d_left, absrect.d_top, z);
	Size sz(leftWidth, containerHeight);

	// left end
	d_left->draw(pos, sz, clipper, colours);
	pos.d_x += sz.d_width;

	// stretchy middle section
	sz.d_width = midWidth;
	d_middle->draw(pos, sz, clipper, colours);
	pos.d_x += sz.d_width;

	// right end.
	sz.d_width = rightWidth;
	d_right->draw(pos, sz, clipper, colours);

	// halves marker
	pos.d_x = absrect.d_left + (absrect.getWidth() * 0.5f);
	pos.d_y = marksY;
	d_halvesMark->draw(pos, clipper, colours);

	// first quarter mark
	pos.d_x = absrect.d_left + (absrect.getWidth() * 0.25f);
	d_quarterMark->draw(pos, clipper, colours);

	// third quarter mark
	pos.d_x = absrect.d_left + (absrect.getWidth() * 0.75f);
	d_quarterMark->draw(pos, clipper, colours);

	//
	// Rendering of the lights
	//
	// set up size to render lights at.
	sz.d_width	= d_lightImages[0]->getWidth();
	sz.d_height	= (containerHeight / d_middle->getHeight()) * d_lightImages[0]->getHeight();

	// set up initial position
	pos.d_x = absrect.d_left + (leftWidth * FirstLightPaddingRatio);
	pos.d_y = absrect.d_top + ((containerHeight - sz.d_height) * 0.5f);

	// construct rect for segment area
	Rect	segClipper;
	segClipper.setPosition(Point(pos.d_x, pos.d_y));
	segClipper.setSize(Size((absrect.getWidth() - ((leftWidth * FirstLightPaddingRatio) * 2.0f)) * d_progress, absrect.getHeight()));

	// clip the clipper to the 'light area'
	clipper = segClipper.getIntersection(clipper);

	// if not totally clipped
	if (clipper.getWidth() != 0)
	{
		// draw the lights
		for (int i = 0; i < d_lightCount; ++i)
		{
			d_lightImages[d_lights[i]]->draw(pos, sz, clipper, colours);
			pos.d_x += d_lightSpacing;
		}

	}

}


/*************************************************************************
	handler called when widget is sized
*************************************************************************/
void TLAlternateProgressBar::onSized(WindowEventArgs& e)
{
	// default handling
	ProgressBar::onSized(e);

	// layout lights again
	setupLightsInfo();
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

	Factory Methods

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
	Create, initialise and return a TLAlternateProgressBar
*************************************************************************/
Window* TLAlternateProgressBarFactory::createWindow(const String& name)
{
	TLAlternateProgressBar* wnd = new TLAlternateProgressBar(d_type, name);
	wnd->initialise();

	return wnd;
}

} // End of  CEGUI namespace section
