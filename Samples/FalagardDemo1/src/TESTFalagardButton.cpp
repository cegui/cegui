/************************************************************************
    filename:   TESTFalagardButton.cpp
    created:    Sat Jun 18 2005
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
#include "TESTFalagardButton.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardButton::WidgetTypeName[] = "FalagardButton";

    FalagardButton::FalagardButton(const String& type, const String& name) :
        PushButton(type, name)
    {
    }

    FalagardButton::~FalagardButton()
    {
    }

    void FalagardButton::drawNormal(float z)
    {
        if (d_needsRedraw)
        {
            d_renderCache.clearCachedImagery();
            d_needsRedraw = false;

            try
            {
                WidgetLookManager::getSingleton().getWidgetLook("ButtonTest").getStateImagery("Normal").render(*this);
            }
            // catch exceptions, but do not exit.
            catch (UnknownObjectException)
            {}
        }
    }

    void FalagardButton::drawHover(float z)
    {
        if (d_needsRedraw)
        {
            d_renderCache.clearCachedImagery();
            d_needsRedraw = false;

            try
            {
                WidgetLookManager::getSingleton().getWidgetLook("ButtonTest").getStateImagery("Hover").render(*this);
            }
            // catch exceptions, but do not exit.
            catch (UnknownObjectException)
            {}
        }
    }

    void FalagardButton::drawPushed(float z)
    {
        if (d_needsRedraw)
        {
            d_renderCache.clearCachedImagery();
            d_needsRedraw = false;

            try
            {
                WidgetLookManager::getSingleton().getWidgetLook("ButtonTest").getStateImagery("Pushed").render(*this);
            }
            // catch exceptions, but do not exit.
            catch (UnknownObjectException)
            {}
        }
    }


    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************
        Create, initialise and return a FalagardButton
    *************************************************************************/
    Window* FalagardButtonFactory::createWindow(const String& name)
    {
        FalagardButton* wnd = new FalagardButton(d_type, name);
        wnd->initialise();

        return wnd;
    }

} // End of  CEGUI namespace section
