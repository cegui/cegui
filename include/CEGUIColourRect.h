/************************************************************************
	filename: 	CEGUIColourRect.h
	created:	8/3/2004
	author:		Paul D Turner
	
	purpose:	Defines class representing colours for four corners of a
				rectangle
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
#ifndef _CEGUIColourRect_h_
#define _CEGUIColourRect_h_

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Class that holds details of colours for the four corners of a rectangle.
*/
class CEGUIBASE_API ColourRect
{
public:
	/*!
	\brief
		Constructor for ColourRect objects (via single colour).  Also handles default construction.
	*/
	ColourRect(colour col = 0xFF000000);


	/*!
	\brief
		Constructor for ColourRect objects
	*/
	ColourRect(colour top_left, colour top_right, colour bottom_left, colour bottom_right);


	/*!
	\brief
		Destructor for ColourRect objects.
	*/
	virtual ~ColourRect(void);


	/*!
	\brief
		Set the alpha value to use for all four corners of the ColourRect.

	\param alpha
		Alpha value to use.

	\return
		Nothing.
	*/
	void	setAlpha(float alpha);


	/*!
	\brief
		Set the alpha value to use for the top edge of the ColourRect.

	\param alpha
		Alpha value to use.

	\return
		Nothing.
	*/
	void	setTopAlpha(float alpha);


	/*!
	\brief
		Set the alpha value to use for the bottom edge of the ColourRect.

	\param alpha
		Alpha value to use.

	\return
		Nothing.
	*/
	void	setBottomAlpha(float alpha);


	/*!
	\brief
		Set the alpha value to use for the left edge of the ColourRect.

	\param alpha
		Alpha value to use.

	\return
		Nothing.
	*/
	void	setLeftAlpha(float alpha);


	/*!
	\brief
		Set the alpha value to use for the right edge of the ColourRect.

	\param alpha
		Alpha value to use.

	\return
		Nothing.
	*/
	void	setRightAlpha(float alpha);


	colour	d_top_left, d_top_right, d_bottom_left, d_bottom_right;		//<! ColourRect component colours
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIColourRect_h_
