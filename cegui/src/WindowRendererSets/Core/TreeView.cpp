/***********************************************************************
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
#include "CEGUI/ImageManager.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
const String FalagardTreeView::TypeName("Core/TreeView");
static ColourRect ICON_COLOUR_RECT(Colour(1, 1, 1, 1));

//----------------------------------------------------------------------------//
FalagardTreeView::FalagardTreeView(const String& type) :
    TreeViewWindowRenderer(type),
    d_subtreeExpanderImagery(nullptr),
    d_subtreeCollapserImagery(nullptr),
    d_subtreeExpanderImagerySize(0, 0)
{
}

//----------------------------------------------------------------------------//
void FalagardTreeView::createRenderGeometry()
{
    const WidgetLookFeel& wlf = getLookNFeel();
    TreeView* tree_view = getView();

    tree_view->prepareForRender();

    bool has_focused_state =
        tree_view->isFocused() && wlf.isStateImageryPresent("EnabledFocused");
    const StateImagery* imagery = &wlf.getStateImagery(
        tree_view->isEffectiveDisabled() ? "Disabled" :
            (has_focused_state ? "EnabledFocused" : "Enabled"));
    imagery->render(*tree_view);

    Rectf items_area(getViewRenderArea());
    glm::vec2 item_pos(getItemRenderStartPosition(tree_view, items_area));
    renderTreeItem(tree_view, items_area, item_pos, &tree_view->getRootItemState(), 0);
}

//----------------------------------------------------------------------------//
void FalagardTreeView::renderTreeItem(TreeView* tree_view, const Rectf& items_area,
    glm::vec2& item_pos, const TreeViewItemRenderingState* item_to_render, size_t depth)
{
    float expander_margin = tree_view->getSubtreeExpanderMargin();
    for (TreeViewItemRenderingState* const item : item_to_render->d_renderedChildren)
    {
        Sizef size = item->d_size;

        // center the expander compared to the item's height
        float half_diff = (size.d_height - d_subtreeExpanderImagerySize.d_height) / 2.0f;

        size.d_width = std::max(items_area.getWidth(), size.d_width);
        float indent = d_subtreeExpanderImagerySize.d_width + expander_margin * 2;
        if (item->d_totalChildCount > 0)
        {
            const ImagerySection* section = item->d_subtreeIsExpanded
                ? d_subtreeCollapserImagery : d_subtreeExpanderImagery;

            Rectf button_rect;
            button_rect.left(item_pos.x + expander_margin);
            button_rect.top(item_pos.y +
                (half_diff > 0 ? half_diff : 0));
            button_rect.setSize(d_subtreeExpanderImagerySize);

            Rectf button_clipper(button_rect.getIntersection(items_area));
            section->render(*tree_view, button_rect, nullptr, &button_clipper);

            indent = button_rect.getWidth() + expander_margin * 2;
        }

        Rectf item_rect;
        item_rect.left(item_pos.x + indent);
        item_rect.top(item_pos.y + (half_diff < 0 ? -half_diff : 0));
        item_rect.setSize(size);

        if (!item->d_icon.empty())
        {
            Image& img = ImageManager::getSingleton().get(item->d_icon);

            Rectf icon_rect(item_rect);
            icon_rect.setWidth(size.d_height);
            icon_rect.setHeight(size.d_height);

            Rectf icon_clipper(icon_rect.getIntersection(items_area));

            ImageRenderSettings renderSettings(icon_rect, &icon_clipper, ICON_COLOUR_RECT, 1.0f);

            img.createRenderGeometry(tree_view->getGeometryBuffers(), renderSettings);

            item_rect.left(item_rect.left() + icon_rect.getWidth());
        }

        Rectf item_clipper(item_rect.getIntersection(items_area));
        createRenderGeometryAndAddToItemView(tree_view, item->d_renderedText, item_rect,
            tree_view->getEffectiveFont(), &tree_view->getTextColourRect(), &item_clipper, item->d_isSelected);

        item_pos.y += std::max(size.d_height, d_subtreeExpanderImagerySize.d_height);

        if (item->d_renderedChildren.empty())
            continue;

        item_pos.x += indent;

        if (item->d_subtreeIsExpanded)
            renderTreeItem(tree_view, items_area, item_pos, item, depth + 1);

        item_pos.x -= indent;
    }
}

//----------------------------------------------------------------------------//
static Sizef getImagerySize(const ImagerySection& section)
{
    //!!!TODO: handle more than 1 imagerycomponent
    return section.getImageryComponents().front().getImage()->getRenderedSize();
}

//----------------------------------------------------------------------------//
void FalagardTreeView::onLookNFeelAssigned()
{
    const WidgetLookFeel& wlf = getLookNFeel();
    d_subtreeExpanderImagery = &wlf.getImagerySection("SubtreeExpander");
    d_subtreeCollapserImagery = &wlf.getImagerySection("SubtreeCollapser");

    Sizef open_size = getImagerySize(*d_subtreeExpanderImagery);
    Sizef close_size = getImagerySize(*d_subtreeCollapserImagery);
    d_subtreeExpanderImagerySize = Sizef(
        (open_size.d_width + close_size.d_width) / 2.0f + getView()->getSubtreeExpanderMargin(),
        (open_size.d_height + close_size.d_height) / 2.0f + getView()->getSubtreeExpanderMargin());
}

//----------------------------------------------------------------------------//
Sizef FalagardTreeView::getSubtreeExpanderSize(void) const
{
    return d_subtreeExpanderImagerySize;
}

//----------------------------------------------------------------------------//
Rectf FalagardTreeView::getViewRenderArea(void) const
{
    return ItemViewRenderer::getViewRenderArea(getView());
}

//----------------------------------------------------------------------------//
float FalagardTreeView::getSubtreeExpanderXIndent(int depth) const
{
    return depth * (
        d_subtreeExpanderImagerySize.d_width +
        getView()->getSubtreeExpanderMargin() * 2);
}

//----------------------------------------------------------------------------//
TreeView* FalagardTreeView::getView() const
{
    return static_cast<TreeView*>(d_window);
}

//----------------------------------------------------------------------------//
void FalagardTreeView::resizeViewToContent(bool fit_width, bool fit_height) const
{
    ItemViewRenderer::resizeViewToContent(getView(), fit_width, fit_height);
}

}
