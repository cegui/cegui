/***********************************************************************
    created:    Sun Jul 3 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/WindowRendererSets/Core/Slider.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/widgets/Thumb.h"
#include "CEGUI/TplWindowRendererProperty.h"

namespace CEGUI
{
const String FalagardSlider::TypeName("Core/Slider");

//----------------------------------------------------------------------------//
FalagardSlider::FalagardSlider(const String& type) :
    SliderWindowRenderer(type)
{
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardSlider, bool,
        "VerticalSlider", "Property to get/set whether the Slider operates in the vertical direction."
        "  Value is boolean.",
        &FalagardSlider::setVertical, &FalagardSlider::isVertical,
        false);
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardSlider, bool,
        "ReversedDirection", "Property to get/set whether the Slider operates in reversed direction."
        "  Value is boolean.",
        &FalagardSlider::setReversedDirection, &FalagardSlider::isReversedDirection,
        false);
}

//----------------------------------------------------------------------------//
void FalagardSlider::createRenderGeometry()
{
    // TODO: use the same LnF for vertical and horizontal, rotate geometry here!

    const auto& lnf = getLookNFeel();
    const auto& imagery = lnf.getStateImagery(
        d_window->isEffectiveDisabled() ? "Disabled" :
        (d_window->isFocused() && lnf.isStateImageryPresent("EnabledFocused")) ? "EnabledFocused" :
        "Enabled");
    imagery.render(*d_window);
}

//----------------------------------------------------------------------------//
bool FalagardSlider::performChildWindowLayout()
{
    updateThumb();
    return true;
}

//----------------------------------------------------------------------------//
void FalagardSlider::updateThumb()
{
    Slider* w = static_cast<Slider*>(d_window);
    Thumb* theThumb = w->getThumb();

    // get area the thumb is supposed to use as it's area.
    const Rectf area(getLookNFeel().getNamedArea("ThumbTrackArea").getArea().getPixelRect(*w));

    const Sizef w_pixel_size(w->getPixelSize());

    // get base location for thumb widget
    const float thumbRelXPos = w_pixel_size.d_width == 0.0f ? 0.0f : (area.left() / w_pixel_size.d_width);
    const float thumbRelYPos = w_pixel_size.d_height == 0.0f ? 0.0f : (area.top() / w_pixel_size.d_height);
    UVector2 thumbPosition(cegui_reldim(thumbRelXPos), cegui_reldim(thumbRelYPos));

    const float fraction = w->getValueFraction();

    if (d_vertical)
    {
        // pixel extent of total available area the thumb moves in
        const float slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;

        // Set range of motion for the thumb widget
        if (w_pixel_size.d_height != 0.0f)
            theThumb->setVertRange(area.top()  / w_pixel_size.d_height,
                                    (area.top() + slideExtent) / w_pixel_size.d_height);
        else
            theThumb->setVertRange(0.0f, 0.0f);

        // calculate vertical positon for thumb
        const float thumbOffset = fraction * slideExtent;

        if (w_pixel_size.d_height != 0.0f)
            thumbPosition.d_y.d_scale +=
                (d_reversed ? thumbOffset : slideExtent - thumbOffset) / w_pixel_size.d_height;
    }
    else
    {
        // pixel extent of total available area the thumb moves in
        const float slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;

        // Set range of motion for the thumb widget
        if (w_pixel_size.d_width != 0.0f)
            theThumb->setHorzRange(area.left() / w_pixel_size.d_width,
                                    (area.left() + slideExtent) / w_pixel_size.d_width);
        else
            theThumb->setHorzRange(0.0f, 0.0f);


        // calculate horizontal positon for thumb
        const float thumbOffset = fraction * slideExtent;

        if (w_pixel_size.d_width != 0.0f)
            thumbPosition.d_x.d_scale +=
                (d_reversed ? slideExtent - thumbOffset : thumbOffset)  / w_pixel_size.d_width;
    }

    theThumb->setPosition(thumbPosition);
}

//----------------------------------------------------------------------------//
FalagardSlider::value_type FalagardSlider::getValueAtPoint(float pt) const
{
    const Slider* w = static_cast<Slider*>(d_window);
    const Thumb* theThumb = w->getThumb();

    // get area the thumb is supposed to use as it's area.
    const Rectf area(getLookNFeel().getNamedArea("ThumbTrackArea").getArea().getPixelRect(*w));

    float thumbCurrOffset;
    float thumbMaxOffset;
    if (d_vertical)
    {
        thumbCurrOffset = pt - area.top();
        thumbMaxOffset = area.getHeight() - theThumb->getPixelSize().d_height;
        if (!d_reversed)
            thumbCurrOffset = thumbMaxOffset - thumbCurrOffset;
    }
    else
    {
        thumbCurrOffset = pt - area.left();
        thumbMaxOffset = area.getWidth() - theThumb->getPixelSize().d_width;
        if (d_reversed)
            thumbCurrOffset = thumbMaxOffset - thumbCurrOffset;
    }

    // Linearly interpolate between min and max
    const float frac = thumbCurrOffset / thumbMaxOffset;
    return w->getMinimumValue() + frac * (w->getMaximumValue() - w->getMinimumValue());
}

//----------------------------------------------------------------------------//
FalagardSlider::value_type FalagardSlider::getValueFromThumb() const
{
    const Slider* w = static_cast<Slider*>(d_window);
    const Thumb* theThumb = static_cast<Slider*>(d_window)->getThumb();
    const float pt = d_vertical ?
        CoordConverter::asAbsolute(theThumb->getYPosition(), w->getPixelSize().d_height) :
        CoordConverter::asAbsolute(theThumb->getXPosition(), w->getPixelSize().d_width);
    return getValueAtPoint(pt);
}

//----------------------------------------------------------------------------//
float FalagardSlider::getAdjustDirectionFromPoint(const glm::vec2& pt) const
{
    Slider* w = static_cast<Slider*>(d_window);
    const Rectf& thumbRect = w->getThumb()->getUnclippedOuterRect().get();

    // Work with coords here to stop moving as soon as the cursor is over the thumb
    float dir;
    if ((d_vertical && (pt.y < thumbRect.top())) ||
        (!d_vertical && (pt.x > thumbRect.right())))
    {
        dir = d_reversed ? -1.0f : 1.0f;
    }
    else if ((d_vertical && (pt.y > thumbRect.bottom())) ||
        (!d_vertical && (pt.x < thumbRect.left())))
    {
        dir = d_reversed ? 1.0f : -1.0f;
    }
    else
    {
        return 0.f;
    }

    const auto& sliderPos = w->getUnclippedOuterRect().get().getPosition();
    const value_type ptValue = getValueAtPoint(d_vertical ? (pt.y - sliderPos.y) : (pt.x - sliderPos.x));
    const value_type currValue = w->getCurrentValue();
    value_type nextValue = currValue + dir * w->getStepSize();

    if (w->isDiscrete())
    {
        // Stop at the nearest value to avoid thumb oscillation
        nextValue = w->roundToStep(nextValue);
        return (std::abs(nextValue - ptValue) < std::abs(currValue - ptValue)) ? dir : 0.f;
    }
    else if (nextValue == currValue)
    {
        // If no step size is specified, jump to the cursor immediately
        w->setCurrentValue(ptValue);
        return 0.f;
    }
    else
    {
        // Magnet exactly to the cursor stepwise
        const float factor = static_cast<float>((ptValue - currValue) / (nextValue - currValue));
        return (factor > 1.f) ? dir : dir * factor;
    }
}

} // End of  CEGUI namespace section
