/************************************************************************
	filename: 	WLListHeaderSegment.h
	created:	16/8/2004
	author:		Paul D Turner
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
#ifndef _WLListHeaderSegment_h_
#define _WLListHeaderSegment_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIListHeaderSegment.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	List header segment widget for the WindowsLook Gui Scheme.
*/
class WINDOWSLOOK_API WLListHeaderSegment : public ListHeaderSegment
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// image / imageset related
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	BackdropMainImageName[];		//!< Image to use for segment backdrop main area.
	static const utf8	BackdropEdgeImageName[];		//!< Image to use for segment backdrop bottom edge.
	static const utf8	SplitterImageName[];			//!< Image to use for splitter / sizing bar in normal state.
	static const utf8	SortUpImageName[];				//!< Image to use for 'sort ascending' indicator.
	static const utf8	SortDownImageName[];			//!< Image to use for 'sort descending' indicator.
	static const utf8	NormalMouseCursor[];			//!< Image to use for mouse when not sizing.
	static const utf8	SizingMouseCursor[];			//!< Image to use for mouse when sizing.
	static const utf8	MovingMouseCursor[];			//!< Image to use for mouse when moving.

	// colours
	static const colour	BackdropNormalColour;			//!< Colour to apply to backdrop image when rendering normally.
	static const colour	BackdropHighlightColour;		//!< Colour to apply to backdrop image when rendering highlighted.
	static const colour	SplitterNormalColour;			//!< Colour to apply to splitter image when rendering normally.
	static const colour	SplitterHighlightColour;		//!< Colour to apply to splitter image when rendering highlighted.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLListHeaderSegment(const String& type, const String& name);
	virtual ~WLListHeaderSegment(void);


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
	const Image*	d_backdropMainImage;	//!< Pointer to image for backdrop main area.
	const Image*	d_backdropEdgeImage;	//!< Pointer to image for backdrop bottom edge.
	const Image*	d_splitterImage;		//!< Pointer to image for splitter.
	const Image*	d_sortAscendImage;		//!< Pointer to image for 'sort ascending' icon.
	const Image*	d_sortDescendImage;		//!< Pointer to image for 'sort descending' icon.
};


/*!
\brief
	Factory class for producing WLListHeaderSegment objects
*/
class WINDOWSLOOK_API WLListHeaderSegmentFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLListHeaderSegmentFactory(void) : WindowFactory((utf8*)"WindowsLook/ListHeaderSegment") { }
	~WLListHeaderSegmentFactory(void){}


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


#endif	// end of guard _WLListHeaderSegment_h_
