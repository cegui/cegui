/************************************************************************
	filename: 	TLProgressBar.h
	created:	23/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to the Taharez Progress Bar
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
#ifndef _TLProgressBar_h_
#define _TLProgressBar_h_

#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIProgressBar.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Standard progress bar for the Taharez Gui Scheme.
*/
class TAHAREZLOOK_API TLProgressBar : public ProgressBar
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Progress bar image names
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	ContainerLeftImageName[];		//!< Name of the image to use for the left end of the progress container.
	static const utf8	ContainerMiddleImageName[];		//!< Name of the image to use for the middle of the progress container.
	static const utf8	ContainerRightImageName[];		//!< Name of the image to use for the right end of the progress container.
	static const utf8	DimSegmentImageName[];			//!< Name of the image to use for dim / unlit segments.
	static const utf8	BrightSegmentImageName[];		//!< Name of the image to use for bright / lit segments.

	// some offsets into imagery
	static const float	FirstSegmentOffsetRatioX;		//!< Value used to calculate required offset for first segment
	static const float	SegmentOverlapRatio;			//!< Value used to calculate amount of overlap required for segments.


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez progress bar objects
	*/
	TLProgressBar(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez progress bar objects
	*/
	virtual ~TLProgressBar(void);

protected:
	/*************************************************************************
		Overridden Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform rendering for this widget
	*/
	virtual void	drawSelf(float z);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Image*	d_left;			//!< container left end image.
	const Image*	d_middle;		//!< container middle image.
	const Image*	d_right;		//!< container right end image.
	const Image*	d_dimSegment;	//!< dim segment image.
	const Image*	d_litSegment;	//!< lit segment image.
};


/*!
\brief
	Factory class for producing TLProgressBar objects
*/
class TAHAREZLOOK_API TLProgressBarFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLProgressBarFactory(void) : WindowFactory(TLProgressBar::WidgetTypeName) { }
	~TLProgressBarFactory(void){}


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


#endif	// end of guard _TLProgressBar_h_
