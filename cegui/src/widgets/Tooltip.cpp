/***********************************************************************
created:    21/2/2005
author:     Paul D Turner
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
#include "CEGUI/widgets/Tooltip.h"
#include "CEGUI/Image.h"
#include "CEGUI/GUIContext.h"

namespace CEGUI
{
const String Tooltip::WidgetTypeName("CEGUI/Tooltip");

//////////////////////////////////////////////////////////////////////////
// Event name constants
const String Tooltip::EventNamespace("Tooltip");
const String Tooltip::EventHoverTimeChanged("HoverTimeChanged");
const String Tooltip::EventDisplayTimeChanged("DisplayTimeChanged");
const String Tooltip::EventFadeTimeChanged("FadeTimeChanged");
const String Tooltip::EventTooltipActive("TooltipActive");
const String Tooltip::EventTooltipInactive("TooltipInactive");
const String Tooltip::EventTooltipTransition("TooltipTransition");
//////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------//
TooltipWindowRenderer::TooltipWindowRenderer(const String& name) :
    WindowRenderer(name, Tooltip::EventNamespace)
{
}

//----------------------------------------------------------------------------//
Tooltip::Tooltip(const String& type, const String& name) :
    Window(type, name)
{
    addTooltipProperties();

    setClippedByParent(false);
    setDestroyedByParent(false);
    setAlwaysOnTop(true);
    setCursorPassThroughEnabled(true);

    // we need updates even when not visible
    setUpdateMode(WindowUpdateMode::Always);

    switchToInactiveState();
    hide();
}

//----------------------------------------------------------------------------//
void Tooltip::positionSelf()
{
    if (!d_guiContext || d_inPositionSelf)
        return;

    d_inPositionSelf = true;

    const Sizef screenSize = getRootContainerSize();
    const Image* cursor_image = d_guiContext->getCursor().getImage();

    const glm::vec2 cursor_pos(d_guiContext->getCursor().getPosition());
    const Sizef cursor_size = cursor_image ? cursor_image->getRenderedSize() : Sizef(0.f, 0.f);

    glm::vec2 tmpPos(cursor_pos.x + cursor_size.d_width, cursor_pos.y + cursor_size.d_height);
    Rectf tipRect(tmpPos, getUnclippedOuterRect().get().getSize());

    // if the tooltip would be off more at the right side of the screen,
    // reposition to the other side of the cursor.
    if (screenSize.d_width - tipRect.right() < tipRect.left() - tipRect.getWidth())
        tmpPos.x = cursor_pos.x - tipRect.getWidth() - 5;

    // if the tooltip would be off more at the bottom side of the screen,
    // reposition to the other side of the cursor.
    if (screenSize.d_height - tipRect.bottom() < tipRect.top() - tipRect.getHeight())
        tmpPos.y = cursor_pos.y - tipRect.getHeight() - 5;

    // prevent being cut off at edges
    tmpPos.x = std::max(0.0f, std::min(tmpPos.x, screenSize.d_width - tipRect.getWidth()));
    tmpPos.y = std::max(0.0f, std::min(tmpPos.y, screenSize.d_height - tipRect.getHeight()));

    // set final position of tooltip window.
    setPosition(UVector2(cegui_absdim(tmpPos.x), cegui_absdim(tmpPos.y)));

    d_inPositionSelf = false;
}

//----------------------------------------------------------------------------//
void Tooltip::sizeSelf()
{
    const Sizef textSize = getTextSize();
    setSize(USize(cegui_absdim(textSize.d_width), cegui_absdim(textSize.d_height)));
}

//----------------------------------------------------------------------------//
Sizef Tooltip::getTextSize() const
{
    return d_windowRenderer ?
        static_cast<TooltipWindowRenderer*>(d_windowRenderer)->getTextSize() :
        getTextSize_impl();
}

//----------------------------------------------------------------------------//
void Tooltip::setHoverTime(float seconds)
{
    if (d_hoverTime == seconds)
        return;

    d_hoverTime = seconds;

    WindowEventArgs args(this);
    onHoverTimeChanged(args);
}

//----------------------------------------------------------------------------//
void Tooltip::setDisplayTime(float seconds)
{
    if (d_displayTime == seconds)
        return;

    d_displayTime = seconds;

    WindowEventArgs args(this);
    onDisplayTimeChanged(args);
}

//----------------------------------------------------------------------------//
bool Tooltip::validateWindowRenderer(const WindowRenderer* renderer) const
{
    return dynamic_cast<const TooltipWindowRenderer*>(renderer) != nullptr;
}

//----------------------------------------------------------------------------//
void Tooltip::setTargetWindow(Window* wnd)
{
    if (wnd == d_target || wnd == this || (wnd && !wnd->getGUIContextPtr()))
        return;

    d_target = wnd;

    // All necessary checks are performed inside addChild
    if (wnd)
        wnd->getGUIContextPtr()->getRootWindow()->addChild(this);

    // Refresh already active tooltip immediately
    if (d_active)
        switchToActiveState();
}

//----------------------------------------------------------------------------//
void Tooltip::updateSelf(float elapsed)
{
    Window::updateSelf(elapsed);

    const bool needTooltip = d_target &&
        d_target->isTooltipEnabled() &&
        !d_target->getTooltipTextIncludingInheritance().empty();

    if (d_active)
    {
        // if no tooltip must be shown, switch immediately to inactive state
        if (!needTooltip)
        {
            hide();
            switchToInactiveState();
        }
        else if (d_displayTime > 0.f) // else see if display timeout has been reached
        {
            d_elapsed += elapsed;
            if (d_elapsed >= d_displayTime)
                switchToInactiveState();
        }
    }
    else if (needTooltip)
    {
        d_elapsed += elapsed;
        if (d_elapsed >= d_hoverTime)
            switchToActiveState();
    }
}

//----------------------------------------------------------------------------//
void Tooltip::switchToInactiveState()
{
    d_active = false;
    d_elapsed = 0.f;

    // fire event before target gets reset in case that information is required in handler.
    WindowEventArgs args(this);
    onTooltipInactive(args);

    d_target = nullptr;
}

//----------------------------------------------------------------------------//
void Tooltip::switchToActiveState()
{
    if (!d_target)
        return;

    // Set text to that of the tooltip text of the target.
    const String& newText = d_target->getTooltipTextIncludingInheritance();
    if (newText != d_textLogical)
    {
        setText(newText);
    }
    else
    {
        // Just in case the font is different, otherwise is called from onTextChanged
        sizeSelf();
        positionSelf();
    }

    show();

    d_elapsed = 0.f;

    WindowEventArgs args(this);
    if (d_active)
    {
        onTooltipTransition(args);
    }
    else
    {
        d_active = true;
        onTooltipActive(args);
    }
}

//----------------------------------------------------------------------------//
void Tooltip::onHidden(WindowEventArgs& e)
{
    Window::onHidden(e);

    // The animation will take care of fade out or whatnot,
    // at the end it will hide the tooltip to let us know the transition
    // is done. At this point we will remove the tooltip from the
    // previous parent.

    // NOTE: There has to be a fadeout animation! Even if it's a 0 second
    //       immediate hide animation.

    if (d_parent)
        d_parent->removeChild(this);
}

//----------------------------------------------------------------------------//
void Tooltip::onCursorEnters(CursorInputEventArgs& e)
{
    positionSelf();

    Window::onCursorEnters(e);
}

//----------------------------------------------------------------------------//
void Tooltip::onTextChanged(WindowEventArgs& e)
{
    Window::onTextChanged(e);

    // set size and position of the tooltip window to consider new text
    sizeSelf();
    positionSelf();

    // we do not signal we handled it, in case user wants to hear
    // about text changes too.
}

//----------------------------------------------------------------------------//
void Tooltip::onHoverTimeChanged(WindowEventArgs& e)
{
    fireEvent(EventHoverTimeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Tooltip::onDisplayTimeChanged(WindowEventArgs& e)
{
    fireEvent(EventDisplayTimeChanged, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Tooltip::onTooltipActive(WindowEventArgs& e)
{
    fireEvent(EventTooltipActive, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Tooltip::onTooltipInactive(WindowEventArgs& e)
{
    fireEvent(EventTooltipInactive, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Tooltip::onTooltipTransition(WindowEventArgs& e)
{
    fireEvent(EventTooltipTransition, e, EventNamespace);
}

//----------------------------------------------------------------------------//
void Tooltip::addTooltipProperties()
{
    const String& propertyOrigin = WidgetTypeName;

    CEGUI_DEFINE_PROPERTY(Tooltip, float,
        "HoverTime", "Property to get/set the hover timeout value in seconds.  Value is a float.",
        &Tooltip::setHoverTime, &Tooltip::getHoverTime, 0.4f
    );

    CEGUI_DEFINE_PROPERTY(Tooltip, float,
        "DisplayTime", "Property to get/set the display timeout value in seconds.  Value is a float.",
        &Tooltip::setDisplayTime, &Tooltip::getDisplayTime, 7.5f
    );
}

}
