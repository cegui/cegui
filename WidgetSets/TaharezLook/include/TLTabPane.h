/************************************************************************
	filename: 	TLTabPane.h
	created:	11/08/2004
	author:		Steve Streeting
	
	purpose:	Defines interface for Taharez Look Tab Pane class.
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
#ifndef _TLTabPane_h_
#define _TLTabPane_h_

#include "TLModule.h"
#include "elements/CEGUITabPane.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Concrete TabPane class for the Taharez Look widget set.	
*/
class TAHAREZLOOK_API TLTabPane : public TabPane
{
public:
    /*************************************************************************
    Constants
    *************************************************************************/
	static const utf8	ImagesetName[];				//!< Name of the imageset to use for rendering.
	static const utf8	LeftImageName[];		    //!< Name of the image to use for the left end of the button.
	static const utf8	MiddleImageName[];		    //!< Name of the image to use for the middle of the button.
	static const utf8	RightImageName[];			//!< Name of the image to use for the right end of the button.
    static const utf8	LowerImageName[];           //!< Name of the image to use for the lower section of the button
    static const utf8	LowerLeftImageName[];       //!< Name of the image to use for the lower left section of the button
    static const utf8	LowerRightImageName[];      //!< Name of the image to use for the lower right section of the button

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look TabPane objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLTabPane(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLButton objects.
	*/
	virtual ~TLTabPane(void);

	
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
    void	drawSelf(float z);

	/*************************************************************************
		Implementation Data
	*************************************************************************/

	// standard button rendering images
	const Image*	d_leftSection;			//!< Image to use when rendering the button left section .
	const Image*	d_middleSection;		//!< Image to use when rendering the button middle section .
	const Image*	d_rightSection;			//!< Image to use when rendering the button right section .
    const Image*	d_lowerLeftSection;		//!< Image to use when rendering the button lower left section .
    const Image*	d_lowerRightSection;	//!< Image to use when rendering the button lower right section .
    const Image*	d_lowerSection;		    //!< Image to use when rendering the button lower section .
};


/*!
\brief
	Factory class for producing TLFrameWindow objects
*/
class TAHAREZLOOK_API TLTabPaneFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Tab Button factory class.
	*/
	TLTabPaneFactory(void) : WindowFactory((utf8*)"Taharez Tab Pane") { }


	/*
	\brief
		Destructor for Taharez Tab Button factory class.
	*/
	~TLTabPaneFactory(void){}


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
