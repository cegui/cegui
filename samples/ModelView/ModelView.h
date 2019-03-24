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
#ifndef _Sample_Model_View_h_
#define _Sample_Model_View_h_

#include "Sample.h"
#include "InventoryModel.h"
#include "CEGUI/views/ListView.h"
#include "CEGUI/views/TreeView.h"
#include "CEGUI/widgets/ListWidget.h"

class ModelViewSample : public Sample
{
public:
    ModelViewSample();
    virtual ~ModelViewSample() {}

    bool initialise(CEGUI::GUIContext* gui_context) override;
    void deinitialise() override;

private:

    bool handleAddRandomItem(const CEGUI::EventArgs& e);
    bool handleAddItemInList(const CEGUI::EventArgs& e);
    bool handleAddItemInTree(const CEGUI::EventArgs& e);
    bool handleRemoveSelectedListItems(const CEGUI::EventArgs& e);
    bool handleRemoveSelectedTreeItems(const CEGUI::EventArgs& e);
    bool handleClearItems(const CEGUI::EventArgs& e);
    bool handleUpdateListItemName(const CEGUI::EventArgs& e);
    bool handleUpdateTreeItemName(const CEGUI::EventArgs& e);
    bool toggleMultiSelect(const CEGUI::EventArgs& e);
    bool toggleSorting(const CEGUI::EventArgs& e);

    void updateSelectedIndexText(CEGUI::ItemView* view, const CEGUI::String& text);
    void removeSelectedItemsFromView(CEGUI::ItemView* view);
    void initListWidgetItems(CEGUI::ListWidget* list_widget);
    CEGUI::Window* d_root;
    InventoryModel d_inventoryModel;

    CEGUI::ListView* d_listView;
    CEGUI::TreeView* d_treeView;
    int d_newItemsCount;
    CEGUI::Window* d_txtNewItemName;
};

#endif
