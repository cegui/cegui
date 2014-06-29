/***********************************************************************
    filename:   TreeView.h
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
#include <vector>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

class CEGUIEXPORT TreeViewWindowRenderer : public ItemViewWindowRenderer
{
public:
    TreeViewWindowRenderer(const String& type);

    /*!
    \brief
        Returns a Size object describing, in unclipped pixels, the size of the
        expander that opens (expands) or closes a subtree.
    */
    virtual Sizef getSubtreeExpanderSize(void) const = 0;
};

struct CEGUIEXPORT TreeViewItemRenderingState
{
    std::vector<TreeViewItemRenderingState> d_renderedChildren;
    //! This represents the total children count this item has, even if not rendered yet.
    //! This is the case when this node has not been opened/expanded yet.
    size_t d_totalChildCount;

    String d_text;
    RenderedString d_string;
    Sizef d_size;
    bool d_isSelected;
    ModelIndex d_parentIndex;
    size_t d_childId;
    bool d_subtreeIsExpanded;

    TreeViewItemRenderingState();
};

/*!
\brief
    View that displays items in a tree fashion.
*/
class CEGUIEXPORT TreeView : public ItemView
{
public:
    //! Window factory name
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;

    TreeView(const String& type, const String& name);

    virtual ~TreeView();

    const TreeViewItemRenderingState& getRootItemState() const;
    virtual void prepareForRender();
    virtual ModelIndex indexAt(const Vector2f& position);

    TreeViewItemRenderingState* getTreeViewItemForIndex(const ModelIndex& index);

protected:
    virtual TreeViewWindowRenderer* getViewRenderer();
    virtual bool handleSelection(const Vector2f& position, bool should_select,
        bool is_cumulative, bool is_range);
    virtual bool handleSelection(const ModelIndex& index, bool should_select,
        bool is_cumulative, bool is_range);

    virtual bool onChildrenRemoved(const EventArgs& args);
    virtual bool onChildrenAdded(const EventArgs& args);

private:
    typedef void (TreeView::*TreeViewItemAction)(
        TreeViewItemRenderingState& item, bool toggles_expander);
    typedef std::vector<TreeViewItemRenderingState> ItemStateVector;

    TreeViewItemRenderingState d_rootItemState;

    TreeViewItemRenderingState computeRenderingStateForIndex(
        const ModelIndex& parent_index, size_t child_id,
        float& rendered_max_width, float& rendered_total_height);

    void computeRenderedChildrenForItem(TreeViewItemRenderingState &item,
        const ModelIndex& index, float& rendered_max_width, float& rendered_total_height);

    void updateRenderingStateForItem(TreeViewItemRenderingState& item,
        float& rendered_max_width, float& rendered_total_height);

    void fillRenderingState(TreeViewItemRenderingState& state, const ModelIndex& index, float& rendered_max_width, float& rendered_total_height);

    ModelIndex indexAtWithAction(const Vector2f& position, TreeViewItemAction action);
    ModelIndex indexAtRecursive(TreeViewItemRenderingState& item, float& cur_height,
        const Vector2f& window_position, bool& handled, TreeViewItemAction action);

    void toggleSubtree(TreeViewItemRenderingState& item);
    void clearItemRenderedChildren(TreeViewItemRenderingState& item, float& renderedTotalHeight);
    void handleSelectionAction(TreeViewItemRenderingState& item, bool toggles_expander);

    void noopAction(TreeViewItemRenderingState& item, bool toggles_expander) {}
};

};

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
