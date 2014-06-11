/***********************************************************************
    filename:   TreeView.h
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

    Child Widgets:
        Scrollbar based widget with name suffix "__auto_vscrollbar__"
        Scrollbar based widget with name suffix "__auto_hscrollbar__"
*/
class COREWRSET_API FalagardTreeView : public ItemViewRenderer
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

    void render();

private:
    void renderTreeItem(TreeView* tree_view, const Rectf& items_area,
        Vector2f& item_pos, TreeViewItemRenderingState& item_state);
};

}

#endif
