/************************************************************************
	filename: 	CEGUICombobox.cpp
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Implementation of Combobox base class
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
#include "elements/CEGUICombobox.h"
#include "elements/CEGUIEditbox.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Constructor for Combobox base class
*************************************************************************/
Combobox::Combobox(const String& type, const String& name) :
	Window(type, name)
{
}


/*************************************************************************
	Destructor for Combobox base class
*************************************************************************/
Combobox::~Combobox(void)
{
}


/*************************************************************************
	return true if the Editbox has input focus.
*************************************************************************/
bool Combobox::hasInputFocus(void) const
{
	return d_editbox->hasInputFocus();
}


/*************************************************************************
	return true if the Editbox is read-only.
*************************************************************************/
bool Combobox::isReadOnly(void) const
{
	return d_editbox->isReadOnly();
}


/*************************************************************************
	return true if the Editbox text is valid given the currently set
	validation string.
*************************************************************************/
bool Combobox::isTextValid(void) const
{
	return d_editbox->isTextValid();
}


/*************************************************************************
	return the currently set validation string
*************************************************************************/
const String& Combobox::getValidationString(void) const
{
	return d_editbox->getValidationString();
}


/*************************************************************************
	return the current position of the carat.
*************************************************************************/
ulong Combobox::getCaratIndex(void) const
{
	return d_editbox->getCaratIndex();
}


/*************************************************************************
	return the current selection start point.
*************************************************************************/
ulong Combobox::getSelectionStartIndex(void) const
{
	return d_editbox->getSelectionStartIndex();
}


/*************************************************************************
	return the current selection end point.
*************************************************************************/
ulong Combobox::getSelectionEndIndex(void) const
{
	return d_editbox->getSelectionEndIndex();
}


/*************************************************************************
	return the length of the current selection (in code points / characters).
*************************************************************************/
ulong Combobox::getSelectionLength(void) const
{
	return d_editbox->getSelectionLength();
}


/*************************************************************************
	return the maximum text length set for this Editbox.
*************************************************************************/
ulong Combobox::getMaxTextLength(void) const
{
	return d_editbox->getMaxTextLength();
}


/*************************************************************************
	return the currently set colour to be used for rendering Editbox text
	in the normal, unselected state.
*************************************************************************/
colour Combobox::getNormalTextColour(void) const	
{
	return d_editbox->getNormalTextColour();
}


/*************************************************************************
	return the currently set colour to be used for rendering the Editbox
	text when within the selected region.
*************************************************************************/
colour Combobox::getSelectedTextColour(void) const
{
	return d_editbox->getSelectedTextColour();
}


/*************************************************************************
	return the currently set colour to be used for rendering the Editbox
	selection highlight when the Editbox is active.
*************************************************************************/
colour Combobox::getNormalSelectBrushColour(void) const
{
	return d_editbox->getNormalSelectBrushColour();
}


/*************************************************************************
	return the currently set colour to be used for rendering the Editbox
	selection highlight when the Editbox is inactive.
*************************************************************************/
colour Combobox::getInactiveSelectBrushColour(void) const
{
	return d_editbox->getInactiveSelectBrushColour();
}


/*************************************************************************
	Specify whether the Editbox is read-only.
*************************************************************************/
void Combobox::setReadOnly(bool setting)
{
	d_editbox->setReadOnly(setting);
}


/*************************************************************************
	Set the text validation string.
*************************************************************************/
void Combobox::setValidationString(const String& validation_string)
{
	d_editbox->setValidationString(validation_string);
}


/*************************************************************************
	Set the current position of the carat.
*************************************************************************/
void Combobox::setCaratIndex(ulong carat_pos)
{
	d_editbox->setCaratIndex(carat_pos);
}


/*************************************************************************
	Define the current selection for the Editbox
*************************************************************************/
void Combobox::setSelection(ulong start_pos, ulong end_pos)
{
	d_editbox->setSelection(start_pos, end_pos);
}


/*************************************************************************
	set the maximum text length for this Editbox.
*************************************************************************/
void Combobox::setMaxTextLength(ulong max_len)
{
	d_editbox->setMaxTextLength(max_len);
}


/*************************************************************************
	Set the colour to be used for rendering Editbox text in the normal,
	unselected state.
*************************************************************************/
void Combobox::setNormalTextColour(colour col)
{
	d_editbox->setNormalTextColour(col);
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox text when within
	the selected region.	
*************************************************************************/
void Combobox::setSelectedTextColour(colour col)
{
	d_editbox->setSelectedTextColour(col);
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox selection
	highlight when the Editbox is active.
*************************************************************************/
void Combobox::setNormalSelectBrushColour(colour col)
{
	d_editbox->setNormalSelectBrushColour(col);
}


/*************************************************************************
	Set the colour to be used for rendering the Editbox selection
	highlight when the Editbox is inactive.
*************************************************************************/
void Combobox::setInactiveSelectBrushColour(colour col)
{
	d_editbox->setInactiveSelectBrushColour(col);
}




} // End of  CEGUI namespace section
