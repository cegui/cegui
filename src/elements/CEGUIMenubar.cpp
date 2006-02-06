/************************************************************************
	filename: 	CEGUIMenubar.cpp
	created:	31/3/2005
	author:		Tomas Lindquist Olsen (based on code by Paul D Turner)
	
	purpose:	Implementation of Menubar widget base class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#include "elements/CEGUIMenubar.h"
#include "elements/CEGUIPopupMenu.h"
#include "elements/CEGUIMenuItem.h"
#include "CEGUICoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
	Constants
*************************************************************************/
const String Menubar::EventNamespace("Menubar");
const String Menubar::WidgetTypeName("CEGUI/Menubar");


/*************************************************************************
	Constructor for Menubar base class.
*************************************************************************/
Menubar::Menubar(const String& type, const String& name)
	: MenuBase(type, name)
{
	d_itemSpacing = 10;
}


/*************************************************************************
	Destructor for Menubar base class.
*************************************************************************/
Menubar::~Menubar(void)
{
}


/*************************************************************************
	Sets up sizes and positions for attached ItemEntry children.
*************************************************************************/
void Menubar::layoutItemWidgets()
{
	Rect render_rect = getItemRenderArea();
	float x0 = PixelAligned(render_rect.d_left);

	URect rect;

	ItemEntryList::iterator item = d_listItems.begin();
	while ( item != d_listItems.end() )
	{
		const Size optimal = (*item)->getItemPixelSize();

		(*item)->setVerticalAlignment(VA_CENTRE);
		rect.setPosition(UVector2(cegui_absdim(x0), cegui_absdim(0)) );
		rect.setSize( UVector2( cegui_absdim(PixelAligned(optimal.d_width)),
                                cegui_absdim(PixelAligned(optimal.d_height))));

		(*item)->setWindowArea(rect);

		x0 += optimal.d_width + d_itemSpacing;
		++item;
	}

}


/*************************************************************************
	Returns the "optimal" size for the content in unclipped pixels
*************************************************************************/
Size Menubar::getContentSize() const
{
	// find the content sizes
	float tallest = 0;
	float total_width = 0;

	size_t i = 0;
	size_t max = d_listItems.size();
	while (i < max)
	{
		const Size sz = d_listItems[i]->getItemPixelSize();
		if (sz.d_height > tallest)
			tallest = sz.d_height;
		total_width += sz.d_width;

		i++;
	}

	const float count = float(i);

	// horz item spacing
	if (count >= 2)
	{
	    total_width += (count-1)*d_itemSpacing;
	}

	// return the content size
	return Size(total_width, tallest);
}

} // End of  CEGUI namespace section
