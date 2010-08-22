/***********************************************************************
	filename: 	CEGUISize.h
	created:	14/3/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for Size class
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
#ifndef _CEGUISize_h_
#define _CEGUISize_h_

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Class that holds the size (width & height) of something.
*/
class CEGUIEXPORT Size
{
public:
	Size(void) {}
	Size(float width, float height) : d_width(width), d_height(height) {}
	Size(const Size& v): d_width(v.d_width), d_height(v.d_height) {}

	bool operator==(const Size& other) const;
	bool operator!=(const Size& other) const;

	float d_width, d_height;

	Size operator*(float c) const
	{
		return Size(d_width * c, d_height * c);
	}

	Size operator+(const Size& s) const
	{
		return Size(d_width + s.d_width, d_height + s.d_height);
	}
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUISize_h_

