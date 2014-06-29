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
#include "CEGUI/Image.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String FalagardTreeView::TypeName("Core/TreeView");
const float SUBTREE_EXPANDER_MARGIN = 5.0f;

//----------------------------------------------------------------------------//
FalagardTreeView::FalagardTreeView(const String& type) :
    TreeViewWindowRenderer(type),
    d_subtreeExpanderImagery(0),
    d_subtreeCollapserImagery(0),
    d_subtreeExpanderImagerySize(0, 0)
{
}

//----------------------------------------------------------------------------//
void FalagardTreeView::render()
{
    const WidgetLookFeel& wlf = getLookNFeel();
    TreeView* tree_view = static_cast<TreeView*>(d_window);

    tree_view->prepareForRender();

    const StateImagery* imagery = &wlf.getStateImagery(
        tree_view->isEffectiveDisabled() ? "Disabled" : "Enabled");
    imagery->render(*tree_view);

    Rectf items_area(getViewRenderArea());
    Vector2f item_pos(getItemRenderStartPosition(tree_view, items_area));
    renderTreeItem(tree_view, items_area, item_pos, tree_view->getRootItemState(), 0);
}

//----------------------------------------------------------------------------//
void FalagardTreeView::renderTreeItem(TreeView* tree_view, const Rectf& items_area,
    Vector2f& item_pos, const TreeViewItemRenderingState& item_to_render,
    size_t depth)
{
    for (size_t i = 0; i < item_to_render.d_renderedChildren.size(); ++i)
    {
        TreeViewItemRenderingState item = item_to_render.d_renderedChildren.at(i);
        RenderedString& rendered_string = item.d_string;
        Sizef size(item.d_size);

        size.d_width = ceguimax(items_area.getWidth(), size.d_width);
        if (item.d_totalChildCount > 0)
        {
            const ImagerySection* section = item.d_subtreeIsExpanded
                ? d_subtreeCollapserImagery : d_subtreeExpanderImagery;

            Rectf button_rect;
            button_rect.left(item_pos.d_x + SUBTREE_EXPANDER_MARGIN);
            button_rect.top(item_pos.d_y + SUBTREE_EXPANDER_MARGIN);
            button_rect.setSize(d_subtreeExpanderImagerySize);
            button_rect.right(button_rect.right() - SUBTREE_EXPANDER_MARGIN);

            Rectf button_clipper(button_rect.getIntersection(items_area));
            section->render(*tree_view, button_rect, 0, &button_clipper);
        }

        Rectf item_rect;
        item_rect.left(item_pos.d_x + d_subtreeExpanderImagerySize.d_width);
        item_rect.top(item_pos.d_y);
        item_rect.setSize(size);

        Rectf item_clipper(item_rect.getIntersection(items_area));
        renderString(tree_view, rendered_string, item_rect,
            tree_view->getFont(), &item_clipper, item.d_isSelected);

        item_pos.d_y += size.d_height;

        if (item.d_renderedChildren.empty())
            continue;

        item_pos.d_x += d_subtreeExpanderImagerySize.d_width;

        if (item.d_subtreeIsExpanded)
        {
            renderTreeItem(tree_view, items_area, item_pos, item, depth + 1);
        }

        item_pos.d_x -= d_subtreeExpanderImagerySize.d_width;
    }
}

static Sizef getImagerySize(const ImagerySection* section)
{
    const ImageryComponent& component = section->getImageryComponentIterator().getCurrentValue();
    const Image* img = component.getImage();
    return img->getRenderedSize();
}

//----------------------------------------------------------------------------//
void FalagardTreeView::onLookNFeelAssigned()
{
    const WidgetLookFeel& wlf = getLookNFeel();
    d_subtreeExpanderImagery = &wlf.getImagerySection("SubtreeExpander");
    d_subtreeCollapserImagery = &wlf.getImagerySection("SubtreeCollapser");

    Sizef open_size = getImagerySize(d_subtreeExpanderImagery);
    Sizef close_size = getImagerySize(d_subtreeCollapserImagery);
    d_subtreeExpanderImagerySize = Sizef(
        ceguimin(
            open_size.d_width + SUBTREE_EXPANDER_MARGIN,
            close_size.d_width + SUBTREE_EXPANDER_MARGIN),
        ceguimin(
            open_size.d_height + SUBTREE_EXPANDER_MARGIN,
            close_size.d_height + SUBTREE_EXPANDER_MARGIN));
}

//----------------------------------------------------------------------------//
Sizef FalagardTreeView::getSubtreeExpanderSize(void) const
{
    return d_subtreeExpanderImagerySize;
}

//----------------------------------------------------------------------------//
Rectf FalagardTreeView::getViewRenderArea(void) const
{
    return ItemViewRenderer::getViewRenderArea(this);
}

//----------------------------------------------------------------------------//
float FalagardTreeView::getSubtreeExpanderXIndent(int depth) const
{
    return depth * d_subtreeExpanderImagerySize.d_width;
}
}
