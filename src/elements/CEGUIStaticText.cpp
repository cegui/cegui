/************************************************************************
	filename: 	CEGUIStaticText.cpp
	created:	4/6/2004
	author:		Paul D Turner
	
	purpose:	Implementation of the static text widget class
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
#include "elements/CEGUIStaticText.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
Static Properties for this class
*************************************************************************/
StaticTextProperties::TextColours		StaticText::d_textColoursProperty;
StaticTextProperties::VertFormatting	StaticText::d_vertFormattingProperty;
StaticTextProperties::HorzFormatting	StaticText::d_horzFormattingProperty;


/*************************************************************************
	Constructor for static text widgets.	
*************************************************************************/
StaticText::StaticText(const String& type, const String& name) :
	Static(type, name),
	d_textCols(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF),
	d_horzFormatting(LeftAligned),
	d_vertFormatting(VertCentred)
{
	addStaticTextProperties();
}


/*************************************************************************
	Destructor for static text widgets.
*************************************************************************/
StaticText::~StaticText(void)
{
}


/*************************************************************************
	Sets the colours to be applied when rendering the text.	
*************************************************************************/
void StaticText::setTextColours(const ColourRect& colours)
{
	d_textCols = colours;
	requestRedraw();
}


/*************************************************************************
	Sets the colours to be applied when rendering the text.	
*************************************************************************/
void StaticText::setTextColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour)
{
	d_textCols.d_top_left		= top_left_colour;
	d_textCols.d_top_right		= top_right_colour;
	d_textCols.d_bottom_left	= bottom_left_colour;
	d_textCols.d_bottom_right	= bottom_right_colour;
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the text.
*************************************************************************/
void StaticText::setFormatting(HorzFormatting h_fmt, VertFormatting v_fmt)
{
	d_horzFormatting = h_fmt;
	d_vertFormatting = v_fmt;
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the text.	
*************************************************************************/
void StaticText::setVerticalFormatting(VertFormatting v_fmt)
{
	d_vertFormatting = v_fmt;
	requestRedraw();
}


/*************************************************************************
	Set the formatting required for the text.	
*************************************************************************/
void StaticText::setHorizontalFormatting(HorzFormatting h_fmt)
{
	d_horzFormatting = h_fmt;
	requestRedraw();
}


/*************************************************************************
	Perform the actual rendering for this Window.
*************************************************************************/
void StaticText::drawSelf(float z)
{
	// get whatever base class needs to render.
	Static::drawSelf(z);

	// render text.
	Rect absarea(getUnclippedInnerRect());
	Rect clipper(absarea.getIntersection(getPixelRect()));

	const Font* font = getFont();

	float textHeight = font->getFormattedLineCount(d_text, absarea, (TextFormatting)d_horzFormatting) * font->getLineSpacing();

	// adjust y positioning according to formatting option
	switch(d_vertFormatting)
	{
	case VertCentred:
		absarea.d_top += ((absarea.getHeight() - textHeight) * 0.5f);
		break;

	case BottomAligned:
		absarea.d_top = absarea.d_bottom - textHeight;
		break;
	}

	float alpha = getEffectiveAlpha();
	ColourRect final_cols(
		calculateModulatedAlphaColour(d_textCols.d_top_left, alpha),
		calculateModulatedAlphaColour(d_textCols.d_top_right, alpha),
		calculateModulatedAlphaColour(d_textCols.d_bottom_left, alpha),
		calculateModulatedAlphaColour(d_textCols.d_bottom_right, alpha)
		);

	getFont()->drawText(getText(), absarea, System::getSingleton().getRenderer()->getZLayer(1), clipper, (TextFormatting)d_horzFormatting, final_cols);
}


/*************************************************************************
	Add properties for static text
*************************************************************************/
void StaticText::addStaticTextProperties(void)
{
	addProperty(&d_textColoursProperty);
	addProperty(&d_vertFormattingProperty);
	addProperty(&d_horzFormattingProperty);
}


} // End of  CEGUI namespace section
