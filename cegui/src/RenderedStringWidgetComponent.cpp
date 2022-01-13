/***********************************************************************
    created:    27/05/2009
    author:     Paul Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RenderedStringWidgetComponent.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/Image.h"
#include "CEGUI/Exceptions.h"

namespace CEGUI
{

//----------------------------------------------------------------------------//
RenderedStringWidgetComponent::RenderedStringWidgetComponent(const String& widget_name)
    : d_windowName(widget_name)
    , d_windowPtrSynched(false)
{
}

//----------------------------------------------------------------------------//
RenderedStringWidgetComponent::RenderedStringWidgetComponent(Window* widget)
    : d_window(widget)
{
}

//----------------------------------------------------------------------------//
void RenderedStringWidgetComponent::setWindow(const String& widget_name)
{
    d_windowName = widget_name;
    d_windowPtrSynched = false;
    d_window = nullptr;
}

//----------------------------------------------------------------------------//
void RenderedStringWidgetComponent::setWindow(Window* widget)
{
    d_window = widget;
    d_windowPtrSynched = true;
}

//----------------------------------------------------------------------------//
void RenderedStringWidgetComponent::setSelection(const Window* /*refWnd*/,
                                                 const float start,
                                                 const float end)
{
    d_selected = (start != end);
}

//----------------------------------------------------------------------------//
std::vector<GeometryBuffer*> RenderedStringWidgetComponent::createRenderGeometry(
    const Window* refWnd,
    const glm::vec2& position,
    const CEGUI::ColourRect* /*mod_colours*/,
    const Rectf* clip_rect,
    const float vertical_space,
    const float /*space_extra*/) const
{
    std::vector<GeometryBuffer*> geomBuffers;

    Window* const window = getEffectiveWindow(refWnd);
    if (!window)
        return geomBuffers;

    glm::vec2 final_pos(position);
    switch (d_verticalTextFormatting)
    {
        case VerticalTextFormatting::BottomAligned:
            final_pos.y += vertical_space - getPixelSize(refWnd).d_height;
            break;
        case VerticalTextFormatting::CentreAligned:
            final_pos.y += (vertical_space - getPixelSize(refWnd).d_height) / 2.f;
            break;
    }

    // render the selection if needed
    if (d_selectionImage && d_selected)
    {
        const Rectf select_area(position, getPixelSize(refWnd));
        ImageRenderSettings imgRenderSettings(select_area, clip_rect, true, ColourRect(0xFF002FFF));
        auto geom = d_selectionImage->createRenderGeometry(imgRenderSettings);
        geomBuffers.insert(geomBuffers.end(), geom.begin(), geom.end());
    }

    // re-adjust for inner-rect of parent
    if (const Window* parent = window->getParent())
        final_pos -= (parent->getUnclippedInnerRect().get().d_min - parent->getUnclippedOuterRect().get().d_min);

    // we do not actually draw the widget, we just move it into position.
    const glm::vec2 wposAbs = final_pos + d_padding.d_min;
    window->setPosition(UVector2(UDim(0, wposAbs.x), UDim(0, wposAbs.y)));

    return geomBuffers;
}

//----------------------------------------------------------------------------//
Window* RenderedStringWidgetComponent::getEffectiveWindow(const Window* refWnd) const
{
    if (!d_windowPtrSynched)
    {
        if (!refWnd)
            return nullptr;

        d_window = refWnd->getChild(d_windowName);
        d_windowPtrSynched = true;
    }
    
    return d_window;
}

//----------------------------------------------------------------------------//
Sizef RenderedStringWidgetComponent::getPixelSize(const Window* refWnd) const
{
    Sizef sz(0.f, 0.f);

    if (auto window = getEffectiveWindow(refWnd))
    {
        sz = window->getPixelSize();
        sz.d_width += (d_padding.d_min.x + d_padding.d_max.x);
        sz.d_height += (d_padding.d_min.y + d_padding.d_max.y);
    }

    return sz;
}

//----------------------------------------------------------------------------//
RenderedStringComponentPtr RenderedStringWidgetComponent::split(
    const Window* /*refWnd*/, float /*split_point*/, bool /*first_component*/,
    bool& /*was_word_split*/)
{
    throw InvalidRequestException("this component does not support being split.");
}

//----------------------------------------------------------------------------//
RenderedStringComponentPtr RenderedStringWidgetComponent::clone() const
{
    return std::make_unique<RenderedStringWidgetComponent>(*this);
}

}
