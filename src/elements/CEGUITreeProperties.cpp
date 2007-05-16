/***********************************************************************
filename:   CEGUITreeProperties.cpp
created:	5-13-07
author:		Jonathan Welch (Based on Code by David Durant)
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
#include "elements/CEGUITreeProperties.h"
#include "elements/CEGUIListbox.h"
#include "CEGUIPropertyHelper.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TreeProperties namespace section
namespace TreeProperties
{
String	Sort::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Listbox*>(receiver)->isSortEnabled());
}


void	Sort::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Listbox*>(receiver)->setSortingEnabled(PropertyHelper::stringToBool(value));
}


String	MultiSelect::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Listbox*>(receiver)->isMultiselectEnabled());
}


void	MultiSelect::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Listbox*>(receiver)->setMultiselectEnabled(PropertyHelper::stringToBool(value));
}


String	ForceVertScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Listbox*>(receiver)->isVertScrollbarAlwaysShown());
}


void	ForceVertScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Listbox*>(receiver)->setShowVertScrollbar(PropertyHelper::stringToBool(value));
}


String	ForceHorzScrollbar::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Listbox*>(receiver)->isHorzScrollbarAlwaysShown());
}


void	ForceHorzScrollbar::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Listbox*>(receiver)->setShowHorzScrollbar(PropertyHelper::stringToBool(value));
}

String	ItemTooltips::get(const PropertyReceiver* receiver) const
{
	return PropertyHelper::boolToString(static_cast<const Listbox*>(receiver)->isItemTooltipsEnabled());
}


void	ItemTooltips::set(PropertyReceiver* receiver, const String& value)
{
	static_cast<Listbox*>(receiver)->setItemTooltipsEnabled(PropertyHelper::stringToBool(value));
}

} // End of  TreeProperties namespace section

} // End of  CEGUI namespace section
