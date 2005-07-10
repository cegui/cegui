/************************************************************************
    filename: 	TLSpinner.cpp
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
#include "TLSpinner.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIFont.h"
#include "CEGUIWindowManager.h"
#include "TLButton.h"
#include "TLEditbox.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    //////////////////////////////////////////////////////////////////////////
    // type name for this widget
    const utf8 TLSpinner::WidgetTypeName[] = "TaharezLook/Spinner";
    // image name constants
    const utf8 TLSpinner::ImagesetName[]                 = "TaharezLook";
    const utf8 TLSpinner::UpButtonNormalImageName[]      = "VertScrollUpNormal";
    const utf8 TLSpinner::UpButtonHighlightImageName[]   = "VertScrollUpHover";
    const utf8 TLSpinner::DownButtonNormalImageName[]    = "VertScrollDownNormal";
    const utf8 TLSpinner::DownButtonHighlightImageName[] = "VertScrollDownHover";
    // type names for the component widgets
    const utf8*	TLSpinner::IncreaseButtonWidgetType = TLButton::WidgetTypeName;
    const utf8*	TLSpinner::DecreaseButtonWidgetType = TLButton::WidgetTypeName;
    const utf8* TLSpinner::EditboxWidgetType        = TLEditbox::WidgetTypeName;
    //////////////////////////////////////////////////////////////////////////

    TLSpinner::TLSpinner(const String& type, const String& name) :
        Spinner(type, name)
    {
        d_buttonsWidth =  ImagesetManager::getSingleton().getImageset(ImagesetName)->
            getImage(DownButtonNormalImageName).getWidth();
    }

    TLSpinner::~TLSpinner(void)
    {
    }

    PushButton* TLSpinner::createIncreaseButton(const String& name) const
    {
        // create the widget
        TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(IncreaseButtonWidgetType, name);

        // perform some initialisation
        btn->setStandardImageryEnabled(false);
        btn->setCustomImageryAutoSized(true);

        Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
        RenderableImage img;
        img.setHorzFormatting(RenderableImage::HorzStretched);
        img.setVertFormatting(RenderableImage::VertStretched);

        img.setImage(&iset->getImage(UpButtonNormalImageName));
        btn->setNormalImage(&img);
        btn->setDisabledImage(&img);
        img.setImage(&iset->getImage(UpButtonHighlightImageName));
        btn->setHoverImage(&img);
        btn->setPushedImage(&img);

        return btn;
    }

    PushButton* TLSpinner::createDecreaseButton(const String& name) const
    {
        // create the widget
        TLButton* btn = (TLButton*)WindowManager::getSingleton().createWindow(DecreaseButtonWidgetType, name);

        // perform some initialisation
        btn->setStandardImageryEnabled(false);
        btn->setCustomImageryAutoSized(true);

        Imageset* iset = ImagesetManager::getSingleton().getImageset(ImagesetName);
        RenderableImage img;
        img.setHorzFormatting(RenderableImage::HorzStretched);
        img.setVertFormatting(RenderableImage::VertStretched);
        img.setImage(&iset->getImage(DownButtonNormalImageName));
        btn->setNormalImage(&img);
        btn->setDisabledImage(&img);
        img.setImage(&iset->getImage(DownButtonHighlightImageName));
        btn->setHoverImage(&img);
        btn->setPushedImage(&img);

        return btn;
    }

    Editbox* TLSpinner::createEditbox(const String& name) const
    {
        // create the widget
        Editbox* ebox = (Editbox*)WindowManager::getSingleton().createWindow(EditboxWidgetType, name);
        return ebox;
    }

    void TLSpinner::layoutComponentWidgets(void)
    {
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

    Window* TLSpinnerFactory::createWindow(const String& name)
    {
        return new TLSpinner(d_type, name);
    }

} // End of  CEGUI namespace section
