/************************************************************************
    filename:   CEGUIScrolledItemListBase.cpp
    created:    Sat Oct 29 2005
    author:     Tomas Lindquist Olsen
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "elements/CEGUIScrolledItemListBase.h"
#include "elements/CEGUIClippedContainer.h"
#include "CEGUIWindowManager.h"
#include "CEGUILogger.h"
#include "CEGUIPropertyHelper.h"

// begin CEGUI namespace
namespace CEGUI
{

/*************************************************************************
    Constants
*************************************************************************/
// event strings
const String ScrolledItemListBase::EventNamespace("ScrolledItemListBase");
const String ScrolledItemListBase::EventVertScrollbarModeChanged("VertScrollbarModeChanged");
const String ScrolledItemListBase::EventHorzScrollbarModeChanged("HorzScrollbarModeChanged");

// component widget name suffixes
const String ScrolledItemListBase::VertScrollbarNameSuffix( "__auto_vscrollbar__" );
const String ScrolledItemListBase::HorzScrollbarNameSuffix( "__auto_hscrollbar__" );
const String ScrolledItemListBase::ContentPaneNameSuffix("__auto_content_pane__");

/*************************************************************************
    Properties
*************************************************************************/
ScrolledItemListBaseProperties::ForceVertScrollbar ScrolledItemListBase::d_forceVertScrollbarProperty;
ScrolledItemListBaseProperties::ForceHorzScrollbar ScrolledItemListBase::d_forceHorzScrollbarProperty;

/************************************************************************
    Constructor
************************************************************************/
ScrolledItemListBase::ScrolledItemListBase(const String& type, const String& name) :
    ItemListBase(type, name),
    d_forceVScroll(false),
    d_forceHScroll(false)
{
    // Make sure the content pane is initially empty
    // NOTE: initialiseComponents() is responsible for creating it
    d_pane = 0;

    // add properties for this class
    addScrolledItemListBaseProperties();
}

/************************************************************************
    Destructor
************************************************************************/
ScrolledItemListBase::~ScrolledItemListBase()
{
}

/************************************************************************
    Initialise
************************************************************************/
void ScrolledItemListBase::initialiseComponents()
{
    // Only process the content pane if it hasn't been done in the past
    // NOTE: This ensures that a duplicate content pane is not created. An example where
    // this would be possible would be when changing the Look'N'Feel of the widget
    // (for instance an ItemListBox), an operation which would reconstruct the child components
    // of the widget by destroying the previous ones and creating new ones with the
    // new Look'N'Feel. However, since the content pane is not defined in the
    // look and feel file and thus not associated with the look'N'Feel itself
    // but instead created here manually, the destruction would not contemplate the content
    // pane itself, so when the children would be rebuilt, a duplicate content pane
    // would be attempted (and an exception would be issued).
    if(!d_pane)
    {
        // IMPORTANT:
        // we must do this before the base class handling or we'll lose the onChildRemoved subscriber!!!
        d_pane = WindowManager::getSingletonPtr()->createWindow("ClippedContainer", d_name+ContentPaneNameSuffix);

        // set up clipping
        static_cast<ClippedContainer*>(d_pane)->setClipperWindow(this);
        addChildWindow(d_pane);
    }

    // base class handling
    ItemListBase::initialiseComponents();

    // set default pane position
    Rect r = getItemRenderArea();
    d_pane->setPosition(UVector2(cegui_absdim(r.d_left),cegui_absdim(r.d_top)));

    // init scrollbars
    Scrollbar* v = getVertScrollbar();
    Scrollbar* h = getHorzScrollbar();

    v->setAlwaysOnTop(true);
    h->setAlwaysOnTop(true);

    v->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&ScrolledItemListBase::handle_VScroll,this));
    h->subscribeEvent(Scrollbar::EventScrollPositionChanged,
        Event::Subscriber(&ScrolledItemListBase::handle_HScroll,this));

    v->hide();
    h->hide();
}

/************************************************************************
    Get vertical scroll bar
************************************************************************/
Scrollbar* ScrolledItemListBase::getVertScrollbar() const
{
    return static_cast<Scrollbar*>(WindowManager::getSingleton()
        .getWindow(d_name+VertScrollbarNameSuffix));
}

/************************************************************************
    Get horizontal scroll bar
************************************************************************/
Scrollbar* ScrolledItemListBase::getHorzScrollbar() const
{
    return static_cast<Scrollbar*>(WindowManager::getSingleton()
        .getWindow(d_name+HorzScrollbarNameSuffix));
}

/************************************************************************
    Configure scroll bars
************************************************************************/
void ScrolledItemListBase::configureScrollbars(const Size& doc_size)
{
    Scrollbar* v = getVertScrollbar();
    Scrollbar* h = getHorzScrollbar();

    Size render_area_size = getItemRenderArea().getSize();

    // setup the pane size
    float pane_size_w = ceguimax(doc_size.d_width, render_area_size.d_width);
    UVector2 pane_size(cegui_absdim(pane_size_w), cegui_absdim(doc_size.d_height));

    d_pane->setMinSize(pane_size);
    d_pane->setMaxSize(pane_size);
    //d_pane->setWindowSize(pane_size);

    // "fix" scrollbar visibility
    if (d_forceVScroll || doc_size.d_height > render_area_size.d_height)
    {
        v->show();
    }
    else
    {
        v->hide();
    }

    //render_area_size = getItemRenderArea().getSize();

    if (d_forceHScroll || doc_size.d_width > render_area_size.d_width)
    {
        h->show();
    }
    else
    {
        h->hide();
    }

    // get a fresh item render area
    Rect render_area = getItemRenderArea();
    render_area_size = render_area.getSize();

    // update the pane clipper area
    static_cast<ClippedContainer*>(d_pane)->setClipArea(render_area);

    // setup vertical scrollbar
    v->setDocumentSize(doc_size.d_height);
    v->setPageSize(render_area_size.d_height);
    v->setStepSize(ceguimax(1.0f, render_area_size.d_height / 10.0f));
    v->setScrollPosition(v->getScrollPosition());

    // setup horizontal scrollbar
    h->setDocumentSize(doc_size.d_width);
    h->setPageSize(render_area_size.d_width);
    h->setStepSize(ceguimax(1.0f, render_area_size.d_width / 10.0f));
    h->setScrollPosition(h->getScrollPosition());
}

/************************************************************************
    Handle mouse wheel event
************************************************************************/
void ScrolledItemListBase::onMouseWheel(MouseEventArgs& e)
{
    ItemListBase::onMouseWheel(e);

    size_t count = getItemCount();
    Scrollbar* v = getVertScrollbar();

    // dont do anything if we are no using scrollbars
    // or have'nt got any items
    if (!v->isVisible(true) || !count)
    {
        return;
    }

    float pixH = d_pane->getUnclippedPixelRect().getHeight();
    float delta = (pixH/float(count)) * -e.wheelChange;
    v->setScrollPosition(v->getScrollPosition() + delta);
    e.handled = true;
}

/************************************************************************
    Event subscribers for scrolling
************************************************************************/
bool ScrolledItemListBase::handle_VScroll(const EventArgs& e)
{
    const WindowEventArgs& we = static_cast<const WindowEventArgs&>(e);
    Scrollbar* v = static_cast<Scrollbar*>(we.window);
    Rect render_area = getItemRenderArea();
    float newpos = -v->getScrollPosition()+render_area.d_top;
    d_pane->setYPosition(cegui_absdim(newpos));
    return true;
}

bool ScrolledItemListBase::handle_HScroll(const EventArgs& e)
{
    const WindowEventArgs& we = static_cast<const WindowEventArgs&>(e);
    Scrollbar* h = static_cast<Scrollbar*>(we.window);
    Rect render_area = getItemRenderArea();
    float newpos = -h->getScrollPosition()+render_area.d_left;
    d_pane->setXPosition(cegui_absdim(newpos));
    return true;
}

/************************************************************************
    Set vertical scrollbar mode
************************************************************************/
void ScrolledItemListBase::setShowVertScrollbar(bool mode)
{
    if (mode != d_forceVScroll)
    {
        d_forceVScroll = mode;
        WindowEventArgs e(this);
        onVertScrollbarModeChanged(e);
    }
}

/************************************************************************
    Set horizontal scrollbar mode
************************************************************************/
void ScrolledItemListBase::setShowHorzScrollbar(bool mode)
{
    if (mode != d_forceHScroll)
    {
        d_forceHScroll = mode;
        WindowEventArgs e(this);
        onHorzScrollbarModeChanged(e);
    }
}

/************************************************************************
    Handle vertical scrollbar mode change
************************************************************************/
void ScrolledItemListBase::onVertScrollbarModeChanged(WindowEventArgs& e)
{
    fireEvent(EventVertScrollbarModeChanged, e);
}

/************************************************************************
    Handle horizontal scrollbar mode change
************************************************************************/
void ScrolledItemListBase::onHorzScrollbarModeChanged(WindowEventArgs& e)
{
    fireEvent(EventHorzScrollbarModeChanged, e);
}

/*************************************************************************
    Add ItemListbox specific properties
*************************************************************************/
void ScrolledItemListBase::addScrolledItemListBaseProperties()
{
    addProperty(&d_forceVertScrollbarProperty);
    addProperty(&d_forceHorzScrollbarProperty);
}

//----------------------------------------------------------------------------//
void ScrolledItemListBase::ensureItemIsVisibleVert(const ItemEntry& item)
{
    const Rect render_area = getItemRenderArea();
    Scrollbar* const v = getVertScrollbar();
    const float currPos = v->getScrollPosition();

    const float top =
        item.getYPosition().asAbsolute(this->getPixelSize().d_height) - currPos;
    const float bottom = top + item.getItemPixelSize().d_height;

    // if top is above the view area, or if item is too big, scroll item to top
    if ((top < render_area.d_top) || ((bottom - top) > render_area.getHeight()))
        v->setScrollPosition(currPos + top);
    // if bottom is below the view area, scroll item to bottom of list
    else if (bottom >= render_area.d_bottom)
        v->setScrollPosition(currPos + bottom - render_area.getHeight());
}

//----------------------------------------------------------------------------//
void ScrolledItemListBase::ensureItemIsVisibleHorz(const ItemEntry& item)
{
    const Rect render_area = getItemRenderArea();
    Scrollbar* const h = getHorzScrollbar();
    const float currPos = h->getScrollPosition();

    const float left =
        item.getXPosition().asAbsolute(this->getPixelSize().d_width) - currPos;
    const float right = left + item.getItemPixelSize().d_width;

    // if left is left of the view area, or if item too big, scroll item to left
    if ((left < render_area.d_left) || ((right - left) > render_area.getWidth()))
        h->setScrollPosition(currPos + left);
    // if right is right of the view area, scroll item to right of list
    else if (right >= render_area.d_right)
        h->setScrollPosition(currPos + right - render_area.getWidth());
}

//----------------------------------------------------------------------------//

} // end CEGUI namespace
