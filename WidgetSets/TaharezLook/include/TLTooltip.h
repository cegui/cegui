/************************************************************************
    filename:   TLTooltip.h
    created:    21/3/2005
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
#ifndef _TLTooltip_h_
#define _TLTooltip_h_

#include "CEGUIRenderableFrame.h"
#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUITooltip.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    /*!
    \brief
        Tooltip class for the TaharezLook Gui Scheme
    */
    class TAHAREZLOOK_API TLTooltip : public Tooltip
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
        TLTooltip(const String& type, const String& name);
        ~TLTooltip(void)
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
        Factory class for producing Tooltip objects for the Taharez GUI Scheme
    */
    class TAHAREZLOOK_API TLTooltipFactory : public WindowFactory
    {
    public:
        /*************************************************************************
            Construction and Destruction
        *************************************************************************/
        TLTooltipFactory(void) : WindowFactory(TLTooltip::WidgetTypeName)
        {}

        ~TLTooltipFactory(void)
        {}

        Window* createWindow(const String& name);

        void destroyWindow(Window* window)
        {
            if (window->getType() == d_type) delete window;
        }
    };

} // End of  CEGUI namespace section


#endif  // end of guard _TLTooltip_h_
