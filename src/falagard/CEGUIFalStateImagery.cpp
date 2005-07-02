/************************************************************************
    filename:   CEGUIFalStateImagery.cpp
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "falagard/CEGUIFalStateImagery.h"
#include "CEGUISystem.h"
#include "CEGUIRenderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    StateImagery::StateImagery(const String& name) :
        d_stateName(name)
    {}

    void StateImagery::render(Window& srcWindow) const
    {
        float base_z = 0;

        // render all layers defined for this state
        for(LayersList::const_iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
        {
            (*curr).render(srcWindow, base_z);
			// TODO: Magic number removal
			base_z -= 0.0000001f;
        }
    }

    void StateImagery::render(Window& srcWindow, const Rect& baseRect) const
    {
        float base_z = 0;

        // render all layers defined for this state
        for(LayersList::const_iterator curr = d_layers.begin(); curr != d_layers.end(); ++curr)
        {
            (*curr).render(srcWindow, baseRect, base_z);
            // TODO: Magic number removal
            base_z -= 0.0000001f;
        }
    }

    void StateImagery::addLayer(const LayerSpecification& layer)
    {
        d_layers.insert(layer);
    }

    void StateImagery::clearLayers()
    {
        d_layers.clear();
    }

    const String& StateImagery::getName() const
    {
        return d_stateName;
    }


} // End of  CEGUI namespace section
