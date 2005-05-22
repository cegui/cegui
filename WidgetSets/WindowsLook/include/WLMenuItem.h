/************************************************************************
    filename:   WLMenuItem.h
    created:    2/4/2005
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
#ifndef _WLMenuItem_h_
#define _WLMenuItem_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableFrame.h"
#include "CEGUIRenderableImage.h"
#include "elements/CEGUIMenuItem.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    MenubarItem class for the WindowsLook Gui Scheme
*/

class WINDOWSLOOK_API WLMenubarItem : public MenuItem
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
		Constructor for WLMenubarItem objects
	*/
	WLMenubarItem(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLMenubarItem objects
	*/
	virtual ~WLMenubarItem(void);


protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual void    drawSelf(float z);

	void	drawBackground(float z);


	/*!
	\brief
		Store the sizes for the frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	RenderableFrame		d_frame;		//!< Handles the frame for the MenubarItem.
	RenderableImage		d_fill;			//!< Handles the client clearing brush for the window.

	// stored sizes of frame regions
	float	d_frameLeftSize;			//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;			//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;				//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;			//!< Height of the bottom frame edge in pixels.	
};


/*!
\brief
    Factory class for producing WLMenubarItem objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLMenubarItemFactory : public WindowFactory
{
public:
    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    WLMenubarItemFactory(void) : WindowFactory(WLMenubarItem::WidgetTypeName)
    {}

    ~WLMenubarItemFactory(void)
    {}

    Window* createWindow(const String& name);

    void destroyWindow(Window* window);
};


///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


/*!
\brief
    PopupMenuItem class for the WindowsLook Gui Scheme
*/

class WINDOWSLOOK_API WLPopupMenuItem : public MenuItem
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

	static const utf8	ArrowLeftImageName[];				//!< Name of the image to use for indicating a child-popup.
	static const utf8	ArrowRightImageName[];				//!< Name of the image to use for indicating a child-popup.

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WLPopupMenuItem objects
	*/
	WLPopupMenuItem(const String& type, const String& name);


	/*!
	\brief
		Destructor for WLPopupMenuItem objects
	*/
	virtual ~WLPopupMenuItem(void);


	/*************************************************************************
		Pure functions from MenuItem
	*************************************************************************/
	/*!
	\brief
		Return the "optimal" size for the item
	
	\return
		Size describing the size in pixel that this TextItem's content requires
		for non-clipped rendering
	*/
	virtual Size getItemPixelSize(void);


protected:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual void    drawSelf(float z);

	void	drawBackground(float z);


	/*!
	\brief
		Store the sizes for the frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	RenderableFrame		d_frame;		//!< Handles the frame for the titlebar.
	RenderableImage		d_fill;			//!< Handles the client clearing brush for the window.
	RenderableImage		d_arrowLeft;	//!< Handles the popup indicator arrow when opened.
	RenderableImage		d_arrowRight;	//!< Handles the popup indicator arrow when closed.

	// stored sizes of frame regions
	float	d_frameLeftSize;			//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;			//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;				//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;			//!< Height of the bottom frame edge in pixels.	
};


/*!
\brief
    Factory class for producing WLPopupMenuItem objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLPopupMenuItemFactory : public WindowFactory
{
public:
    /*************************************************************************
        Construction and Destruction
    *************************************************************************/
    WLPopupMenuItemFactory(void) : WindowFactory(WLPopupMenuItem::WidgetTypeName)
    {}

    ~WLPopupMenuItemFactory(void)
    {}

    Window* createWindow(const String& name);

    void destroyWindow(Window* window);
};

} // End of  CEGUI namespace section


#endif  // end of guard _WLMenuItem_h_
