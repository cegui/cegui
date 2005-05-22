/************************************************************************
    filename:   WLTextItem.h
    created:    31/3/2005
    author:     Tomas Lindquist Olsen
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
#ifndef _WLTextItem_h_
#define _WLTextItem_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUITextItem.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    TextItem class for the WindowsLook Gui Scheme
*/
class WINDOWSLOOK_API WLTextItem : public TextItem
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8   WidgetTypeName[];               //!< The unique typename of this widget


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WLTextItem objects
	*/
	WLTextItem(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLTextItem objects
	*/
	virtual ~WLTextItem(void);


protected:
};


/*!
\brief
    Factory class for producing TextItem objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLTextItemFactory : public WindowFactory
{
public:
    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    WLTextItemFactory(void) : WindowFactory(WLTextItem::WidgetTypeName)
    {}

    ~WLTextItemFactory(void)
    {}

    Window* createWindow(const String& name);

    void destroyWindow(Window* window)
    {
        if (window->getType() == d_type) delete window;
    }
};

} // End of  CEGUI namespace section


#endif  // end of guard _WLTextItem_h_
