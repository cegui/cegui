/************************************************************************
    filename:   FalItemEntry.cpp
    created:    Thu Sep 22 2005
    author:     Tomas Lindquist Olsen
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
#include "FalItemEntry.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardItemEntry::TypeName[] = "Falagard/ItemEntry";

    FalagardItemEntry::FalagardItemEntry(const String& type) :
        ItemEntryWindowRenderer(type)
    {
    }

    void FalagardItemEntry::render()
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // render basic imagery
        wlf.getStateImagery(d_window->isDisabled() ? "Disabled" : "Enabled").render(*d_window);
    }

    Size FalagardItemEntry::getItemPixelSize() const
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        return wlf.getNamedArea("ContentSize").getArea().getPixelRect(*d_window).getSize();
    }

} // End of  CEGUI namespace section
