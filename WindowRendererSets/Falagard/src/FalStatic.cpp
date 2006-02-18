/************************************************************************
    filename:   FalStatic.cpp
    created:    Sat Sep 17 2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
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
#include "FalStatic.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardStatic::TypeName[] = "Falagard/Static";

    FalagardStaticProperties::FrameEnabled          FalagardStatic::d_frameEnabledProperty;
    FalagardStaticProperties::BackgroundEnabled     FalagardStatic::d_backgroundEnabledProperty;

    FalagardStatic::FalagardStatic(const String& type) :
        WindowRenderer(type),
        d_frameEnabled(false),
        d_backgroundEnabled(false)
    {
        registerProperty(&d_frameEnabledProperty);
        registerProperty(&d_backgroundEnabledProperty);
    }

    void FalagardStatic::setFrameEnabled(bool setting)
    {
        if (d_frameEnabled != setting)
        {
            d_frameEnabled = setting;
            d_window->requestRedraw();
        }
    }

    void FalagardStatic::setBackgroundEnabled(bool setting)
    {
        if (d_backgroundEnabled != setting)
        {
            d_backgroundEnabled = setting;
            d_window->requestRedraw();
        }
    }

    void FalagardStatic::render()
    {
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();

		bool is_enabled = !d_window->isDisabled();

        // render frame section
        if (d_frameEnabled)
        {
            wlf.getStateImagery(is_enabled ? "EnabledFrame" : "DisabledFrame").render(*d_window);
        }

        // render background section
        if (d_backgroundEnabled)
        {
            const StateImagery* imagery;
            if (d_frameEnabled)
            {
                imagery = &wlf.getStateImagery(is_enabled ? "WithFrameEnabledBackground" : "WithFrameDisabledBackground");
            }
            else
            {
                imagery = &wlf.getStateImagery(is_enabled ? "NoFrameEnabledBackground" : "NoFrameDisabledBackground");
            }
            // peform the rendering operation.
            imagery->render(*d_window);
        }

        // render basic imagery
        wlf.getStateImagery(is_enabled ? "Enabled" : "Disabled").render(*d_window);
    }

} // End of  CEGUI namespace section
