/***********************************************************************
	filename: 	CEGUIPropertyHelper.h
	created:	6/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to the PropertyHelper class
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
#ifndef _CEGUIPropertyHelper_h_
#define _CEGUIPropertyHelper_h_

#include "CEGUIWindow.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Helper class used to convert various data types to and from the format expected in Propery strings
*/
class CEGUIEXPORT PropertyHelper
{
public:
	static	float	stringToFloat(const String& str);
	static	uint	stringToUint(const String& str);
	static	int		stringToInt(const String& str);
	static	bool	stringToBool(const String& str);
	static	Size	stringToSize(const String& str);
	static	Point	stringToPoint(const String& str);
    static  Vector3 stringToVector3(const String& str);
	static	Rect	stringToRect(const String& str);
	static const Image*	stringToImage(const String& str);
	static	colour	stringToColour(const String& str);
	static	ColourRect	stringToColourRect(const String& str);
	static	UDim	stringToUDim(const String& str);
	static	UVector2	stringToUVector2(const String& str);
	static	URect	stringToURect(const String& str);

	static String	floatToString(float val);
	static String	uintToString(uint val);
	static String	intToString(int val);
	static String	boolToString(bool val);
	static String	sizeToString(const Size& val);
	static String	pointToString(const Point& val);
    static String   vector3ToString(const Vector3& val);
	static String	rectToString(const Rect& val);
	static String	imageToString(const Image* const val);
	static String	colourToString(const colour& val);
	static String	colourRectToString(const ColourRect& val);
	static String	udimToString(const UDim& val);
	static String	uvector2ToString(const UVector2& val);
	static String	urectToString(const URect& val);
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIPropertyHelper_h_
