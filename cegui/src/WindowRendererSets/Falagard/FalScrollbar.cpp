/***********************************************************************
    filename:   FalScrollbar.cpp
    created:    Mon Jul 4 2005
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
#include "FalScrollbar.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIThumb.h"
#include "elements/CEGUIPushButton.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardScrollbar::TypeName[] = "Falagard/Scrollbar";
    FalagardScrollbarProperties::VerticalScrollbar FalagardScrollbar::d_verticalProperty;


    FalagardScrollbar::FalagardScrollbar(const String& type) :
        ScrollbarWindowRenderer(type),
        d_vertical(false)
    {
        registerProperty(&d_verticalProperty);
    }

    void FalagardScrollbar::render()
    {
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(d_window->isDisabled() ? "Disabled" : "Enabled");
        // peform the rendering operation.
        imagery->render(*d_window);
    }

    void FalagardScrollbar::performChildWindowLayout()
    {
        updateThumb();
    }

    void FalagardScrollbar::updateThumb(void)
    {
        Scrollbar* w = (Scrollbar*)d_window;
        const WidgetLookFeel& wlf = getLookNFeel();
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*w));

        Thumb* theThumb = w->getThumb();

        float posExtent = w->getDocumentSize() - w->getPageSize();
        float slideExtent;

        if (d_vertical)
        {
            slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;
            theThumb->setVertRange(area.d_top / w->getPixelSize().d_height, (area.d_top + slideExtent) / w->getPixelSize().d_height);
            theThumb->setPosition(UVector2(cegui_absdim(area.d_left),
                                                 cegui_reldim((area.d_top + (w->getScrollPosition() * (slideExtent / posExtent))) / w->getPixelSize().d_height)));
        }
        else
        {
            slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;
            theThumb->setHorzRange(area.d_left / w->getPixelSize().d_width, (area.d_left + slideExtent)  / w->getPixelSize().d_width);
            theThumb->setPosition(UVector2(cegui_reldim((area.d_left + (w->getScrollPosition() * (slideExtent / posExtent))) / w->getPixelSize().d_width),
                                                 cegui_absdim(area.d_top)));
        }
    }

    float FalagardScrollbar::getValueFromThumb(void) const
    {
        Scrollbar* w = (Scrollbar*)d_window;
        const WidgetLookFeel& wlf = getLookNFeel();
        Rect area(wlf.getNamedArea("ThumbTrackArea").getArea().getPixelRect(*w));

        Thumb* theThumb = w->getThumb();
        float posExtent = w->getDocumentSize() - w->getPageSize();

        if (d_vertical)
        {
            float slideExtent = area.getHeight() - theThumb->getPixelSize().d_height;
            return (theThumb->getYPosition().asAbsolute(w->getPixelSize().d_height) - area.d_top) / (slideExtent / posExtent);
        }
        else
        {
            float slideExtent = area.getWidth() - theThumb->getPixelSize().d_width;
            return (theThumb->getXPosition().asAbsolute(w->getPixelSize().d_width) - area.d_left) / (slideExtent / posExtent);
        }
    }

    float FalagardScrollbar::getAdjustDirectionFromPoint(const Point& pt) const
    {
        Scrollbar* w = (Scrollbar*)d_window;
        Rect absrect(w->getThumb()->getUnclippedPixelRect());

        if ((d_vertical && (pt.d_y > absrect.d_bottom)) ||
            (!d_vertical && (pt.d_x > absrect.d_right)))
        {
            return 1;
        }
        else if ((d_vertical && (pt.d_y < absrect.d_top)) ||
            (!d_vertical && (pt.d_x < absrect.d_left)))
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }

    bool FalagardScrollbar::isVertical() const
    {
        return d_vertical;
    }

    void FalagardScrollbar::setVertical(bool setting)
    {
        d_vertical = setting;
    }

} // End of  CEGUI namespace section
