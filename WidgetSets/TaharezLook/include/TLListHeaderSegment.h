/************************************************************************
	filename: 	TLListHeaderSegment.h
	created:	15/6/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez list header segment widget.
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
    Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _TLListHeaderSegment_h_
#define _TLListHeaderSegment_h_

#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIListHeaderSegment.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	List header segment widget for the Taharez Gui Scheme.
*/
class TAHAREZLOOK_API TLListHeaderSegment : public ListHeaderSegment
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// image / imageset related
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	BackdropNormalImageName[];		//!< Image to use for segment backdrop in normal state.
	static const utf8	BackdropHoverImageName[];		//!< Image to use for segment backdrop when mouse is hovering.
	static const utf8	SplitterNormalImageName[];		//!< Image to use for splitter / sizing bar in normal state.
	static const utf8	SplitterHoverImageName[];		//!< Image to use for splitter / sizing bar in hovering state.
	static const utf8	SortUpImageName[];				//!< Image to use for 'sort ascending' indicator.
	static const utf8	SortDownImageName[];			//!< Image to use for 'sort descending' indicator.
	static const utf8	NormalMouseCursor[];			//!< Image to use for mouse when not sizing.
	static const utf8	SizingMouseCursor[];			//!< Image to use for mouse when sizing.
	static const utf8	MovingMouseCursor[];			//!< Image to use for mouse when moving.



	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLListHeaderSegment(const String& type, const String& name);
	virtual ~TLListHeaderSegment(void);


protected:
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
		Render segment
	*/
	void	renderSegmentImagery(Vector3 pos, float alpha, const Rect& clipper);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Image*	d_backNormalImage;		//!< Pointer to image for normal backdrop.
	const Image*	d_backHoverImage;		//!< Pointer to image for hover backdrop.
	const Image*	d_splitterNormalImage;	//!< Pointer to image for normal splitter.
	const Image*	d_splitterHoverImage;	//!< Pointer to image for hover splitter.
	const Image*	d_sortAscendImage;		//!< Pointer to image for 'sort ascending' icon.
	const Image*	d_sortDescendImage;		//!< Pointer to image for 'sort descending' icon.
};


/*!
\brief
	Factory class for producing TLListHeaderSegment objects
*/
class TAHAREZLOOK_API TLListHeaderSegmentFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLListHeaderSegmentFactory(void) : WindowFactory((utf8*)"Taharez ListHeaderSegment") { }
	~TLListHeaderSegmentFactory(void){}


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


#endif	// end of guard _TLListHeaderSegment_h_
