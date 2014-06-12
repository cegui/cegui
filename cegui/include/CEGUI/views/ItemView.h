/***********************************************************************
    filename:   ItemView.h
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Interface of the base class for all item model-based views.
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
#ifndef _CEGUIItemView_h_
#define _CEGUIItemView_h_

#include "CEGUI/Base.h"
#include "CEGUI/ItemModel.h"
#include "CEGUI/Window.h"
#include "CEGUI/Colour.h"
#include "CEGUI/ColourRect.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    Stores the selection state of a ModelIndex. This is used to regenerate
    the new proper selection index when the model changes in any way (e.g.:
    new item, removed item).
*/
struct CEGUIEXPORT ModelIndexSelectionState
{
    ModelIndex d_parentIndex;
    size_t d_childId;

    ModelIndex d_selectedIndex;
};

/*!
\brief
    Abstract base class for all view classes based on ItemModel
*/
class CEGUIEXPORT ItemView : public Window
{
public:
    ItemView(const String& type, const String& name);

    virtual ~ItemView();

    static const Colour DefaultTextColour;

    /*!
    \brief
        Sets the ItemModel to be used inside this view.

    \param item_model
        The ItemModel instance to be set.
    */
    virtual void setModel(ItemModel* item_model);

    /*!
    \brief
        Returns the current ItemModel of this view.
    */
    virtual ItemModel* getModel() const;

    /*!
    \brief
        Instructs this ItemView to prepare its rendering state for rendering.
        This is usually done by updating the rendering state if it got dirty
        in the meantime.
    */
    virtual void prepareForRender();

    const ColourRect& getTextColourRect() const;
    void setTextColourRect(const ColourRect& colour_rect);

    bool isDirty() const;
    //! Specifies whether this view requires processing before being able to render it.
    void setIsDirty(bool value);

    //! Gets the current state of the indices used for selection.
    const std::vector<ModelIndexSelectionState>& getIndexSelectionStates() const;

protected:
    ItemModel* d_itemModel;
    ColourRect d_textColourRect;
    bool d_isDirty;
    std::vector<ModelIndexSelectionState> d_indexSelectionStates;

    //! Invalidates this view by marking the rendering state as dirty and calling the base
    virtual void invalidateView(bool recursive);

    virtual bool onChildrenAdded(const EventArgs& args);
    virtual bool onChildrenRemoved(const EventArgs& args);

    Event::Connection d_eventChildrenAddedConnection;
    Event::Connection d_eventChildrenRemovedConnection;
    void connectToModelEvents(ItemModel* d_itemModel);
    void disconnectModelEvents();
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
