/************************************************************************
    filename:   WLMenubar.h
    created:    31/3/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
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
#ifndef _WLMenubar_h_
#define _WLMenubar_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIMenubar.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Menubar class for the WindowsLook Gui Scheme
*/
class WINDOWSLOOK_API WLMenubar : public Menubar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8   WidgetTypeName[];               //!< The unique typename of this widget

	// images
	static const utf8	ImagesetName[];					//!< Name of the Imageset containing the imagery to use.
	static const utf8	BackgroundImageName[];			//!< Name of the image to use as the background area brush.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WLMenubar objects
	*/
	WLMenubar(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLMenubar objects
	*/
	virtual ~WLMenubar(void);


protected:
	/*************************************************************************
		Pure functions from Menubar
	*************************************************************************/
	/*!
	\brief
		Resize the Menubar to exactly fit the content that is attached to it.
		Return a Rect object describing, in un-clipped pixels, the window relative area
		that is to be used for rendering list items.

	\return
		Nothing.
	*/
	virtual	void	sizeToContent_impl(void);


	/*!
	\brief
		Return a Rect object describing, in un-clipped pixels, the window relative area
		that is to be used for rendering list items.

	\return
		Rect object describing the area of the Window to be used for rendering
		list box items.
	*/
	virtual	Rect	getItemRenderArea(void) const;


	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual	void	drawSelf(float z);
};


/*!
\brief
    Factory class for producing Menubar objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLMenubarFactory : public WindowFactory
{
public:
    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    WLMenubarFactory(void) : WindowFactory(WLMenubar::WidgetTypeName)
    {}

    ~WLMenubarFactory(void)
    {}

    Window* createWindow(const String& name);

    void destroyWindow(Window* window);
};

} // End of  CEGUI namespace section


#endif  // end of guard _WLMenubar_h_
