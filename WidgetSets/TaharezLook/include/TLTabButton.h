/************************************************************************
	filename: 	TLTabButton.h
	created:	11/08/2004
	author:		Steve Streeting
	
	purpose:	Defines interface for Taharez Look Tab Button class.
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
#ifndef _TLTabButton_h_
#define _TLTabButton_h_

#include "TLModule.h"
#include "elements/CEGUITabButton.h"
#include "TLButton.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Concrete TabButton class for the Taharez Look widget set.	
*/
class TAHAREZLOOK_API TLTabButton : public TabButton
{
public:
    /*************************************************************************
    Constants
    *************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	LeftNormalImageName[];			//!< Name of the image to use for the left end of the button (normal).
	static const utf8	MiddleNormalImageName[];		//!< Name of the image to use for the middle of the button (normal).
	static const utf8	RightNormalImageName[];			//!< Name of the image to use for the right end of the button (normal).
    static const utf8	UpperRightNormalImageName[];	//!< Name of the image to use for the upper right end of the button (normal).
    static const utf8	UpperNormalImageName[];	        //!< Name of the image to use for the upper end of the button (normal).
    static const utf8	UpperLeftNormalImageName[];	    //!< Name of the image to use for the upper left end of the button (normal).
    static const utf8	LowerNormalImageName[];	        //!< Name of the image to use for the lower section of the button (normal)
    static const utf8	LowerLeftNormalImageName[];	    //!< Name of the image to use for the lower left section of the button (normal)
    static const utf8	LowerRightNormalImageName[];	//!< Name of the image to use for the lower left section of the button (normal)
	static const utf8	LeftSelectedImageName[];		//!< Name of the image to use for the left end of the button (selected).
	static const utf8	MiddleSelectedImageName[];		//!< Name of the image to use for the middle of the button (selected).
	static const utf8	RightSelectedImageName[];		//!< Name of the image to use for the right end of the button (selected).
    static const utf8	UpperRightSelectedImageName[];	//!< Name of the image to use for the upper right end of the button (selected).
    static const utf8	UpperLeftSelectedImageName[];	//!< Name of the image to use for the upper left end of the button (selected).
    static const utf8	UpperSelectedImageName[];	        //!< Name of the image to use for the upper end of the button (Selected).
    static const utf8	LowerSelectedImageName[];	    //!< Name of the image to use for the lower section of the button (selected)

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look TabButton objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLTabButton(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLButton objects.
	*/
	virtual ~TLTabButton(void);

	
	/*************************************************************************
		Accessors
	*************************************************************************/

	/*************************************************************************
		Manipulators
	*************************************************************************/


protected:
	/*************************************************************************
		Overridden Event Handling Functions
	*************************************************************************/


	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
    void	drawNormal(float z);
    void	drawPushed(float z);

	/*************************************************************************
		Implementation Data
	*************************************************************************/

	// standard button rendering images
	const Image*	d_leftSectionNormal;			//!< Image to use when rendering the button left section (normal state).
	const Image*	d_middleSectionNormal;			//!< Image to use when rendering the button middle section (normal state).
	const Image*	d_rightSectionNormal;			//!< Image to use when rendering the button right section (normal state).
    const Image*	d_upperRightSectionNormal;		//!< Image to use when rendering the button upper right section (normal state).
    const Image*	d_upperLeftSectionNormal;		//!< Image to use when rendering the button upper left section (normal state).
    const Image*	d_lowerRightSectionNormal;		//!< Image to use when rendering the button lower right section (normal state).
    const Image*	d_lowerLeftSectionNormal;		//!< Image to use when rendering the button lower left section (normal state).
    const Image*	d_upperSectionNormal;		    //!< Image to use when rendering the button upper section (normal state).
    const Image*	d_lowerSectionNormal;		    //!< Image to use when rendering the button lower section (normal state).
	const Image*	d_leftSectionSelected;			//!< Image to use when rendering the button left section (selected state).
	const Image*	d_middleSectionSelected;		//!< Image to use when rendering the button middle section (selected state).
	const Image*	d_rightSectionSelected;			//!< Image to use when rendering the button right section (selected state).
    const Image*	d_upperRightSectionSelected;	//!< Image to use when rendering the button upper right section (selected state).
    const Image*	d_upperLeftSectionSelected;		//!< Image to use when rendering the button upper left section (selected state).
    const Image*	d_upperSectionSelected;		    //!< Image to use when rendering the button upper section (Selected state).
    const Image*	d_lowerSectionSelected;		    //!< Image to use when rendering the button lower section (selected state).
};


/*!
\brief
	Factory class for producing TLFrameWindow objects
*/
class TAHAREZLOOK_API TLTabButtonFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Tab Button factory class.
	*/
	TLTabButtonFactory(void) : WindowFactory(TLTabButton::WidgetTypeName) { }


	/*
	\brief
		Destructor for Taharez Tab Button factory class.
	*/
	~TLTabButtonFactory(void){}


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

#endif	// end of guard _TLFrameWindow_h_
