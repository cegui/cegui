/***********************************************************************
	created:	5th January 2016
	author:		Lukas Meindl
	
	purpose:	Defines the VerticalAlignment enum
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2017 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIVerticalAlignment_h_
#define _CEGUIVerticalAlignment_h_

namespace CEGUI
{

/*!
\brief Enumerated type used when specifying vertical alignments for Element

\see HorizontalAlignment
 */
enum class VerticalAlignment : int
{
    /**
     * Element's position specifies an offset of it's top edge from the top edge
     * of it's parent.
     */
    Top,
    /**
     * Element's position specifies an offset of it's vertical centre from the
     * vertical centre of it's parent.
     */
    Centre,
    /**
     * Element's position specifies an offset of it's bottom edge from the
     * bottom edge of it's parent.
     */
    Bottom
};

}

#endif