/***********************************************************************
    filename: 	CEGUIColourPickerConversions.h
    created:	20th February 2010
    author:		Lukas E Meindl

    purpose:	Header of the ColourPicker colour type classes
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
#ifndef CEGUI_COLOUR_PICKER_TYPES_H
#define CEGUI_COLOUR_PICKER_TYPES_H

#include "CEGUI/Window.h"

class Lab_Colour;
class RGB_Colour;
class HSV_Colour;

class Position
{
public:
    Position(float xPosition, float yPosition) :
        x(xPosition), y(yPosition)
    {}

    Position()
    {}

    float x;
    float y;
};

class RGB_Colour
{
public:
    RGB_Colour(unsigned char red, unsigned char green, unsigned char blue) :
        r(red), g(green), b(blue)
    {}
    RGB_Colour() {}
    RGB_Colour(Lab_Colour colour);

    unsigned char   r;
    unsigned char   g;
    unsigned char   b;

    RGB_Colour operator*(const float& number) const;
    RGB_Colour operator+(const RGB_Colour& colour) const;
};

class Lab_Colour
{
public:
    Lab_Colour(unsigned char R, unsigned char G, unsigned char B);
    Lab_Colour(float LValue, float aValue, float bValue) :
        L(LValue), a(aValue), b(bValue)
    {}
    Lab_Colour(const RGB_Colour& rgbColour);
    Lab_Colour() {}

    float L;
    float a;
    float b;
};

class HSV_Colour
{
public:
    HSV_Colour(unsigned char R, unsigned char G, unsigned char B);
    HSV_Colour(float HValue, float SValue, float VValue) :
        H(HValue), S(SValue), V(VValue)
    {}
    HSV_Colour(const RGB_Colour& rgbColour);
    HSV_Colour() {}

    float H;
    float S;
    float V;
};

enum ColourPickerSliderMode
{
    ColourPickerSliderMode_L,
    ColourPickerSliderMode_A,
    ColourPickerSliderMode_B
};

#endif

