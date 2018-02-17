/***********************************************************************
    created:    Tue May 20 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Provides ModelIndex and ItemModel which are the basis of the
                Model-View architecture.
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
#ifndef _CEGUIItemModel_h_
#define _CEGUIItemModel_h_

#include "CEGUI/Base.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/EventSet.h"
#include "CEGUI/String.h"

namespace CEGUI
{

/*!
\brief
    ModelIndex is CEGUI's way of representing user data in the views. It uses
    a \b void* pointer to store a reference to arbitrary, user-owned data.

    Even though the naming might suggest a particular order, it doesn't have any.
    For comparing two indices ItemModel::compareIndices(const ModelIndex&, const ModelIndex&)
    should be used.

    The index should not be stored because the pointed data might be deleted in
    the meantime. The only entity that should interpret the contents of this
    class is the ItemModel (and its descendants). All decisions (e.g.: comparisons,
    data providing) based on an index need to go through the ItemModel.

    All operations with the view are done using the ModelIndex. For example, if you want
    to select something you need to give it the ModelIndex of that item's. What that means
    is that the view doesn't know about the actual model items.
*/
class CEGUIEXPORT ModelIndex
{
public:
    /*!
    \brief
        Constructs an index with the specified model data.

    \param model_data
        Optional model data associated with this index.
    */
    explicit ModelIndex(void* model_data = nullptr);

    /*!
    \brief
        Pointer to model-specific data.

    \remark
        DO NOT USE/INTERPRET in view. This is just a simple way for the model
        to be able to manage its data and logic.
    */
    void* d_modelData;

    CEGUIEXPORT friend std::ostream& operator<< (std::ostream& os, const ModelIndex& arg);
};

/*!
\brief
    Enumeration that specifies which type of data is required from the ItemModel
    in order for the view to render it.

    Users can use the ItemDataRole::User member as a starting base for custom roles. Anything
    below is reserved for CEGUI usage.
*/
enum class ItemDataRole : int
{
    Text,
    //! The string returned for the icon must be an image name that can
    //! retrieved from ImageManager
    Icon,
    Tooltip,

    //! This marks the beginning of the user-defined item data roles
    User = 0x1000
};

/*
TODO: The count thing is very ugly and counter-intuitive to the whole design.
    We should either:
    * Fire an event per index
    * Store the actual indices in a vector.
*/

/*!
\brief
    Arguments class for events that happened with regard to the specified ItemModel
    on the specified indices.
*/
class CEGUIEXPORT ModelEventArgs : public EventArgs
{
public:
    ModelEventArgs(ItemModel* item_model, ModelIndex parent_index, size_t start_id, size_t count = 1);

    //! The source ItemModel that triggered the event.
    ItemModel* d_itemModel;

    //! The parent ModelIndex the event happened under.
    ModelIndex d_parentIndex;

    //! The starting id the event happened on.
    size_t d_startId;

    //! The number of items after the start index that have been affected by the event.
    size_t d_count;
};

/*!
\brief
    Abstract class defining the interface between the view and the user data.
    This is used by views to query data that is to be shown.

    A view will require a custom implementation of this provider. CEGUI provides
    a basic implementation in form of GenericItemModel.

    This model provides events to notify listeners (usually the view it is
    attached to) of different events that will happen or happened. It is the
    implementer's job to properly raise the events with the proper arguments so
    that the view can correctly process those and render the view. For convenience,
    there are the notifyChildren* method which raise the specified events
    with the proper event arguments.
*/
class CEGUIEXPORT ItemModel : public EventSet
{
public:
    virtual ~ItemModel();

    //! Name of the event triggered \b before children will be added
    static const String EventChildrenWillBeAdded;
    //! Name of the event triggered \b after new children were added
    static const String EventChildrenAdded;

    //! Name of the event triggered \b before existing children will be removed
    static const String EventChildrenWillBeRemoved;
    //! Name of the event triggered \b after existing children were removed
    static const String EventChildrenRemoved;

    //! Name of the event triggered \b before existing children's data will be changed
    static const String EventChildrenDataWillChange;
    //! Name of the event triggered \b after existing children's data was changed
    static const String EventChildrenDataChanged;

    /*!
    \brief
        Returns true if the specified ModelIndex is valid, false otherwise.

        Usually, an index is valid if at least it does not contain a reference
        to a NULL object. Extra logic can be added to check that the referenced
        object is actually part of the model.
    */
    virtual bool isValidIndex(const ModelIndex& model_index) const = 0;

    /*!
    \brief
        Creates a new ModelIndex that points to the specified child of the
        specified parent index.

        To create an index for a nested item, you need to chain the index creation.
        Given a model for the following tree:
        <pre>
        A
        |
        |--B
        |  |--C
        |
        |--D
        </pre>

        We can compute the indices for the nodes in the following way:
        \code{.cpp}
        ModelIndex a = makeIndex(0, getRootIndex());
        ModelIndex b = makeIndex(0, a);
        ModelIndex c = makeIndex(0, b);
        ModelIndex d = makeIndex(1, a);
        \endcode

    \param child
        The ordinal child id (index), which is a number between 0 and
        getChildrenCount(parent_index). This will specify which children in the
        parent's index list of children should be referenced.
    */
    virtual ModelIndex makeIndex(size_t child, const ModelIndex& parent_index) = 0;

    /*!
    \brief
        Compares semantically two indices and returns true if they are equal,
        false otherwise.
    */
    virtual bool areIndicesEqual(const ModelIndex& index1, const ModelIndex& index2) const;

    /*!
    \brief
        Compares semantically the contents of the specified two indices and returns:
        - 0 if they are equal.
        - -1 if first index is less than the second index.
        - 1 if the first index is bigger than the second index.

        This function is used by the view when it needs to sort the items.
    */
    virtual int compareIndices(const ModelIndex& index1, const ModelIndex& index2) const = 0;

    /*!
    \brief
        Returns the ModelIndex which is parent for the specified ModelIndex.

    \return
        The returned index should be equal to getRootIndex() for direct children
        of the root index. If this is called on the root index, it returns
        an invalid (empty/default) ModelIndex.
    */
    virtual ModelIndex getParentIndex(const ModelIndex& model_index) const = 0;

    /*!
    \brief
        Returns the ordinal id (index) of the child represented by the given
        index, in its parent's list or -1 if no such child exists.
    */
    virtual int getChildId(const ModelIndex& model_index) const = 0;

    /*!
    \brief
        Returns the root ModelIndex of this model.

    \remark
        This index is not used for any rendering, but only to provide the initial
        access to the items. For example, in the case of a list this might contain
        all the items in the list.
    */
    virtual ModelIndex getRootIndex() const = 0;

    /*!
    \brief
        Returns the number of direct children of the specified ModelIndex.
    */
    virtual size_t getChildCount(const ModelIndex& model_index) const = 0;

    /*!
    \brief
        Returns the string representation of the specified ModelIndex based on
        the specified role.
        It is up to the view to interpret the string based on the role requested.

    \remark
        For example, in the case of an image decoration, the name of the image
        could be returned, and the view would use ImageManager to retrieve the
        specific Image instance by the name and render that.

        An example implementation for the IDR_Icon role could be:
        \code{.cpp}
        Image* img; // get the image from somewhere

        String getData(const ModelIndex& model_index, ItemDataRole role)
        {
            if (role == IDR_Icon) return img->getName();
        }
        \endcode
    */
    virtual String getData(const ModelIndex& model_index, ItemDataRole role = ItemDataRole::Text) = 0;

    /*!
    \brief
        Notifies any listeners of the EventChildrenWillBeAdded event that new children
        will be added to this model.

    \param parent_index
        The parent index under which children will be added.

    \param start_id
        The id of the child starting from which children will be added.

    \param count
        The number of children that will be added.

    \remark
        If this method is overridden, it *needs* to call this base method or invoke
        manually the EventChildrenWillBeAdded event.
    */
    virtual void notifyChildrenWillBeAdded(ModelIndex parent_index,
        size_t start_id, size_t count);

    /*!
    \brief
        Notifies any listeners of the EventChildrenAdded event that new children
        have been added to this model.

    \param parent_index
        The parent index under which children have been added.

    \param start_id
        The id of the child starting from which children have been added.

    \param count
        The number of children that have been added.

    \remark
        If this method is overridden, it *needs* to call this base method or invoke
        manually the EventChildrenAdded event.
    */
    virtual void notifyChildrenAdded(ModelIndex parent_index,
        size_t start_id, size_t count);

    /*!
    \brief
        Notifies any listeners of the EventChildrenWillBeRemoved event that existing
        children will be removed from this model.

    \param parent_index
        The parent index under which children will be removed.

    \param start_id
        The id of the child starting from which children will be removed.

    \param count
        The number of children that will be removed.

    \remark
        If this method is overridden, it *needs* to call this base method or invoke
        manually the EventChildrenWillBeRemoved event.
    */
    virtual void notifyChildrenWillBeRemoved(ModelIndex parent_index,
        size_t start_id, size_t count);

    /*!
    \brief
        Notifies any listeners of the EventChildrenRemoved event that existing
        children have been removed from this model.

    \param parent_index
        The parent index under which children have been removed.

    \param start_id
        The id of the child starting from which children have been removed.

    \param count
        The number of children that have been removed.

    \remark
        If this method is overridden, it *needs* to call this base method or invoke
        manually the EventChildrenRemoved event.
    */
    virtual void notifyChildrenRemoved(ModelIndex parent_index,
        size_t start_id, size_t count);

    /*!
    \brief
        Notifies any listeners of the EventChildDataWillChange event that existing
        children's data will change.

    \remark
        If this method is overridden, it *needs* to call this base method or invoke
        manually the EventChildDataWillChange event.
    */
    virtual void notifyChildrenDataWillChange(ModelIndex parent_index,
        size_t start_id, size_t count);

    /*!
    \brief
        Notifies any listeners of the EventChildDataChanged event that existing
        children's data has been updated.

    \remark
        If this method is overridden, it *needs* to call this base method or invoke
        manually the EventChildDataChanged event.
    */
    virtual void notifyChildrenDataChanged(ModelIndex parent_index,
        size_t start_id, size_t count);
};

}

#endif
