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

namespace CEGUI
{

/*!
\brief
    This class is used as an item in the ListWidget. I
*/
class CEGUIEXPORT ListWidgetItem
{
public:
    ListWidgetItem();
    virtual ~ListWidgetItem();

    String getText() const { return d_text; }
    void setText(String val) { d_text = val; }

    bool operator== (const ListWidgetItem& other) const;
    bool operator< (const ListWidgetItem& other) const;

protected:
    String d_text;
};

/*!
\brief
    This is a convenience widget as an alternative to the new list view, for
    simple scenarios that don't require a custom ItemModel implementation.
*/
class CEGUIEXPORT ListWidget : public ListView, public ItemModel
{
public:
    //! Window factory name
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;

    ListWidget(const String& type, const String& name);
    virtual ~ListWidget();

    virtual void addItem(String text);
    virtual void addItem(const ListWidgetItem& item);

    virtual bool isValidIndex(const ModelIndex& model_index) const;
    virtual ModelIndex makeIndex(size_t child, const ModelIndex& parent_index);
    virtual bool areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const;
    virtual int compareIndices(const ModelIndex& index1, const ModelIndex& index2) const;
    virtual ModelIndex getParentIndex(const ModelIndex& model_index) const;
    virtual int getChildId(const ModelIndex& model_index) const;
    virtual ModelIndex getRootIndex() const;
    virtual size_t getChildCount(const ModelIndex& model_index) const;
    virtual String getData(const ModelIndex& model_index, ItemDataRole role = IDR_Text);

protected:
    virtual void initialiseComponents();

private:
    std::vector<ListWidgetItem> d_widgetItems;
};

}
#endif
