/************************************************************************
    filename:   FalRadioButton.cpp
    created:    Thu Jun 23 2005
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
#include "FalRadioButton.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardRadioButton::WidgetTypeName[] = "Falagard/RadioButton";

    FalagardRadioButton::FalagardRadioButton(const String& type, const String& name) :
        RadioButton(type, name)
    {
    }

    FalagardRadioButton::~FalagardRadioButton()
    {
    }

    void FalagardRadioButton::populateRenderCache()
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);

        bool norm = false;
        String state = d_selected ? "Selected" : "";

		if (isDisabled())
		{
		    state += "Disabled";
		}
		else if (d_pushed)
		{
		    state += "Pushed";
		}
		else if (d_hovering)
		{
		    state += "Hover";
		}
		else
		{
		    state += "Normal";
		    norm = true;
		}

        if (!norm && !wlf.isStateImageryPresent(state))
        {
            state = d_selected ? "SelectedNormal" : "Normal";
        }

        wlf.getStateImagery(state).render(*this);
    }

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardRadioButtonFactory::createWindow(const String& name)
    {
        return new FalagardRadioButton(d_type, name);
    }

    void FalagardRadioButtonFactory::destroyWindow(Window* window)
    {
        delete window;
    }

} // End of  CEGUI namespace section
