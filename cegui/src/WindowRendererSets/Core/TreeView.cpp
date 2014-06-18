/***********************************************************************
    filename:   TreeView.cpp
    created:    Fri Jun 06 2014
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
#include "CEGUI/WindowRendererSets/Core/TreeView.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String FalagardTreeView::TypeName("Core/TreeView");

//----------------------------------------------------------------------------//
FalagardTreeView::FalagardTreeView(const String& type) :
    ItemViewRenderer(type)
{
}

//----------------------------------------------------------------------------//
void FalagardTreeView::render()
{
    const StateImagery* imagery;
    const WidgetLookFeel& wlf = getLookNFeel();
    TreeView* tree_view = static_cast<TreeView*>(d_window);

    tree_view->prepareForRender();

    imagery = &wlf.getStateImagery(tree_view->isEffectiveDisabled() ? "Disabled" : "Enabled");
    imagery->render(*tree_view);

    Rectf items_area(getViewRenderArea());
    Vector2f item_pos(items_area.left(), items_area.top());
    renderTreeItem(tree_view, items_area, item_pos, tree_view->getRootItemState());
}

//----------------------------------------------------------------------------//
void FalagardTreeView::renderTreeItem(TreeView* tree_view, const Rectf& items_area,
    Vector2f& item_pos, const TreeViewItemRenderingState& item_state)
{
    const float SUBTREE_IDENT = 20.0f;
    for (size_t i = 0; i < item_state.d_children.size(); ++i)
    {
        TreeViewItemRenderingState item = item_state.d_children.at(i);
        RenderedString& rendered_string = item.d_string;
        Sizef size(item.d_size);

        size.d_width = ceguimax(items_area.getWidth(), size.d_width);

        Rectf item_rect;
        item_rect.left(item_pos.d_x);
        item_rect.top(item_pos.d_y);
        item_rect.setSize(size);

        Rectf item_clipper(item_rect.getIntersection(items_area));

        //TODO: selection
        renderString(tree_view, rendered_string, item_rect,
            tree_view->getFont(), &item_clipper, item.d_isSelected);

        item_pos.d_y += size.d_height;

        //TODO: add opener/closer button
        item_pos.d_x += SUBTREE_IDENT;

        renderTreeItem(tree_view, items_area, item_pos, item);

        item_pos.d_x -= SUBTREE_IDENT;
    }
}

}
