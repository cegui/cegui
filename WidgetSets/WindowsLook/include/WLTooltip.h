/************************************************************************
    filename:   WLTooltip.h
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
#ifndef _WLTooltip_h_
#define _WLTooltip_h_

#include "CEGUIRenderableFrame.h"
#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUITooltip.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    /*!
    \brief
        Tooltip class for the WindowsLook Gui Scheme
    */
    class WINDOWSLOOK_API WLTooltip : public Tooltip
    {
    public:
        /*************************************************************************
            Constants
        *************************************************************************/
        // type name for this widget
        static const utf8   WidgetTypeName[];               //!< The unique typename of this widget


        /*************************************************************************
            Construction / Destruction
        *************************************************************************/
        WLTooltip(const String& type, const String& name);
        ~WLTooltip(void)
        {}
        
        /*************************************************************************
            Overridden from Window
        *************************************************************************/
        void initialise(void);

    protected:
        /*************************************************************************
            Overridden from Window
        *************************************************************************/
        void drawSelf(float z);
        void onSized(WindowEventArgs& e);
        void onAlphaChanged(WindowEventArgs& e);
        
        const Image*        d_background;       //!< Image to use for widget background.
        RenderableFrame     d_frame;            //!< Frame object used for rendering a frame for the static element.
        ColourRect          d_backgroundCols;   //!< Colours to use when drawing background.
        ColourRect          d_frameCols;        //!< Used to store frame colours.

        // cache of frame edge sizes
        float   d_left_width;           //!< Width of the left edge image for the current frame.
        float   d_right_width;          //!< Width of the right edge image for the current frame.
        float   d_top_height;           //!< Height of the top edge image for the current frame.
        float   d_bottom_height;        //!< Height of the bottom edge image for the current frame.
    };


    /*!
    \brief
        Factory class for producing Tooltip objects for the WindowsLook GUI Scheme
    */
    class WINDOWSLOOK_API WLTooltipFactory : public WindowFactory
    {
    public:
        /*************************************************************************
            Construction and Destruction
        *************************************************************************/
        WLTooltipFactory(void) : WindowFactory(WLTooltip::WidgetTypeName)
        {}

        ~WLTooltipFactory(void)
        {}

        Window* createWindow(const String& name);

        void destroyWindow(Window* window)
        {
            if (window->getType() == d_type) delete window;
        }
    };

} // End of  CEGUI namespace section


#endif  // end of guard _WLTooltip_h_
