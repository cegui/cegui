/************************************************************************
    filename:   Sample_Demo6.cpp
    created:    20/8/2005
    author:     Paul D Turner
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
#include "Sample_Demo6.h"
#include "CEGUI.h"
#include "CEGuiBaseApplication.h"

#include <cstdlib>
#include <cstdio>
#include <string>

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    Demo6Sample app;
    return app.run();
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool Demo6Sample::initialiseSample()
{
    using namespace CEGUI;

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().loadScheme("../datafiles/schemes/TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    FontManager::getSingleton().createFont("../datafiles/fonts/Commonwealth-10.font");

    // load an image to use as a background
    ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "../datafiles/imagesets/GPN-2000-001437.tga");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    StaticImage* background = static_cast<StaticImage*>(winMgr.createWindow("TaharezLook/StaticImage", "root_wnd"));
    // set position and size
    background->setPosition(Point(0, 0));
    background->setSize(Size(1, 1));
    // disable frame and standard background
    background->setFrameEnabled(false);
    background->setBackgroundEnabled(false);
    // set the background image
    background->setImage("BackgroundImage", "full_image");
    // install this as the root GUI sheet
    System::getSingleton().setGUISheet(background);

    // do demo stuff
    createDemoWindows();
    initDemoEventWiring();

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void Demo6Sample::cleanupSample()
{
    // nothing to do here!
}

/*************************************************************************
    Create the windows and widgets for the demo
*************************************************************************/
void Demo6Sample::createDemoWindows(void)
{
    using namespace CEGUI;
    ListboxTextItem* itm;

    WindowManager& winMgr = WindowManager::getSingleton();
    Window* root = winMgr.getWindow("root_wnd");

    // create the main list.
    MultiColumnList* mcl = static_cast<MultiColumnList*>(winMgr.createWindow("TaharezLook/MultiColumnList", "Demo6/MainList"));
    root->addChildWindow(mcl);
    mcl->setPosition(Point(0.01f, 0.1f));
    mcl->setSize(Size(0.5f, 0.8f));

    // create frame window for control panel
    FrameWindow* fwnd = static_cast<FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "Demo6/ControlPanel"));
    root->addChildWindow(fwnd);
    fwnd->setPosition(Point(0.53, 0.03f));
    fwnd->setMaximumSize(Size(1.0f, 1.0f));
    fwnd->setSize(Size(0.44f, 0.94f));
    fwnd->setText("Demo 6 - Control Panel");

    // create combo-box.
    Combobox* cbbo = static_cast<Combobox*>(winMgr.createWindow("TaharezLook/Combobox", "Demo6/ControlPanel/SelModeBox"));
    fwnd->addChildWindow(cbbo);
    cbbo->setPosition(Point(0.04f, 0.06f));
    cbbo->setSize(Size(0.66f, 0.33f));

    // populate combobox with possible selection modes
    const CEGUI::Image* sel_img = &ImagesetManager::getSingleton().getImageset("TaharezLook")->getImage("MultiListSelectionBrush");
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
    itm = new ListboxTextItem("Nominated Row (Single)", 8);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    itm = new ListboxTextItem("Nominated Row (Multiple)", 9);
    itm->setSelectionBrushImage(sel_img);
    cbbo->addItem(itm);
    cbbo->setReadOnly(true);

    // column control section
    StaticText* st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/ColumnPanel"));
    fwnd->addChildWindow(st);
    st->setPosition(Point(0.02f, 0.12f));
    st->setSize(Size(0.96f, 0.25f));
    st->setText("Column Control");
    st->setVerticalFormatting(StaticText::TopAligned);

    StaticText* label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label1"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("ID Code:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label2"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.23f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Width:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label3"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.44f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Caption:");

    PushButton* btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo6/ControlPanel/ColumnPanel/AddColButton"));
    st->addChildWindow(btn);
    btn->setPosition(Point(0.81f, 0.32f));
    btn->setSize(Size(0.15f, 0.2f));
    btn->setText("Add");

    Editbox* ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/NewColIDBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.02f, 0.32f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/NewColWidthBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.23f, 0.32f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/NewColTextBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.44f, 0.32f));
    ebox->setSize(Size(0.36f, 0.2f));
    ebox->setValidationString(".*");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label4"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.55f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("ID Code:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/DelColIDBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.02f, 0.67f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo6/ControlPanel/ColumnPanel/DelColButton"));
    st->addChildWindow(btn);
    btn->setPosition(Point(0.25f, 0.67f));
    btn->setSize(Size(0.4f, 0.2f));
    btn->setText("Delete Column");

    // Row control box
    st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/RowControl"));
    fwnd->addChildWindow(st);
    st->setPosition(Point(0.02f, 0.38f));
    st->setSize(Size(0.96f, 0.25f));
    st->setText("Row Control");
    st->setVerticalFormatting(StaticText::TopAligned);

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label5"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Col ID:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label6"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.23f, 0.2f));
    label->setSize(Size(0.55f, 0.12f));
    label->setText("Item Text:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/RowColIDBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.02f, 0.32f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/RowTextBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.23f, 0.32f));
    ebox->setSize(Size(0.55f, 0.2f));
    ebox->setValidationString(".*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo6/ControlPanel/ColumnPanel/AddRowButton"));
    st->addChildWindow(btn);
    btn->setPosition(Point(0.81f, 0.32f));
    btn->setSize(Size(0.15f, 0.2f));
    btn->setText("Add");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label7"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.55f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Row Idx:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/DelRowIdxBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.02f, 0.67f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo6/ControlPanel/ColumnPanel/DelRowButton"));
    st->addChildWindow(btn);
    btn->setPosition(Point(0.25f, 0.67f));
    btn->setSize(Size(0.4f, 0.2f));
    btn->setText("Delete Row");

    // set item box
    st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/SetItemPanel"));
    fwnd->addChildWindow(st);
    st->setPosition(Point(0.02f, 0.65f));
    st->setSize(Size(0.96f, 0.25f));
    st->setText("Item Modification");
    st->setVerticalFormatting(StaticText::TopAligned);

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label8"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Row Idx:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label9"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.23f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Col ID:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/Label10"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.44f, 0.2f));
    label->setSize(Size(0.2f, 0.12f));
    label->setText("Item Text:");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/SetItemRowBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.02f, 0.32f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/SetItemIDBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.23f, 0.32f));
    ebox->setSize(Size(0.2f, 0.2f));
    ebox->setValidationString("\\d*");

    ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo6/ControlPanel/ColumnPanel/SetItemTextBox"));
    st->addChildWindow(ebox);
    ebox->setPosition(Point(0.44f, 0.32f));
    ebox->setSize(Size(0.36f, 0.2f));
    ebox->setValidationString(".*");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo6/ControlPanel/ColumnPanel/SetItemButton"));
    st->addChildWindow(btn);
    btn->setPosition(Point(0.81f, 0.32f));
    btn->setSize(Size(0.15f, 0.2f));
    btn->setText("Set");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/RowCount"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.55f));
    label->setSize(Size(1.0f, 0.12f));
    label->setText("Current Row Count:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/ColCount"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.67f));
    label->setSize(Size(1.0f, 0.12f));
    label->setText("Current Column Count:");

    label = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "Demo6/ControlPanel/SelCount"));
    st->addChildWindow(label);
    label->setFrameEnabled(false);
    label->setBackgroundEnabled(false);
    label->setPosition(Point(0.02f, 0.79f));
    label->setSize(Size(1.0f, 0.12f));
    label->setText("Current Selected Count:");

    btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo6/QuitButton"));
    fwnd->addChildWindow(btn);
    btn->setPosition(Point(0.25f, 0.93f));
    btn->setSize(Size(0.50f, 0.05f));
    btn->setText("Quit This Demo!");
}


void Demo6Sample::initDemoEventWiring(void)
{
    using namespace CEGUI;

    WindowManager& winMgr = WindowManager::getSingleton();

    // subscribe handler that adds a new column
    winMgr.getWindow("Demo6/ControlPanel/ColumnPanel/AddColButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleAddColumn, this));

    // subscribe handler that deletes a column
    winMgr.getWindow("Demo6/ControlPanel/ColumnPanel/DelColButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleDeleteColumn, this));

    // subscribe handler that adds a new row
    winMgr.getWindow("Demo6/ControlPanel/ColumnPanel/AddRowButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleAddRow, this));

    // subscribe handler that deletes a row
    winMgr.getWindow("Demo6/ControlPanel/ColumnPanel/DelRowButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleDeleteRow, this));

    // subscribe handler that sets the text for an existing item
    winMgr.getWindow("Demo6/ControlPanel/ColumnPanel/SetItemButton")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleSetItem, this));

    // subscribe handler that quits the application
   winMgr.getWindow("Demo6/QuitButton")->
       subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo6Sample::handleQuit, this));

    // subscribe handler that processes a change in the 'selection mode' combobox
    winMgr.getWindow("Demo6/ControlPanel/SelModeBox")->
        subscribeEvent(Combobox::EventListSelectionAccepted, Event::Subscriber(&Demo6Sample::handleSelectModeChanged, this));

    // subscribe handler that processes a change in the item(s) selected in the list
    winMgr.getWindow("Demo6/MainList")->
        subscribeEvent(MultiColumnList::EventSelectionChanged, Event::Subscriber(&Demo6Sample::handleSelectChanged, this));

    // subscribe handler that processes a change in the list content.
    winMgr.getWindow("Demo6/MainList")->
        subscribeEvent(MultiColumnList::EventListContentsChanged, Event::Subscriber(&Demo6Sample::handleContentsChanged, this));
}

bool Demo6Sample::handleQuit(const CEGUI::EventArgs& e)
{
    // signal quit
    d_sampleApp->setQuitting();

    // event was handled
    return true;
}

bool Demo6Sample::handleAddColumn(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the column to add
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    Editbox* idbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/NewColIDBox"));
    Editbox* widthbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/NewColWidthBox"));
    Editbox* textbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/NewColTextBox"));

    // get ID for new column
    CEGUI::uint id = std::atoi(idbox->getText().c_str());
    // get width to use for new column (in pixels)
    float width = std::atof(widthbox->getText().c_str());
    // get column label text
    String text = textbox->getText();

    // re-set the widget contents
    idbox->setText("");
    widthbox->setText("");
    textbox->setText("");

    // ensure a minimum width of 10 pixels
    if (width < 10.0f)
        width = 10.0f;

    // convert our pixel width into a relative value.
    width = mcl->absoluteToRelativeX(width);

    // finally, add the new column to the list.
    mcl->addColumn(text, id, width);

    // event was handled.
    return true;
}

bool Demo6Sample::handleDeleteColumn(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the column to delete
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    Editbox* idbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/DelColIDBox"));

    // obtain the id of the column to be deleted
    CEGUI::uint id = std::atoi(idbox->getText().c_str());

    // attempt to delete the column, ignoring any errors.
    try
    {
        mcl->removeColumnWithID(id);
    }
    catch (InvalidRequestException)
    {}

    // reset the delete column ID box.
    idbox->setText("");

    // event was handled.
    return true;
}

bool Demo6Sample::handleAddRow(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the row to add
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    Editbox* idbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/RowColIDBox"));
    Editbox* textbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/RowTextBox"));

    // get the ID of the initial column item to set
    CEGUI::uint id = std::atoi(idbox->getText().c_str());
    // get the text that is to be set initially into the specified column of the new row
    String text = textbox->getText();

    // reset input boxes
    idbox->setText("");
    textbox->setText("");

    // construct a new ListboxTextItem with the required string
    ListboxTextItem* item = new ListboxTextItem(text);
    // set the selection brush to use for this item.
    item->setSelectionBrushImage(&ImagesetManager::getSingleton().getImageset("TaharezLook")->getImage("MultiListSelectionBrush"));

    // attempt to add a new row, using the new ListboxTextItem as the initial content for one of the columns
    try
    {
        mcl->addRow(item, id);
    }
    // something went wrong, so cleanup the ListboxTextItem
    catch (InvalidRequestException)
    {
        delete item;
    }

    // event was handled.
    return true;
}

bool Demo6Sample::handleDeleteRow(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the row to delete.
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    Editbox* idxbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/DelRowIdxBox"));

    // get index of row to delete.
    CEGUI::uint idx = std::atoi(idxbox->getText().c_str());

    // attempt to delete the row, ignoring any errors.
    try
    {
        mcl->removeRow(idx);
    }
    catch (InvalidRequestException)
    {}

    // clear the row index box
    idxbox->setText("");

    // event was handled.
    return true;
}

bool Demo6Sample::handleSetItem(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to the widgets that contain details about the item to be modified
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    Editbox* idbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/SetItemIDBox"));
    Editbox* rowbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/SetItemRowBox"));
    Editbox* textbox = static_cast<Editbox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColumnPanel/SetItemTextBox"));

    // get ID of column to be affected
    CEGUI::uint id = std::atoi(idbox->getText().c_str());
    // get index of row to be affected
    CEGUI::uint row = std::atoi(rowbox->getText().c_str());
    // get new text for item
    String text = textbox->getText();

    // reset input boxes
    idbox->setText("");
    rowbox->setText("");
    textbox->setText("");

    // create a new ListboxTextItem using the new text string
    ListboxTextItem* item = new ListboxTextItem(text);
    // set the selection brush to be used for this item.
    item->setSelectionBrushImage(&ImagesetManager::getSingleton().getImageset("TaharezLook")->getImage("MultiListSelectionBrush"));

    // attempt to set the new item in place
    try
    {
        mcl->setItem(item, id, row);
    }
    // something went wrong, so cleanup the ListboxTextItem.
    catch (InvalidRequestException)
    {
        delete item;
    }

    // event was handled.
    return true;
}

bool Demo6Sample::handleSelectChanged(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // Get access to the list
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));

    // update the selected count
    std::string tmp("Current Selected Count: ");

    char buff[16];
    std::sprintf(buff, "%d", mcl->getSelectedCount());

    tmp += buff;

    WindowManager::getSingleton().getWindow("Demo6/ControlPanel/SelCount")->setText(tmp);

    // event was handled.
    return true;
}

bool Demo6Sample::handleSelectModeChanged(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to list
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    // get access to the combobox
    Combobox* combo = static_cast<Combobox*>(WindowManager::getSingleton().getWindow("Demo6/ControlPanel/SelModeBox"));

    // find the selected item in the combobox
    ListboxItem* item = combo->findItemWithText(combo->getText(), NULL);

    // set new selection mode according to ID of selected ListboxItem
    if (item != NULL)
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

bool Demo6Sample::handleContentsChanged(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get access to required widgets
    MultiColumnList* mcl = static_cast<MultiColumnList*>(WindowManager::getSingleton().getWindow("Demo6/MainList"));
    Window* colText = WindowManager::getSingleton().getWindow("Demo6/ControlPanel/ColCount");
    Window* rowText = WindowManager::getSingleton().getWindow("Demo6/ControlPanel/RowCount");

    std::string tmp;
    char buff[16];

    // update the column count
    tmp = "Current Column Count: ";
    std::sprintf(buff, "%d", mcl->getColumnCount());
    tmp += buff;
    colText->setText(tmp);

    // update the row count
    tmp = "Current Row Count: ";
    std::sprintf(buff, "%d", mcl->getRowCount());
    tmp += buff;
    rowText->setText(tmp);

    // event was handled.
    return true;
}
