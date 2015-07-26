/***********************************************************************
created:    18/4/2015
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & Thce CEGUI Development Team
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
#include "CEGUI/InputEvent.h"

#include <cmath>


using namespace CEGUI;

/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool LookNFeelOverviewDemo::initialise(CEGUI::GUIContext* guiContext)
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
    guiContext->getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // load all Fonts we are going to use and which are not loaded via scheme
    d_fontForTaharez = &FontManager::getSingleton().createFromFile("Jura-10.font");
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

    // load all layouts we want to use later
    d_taharezOverviewLayout = winMgr.loadLayoutFromFile("TaharezLookOverview.layout");
    d_vanillaOverviewLayout = winMgr.loadLayoutFromFile("VanillaLookOverview.layout");

    // create a root window
    Window* root = winMgr.createWindow("DefaultWindow", "root");
    guiContext->setRootWindow(root);

    CEGUI::Window* skinSelectionContainer = winMgr.createWindow("Vanilla/StaticText", "SkinSelectionContainer");
    skinSelectionContainer->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
    skinSelectionContainer->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    root->addChild(skinSelectionContainer);

    d_loadedLayoutContainer = winMgr.createWindow("DefaultWindow", "LoadedLayoutContainer");
    d_loadedLayoutContainer->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.1f)));
    d_loadedLayoutContainer->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(0.9f)));
    skinSelectionContainer->addChild(d_loadedLayoutContainer);

    CEGUI::Window* skinSelectionComboboxLabel = winMgr.createWindow("Vanilla/Label", "SkinSelectionComboboxLabel");
    skinSelectionContainer->addChild(skinSelectionComboboxLabel);
    skinSelectionComboboxLabel->setHorizontalAlignment(HA_CENTRE);
    skinSelectionComboboxLabel->setText("Choose a Look N' Feel (= a skin) to display");
    skinSelectionComboboxLabel->setPosition(CEGUI::UVector2(cegui_reldim(-0.18f), cegui_reldim(0.0f)));
    skinSelectionComboboxLabel->setSize(CEGUI::USize(cegui_reldim(0.36f), cegui_reldim(0.04f)));
    skinSelectionComboboxLabel->setProperty("HorzFormatting", "RightAligned");


    CEGUI::Combobox* skinSelectionCombobox = static_cast<CEGUI::Combobox*>(winMgr.createWindow("Vanilla/Combobox", "SkinSelectionCombobox"));
    skinSelectionContainer->addChild(skinSelectionCombobox);
    skinSelectionCombobox->setHorizontalAlignment(HA_CENTRE);
    skinSelectionCombobox->setPosition(CEGUI::UVector2(cegui_reldim(0.08f), cegui_reldim(0.003f)));
    skinSelectionCombobox->setSize(CEGUI::USize(cegui_reldim(0.15f), cegui_reldim(0.3f)));
    skinSelectionCombobox->setReadOnly(true);
    skinSelectionCombobox->setSortingEnabled(false);
    skinSelectionCombobox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, Event::Subscriber(&LookNFeelOverviewDemo::handleSkinSelectionAccepted, this));

    d_taharezLookListboxItem = new CEGUI::ListboxTextItem("TaharezLook");
    skinSelectionCombobox->addItem(d_taharezLookListboxItem);
    d_taharezLookListboxItem->setSelectionBrushImage("Vanilla-Images/GenericBrush");

    d_vanillaLookListboxItem = new CEGUI::ListboxTextItem("Vanilla");
    skinSelectionCombobox->addItem(d_vanillaLookListboxItem);
    d_vanillaLookListboxItem->setSelectionBrushImage("Vanilla-Images/GenericBrush");

    skinSelectionCombobox->setItemSelectState(d_taharezLookListboxItem, true);
    WindowEventArgs winArgs(skinSelectionCombobox);
    skinSelectionCombobox->fireEvent(Combobox::EventListSelectionAccepted, winArgs);

    // success!
    return true;
}

/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void LookNFeelOverviewDemo::deinitialise()
{
}

/*************************************************************************
An event handler to handle selections
*************************************************************************/
bool LookNFeelOverviewDemo::handleSkinSelectionAccepted(const CEGUI::EventArgs& args)
{
    const CEGUI::WindowEventArgs& winEventArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
    CEGUI::Combobox* skinSelectionCombobox = static_cast<CEGUI::Combobox*>(winEventArgs.window);

    ListboxItem* selectedItem = skinSelectionCombobox->getSelectedItem();

    while(d_loadedLayoutContainer->getChildCount() > 0)
        d_loadedLayoutContainer->removeChild(d_loadedLayoutContainer->getChildAtIdx(0));

    if(selectedItem == d_taharezLookListboxItem)
    {
        d_loadedLayoutContainer->addChild(d_taharezOverviewLayout);
        
        // Set default font for the gui context
        d_guiContext->setDefaultFont(d_fontForTaharez);
    }
    else if(selectedItem == d_vanillaLookListboxItem)
    {
        d_loadedLayoutContainer->addChild(d_vanillaOverviewLayout);
        d_guiContext->setDefaultFont(d_fontForTaharez); 
    }

    return false;
}

/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static LookNFeelOverviewDemo sample;
    return sample;
}