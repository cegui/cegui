/***********************************************************************
    created:    Sat Aug 09 2014
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
#ifndef _CEGUIAbstractItemModel_h_
#define _CEGUIAbstractItemModel_h_

#include "CEGUI/views/ItemModel.h"
#include "CEGUI/Exceptions.h"
#include <vector>

namespace CEGUI
{

class CEGUIEXPORT AbstractItem
{
public:
    AbstractItem();
    explicit AbstractItem(const String& text);
    AbstractItem(const String& text, const String& icon);
    virtual ~AbstractItem();

    String getText() const { return d_text; }
    void setText(const String& val) { d_text = val; }

    String getIcon() const { return d_icon; }
    void setIcon(const String& icon) { d_icon = icon; }

    AbstractItem* getParent() const { return d_parent; }
    void setParent(AbstractItem* item) { d_parent = item; }

    std::vector<AbstractItem*>& getChildren() { return d_children; }

    virtual void addItem(AbstractItem* child);

    virtual bool operator== (const AbstractItem& other) const;
    virtual bool operator!= (const AbstractItem& other) const;
    virtual bool operator< (const AbstractItem& other) const;

protected:
    String d_text;
    String d_icon;

    std::vector<AbstractItem*> d_children;
    AbstractItem* d_parent;
};

/*!
\brief
    An abstract implementation of the ItemModel that works with
    a tree structure using items of class that inherits from AbstractItem.

    Users of this class can either create a template instantiation of this
    implementation, or inherit it and augment with custom operations or
    overwrite certain methods (e.g.: getData)

    One such example is the following, where we instantiate a model for our
    own MyItem type:
    \code{.cpp}
    class MyItem : public AbstractItem
    {
    };

    typedef AbstractItemModel<MyItem> MyItemModel;
    \endcode

\remark
    Items added to the model are taken over and managed by the model, which
    means they are deleted by it. CEGUI_DELETE_AO is used for that, so
    CEGUI_NEW_AO should be used for already created objects that are added.

\tparam TAbstractItem
    The type of the item this model operates on. The type \b needs to inherit
    directly or indirectly from AbstractItem.
*/
template <typename TAbstractItem>
class AbstractItemModel : public ItemModel
{
public:
    /*!
    \brief
        Creates a new model instance using the specified \a root item, which
        must not be NULL - otherwise an InvalidRequestException is thrown.

        The \a root will be taken ownership of, and cleaned when the model is
        destructed (using CEGUI_DELETE_AO).
    */
    explicit AbstractItemModel(TAbstractItem* root);
    virtual ~AbstractItemModel();

    /*!
    \brief
        Adds a new item in the model with the specified text, taking ownership
        of it.

        If this method is used, the TAbstractItem type should have a string
        constructor in order to successfully compile.
    */
    virtual void addItem(String text);
    //! Adds the item as root child and takes ownership of it.
    virtual void addItem(AbstractItem* item);
    //! Adds the item as root child at the specified position
    virtual void addItemAtPosition(AbstractItem* item, size_t pos);
    //! Adds the item as child of the specified parent, at the specified position
    virtual void addItemAtPosition(AbstractItem* item, const ModelIndex& parent,
        size_t position);

    /*!
    \brief
        Inserts the specified item before the specified position item. If position
        is NULL the new item will be added at the beginning.
    */
    virtual void insertItem(AbstractItem* item, const AbstractItem* position);
    virtual void removeItem(const AbstractItem* item);
    virtual void removeItem(const ModelIndex& index);

    /*!
    \brief
        Clears the items from this ItemModel, deleting them.

    \param notify
        If true, it will raise the EventChildrenRemoved event for each deleted
        item.
    */
    void clear(bool notify = true);

    inline virtual TAbstractItem* getItemForIndex(const ModelIndex& index) const;
    virtual int getChildId(const AbstractItem* item) const;
    virtual ModelIndex getIndexForItem(const AbstractItem* item) const;

    /*!
    \brief
        This represents the conceptual root of this model. This root will be
        ignored in normal operations. It's merely used in order to provide
        an access point to the items tree.
    */
    TAbstractItem& getRoot() { return static_cast<TAbstractItem&>(*d_root); }

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
    //! Deletes all children of the specified item, optionally invoking the
    //! EventChildrenRemoved event
    void deleteChildren(AbstractItem* item, bool notify);

    //! Makes a valid index if \a id is withing \a vector's bounds.
    template <typename T>
    ModelIndex makeValidIndex(size_t id, std::vector<T>& vector);

    AbstractItem* d_root;
};

//----------------------------------------------------------------------------//
// Implementation of the AbstractItemModel
//----------------------------------------------------------------------------//
template <typename TAbstractItem>
AbstractItemModel<TAbstractItem>::AbstractItemModel(TAbstractItem* root) :
d_root(root)
{
    if (root == 0)
        CEGUI_THROW(InvalidRequestException("Root cannot be null"));
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
template <typename T>
ModelIndex AbstractItemModel<TAbstractItem>::makeValidIndex(size_t id,
    std::vector<T>& vector)
{
    if (id >= 0 && id < vector.size())
        return ModelIndex(vector.at(id));

    return ModelIndex();
}

template <typename TAbstractItem>
AbstractItemModel<TAbstractItem>::~AbstractItemModel()
{
    clear(false);

    CEGUI_DELETE_AO d_root;
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
bool AbstractItemModel<TAbstractItem>::isValidIndex(const ModelIndex& model_index) const
{
    return model_index.d_modelData != 0;
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
ModelIndex AbstractItemModel<TAbstractItem>::makeIndex(size_t child,
    const ModelIndex& parent_index)
{
    if (parent_index.d_modelData == 0)
        return ModelIndex();

    AbstractItem* item = static_cast<AbstractItem*>(parent_index.d_modelData);
    return makeValidIndex(child, item->getChildren());
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
bool AbstractItemModel<TAbstractItem>::areIndicesEqual(const ModelIndex& index1,
    const ModelIndex& index2) const
{
    return compareIndices(index1, index2) == 0;
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
int AbstractItemModel<TAbstractItem>::compareIndices(const ModelIndex& index1,
    const ModelIndex& index2) const
{
    if (!isValidIndex(index1) || !isValidIndex(index2) ||
        index1.d_modelData == index2.d_modelData)
        return 0;

    if (*getItemForIndex(index1) < *getItemForIndex(index2))
        return -1;

    return *getItemForIndex(index1) == *getItemForIndex(index2) ? 0 : 1;
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
ModelIndex AbstractItemModel<TAbstractItem>::getParentIndex(const ModelIndex& model_index) const
{
    if (model_index.d_modelData == d_root)
        return ModelIndex();

    AbstractItem* item = static_cast<AbstractItem*>(model_index.d_modelData);
    if (item->getParent() == 0)
        return getRootIndex();

    return ModelIndex(item->getParent());
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
int AbstractItemModel<TAbstractItem>::getChildId(const ModelIndex& model_index) const
{
    return getChildId(getItemForIndex(model_index));
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
int AbstractItemModel<TAbstractItem>::getChildId(const AbstractItem* item) const
{
    if (item == 0 || item->getParent() == 0)
        return -1;

    AbstractItem* parent_item = item->getParent();

    std::vector<AbstractItem*>::iterator itor = std::find(
        parent_item->getChildren().begin(), parent_item->getChildren().end(), item);

    if (itor == parent_item->getChildren().end())
        return -1;

    return std::distance(parent_item->getChildren().begin(), itor);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
ModelIndex AbstractItemModel<TAbstractItem>::getRootIndex() const
{
    return ModelIndex(d_root);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
size_t AbstractItemModel<TAbstractItem>::getChildCount(const ModelIndex& model_index) const
{
    if (model_index.d_modelData == 0)
        return d_root->getChildren().size();

    return static_cast<AbstractItem*>(model_index.d_modelData)->getChildren().size();
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
String AbstractItemModel<TAbstractItem>::getData(const ModelIndex& model_index,
    ItemDataRole role /*= IDR_Text*/)
{
    if (!isValidIndex(model_index))
        return "";

    AbstractItem* item = static_cast<AbstractItem*>(model_index.d_modelData);
    if (role == IDR_Text) return item->getText();
    if (role == IDR_Icon) return item->getIcon();
    if (role == IDR_Tooltip) return "Tooltip for " + item->getText();

    return "";
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::addItem(String text)
{
    addItem(CEGUI_NEW_AO TAbstractItem(text));
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::addItem(AbstractItem* item)
{
    if (item == 0)
        CEGUI_THROW(InvalidRequestException("Cannot add a NULL item to the model!"));

    addItemAtPosition(item,
        d_root->getChildren().empty() ? 0 : d_root->getChildren().size());
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::addItemAtPosition(AbstractItem* item, size_t pos)
{
    addItemAtPosition(item, getRootIndex(), pos);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::addItemAtPosition(AbstractItem* new_item,
    const ModelIndex& parent_index, size_t position)
{
    AbstractItem* parent = static_cast<AbstractItem*>(parent_index.d_modelData);
    if (position > parent->getChildren().size())
        CEGUI_THROW(InvalidRequestException("The specified position is out of range."));

    new_item->setParent(parent);
    parent->getChildren().insert(parent->getChildren().begin() + position, new_item);

    notifyChildrenAdded(parent_index, position, 1);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::insertItem(AbstractItem* item,
    const AbstractItem* position)
{
    int child_id = position == 0 ? -1 : getChildId(position);

    addItemAtPosition(item, child_id == -1 ? 0 : static_cast<size_t>(child_id));
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::removeItem(const ModelIndex& index)
{
    removeItem(getItemForIndex(index));
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::removeItem(const AbstractItem* item)
{
    AbstractItem* parent_item = item->getParent();

    std::vector<AbstractItem*>::iterator itor = std::find(
        parent_item->getChildren().begin(), parent_item->getChildren().end(),
        item);

    if (itor != parent_item->getChildren().end())
    {
        size_t child_id = std::distance(parent_item->getChildren().begin(), itor);

        deleteChildren(*itor, true);
        CEGUI_DELETE_AO *itor;
        parent_item->getChildren().erase(itor);

        notifyChildrenRemoved(ModelIndex(parent_item), child_id, 1);
    }
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
TAbstractItem* AbstractItemModel<TAbstractItem>::getItemForIndex(
    const ModelIndex& index) const
{
    if (index.d_modelData == 0)
        return 0;

    return static_cast<TAbstractItem*>(index.d_modelData);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void AbstractItemModel<TAbstractItem>::clear(bool notify /*= true */)
{
    deleteChildren(d_root, notify);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
ModelIndex AbstractItemModel<TAbstractItem>::getIndexForItem(
    const AbstractItem* item) const
{
    // TODO: this is annoying. We should be able to just hand out the ModelIndex
    // Right now we can't, since we're in a const method, operating on a const item
    // In hindsight, the index we hand out will be modifiable so maybe we need
    // to change the const-ness of the item parameter.
    AbstractItem* parent = item->getParent();
    int child_id = getChildId(item);
    return ModelIndex(child_id != -1 ? parent->getChildren().at(child_id) : 0);
}

//----------------------------------------------------------------------------//
template <typename TAbstractItem>
void CEGUI::AbstractItemModel<TAbstractItem>::deleteChildren(AbstractItem* item,
    bool notify)
{
    if (item == 0)
        CEGUI_THROW(InvalidRequestException("Cannot delete children of a NULL item!"));

    size_t items_count = item->getChildren().size();
    std::vector<AbstractItem*>::iterator itor = item->getChildren().begin();

    while (itor != item->getChildren().end())
    {
        deleteChildren(*itor, notify);
        CEGUI_DELETE_AO *itor;
        itor = item->getChildren().erase(itor);
    }

    if (notify)
    {
        notifyChildrenRemoved(ModelIndex(item), 0, items_count);
    }
}
}

#endif
