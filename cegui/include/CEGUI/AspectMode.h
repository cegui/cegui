/***********************************************************************
	created:	19th August 2015
	author:		Lukas Meindl (based on code by Paul D Turner)
	
	purpose:	Defines the AspectMode enum
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIAspectMode_h_
#define _CEGUIAspectMode_h_


// Start of CEGUI namespace section
namespace CEGUI
{
    
/*
\brief
    The AspectMode enumerators are used for the auto scale features of CEGUI
*/
enum class AspectMode
{
    //! Ignores the target aspect (default)
    NONE,
    /*!
    Satisfies the aspect ratio by shrinking the size as little
    as possible to fit inside it
    */
    SHRINK,
    /*!
    Satisfies the aspect ratio by expanding the widget as little
    as possible outside it
    */
    EXPAND,
    //! Satisfy the aspect ratio by adjusting the height according to the width.
    ADJUST_HEIGHT,
    //! Satisfy the aspect ratio by adjusting the width according to the height.
    ADJUST_WIDTH
};


} // End of  CEGUI namespace section

#endif
