/************************************************************************
    filename:   CEGuiRendererSelector.cpp
    created:    24/9/2004
    author:     Paul D Turner
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
#include "CEGuiRendererSelector.h"


/*************************************************************************
    Constructor.
*************************************************************************/
CEGuiRendererSelector::CEGuiRendererSelector()
{
    d_lastSelected = InvalidGuiRendererType;

    // make all renderer types un-available.  sub-classes should enable appropriate types.
    for (int i = 0; i < RendererTypeCount; ++i)
    {
        d_rendererAvailability[i] = false;
    }
}


/*************************************************************************
    Destructor.
*************************************************************************/
CEGuiRendererSelector::~CEGuiRendererSelector()
{}


/*************************************************************************
    Return the CEGuiRendererType value of the renderer most recently
    selected by the user.
*************************************************************************/
CEGuiRendererType CEGuiRendererSelector::getSelectedRendererType()
{
    return d_lastSelected;
}


/*************************************************************************
    Set whether or not a specific renderer type will be available for
    selection from the dialog the next time it is displayed.
*************************************************************************/
void CEGuiRendererSelector::setRendererAvailability(CEGuiRendererType rendererType, bool available)
{
    if (rendererType < RendererTypeCount)
    {
        d_rendererAvailability[rendererType] = available;
    }
}
