/************************************************************************
    filename:   WLPopupMenu.h
    created:    1/4/2005
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
#ifndef _WLPopupMenu_h_
#define _WLPopupMenu_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableFrame.h"
#include "CEGUIRenderableImage.h"
#include "elements/CEGUIPopupMenu.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Menubar class for the WindowsLook Gui Scheme
*/
class WINDOWSLOOK_API WLPopupMenu : public PopupMenu
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8   WidgetTypeName[];               //!< The unique typename of this widget

	// images
	static const utf8	ImagesetName[];					//!< Name of the Imageset containing the imagery to use.
	static const utf8	TopLeftFrameImageName[];		//!< Name of the image to use for the top-left corner.
	static const utf8	TopRightFrameImageName[];		//!< Name of the image to use for the top-right corner.
	static const utf8	BottomLeftFrameImageName[];		//!< Name of the image to use for the bottom-left corner.
	static const utf8	BottomRightFrameImageName[];	//!< Name of the image to use for the bottom-right corner.
	static const utf8	LeftFrameImageName[];			//!< Name of the image to use for the left edge.
	static const utf8	RightFrameImageName[];			//!< Name of the image to use for the right edge.
	static const utf8	TopFrameImageName[];			//!< Name of the image to use for the top edge.
	static const utf8	BottomFrameImageName[];			//!< Name of the image to use for the bottom edge.
	static const utf8	BackgroundImageName[];			//!< Name of the image to use as the background area brush.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WLPopupMenu objects
	*/
	WLPopupMenu(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLPopupMenu objects
	*/
	virtual ~WLPopupMenu(void);


protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Store the sizes for the frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Pure functions from Menubar
	*************************************************************************/
	/*!
	\brief
		Resize the PopupMenu to exactly fit the content that is attached to it.
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


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	RenderableFrame		d_frame;		//!< Handles the frame for the popup menu.
	RenderableImage		d_fill;			//!< Handles the client clearing brush for the window.

	// stored sizes of frame regions
	float	d_frameLeftSize;			//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;			//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;				//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;			//!< Height of the bottom frame edge in pixels.
};


/*!
\brief
    Factory class for producing Menubar objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLPopupMenuFactory : public WindowFactory
{
public:
    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    WLPopupMenuFactory(void) : WindowFactory(WLPopupMenu::WidgetTypeName)
    {}

    ~WLPopupMenuFactory(void)
    {}

    Window* createWindow(const String& name);

    void destroyWindow(Window* window);
};

} // End of  CEGUI namespace section


#endif  // end of guard _WLPopupMenu_h_
