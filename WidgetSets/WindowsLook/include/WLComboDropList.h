/************************************************************************
	filename: 	WLComboDropList.h
	created:	16/8/2004
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
#ifndef _WLComboDropList_h_
#define _WLComboDropList_h_

#include "WLModule.h"
#include "elements/CEGUIComboDropList.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableFrame.h"
#include "CEGUIRenderableImage.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Combobox Drop-down list class for the WindowsLook Gui Scheme.
*/
class WINDOWSLOOK_API WLComboDropList : public ComboDropList
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// image / imageset related
	static const utf8	ImagesetName[];				//!< Name of the imageset to use for rendering.
	static const utf8	TopLeftImageName[];			//!< Name of the image to use for the top-left corner of the box.
	static const utf8	TopRightImageName[];		//!< Name of the image to use for the top-right corner of the box.
	static const utf8	BottomLeftImageName[];		//!< Name of the image to use for the bottom left corner of the box.
	static const utf8	BottomRightImageName[];		//!< Name of the image to use for the bottom right corner of the box.
	static const utf8	LeftEdgeImageName[];		//!< Name of the image to use for the left edge of the box.
	static const utf8	RightEdgeImageName[];		//!< Name of the image to use for the right edge of the box.
	static const utf8	TopEdgeImageName[];			//!< Name of the image to use for the top edge of the box.
	static const utf8	BottomEdgeImageName[];		//!< Name of the image to use for the bottom edge of the box.
	static const utf8	BackgroundImageName[];		//!< Name of the image to use for the box background.
	static const utf8	SelectionBrushImageName[];	//!< Name of the image to use for the selection highlight brush.
	static const utf8	MouseCursorImageName[];		//!< Name of the image to use for the mouse cursor.

	// component widget type names
	static const utf8*	HorzScrollbarTypeName;		//!< Type name of widget to be created as horizontal scroll bar.
	static const utf8*	VertScrollbarTypeName;		//!< Type name of widget to be created as vertical scroll bar.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook Combobox drop-down list.
	*/
	WLComboDropList(const String& type, const String& name);


	/*!
	\brief
		Destructor for WindowsLook Combobox drop-down list.
	*/
	virtual ~WLComboDropList(void);


protected:

	protected:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Return a Rect object describing, in un-clipped pixels, the window relative area
		that is to be used for rendering list items.

	\return
		Rect object describing the area of the Window to be used for rendering
		list items.
	*/
	virtual	Rect	getListRenderArea(void) const;


	/*!
	\brief
		create and return a pointer to a Scrollbar widget for use as vertical scroll bar

	\return
		Pointer to a Scrollbar to be used for scrolling the list vertically.
	*/
	virtual Scrollbar*	createVertScrollbar(void) const;
 

	/*!
	\brief
		create and return a pointer to a Scrollbar widget for use as horizontal scroll bar

	\return
		Pointer to a Scrollbar to be used for scrolling the list horizontally.
	*/
	virtual Scrollbar*	createHorzScrollbar(void) const;


	/*!
	\brief
		Setup size and position for the component widgets attached to this Listbox

	\return
		Nothing.
	*/
	virtual void	layoutComponentWidgets();


	/*!
	\brief
		Perform rendering of the widget control frame and other 'static' areas.  This
		method should not render the actual items.  Note that the items are typically
		rendered to layer 3, other layers can be used for rendering imagery behind and
		infront of the items.

	\param z
		Z co-ordinate for layer 0.

	\return
		Nothing.
	*/
	virtual	void	renderListboxBaseImagery(float z);


	/*!
	\brief
		Store the sizes for the frame edges
	*/
	void	storeFrameSizes(void);


	/*************************************************************************
		Overridden Event handlers
	*************************************************************************/
	virtual void	onSized(WindowEventArgs& e);
	virtual void	onAlphaChanged(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	RenderableFrame	d_frame;		//!< Used for the frame of the drop-list box.
	RenderableImage	d_background;	//!< Used for the background area of the drop-list box.

	// sizes of frame edges
	float	d_frameLeftSize;		//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;		//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;			//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;		//!< Height of the bottom frame edge in pixels.

};


/*!
\brief
	Factory class for producing WLComboDropList objects
*/
class WINDOWSLOOK_API WLComboDropListFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLComboDropListFactory(void) : WindowFactory(WLComboDropList::WidgetTypeName) { }
	~WLComboDropListFactory(void){}


	/*!
	\brief
		Create a new Window object of whatever type this WindowFactory produces.

	\param name
		A unique name that is to be assigned to the newly created Window object

	\return
		Pointer to the new Window object.
	*/
	Window*	createWindow(const String& name);


	/*!
	\brief
		Destroys the given Window object.

	\param window
		Pointer to the Window object to be destroyed.

	\return
		Nothing.
	*/
	virtual void	destroyWindow(Window* window)	 { if (window->getType() == d_type) delete window; }
};


} // End of  CEGUI namespace section


#endif	// end of guard _WLComboDropList_h_
