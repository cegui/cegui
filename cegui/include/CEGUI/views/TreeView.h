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

struct CEGUIEXPORT TreeViewItemRenderingState
{
    std::vector<TreeViewItemRenderingState> d_renderedChildren;
    //! This represents the total children count this item has, even if not rendered yet.
    //! This is the case when this node has not been opened/expanded yet.
    size_t d_totalChildCount;

    RenderedString d_string;
    Sizef d_size;
    bool d_isSelected;
    ModelIndex d_parentIndex;
    size_t d_childId;
    bool d_subtreeIsOpen;

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

private:
    TreeViewItemRenderingState d_rootItemState;

    TreeViewItemRenderingState computeRenderingStateForIndex(
        const ModelIndex& index, bool is_root, float& rendered_max_width,
        float& rendered_total_height);
    ModelIndex indexAtRecursive(TreeViewItemRenderingState& item, float& cur_height,
        const Vector2f& window_position);
};

};

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
