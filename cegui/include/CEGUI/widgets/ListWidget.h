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

    void setItemSelectionState(size_t item_index, bool state);
    StandardItem* getFirstSelectedItem();
    StandardItem* getItemAtIndex(size_t index);
    size_t getItemCount();

    virtual StandardItemModel* getModel();

    void addItem(const String& text);
    void addItem(StandardItem* item);

protected:
    virtual void initialiseComponents();
    StandardItemModel d_itemModel;
};

}
#endif
