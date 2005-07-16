/************************************************************************
	filename: 	TLScrollablePane.h
	created:	1/3/2005
	author:		Paul D Turner
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
#ifndef _TLScrollablePane_h_
#define _TLScrollablePane_h_

#include "elements/CEGUIScrollablePane.h"
#include "CEGUIWindowManager.h"
#include "TLMiniHorzScrollbar.h"
#include "TLMiniVertScrollbar.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    class TAHAREZLOOK_API TLScrollablePane : public ScrollablePane
    {
    public:
        /*************************************************************************
            Constants
        *************************************************************************/
        // type name for this widget
        static const utf8 WidgetTypeName[];     //!< The unique typename of this widget

        // component widget type names
        static const utf8* HorzScrollbarTypeName;  //!< Type name of widget to be created as horizontal scroll bar.
        static const utf8* VertScrollbarTypeName;  //!< Type name of widget to be created as vertical scroll bar.

        /*************************************************************************
        	Object construction and destruction.
        *************************************************************************/
        TLScrollablePane(const String& type, const String& name);
        ~TLScrollablePane(void);

    protected:
        /*************************************************************************
            Implementation of abstract methods in ScrollablePane
        *************************************************************************/
        Scrollbar* createHorizontalScrollbar(const String& name) const;
        Scrollbar* createVerticalScrollbar(const String& name) const;
        void performChildWindowLayout();
        Rect getViewableArea(void) const;

        /*************************************************************************
        	Implementation of abstract methods in Window
        *************************************************************************/
        void drawSelf(float z);

        /*************************************************************************
        	data fields
        *************************************************************************/
    };

    /*!
    \brief
        Factory class for producing TLScrollablePane objects
    */
    class TAHAREZLOOK_API TLScrollablePaneFactory : public WindowFactory
    {
    public:
        /*************************************************************************
            Construction and Destruction
        *************************************************************************/
        TLScrollablePaneFactory(void) : WindowFactory(TLScrollablePane::WidgetTypeName) { }
        ~TLScrollablePaneFactory(void){}

        Window*	createWindow(const String& name)
        {
            return new TLScrollablePane(d_type, name);
        }

        virtual void destroyWindow(Window* window)
        {
            if (window->getType() == d_type)
                delete window;
        }
    };


} // End of  CEGUI namespace section


#endif	// end of guard _TLScrollablePane_h_
