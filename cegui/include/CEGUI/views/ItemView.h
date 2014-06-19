/***********************************************************************
    filename:   ItemView.h
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

#include "CEGUI/Base.h"
#include "CEGUI/ItemModel.h"
#include "CEGUI/Window.h"
#include "CEGUI/Colour.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/widgets/Scrollbar.h"

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

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
};

enum ScrollbarDisplayMode
{
    SDM_Shown,
    SDM_Hidden,
    //! The scrollbar will be shown only if the underlining view's size is too small to contain its items
    SDM_WhenNeeded
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

/*!
\brief
    Stores the selection state of a ModelIndex. This is used to regenerate
    the new proper selection index when the model changes in any way (e.g.:
    new item, removed item).
*/
struct CEGUIEXPORT ModelIndexSelectionState
{
    ModelIndex d_parentIndex;
    size_t d_childId;

    ModelIndex d_selectedIndex;
};
typedef std::vector<ModelIndexSelectionState> SelectionStatesVector;

/*!
\brief
    Abstract base class for all view classes based on ItemModel
*/
class CEGUIEXPORT ItemView : public Window
{
public:
    ItemView(const String& type, const String& name);

    virtual ~ItemView();

    static const Colour DefaultTextColour;
    //!< Widget name for the vertical scrollbar component.
    static const String VertScrollbarName;
    //!< Widget name for the horizontal scrollbar component
    static const String HorzScrollbarName;
    static const String EventVertScrollbarDisplayModeChanged;
    static const String EventHorzScrollbarDisplayModeChanged;

    //!Sets the ItemModel to be used inside this view.
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

    const ColourRect& getTextColourRect() const;
    void setTextColourRect(const ColourRect& colour_rect);

    bool isDirty() const;
    //! Specifies whether this view requires processing before being able to render it.
    void setIsDirty(bool value);

    //! Gets the current state of the indices used for selection.
    const std::vector<ModelIndexSelectionState>& getIndexSelectionStates() const;

    /*!
    \brief
        Returns the ModelIndex at the specified position.

    \return
        The ModelIndex for the position or a default-constructed ModelIndex
        if nothing was found at that position.
    */
    virtual ModelIndex indexAt(const Vector2f& position) = 0;

    /*!
    \brief
        Sets the specified item index's as the currently selected item.

    \return
        True if the item has been successfully selected, false otherwise.
    */
    virtual bool setSelectedItem(const ModelIndex& index);

    void setSelectionBrushImage(const Image* image);
    void setSelectionBrushImage(const String& name);
    const Image* getSelectionBrushImage(void) const;

    /*!
    \brief
        Returns a pointer to the vertical scrollbar component widget for this
        Listbox.

    \exception UnknownObjectException
        Thrown if the vertical Scrollbar component does not exist.
    */
    Scrollbar* getVertScrollbar() const;

    void setVertScrollbarDisplayMode(ScrollbarDisplayMode mode);
    ScrollbarDisplayMode getVertScrollbarDisplayMode() const;

    /*!
    \brief
        Returns a pointer to the horizontal scrollbar component widget for this
        Listbox.

    \exception UnknownObjectException
        Thrown if the horizontal Scrollbar component does not exist.
    */
    Scrollbar* getHorzScrollbar() const;

    void setHorzScrollbarDisplayMode(ScrollbarDisplayMode mode);
    ScrollbarDisplayMode getHorzScrollbarDisplayMode() const;

protected:
    ItemModel* d_itemModel;
    ColourRect d_textColourRect;
    bool d_isDirty;
    std::vector<ModelIndexSelectionState> d_indexSelectionStates;
    const Image* d_selectionBrush;
    ScrollbarDisplayMode d_vertScrollbarDisplayMode;
    ScrollbarDisplayMode d_horzScrollbarDisplayMode;

    //TODO: move this into the renderer instead?
    float d_renderedMaxWidth;
    float d_renderedTotalHeight;

    void addItemViewProperties();
    void updateScrollbars();
    void updateScrollbar(Scrollbar* scrollbar, float available_area,
        float rendered_area, ScrollbarDisplayMode display_mode);
    ItemViewWindowRenderer* getViewRenderer();
    void updateScrollbarDisplayMode(ScrollbarDisplayMode& target_mode,
        ScrollbarDisplayMode new_mode, String change_event);

    //! Invalidates this view by marking the rendering state as dirty and calling the base
    virtual void invalidateView(bool recursive);

    virtual bool isIndexSelected(const ModelIndex& index) const;

    virtual void initialiseComponents(void);
    virtual void onPointerPressHold(PointerEventArgs& e);

    virtual bool onChildrenAdded(const EventArgs& args);
    virtual bool onChildrenRemoved(const EventArgs& args);
    virtual bool onChildrenDataChanged(const EventArgs& args);
    virtual bool onScrollPositionChanged(const EventArgs& args);

    Event::Connection d_eventChildrenAddedConnection;
    Event::Connection d_eventChildrenRemovedConnection;
    Event::Connection d_eventChildrenDataChangedConnection;
    void connectToModelEvents(ItemModel* d_itemModel);
    void disconnectModelEvents();
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
