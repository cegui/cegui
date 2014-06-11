/***********************************************************************
    filename:   ItemView.cpp
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Implementation of the base class for all item model-based views.
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
#include "CEGUI/views/ItemView.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const Colour ItemView::DefaultTextColour = 0xFFFFFFFF;

//----------------------------------------------------------------------------//
ViewRenderingState::ViewRenderingState() :
    d_isDirty(true)
{
}

//----------------------------------------------------------------------------//
ViewRenderingState::~ViewRenderingState()
{
}

//----------------------------------------------------------------------------//
void ViewRenderingState::setIsDirty(bool value)
{
    d_isDirty = value;
}

//----------------------------------------------------------------------------//
bool ViewRenderingState::isDirty() const
{
    return d_isDirty;
}

//----------------------------------------------------------------------------//
ItemView::ItemView(const String& type, const String& name) :
    Window(type, name),
    d_itemModel(0),
    d_textColourRect(ColourRect(DefaultTextColour)),
    d_eventChildrenAddedConnection(0),
    d_eventChildrenRemovedConnection(0)
{
}

//----------------------------------------------------------------------------//
ItemView::~ItemView()
{
    disconnectModelEvents();
}

//----------------------------------------------------------------------------//
void ItemView::setModel(ItemModel* item_model)
{
    if (item_model == d_itemModel)
        return;

    if (d_itemModel != 0)
    {
        disconnectModelEvents();
    }

    d_itemModel = item_model;

    connectToModelEvents(d_itemModel);
    invalidateView(true);
}

//----------------------------------------------------------------------------//
void ItemView::connectToModelEvents(ItemModel* d_itemModel)
{
    d_eventChildrenAddedConnection = d_itemModel->subscribeEvent(ItemModel::EventChildrenAdded,
        &ItemView::onChildrenAdded, this);
    d_eventChildrenRemovedConnection = d_itemModel->subscribeEvent(ItemModel::EventChildrenRemoved,
        &ItemView::onChildrenRemoved, this);
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenAdded(const EventArgs& args)
{
    invalidateView(false);

    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenRemoved(const EventArgs& args)
{
    invalidateView(false);

    return true;
}

//----------------------------------------------------------------------------//
void ItemView::disconnectModelEvents()
{
    if (d_eventChildrenAddedConnection != 0)
        d_eventChildrenAddedConnection->disconnect();

    if (d_eventChildrenRemovedConnection != 0)
        d_eventChildrenRemovedConnection->disconnect();
}

//----------------------------------------------------------------------------//
void ItemView::invalidateView(bool recursive)
{
    getRenderingState()->setIsDirty(true);
    invalidate(recursive);
}

//----------------------------------------------------------------------------//
ItemModel* ItemView::getModel() const
{
    return d_itemModel;
}

//----------------------------------------------------------------------------//
void ItemView::prepareForRender()
{
}

//----------------------------------------------------------------------------//
const ColourRect& ItemView::getTextColourRect() const
{
    return d_textColourRect;
}

//----------------------------------------------------------------------------//
void ItemView::setTextColourRect(const ColourRect& colour_rect)
{
    d_textColourRect = colour_rect;
}
}
