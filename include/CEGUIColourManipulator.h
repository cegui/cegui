/************************************************************************
	filename: 	CEGUIColourManipulator.h
	created:	7/2/2004
	author:		Jeff Leigh
	
	purpose:	Defines a class for manipulating colours.
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
#ifndef _CEGUIColourManipulator_h_
#define _CEGUIColourManipulator_h_

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Class that manipulates colours.
*/
class CEGUIBASE_API ColourManipulator
{
public:
	ColourManipulator();
	ColourManipulator( const colour &colorval );
	ColourManipulator( float r, float g, float b, float a = 1 );

	void setColour( const colour &colourval );
	void setColourRGB( float r, float g, float b, float a = 1 );
	void setColourHSL( float h, float s, float l, float a = 1 );

	float getRed();
	float getGreen();
	float getBlue();
	float getAlpha();

	float getHue();
	float getSatration();
	float getLumination();
    
	ColourManipulator operator +( const ColourManipulator &p ) const 
	{
		ColourManipulator out;

        out.d_a = d_a + p.d_a;
		out.d_r = d_r + p.d_r;
		out.d_g = d_g + p.d_g;
		out.d_b = d_b + p.d_b;
		
		return out;
	}

	ColourManipulator operator -( const ColourManipulator &p ) const
	{
		ColourManipulator out;

        out.d_a = d_a - p.d_a;
		out.d_r = d_r - p.d_r;
		out.d_g = d_g - p.d_g;
		out.d_b = d_b - p.d_b;
		
		return out;
	}

	ColourManipulator operator *( const float p ) const
	{
		ColourManipulator out;

        out.d_a = d_a * p;
		out.d_r = d_r * p;
		out.d_g = d_g * p;
		out.d_b = d_b * p;
		
		return out;
	}

	operator colour() const
	{
		return (static_cast<colour>(d_a * 255) << 24) +
				(static_cast<colour>(d_r * 255) << 16) +
				(static_cast<colour>(d_g * 255) << 8) +
				(static_cast<colour>(d_b * 255));
	}

protected:
	float getMax();
	float getMin();

private:
	float d_r, d_g, d_b, d_a;
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIColourManipulator_h_
