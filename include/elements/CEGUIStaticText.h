/************************************************************************
	filename: 	CEGUIStaticText.h
	created:	4/6/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for a static text widget
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
#ifndef _CEGUIStaticText_h_
#define _CEGUIStaticText_h_

#include "elements/CEGUIStatic.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class for a static text widget.

	This base class performs it's own rendering.  There is no need to override this widget to perform rendering
	of static texts.
*/
class CEGUIBASE_API StaticText : public Static
{
public:
	/*************************************************************************
		Formatting Enumerations
	*************************************************************************/
	/*!
	\brief
		Enumeration of horizontal formatting options for static text widgets
	*/
	enum HorzFormatting
	{
		LeftAligned,			//!< Text is output as a single line of text with the first character aligned with the left edge of the widget.
		RightAligned,			//!< Text is output as a single line of text with the last character aligned with the right edge of the widget.
		HorzCentred,			//!< Text is output as a single line of text horizontally centred within the widget.
		WordWrapLeftAligned,	//!< Text is output as multiple word-wrapped lines of text with the first character of each line aligned with the left edge of the widget.
		WordWrapRightAligned,	//!< Text is output as multiple word-wrapped lines of text with the last character of each line aligned with the right edge of the widget.
		WordWrapCentred,		//!< Text is output as multiple word-wrapped lines of text with each line horizontally centered within the widget.
	};


	/*!
	\brief
		Enumeration of vertical formatting options for a static text widgets
	*/
	enum VertFormatting
	{
		TopAligned,		//!< Text is output with the top of first line of text aligned with the top edge of the widget.
		BottomAligned,	//!< Text is output with the bottom of last line of text aligned with the bottom edge of the widget.
		VertCentred,	//!< Text is output vertically centred within the widget.
	};


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for static text widgets.
	*/
	StaticText(const String& type, const String& name);


	/*!
	\brief
		Destructor for static text widgets.
	*/
	virtual ~StaticText(void);


	/*************************************************************************
		Accessors
	*************************************************************************/
	/*!
	\brief
		Return a ColourRect object containing the colours used when rendering this widget.

	\return
		ColourRect object initialised with the colours used when rendering the text for this widget.
	*/
	ColourRect	getTextColours(void) const		{return d_textCols;}

	
	/*!
	\brief
		Return the current horizontal formatting option set for this widget.

	\return
		One of the StaticText::HorzFormatting enumerated values specifying the horizontal formatting currently set.
	*/
	HorzFormatting	getHorizontalFormatting(void) const		{return	d_horzFormatting;}


	/*!
	\brief
		Return the current vertical formatting option set for this widget.

	\return
		One of the StaticText::VertFormatting enumerated values specifying the vertical formatting currently set.
	*/
	VertFormatting	getVerticalFormatting(void) const		{return	d_vertFormatting;}


	/*************************************************************************
		Manipulators
	*************************************************************************/
	/*!
	\brief
		Sets the colours to be applied when rendering the text.

	\param colours
		ColourRect object describing the colours to be used for each text glyph rendered.

	\return 
		Nothing.
	*/
	void	setTextColours(const ColourRect& colours);


	/*!
	\brief
		Sets the colours to be applied when rendering the text.

	\param top_left_colour
		Colour (as ARGB value) to be applied to the top-left corner of each text glyph rendered.

	\param top_right_colour
		Colour (as ARGB value) to be applied to the top-right corner of each text glyph rendered.

	\param bottom_left_colour
		Colour (as ARGB value) to be applied to the bottom-left corner of each text glyph rendered.

	\param bottom_right_colour
		Colour (as ARGB value) to be applied to the bottom-right corner of each text glyph rendered.

	\return 
		Nothing.
	*/
	void	setTextColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour);


	/*!
	\brief
		Sets the colour to be applied when rendering the text.

	\param col
		colour value to be used when rendering.

	\return
		Nothing.
	*/
	void	setTextColours(colour col)		{setTextColours(col, col, col, col);}


	/*!
	\brief
		Set the formatting required for the text.

	\param h_fmt
		One of the HorzFormatting enumerated values specifying the formatting required.

	\param v_fmt
		One of the VertFormatting enumerated values specifying the formatting required.

	\return
		Nothing
	*/
	void	setFormatting(HorzFormatting h_fmt, VertFormatting v_fmt);


	/*!
	\brief
		Set the formatting required for the text.

	\param v_fmt
		One of the VertFormatting enumerated values specifying the formatting required.

	\return
		Nothing
	*/
	void	setVerticalFormatting(VertFormatting v_fmt);


	/*!
	\brief
		Set the formatting required for the text.

	\param h_fmt
		One of the HorzFormatting enumerated values specifying the formatting required.

	\return
		Nothing
	*/
	void	setHorizontalFormatting(HorzFormatting h_fmt);


protected:
	/*************************************************************************
		Overridden from base class
	*************************************************************************/
	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual	void	drawSelf(float z);


	/*************************************************************************
		Implementation methods
	*************************************************************************/

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	HorzFormatting	d_horzFormatting;
	VertFormatting	d_vertFormatting;
	ColourRect		d_textCols;
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIStaticText_h_
