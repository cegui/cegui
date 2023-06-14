/***********************************************************************
    created:    Sat May 24 2014
    author:     Timotei Dolean <timotei21@gmail.com>

    purpose:    Implementation of the base class for all item model-based views.
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
#include "CEGUI/views/ItemView.h"
#include "CEGUI/System.h" // FIXME: needed for duplicated code from TextComponent, remove when fixed!
#include "CEGUI/ImageManager.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/widgets/Scrollbar.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
ItemViewWindowRenderer::ItemViewWindowRenderer(const String& name) :
    WindowRenderer(name, "ItemView")
{
}

//----------------------------------------------------------------------------//
ItemView* ItemViewWindowRenderer::getView() const
{
    return static_cast<ItemView*>(d_window);
}

//----------------------------------------------------------------------------//
const String& PropertyHelper<ScrollbarDisplayMode>::getDataTypeName()
{
    static String type("ScrollbarDisplayMode");

    return type;
}

//----------------------------------------------------------------------------//
PropertyHelper<ScrollbarDisplayMode>::return_type
PropertyHelper<ScrollbarDisplayMode>::fromString(const String& str)
{
    if (str == "Shown") return ScrollbarDisplayMode::Shown;
    if (str == "Hidden") return ScrollbarDisplayMode::Hidden;

    // default
    return ScrollbarDisplayMode::WhenNeeded;
}

//----------------------------------------------------------------------------//
PropertyHelper<ScrollbarDisplayMode>::string_return_type
PropertyHelper<ScrollbarDisplayMode>::toString(pass_type val)
{
    switch(val)
    {
    case ScrollbarDisplayMode::Shown: return "Shown";
    case ScrollbarDisplayMode::Hidden: return "Hidden";
    case ScrollbarDisplayMode::WhenNeeded: return "WhenNeeded";
    default: return "InvalidDisplayMode";
    }
}

//----------------------------------------------------------------------------//
const String& PropertyHelper<ViewSortMode>::getDataTypeName()
{
    static String type("ViewSortMode");

    return type;
}

//----------------------------------------------------------------------------//
PropertyHelper<ViewSortMode>::return_type
PropertyHelper<ViewSortMode>::fromString(const String& str)
{
    if (str == "Ascending") return ViewSortMode::Ascending;
    if (str == "Descending") return ViewSortMode::Descending;

    // default
    return ViewSortMode::NoSorting;
}

//----------------------------------------------------------------------------//
PropertyHelper<ViewSortMode>::string_return_type
PropertyHelper<ViewSortMode>::toString(pass_type val)
{
    switch (val)
    {
    case ViewSortMode::Ascending: return "Ascending";
    case ViewSortMode::Descending: return "Descending";
    default: return "NoSorting";
    }
}

//----------------------------------------------------------------------------//
const Colour ItemView::DefaultTextColour = 0xFFFFFFFF;
const Colour ItemView::DefaultSelectionColour = Colour(0xFF4444AA);
const String ItemView::HorzScrollbarName("__auto_hscrollbar__");
const String ItemView::VertScrollbarName("__auto_vscrollbar__");
const String ItemView::EventVertScrollbarDisplayModeChanged("VertScrollbarDisplayModeChanged");
const String ItemView::EventHorzScrollbarDisplayModeChanged("HorzScrollbarDisplayModeChanged");
const String ItemView::EventSelectionChanged("SelectionChanged");
const String ItemView::EventMultiselectModeChanged("MultiselectModeChanged");
const String ItemView::EventSortModeChanged("SortModeChanged");
const String ItemView::EventViewContentsChanged("ViewContentsChanged");

//----------------------------------------------------------------------------//
ItemView::ItemView(const String& type, const String& name) :
    Window(type, name),
    d_itemModel(nullptr),
    d_textColourRect(ColourRect(DefaultTextColour)),
    d_selectionColourRect(ColourRect(DefaultSelectionColour)),
    d_isDirty(true),
    d_needsFullRender(true),
    d_lastSelectedIndex(nullptr),
    d_selectionBrush(nullptr),
    d_vertScrollbarDisplayMode(ScrollbarDisplayMode::WhenNeeded),
    d_horzScrollbarDisplayMode(ScrollbarDisplayMode::WhenNeeded),
    d_isItemTooltipsEnabled(false),
    d_isMultiSelectEnabled(false),
    d_sortMode(ViewSortMode::NoSorting),
    d_isAutoResizeHeightEnabled(false),
    d_isAutoResizeWidthEnabled(false),
    d_renderedMaxWidth(0),
    d_renderedTotalHeight(0),
    d_eventChildrenAddedConnection(nullptr),
    d_eventChildrenRemovedConnection(nullptr)
{
    addItemViewProperties();
}

//----------------------------------------------------------------------------//
ItemView::~ItemView()
{
    disconnectModelEvents();
}

//----------------------------------------------------------------------------//
TextParser* ItemView::getTextParser() const
{
    if (auto renderer = getWindowRenderer())
    {
        // Don't parse text if it is explicitly disabled
        if (!renderer->isTextParsingEnabled())
            return nullptr;

        // Prefer a custom parser assigned to this Window
        if (auto parser = renderer->getTextParser())
            return parser;
    }

    // Otherwise use a global default parser
    return CEGUI::System::getSingleton().getDefaultTextParser();
}

//----------------------------------------------------------------------------//
void ItemView::addItemViewProperties()
{
    const String& propertyOrigin = "ItemView";

    CEGUI_DEFINE_PROPERTY(ItemView, Image*,
        "SelectionBrushImage",
        "Property to get/set the selection brush image for the item view. "
        "Value should be \"set:[imageset name] image:[image name]\".",
        &ItemView::setSelectionBrushImage, &ItemView::getSelectionBrushImage, nullptr
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ColourRect,
        "SelectionColour",
        "Property to get/set the selection color for the item view. "
        "Value should be a \"ColourRect\".",
        &ItemView::setSelectionColourRect, &ItemView::getSelectionColourRect, DefaultSelectionColour
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ScrollbarDisplayMode,
        "VertScrollbarDisplayMode",
        "Property to get/set the display mode of the vertical scroll bar of the "
        "item view. Value can be \"Shown\", \"Hidden\" or \"WhenNeeded\".",
        &ItemView::setVertScrollbarDisplayMode,
        &ItemView::getVertScrollbarDisplayMode, ScrollbarDisplayMode::WhenNeeded
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ScrollbarDisplayMode,
        "HorzScrollbarDisplayMode",
        "Property to get/set the display mode of the horizontal scroll bar of "
        "the item view. Value can be \"Shown\", \"Hidden\" or \"WhenNeeded\".",
        &ItemView::setHorzScrollbarDisplayMode,
        &ItemView::getHorzScrollbarDisplayMode, ScrollbarDisplayMode::WhenNeeded
        )

    CEGUI_DEFINE_PROPERTY(ItemView, bool,
        "ItemTooltips",
        "Property to access the show item tooltips setting of the item view. "
        "Value is either \"True\" or \"False\".",
        &ItemView::setItemTooltipsEnabled, &ItemView::isItemTooltipsEnabled, false
        )

    CEGUI_DEFINE_PROPERTY(ItemView, bool,
        "MultiSelect",
        "Property to get/set the multi-select setting of the item view. "
        "Value is either \"True\" or \"False\".",
        &ItemView::setMultiSelectEnabled, &ItemView::isMultiSelectEnabled, false
        )

    CEGUI_DEFINE_PROPERTY(ItemView, ViewSortMode,
        "SortMode",
        "Property to get/set how the item view is sorting its items. "
        "Value is either \"NoSorting\", \"Ascending\" or \"Descending\".",
        &ItemView::setSortMode, &ItemView::getSortMode, ViewSortMode::NoSorting
        )


    CEGUI_DEFINE_PROPERTY(ItemView, bool,
        "AutoSizeHeight",
        "Property to get/set whether the item view will vertically auto-size "
        "itself to fit its content. Value is either \"true\" or \"false\".",
        &ItemView::setAutoResizeHeightEnabled,
        &ItemView::isAutoResizeHeightEnabled, false
        )

    CEGUI_DEFINE_PROPERTY(ItemView, bool,
        "AutoSizeWidth",
        "Property to get/set whether the item view will vertically auto-size "
        "itself to fit its content. Value is either \"true\" or \"false\".",
        &ItemView::setAutoResizeWidthEnabled,
        &ItemView::isAutoResizeWidthEnabled, false
        )
}

//----------------------------------------------------------------------------//
void ItemView::initialiseComponents()
{
    getVertScrollbar()->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&ItemView::onScrollPositionChanged, this));
    getHorzScrollbar()->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&ItemView::onScrollPositionChanged, this));

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
void ItemView::setModel(ItemModel* item_model)
{
    if (item_model == d_itemModel)
        return;

    if (d_itemModel != nullptr)
    {
        disconnectModelEvents();
        d_lastSelectedIndex = ModelIndex(nullptr);
    }

    d_itemModel = item_model;

    connectToModelEvents(d_itemModel);
    d_indexSelectionStates.clear();
    d_needsFullRender = true;

    ItemViewEventArgs args(this);
    onSelectionChanged(args);
}

//----------------------------------------------------------------------------//
void ItemView::connectToModelEvents(ItemModel* d_itemModel)
{
    if (d_itemModel == nullptr)
        return;

    d_eventChildrenWillBeAddedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenWillBeAdded,
        &ItemView::onChildrenWillBeAdded, this);
    d_eventChildrenAddedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenAdded,
        &ItemView::onChildrenAdded, this);

    d_eventChildrenWillBeRemovedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenWillBeRemoved,
        &ItemView::onChildrenWillBeRemoved, this);
    d_eventChildrenRemovedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenRemoved,
        &ItemView::onChildrenRemoved, this);

    d_eventChildrenDataWillChangeConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenDataWillChange,
        &ItemView::onChildrenDataWillChange, this);
    d_eventChildrenDataChangedConnection = d_itemModel->subscribeEvent(
        ItemModel::EventChildrenDataChanged,
        &ItemView::onChildrenDataChanged, this);
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenWillBeAdded(const EventArgs&)
{
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenAdded(const EventArgs& args)
{
    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    for (SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
        itor != d_indexSelectionStates.end(); ++itor)
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId &&
            d_itemModel->areIndicesEqual(state.d_parentIndex, model_args.d_parentIndex))
        {
            state.d_childId += model_args.d_count;
            state.d_selectedIndex = d_itemModel->makeIndex(state.d_childId, state.d_parentIndex);
        }
    }

    invalidateView(false);
    WindowEventArgs evt_args(this);
    onViewContentsChanged(evt_args);
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenWillBeRemoved(const EventArgs& args)
{
    if (d_itemModel == nullptr)
        return false;

    const ModelEventArgs& model_args = static_cast<const ModelEventArgs&>(args);

    SelectionStatesVector::iterator itor = d_indexSelectionStates.begin();
    while (itor != d_indexSelectionStates.end())
    {
        ModelIndexSelectionState& state = *itor;

        if (state.d_childId >= model_args.d_startId &&
            state.d_childId <= model_args.d_startId + model_args.d_count)
        {
            if (d_itemModel->areIndicesEqual(d_lastSelectedIndex, state.d_selectedIndex))
                d_lastSelectedIndex = ModelIndex(nullptr);

            itor = d_indexSelectionStates.erase(itor);
        }
        else
        {
            ++itor;
        }
    }

    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenRemoved(const EventArgs&)
{

    ItemViewEventArgs wargs(this);
    onSelectionChanged(wargs);

    WindowEventArgs evt_args(this);
    onViewContentsChanged(evt_args);
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenDataWillChange(const EventArgs&)
{
    return true;
}

//----------------------------------------------------------------------------//
bool ItemView::onChildrenDataChanged(const EventArgs&)
{
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
void ItemView::onSelectionChanged(ItemViewEventArgs& args)
{
    invalidateView(false);
    fireEvent(EventSelectionChanged, args);
}

//----------------------------------------------------------------------------//
bool ItemView::onScrollPositionChanged(const EventArgs&)
{
    invalidateView(false);
    return true;
}

//----------------------------------------------------------------------------//
void ItemView::onMouseButtonDown(MouseButtonEventArgs& e)
{
    Window::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
    {
        if (e.d_modifiers.hasCtrl())
            handleSelection(e.d_localPos, true, d_isMultiSelectEnabled, false);
        else if (e.d_modifiers.hasShift())
            handleSelection(e.d_localPos, true, d_isMultiSelectEnabled, true);
        else
            handleSelection(e.d_localPos, true, false, false);
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void ItemView::onCursorMove(CursorMoveEventArgs& e)
{
    Window::onCursorMove(e);

    ++e.handled;

    if (!d_isItemTooltipsEnabled || !d_itemModel)
        return;

    ModelIndex index = indexAtLocal(e.d_localPos);
    if (d_itemModel->areIndicesEqual(index, d_lastHoveredIndex))
        return;

    d_lastHoveredIndex = index;

    setTooltipText(d_itemModel->isValidIndex(index) ? d_itemModel->getData(index, ItemDataRole::Tooltip) : "");

    if (d_guiContext)
        d_guiContext->positionTooltip();
}

//----------------------------------------------------------------------------//
void ItemView::onKeyDown(KeyEventArgs& e)
{
    Window::onKeyDown(e);

    if (e.d_key != Key::Scan::ArrowDown && e.d_key != Key::Scan::ArrowUp)
        return;

    ModelIndex parentIndex = d_itemModel->getRootIndex();
    ptrdiff_t lastSelectedChildId = -1;
    if (!d_indexSelectionStates.empty())
    {
        ModelIndexSelectionState lastSelection = d_indexSelectionStates.back();
        lastSelectedChildId = lastSelection.d_childId;
        parentIndex = lastSelection.d_parentIndex;
    }

    const size_t childrenCount = d_itemModel->getChildCount(parentIndex);
    if (childrenCount == 0)
        return;

    auto nextSelectedChildId = (e.d_key == Key::Scan::ArrowDown) ?
        std::min(lastSelectedChildId + 1, static_cast<ptrdiff_t>(childrenCount) - 1) :
        std::max<ptrdiff_t>(0, lastSelectedChildId - 1);

    if (nextSelectedChildId != -1 && nextSelectedChildId != lastSelectedChildId)
    {
        setSelectedIndex(d_itemModel->makeIndex(static_cast<size_t>(nextSelectedChildId), parentIndex));
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
static inline void disconnectIfNotNull(Event::Connection& connection)
{
    if (connection)
        connection->disconnect();
}

//----------------------------------------------------------------------------//
void ItemView::disconnectModelEvents()
{
    disconnectIfNotNull(d_eventChildrenWillBeAddedConnection);
    disconnectIfNotNull(d_eventChildrenAddedConnection);
    disconnectIfNotNull(d_eventChildrenWillBeRemovedConnection);
    disconnectIfNotNull(d_eventChildrenRemovedConnection);
    disconnectIfNotNull(d_eventChildrenDataWillChangeConnection);
    disconnectIfNotNull(d_eventChildrenDataChangedConnection);
}

//----------------------------------------------------------------------------//
void ItemView::invalidateView(bool recursive)
{
    //TODO: allow invalidation only of certain parts (e.g.: items/indices)
    updateScrollbars();
    resizeToContent();
    setIsDirty(true);
    invalidate(recursive);
}

//----------------------------------------------------------------------------//
ItemModel* ItemView::getModel() const
{
    return d_itemModel;
}

//----------------------------------------------------------------------------//
void ItemView::prepareForRender()
{
}

//----------------------------------------------------------------------------//
const ColourRect& ItemView::getTextColourRect() const
{
    return d_textColourRect;
}

//----------------------------------------------------------------------------//
void ItemView::setTextColourRect(const ColourRect& colour_rect)
{
    d_textColourRect = colour_rect;
}

//----------------------------------------------------------------------------//
void ItemView::setTextColour(Colour colour)
{
    ColourRect rect(colour);
    setTextColourRect(rect);
}

//----------------------------------------------------------------------------//
const ColourRect& ItemView::getSelectionColourRect() const
{
    return d_selectionColourRect;
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionColourRect(const ColourRect& colour_rect)
{
    d_selectionColourRect = colour_rect;
}

//----------------------------------------------------------------------------//
const std::vector<ModelIndexSelectionState>& ItemView::getIndexSelectionStates() const
{
    return d_indexSelectionStates;
}

//----------------------------------------------------------------------------//
ModelIndex ItemView::indexAt(const glm::vec2& position)
{
    return indexAtLocal(CoordConverter::screenToWindow(*this, position));
}

//----------------------------------------------------------------------------//
bool ItemView::isDirty() const
{
    return d_isDirty;
}

//----------------------------------------------------------------------------//
void ItemView::setIsDirty(bool value)
{
    d_isDirty = value;
}

//----------------------------------------------------------------------------//
int ItemView::getSelectedIndexPosition(const ModelIndex& index) const
{
    if (d_itemModel == nullptr)
        return false;

    for (size_t i = 0; i < d_indexSelectionStates.size(); ++i)
    {
        if (d_itemModel->areIndicesEqual(index, d_indexSelectionStates.at(i).d_selectedIndex))
            return static_cast<uint32_t>(i);
    }

    return -1;
}

//----------------------------------------------------------------------------//
bool ItemView::isIndexSelected(const ModelIndex& index) const
{
    return getSelectedIndexPosition(index) != -1;
}

//----------------------------------------------------------------------------//
bool ItemView::setSelectedIndex(const ModelIndex& index)
{
    // simple calls of this method shouldn't do cumulative selection
    return handleSelection(index, true, false, false);
}

//----------------------------------------------------------------------------//
bool ItemView::setIndexSelectionState(const ModelIndex& index, bool selected)
{
    return handleSelection(index, selected, d_isMultiSelectEnabled, false);
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionBrushImage(const String& name)
{
    setSelectionBrushImage(&ImageManager::getSingleton().get(name));
}

//----------------------------------------------------------------------------//
void ItemView::setSelectionBrushImage(const Image* image)
{
    d_selectionBrush = image;
    invalidateView(false);
}

//----------------------------------------------------------------------------//
const Image* ItemView::getSelectionBrushImage(void) const
{
    return d_selectionBrush;
}

//----------------------------------------------------------------------------//
Scrollbar* ItemView::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(VertScrollbarName));
}

//----------------------------------------------------------------------------//
Scrollbar* ItemView::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(getChild(HorzScrollbarName));
}

//----------------------------------------------------------------------------//
void ItemView::updateScrollbars()
{
    Rectf render_area = getViewRenderer()->getViewRenderArea();

    updateScrollbar(getVertScrollbar(), render_area.getHeight(),
        d_renderedTotalHeight, d_vertScrollbarDisplayMode);
    updateScrollbar(getHorzScrollbar(), render_area.getWidth(),
        d_renderedMaxWidth, d_horzScrollbarDisplayMode);
}

//----------------------------------------------------------------------------//
void ItemView::updateScrollbar(Scrollbar* scrollbar, float available_area,
    float rendered_area, ScrollbarDisplayMode display_mode)
{
    scrollbar->setDocumentSize(rendered_area);
    scrollbar->setPageSize(available_area);
    scrollbar->setStepSize(std::max(1.0f, rendered_area / 10.0f));
    scrollbar->setScrollPosition(scrollbar->getScrollPosition());

    if (display_mode == ScrollbarDisplayMode::Hidden)
    {
        scrollbar->hide();
        return;
    }

    if (display_mode == ScrollbarDisplayMode::Shown ||
        rendered_area > available_area)
    {
        scrollbar->show();
        return;
    }

    scrollbar->hide();
}

//----------------------------------------------------------------------------//
void ItemView::setVertScrollbarDisplayMode(ScrollbarDisplayMode mode)
{
    updateScrollbarDisplayMode(d_vertScrollbarDisplayMode, mode,
        EventVertScrollbarDisplayModeChanged);
}

//----------------------------------------------------------------------------//
ScrollbarDisplayMode ItemView::getVertScrollbarDisplayMode() const
{
    return d_vertScrollbarDisplayMode;
}

//----------------------------------------------------------------------------//
void ItemView::setHorzScrollbarDisplayMode(ScrollbarDisplayMode mode)
{
    updateScrollbarDisplayMode(d_horzScrollbarDisplayMode, mode,
        EventHorzScrollbarDisplayModeChanged);
}

//----------------------------------------------------------------------------//
ScrollbarDisplayMode ItemView::getHorzScrollbarDisplayMode() const
{
    return d_horzScrollbarDisplayMode;
}

//----------------------------------------------------------------------------//
ItemViewWindowRenderer* ItemView::getViewRenderer()
{
    if (d_windowRenderer == nullptr)
    {
        throw InvalidRequestException(
            "The view should have a window renderer attached!");
    }

    return static_cast<ItemViewWindowRenderer*>(d_windowRenderer);
}

//----------------------------------------------------------------------------//
void ItemView::updateScrollbarDisplayMode(ScrollbarDisplayMode& target_mode,
    ScrollbarDisplayMode new_mode, String change_event)
{
    if (target_mode == new_mode)
        return;

    target_mode = new_mode;

    updateScrollbars();
    invalidateView(false);
    WindowEventArgs args(this);
    fireEvent(change_event, args);
}

//----------------------------------------------------------------------------//
void ItemView::onScroll(ScrollEventArgs& e)
{
    Window::onScroll(e);

    if (Scrollbar::standardProcessing(getVertScrollbar(), getHorzScrollbar(), -e.d_delta, e.d_modifiers.hasAlt()))
        ++e.handled;
}

//----------------------------------------------------------------------------//
bool ItemView::isItemTooltipsEnabled() const
{
    return d_isItemTooltipsEnabled;
}

//----------------------------------------------------------------------------//
void ItemView::setItemTooltipsEnabled(bool enabled)
{
    d_isItemTooltipsEnabled = enabled;
}

//----------------------------------------------------------------------------//
bool ItemView::isMultiSelectEnabled() const
{
    return d_isMultiSelectEnabled;
}

//----------------------------------------------------------------------------//
void ItemView::setMultiSelectEnabled(bool enabled)
{
    if (d_isMultiSelectEnabled == enabled)
    {
        return;
    }

    d_isMultiSelectEnabled = enabled;

    // deselect others
    if (!d_isItemTooltipsEnabled && d_indexSelectionStates.size() > 1)
    {
        setIndexSelectionState(d_indexSelectionStates.front().d_selectedIndex, true);
    }

    WindowEventArgs args(this);
    onMultiselectModeChanged(args);
}

//----------------------------------------------------------------------------//
void ItemView::onMultiselectModeChanged(WindowEventArgs& args)
{
    fireEvent(EventMultiselectModeChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void ItemView::onSized(ElementEventArgs& e)
{
    Window::onSized(e);
    invalidateView(false);
}

//----------------------------------------------------------------------------//
void ItemView::onFontChanged(WindowEventArgs& e)
{
    Window::onFontChanged(e);
    resizeToContent();
}

//----------------------------------------------------------------------------//
// Context might change, we should update contents if it is valid
void ItemView::onTargetSurfaceChanged(RenderingSurface* newSurface)
{
    Window::onTargetSurfaceChanged(newSurface);
    if (d_guiContext)
    {
        performChildLayout(false, false);
        updateScrollbars();
        resizeToContent(); // call invalidateView(false) instead?
    }
}

//----------------------------------------------------------------------------//
bool ItemView::handleSelection(const glm::vec2& localPos, bool should_select,
    bool is_cumulative, bool is_range)
{
    return handleSelection(indexAtLocal(localPos), should_select, is_cumulative, is_range);
}

//----------------------------------------------------------------------------//
bool ItemView::handleSelection(const ModelIndex& index, bool should_select,
    bool is_cumulative, bool is_range)
{
    if (d_itemModel == nullptr ||
        !d_itemModel->isValidIndex(index))
        return false;

    int index_position = getSelectedIndexPosition(index);
    if (index_position != -1)
    {
        if (!should_select)
        {
            d_indexSelectionStates.erase(d_indexSelectionStates.begin() + index_position);

            ItemViewEventArgs args(this, index);
            onSelectionChanged(args);
            return true;
        }

        // if we select the node again, and we don't cumulate selection, we need
        // to make just that one be selected now
        if (is_cumulative)
            return true;
    }

    if (!is_cumulative)
        d_indexSelectionStates.clear();

    ModelIndex parent_index = d_itemModel->getParentIndex(index);
    size_t end_child_id = d_itemModel->getChildId(index);
    size_t start_child_id = end_child_id;
    if (is_range && is_cumulative && d_lastSelectedIndex.d_modelData != nullptr)
    {
        start_child_id = d_itemModel->getChildId(d_lastSelectedIndex);
    }

    for (size_t id = start_child_id; id <= end_child_id; ++id)
    {
        ModelIndexSelectionState selection_state;
        selection_state.d_selectedIndex = d_itemModel->makeIndex(id, parent_index);

        // ignore already selected indices
        if (getSelectedIndexPosition(selection_state.d_selectedIndex) != -1)
            continue;

        selection_state.d_childId = id;
        selection_state.d_parentIndex = parent_index;

        d_indexSelectionStates.push_back(selection_state);
    }

    d_lastSelectedIndex = index;

    ItemViewEventArgs args(this, index);
    onSelectionChanged(args);
    return true;
}

//----------------------------------------------------------------------------//
ViewSortMode ItemView::getSortMode() const
{
    return d_sortMode;
}

//----------------------------------------------------------------------------//
void ItemView::setSortMode(ViewSortMode sort_mode)
{
    if (d_sortMode == sort_mode)
        return;

    d_sortMode = sort_mode;

    resortView();

    WindowEventArgs args(this);
    onSortModeChanged(args);
}

//----------------------------------------------------------------------------//
void ItemView::onSortModeChanged(WindowEventArgs& args)
{
    invalidateView(false);
    //TODO: make all events be triggered on view's event namespace.
    fireEvent(EventSortModeChanged, args);
}

//----------------------------------------------------------------------------//
void ItemView::onViewContentsChanged(WindowEventArgs& args)
{
    fireEvent(EventViewContentsChanged, args, EventNamespace);
}

//----------------------------------------------------------------------------//
void ItemView::clearSelections()
{
    d_indexSelectionStates.clear();
}

//----------------------------------------------------------------------------//
float ItemView::getRenderedMaxWidth() const
{
    return d_renderedMaxWidth;
}

//----------------------------------------------------------------------------//
float ItemView::getRenderedTotalHeight() const
{
    return d_renderedTotalHeight;
}

//----------------------------------------------------------------------------//
void ItemView::ensureIndexIsVisible(const ModelIndex& index)
{
    Scrollbar* vert_scroll = getVertScrollbar();
    Scrollbar* horz_scroll = getHorzScrollbar();
    Rectf render_area = getViewRenderer()->getViewRenderArea();
    float view_height = render_area.getHeight();
    float view_width = render_area.getWidth();

    Rectf rect = getIndexRect(index);
    float bottom = rect.bottom();
    float top = rect.top();

    // account for current scrollbar value
    float currPos = vert_scroll->getScrollPosition();
    top -= currPos;
    bottom -= currPos;

    // if top is above the view area, or if item is too big to fit
    if ((top < 0.0f) || ((bottom - top) > view_height))
    {
        // scroll top of item to top of box.
        vert_scroll->setScrollPosition(currPos + top);
    }
    // if bottom is below the view area
    else if (bottom >= view_height)
    {
        // position bottom of item at the bottom of the list
        vert_scroll->setScrollPosition(currPos + bottom - view_height);
    }

    const float left = rect.left() - currPos;
    const float right = left + rect.getWidth();

    // if left is left of the view area, or if item too big
    if ((left < render_area.d_min.x) || ((right - left) > view_width))
    {
        // scroll item to left
        horz_scroll->setScrollPosition(currPos + left);
    }
    // if right is right of the view area
    else if (right >= render_area.d_max.x)
    {
        // scroll item to right of list
        horz_scroll->setScrollPosition(currPos + right - view_width);
    }
}

//----------------------------------------------------------------------------//
void ItemView::setAutoResizeHeightEnabled(bool enabled)
{
    if (d_isAutoResizeHeightEnabled != enabled)
        return;

    d_isAutoResizeHeightEnabled = enabled;
    resizeToContent();
}

//----------------------------------------------------------------------------//
bool ItemView::isAutoResizeHeightEnabled() const
{
    return d_isAutoResizeHeightEnabled;
}

//----------------------------------------------------------------------------//
void ItemView::setAutoResizeWidthEnabled(bool enabled)
{
    if (d_isAutoResizeWidthEnabled != enabled)
        return;

    d_isAutoResizeWidthEnabled = enabled;
    resizeToContent();
}

//----------------------------------------------------------------------------//
bool ItemView::isAutoResizeWidthEnabled() const
{
    return d_isAutoResizeWidthEnabled;
}

//----------------------------------------------------------------------------//
void ItemView::resizeToContent()
{
    if (d_initialising ||
        !(d_isAutoResizeWidthEnabled || d_isAutoResizeHeightEnabled))
        return;

    getViewRenderer()->resizeViewToContent(
        d_isAutoResizeWidthEnabled, d_isAutoResizeHeightEnabled);
}

//----------------------------------------------------------------------------//
ItemViewEventArgs::ItemViewEventArgs(ItemView* wnd, ModelIndex index)
    : WindowEventArgs(wnd)
    , d_index(index)
{
}

}
