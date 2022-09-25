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
#include "CEGUI/GUIContext.h"

namespace CEGUI
{
const String MenuItem::WidgetTypeName("CEGUI/MenuItem");
const String MenuItem::EventNamespace("MenuItem");
const String MenuItem::EventClicked("Clicked");

// Implementation details for popup clipping computations
namespace
{
//----------------------------------------------------------------------------//
// translate_within takes two Rectf's. It tries to find the smallest translation
// which will move the "test rectangle" so that it is witin the "clip rectangle".
// This is used for popup menu placement.
// If the test rectangle does not fit (too wide or too tall) then this implementation
// will prioritize getting the top and left-most edges correct.
glm::vec2 translate_within(const Rectf& test_rect, const Rectf& clip_rect)
{
    glm::vec2 result(0.f, 0.f);

    if (test_rect.top() < clip_rect.top())
        result.y = clip_rect.top() - test_rect.top();
    else if (test_rect.bottom() > clip_rect.bottom())
        result.y = clip_rect.bottom() - test_rect.bottom();

    if (test_rect.left() < clip_rect.left())
        result.x = clip_rect.left() - test_rect.left();
    else if (test_rect.right() > clip_rect.right())
        result.x = clip_rect.right() - test_rect.right();

    return result;
}

//----------------------------------------------------------------------------//
// helper function for glm::vec2 -> UVector2 conversion
UVector2 as_uvector(const glm::vec2& v)
{
    return UVector2(cegui_absdim(v.x), cegui_absdim(v.y));
}

//----------------------------------------------------------------------------//
// version of Rectf::offset, which yields a new rectangle
Rectf offset_rect(Rectf result, const glm::vec2& v)
{
    result.offset(v);
    return result;
}

}

//----------------------------------------------------------------------------//
MenuItem::MenuItem(const String& type, const String& name)
    : ItemEntry(type, name)
{
    addMenuItemProperties();
}

//----------------------------------------------------------------------------//
void MenuItem::setHovered(bool hovered)
{
    if (hovered == d_hovering)
        return;

    d_hovering = hovered;

    if (d_hovering)
    {
        if (auto menu = dynamic_cast<MenuBase*>(d_ownerList))
        {
            // does this menubar only allow one popup open? and is there a popup open?
            if (!menu->isMultiplePopupsAllowed())
            {
                const MenuItem* currPopup = menu->getPopupMenuItem();
                if (currPopup && currPopup != this)
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
    else
    {
        if (auto menu = dynamic_cast<MenuBase*>(d_ownerList))
        {
            // does this menubar only allow one popup open? and is there a popup open?
            if (!menu->isMultiplePopupsAllowed())
            {
                const MenuItem* currPopup = menu->getPopupMenuItem();
                const auto hoveredWindow = d_guiContext ? d_guiContext->getWindowContainingCursor() : nullptr;
                if (currPopup && currPopup == this && (!hoveredWindow || !hoveredWindow->isDescendantOf(this)))
                {
                    if (hasAutoPopup())
                        startPopupClosing();
                }
            }
        }
    }

    invalidate();
}

//----------------------------------------------------------------------------//
bool MenuItem::calcHoveredState(const glm::vec2& cursorPos) const
{
    if (!d_guiContext)
        return false;

    // If input is captured, but not by 'this', then we never hover highlight
    auto targetWnd = d_guiContext->getInputCaptureWindow();
    if (!targetWnd)
        targetWnd = d_guiContext->getWindowContainingCursor();

    return (targetWnd == this && isHit(cursorPos));
}

//----------------------------------------------------------------------------//
void MenuItem::setPopupMenu(PopupMenu* popup)
{
    setPopupMenu_impl(popup);
}

//----------------------------------------------------------------------------//
void MenuItem::setPopupMenu_impl(PopupMenu* popup, bool add_as_child)
{
    if (popup == d_popup)
        return;

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
            WindowManager::getSingletonPtr()->destroyWindow(old_popup);
    }

    // we are setting a new popup and not just clearing. and we are told to add the child
    if (popup && add_as_child)
        addChild(popup);

    invalidate();
}

//----------------------------------------------------------------------------//
bool MenuItem::computePopupOffset(UVector2 & output) const
{
    if (!d_ownerList)
        return false;

    // Current size of popup window
    const Sizef popup_size = d_popup->getPixelSize();
    // Absolute coords corresponding to upper left corner of menu item
    const glm::vec2 base_pos = this->getClipRect(false).d_min;

    const Rectf popup_rect{base_pos, popup_size};

    // The bounding box assumed to clip the popup menus
    const Rectf clip_rect = this->popupBoundingBox();

    if (auto menubar = dynamic_cast<Menubar*>(d_ownerList))
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
    else if (dynamic_cast<PopupMenu*>(d_ownerList))
    {
        // Use a horizontal orientation

        // candidate 1: align the top left of popup to the top-right of the menuitem
        const glm::vec2 pos1(d_pixelSize.d_width, 0);

        // candidate 2: align the top right of popup to the top-left of menuitem
        const glm::vec2 pos2(-popup_size.d_width, 0);

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

    return false;  
}

//----------------------------------------------------------------------------//
Rectf MenuItem::popupBoundingBox() const
{
    return Rectf(glm::vec2(0, 0), getRootContainerSize());
}

//----------------------------------------------------------------------------//
void MenuItem::openPopupMenu(bool notify)
{
    if (!d_popup || d_opened)
        return;

    d_popupOpening = false;
    d_popupClosing = false;

    // should we notify parent?
    // if so, and we are attached to a menu bar or popup menu, we let it handle the "activation"
    if (notify && d_ownerList)
    {
        if (auto m = dynamic_cast<MenuBase*>(d_ownerList))
        {
            m->changePopupMenuItem(this);
            return; // the rest is handled when the menu bar eventually calls us itself with notify = false
        }
    }

    // by now we must handle it ourselves
    // Update the position of the popup menu before opening it
    UVector2 pos;
    if (computePopupOffset(pos))
        d_popup->setPosition(pos);

    d_popup->openPopupMenu(false);

    d_opened = true;
    invalidate();
}

//----------------------------------------------------------------------------//
void MenuItem::closePopupMenu(bool notify)
{
    if (!d_popup || !d_opened)
        return;

    d_popupOpening = false;
    d_popupClosing = false;

    // should we notify the parent menu base?
    // if we are attached to a menu base, we let it handle the "deactivation"
    auto menu = dynamic_cast<MenuBase*>(d_ownerList);
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
        d_popup->closePopupMenu(false);
    }

    d_opened = false;
    invalidate();
}

//----------------------------------------------------------------------------//
bool MenuItem::togglePopupMenu()
{
    if (d_opened)
    {
        closePopupMenu();
        return false;
    }

    openPopupMenu();
    return true;
}

//----------------------------------------------------------------------------//
void MenuItem::startPopupClosing()
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

//----------------------------------------------------------------------------//
void MenuItem::startPopupOpening()
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

//----------------------------------------------------------------------------//
void MenuItem::closeAllMenuItemPopups()
{
    if (!d_ownerList)
        return;

    if (dynamic_cast<Menubar*>(d_ownerList))
    {
        closePopupMenu();
        return;
    }

    if (auto ownerPopup = dynamic_cast<PopupMenu*>(d_ownerList))
    {
        ownerPopup->closePopupMenu(true);

        // is this parent popup attached to a menu item?
        if (auto mi = dynamic_cast<MenuItem*>(ownerPopup->getParent()))
            mi->closeAllMenuItemPopups();
    }
}

//----------------------------------------------------------------------------//
void MenuItem::onClicked(WindowEventArgs& e)
{
    fireEvent(EventClicked, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void MenuItem::onCursorMove(CursorMoveEventArgs& e)
{
    // this is needed to discover whether cursor is in the widget area or not.
    // The same thing used to be done each frame in the rendering method,
    // but in this version the rendering method may not be called every frame
    // so we must discover the internal widget state here - which is actually
    // more efficient anyway.

    ItemEntry::onCursorMove(e);

    setHovered(calcHoveredState(e.d_surfacePos));
    ++e.handled;
}

//----------------------------------------------------------------------------//
void MenuItem::onMouseButtonDown(MouseButtonEventArgs& e)
{
    ItemEntry::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left)
    {
        d_pushed = true;
        setHovered(calcHoveredState(e.d_surfacePos));
        d_popupWasClosed = !togglePopupMenu();

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void MenuItem::onMouseButtonUp(MouseButtonEventArgs& e)
{
    ItemEntry::onMouseButtonUp(e);

    if (e.d_button == MouseButton::Left)
    {
        // Close the popup if we did'nt spawn a child
        if (!d_opened && !d_popupWasClosed)
            closeAllMenuItemPopups();

        d_popupWasClosed = false;

        WindowEventArgs we(this);
        onClicked(we);

        d_pushed = false;
        setHovered(calcHoveredState(e.d_surfacePos));

        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void MenuItem::onCursorLeaves(CursorInputEventArgs& e)
{
    ItemEntry::onCursorLeaves(e);
    setHovered(false);
    ++e.handled;
}

//----------------------------------------------------------------------------//
void MenuItem::onTextChanged(WindowEventArgs& e)
{
    ItemEntry::onTextChanged(e);

    // if we are attached to a ItemListBase, we make it update as necessary
    if (auto ilb = dynamic_cast<ItemListBase*>(getParent()))
        ilb->handleUpdatedItemData();

    ++e.handled;
}

//----------------------------------------------------------------------------//
void MenuItem::onDeactivated(ActivationEventArgs& e)
{
    ItemEntry::onDeactivated(e);
    closePopupMenu();
}

//----------------------------------------------------------------------------//
void MenuItem::updateSelf(float elapsed)
{
    ItemEntry::updateSelf(elapsed);

    // Handle delayed popup closing/opening when hovering with the cursor
    if (d_autoPopupTimeout >= 0.f && (d_popupOpening || d_popupClosing))
    {
        // stop timer if the hovering state isn't set appropriately anymore
        if (d_hovering)
            d_popupClosing = false;
        else
            d_popupOpening = false;

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

//----------------------------------------------------------------------------//
void MenuItem::addChild_impl(Element* element)
{
    Window* wnd = dynamic_cast<Window*>(element);

    if (!wnd)
        throw InvalidRequestException(
            "MenuItem can only have Elements of type Window added as children "
            "(Window path: " + getNamePath() + ").");

    ItemEntry::addChild_impl(wnd);

    // if this is a PopupMenu we add it like one
    if (PopupMenu* pop = dynamic_cast<PopupMenu*>(wnd))
        setPopupMenu_impl(pop, false);
}

//----------------------------------------------------------------------------//
void MenuItem::addMenuItemProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(MenuItem, float,
        "AutoPopupTimeout",
        "The time in seconds, which has to elapse before the popup window is opened/closed if the hovering state changes. Set negative number to disable auto-popup. Value is a float property value.",
        &MenuItem::setAutoPopupTimeout, &MenuItem::getAutoPopupTimeout, 0.0f
    );
}

}
