/***********************************************************************
    created:    20/8/2005
    author:     Paul D Turner
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
#include "Sample_Demo6.h"
#include "CEGUI/CEGUI.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>

using namespace CEGUI;

/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool Demo6Sample::initialise(CEGUI::GUIContext* guiContext)
{
    d_usedFiles = CEGUI::String(__FILE__);

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");


    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // load an image to use as a background
    if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
        ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "root_wnd");
    // set position and size
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(USize(cegui_reldim(1), cegui_reldim(1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");
    // install this as the root GUI sheet
    guiContext->setRootWindow(background);

    // do demo stuff
    createDemoWindows(background);
    initDemoEventWiring(background);

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void Demo6Sample::deinitialise()
{
    // nothing to do here!
}

/*************************************************************************
    Create the windows and widgets for the demo
*************************************************************************/
void Demo6Sample::createDemoWindows(CEGUI::Window* root)
{
    using namespace CEGUI;
    ListboxTextItem* itm;

    WindowManager& winMgr = WindowManager::getSingleton();

    // create the main list.
    MultiColumnList* mcl = static_cast<MultiColumnList*>(winMgr.createWindow("TaharezLook/MultiColumnList", "MainList"));
    root->addChild(mcl);
    mcl->setPosition(UVector2(cegui_reldim(0.01f), cegui_reldim( 0.1f)));
    mcl->setSize(USize(cegui_reldim(0.5f), cegui_reldim( 0.8f)));

    // create frame window for control panel
    FrameWindow* fwnd = static_cast<FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "ControlPanel"));
    root->addChild(fwnd);
    fwnd->setPosition(UVector2(cegui_reldim(0.53f), cegui_reldim( 0.03f)));
    fwnd->setMaxSize(USize(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    fwnd->setSize(USize(cegui_reldim(0.44f), cegui_reldim( 0.94f)));
    fwnd->setText("Demo 6 - Control Panel");

    // create combo-box.
    Combobox* cbbo = static_cast<Combobox*>(winMgr.createWindow("TaharezLook/Combobox", "SelModeBox"));
    fwnd->addChild(cbbo);
    cbbo->setPosition(UVector2(cegui_reldim(0.04f), cegui_reldim( 0.06f)));
    cbbo->setSize(USize(cegui_reldim(0.66f), cegui_reldim( 0.33f)));
    //cbbo->setSortingEnabled(true);

    // populate combobox with possible selection modes
    const CEGUI::Image* sel_img = &ImageManager::getSingleton().get("TaharezLook/MultiListSelectionBrush");
    itm = new ListboxTextItem("Full Row (Single)", 0);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Full Row (Multiple)", 1);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Full Column (Single)", 2);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Full Column (Multiple)", 3);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Single Cell (Single)", 4);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Single Cell (Multiple)", 5);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Nominated Column (Single)", 6);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Nominated Column (Multiple)", 7);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    ListboxTextItem* pStore = itm;
    itm = new ListboxTextItem("Nominated Row (Single)", 8);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Nominated Row (Multiple)", 9);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    cbbo->setReadOnly(true);
    // Now change the text to test the sorting
    pStore->setText("Abracadabra");
    //cbbo->setSortingEnabled(false);
    cbbo->setSortingEnabled(true);
    //cbbo->handleUpdatedListItemData();

    // column control section
    Window* st = winMgr.createWindow("TaharezLook/StaticText", "ColumnPanel");
    fwnd->addChild(st);
    st->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.12f)));
    st->setSize(USize(cegui_reldim(0.96f), cegui_reldim( 0.25f)));
    st->setText("Column Control");
    st->setProperty("VertFormatting", "TopAligned");

    Window* label = winMgr.createWindow("TaharezLook/StaticText", "Label1");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("ID Code:");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label2");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.23f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Width:");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label3");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.44f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Caption:");

    PushButton* btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "AddColButton"));
    st->addChild(btn);
    btn->setPosition(UVector2(cegui_reldim(0.81f), cegui_reldim( 0.32f)));
    btn->setSize(USize(cegui_reldim(0.15f), cegui_reldim( 0.2f)));
    btn->setText("Add");

    Editbox* ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "NewColIDBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");
    ebox->setText("Test -- ");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "NewColWidthBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.23f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "NewColTextBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.44f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.36f), cegui_reldim( 0.2f)));
    ebox->setValidationString(".*");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label4");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.55f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("ID Code:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "DelColIDBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.67f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "DelColButton"));
    st->addChild(btn);
    btn->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.67f)));
    btn->setSize(USize(cegui_reldim(0.4f), cegui_reldim( 0.2f)));
    btn->setText("Delete Column");

    // Row control box
    st = winMgr.createWindow("TaharezLook/StaticText", "RowControl");
    fwnd->addChild(st);
    st->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.38f)));
    st->setSize(USize(cegui_reldim(0.96f), cegui_reldim( 0.25f)));
    st->setText("Row Control");
    st->setProperty("VertFormatting", "TopAligned");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label5");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Col ID:");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label6");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.23f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.55f), cegui_reldim( 0.12f)));
    label->setText("Item Text:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "RowColIDBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "RowTextBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.23f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.55f), cegui_reldim( 0.2f)));
    ebox->setValidationString(".*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "AddRowButton"));
    st->addChild(btn);
    btn->setPosition(UVector2(cegui_reldim(0.81f), cegui_reldim( 0.32f)));
    btn->setSize(USize(cegui_reldim(0.15f), cegui_reldim( 0.2f)));
    btn->setText("Add");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label7");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.55f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Row Idx:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "DelRowIdxBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.67f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "DelRowButton"));
    st->addChild(btn);
    btn->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.67f)));
    btn->setSize(USize(cegui_reldim(0.4f), cegui_reldim( 0.2f)));
    btn->setText("Delete Row");

    // set item box
    st = winMgr.createWindow("TaharezLook/StaticText", "SetItemPanel");
    fwnd->addChild(st);
    st->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.65f)));
    st->setSize(USize(cegui_reldim(0.96f), cegui_reldim( 0.25f)));
    st->setText("Item Modification");
    st->setProperty("VertFormatting", "TopAligned");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label8");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Row Idx:");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label9");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.23f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Col ID:");

    label = winMgr.createWindow("TaharezLook/StaticText", "Label10");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.44f), cegui_reldim( 0.2f)));
    label->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.12f)));
    label->setText("Item Text:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "SetItemRowBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "SetItemIDBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.23f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "SetItemTextBox"));
    st->addChild(ebox);
    ebox->setPosition(UVector2(cegui_reldim(0.44f), cegui_reldim( 0.32f)));
    ebox->setSize(USize(cegui_reldim(0.36f), cegui_reldim( 0.2f)));
    ebox->setValidationString(".*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "SetItemButton"));
    st->addChild(btn);
    btn->setPosition(UVector2(cegui_reldim(0.81f), cegui_reldim( 0.32f)));
    btn->setSize(USize(cegui_reldim(0.15f), cegui_reldim( 0.2f)));
    btn->setText("Set");

    label = winMgr.createWindow("TaharezLook/StaticText", "RowCount");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.55f)));
    label->setSize(USize(cegui_reldim(1.0f), cegui_reldim( 0.12f)));
    label->setText("Current Row Count:");

    label = winMgr.createWindow("TaharezLook/StaticText", "ColCount");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.67f)));
    label->setSize(USize(cegui_reldim(1.0f), cegui_reldim( 0.12f)));
    label->setText("Current Column Count:");

    label = winMgr.createWindow("TaharezLook/StaticText", "SelCount");
    st->addChild(label);
    label->setProperty("FrameEnabled", "false");
    label->setProperty("BackgroundEnabled", "false");
    label->setPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.79f)));
    label->setSize(USize(cegui_reldim(1.0f), cegui_reldim( 0.12f)));
    label->setText("Current Selected Count:");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "QuitButton"));
    fwnd->addChild(btn);
    btn->setPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.93f)));
    btn->setSize(USize(cegui_reldim(0.50f), cegui_reldim( 0.05f)));
    btn->setText("Quit This Demo!");
}


void Demo6Sample::initDemoEventWiring(CEGUI::Window* root)
{
    using namespace CEGUI;

    // subscribe handler that adds a new column
    root->getChild("ControlPanel/ColumnPanel/AddColButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleAddColumn, this));

    // subscribe handler that deletes a column
    root->getChild("ControlPanel/ColumnPanel/DelColButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleDeleteColumn, this));

    // subscribe handler that adds a new row
    root->getChild("ControlPanel/RowControl/AddRowButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleAddRow, this));

    // subscribe handler that deletes a row
    root->getChild("ControlPanel/RowControl/DelRowButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleDeleteRow, this));

    // subscribe handler that sets the text for an existing item
    root->getChild("ControlPanel/SetItemPanel/SetItemButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleSetItem, this));

    // subscribe handler that quits the application
   root->getChild("ControlPanel/QuitButton")->
       subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleQuit, this));

    // subscribe handler that processes a change in the 'selection mode' combobox
    root->getChild("ControlPanel/SelModeBox")->
        subscribeEvent(Combobox::EventListSelectionAccepted, Event::Subscriber(&Demo6Sample::handleSelectModeChanged, this));

    // subscribe handler that processes a change in the item(s) selected in the list
    root->getChild("MainList")->
        subscribeEvent(MultiColumnList::EventSelectionChanged, Event::Subscriber(&Demo6Sample::handleSelectChanged, this));

    // subscribe handler that processes a change in the list content.
    root->getChild("MainList")->
        subscribeEvent(MultiColumnList::EventListContentsChanged, Event::Subscriber(&Demo6Sample::handleContentsChanged, this));
}

bool Demo6Sample::handleQuit(const CEGUI::EventArgs&)
{

    // event was handled
    return true;
}

bool Demo6Sample::handleAddColumn(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the column to add
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    Editbox* idbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/ColumnPanel/NewColIDBox"));
    Editbox* widthbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/ColumnPanel/NewColWidthBox"));
    Editbox* textbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/ColumnPanel/NewColTextBox"));

    // get ID for new column
    CEGUI::uint id = atoi(idbox->getText().c_str());
    // get width to use for new column (in pixels)
    float width = static_cast<float>(atof(widthbox->getText().c_str()));
    // get column label text
    String text = textbox->getText();

    // re-set the widget contents
    idbox->setText("");
    widthbox->setText("");
    textbox->setText("");

    // ensure a minimum width of 10 pixels
    if (width < 10.0f)
        width = 10.0f;

    // finally, add the new column to the list.
    mcl->addColumn(text, id, cegui_absdim(width));

    // event was handled.
    return true;
}

bool Demo6Sample::handleDeleteColumn(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the column to delete
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    Editbox* idbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/ColumnPanel/DelColIDBox"));

    // obtain the id of the column to be deleted
    CEGUI::uint id = atoi(idbox->getText().c_str());

    // attempt to delete the column, ignoring any errors.
    CEGUI_TRY
    {
        mcl->removeColumnWithID(id);
    }
    CEGUI_CATCH (InvalidRequestException)
    {}

    // reset the delete column ID box.
    idbox->setText("");

    // event was handled.
    return true;
}

bool Demo6Sample::handleAddRow(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the row to add
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    Editbox* idbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/RowControl/RowColIDBox"));
    Editbox* textbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/RowControl/RowTextBox"));

    // get the ID of the initial column item to set
    CEGUI::uint id = atoi(idbox->getText().c_str());
    // get the text that is to be set initially into the specified column of the new row
    String text = textbox->getText();

    // reset input boxes
    idbox->setText("");
    textbox->setText("");

    // construct a new ListboxTextItem with the required string
    ListboxTextItem* item = new ListboxTextItem(text);
    // set the selection brush to use for this item.
    item->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");

    // attempt to add a new row, using the new ListboxTextItem as the initial content for one of the columns
    CEGUI_TRY
    {
        mcl->addRow(item, id);
    }
    // something went wrong, so cleanup the ListboxTextItem
    CEGUI_CATCH (InvalidRequestException)
    {
        delete item;
    }

    // event was handled.
    return true;
}

bool Demo6Sample::handleDeleteRow(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the row to delete.
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    Editbox* idxbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/RowControl/DelRowIdxBox"));

    // get index of row to delete.
    CEGUI::uint idx = atoi(idxbox->getText().c_str());

    // attempt to delete the row, ignoring any errors.
    CEGUI_TRY
    {
        mcl->removeRow(idx);
    }
    CEGUI_CATCH (InvalidRequestException)
    {}

    // clear the row index box
    idxbox->setText("");

    // event was handled.
    return true;
}

bool Demo6Sample::handleSetItem(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the item to be modified
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    Editbox* idbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SetItemPanel/SetItemIDBox"));
    Editbox* rowbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SetItemPanel/SetItemRowBox"));
    Editbox* textbox = static_cast<Editbox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SetItemPanel/SetItemTextBox"));

    // get ID of column to be affected
    CEGUI::uint id = atoi(idbox->getText().c_str());
    // get index of row to be affected
    CEGUI::uint row = atoi(rowbox->getText().c_str());
    // get new text for item
    String text = textbox->getText();

    // reset input boxes
    idbox->setText("");
    rowbox->setText("");
    textbox->setText("");

    // create a new ListboxTextItem using the new text string
    ListboxTextItem* item = new ListboxTextItem(text);
    // set the selection brush to be used for this item.
    item->setSelectionBrushImage("TaharezLook/MultiListSelectionBrush");

    // attempt to set the new item in place
    CEGUI_TRY
    {
        mcl->setItem(item, id, row);
    }
    // something went wrong, so cleanup the ListboxTextItem.
    CEGUI_CATCH (InvalidRequestException)
    {
        delete item;
    }

    // event was handled.
    return true;
}

bool Demo6Sample::handleSelectChanged(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // Get access to the list
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));

    // update the selected count
    std::string tmp("Current Selected Count: ");

    char buff[16];
    sprintf(buff, "%d", mcl->getSelectedCount());

    tmp += buff;

    static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SetItemPanel/SelCount")->setText(tmp.c_str());

    // event was handled.
    return true;
}

bool Demo6Sample::handleSelectModeChanged(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to list
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    // get access to the combobox
    Combobox* combo = static_cast<Combobox*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SelModeBox"));

    // find the selected item in the combobox
    ListboxItem* item = combo->findItemWithText(combo->getText(), 0);

    // set new selection mode according to ID of selected ListboxItem
    if (item)
    {
        switch (item->getID())
        {
        case 0:
            mcl->setSelectionMode(MultiColumnList::RowSingle);
            break;

        case 1:
            mcl->setSelectionMode(MultiColumnList::RowMultiple);
            break;

        case 2:
            mcl->setSelectionMode(MultiColumnList::ColumnSingle);
            break;

        case 3:
            mcl->setSelectionMode(MultiColumnList::ColumnMultiple);
            break;

        case 4:
            mcl->setSelectionMode(MultiColumnList::CellSingle);
            break;

        case 5:
            mcl->setSelectionMode(MultiColumnList::CellMultiple);
            break;

        case 6:
            mcl->setSelectionMode(MultiColumnList::NominatedColumnSingle);
            break;

        case 7:
            mcl->setSelectionMode(MultiColumnList::NominatedColumnMultiple);
            break;

        case 8:
            mcl->setSelectionMode(MultiColumnList::NominatedRowSingle);
            break;

        case 9:
            mcl->setSelectionMode(MultiColumnList::NominatedRowMultiple);
            break;

        default:
            mcl->setSelectionMode(MultiColumnList::RowSingle);
            break;

        }
    }

    // event was handled.
    return true;
}

bool Demo6Sample::handleContentsChanged(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get access to required widgets
    MultiColumnList* mcl = static_cast<MultiColumnList*>(static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("MainList"));
    Window* colText = static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SetItemPanel/ColCount");
    Window* rowText = static_cast<const WindowEventArgs&>(args).window->getRootWindow()->getChild("ControlPanel/SetItemPanel/RowCount");

    std::string tmp;
    char buff[16];

    // update the column count
    tmp = "Current Column Count: ";
    sprintf(buff, "%d", mcl->getColumnCount());
    tmp += buff;
    colText->setText(tmp.c_str());

    // update the row count
    tmp = "Current Row Count: ";
    sprintf(buff, "%d", mcl->getRowCount());
    tmp += buff;
    rowText->setText(tmp.c_str());

    // event was handled.
    return true;
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static Demo6Sample sample;
    return sample;
}