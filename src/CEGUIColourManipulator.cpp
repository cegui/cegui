/************************************************************************
	filename: 	CEGUIColourManipulator.cpp
	created:	7/6/2004
	author:		Jeff Leigh
	
	purpose:	Implements a class for manipulating colours.
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
#include "CEGUIColourManipulator.h"

// Start of CEGUI namespace section
namespace CEGUI
{

ColourManipulator::ColourManipulator()
{
	d_r = 0;
	d_g = 0;
	d_b = 0;
	d_a = 1;
}

ColourManipulator::ColourManipulator( const colour &colourval )
{
	setColour( colourval );
}

ColourManipulator::ColourManipulator( float r, float g, float b, float a )
{
	d_r = r;
	d_g = g;
	d_b = b;
	d_a = a;
}

void ColourManipulator::setColour( const colour &colourval )
{
	d_a = static_cast<float>((colourval & 0xFF000000) >> 24) / 255;
    d_r = static_cast<float>((colourval & 0x00FF0000) >> 16) / 255;
	d_g = static_cast<float>((colourval & 0x0000FF00) >> 8) / 255;
	d_b = static_cast<float>(colourval & 0x000000FF) / 255;
}

void ColourManipulator::setColourRGB( float r, float g, float b, float a )
{
	d_r = r;
	d_g = g;
	d_b = b;
	d_a = a;
}

void ColourManipulator::setColourHSL( float h, float s, float l, float a )
{
	d_a = a;

	float temp3[3];

	float pHue = h;
	float pSat = s;
	float pLum = l;

	if( pSat == 0 )
	{
		d_r = pLum;
		d_g = pLum;
		d_b = pLum;
	}
	else
	{
		float temp2;
		if( pLum < 0.5f )
		{
			temp2 = pLum * (1 + pSat);
		}
		else
		{
            temp2 = pLum + pSat - pLum * pSat;
		}

		float temp1 = 2 * pLum - temp2;

		temp3[0] = pHue + (1.0f / 3);
		temp3[1] = pHue;
		temp3[2] = pHue - (1.0f / 3);

		for( int n = 0; n < 3; n ++ )
		{
			if( temp3[n] < 0 )
				temp3[n] ++;
			if( temp3[n] > 1 )
				temp3[n] --;

			if( (temp3[n] * 6) < 1 )
			{
				temp3[n] = temp1 + (temp2 - temp1) * 6 * temp3[n];
			}
			else
			{
                if( (temp3[n] * 2) < 1 )
				{
                    temp3[n] = temp2;
				}
				else
				{
                    if( (temp3[n] * 3) < 2 )
					{
						temp3[n] = temp1 + (temp2 - temp1) * ((2.0f / 3) - temp3[n]) * 6;
					}
					else
					{
                        temp3[n] = temp1;
					}
				}
			}
		}
    
		d_r = temp3[0];
		d_g = temp3[1];
		d_b = temp3[2];
	}    
}

float ColourManipulator::getRed()
{
    return d_r;
}
float ColourManipulator::getGreen()
{
	return d_g;
}
float ColourManipulator::getBlue()
{
	return d_b;
}
float ColourManipulator::getAlpha()
{
	return d_a;
}

float ColourManipulator::getMax()
{
	float pRed = d_r;
	float pGreen = d_g;
	float pBlue = d_b;

	float pMax;
		
    if( pRed > pGreen )
	{
		if( pRed > pBlue )
			pMax = pRed;
		else
			pMax = pBlue;
	}
	else if( pGreen > pBlue )
	{
		pMax = pGreen;
	}
	else
	{
		pMax = pBlue;
	}

	return pMax;
}

float ColourManipulator::getMin()
{
	float pRed = d_r;
	float pGreen = d_g;
	float pBlue = d_b;

	float pMin;

	if( pRed < pGreen )
	{
        if( pRed < pBlue )
			pMin = pRed;
		else
			pMin = pBlue;
	}
	else if( pGreen < pBlue )
	{
		pMin = pGreen;
	}
	else
	{
		pMin = pBlue;
	}
	return pMin;
}

float ColourManipulator::getHue()
{
	float pRed = d_r;
	float pGreen = d_g;
	float pBlue = d_b;

	float pMax = getMax();
	float pMin = getMin();

	float pHue;

    if( pMax == pMin )
	{
		pHue = 0;
	}
	else
	{
		if( pMax == pRed )
		{
				pHue = (pGreen - pBlue) / (pMax - pMin);
		}
		else if( pMax == pGreen )
		{
			pHue = 2 + (pBlue - pRed) / (pMax - pMin);
		}
		else
		{
			pHue = 4 + (pRed - pGreen) / (pMax - pMin);
		}
	}

	float Hue = pHue / 6;
	if( Hue < 0 )
		Hue += 1;

	return Hue;
}

float ColourManipulator::getSatration()
{
	float pRed = d_r;
	float pGreen = d_g;
	float pBlue = d_b;

	float pMax = getMax();
	float pMin = getMin();

    float pLum = (pMax + pMin) / 2;
	float pSat;

    if( pMax == pMin )
	{
		pSat = 0;
	}
	else
	{
		if( pLum < 0.5 )
			pSat = (pMax - pMin) / (pMax + pMin);
		else
			pSat = (pMax - pMin) / (2 - pMax - pMin);
	}

	return pSat;
}

float ColourManipulator::getLumination()
{
	float pRed = d_r;
	float pGreen = d_g;
	float pBlue = d_b;

	float pMax = getMax();
	float pMin = getMin();

    float pLum = (pMax + pMin) / 2;
	return pLum;
}

}
