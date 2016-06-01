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
#ifndef _FalTreeView_h_
#define _FalTreeView_h_

#include "CEGUI/WindowRendererSets/Core/ItemViewRenderer.h"
#include "CEGUI/views/TreeView.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    TreeView class for the FalagardBase module.

    This class requires LookNFeel to be assigned.
    The LookNFeel should provide the following:

    States:
        - Enabled
        - Disabled

    Named Areas:
        - ItemRenderingArea
        - ItemRenderingAreaHScroll
        - ItemRenderingAreaVScroll
        - ItemRenderingAreaHVScroll

          OR

        - ItemRenderArea
        - ItemRenderAreaHScroll
        - ItemRenderAreaVScroll
        - ItemRenderAreaHVScroll

    Imagery sections:
        - SubtreeExpander (shown when the subtree is collapsed)
        - SubtreeCollapser (shown when the subtree is expanded)

    Child Widgets:
        Scrollbar based widget with name suffix "__auto_vscrollbar__"
        Scrollbar based widget with name suffix "__auto_hscrollbar__"
*/
class COREWRSET_API FalagardTreeView :
    public TreeViewWindowRenderer,
    public ItemViewRenderer
{
public:
    //! Type name for this widget.
    static const String TypeName;
    /*!
    \brief
        Constructor for the TreeView Falagard class.

    \param type
        The name of this renderer's factory.
    */
    FalagardTreeView(const String& type);

    void createRenderGeometry() override;

    Sizef getSubtreeExpanderSize(void) const override;
    float getSubtreeExpanderXIndent(int depth) const override;
    Rectf getViewRenderArea(void) const override;
    inline TreeView* getView() const override;
    void resizeViewToContent(bool fit_width, bool fit_height) const override;

protected:
    void onLookNFeelAssigned() override;

private:
    void renderTreeItem(TreeView* tree_view, const Rectf& items_area,
        glm::vec2& item_pos, const TreeViewItemRenderingState* item_to_render,
        size_t depth);

    const ImagerySection* d_subtreeExpanderImagery;
    const ImagerySection* d_subtreeCollapserImagery;
    Sizef d_subtreeExpanderImagerySize;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
