/************************************************************************
	filename: 	CEGUIColourRect.cpp
	created:	8/3/2004
	author:		Paul D Turner
	
	purpose:	Implements ColourRect class
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
#include "CEGUIColourRect.h"
#include "CEGUIColourManipulator.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor
*************************************************************************/
ColourRect::ColourRect(colour top_left, colour top_right, colour bottom_left, colour bottom_right) :
	d_top_left(top_left),
	d_top_right(top_right),
	d_bottom_left(bottom_left),
	d_bottom_right(bottom_right)
{
}


/*************************************************************************
	Constructor for ColourRect objects (via single colour).
	Also handles default construction.	
*************************************************************************/
ColourRect::ColourRect(colour col) :
	d_top_left(col),
	d_top_right(col),
	d_bottom_left(col),
	d_bottom_right(col)
{
}


/*************************************************************************
	Destructor
*************************************************************************/
ColourRect::~ColourRect(void)
{
}


/*************************************************************************
	Set the alpha value to use for all four corners of the ColourRect.	
*************************************************************************/
void ColourRect::setAlpha(float alpha)
{
	colour alpha_comp = ((colour)(255.0f * alpha)) << 24;

	// remove old alpha values
	d_top_left		&= 0x00FFFFFF;
	d_top_right		&= 0x00FFFFFF;
	d_bottom_left	&= 0x00FFFFFF;
	d_bottom_right	&= 0x00FFFFFF;

	// set new alpha values
	d_top_left		|= alpha_comp;
	d_top_right		|= alpha_comp;
	d_bottom_left	|= alpha_comp;
	d_bottom_right	|= alpha_comp;
}


/*************************************************************************
	Set the alpha value to use for the top edge of the ColourRect.	
*************************************************************************/
void ColourRect::setTopAlpha(float alpha)
{
	colour alpha_comp = ((colour)(255.0f * alpha)) << 24;

	// remove old alpha values
	d_top_left		&= 0x00FFFFFF;
	d_top_right		&= 0x00FFFFFF;

	// set new alpha values
	d_top_left		|= alpha_comp;
	d_top_right		|= alpha_comp;
}


/*************************************************************************
	Set the alpha value to use for the bottom edge of the ColourRect.	
*************************************************************************/
void ColourRect::setBottomAlpha(float alpha)
{
	colour alpha_comp = ((colour)(255.0f * alpha)) << 24;

	// remove old alpha values
	d_bottom_left	&= 0x00FFFFFF;
	d_bottom_right	&= 0x00FFFFFF;

	// set new alpha values
	d_bottom_left	|= alpha_comp;
	d_bottom_right	|= alpha_comp;
}


/*************************************************************************
	Set the alpha value to use for the left edge of the ColourRect.	
*************************************************************************/
void ColourRect::setLeftAlpha(float alpha)
{
	colour alpha_comp = ((colour)(255.0f * alpha)) << 24;

	// remove old alpha values
	d_top_left		&= 0x00FFFFFF;
	d_bottom_left	&= 0x00FFFFFF;

	// set new alpha values
	d_top_left		|= alpha_comp;
	d_bottom_left	|= alpha_comp;
}


/*************************************************************************
	Set the alpha value to use for the right edge of the ColourRect.	
*************************************************************************/
void ColourRect::setRightAlpha(float alpha)
{
	colour alpha_comp = ((colour)(255.0f * alpha)) << 24;

	// remove old alpha values
	d_top_right		&= 0x00FFFFFF;
	d_bottom_right	&= 0x00FFFFFF;

	// set new alpha values
	d_top_right		|= alpha_comp;
	d_bottom_left	|= alpha_comp;
}

/*************************************************************************
	Get the colour at a specified point
*************************************************************************/
colour ColourRect::getColourAtPoint( float x, float y ) const
{
	ColourManipulator topleft( d_top_left );
	ColourManipulator topright( d_top_right );
	ColourManipulator bottomleft( d_bottom_left );
	ColourManipulator bottomright( d_bottom_right );
	ColourManipulator h1, h2;
	ColourManipulator ColourAtPoint;

    h1 = (topright - topleft) * x + topleft;
	h2 = (bottomright - bottomleft) * x + bottomleft;
	ColourAtPoint = (h2 - h1) * y + h1;

	return static_cast<colour>(ColourAtPoint);
}

/*************************************************************************
	Get a ColourRectangle from the specified Region
*************************************************************************/
ColourRect ColourRect::getSubRectangle( float left, float right, float top, float bottom ) const
{
	ColourRect OurRect;
	
	OurRect.d_top_left = getColourAtPoint( left, top );
	OurRect.d_top_right = getColourAtPoint( right, top );
	OurRect.d_bottom_left = getColourAtPoint( left, bottom );
	OurRect.d_bottom_right = getColourAtPoint( right, bottom );
    
	return OurRect;
}

} // End of  CEGUI namespace section
