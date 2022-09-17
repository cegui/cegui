/***********************************************************************
created:    18/4/2015
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#include "LookNFeelOverview.h"

#include "CEGUI/CEGUI.h"


using namespace CEGUI;

//////////////////////////////////////////////////////////////////////////
/*************************************************************************

LookNFeelOverviewSample class

*************************************************************************/

LookNFeelOverviewSample::LookNFeelOverviewSample()
{
    Sample::d_name = "LookNFeelOverviewSample";
    Sample::d_credits = "Lukas \"Ident\" Meindl";
    Sample::d_description =
        "This sample gives a quick overview of the available stock LookNFeels (Skins) and their skinned widgets."
        "Most widgets and most skins are shown here and can be directly interacted with.";
    Sample::d_summary =
        "The demo loads several layouts, each showing a set of widgets from a single skin. "
        "A combobox is used for selection. It also shows how font and widget scaling works on different resolutions";
    Sample::d_priority = 99;
}

static void fillList(Element* listBase)
{
    if (auto list = dynamic_cast<ListWidget*>(listBase))
    {
        list->addItem("Item 1");
        list->addItem("Item 2");
        list->addItem("Item 3");
        list->setIndexSelectionState((size_t)0u, true);
    }
}

/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool LookNFeelOverviewSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    d_guiContext = guiContext;
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    SchemeManager::getSingleton().createFromFile("AlfiskoSkin.scheme");
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
    guiContext->setDefaultCursorImage("Vanilla-Images/MouseArrow");

    // load the font we are going to use and which is not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("Jura-10.font");
    d_guiContext->setDefaultFont(loadedFonts.empty() ? nullptr : loadedFonts.front());

    // load all layouts we want to use later
    d_taharezOverviewLayout = winMgr.loadLayoutFromFile("TaharezLookOverview.layout");
    d_vanillaOverviewLayout = winMgr.loadLayoutFromFile("VanillaLookOverview.layout");

    // model-based lists do not support child adding from the layout
    // TODO: can override ListWidget::addChild_impl to work as addItem!
    fillList(d_taharezOverviewLayout->getChild("listbox"));
    fillList(d_taharezOverviewLayout->getChild("disabledListbox"));
    fillList(d_vanillaOverviewLayout->getChild("StaticImage/listWidget"));
    fillList(d_vanillaOverviewLayout->getChild("StaticImage/disabledListWidget"));

    // create a root window
    Window* root = winMgr.createWindow("DefaultWindow", "root");
    guiContext->setRootWindow(root);

    CEGUI::Window* skinSelectionContainer = winMgr.createWindow("Vanilla/StaticText", "SkinSelectionContainer");
    skinSelectionContainer->setArea(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)),
        CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    root->addChild(skinSelectionContainer);

    d_loadedLayoutContainer = winMgr.createWindow("DefaultWindow", "LoadedLayoutContainer");
    d_loadedLayoutContainer->setArea(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.1f)),
        CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(0.9f)));
    skinSelectionContainer->addChild(d_loadedLayoutContainer);

    CEGUI::Window* skinSelectionComboboxLabel = winMgr.createWindow("Vanilla/Label", "SkinSelectionComboboxLabel");
    skinSelectionContainer->addChild(skinSelectionComboboxLabel);
    skinSelectionComboboxLabel->setHorizontalAlignment(HorizontalAlignment::Centre);
    skinSelectionComboboxLabel->setText("Choose a Look N' Feel (= a skin) to display");
    skinSelectionComboboxLabel->setArea(CEGUI::UVector2(cegui_reldim(-0.18f), cegui_reldim(0.0f)),
        CEGUI::USize(cegui_reldim(0.36f), cegui_reldim(0.04f)));
    skinSelectionComboboxLabel->setProperty("HorzFormatting", "RightAligned");


    CEGUI::Combobox* skinSelectionCombobox = static_cast<CEGUI::Combobox*>(winMgr.createWindow("Vanilla/Combobox", "SkinSelectionCombobox"));
    skinSelectionContainer->addChild(skinSelectionCombobox);
    skinSelectionCombobox->setHorizontalAlignment(HorizontalAlignment::Centre);
    skinSelectionCombobox->setArea(CEGUI::UVector2(cegui_reldim(0.08f), cegui_reldim(0.003f)),
        CEGUI::USize(cegui_reldim(0.15f), cegui_reldim(0.3f)));
    skinSelectionCombobox->setReadOnly(true);
    skinSelectionCombobox->setSortingEnabled(false);
    skinSelectionCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, Event::Subscriber(&LookNFeelOverviewSample::handleSkinSelectionAccepted, this));

    d_taharezLookListboxItem = new CEGUI::StandardItem("TaharezLook");
    skinSelectionCombobox->addItem(d_taharezLookListboxItem);

    d_vanillaLookListboxItem = new CEGUI::StandardItem("Vanilla");
    skinSelectionCombobox->addItem(d_vanillaLookListboxItem);

    skinSelectionCombobox->setItemSelectState(d_taharezLookListboxItem, true);

    // success!
    return true;
}

/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void LookNFeelOverviewSample::deinitialise()
{
}

/*************************************************************************
An event handler to handle selections
*************************************************************************/
bool LookNFeelOverviewSample::handleSkinSelectionAccepted(const CEGUI::EventArgs& args)
{
    const CEGUI::WindowEventArgs& winEventArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
    CEGUI::Combobox* skinSelectionCombobox = static_cast<CEGUI::Combobox*>(winEventArgs.window);

    const StandardItem* selectedItem = skinSelectionCombobox->getSelectedItem();

    // TODO: need removeAllChildren!
    while (d_loadedLayoutContainer->getChildCount() > 0)
        d_loadedLayoutContainer->removeChild(d_loadedLayoutContainer->getChildAtIndex(0));

    if (selectedItem == d_taharezLookListboxItem)
    {
        d_loadedLayoutContainer->addChild(d_taharezOverviewLayout);
    }
    else if(selectedItem == d_vanillaLookListboxItem)
    {
        d_loadedLayoutContainer->addChild(d_vanillaOverviewLayout);
    }

    return false;
}
