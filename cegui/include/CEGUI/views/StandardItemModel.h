/***********************************************************************
    created:    Wed May 23 2014
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
#ifndef _CEGUIStandardItemModel_h_
#define _CEGUIStandardItemModel_h_

#include "CEGUI/views/ItemModel.h"
#include <vector>

namespace CEGUI
{

class CEGUIEXPORT StandardItem
{
public:
    StandardItem();
    explicit StandardItem(String text, uint id = 0);
    virtual ~StandardItem();

    String getText() const { return d_text; }
    void setText(String val) { d_text = val; }

    //! Id of this item
    uint getId() const { return d_id; }
    void setId(uint val) { d_id = val; }

    bool operator== (const StandardItem& other) const;
    bool operator< (const StandardItem& other) const;

protected:
    String d_text;
    uint d_id;
};

class CEGUIEXPORT StandardItemModel : public ItemModel
{
public:
    virtual void addItem(String text);
    virtual void addItem(const StandardItem& item);

    virtual bool isValidIndex(const ModelIndex& model_index) const;
    virtual ModelIndex makeIndex(size_t child, const ModelIndex& parent_index);
    virtual bool areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const;
    virtual int compareIndices(const ModelIndex& index1, const ModelIndex& index2) const;
    virtual ModelIndex getParentIndex(const ModelIndex& model_index) const;
    virtual int getChildId(const ModelIndex& model_index) const;
    virtual ModelIndex getRootIndex() const;
    virtual size_t getChildCount(const ModelIndex& model_index) const;
    virtual String getData(const ModelIndex& model_index, ItemDataRole role = IDR_Text);

    inline virtual StandardItem* getItemForIndex(const ModelIndex& index) const;
protected:
    std::vector<StandardItem> d_items;
};

}

#endif
