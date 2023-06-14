/***********************************************************************
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Interface for a view that displays a list of model items.
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
#ifndef _CEGUIListView_h_
#define _CEGUIListView_h_

#include "CEGUI/views/ItemView.h"
#include "CEGUI/falagard/Enums.h"
#include "CEGUI/text/RenderedText.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

/*!
\brief
    This internal struct represents the rendering state of an item that is part
    of the ListView. This is recomputed each time the ListView is invalidated.

    This struct is meant to be used as a caching mechanism so that the ItemModel
    is not queries each time rendering is done. That means, the users of ListView
    shouldn't use this struct for interacting with the list, but rather use the
    attached ItemModel.
*/
struct CEGUIEXPORT ListViewItemRenderingState final
{
    ListViewItemRenderingState(ListView* list_view);
    ListViewItemRenderingState(const ListViewItemRenderingState&) = delete;
    ListViewItemRenderingState(ListViewItemRenderingState&&) noexcept;
    ~ListViewItemRenderingState();

    ListViewItemRenderingState& operator =(const ListViewItemRenderingState&) = delete;
    ListViewItemRenderingState& operator =(ListViewItemRenderingState&&) noexcept;

    bool operator <(const ListViewItemRenderingState& other) const;
    bool operator >(const ListViewItemRenderingState& other) const;

    RenderedText d_renderedText;
    String d_icon; //!< The name of the image that represents the icon
    Sizef d_size;
    ModelIndex d_index;
    String d_text;
    ListView* d_attachedListView = nullptr;
    bool d_isSelected = false;
};

/*!
\brief
    View that displays items in a listed fashion.

    ItemModel%s that use a tree structure can be still rendered by this view,
    but only the children that are direct children of the ItemModel::getRootIndex()
    are taken into considerations. That is, you cannot use a list to render
    arbitrary list of children, unless a specific ItemModel implementation that
    provides that is specified.
*/
class CEGUIEXPORT ListView : public ItemView
{
public:

    //! Window factory name
    static const String WidgetTypeName;
    //! Namespace for global events
    static const String EventNamespace;

    ListView(const String& type, const String& name);
    virtual ~ListView() override;

    const std::vector<ListViewItemRenderingState*>& getItems() const;

    void prepareForRender() override;

    ModelIndex indexAtLocal(const glm::vec2& localPos) override;

    HorizontalTextFormatting getHorizontalFormatting() const { return d_horzFormatting; }
    void setHorizontalFormatting(HorizontalTextFormatting h_fmt);

    bool isWordWrapEnabled() const { return d_wordWrap; }
    void setWordWrapEnabled(bool wrap);

protected:

    bool onChildrenAdded(const EventArgs& args) override;
    bool onChildrenRemoved(const EventArgs& args) override;

    void resortListView();
    void resortView() override;

    //! Updates the rendering state for the specified \a item using the specified
    //! \a index as the data source.
    void updateItem(ListViewItemRenderingState& item, ModelIndex index,
        float& max_width, float& total_height);

    Rectf getIndexRect(const ModelIndex& index) override;

    std::vector<ListViewItemRenderingState> d_items;
    std::vector<ListViewItemRenderingState*> d_sortedItems;

    HorizontalTextFormatting d_horzFormatting = HorizontalTextFormatting::LeftAligned;
    bool d_wordWrap = false;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
