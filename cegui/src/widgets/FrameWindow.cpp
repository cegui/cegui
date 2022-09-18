/***********************************************************************
    created:    13/4/2004
    author:        Paul D Turner

    purpose:    Implementation of FrameWindow base class
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
#include "CEGUI/widgets/FrameWindow.h"
#include "CEGUI/widgets/Titlebar.h"
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/GUIContext.h"

namespace CEGUI
{
const String FrameWindow::EventNamespace("FrameWindow");
const String FrameWindow::WidgetTypeName("CEGUI/FrameWindow");
const String FrameWindow::EventRollupToggled("RollupToggled");
const String FrameWindow::EventCloseClicked("CloseClicked");
const String FrameWindow::EventDragSizingStarted("DragSizingStarted");
const String FrameWindow::EventDragSizingEnded("DragSizingEnded");
const String FrameWindow::TitlebarName( "__auto_titlebar__" );
const String FrameWindow::CloseButtonName( "__auto_closebutton__" );

//----------------------------------------------------------------------------//
FrameWindow::FrameWindow(const String& type, const String& name)
    : Window(type, name)
{
    addFrameWindowProperties();
}

//----------------------------------------------------------------------------//
void FrameWindow::initialiseComponents()
{
    Titlebar* titlebar = getTitlebar();
    PushButton* closeButton = getCloseButton();

    titlebar->setDraggingEnabled(d_dragMovable);
    titlebar->setText(getText());
    titlebar->setAutoWindow(true);
    closeButton->setAutoWindow(true);

    // ban some properties on components, since they are linked to settings defined here
    titlebar->banPropertyFromXML("Text");
    titlebar->banPropertyFromXML("Visible");
    titlebar->banPropertyFromXML("Disabled");
    closeButton->banPropertyFromXML("Visible");
    closeButton->banPropertyFromXML("Disabled");

    // bind handler to close button 'Click' event
    closeButton->subscribeEvent(PushButton::EventClicked, [this]()
    {
        WindowEventArgs args(this);
        onCloseClicked(args);
        return true;
    });

    Window::initialiseComponents();
}

//----------------------------------------------------------------------------//
bool FrameWindow::isTitleBarEnabled() const
{
    return !getTitlebar()->isDisabled();
}

//----------------------------------------------------------------------------//
bool FrameWindow::isCloseButtonEnabled() const
{
    return !getCloseButton()->isDisabled();
}

//----------------------------------------------------------------------------//
void FrameWindow::setFrameEnabled(bool setting)
{
    d_frameEnabled = setting;
    invalidate();
}

//----------------------------------------------------------------------------//
void FrameWindow::setTitleBarEnabled(bool setting)
{
    if (auto titlebar = getTitlebar())
    {
        titlebar->setEnabled(setting);
        titlebar->setVisible(setting);
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::setCloseButtonEnabled(bool setting)
{
    if (auto closebtn = getCloseButton())
    {
        closebtn->setEnabled(setting);
        closebtn->setVisible(setting);
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::setRollupEnabled(bool setting)
{
    if (!setting && isRolledup())
        toggleRollup();

    d_rollupEnabled = setting;
}

//----------------------------------------------------------------------------//
void FrameWindow::toggleRollup()
{
    if (isRollupEnabled())
    {
        d_rolledup = !d_rolledup;

        WindowEventArgs args(this);
        onRollupToggled(args);
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::setRolledup(bool val)
{
    if(val != isRolledup())
        toggleRollup();
}

//----------------------------------------------------------------------------//
FrameWindow::SizingLocation FrameWindow::getSizingBorderAtPoint(const glm::vec2& pt) const
{
    Rectf frame(getSizingRect());

    // We can only size if the frame is enabled and sizing is on. Point must be inside the outer edge.
    if (!isSizingEnabled() || !isFrameEnabled() || !frame.isPointInRectf(pt))
        return SizingLocation::Invalid;

    // adjust rect to get inner edge
    frame.d_min.x += d_borderSize;
    frame.d_min.y += d_borderSize;
    frame.d_max.x -= d_borderSize;
    frame.d_max.y -= d_borderSize;

    const bool left = (pt.x < frame.d_min.x);
    const bool right = (pt.x >= frame.d_max.x);

    if (pt.y < frame.d_min.y)
        return left ? SizingLocation::TopLeft : right ? SizingLocation::TopRight : SizingLocation::Top;
    if (pt.y >= frame.d_max.y)
        return left ? SizingLocation::BottomLeft : right ? SizingLocation::BottomRight : SizingLocation::Bottom;
    if (left)
        return SizingLocation::Left;
    if (right)
        return SizingLocation::Right;

    return SizingLocation::Invalid;
}

//----------------------------------------------------------------------------//
void FrameWindow::moveLeftEdge(float delta, URect& outArea)
{
    float newWidth = d_pixelSize.d_width - delta;

    // ensure that we only size to the set constraints.
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    const float rootWidth = getRootContainerSize().d_width;
    const float maxWidth(CoordConverter::asAbsolute(d_maxSize.d_width, rootWidth));
    const float minWidth(CoordConverter::asAbsolute(d_minSize.d_width, rootWidth));

    if (maxWidth != 0.0f && newWidth > maxWidth)
        newWidth = maxWidth;
    else if (newWidth < minWidth)
        newWidth = minWidth;

    // Calculate size delta from what our size would be without limitations
    const float unlimitedWidth = CoordConverter::asAbsolute(getSize().d_width, getBasePixelSize().d_width);
    if (unlimitedWidth == newWidth) return;

    // ensure adjustment will be whole pixel
    const float adjustment = unlimitedWidth - newWidth;

    if (d_horizontalAlignment == HorizontalAlignment::Right)
    {
        outArea.d_max.d_x.d_offset -= adjustment;
    }
    else if (d_horizontalAlignment == HorizontalAlignment::Centre)
    {
        outArea.d_max.d_x.d_offset -= adjustment * 0.5f;
        outArea.d_min.d_x.d_offset += adjustment * 0.5f;
    }
    else
    {
        outArea.d_min.d_x.d_offset += adjustment;
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::moveRightEdge(float delta, URect& outArea)
{
    float newWidth = d_pixelSize.d_width + delta;

    // ensure that we only size to the set constraints.
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    const float rootWidth = getRootContainerSize().d_width;
    const float maxWidth(CoordConverter::asAbsolute(d_maxSize.d_width, rootWidth));
    const float minWidth(CoordConverter::asAbsolute(d_minSize.d_width, rootWidth));

    if (maxWidth != 0.0f && newWidth > maxWidth)
        newWidth = maxWidth;
    else if (newWidth < minWidth)
        newWidth = minWidth;

    // Calculate size delta from what our size would be without limitations
    const float unlimitedWidth = CoordConverter::asAbsolute(getSize().d_width, getBasePixelSize().d_width);
    if (unlimitedWidth == newWidth) return;

    // ensure adjustment will be whole pixel
    const float adjustment = newWidth - unlimitedWidth;

    outArea.d_max.d_x.d_offset += adjustment;

    if (d_horizontalAlignment == HorizontalAlignment::Right)
    {
        outArea.d_max.d_x.d_offset += adjustment;
        outArea.d_min.d_x.d_offset += adjustment;
    }
    else if (d_horizontalAlignment == HorizontalAlignment::Centre)
    {
        outArea.d_max.d_x.d_offset += adjustment * 0.5f;
        outArea.d_min.d_x.d_offset += adjustment * 0.5f;
    }

    // move the dragging point so cursor remains 'attached' to edge of window
    d_dragPoint.x += (newWidth - d_pixelSize.d_width);
}

//----------------------------------------------------------------------------//
void FrameWindow::moveTopEdge(float delta, URect& outArea)
{
    float newHeight = d_pixelSize.d_height - delta;

    // ensure that we only size to the set constraints.
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    const float rootHeight = getRootContainerSize().d_height;
    const float maxHeight(CoordConverter::asAbsolute(d_maxSize.d_height, rootHeight));
    const float minHeight(CoordConverter::asAbsolute(d_minSize.d_height, rootHeight));

    if (maxHeight != 0.0f && newHeight > maxHeight)
        newHeight = maxHeight;
    else if (newHeight < minHeight)
        newHeight = minHeight;

    // Calculate size delta from what our size would be without limitations
    const float unlimitedHeight = CoordConverter::asAbsolute(getSize().d_height, getBasePixelSize().d_height);
    if (unlimitedHeight == newHeight) return;

    // ensure adjustment will be whole pixel
    const float adjustment = unlimitedHeight - newHeight;

    if (d_verticalAlignment == VerticalAlignment::Bottom)
    {
        outArea.d_max.d_y.d_offset -= adjustment;
    }
    else if (d_verticalAlignment == VerticalAlignment::Centre)
    {
        outArea.d_max.d_y.d_offset -= adjustment * 0.5f;
        outArea.d_min.d_y.d_offset += adjustment * 0.5f;
    }
    else
    {
        outArea.d_min.d_y.d_offset += adjustment;
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::moveBottomEdge(float delta, URect& outArea)
{
    float newHeight = d_pixelSize.d_height + delta;

    // ensure that we only size to the set constraints.
    // NB: We are required to do this here due to our virtually unique sizing nature; the
    // normal system for limiting the window size is unable to supply the information we
    // require for updating our internal state used to manage the dragging, etc.
    const float rootHeight = getRootContainerSize().d_height;
    const float maxHeight(CoordConverter::asAbsolute(d_maxSize.d_height, rootHeight));
    const float minHeight(CoordConverter::asAbsolute(d_minSize.d_height, rootHeight));

    if (maxHeight != 0.0f && newHeight > maxHeight)
        newHeight = maxHeight;
    else if (newHeight < minHeight)
        newHeight = minHeight;

    // Calculate size delta from what our size would be without limitations
    const float unlimitedHeight = CoordConverter::asAbsolute(getSize().d_height, getBasePixelSize().d_height);
    if (unlimitedHeight == newHeight) return;

    // ensure adjustment will be whole pixel
    const float adjustment = newHeight - unlimitedHeight;

    outArea.d_max.d_y.d_offset += adjustment;

    if (d_verticalAlignment == VerticalAlignment::Bottom)
    {
        outArea.d_max.d_y.d_offset += adjustment;
        outArea.d_min.d_y.d_offset += adjustment;
    }
    else if (d_verticalAlignment == VerticalAlignment::Centre)
    {
        outArea.d_max.d_y.d_offset += adjustment * 0.5f;
        outArea.d_min.d_y.d_offset += adjustment * 0.5f;
    }

    // move the dragging point so cursor remains 'attached' to edge of window
    d_dragPoint.y += (newHeight - d_pixelSize.d_height);
}

//----------------------------------------------------------------------------//
void FrameWindow::setCursorForSizingLocation(SizingLocation location) const
{
    switch(location)
    {
        case SizingLocation::Top:
        case SizingLocation::Bottom:
            getGUIContext().setCursorImage(d_nsSizingCursor);
            break;

        case SizingLocation::Left:
        case SizingLocation::Right:
            getGUIContext().setCursorImage(d_ewSizingCursor);
            break;

        case SizingLocation::TopLeft:
        case SizingLocation::BottomRight:
            getGUIContext().setCursorImage(d_nwseSizingCursor);
            break;

        case SizingLocation::TopRight:
        case SizingLocation::BottomLeft:
            getGUIContext().setCursorImage(d_neswSizingCursor);
            break;

        default:
            getGUIContext().setCursorImage(getEffectiveCursor());
            break;
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::onRollupToggled(WindowEventArgs& e)
{
    invalidate(!d_rolledup);

    // TODO: need to check necessity of auto-sizing disabling
    notifyScreenAreaChanged(false);

    fireEvent(EventRollupToggled, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void FrameWindow::onCloseClicked(WindowEventArgs& e)
{
    fireEvent(EventCloseClicked, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void FrameWindow::onCursorMove(CursorMoveEventArgs& e)
{
    // default processing (this is now essential as it controls event firing).
    Window::onCursorMove(e);

    // if we are not the window containing the cursor, do NOT change the indicator
    const Window* wndUnderCursor = getGUIContext().getWindowContainingCursor();
    if (wndUnderCursor != this && wndUnderCursor != getTitlebar())
        return;

    if (isSizingEnabled())
    {
        SizingLocation dragEdge;
        if (d_beingSized)
        {
            dragEdge = getSizingBorderAtPoint(d_dragPoint);

            URect newArea(d_area);

            // size left or right edges
            const float deltaX = e.d_localPos.x - d_dragPoint.x;
            if (isLeftSizingLocation(dragEdge))
                moveLeftEdge(deltaX, newArea);
            else if (isRightSizingLocation(dragEdge))
                moveRightEdge(deltaX, newArea);

            // size top or bottom edges
            const float deltaY = e.d_localPos.y - d_dragPoint.y;
            if (isTopSizingLocation(dragEdge))
                moveTopEdge(deltaY, newArea);
            else if (isBottomSizingLocation(dragEdge))
                moveBottomEdge(deltaY, newArea);

            setArea(newArea);
        }
        else
        {
            dragEdge = getSizingBorderAtPoint(e.d_localPos);
        }

        // Update cursor every time because titlebar might reset it
        setCursorForSizingLocation(dragEdge);
    }

    ++e.handled;
}

//----------------------------------------------------------------------------//
void FrameWindow::onMouseButtonDown(MouseButtonEventArgs& e)
{
    // default processing (this is now essential as it controls event firing).
    Window::onMouseButtonDown(e);

    if (e.d_button == MouseButton::Left && isSizingEnabled() && getSizingBorderAtPoint(e.d_localPos) != SizingLocation::Invalid)
    {
        // ensure all inputs come to us from now on
        if (captureInput())
        {
            // setup the 'dragging' state variables
            d_beingSized = true;
            d_dragPoint = e.d_localPos;

            // do drag-sizing started notification
            WindowEventArgs args(this);
            onDragSizingStarted(args);

            ++e.handled;
        }
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::onMouseButtonUp(MouseButtonEventArgs& e)
{
    Window::onMouseButtonUp(e);

    if (e.d_button == MouseButton::Left)
    {
        releaseInput();
        ++e.handled;
    }
}

//----------------------------------------------------------------------------//
void FrameWindow::onCaptureLost(WindowEventArgs& e)
{
    // default processing (this is now essential as it controls event firing).
    Window::onCaptureLost(e);

    // reset sizing state
    d_beingSized = false;

    // do drag-sizing ended notification
    WindowEventArgs args(this);
    onDragSizingEnded(args);

    ++e.handled;
}

//----------------------------------------------------------------------------//
void FrameWindow::onTextChanged(WindowEventArgs& e)
{
    Window::onTextChanged(e);
    // pass this onto titlebar component.
    getTitlebar()->setText(getText());
    // maybe the user is using a fontdim for titlebar dimensions ;)
    performChildLayout(false, false);
}

//----------------------------------------------------------------------------//
void FrameWindow::onActivated(ActivationEventArgs& e)
{
    Window::onActivated(e);
    getTitlebar()->invalidate();
}

//----------------------------------------------------------------------------//
void FrameWindow::onDeactivated(ActivationEventArgs& e)
{
    Window::onDeactivated(e);
    getTitlebar()->invalidate();
}

//----------------------------------------------------------------------------//
void FrameWindow::setDragMovingEnabled(bool setting)
{
    if (d_dragMovable != setting)
    {
        d_dragMovable = setting;
        getTitlebar()->setDraggingEnabled(setting);
    }
}

//----------------------------------------------------------------------------//
bool FrameWindow::isHit(const glm::vec2& position, bool allow_disabled) const
{
    return !d_rolledup && Window::isHit(position, allow_disabled);
}

//----------------------------------------------------------------------------//
Titlebar* FrameWindow::getTitlebar() const
{
    return static_cast<Titlebar*>(getChild(TitlebarName));
}

//----------------------------------------------------------------------------//
PushButton* FrameWindow::getCloseButton() const
{
    return static_cast<PushButton*>(getChild(CloseButtonName));
}

//----------------------------------------------------------------------------//
void FrameWindow::onDragSizingStarted(WindowEventArgs& e)
{
    fireEvent(EventDragSizingStarted, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void FrameWindow::onDragSizingEnded(WindowEventArgs& e)
{
    fireEvent(EventDragSizingEnded, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void FrameWindow::addFrameWindowProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "SizingEnabled", "Property to get/set the state of the sizable setting for the FrameWindow. Value is either \"true\" or \"false\".",
        &FrameWindow::setSizingEnabled, &FrameWindow::isSizingEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "FrameEnabled", "Property to get/set the setting for whether the window frame will be displayed. Value is either \"true\" or \"false\".",
        &FrameWindow::setFrameEnabled, &FrameWindow::isFrameEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "TitlebarEnabled", "Property to get/set the setting for whether the window title-bar will be enabled (or displayed depending upon choice of final widget type). Value is either \"true\" or \"false\".",
        &FrameWindow::setTitleBarEnabled, &FrameWindow::isTitleBarEnabled, true
    ); // TODO: Inconsistency between Titlebar and TitleBar

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "CloseButtonEnabled", "Property to get/set the setting for whether the window close button will be enabled (or displayed depending upon choice of final widget type). Value is either \"true\" or \"false\".",
        &FrameWindow::setCloseButtonEnabled, &FrameWindow::isCloseButtonEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "RollUpEnabled", "Property to get/set the setting for whether the user is able to roll-up / shade the window. Value is either \"true\" or \"false\".",
        &FrameWindow::setRollupEnabled, &FrameWindow::isRollupEnabled, true
    ); // TODO: Inconsistency between RollUp and Rollup

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "RollUpState", "Property to get/set the roll-up / shade state of the window.  Value is either \"true\" or \"false\".",
        &FrameWindow::setRolledup, &FrameWindow::isRolledup, false /* TODO: Inconsistency */
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, bool,
        "DragMovingEnabled", "Property to get/set the setting for whether the user may drag the window around by its title bar. Value is either \"true\" or \"false\".",
        &FrameWindow::setDragMovingEnabled, &FrameWindow::isDragMovingEnabled, true
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, float,
        "SizingBorderThickness", "Property to get/set the setting for the sizing border thickness. Value is a float specifying the border thickness in pixels.",
        &FrameWindow::setSizingBorderThickness, &FrameWindow::getSizingBorderThickness, 8.0f
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, Image*,
        "NSSizingCursorImage", "Property to get/set the N-S (up-down) sizing cursor image for the FrameWindow. Value should be \"set:[imageset name] image:[image name]\".",
        &FrameWindow::setNSSizingIndicatorImage, &FrameWindow::getNSSizingIndicatorImage, nullptr
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, Image*,
        "EWSizingCursorImage", "Property to get/set the E-W (left-right) sizing cursor image for the FrameWindow. Value should be \"set:[imageset name] image:[image name]\".",
        &FrameWindow::setEWSizingIndicatorImage, &FrameWindow::getEWSizingIndicatorImage, nullptr
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, Image*,
        "NWSESizingCursorImage", "Property to get/set the NW-SE diagonal sizing cursor image for the FrameWindow. Value should be \"set:[imageset name] image:[image name]\".",
        &FrameWindow::setNWSESizingIndicatorImage, &FrameWindow::getNWSESizingIndicatorImage, nullptr
    );

    CEGUI_DEFINE_PROPERTY(FrameWindow, Image*,
        "NESWSizingCursorImage", "Property to get/set the NE-SW diagonal sizing cursor image for the FramwWindow. Value should be \"set:[imageset name] image:[image name]\".",
        &FrameWindow::setNESWSizingIndicatorImage, &FrameWindow::getNESWSizingIndicatorImage, nullptr
    );
}

}
