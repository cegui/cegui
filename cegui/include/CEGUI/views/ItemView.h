/***********************************************************************
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Interface of the base class for all item model-based views.
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
#ifndef _CEGUIItemView_h_
#define _CEGUIItemView_h_

#include "CEGUI/Window.h"
#include "CEGUI/WindowRenderer.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/views/ItemModel.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class ItemView;

/*!
\brief
    This renderer interface provides data for the views to aid in the rendering
    process.
*/
class CEGUIEXPORT ItemViewWindowRenderer : public WindowRenderer
{
public:
    ItemViewWindowRenderer(const String& name);

    /*!
    \brief
        Returns a Rectf object describing, in unclipped pixels, the window
        relative area that is to be used for rendering the view.
    */
    virtual Rectf getViewRenderArea(void) const = 0;

    /*!
    \brief
        Resize the view such that its content can be displayed without needing
        scrollbars if there is enough space, otherwise make the view as
        large as possible (without moving it).
    */
    virtual void resizeViewToContent(bool fit_width, bool fit_height) const = 0;

    virtual ItemView* getView() const;
};

/*!
\brief
    This enumeration controls the \b display mode of the scrollbar, that is,
    it's visibility. This does not affect the scrollbar behaviour. For example,
    even if the scrollbar is ScrollbarDisplayMode::Hidden, one can still scroll it if the size
    allows it (content is bigger than the view).
*/
enum class ScrollbarDisplayMode : int
{
    //! The scrollbar will be shown always, even if the content is smaller than
    //! the view.
    Shown,
    //! The scrollbar will be hidden, even if the content is bigger than the
    //! view and scrolling is possible.
    Hidden,
    //! The scrollbar will be shown only if the underlining view's size is too
    //! small to contain its items.
    WhenNeeded
};

enum class ViewSortMode : int
{
    //! Items are not sorted, but shown in the same order as they are provided by the model.
    NoSorting,
    Ascending,
    Descending
};

template<>
class CEGUIEXPORT PropertyHelper<ScrollbarDisplayMode>
{
public:
    typedef ScrollbarDisplayMode return_type;
    typedef return_type safe_method_return_type;
    typedef ScrollbarDisplayMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();

    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

template<>
class CEGUIEXPORT PropertyHelper<ViewSortMode>
{
public:
    typedef ViewSortMode return_type;
    typedef return_type safe_method_return_type;
    typedef ViewSortMode pass_type;
    typedef String string_return_type;

    static const String& getDataTypeName();

    static return_type fromString(const String& str);
    static string_return_type toString(pass_type val);
};

/*!
\brief
    Stores the selection state of a ModelIndex. This is used to regenerate
    the new proper selection index when the model changes in any way (e.g.:
    new item, removed item).
*/
struct CEGUIEXPORT ModelIndexSelectionState
{
    ModelIndexSelectionState() : d_childId(0) {}

    ModelIndex d_parentIndex;
    size_t d_childId;

    ModelIndex d_selectedIndex;
};
typedef std::vector<ModelIndexSelectionState> SelectionStatesVector;

class CEGUIEXPORT ItemViewEventArgs : public WindowEventArgs
{
public:
    ItemViewEventArgs(ItemView* wnd, ModelIndex index = ModelIndex());

    //! The index affected by the event.
    ModelIndex d_index;
};

/*!
\brief
    Abstract base class for all view classes that use an ItemModel to provide
    the data to be rendered. In order for a view to properly display data,
    the setModel(ItemModel*) function should be called with an instance of the
    model.

    This class is mean to be inherited by a specific view before being used.
*/
class CEGUIEXPORT ItemView : public Window
{
public:
    ItemView(const String& type, const String& name);

    virtual ~ItemView() override;

    static const Colour DefaultTextColour;
    static const Colour DefaultSelectionColour;
    //! Widget name for the vertical scrollbar component.
    static const String VertScrollbarName;
    //! Widget name for the horizontal scrollbar component.
    static const String HorzScrollbarName;

    static const String EventVertScrollbarDisplayModeChanged;
    static const String EventHorzScrollbarDisplayModeChanged;
    static const String EventSelectionChanged;
    static const String EventMultiselectModeChanged;
    static const String EventSortModeChanged;
    //! Triggered when items are added, removed or when the view's item are cleared.
    static const String EventViewContentsChanged;

    //! Sets the ItemModel to be used inside this view.
    virtual void setModel(ItemModel* item_model);

    //! Returns the current ItemModel of this view.
    virtual ItemModel* getModel() const;

    /*!
    \brief
        Instructs this ItemView to prepare its rendering state for rendering.
        This is usually done by updating the rendering state if it got dirty
        in the meantime.
    */
    virtual void prepareForRender();

    //! Gets the colour used for rendering the text.
    const ColourRect& getTextColourRect() const;
    //! Sets the colour used for rendering the text.
    void setTextColourRect(const ColourRect& colour_rect);
    void setTextColour(Colour colour);

    //! Gets the colour used for highlighting the selection.
    const ColourRect& getSelectionColourRect() const;
    //! Sets the colour used for highlighting the selection.
    void setSelectionColourRect(const ColourRect& colour_rect);

    bool isDirty() const;
    //! Specifies whether this view requires processing before being able to render it.
    void setIsDirty(bool value);

    /*!
    \brief
       Invalidates this view by marking the rendering state as dirty. That means
       that the next call to prepareForRender() will have to reconstruct it's
       rendering state.

       This also calls the base Window::invalidate.
    */
    virtual void invalidateView(bool recursive);

    /*!
    \brief
       Gets the current state of the indices used for selection.

    \remark
        This vector's iterator might get invalidated in the case when items are
        removed from the underlying ItemModel while iterating over this vector.

        In the following example you can see a way of removing the selecting items:
        \code
            while(!view->getIndexSelectionStates().empty())
            {
                ModelIndexSelectionState& state = view->getIndexSelectionStates().back();
                view->getIndexSelectionStates().pop_back();
                // remove item from model
            }
        \endcode
    */
    const std::vector<ModelIndexSelectionState>& getIndexSelectionStates() const;

    /*!
    \brief
        Returns the ModelIndex of the item at the specified position.

    \param position
        The position is expected to be in screen coordinates - it will be converted
        to window coordinates internally.

    \return
        The ModelIndex for the position or a default-constructed ModelIndex
        if nothing was found at that position or if the position is outside
        the view's rendering area.
    */
    ModelIndex indexAt(const glm::vec2& position);
    virtual ModelIndex indexAtLocal(const glm::vec2& localPos) = 0;

    /*!
    \brief
        Sets the item specified by the \a index as the currently selected one.

    \return
        True if the index has been successfully selected, false otherwise.
    */
    virtual bool setSelectedIndex(const ModelIndex& index);
    virtual bool setIndexSelectionState(const ModelIndex& index, bool selected);

    virtual bool isIndexSelected(const ModelIndex& index) const;

    /*!
    \brief
        Ensures that the item specified by the \a index is visible by setting
        the proper the vertical and horizontal scrollbars' position.
    */
    virtual void ensureIndexIsVisible(const ModelIndex& index);

    //! Clears all selected indices.
    void clearSelections();

    void setSelectionBrushImage(const Image* image);
    /*!
    \brief
        Sets the image represented by the specified \name as the selection brush.

        This call is the same as:
        \code{.cpp}
        Image* img = ImageManager::getSingleton().getImage(name);
        view->setSelectionBrushImage(img);
        \endcode
    */
    void setSelectionBrushImage(const String& name);
    const Image* getSelectionBrushImage(void) const;

    /*!
    \brief
        Returns a pointer to the vertical Scrollbar component widget for this view.

    \exception UnknownObjectException
        Thrown if the vertical Scrollbar component does not exist.
    */
    Scrollbar* getVertScrollbar() const;

    void setVertScrollbarDisplayMode(ScrollbarDisplayMode mode);
    ScrollbarDisplayMode getVertScrollbarDisplayMode() const;

    /*!
    \brief
        Returns a pointer to the horizontal Scrollbar component widget for this view.

    \exception UnknownObjectException
        Thrown if the horizontal Scrollbar component does not exist.
    */
    Scrollbar* getHorzScrollbar() const;

    void setHorzScrollbarDisplayMode(ScrollbarDisplayMode mode);
    ScrollbarDisplayMode getHorzScrollbarDisplayMode() const;

    /*!
    \brief
        Specifies whether the view will vertically auto-resize to match the
        content's height or not. This happens when the parent size is changed,
        items are added or removed.
    */
    void setAutoResizeHeightEnabled(bool enabled);
    bool isAutoResizeHeightEnabled() const;

    /*!
    \brief
        Specifies whether the view will horizontally auto-resize to match the
        content's height or not. This happens when the parent size is changed,
        items are added or removed.
    */
    void setAutoResizeWidthEnabled(bool enabled);
    bool isAutoResizeWidthEnabled() const;

    //! Specifies whether this view should show tooltips for its items or not.
    bool isItemTooltipsEnabled() const;
    void setItemTooltipsEnabled(bool enabled);

    //! Specifies whether one can select multiple items in the view or not.
    bool isMultiSelectEnabled() const;
    void setMultiSelectEnabled(bool enabled);

    ViewSortMode getSortMode() const;
    //! Setting a new sorting mode will trigger the instant sorting of this view.
    void setSortMode(ViewSortMode sort_mode);

    //! Returns the width of the rendered contents.
    float getRenderedMaxWidth() const;
    //! Returns the height of the rendered contents.
    float getRenderedTotalHeight() const;

protected:
    ItemModel* d_itemModel;
    ColourRect d_textColourRect;
    ColourRect d_selectionColourRect;
    bool d_isDirty;
    bool d_needsFullRender;
    std::vector<ModelIndexSelectionState> d_indexSelectionStates;
    ModelIndex d_lastSelectedIndex;
    ModelIndex d_lastHoveredIndex;
    const Image* d_selectionBrush;
    ScrollbarDisplayMode d_vertScrollbarDisplayMode;
    ScrollbarDisplayMode d_horzScrollbarDisplayMode;
    bool d_isItemTooltipsEnabled;
    bool d_isMultiSelectEnabled;
    ViewSortMode d_sortMode;
    bool d_isAutoResizeHeightEnabled;
    bool d_isAutoResizeWidthEnabled;

    //TODO: move this into the renderer instead?
    float d_renderedMaxWidth;
    float d_renderedTotalHeight;

    //!!!FIXME: remove duplication with TextComponent! And maybe move into renderer?!
    //! helper to get a rendered string parser for the current window
    TextParser* getTextParser() const;

    void addItemViewProperties();
    virtual void updateScrollbars();
    void updateScrollbar(Scrollbar* scrollbar, float available_area,
        float rendered_area, ScrollbarDisplayMode display_mode);
    virtual ItemViewWindowRenderer* getViewRenderer();
    void updateScrollbarDisplayMode(ScrollbarDisplayMode& target_mode,
        ScrollbarDisplayMode new_mode, String change_event);

    void initialiseComponents() override;

    virtual bool onChildrenWillBeAdded(const EventArgs& args);
    virtual bool onChildrenAdded(const EventArgs& args);
    virtual bool onChildrenWillBeRemoved(const EventArgs& args);
    virtual bool onChildrenRemoved(const EventArgs& args);
    virtual bool onChildrenDataWillChange(const EventArgs& args);
    virtual bool onChildrenDataChanged(const EventArgs& args);

    virtual bool onScrollPositionChanged(const EventArgs& args);
    virtual void onSelectionChanged(ItemViewEventArgs& args);
    virtual void onMultiselectModeChanged(WindowEventArgs& args);
    virtual void onSortModeChanged(WindowEventArgs& args);
    virtual void onViewContentsChanged(WindowEventArgs& args);

    void onScroll(ScrollEventArgs& e) override;
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onCursorMove(CursorMoveEventArgs& e) override;
    void onKeyDown(KeyEventArgs& e) override;

    virtual void onSized(ElementEventArgs& e) override;
    virtual void onFontChanged(WindowEventArgs& e) override;
    virtual void onTargetSurfaceChanged(RenderingSurface* newSurface) override;

    Event::Connection d_eventChildrenWillBeAddedConnection;
    Event::Connection d_eventChildrenAddedConnection;
    Event::Connection d_eventChildrenWillBeRemovedConnection;
    Event::Connection d_eventChildrenRemovedConnection;
    Event::Connection d_eventChildrenDataWillChangeConnection;
    Event::Connection d_eventChildrenDataChangedConnection;
    void connectToModelEvents(ItemModel* d_itemModel);
    void disconnectModelEvents();

    int getSelectedIndexPosition(const ModelIndex& index) const;
    virtual bool handleSelection(const glm::vec2& localPos, bool should_select,
        bool is_cumulative, bool is_range);
    virtual bool handleSelection(const ModelIndex& index, bool should_select,
        bool is_cumulative, bool is_range);

    virtual void resortView() = 0;

    //! Returns the Rectf that contains the specified \a index.
    virtual Rectf getIndexRect(const ModelIndex& index) = 0;

    void resizeToContent();
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
