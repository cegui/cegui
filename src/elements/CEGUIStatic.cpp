/************************************************************************
	filename: 	CEGUIStatic.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Static widget base class
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
#include "elements/CEGUIStatic.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor for static widget base class
*************************************************************************/
Static::Static(const String& type, const String& name) :
	Window(type, name),
	d_frameEnabled(false),
	d_frameCols(-1, -1, -1, -1),
	d_backgroundCols(-1, -1, -1, -1)
{
}


/*************************************************************************
	Destructor for static widget base class.
*************************************************************************/
Static::~Static(void)
{
}


/*************************************************************************
	overridden so derived classes are auto-clipped to within the inner
	area of the frame when it's active.
*************************************************************************/
Rect Static::getUnclippedInnerRect(void) const
{
	// if frame is enabled, return rect for area inside frame
	if (d_frameEnabled)
	{
		Rect tmp(Window::getUnclippedInnerRect());
		tmp.d_left		+= d_left_width;
		tmp.d_right		-= d_right_width;
		tmp.d_top		+= d_top_height;
		tmp.d_bottom	-= d_bottom_height;
		return tmp;
	}
	// no frame, so return default inner rect.
	else
	{
		return Window::getUnclippedInnerRect();
	}

}


/*************************************************************************
	Enable or disable rendering of the frame for this static widget.
*************************************************************************/
void Static::setFrameEnabled(bool setting)
{
	if (d_frameEnabled != setting)
	{
		d_frameEnabled = setting;
		onStaticFrameChanged(WindowEventArgs(this));
		requestRedraw();
	}
}


/*************************************************************************
	specify the Image objects to use for each part of the frame.
	A NULL may be used to omit any part.	
*************************************************************************/
void Static::setFrameImages(const Image* topleft, const Image* topright, const Image* bottomleft, const Image* bottomright, const Image* left, const Image* top, const Image* right, const Image* bottom)
{
	// install the new images into the RenderableFrame
	d_frame.setImages(topleft, topright, bottomleft, bottomright, left, top, right, bottom);

	// get sizes of frame edges
	d_left_width	= (left != NULL) ? left->getWidth() : 0.0f;
	d_right_width	= (right != NULL) ? right->getWidth() : 0.0f;
	d_top_height	= (top != NULL) ? top->getHeight() : 0.0f;
	d_bottom_height	= (bottom != NULL) ? bottom->getHeight() : 0.0f;

	// redraw only if change would be seen.
	if (d_frameEnabled)
	{
		onStaticFrameChanged(WindowEventArgs(this));
		requestRedraw();
	}

}


/*************************************************************************
	Sets the colours to be applied when rendering the frame	
*************************************************************************/
void Static::setFrameColours(const ColourRect& colours)
{
	d_frameCols = colours;
	updateRenderableFrameColours();

	// redraw only if change would be seen.
	if (d_frameEnabled)
	{
		onStaticFrameChanged(WindowEventArgs(this));
		requestRedraw();
	}

}


/*************************************************************************
	Sets the colours to be applied when rendering the frame	
*************************************************************************/
void Static::setFrameColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour)
{
	d_frameCols.d_top_left		= top_left_colour;
	d_frameCols.d_top_right		= top_right_colour;
	d_frameCols.d_bottom_left	= bottom_left_colour;
	d_frameCols.d_bottom_right	= bottom_right_colour;
	updateRenderableFrameColours();

	// redraw only if change would be seen.
	if (d_frameEnabled)
	{
		onStaticFrameChanged(WindowEventArgs(this));
		requestRedraw();
	}

}


/*************************************************************************
	Enable or disable rendering of the background for this static widget.	
*************************************************************************/
void Static::setBackgoundEnabled(bool setting)
{
	if (d_backgroundEnabled != setting)
	{
		d_backgroundEnabled = setting;
		requestRedraw();
	}

}


/*************************************************************************
	Set the image to use as the background for the static widget.
*************************************************************************/
void Static::setBackgroundImage(const Image* image)
{
	d_background = image;

	if (d_backgroundEnabled)
	{
		requestRedraw();
	}

}


/*************************************************************************
	Set the image to use as the background for the static widget.	
*************************************************************************/
void Static::setBackgroundImage(const String& imageset, const String& image)
{
	setBackgroundImage(&ImagesetManager::getSingleton().getImageset(imageset)->getImage(image));
}


/*************************************************************************
	Sets the colours to be applied when rendering the background.	
*************************************************************************/
void Static::setBackgroundColours(const ColourRect& colours)
{
	d_backgroundCols = colours;

	if (d_backgroundEnabled)
	{
		requestRedraw();
	}

}


/*************************************************************************
	Sets the colours to be applied when rendering the background.	
*************************************************************************/
void Static::setBackgroundColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour)
{
	d_backgroundCols.d_top_left		= top_left_colour;
	d_backgroundCols.d_top_right	= top_right_colour;
	d_backgroundCols.d_bottom_left	= bottom_left_colour;
	d_backgroundCols.d_bottom_right	= bottom_right_colour;

	if (d_backgroundEnabled)
	{
		requestRedraw();
	}

}


/*************************************************************************
	update the internal RenderableFrame with currently set colours and
	alpha settings.
*************************************************************************/
void Static::updateRenderableFrameColours(void)
{
	float alpha = getEffectiveAlpha();

	d_frame.setColours(
		calculateModulatedAlphaColour(d_frameCols.d_top_left, alpha),
		calculateModulatedAlphaColour(d_frameCols.d_top_right, alpha),
		calculateModulatedAlphaColour(d_frameCols.d_bottom_left, alpha),
		calculateModulatedAlphaColour(d_frameCols.d_bottom_right, alpha)
	);

}


/*************************************************************************
	given an ARGB colour value and a alpha float value return the colour
	value with the alpha component modulated by the given alpha float.
*************************************************************************/
colour Static::calculateModulatedAlphaColour(colour col, float alpha) const
{
	return ((col & 0x00FFFFFF) | (((colour)(((float)(col >> 24)) * alpha)) << 24));
}


/*************************************************************************
	Perform the actual rendering for this Window.	
*************************************************************************/
void Static::drawSelf(float z)
{
	Rect clipper(getPixelRect());

	// do nothing if the widget is totally clipped.
	if (clipper.getWidth() == 0)
	{
		return;
	}

	Rect absrect(getUnclippedPixelRect());

	// draw frame
	if (d_frameEnabled)
	{
		d_frame.draw(Vector3(absrect.d_left, absrect.d_top, z), clipper);

		// adjust absrect and clipper so that later stages of render to not overwite frame
		absrect.d_left		+= d_left_width;
		absrect.d_right		-= d_right_width;
		absrect.d_top		+= d_top_height;
		absrect.d_bottom	-= d_bottom_height;

		clipper = clipper.getIntersection(absrect);
	}

	// draw backdrop
	if (d_backgroundEnabled && (d_background != NULL))
	{
		// factor window alpha into colours to use when rendering background
		float alpha = getEffectiveAlpha();
		ColourRect colours;
		colours.d_top_left		= calculateModulatedAlphaColour(d_backgroundCols.d_top_left, alpha);
		colours.d_top_right		= calculateModulatedAlphaColour(d_backgroundCols.d_top_right, alpha);
		colours.d_bottom_left	= calculateModulatedAlphaColour(d_backgroundCols.d_bottom_left, alpha);
		colours.d_top_right		= calculateModulatedAlphaColour(d_backgroundCols.d_bottom_right, alpha);

		d_background->draw(absrect, z, clipper, colours);
	}

}


/*************************************************************************
	Handler for when window is sized
*************************************************************************/
void Static::onSized(WindowEventArgs& e)
{
	// base class processing
	Window::onSized(e);

	// update frame size.
	d_frame.setSize(getAbsoluteSize());

	e.handled = true;
}


/*************************************************************************
	Handler for when alpha value changes
*************************************************************************/
void Static::onAlphaChanged(WindowEventArgs& e)
{
	// base class processing
	Window::onAlphaChanged(e);

	// update frame colours to use new alpha value
	updateRenderableFrameColours();

	e.handled = true;
}

} // End of  CEGUI namespace section
