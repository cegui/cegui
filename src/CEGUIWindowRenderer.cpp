/************************************************************************
    filename:   CEGUIWindowRenderer.cpp
    created:    Jan 11 2006
    author:     Tomas Lindquist Olsen

    purpose:    Implements interface for the WindowRenderer base class
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#include "CEGUIWindowRenderer.h"
#include "falagard/CEGUIFalWidgetLookManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/************************************************************************
    Constructor
*************************************************************************/
WindowRenderer::WindowRenderer(const String& name, const String& class_name) :
    d_window(0),
    d_name(name),
    d_class(class_name)
{
}

/************************************************************************
    Destructor
*************************************************************************/
WindowRenderer::~WindowRenderer()
{
}

/************************************************************************
    Get the Look'N'Feel assigned to our window
*************************************************************************/
const WidgetLookFeel& WindowRenderer::getLookNFeel() const
{
    return WidgetLookManager::getSingleton().getWidgetLook(d_window->getLookNFeel());
}

/************************************************************************
    Get unclipped inner rectangle.
*************************************************************************/
Rect WindowRenderer::getUnclippedInnerRect() const
{
    return d_window->getUnclippedPixelRect();
}

/************************************************************************
    Get actual pixel rectangle
*************************************************************************/
Rect WindowRenderer::getPixelRect() const
{
    return d_window->getPixelRect_impl();
}

/************************************************************************
    Register property with window renderer
*************************************************************************/
void WindowRenderer::registerProperty(Property* property)
{
    d_properties.push_back(property);
}

/************************************************************************
    On attached to window
*************************************************************************/
void WindowRenderer::onAttach()
{
    PropertyList::iterator i = d_properties.begin();
    while (i != d_properties.end())
    {
        d_window->addProperty(*i);
        ++i;
    }
}

/************************************************************************
    On detached from window
*************************************************************************/
void WindowRenderer::onDetach()
{
    PropertyList::reverse_iterator i = d_properties.rbegin();
    while (i != d_properties.rend())
    {
        d_window->removeProperty((*i)->getName());
        ++i;
    }
}

} // End of CEGUI namespace
