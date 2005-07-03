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


    FalagardSlider::FalagardSlider(const String& type, const String& name) :
        Slider(type, name),
        d_vertical(false)
    {
        addProperty(&d_verticalProperty);
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
        imagery = &wlf.getStateImagery(d_enabled ? "Enabled" : "Disabled");
        // peform the rendering operation.
        imagery->render(*this);
    }

    Thumb* FalagardSlider::createThumb(const String& name) const
    {
        // return thumb created by look'n'feel assignment.
        return static_cast<Thumb*>(WindowManager::getSingleton().getWindow(name));
    }

    void FalagardSlider::layoutComponentWidgets(void)
    {
        try
        {
            // get WidgetLookFeel for the assigned look.
            const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
            // get look'n'feel to layout any child windows it created.
            wlf.layoutChildWidgets(*this);
        }
        catch (UnknownObjectException)
        {}

        updateThumb();
    }

    void FalagardSlider::updateThumb(void)
    {
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*this));

        Thumb* theThumb = static_cast<Thumb*>(WindowManager::getSingleton().getWindow(getName() + "__auto_thumb__"));

        float fltVal    = d_value;
        float posExtent = d_maxValue;
        float slideExtent;

        if (d_vertical)
        {
            slideExtent = area.getHeight() - theThumb->getAbsoluteHeight();
            theThumb->setVertRange(absoluteToRelativeY_impl(this, area.d_top), absoluteToRelativeY_impl(this, area.d_top + slideExtent));
            theThumb->setPosition(Point(absoluteToRelativeX_impl(this, area.d_left), absoluteToRelativeY_impl(this, area.d_top + slideExtent - (fltVal * (slideExtent / posExtent)))));
        }
        else
        {
            slideExtent = area.getWidth() - theThumb->getAbsoluteWidth();
            theThumb->setHorzRange(absoluteToRelativeX_impl(this, area.d_left), absoluteToRelativeX_impl(this, area.d_left + slideExtent));
            theThumb->setPosition(Point(absoluteToRelativeX_impl(this, area.d_left + slideExtent - (fltVal * (slideExtent / posExtent))), absoluteToRelativeY_impl(this, area.d_top)));
        }
    }

    float FalagardSlider::getValueFromThumb(void) const
    {
        const WidgetLookFeel& wlf = WidgetLookManager::getSingleton().getWidgetLook(d_lookName);
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*this));

        Thumb* theThumb = static_cast<Thumb*>(WindowManager::getSingleton().getWindow(getName() + "__auto_thumb__"));
        float posExtent = d_maxValue;

        if (d_vertical)
        {
            float slideExtent = area.getHeight() - theThumb->getAbsoluteHeight();
            return d_maxValue - ((theThumb->getAbsoluteYPosition() - area.d_top) / (slideExtent / posExtent));
        }
        else
        {
            float slideExtent = area.getWidth() - theThumb->getAbsoluteWidth();
            return d_maxValue - ((theThumb->getAbsoluteXPosition() - area.d_left) / (slideExtent / posExtent));
        }
    }

    float FalagardSlider::getAdjustDirectionFromPoint(const Point& pt) const
    {
        Rect absrect(WindowManager::getSingleton().getWindow(getName() + "__auto_thumb__")->getUnclippedPixelRect());

        if ((d_vertical && (pt.d_y < absrect.d_top)) ||
            (!d_vertical && (pt.d_x < absrect.d_left)))
        {
            return 1;
        }
        else if ((d_vertical && (pt.d_y > absrect.d_bottom)) ||
            (!d_vertical && (pt.d_x > absrect.d_right)))
        {
            return -1;
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
        if (window->getType() == d_type)
            delete window;
    }

} // End of  CEGUI namespace section
