/***********************************************************************
    created:    2/4/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)

    purpose:    Implementation of MenuItem widget base class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/widgets/MenuItem.h"
#include "CEGUI/widgets/Menubar.h"
#include "CEGUI/widgets/PopupMenu.h"

#include "CEGUI/WindowManager.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*************************************************************************
    Constants
*************************************************************************/
// event strings
const String MenuItem::WidgetTypeName("CEGUI/MenuItem");
const String MenuItem::EventNamespace("MenuItem");
const String MenuItem::EventClicked("Clicked");

/*************************************************************************
    Constructor for MenuItem base class.
*************************************************************************/
MenuItem::MenuItem(const String& type, const String& name)
    : ItemEntry(type, name),
      d_pushed(false),
      d_hovering(false),
      d_opened(false),
      d_popupClosing(false),
      d_popupOpening(false),
      d_autoPopupTimeout(0.0f),
      d_autoPopupTimeElapsed(0.0f),
      d_popup(nullptr)
{
    // add the new properties
    addMenuItemProperties();
}


/*************************************************************************
    Destructor for MenuItem base class.
*************************************************************************/
MenuItem::~MenuItem(void)
{
}


/*************************************************************************
    Update the internal state of the Widget
*************************************************************************/
void MenuItem::updateInternalState(const glm::vec2& cursor_pos)
{
    bool oldstate = d_hovering;

    // assume not hovering
    d_hovering = false;

    // if input is captured, but not by 'this', then we never hover highlight
    const Window* capture_wnd = getCaptureWindow();

    if (capture_wnd == nullptr)
        d_hovering = (getGUIContext().getWindowContainingCursor() == this && isHit(cursor_pos));
    else
        d_hovering = (capture_wnd == this && isHit(cursor_pos));

    // if state has changed, trigger a re-draw
    // and possible make the parent menu open another popup
    if (oldstate != d_hovering)
    {
        // are we attached to a menu ?
        MenuBase* menu = dynamic_cast<MenuBase*>(d_ownerList);
        if (menu)
        {
            if (d_hovering)
            {
                // does this menubar only allow one popup open? and is there a popup open?
                const MenuItem* curpopup = menu->getPopupMenuItem();

                if (!menu->isMultiplePopupsAllowed())
                {
                    if (curpopup != this && curpopup != nullptr)
                    {
                        if (!hasAutoPopup())
                        {
                            // open this popup instead
                            openPopupMenu();
                        }
                        else
                        {
                            // start close timer on current popup
                            menu->setPopupMenuItemClosing();
                            startPopupOpening();
                        }
                    }
                    else
                    {
                        startPopupOpening();
                    }
                }
            }
        }

        invalidate();
    }
}


/*************************************************************************
    Set the popup menu for this item.
*************************************************************************/
void MenuItem::setPopupMenu(PopupMenu* popup)
{
    setPopupMenu_impl(popup);
}


/*************************************************************************
    Set the popup menu for this item.
*************************************************************************/
void MenuItem::setPopupMenu_impl(PopupMenu* popup, bool add_as_child)
{
    // is it the one we have already ?
    if (popup == d_popup)
    {
        // then do nothing;
        return;
    }

    // keep the old one around
    PopupMenu* old_popup = d_popup;
    // update the internal state pointer
    d_popup = popup;
    d_opened = false;

    // is there already a popup ?
    if (old_popup)
    {
        removeChild(old_popup);

        // should we destroy it as well?
        if (old_popup->isDestroyedByParent())
        {
            // then do so
            WindowManager::getSingletonPtr()->destroyWindow(old_popup);
        }
    }

    // we are setting a new popup and not just clearing. and we are told to add the child
    if (popup != nullptr && add_as_child)
    {
        addChild(popup);
    }

    invalidate();
}

namespace
{

/***
 * Implementation details for popup clipping computations
 */
 
// translate_within takes two Rectf's. It tries to find the smallest translation
// which will move the "test rectangle" so that it is witin the "clip rectangle".
// This is used for popup menu placement.
// If the test rectangle does not fit (too wide or too tall) then this implementation
// will prioritize getting the top and left-most edges correct.
glm::vec2 translate_within(const Rectf & test_rect, const Rectf & clip_rect)
{
    glm::vec2 result(0, 0);

    if (test_rect.top() < clip_rect.top())
    {
        result.y = clip_rect.top() - test_rect.top(); 
    }
    else if (test_rect.bottom() > clip_rect.bottom())
    {
        result.y = clip_rect.bottom() - test_rect.bottom();
    }

    if (test_rect.left() < clip_rect.left())
    {
        result.x = clip_rect.left() - test_rect.left();
    }
    else if (test_rect.right() > clip_rect.right())
    {
        result.x = clip_rect.right() - test_rect.right();
    }

    return result;
}

// helper function for glm::vec2 -> UVector2 conversion
UVector2 as_uvector(const glm::vec2 & v)
{
    return UVector2(cegui_absdim(v.x), cegui_absdim(v.y));
}

// version of Rectf::offset, which yields a new rectangle
Rectf offset_rect(Rectf result, const glm::vec2 & v)
{
    result.offset(v);
    return result;
}

} // end anonymous namespace

/*************************************************************************
    Computes the offset at which a popup menu will appear.
*************************************************************************/
bool MenuItem::computePopupOffset(UVector2 & output) const
{
    Window* p = d_ownerList;
    if (p)
    {
        // Current size of popup window
        const Sizef popup_size = d_popup->getPixelSize();
        // Absolute coords corresponding to upper left corner of menu item
        const glm::vec2 base_pos = this->getClipRect(false).d_min;

        const Rectf popup_rect{base_pos, popup_size};

        // The bounding box assumed to clip the popup menus
        const Rectf clip_rect = this->popupBoundingBox();

        if (Menubar * menubar = dynamic_cast<Menubar *>(p))
        {
            // Use a vertical orientation

            // candidate 1: align the top left of popup to the bottom-left of the menuitem
            const glm::vec2 pos1(0, d_pixelSize.d_height);

            // candidate 2: align the popup to the top-left of menuitem
            const glm::vec2 pos2(0, - popup_size.d_height);

            switch (menubar->getMenubarDirection())
            {
            case MenubarDirection::Down:
                output = as_uvector(pos1);
                return true;
            case MenubarDirection::Up:
                output = as_uvector(pos2);
                return true;
            default:
                return false;
            }
        }
        else if (dynamic_cast<PopupMenu *>(p))
        {
            // Use a horizontal orientation

            // candidate 1: align the top left of popup to the top-right of the menuitem
            const glm::vec2 pos1(d_pixelSize.d_width, 0);

            // candidate 2: align the top right of popup to the top-left of menuitem
            const glm::vec2 pos2(- popup_size.d_width, 0);

            // Compute correction vectors for each
            const glm::vec2 pos1_corr = translate_within(offset_rect(popup_rect, pos1), clip_rect);
            const glm::vec2 pos2_corr = translate_within(offset_rect(popup_rect, pos2), clip_rect);

            // If pos2 does not require x correction and pos1 does, then use pos2
            if (pos1_corr.x && !pos2_corr.x)
            {
                output = as_uvector(pos2 + pos2_corr);
                return true;
            }
            else
            {
                output = as_uvector(pos1 + pos1_corr);
                return true;
            }
        }
    }
    return false;  
}


/*************************************************************************
    Compute bounding box (assumed clipping rectangle) of our popup menu
*************************************************************************/
Rectf MenuItem::popupBoundingBox() const
{
    return Rectf(glm::vec2(0, 0), getRootContainerSize());
}


/*************************************************************************
    Open the PopupMenu attached to this item.
*************************************************************************/
void MenuItem::openPopupMenu(bool notify)
{
    // no popup? or already open...
    if (d_popup == nullptr || d_opened)
        return;

    d_popupOpening = false;
    d_popupClosing = false;

    // should we notify parent ?
    // if so, and we are attached to a menu bar or popup menu, we let it handle the "activation"
    Window* p = d_ownerList;

    if (notify && p)
    {
        if (Menubar * m = dynamic_cast<Menubar*>(p))
        {
            m->changePopupMenuItem(this);
            return; // the rest is handled when the menu bar eventually calls us itself with notify = false
        }
        else if (PopupMenu * pm = dynamic_cast<PopupMenu*>(p))
        {
            pm->changePopupMenuItem(this);
            return; // the rest is handled when the menu bar eventually calls us itself with notify = false
        }
    }

    // by now we must handle it ourselves
    // Update the position of the popup menu before opening it
    UVector2 pos;
    if (this->computePopupOffset(pos)) {
        d_popup->setPosition(pos);
    }

    d_popup->openPopupMenu(false);

    d_opened = true;
    invalidate();
}


/*************************************************************************
    Close the PopupMenu attached to this item.
*************************************************************************/
void MenuItem::closePopupMenu(bool notify)
{
    // no popup? or not open...
    if (d_popup == nullptr || !d_opened)
        return;

    d_popupOpening = false;
    d_popupClosing = false;

    // should we notify the parent menu base?
    // if we are attached to a menu base, we let it handle the "deactivation"
    MenuBase* menu = dynamic_cast<MenuBase*>(d_ownerList);
    if (notify && menu)
    {
        // only if the menu base does not allow multiple popups
        if (!menu->isMultiplePopupsAllowed())
        {
            menu->changePopupMenuItem(nullptr);
            return; // the rest is handled when the menu base eventually call us again itself
        }
    }
    // otherwise we do ourselves
    else
    {
        // match up with Menubar::changePopupMenu
        //d_popup->hide();
        d_popup->closePopupMenu(false);
    }

    d_opened = false;
    invalidate();
}


/*************************************************************************
    Toggles the PopupMenu.
*************************************************************************/
bool MenuItem::togglePopupMenu(void)
{
    if (d_opened)
    {
        closePopupMenu();
        return false;
    }

    openPopupMenu();
    return true;
}

void MenuItem::startPopupClosing(void)
{
    d_popupOpening = false;

    if (d_opened)
    {
        d_autoPopupTimeElapsed = 0.0f;
        d_popupClosing = true;
        invalidate();
    }
    else
    {
        d_popupClosing = false;
    }
}

void MenuItem::startPopupOpening(void)
{
    d_popupClosing = false;

    if (d_opened)
    {
        d_popupOpening = false;
    }
    else
    {
        d_autoPopupTimeElapsed = 0.0f;
        d_popupOpening = true;
    }
}

/*************************************************************************
    Recursive function that closes all popups down the hierarchy starting
    with this one.
*************************************************************************/
void MenuItem::closeAllMenuItemPopups()
{
    // are we attached to a PopupMenu?
    if (!d_ownerList)
        return;

    if (dynamic_cast<Menubar*>(d_ownerList))
    {
        closePopupMenu();
        return;
    }

    PopupMenu* pop = dynamic_cast<PopupMenu*>(d_ownerList);
    if (pop)
    {
        // is this parent popup attached to a menu item?
        Window* popParent = pop->getParent();
        MenuItem* mi = dynamic_cast<MenuItem*>(popParent);

        if (mi)
        {
            // recurse
            mi->closeAllMenuItemPopups();
        }
        // otherwise we just hide the parent popup
        else
        {
            pop->closePopupMenu(false);
        }
    }
}


/*************************************************************************
    handler invoked internally when the menuitem is clicked.
*************************************************************************/
void MenuItem::onClicked(WindowEventArgs& e)
{
    // close the popup if we did'nt spawn a child
    if (!d_opened && !d_popupWasClosed)
    {
        closeAllMenuItemPopups();
    }

    d_popupWasClosed = false;
    fireEvent(EventClicked, e, EventNamespace);
}


/*************************************************************************
    Handler for when the cursor moves
*************************************************************************/
void MenuItem::onCursorMove(CursorInputEventArgs& e)
{
    // this is needed to discover whether cursor is in the widget area or not.
    // The same thing used to be done each frame in the rendering method,
    // but in this version the rendering method may not be called every frame
    // so we must discover the internal widget state here - which is actually
    // more efficient anyway.

    // base class processing
    ItemEntry::onCursorMove(e);

    updateInternalState(e.position);
    ++e.handled;
}


/*************************************************************************
    Handler for cursor pressed events
*************************************************************************/
void MenuItem::onCursorPressHold(CursorInputEventArgs& e)
{
    // default processing
    ItemEntry::onCursorPressHold(e);

    if (e.source == CursorInputSource::Left)
    {
        d_popupWasClosed = false;

        if (captureInput())
        {
            d_pushed = true;
            updateInternalState(e.position);
            d_popupWasClosed = !togglePopupMenu();
            invalidate();
        }

        // event was handled by us.
        ++e.handled;
    }
}


/*************************************************************************
    Handler for cursor activation events
*************************************************************************/
void MenuItem::onCursorActivate(CursorInputEventArgs& e)
{
    // default processing
    ItemEntry::onCursorActivate(e);

    if (e.source == CursorInputSource::Left)
    {
        releaseInput();

        // was the button released over this window?
        // (use cursor position, as e.position in args has been unprojected)
        if (!d_popupWasClosed &&
                getGUIContext().getRootWindow()->getTargetChildAtPosition(
                    getGUIContext().getCursor().getPosition()) == this)
        {
            WindowEventArgs we(this);
            onClicked(we);
        }

        // event was handled by us.
        ++e.handled;
    }
}

/*************************************************************************
    Handler for when cursor capture is lost
*************************************************************************/
void MenuItem::onCaptureLost(WindowEventArgs& e)
{
    // Default processing
    ItemEntry::onCaptureLost(e);

    d_pushed = false;
    updateInternalState(getUnprojectedPosition(
        getGUIContext().getCursor().getPosition()));
    invalidate();

    // event was handled by us.
    ++e.handled;
}


/*************************************************************************
    Handler for when cursor leaves the widget
*************************************************************************/
void MenuItem::onCursorLeaves(CursorInputEventArgs& e)
{
    // default processing
    ItemEntry::onCursorLeaves(e);

    d_hovering = false;
    invalidate();

    ++e.handled;
}


/*************************************************************************
    Handler called when text is changed.
*************************************************************************/
void MenuItem::onTextChanged(WindowEventArgs& e)
{
    ItemEntry::onTextChanged(e);

    // if we are attached to a ItemListBase, we make it update as necessary
    Window* parent = getParent();
    ItemListBase* ilb = dynamic_cast<ItemListBase*>(parent);

    if (ilb)
    {
        ilb->handleUpdatedItemData();
    }

    ++e.handled;
}

/*************************************************************************
Perform actual update processing for this Window.
*************************************************************************/
void MenuItem::updateSelf(float elapsed)
{
    ItemEntry::updateSelf(elapsed);

    //handle delayed popup closing/opening when hovering with the cursor
    if (d_autoPopupTimeout != 0.0f && (d_popupOpening || d_popupClosing))
    {
        // stop timer if the hovering state isn't set appropriately anymore
        if (d_hovering)
        {
            d_popupClosing = false;
        }
        else
        {
            d_popupOpening = false;
        }

        //check if the timer elapsed and take action appropriately
        d_autoPopupTimeElapsed += elapsed;

        if (d_autoPopupTimeElapsed > d_autoPopupTimeout)
        {
            if (d_popupOpening)
            {
                d_popupOpening = false;
                openPopupMenu(true);
            }
            else if (d_popupClosing)
            {
                d_popupClosing = false;
                closePopupMenu(true);
            }
        }
    }
}

/*************************************************************************
    Internal version of adding a child window.
*************************************************************************/
void MenuItem::addChild_impl(Element* element)
{
    Window* wnd = dynamic_cast<Window*>(element);

    if (!wnd)
        throw InvalidRequestException(
            "MenuItem can only have Elements of type Window added as children "
            "(Window path: " + getNamePath() + ").");

    ItemEntry::addChild_impl(wnd);

    PopupMenu* pop = dynamic_cast<PopupMenu*>(wnd);
    // if this is a PopupMenu we add it like one
    if (pop)
    {
        setPopupMenu_impl(pop, false);
    }
}

/*************************************************************************
Add MenuItem specific properties
*************************************************************************/
void MenuItem::addMenuItemProperties(void)
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(MenuItem, float,
        "AutoPopupTimeout","Property to specify the time, which has to elapse before the popup window is opened/closed if the hovering state changes. Value is a float property value.",
        &MenuItem::setAutoPopupTimeout, &MenuItem::getAutoPopupTimeout, 0.0f
    );
}

} // End of  CEGUI namespace section
