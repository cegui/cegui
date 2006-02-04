/************************************************************************
    filename:   FalSlider.cpp
    created:    Sun Jul 3 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "FalSlider.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIThumb.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardSlider::TypeName[] = "Falagard/Slider";
    FalagardSliderProperties::VerticalSlider FalagardSlider::d_verticalProperty;
    FalagardSliderProperties::ReversedDirection FalagardSlider::d_reversedProperty;


    FalagardSlider::FalagardSlider(const String& type) :
        SliderWindowRenderer(type),
        d_vertical(false),
        d_reversed(false)
    {
        registerProperty(&d_verticalProperty);
        registerProperty(&d_reversedProperty);
    }

    void FalagardSlider::render()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(d_window->isDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*d_window);
    }

    void FalagardSlider::performChildWindowLayout()
    {
        updateThumb();
    }

    void FalagardSlider::updateThumb(void)
    {
        Slider* w = (Slider*)d_window;
        // get area the thumb is supposed to use as it's area.
        const WidgetLookFeel& wlf = getLookNFeel();
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*w));
        // get accesss to the thumb
        Thumb* theThumb = w->getThumb();

        // get base location for thumb widget
        UVector2 thumbPosition(cegui_absdim(area.d_left), cegui_absdim(area.d_top));

        // Is this a vertical slider
        if (d_vertical)
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;
            // Set range of motion for the thumb widget
            theThumb->setVertRange(area.d_top  / w->getPixelSize().d_height, (area.d_top + slideExtent) / w->getPixelSize().d_height);

            // calculate vertical positon for thumb
            float thumbOffset = w->getCurrentValue() * (slideExtent / w->getMaxValue());
            thumbPosition.d_y.d_scale += (d_reversed ? thumbOffset : slideExtent - thumbOffset) / w->getPixelSize().d_height;
        }
        // Horizontal slider
        else
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;
            // Set range of motion for the thumb widget
            theThumb->setHorzRange(area.d_left / w->getPixelSize().d_width, (area.d_left + slideExtent) / w->getPixelSize().d_width);

            // calculate horizontal positon for thumb
            float thumbOffset = w->getCurrentValue() * (slideExtent / w->getMaxValue());
            thumbPosition.d_x.d_scale += (d_reversed ? slideExtent - thumbOffset : thumbOffset)  / w->getPixelSize().d_width;
        }

        // set new position for thumb.
        theThumb->setWindowPosition(thumbPosition);
    }

    float FalagardSlider::getValueFromThumb(void) const
    {
        Slider* w = (Slider*)d_window;
        // get area the thumb is supposed to use as it's area.
        const WidgetLookFeel& wlf = getLookNFeel();
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*w));
        // get accesss to the thumb
        Thumb* theThumb = w->getThumb();

        // slider is vertical
        if (d_vertical)
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;
            // calculate value represented by current thumb position
            float thumbValue = (theThumb->getWindowYPosition().asAbsolute(w->getPixelSize().d_height) - area.d_top) / (slideExtent / w->getMaxValue());
            // return final thumb value according to slider settings
            return d_reversed ? thumbValue : w->getMaxValue() - thumbValue;
        }
        // slider is horizontal
        else
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;
            // calculate value represented by current thumb position
            float thumbValue = (theThumb->getWindowXPosition().asAbsolute(w->getPixelSize().d_width) - area.d_left) / (slideExtent / w->getMaxValue());
            // return final thumb value according to slider settings
            return d_reversed ? w->getMaxValue() - thumbValue : thumbValue;
        }
    }

    float FalagardSlider::getAdjustDirectionFromPoint(const Point& pt) const
    {
        Slider* w = (Slider*)d_window;
        Rect absrect(w->getThumb()->getUnclippedPixelRect());

        if ((d_vertical && (pt.d_y < absrect.d_top)) ||
            (!d_vertical && (pt.d_x > absrect.d_right)))
        {
            return d_reversed ? -1.0f : 1.0f;
        }
        else if ((d_vertical && (pt.d_y > absrect.d_bottom)) ||
            (!d_vertical && (pt.d_x < absrect.d_left)))
        {
            return d_reversed ? 1.0f : -1.0f;
        }
        else
        {
            return 0;
        }
    }

    bool FalagardSlider::isVertical() const
    {
        return d_vertical;
    }

    void FalagardSlider::setVertical(bool setting)
    {
        d_vertical = setting;
    }

    bool FalagardSlider::isReversedDirection() const
    {
        return d_reversed;
    }

    void FalagardSlider::setReversedDirection(bool setting)
    {
        d_reversed = setting;
    }

} // End of  CEGUI namespace section
