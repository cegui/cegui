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
#include <algorithm>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    Base class for the items used by GenericItemModel. The item has a list
    of children attached to it, thus being able to represent a tree hierarchy.

    It's important to understand that this is a simple class that only holds
    structured data. Direct changes to this item won't be propagated to the
    ItemModel it's part of, unless one links them manually. In order to modify
    this item and update the ItemModel (and consequently the view) you need to
    call ItemModel implementation's functions (e.g.:
    GenericItemModel::removeItem(const GenericItem*)).
*/
class CEGUIEXPORT GenericItem
{
public:
    GenericItem();
    explicit GenericItem(const String& text);
    GenericItem(const String& text, const String& icon);
    virtual ~GenericItem();

    String getText() const { return d_text; }
    void setText(const String& val) { d_text = val; }

    virtual String getTooltipText() const { return {}; }

    String getIcon() const { return d_icon; }
    void setIcon(const String& icon) { d_icon = icon; }

    GenericItem* getParent() const { return d_parent; }
    void setParent(GenericItem* item) { d_parent = item; }

    std::vector<GenericItem*>& getChildren() { return d_children; }

    /*!
    \brief
        Adds a child item to the item.

    \remark
        This method <b>does not</b> notify anyone of the child that was just
        added. If you want the notifications, you can use one of the following
        methods:
        - GenericItemModel::addItemAtPosition(GenericItem*, size_t)
        - GenericItemModel::addItemAtPosition(GenericItem*, const ModelIndex&, size_t)
        - GenericItemModel::insertItem(GenericItem*, const GenericItem*)

    */
    virtual void addItem(GenericItem* child);

    virtual bool operator== (const GenericItem& other) const;
    virtual bool operator!= (const GenericItem& other) const;
    virtual bool operator< (const GenericItem& other) const;

protected:
    String d_text;
    String d_icon;

    std::vector<GenericItem*> d_children;
    GenericItem* d_parent;
};

/*!
\brief
    An abstract implementation of the ItemModel that works with
    a tree structure using items of class that inherits from GenericItem.

    Users of this class can either create a template instantiation of this
    implementation, or inherit it and augment with custom operations or
    overwrite certain methods to provide more functionality (e.g.: getData).

    One such example is the following, where we instantiate a model for our
    own MyItem type:
    \code{.cpp}
    class MyItem : public GenericItem
    {
    };

    typedef GenericItemModel<MyItem> MyItemModel;
    \endcode

\remark
    Items added to the model are taken over and managed by the model, which
    means they are deleted by it. delete is used for that, so
    new should be used for already created objects that are added.

\tparam TGenericItem
    The type of the item this model operates on. The type \b needs to inherit
    directly or indirectly from GenericItem.
*/
template <typename TGenericItem>
class GenericItemModel : public ItemModel
{
public:
    /*!
    \brief
        Creates a new model instance using the specified \a root item, which
        must not be NULL - otherwise an InvalidRequestException is thrown.

        The \a root will be taken ownership of, and cleaned when the model is
        destructed (using delete).

        An example of initializing such GenericItemModel implementation is the
        following:
        \code{.cpp}
        class MyItemModel : public GenericItemModel<MyItem>
        {
            MyItemModel() : GenericItemModel<MyItem>(new MyItem)
            {
            }
        };
        \endcode

        Alternatively, if you want to create a root upon declaring the model, you
        can let the caller provide it, in which case our \b MyItemModel constructor
        will receive the root TGenericItem instance.
    */
    explicit GenericItemModel(TGenericItem* root);
    virtual ~GenericItemModel();

    /*!
    \brief
        Adds a new item in the model with the specified text, taking ownership
        of it.

        If this method is used, the TGenericItem type should have a string
        constructor in order to successfully compile. This method is equivalent
        with:
        \code{.cpp}
        GenericItem* item = new GenericItem("MyText");
        model->addItem(item);
        \endcode
    */
    virtual void addItem(String text);
    //! Adds the item as child of the root and takes ownership of it.
    virtual void addItem(GenericItem* item);
    //! Adds the item as child of the root at the specified position.
    virtual void addItemAtPosition(GenericItem* item, size_t pos);
    /*!
    \brief
        Adds the item as child of the specified parent, at the specified position.

        After the addition, the \a parent's node will contain \a item as child
        at the \a position.
    */
    virtual void addItemAtPosition(GenericItem* item, const ModelIndex& parent,
        size_t position);

    /*!
    \brief
        Inserts the specified \a item before the specified \a position item.
        If \a position is NULL the new item will be added at the beginning of
        the root's children list.

        This method allows for arbitrary level (nestedness/depth) insertions.
        Another function which can be used instead of this, is
        addItemAtPosition(GenericItem*, const ModelIndex&, size_t).
    */
    virtual void insertItem(GenericItem* item, const GenericItem* position);

    virtual void removeItem(const GenericItem* item);
    virtual void removeItem(const ModelIndex& index);

    /*!
    \brief
        Clears the items of this ItemModel, deleting them, using delete
        on each, optionally notifying any listeners of the removal of the items.

    \param notify
        If true, it will raise the EventChildrenWillBeRemoved and
        EventChildrenRemoved events for each deleted item.
    */
    virtual void clear(bool notify = true);

    /*!
    \brief
        Gets the underlying TGenericItem represented by the given \a index or
        NULL if the index does not represent a valid item.

        This method is the inverse of to getIndexForItem(const GenericItem*).
    */
    virtual TGenericItem* getItemForIndex(const ModelIndex& index) const;

    /*!
    \brief
        Creates a ModelIndex that represents the specified index.

        This method is the inverse of getItemForIndex(const ModelIndex&).
    */
    virtual ModelIndex getIndexForItem(const GenericItem* item) const;

    //! Gets the ordinal id (index) of the specified item in parent's children list.
    virtual int getChildId(const GenericItem* item) const;

    /*!
    \brief
        This represents the conceptual root of this model. This root will be
        ignored in normal operations. It's merely used in order to provide
        an access point to the items tree.
    */
    TGenericItem& getRoot() { return static_cast<TGenericItem&>(*d_root); }

    bool isValidIndex(const ModelIndex& model_index) const override;
    ModelIndex makeIndex(size_t child, const ModelIndex& parent_index) override;
    bool areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const override;
    int compareIndices(const ModelIndex& index1, const ModelIndex& index2) const override;
    ModelIndex getParentIndex(const ModelIndex& model_index) const override;
    int getChildId(const ModelIndex& model_index) const override;
    ModelIndex getRootIndex() const override;
    size_t getChildCount(const ModelIndex& model_index) const override;
    String getData(const ModelIndex& model_index, ItemDataRole role = ItemDataRole::Text) override;

protected:
    //! Deletes all children of the specified item, optionally invoking the
    //! EventChildren(WillBe)Removed event
    void deleteChildren(GenericItem* item, bool notify);

    //! Makes a valid index if \a id is withing \a vector's bounds.
    template <typename T>
    ModelIndex makeValidIndex(size_t id, std::vector<T>& vector);

    GenericItem* d_root;
};

//----------------------------------------------------------------------------//
// Implementation of the GenericItemModel
//----------------------------------------------------------------------------//
template <typename TGenericItem>
GenericItemModel<TGenericItem>::GenericItemModel(TGenericItem* root) :
d_root(root)
{
    if (root == nullptr)
        throw InvalidRequestException("Root cannot be null");
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
template <typename T>
ModelIndex GenericItemModel<TGenericItem>::makeValidIndex(size_t id,
    std::vector<T>& vector)
{
    return (id < vector.size()) ? ModelIndex(vector.at(id)) : ModelIndex();
}

template <typename TGenericItem>
GenericItemModel<TGenericItem>::~GenericItemModel()
{
    GenericItemModel<TGenericItem>::clear(false);

    delete d_root;
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
bool GenericItemModel<TGenericItem>::isValidIndex(const ModelIndex& model_index) const
{
    return model_index.d_modelData != nullptr;
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
ModelIndex GenericItemModel<TGenericItem>::makeIndex(size_t child,
    const ModelIndex& parent_index)
{
    if (parent_index.d_modelData == nullptr)
        return ModelIndex();

    GenericItem* item = static_cast<GenericItem*>(parent_index.d_modelData);
    return makeValidIndex(child, item->getChildren());
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
bool GenericItemModel<TGenericItem>::areIndicesEqual(const ModelIndex& index1,
    const ModelIndex& index2) const
{
    return compareIndices(index1, index2) == 0;
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
int GenericItemModel<TGenericItem>::compareIndices(const ModelIndex& index1,
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
template <typename TGenericItem>
ModelIndex GenericItemModel<TGenericItem>::getParentIndex(const ModelIndex& model_index) const
{
    if (model_index.d_modelData == d_root)
        return ModelIndex();

    GenericItem* item = static_cast<GenericItem*>(model_index.d_modelData);
    if (item->getParent() == nullptr)
        return getRootIndex();

    return ModelIndex(item->getParent());
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
int GenericItemModel<TGenericItem>::getChildId(const ModelIndex& model_index) const
{
    return getChildId(getItemForIndex(model_index));
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
int GenericItemModel<TGenericItem>::getChildId(const GenericItem* item) const
{
    if (item == nullptr || item->getParent() == nullptr)
        return -1;

    GenericItem* parent_item = item->getParent();

    std::vector<GenericItem*>::iterator itor = std::find(
        parent_item->getChildren().begin(), parent_item->getChildren().end(), item);

    if (itor == parent_item->getChildren().end())
        return -1;

    return static_cast<int>(std::distance(parent_item->getChildren().begin(), itor));
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
ModelIndex GenericItemModel<TGenericItem>::getRootIndex() const
{
    return ModelIndex(d_root);
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
size_t GenericItemModel<TGenericItem>::getChildCount(const ModelIndex& model_index) const
{
    if (model_index.d_modelData == nullptr)
        return d_root->getChildren().size();

    return static_cast<GenericItem*>(model_index.d_modelData)->getChildren().size();
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
String GenericItemModel<TGenericItem>::getData(const ModelIndex& model_index,
    ItemDataRole role /*= TEXT*/)
{
    if (!isValidIndex(model_index))
        return "";

    GenericItem* item = static_cast<GenericItem*>(model_index.d_modelData);
    if (role == ItemDataRole::Text) return item->getText();
    if (role == ItemDataRole::Icon) return item->getIcon();
    if (role == ItemDataRole::Tooltip) return item->getTooltipText();

    return "";
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::addItem(String text)
{
    addItem(new TGenericItem(text));
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::addItem(GenericItem* item)
{
    if (item == nullptr)
        throw InvalidRequestException("Cannot add a NULL item to the model!");

    addItemAtPosition(item,
        d_root->getChildren().empty() ? 0 : d_root->getChildren().size());
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::addItemAtPosition(GenericItem* item, size_t pos)
{
    addItemAtPosition(item, getRootIndex(), pos);
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::addItemAtPosition(GenericItem* new_item,
    const ModelIndex& parent_index, size_t position)
{
    notifyChildrenWillBeAdded(parent_index, position, 1);

    GenericItem* parent = static_cast<GenericItem*>(parent_index.d_modelData);
    if (position > parent->getChildren().size())
        throw InvalidRequestException("The specified position is out of range.");

    new_item->setParent(parent);
    parent->getChildren().insert(parent->getChildren().begin() + position, new_item);

    notifyChildrenAdded(parent_index, position, 1);
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::insertItem(GenericItem* item,
    const GenericItem* position)
{
    int child_id = position == nullptr ? -1 : getChildId(position);

    ModelIndex parent_index = getRootIndex();
    if (position != nullptr)
        parent_index = getParentIndex(getIndexForItem(position));

    addItemAtPosition(item, parent_index,
        child_id <= 0 ? 0 : static_cast<size_t>(child_id));
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::removeItem(const ModelIndex& index)
{
    removeItem(getItemForIndex(index));
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::removeItem(const GenericItem* item)
{
    GenericItem* parent_item = item->getParent();

    std::vector<GenericItem*>::iterator itor = std::find(
        parent_item->getChildren().begin(), parent_item->getChildren().end(),
        item);

    if (itor != parent_item->getChildren().end())
    {
        size_t child_id = std::distance(parent_item->getChildren().begin(), itor);

        notifyChildrenWillBeRemoved(ModelIndex(parent_item), child_id, 1);

        deleteChildren(*itor, true);
        delete *itor;
        parent_item->getChildren().erase(itor);

        notifyChildrenRemoved(ModelIndex(parent_item), child_id, 1);
    }
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
TGenericItem* GenericItemModel<TGenericItem>::getItemForIndex(
    const ModelIndex& index) const
{
    if (index.d_modelData == nullptr)
        return nullptr;

    return static_cast<TGenericItem*>(index.d_modelData);
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void GenericItemModel<TGenericItem>::clear(bool notify /*= true */)
{
    deleteChildren(d_root, notify);
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
ModelIndex GenericItemModel<TGenericItem>::getIndexForItem(
    const GenericItem* item) const
{
    // TODO: this is annoying. We should be able to just hand out the ModelIndex
    // Right now we can't, since we're in a const method, operating on a const item
    // In hindsight, the index we hand out will be modifiable so maybe we need
    // to change the const-ness of the item parameter.
    GenericItem* parent = item->getParent();
    int child_id = getChildId(item);
    return ModelIndex(child_id != -1 ? parent->getChildren().at(child_id) : 0);
}

//----------------------------------------------------------------------------//
template <typename TGenericItem>
void CEGUI::GenericItemModel<TGenericItem>::deleteChildren(GenericItem* item,
    bool notify)
{
    if (item == nullptr)
        throw InvalidRequestException("Cannot delete children of a NULL item!");

    size_t items_count = item->getChildren().size();
    std::vector<GenericItem*>::iterator itor = item->getChildren().begin();

    if (notify)
    {
        notifyChildrenWillBeRemoved(ModelIndex(item), 0, items_count);
    }

    while (itor != item->getChildren().end())
    {
        deleteChildren(*itor, notify);
        delete *itor;
        itor = item->getChildren().erase(itor);
    }

    if (notify)
    {
        notifyChildrenRemoved(ModelIndex(item), 0, items_count);
    }
}
}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
