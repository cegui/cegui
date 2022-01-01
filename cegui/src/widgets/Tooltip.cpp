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

//----------------------------------------------------------------------------//
TooltipWindowRenderer::TooltipWindowRenderer(const String& name) :
    WindowRenderer(name, "Tooltip")
{
}

//----------------------------------------------------------------------------//
Tooltip::Tooltip(const String& type, const String& name) :
    Window(type, name)
{
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
    const Rectf tipRect(tmpPos, getUnclippedOuterRect().get().getSize());

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
bool Tooltip::validateWindowRenderer(const WindowRenderer* renderer) const
{
    return dynamic_cast<const TooltipWindowRenderer*>(renderer) != nullptr;
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

}
