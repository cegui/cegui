/************************************************************************
	filename: 	WLListHeader.h
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
#ifndef _WLListHeader_h_
#define _WLListHeader_h_

#include "WLModule.h"
#include "elements/CEGUIListHeader.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	List header class for the WindowsLook Gui Scheme.
*/
class WINDOWSLOOK_API WLListHeader : public ListHeader
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// image / imageset related
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	MouseCursorImageName[];			//!< Image to use for mouse.

	// type names for the component widgets
	static const utf8*	SegmentWidgetType;				//!< Type of widget to create for the header segments;


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook List Header widgets.
	*/
	WLListHeader(const String& type, const String& name);


	/*!
	\brief
		Destructor for WindowsLook List Header widgets.
	*/
	virtual ~ WLListHeader(void);


protected:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Render the widget.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual void	drawSelf(float z);


	/*!
	\brief
		Create and return a pointer to a new ListHeaderSegment based object.

	\param name
		String object holding the name that should be given to the new Window.

	\return
		Pointer to an ListHeaderSegment based object of whatever type is appropriate for
		this ListHeader.
	*/
	virtual ListHeaderSegment*	createNewSegment(const String& name) const;


	/*!
	\brief
		Cleanup and destroy the given ListHeaderSegment that was created via the
		createNewSegment method.

	\param segment
		Pointer to a ListHeaderSegment based object to be destroyed.

	\return
		Nothing.
	*/
	virtual void	destroyListSegment(ListHeaderSegment* segment) const;
};


/*!
\brief
	Factory class for producing WLListHeader objects
*/
class WINDOWSLOOK_API WLListHeaderFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLListHeaderFactory(void) : WindowFactory(WLListHeader::WidgetTypeName) { }
	~WLListHeaderFactory(void){}


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


#endif	// end of guard _WLListHeader_h_
