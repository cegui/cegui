/***********************************************************************
    created:    Fri Apr 22 2011
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#include "Sample_InventoryDemo.h"
#include "InventoryItem.h"
#include "InventoryItemRenderer.h"
#include "InventoryReceiver.h"

#include <CEGUI/CEGUI.h>

//----------------------------------------------------------------------------//
bool InventoryDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

     d_usedFiles = CEGUI::String(__FILE__);

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // basic system init using TaharezLook.
    WindowManager& winMgr = WindowManager::getSingleton();
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    Window* root = winMgr.createWindow("DefaultWindow", "root");
    guiContext->setRootWindow(root);

    // register custom objects with CEGUI.
    WindowFactoryManager::addFactory<TplWindowFactory<InventoryReceiver> >();
    WindowFactoryManager::addFactory<TplWindowFactory<InventoryItem> >();
    WindowRendererManager::addFactory<TplWindowRendererFactory<InventoryItemRenderer> >();

    // load looknfeel for custom inventory components (needs TaharezLook images)
    WidgetLookManager::getSingleton().parseLookNFeelSpecificationFromFile("InventoryComponents.looknfeel");

    // create mapping for the item type
    // This is the equivalent to the following entry in a scheme xml file:
    // <FalagardMapping WindowType="TaharezLook/InventoryItem" TargetType="InventoryItem" LookNFeel="TaharezLook/InventoryItem" Renderer="InventoryItemRenderer" />
    WindowFactoryManager::getSingleton().addFalagardWindowMapping(
        "TaharezLook/InventoryItem",    // type to create
        "InventoryItem",                // 'base' widget type
        "TaharezLook/InventoryItem",    // WidgetLook to use.
        "InventoryItemRenderer");       // WindowRenderer to use.

    // Create Backpack window
    Window* wnd = winMgr.createWindow("TaharezLook/FrameWindow");
    root->addChild(wnd);
    wnd->setPosition(UVector2(UDim(0.1f, 0), UDim(0.1f, 0)));
    wnd->setSize(USize(UDim(0.2f, 0), UDim(0.4f, 0)));
    wnd->setText("Backpack");

    InventoryReceiver& receiver1 = dynamic_cast<InventoryReceiver&>(*winMgr.createWindow("InventoryReceiver"));
    wnd->addChild(&receiver1);
    receiver1.setPosition(UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
    receiver1.setSize(USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    receiver1.setContentSize(3, 6);
    receiver1.setUserString("BlockImage", "TaharezLook/GenericBrush");

    // Create vault window
    Window* wnd2 = winMgr.createWindow("TaharezLook/FrameWindow");
    root->addChild(wnd2);
    wnd2->setPosition(UVector2(UDim(0.48f, 0), UDim(0.2f, 0)));
    wnd2->setSize(USize(UDim(0.5f, 0), UDim(0.5f, 0)));
    wnd2->setText("Bank Vault");

    InventoryReceiver& receiver2 = dynamic_cast<InventoryReceiver&>(*winMgr.createWindow("InventoryReceiver"));
    wnd2->addChild(&receiver2);
    receiver2.setPosition(UVector2(cegui_reldim(0.0f), cegui_reldim( 0.0f)));
    receiver2.setSize(USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    receiver2.setContentSize(10, 10);
    receiver2.setUserString("BlockImage", "TaharezLook/GenericBrush");

    // create some items and add them to the vault.
    InventoryItem& item1 = dynamic_cast<InventoryItem&>(*winMgr.createWindow("TaharezLook/InventoryItem"));
    item1.setContentSize(2, 2);
    receiver2.addItemAtLocation(item1, 0, 0);
    item1.setProperty("Image", "TaharezLook/MouseArrow");

    InventoryItem& item2 = dynamic_cast<InventoryItem&>(*winMgr.createWindow("InventoryItem"));
    item2.setUserString("BlockImage", "TaharezLook/GenericBrush");
    item2.setContentSize(3, 1);
    receiver2.addItemAtLocation(item2, 1, 3);

    InventoryItem& item3 = dynamic_cast<InventoryItem&>(*winMgr.createWindow("InventoryItem"));
    item3.setUserString("BlockImage", "TaharezLook/GenericBrush");
    item3.setContentSize(1, 4);
    receiver2.addItemAtLocation(item3, 5, 2);

    InventoryItem& item4 = dynamic_cast<InventoryItem&>(*winMgr.createWindow("InventoryItem"));
    item4.setUserString("BlockImage", "TaharezLook/GenericBrush");
    item4.setContentSize(1, 1);
    receiver2.addItemAtLocation(item4, 8, 6);

    InventoryItem& item5 = dynamic_cast<InventoryItem&>(*winMgr.createWindow("InventoryItem"));
    item5.setUserString("BlockImage", "TaharezLook/GenericBrush");
    item5.setContentSize(2, 3);

    bool data[] = {
        1, 0,
        1, 1,
        1, 0
    };

    item5.setItemLayout(data);

    receiver2.addItemAtLocation(item5, 2, 5);

    return true;
}

//----------------------------------------------------------------------------//
void InventoryDemo::deinitialise()
{
    // nothing to do here!
}

//----------------------------------------------------------------------------//


/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static InventoryDemo sample;
    return sample;
}