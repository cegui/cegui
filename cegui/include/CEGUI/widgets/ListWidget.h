/***********************************************************************
    created:    Sat Jul 19 2014
    author:     Timotei Dolean <timotei21@gmail.com>
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
#ifndef _CEGUIListWidget_h_
#define _CEGUIListWidget_h_

#include "CEGUI/views/ListView.h"
#include "CEGUI/views/StandardItemModel.h"

namespace CEGUI
{

/*!
\brief
    This is a convenience widget as an alternative to the new list view, for
    simple scenarios that don't require a custom ItemModel implementation.

    Basically, what this class does, is to use a StandardItemModel as the source
    ItemModel, and provide some quick-access methods directly to the items,
    instead of going through the ModelIndex. Most of the ListView functions
    have the item-typed variants/overloads here. It also provides size_t-based
    index overloads, since this is a list with only one level of nestedness.

    This a direct alternative for the old ListBox or ItemListbox widgets.
*/
class CEGUIEXPORT ListWidget : public ListView
{
public:
    //! Window factory name
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;

    ListWidget(const String& type, const String& name);
    virtual ~ListWidget();

    void setIndexSelectionState(size_t item_index, bool state);
    void setIndexSelectionState(StandardItem* item, bool state);

    /*!
    \brief
        Gets the ordinal first item. If the ListWidget has multi selection enabled
        and multiple items are selected, the first one will be returned.
    */
    StandardItem* getFirstSelectedItem();

    /*!
    \brief
        Returns the next selected item after the specified \a start_item

    \param start_item
        The item to start the search after. If this is NULL the search will
        begin at the beginning of the list.
    */
    StandardItem* getNextSelectedItem(const StandardItem* start_item);
    StandardItem* getItemAtIndex(size_t index);

    /*!
    \brief
        Returns a StandardItem that has the specified \a text, the searching
        procedure starting from \a start_item

    \param start_item
        If \a start_item is NULL, the search will start from the first item
        of the list.
    */
    StandardItem* findItemWithText(const String& text, const StandardItem* start_item);

    bool isItemInList(const StandardItem* item);
    bool isItemSelected(const StandardItem* item);
    bool isIndexSelected(size_t index);

    size_t getItemCount() const;
    size_t getSelectedItemsCount() const;

    virtual StandardItemModel* getModel();

    void addItem(const String& text);
    void addItem(StandardItem* item);
    void insertItem(StandardItem* item, const StandardItem* position);

    void removeItem(const StandardItem* item);
    //! Clears the items in this list and deletes all associated items.
    void clearList();

    virtual void ensureIndexIsVisible(const StandardItem* item);

protected:
    void initialiseComponents() override;

    StandardItemModel d_itemModel;
};

}
#endif
