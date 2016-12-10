/***********************************************************************
    created:    Sun May 25 2014
    author:     Timotei Dolean
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#include "ModelView.h"
#include "CEGUI/CEGUI.h"

#include <iostream>
#include <sstream>

using namespace CEGUI;

ModelViewSample::ModelViewSample()
{
    d_name = "ModelViewSample";
    d_credits = "Timotei Dolean";
    d_summary = "A demo that shows the usage of the new Model-View widgets.";
    d_description = "The demo uses the WindowManager to create from code a window with a listview, treeview and GridView.";
}

/** This sample uses most of the code from the 'HelloWorld' sample.
    Thus, most of the clarifying comments have been removed for brevity. **/

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool ModelViewSample::initialise(GUIContext* gui_context)
{
    d_usedFiles = String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");

    gui_context->getCursor().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();
    d_root = win_mgr.loadLayoutFromFile("ModelViewSample.layout");

    ImageManager::getSingleton().loadImageset("DriveIcons.imageset");

    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();
    gui_context->setDefaultFont(defaultFont);
    gui_context->setRootWindow(d_root);
    gui_context->setDefaultTooltipType("TaharezLook/Tooltip");

    d_inventoryModel.load();
    d_newItemsCount = 0;

    d_listView = static_cast<ListView*>(win_mgr.createWindow("TaharezLook/ListView", "listView"));
    d_listView->setModel(&d_inventoryModel);
    d_listView->setItemTooltipsEnabled(true);
    d_listView->setSelectionColourRect(ColourRect(Colour(1.0f, 0, 0, 1)));
    d_root->getChild("ListViewHolder")->addChild(d_listView);

    d_treeView = static_cast<TreeView*>(win_mgr.createWindow("TaharezLook/TreeView", "treeView"));
    d_treeView->setModel(&d_inventoryModel);
    d_treeView->setItemTooltipsEnabled(true);
    d_root->getChild("TreeViewHolder")->addChild(d_treeView);

    ListWidget* list_widget = static_cast<ListWidget*>(win_mgr.createWindow("TaharezLook/ListWidget", "listWidget"));
    d_root->getChild("ListWidgetHolder")->addChild(list_widget);
    initListWidgetItems(list_widget);

    d_root->getChild("btnAddRandomItem")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleAddRandomItem, this));

    d_root->getChild("btnAddRandomItemInList")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleAddItemInList, this));

    d_root->getChild("btnAddRandomItemInTree")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleAddItemInTree, this));

    d_root->getChild("btnRemoveSelectedListItem")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleRemoveSelectedListItems, this));

    d_root->getChild("btnRemoveSelectedTreeItem")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleRemoveSelectedTreeItems, this));

    d_root->getChild("btnClearAllItems")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleClearItems, this));

    d_root->getChild("btnUpdateListItemName")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleUpdateListItemName, this));

    d_root->getChild("btnUpdateTreeItemName")->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::handleUpdateTreeItemName, this));

    d_root->getChild("chkMultiSelectEnabled")->subscribeEvent(
        ToggleButton::EventSelectStateChanged,
        Event::Subscriber(&ModelViewSample::toggleMultiSelect, this));

    d_root->getChild("btnSwitchSortingMode")->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&ModelViewSample::toggleSorting, this));

    d_txtNewItemName = d_root->getChild("txtNewItemName");

    return true;

}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void ModelViewSample::deinitialise()
{
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleClearItems(const EventArgs&)
{
    d_inventoryModel.clear();
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleRemoveSelectedListItems(const EventArgs&)
{
    removeSelectedItemsFromView(d_listView);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleRemoveSelectedTreeItems(const EventArgs&)
{
    removeSelectedItemsFromView(d_treeView);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleAddRandomItem(const EventArgs&)
{
    d_inventoryModel.addRandomItemWithChildren(d_inventoryModel.getRootIndex(), 0);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleAddItemInList(const EventArgs&)
{
    const std::vector<ModelIndexSelectionState>& selections = d_listView->getIndexSelectionStates();
    if (selections.empty())
        return false;

    const ModelIndexSelectionState& selection = (*selections.begin());

    d_inventoryModel.addRandomItemWithChildren(selection.d_parentIndex, selection.d_childId + 1);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleAddItemInTree(const EventArgs&)
{
    const std::vector<ModelIndexSelectionState>& selections = d_treeView->getIndexSelectionStates();
    if (selections.empty())
        return false;

    const ModelIndexSelectionState& selection = (*selections.begin());

    d_inventoryModel.addRandomItemWithChildren(selection.d_selectedIndex, 0);
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleUpdateListItemName(const EventArgs&)
{
    updateSelectedIndexText(d_listView, d_txtNewItemName->getText());
    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::handleUpdateTreeItemName(const EventArgs&)
{
    updateSelectedIndexText(d_treeView, d_txtNewItemName->getText());
    return true;
}

//----------------------------------------------------------------------------//
void ModelViewSample::removeSelectedItemsFromView(ItemView* view)
{
    while(!view->getIndexSelectionStates().empty())
    {
        d_inventoryModel.removeItem(view->getIndexSelectionStates().back().d_selectedIndex);
    }
}

//----------------------------------------------------------------------------//
void ModelViewSample::updateSelectedIndexText(ItemView* view, const String& text)
{
    const std::vector<ModelIndexSelectionState>& selections = view->getIndexSelectionStates();
    if (selections.empty())
        return;

    const ModelIndexSelectionState& selection = (*selections.begin());

    d_inventoryModel.updateItemName(selection.d_selectedIndex, text);
}

//----------------------------------------------------------------------------//
bool ModelViewSample::toggleMultiSelect(const EventArgs&)
{
    bool enabled = d_listView->isMultiSelectEnabled();

    d_listView->setMultiSelectEnabled(!enabled);
    d_treeView->setMultiSelectEnabled(!enabled);

    return true;
}

//----------------------------------------------------------------------------//
bool ModelViewSample::toggleSorting(const EventArgs&)
{
    Window* switch_button = d_root->getChild("btnSwitchSortingMode");
    int sortMode = static_cast<int>(d_listView->getSortMode());

    ViewSortMode next_sort_mode = static_cast<ViewSortMode>((sortMode + 1) % 3);

    d_listView->setSortMode(next_sort_mode);
    d_treeView->setSortMode(next_sort_mode);

    switch_button->setText(
        "Current sort mode: " + PropertyHelper<ViewSortMode>::toString(next_sort_mode));

    return true;
}

//----------------------------------------------------------------------------//
void ModelViewSample::initListWidgetItems(ListWidget* list_widget)
{
    ImageManager::ImageIterator itor = ImageManager::getSingleton().getIterator();
    while (!itor.isAtEnd())
    {
        list_widget->addItem(new StandardItem(itor.getCurrentKey(), itor.getCurrentKey()));
        ++itor;
    }
}
