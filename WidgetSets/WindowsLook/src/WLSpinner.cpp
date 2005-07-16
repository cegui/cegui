/************************************************************************
    filename: 	WLSpinner.cpp
    created:	3/2/2005
    author:		Paul D Turner
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
#include "WLSpinner.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "CEGUIWindowManager.h"
#include "WLButton.h"
#include "WLEditbox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    //////////////////////////////////////////////////////////////////////////
    // type name for this widget
    const utf8 WLSpinner::WidgetTypeName[] = "WindowsLook/Spinner";
    // image name constants
    const utf8 WLSpinner::ImagesetName[]                 = "WindowsLook";
    const utf8 WLSpinner::UpButtonNormalImageName[]      = "LargeUpArrow";
    const utf8 WLSpinner::UpButtonHighlightImageName[]   = "LargeUpArrow";
    const utf8 WLSpinner::DownButtonNormalImageName[]    = "LargeDownArrow";
    const utf8 WLSpinner::DownButtonHighlightImageName[] = "LargeDownArrow";
    // type names for the component widgets
    const utf8*	WLSpinner::IncreaseButtonWidgetType = WLButton::WidgetTypeName;
    const utf8*	WLSpinner::DecreaseButtonWidgetType = WLButton::WidgetTypeName;
    const utf8* WLSpinner::EditboxWidgetType        = WLEditbox::WidgetTypeName;
    //////////////////////////////////////////////////////////////////////////

    WLSpinner::WLSpinner(const String& type, const String& name) :
        Spinner(type, name)
    {
        d_buttonsWidth =  ImagesetManager::getSingleton().getImageset(ImagesetName)->
            getImage(DownButtonNormalImageName).getWidth() * 1.5f;
    }

    WLSpinner::~WLSpinner(void)
    {
    }

    PushButton* WLSpinner::createIncreaseButton(const String& name) const
    {
        // create the widget
        WLButton* btn = (WLButton*)WindowManager::getSingleton().createWindow(IncreaseButtonWidgetType, name);

        // perform some initialisation
        btn->setStandardImageryEnabled(true);
        btn->setCustomImageryAutoSized(true);

        Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
        RenderableImage img;
        img.setHorzFormatting(RenderableImage::HorzCentred);
        img.setVertFormatting(RenderableImage::VertCentred);

        img.setImage(&iset->getImage(UpButtonNormalImageName));
        btn->setNormalImage(&img);
        btn->setDisabledImage(&img);
        img.setImage(&iset->getImage(UpButtonHighlightImageName));
        btn->setHoverImage(&img);
        btn->setPushedImage(&img);

        return btn;
    }

    PushButton* WLSpinner::createDecreaseButton(const String& name) const
    {
        // create the widget
        WLButton* btn = (WLButton*)WindowManager::getSingleton().createWindow(DecreaseButtonWidgetType, name);

        // perform some initialisation
        btn->setStandardImageryEnabled(true);
        btn->setCustomImageryAutoSized(true);

        Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
        RenderableImage img;
        img.setHorzFormatting(RenderableImage::HorzCentred);
        img.setVertFormatting(RenderableImage::VertCentred);
        img.setImage(&iset->getImage(DownButtonNormalImageName));
        btn->setNormalImage(&img);
        btn->setDisabledImage(&img);
        img.setImage(&iset->getImage(DownButtonHighlightImageName));
        btn->setHoverImage(&img);
        btn->setPushedImage(&img);

        return btn;
    }

    Editbox* WLSpinner::createEditbox(const String& name) const
    {
        // create the widget
        Editbox* ebox = (Editbox*)WindowManager::getSingleton().createWindow(EditboxWidgetType, name);
        return ebox;
    }

    void WLSpinner::performChildWindowLayout()
    {
        Spinner::performChildWindowLayout();

        Point pt(0,0);
        Size sz(getAbsoluteSize());
        sz.d_width -= d_buttonsWidth;
        d_editbox->setPosition(pt);
        d_editbox->setSize(Absolute, sz);
        
        pt.d_x = d_editbox->getWidth();
        sz.d_width = d_buttonsWidth;
        sz.d_height *= 0.5f;
        d_increaseButton->setPosition(pt);
        d_increaseButton->setSize(Absolute, sz);
        pt.d_y = 0.5f;
        d_decreaseButton->setPosition(pt);
        d_decreaseButton->setSize(Absolute, sz);
    }

    //////////////////////////////////////////////////////////////////////////

    Window* WLSpinnerFactory::createWindow(const String& name)
    {
        return new WLSpinner(d_type, name);
    }

} // End of  CEGUI namespace section
