/************************************************************************
    filename:   WLTooltip.cpp
    created:    24/3/2005
    author:     Paul D Turner
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
#include "WLTooltip.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIImageset.h"
#include "CEGUIWindowManager.h"
#include "CEGUIFont.h"


// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 WLTooltip::WidgetTypeName[] = "WindowsLook/Tooltip";

    WLTooltip::WLTooltip(const String& type, const String& name) :
            Tooltip(type, name),
            d_backgroundCols(0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF),
            d_frameCols(0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF, 0x00FFFFFF)
    {}

    void WLTooltip::initialise(void)
    {
        Tooltip::initialise();

        Imageset* iset = ImagesetManager::getSingleton().getImageset("WindowsLook");

        d_frame.setImages(
            &iset->getImage("TooltipTopLeft"),
            &iset->getImage("TooltipTopRight"),
            &iset->getImage("TooltipBottomLeft"),
            &iset->getImage("TooltipBottomRight"),
            &iset->getImage("TooltipLeftEdge"),
            &iset->getImage("TooltipTopEdge"),
            &iset->getImage("TooltipRightEdge"),
            &iset->getImage("TooltipBottomEdge"));

        ColourRect colours(d_frameCols);
        colours.setAlpha(getEffectiveAlpha());
        d_frame.setColours(colours);

        // get sizes of frame edges
        d_left_width    = d_frame.getImageForLocation(LeftEdge)->getWidth();
        d_right_width   = d_frame.getImageForLocation(RightEdge)->getWidth();
        d_top_height    = d_frame.getImageForLocation(TopEdge)->getHeight();
        d_bottom_height = d_frame.getImageForLocation(BottomEdge)->getHeight();

        d_background = &iset->getImage("TooltipMiddle");
    }


    void WLTooltip::drawSelf(float z)
    {
        Rect clipper(getPixelRect());

        // do nothing if the widget is totally clipped.
        if (clipper.getWidth() == 0)
        {
            return;
        }

        Rect absrect(getUnclippedPixelRect());

        // draw frame
        d_frame.draw(Vector3(absrect.d_left, absrect.d_top, z), clipper);

        // adjust absrect and clipper so that later stages of render to not overwite frame
        absrect.d_left      += d_left_width;
        absrect.d_right     -= d_right_width;
        absrect.d_top       += d_top_height;
        absrect.d_bottom    -= d_bottom_height;

        clipper = clipper.getIntersection(absrect);

        // factor window alpha into colours to use when rendering background
        float alpha = getEffectiveAlpha();
        ColourRect colours(d_backgroundCols);
        colours.setAlpha(alpha);

        d_background->draw(absrect, z, clipper, colours);

        if (d_target)
        {
            colours.setColours((static_cast<argb_t>(alpha * 255) << 24) | 0x000000);
            // render text
            getFont()->drawText(d_target->getTooltipText(), absrect, System::getSingleton().getRenderer()->getZLayer(1), clipper, LeftAligned, colours);
        }
    }

    void WLTooltip::onSized(WindowEventArgs& e)
    {
        // update size to account for frame;
        Size sz(getAbsoluteSize());
        sz.d_width += PixelAligned(d_left_width + d_right_width);
        sz.d_height += PixelAligned(d_top_height + d_bottom_height);

        d_abs_area.setSize(sz);
        d_abs_area.constrainSize(d_maxSize, d_minSize);

        // update Rect for the other metrics system.
        d_rel_area.setSize(absoluteToRelative_impl(d_parent, sz));

        // update frame size.
        d_frame.setSize(sz);

        // base class processing
        Tooltip::onSized(e);

        e.handled = true;
    }


    void WLTooltip::onAlphaChanged(WindowEventArgs& e)
    {
        Tooltip::onAlphaChanged(e);

        float alpha = getEffectiveAlpha();

        ColourRect colours(d_frameCols);
        colours.setAlpha(alpha);
        d_frame.setColours(colours);
    }



    //////////////////////////////////////////////////////////////////////////
    /*************************************************************************
     
        Factory Methods
     
    *************************************************************************/
    //////////////////////////////////////////////////////////////////////////

    Window* WLTooltipFactory::createWindow(const String& name)
    {
        WLTooltip* wnd = new WLTooltip(d_type, name);
        wnd->initialise();

        return wnd;
    }

} // End of  CEGUI namespace section
