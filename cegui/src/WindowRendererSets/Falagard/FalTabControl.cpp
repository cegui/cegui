/***********************************************************************
    filename:   FalTabControl.cpp
    created:    Fri Jul 8 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "FalTabControl.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUITabButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardTabControl::TypeName[] = "Falagard/TabControl";
    FalagardTabControlProperties::TabButtonType FalagardTabControl::d_tabButtonTypeProperty;


    FalagardTabControl::FalagardTabControl(const String& type) :
        TabControlWindowRenderer(type)
    {
        registerProperty(&d_tabButtonTypeProperty);
    }

    void FalagardTabControl::render()
    {
        const StateImagery* imagery;
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // render basic imagery
        imagery = &wlf.getStateImagery(d_window->isDisabled() ? "Disabled" : "Enabled");
        imagery->render(*d_window);
    }

    TabButton* FalagardTabControl::createTabButton(const String& name) const
    {
        if (d_tabButtonType.empty())
        {
            throw InvalidRequestException("FalagardTabControl::createTabButton - d_tabButtonType has not been set!");
        }

        return static_cast<TabButton*>(WindowManager::getSingleton().createWindow(d_tabButtonType, name));
    }

    const String& FalagardTabControl::getTabButtonType() const
    {
        return d_tabButtonType;
    }

    void FalagardTabControl::setTabButtonType(const String& type)
    {
        d_tabButtonType = type;
    }

} // End of  CEGUI namespace section
