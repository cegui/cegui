/***********************************************************************
    filename:   TreeView.cpp
    created:    Fri Jun 06 2014
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
#include "CEGUI/views/TreeView.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String TreeView::EventNamespace("TreeView");
const String TreeView::WidgetTypeName("CEGUI/TreeView");

//----------------------------------------------------------------------------//
TreeView::TreeView(const String& type, const String& name) :
    ItemView(type, name)
{

}

//----------------------------------------------------------------------------//
TreeView::~TreeView()
{

}

//----------------------------------------------------------------------------//
const TreeViewItemRenderingState& TreeView::getRootItemState() const
{
    return d_rootItemState;
}

//----------------------------------------------------------------------------//
void TreeView::prepareForRender()
{
    if (d_itemModel == 0 || !isDirty())
        return;

    ModelIndex root_index = d_itemModel->getRootIndex();
    d_rootItemState = computeRenderingStateForIndex(root_index);
    setIsDirty(false);
}

//----------------------------------------------------------------------------//
TreeViewItemRenderingState TreeView::computeRenderingStateForIndex(const ModelIndex& index)
{
    if (d_itemModel == 0)
        return TreeViewItemRenderingState();

    size_t child_count = d_itemModel->getChildCount(index);
    ColourRect text_colour = getTextColourRect();

    TreeViewItemRenderingState state;
    String text = d_itemModel->getData(index);
    RenderedString rendered_string = getRenderedStringParser().parse(
        text, getFont(), &d_textColourRect);
    state.d_string = rendered_string;
    state.d_size = Sizef(
        rendered_string.getHorizontalExtent(this),
        rendered_string.getVerticalExtent(this));

    //TODO: move the selection state to ItemView
    //state.d_isSelected = isIndexSelected(index);
    for (size_t child = 0; child < child_count; ++child)
    {
        ModelIndex child_index = d_itemModel->makeIndex(child, index);
        state.d_children.push_back(computeRenderingStateForIndex(child_index));
    }

    return state;
}

}
