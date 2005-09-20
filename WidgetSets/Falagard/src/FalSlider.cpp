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
    const utf8 FalagardSlider::WidgetTypeName[] = "Falagard/Slider";
    FalagardSliderProperties::VerticalSlider FalagardSlider::d_verticalProperty;
    FalagardSliderProperties::ReversedDirection FalagardSlider::d_reversedProperty;


    FalagardSlider::FalagardSlider(const String& type, const String& name) :
        Slider(type, name),
        d_vertical(false),
        d_reversed(false)
    {
        addProperty(&d_verticalProperty);
        addProperty(&d_reversedProperty);
    }

    FalagardSlider::~FalagardSlider()
    {
    }

    void FalagardSlider::populateRenderCache()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(isDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*this);
    }

    Thumb* FalagardSlider::createThumb(const String& name) const
    {
        // return thumb created by look'n'feel assignment.
        return static_cast<Thumb*>(WindowManager::getSingleton().getWindow(name));
    }

    void FalagardSlider::performChildWindowLayout()
    {
        Slider::performChildWindowLayout();
        updateThumb();
    }

    void FalagardSlider::updateThumb(void)
    {
        // get area the thumb is supposed to use as it's area.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*this));
        // get accesss to the thumb
        Thumb* theThumb = static_cast<Thumb*>(WindowManager::getSingleton().getWindow(getName() + "__auto_thumb__"));

        // get base location for thumb widget
        UVector2 thumbPosition(cegui_absdim(area.d_left), cegui_absdim(area.d_top));

        // Is this a vertical slider
        if (d_vertical)
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;
            // Set range of motion for the thumb widget
            theThumb->setVertRange(area.d_top, area.d_top + slideExtent);

            // calculate vertical positon for thumb
            float thumbOffset = d_value * (slideExtent / d_maxValue);
            thumbPosition.d_y.d_offset += d_reversed ? thumbOffset : slideExtent - thumbOffset;
        }
        // Horizontal slider
        else
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;
            // Set range of motion for the thumb widget
            theThumb->setHorzRange(area.d_left, area.d_left + slideExtent);

            // calculate horizontal positon for thumb
            float thumbOffset = d_value * (slideExtent / d_maxValue);
            thumbPosition.d_x.d_offset += d_reversed ? slideExtent - thumbOffset : thumbOffset;
        }

        // set new position for thumb.
        theThumb->setWindowPosition(thumbPosition);
    }

    float FalagardSlider::getValueFromThumb(void) const
    {
        // get area the thumb is supposed to use as it's area.
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*this));
        // get accesss to the thumb
        Thumb* theThumb = static_cast<Thumb*>(WindowManager::getSingleton().getWindow(getName() + "__auto_thumb__"));

        // slider is vertical
        if (d_vertical)
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;
            // calculate value represented by current thumb position
            float thumbValue = (theThumb->getWindowYPosition().asAbsolute(d_pixelSize.d_height) - area.d_top) / (slideExtent / d_maxValue);
            // return final thumb value according to slider settings
            return d_reversed ? thumbValue : d_maxValue - thumbValue;
        }
        // slider is horizontal
        else
        {
            // pixel extent of total available area the thumb moves in
            float slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;
            // calculate value represented by current thumb position
            float thumbValue = (theThumb->getWindowXPosition().asAbsolute(d_pixelSize.d_width) - area.d_left) / (slideExtent / d_maxValue);
            // return final thumb value according to slider settings
            return d_reversed ? d_maxValue - thumbValue : thumbValue;
        }
    }

    float FalagardSlider::getAdjustDirectionFromPoint(const Point& pt) const
    {
        Rect absrect(WindowManager::getSingleton().getWindow(getName() + "__auto_thumb__")->getUnclippedPixelRect());

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

    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************

        Factory Methods

    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////
    Window* FalagardSliderFactory::createWindow(const String& name)
    {
        return new FalagardSlider(d_type, name);
    }

    void FalagardSliderFactory::destroyWindow(Window* window)
    {
        delete window;
    }

} // End of  CEGUI namespace section
