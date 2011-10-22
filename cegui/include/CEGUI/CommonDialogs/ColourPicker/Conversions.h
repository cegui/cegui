/***********************************************************************
    filename: 	CEGUIColourPickerConversions.h
    created:	20th February 2010
    author:		Lukas E Meindl

    purpose:	Header of the class used to provide conversions between
                the ColourPicker colour types
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIColourPickerConversions_h_
#define _CEGUIColourPickerConversions_h_

#include "CEGUI/CommonDialogs/ColourPicker/Types.h"
#include "CEGUI/Colour.h"

namespace ColourPickerConversions
{
//Umrechnung von RGB auf XYZ
float toX(unsigned char R, unsigned char G, unsigned char B);
float toY(unsigned char R, unsigned char G, unsigned char B);
float toZ(unsigned char R, unsigned char G, unsigned char B);
float toX(float R, float G, float B);
float toY(float R, float G, float B);
float toZ(float R, float G, float B);
//Berechnung der L a und b Komponenten aus dem Lab-Farbraum
float toL(float Y);
float toA(float X, float Y);
float toB(float Y, float Z);
//Umrechnung auf HSV
HSV_Colour toHSV(RGB_Colour colour);

//Umrechnungen
RGB_Colour toRGB(const Lab_Colour& colour);
RGB_Colour toRGB(float L, float a, float b);
RGB_Colour toRGB(const CEGUI::Colour& colour);
RGB_Colour toRGB(const HSV_Colour& colour);
CEGUI::Colour toCeguiColour(const RGB_Colour& colourRGB);

//Lineare interpolation von Farben
RGB_Colour linearInterpolationRGB(float interPolBalance,
                                  const RGB_Colour& start,
                                  const RGB_Colour& end);
unsigned char linearInterpolationAlpha(float interPolBalance,
                                       unsigned char startAlpha,
                                       unsigned char endAlpha);
Lab_Colour linearInterpolationLab(float interPolBalance,
                                  const Lab_Colour& start,
                                  const Lab_Colour& end);
HSV_Colour linearInterpolationHSV(float interPolBalance,
                                  const HSV_Colour& start,
                                  const HSV_Colour& end);

void clampInterpolationValue(float& interPolBalance);

//Normierten Bezugsgrößen für Lab Berechnung
//Für CIELab (D65)
const float Xn = 0.95047f;
const float Yn = 1.00000f;
const float Zn = 1.08883f;

//Vergleichswert für die Kalkulation von Lab bei geringen P/Pn Werten
//(P/Pn < LAB_COMPARE_VALUE_CONST)
const float LAB_COMPARE_VALUE_CONST = 0.00885645167903563081717167575546f;

//Helferfunktionen für Lab Einzelkomponentenberechnung
float normCalc(float div);
float XNormCalc(float X);
float YNormCalc(float Y);
float ZNormCalc(float Z);
}

#endif

