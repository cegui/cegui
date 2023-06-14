/***********************************************************************
    created:    Fri Jun 06 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Interface for a view that displays a tree of model items.
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
#ifndef _CEGUITreeView_h_
#define _CEGUITreeView_h_

#include "CEGUI/views/ItemView.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/InputEvent.h"
#include "CEGUI/text/RenderedText.h"
#include <vector>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    This renderer provides extra TreeView-specific rendering information.
*/
class CEGUIEXPORT TreeViewWindowRenderer : public ItemViewWindowRenderer
{
public:
    TreeViewWindowRenderer(const String& type);

    /*!
    \brief
        Returns a Size object describing, in unclipped pixels, the size of the
        expander that opens (expands) or closes a subtree. This includes
        any potential padding around it.
    */
    virtual Sizef getSubtreeExpanderSize() const = 0;

    /*!
    \brief
        Returns the horizontal (X-axis) indentation size for the subtree expander.
        The indent specifies the X-coordinate where the expander is positioned.

    \param depth
        The depth of the item for which to compute the actual indentation.
        Depth 0 is the depth for the root's children items.
    */
    virtual float getSubtreeExpanderXIndent(int depth) const = 0;
};

/*!
\brief
    This struct represents the internal rendering state of the TreeView. It should
    not be used to manipulate the TreeView or its items unless a TreeView's
    method requires it. This struct is exposed only because it's cheaper to use
    this for specific operations rather than compute it based of a ModelIndex
    each request.

    Access to the root state is done via TreeView::getRootItemState().
*/
class CEGUIEXPORT TreeViewItemRenderingState
{
public:

    TreeViewItemRenderingState(TreeView* attached_tree_view);
    TreeViewItemRenderingState(const TreeViewItemRenderingState&) = delete;
    TreeViewItemRenderingState(TreeViewItemRenderingState&&) = default;

    TreeViewItemRenderingState& operator =(const TreeViewItemRenderingState&) = delete;
    TreeViewItemRenderingState& operator =(TreeViewItemRenderingState&&) = default;

    bool operator <(const TreeViewItemRenderingState& other) const;
    bool operator >(const TreeViewItemRenderingState& other) const;

    //! These children are rendered via the renderer. If sorting is enabled,
    //! this vector will be sorted.
    std::vector<TreeViewItemRenderingState*> d_renderedChildren;

    //! This represents the total children count this item has, even if not rendered yet.
    //! This is the case when this node has not been opened/expanded yet.
    size_t d_totalChildCount;

    String d_text;
    //! The name of the image that represents the icon
    String d_icon;
    RenderedText d_renderedText;
    Sizef d_size;
    bool d_isSelected;
    ModelIndex d_parentIndex;
    size_t d_childId;
    bool d_subtreeIsExpanded;
    int d_nestedLevel;

    TreeView* d_attachedTreeView;

protected:
    //! Holds the unsorted children on which all tree operations are done.
    std::vector<TreeViewItemRenderingState> d_children;

    void sortChildren();

    friend class TreeView;
};

/*!
\brief
    View that displays items in a tree fashion. A list-only ItemModel can be
    provided as well as the ItemModel of this view.
*/
class CEGUIEXPORT TreeView : public ItemView
{
public:
    //! Window factory name
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;
    static const String EventSubtreeExpanded;
    static const String EventSubtreeCollapsed;

    TreeView(const String& type, const String& name);
    virtual ~TreeView();

    const TreeViewItemRenderingState& getRootItemState() const;

    void prepareForRender() override;

    ModelIndex indexAtLocal(const glm::vec2& localPos) override;

    float getSubtreeExpanderMargin() const;
    //! Allows setting extra margin around the subtree expander component.
    void setSubtreeExpanderMargin(float value);

    TreeViewItemRenderingState* getTreeViewItemForIndex(const ModelIndex& index);

    /*!
    \brief
        Triggers the \b recursive expansion of all subtrees in the TreeView.
        This will raise the EventSubtreeExpanded event for all non-expanded
        subtrees.

        This function invokes the expandSubtreeRecursive(TreeViewItemRenderingState)
        recursively on each item.
    */
    void expandAllSubtrees();
    void expandSubtreeRecursive(TreeViewItemRenderingState& item);

    /*!
    \brief
        Toggles the expanded/collapsed state of the specified tree item.
        This will raise the EventSubtreeExpanded or EventSubtreeCollapsed
        events based on the current item's state.
    */
    void toggleSubtree(TreeViewItemRenderingState& item);

protected:
    using ItemView::handleSelection;

    TreeViewWindowRenderer* getViewRenderer() override;
    bool handleSelection(const glm::vec2& localPos, bool should_select,
        bool is_cumulative, bool is_range) override;

    bool onChildrenRemoved(const EventArgs& args) override;
    bool onChildrenAdded(const EventArgs& args) override;

    virtual void onSubtreeExpanded(ItemViewEventArgs& args);
    virtual void onSubtreeCollapsed(ItemViewEventArgs& args);

private:
    typedef void (TreeView::*TreeViewItemAction)(
        TreeViewItemRenderingState& item, bool toggles_expander);

    TreeViewItemRenderingState d_rootItemState;
    TreeViewItemRenderingState computeRenderingStateForIndex(
        const ModelIndex& parent_index, size_t child_id, size_t nested_level,
        float& rendered_max_width, float& rendered_total_height);

    float d_subtreeExpanderMargin;

    void addTreeViewProperties();

    void computeRenderedChildrenForItem(TreeViewItemRenderingState &item,
        const ModelIndex& index, float& rendered_max_width, float& rendered_total_height);

    void updateRenderingStateForItem(TreeViewItemRenderingState& item,
        float& rendered_max_width, float& rendered_total_height);

    void fillRenderingState(TreeViewItemRenderingState& state, const ModelIndex& index, float& rendered_max_width, float& rendered_total_height);

    ModelIndex indexAtWithAction(const glm::vec2& localPos, TreeViewItemAction action);
    ModelIndex indexAtRecursive(TreeViewItemRenderingState& item, float& cur_height,
        const glm::vec2& window_position, bool& handled, TreeViewItemAction action);

    void clearItemRenderedChildren(TreeViewItemRenderingState& item, float& renderedTotalHeight);
    void handleSelectionAction(TreeViewItemRenderingState& item, bool toggles_expander);

    void noopAction(TreeViewItemRenderingState& /*item*/, bool /*toggles_expander*/) {}

    void resortView() override;

    Rectf getIndexRect(const ModelIndex& index) override;
};

};

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
