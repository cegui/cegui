/************************************************************************
	filename: 	TLButton.h
	created:	11/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez look button widget
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
#ifndef _TLButton_h_
#define _TLButton_h_

#include "TLModule.h"
#include "elements/CEGUIPushButton.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableImage.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Button class for the TaharezLook GUI scheme
*/
class TAHAREZLOOK_API TLButton : public PushButton
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
	static const utf8	LeftHighlightImageName[];		//!< Name of the image to use for the left end of the button (hover / highlighted).
	static const utf8	MiddleHighlightImageName[];		//!< Name of the image to use for the middle of the button (hover / highlighted).
	static const utf8	RightHighlightImageName[];		//!< Name of the image to use for the right end of the button (hover / highlighted).
	static const utf8	LeftPushedImageName[];			//!< Name of the image to use for the left end of the button (pushed state).
	static const utf8	MiddlePushedImageName[];		//!< Name of the image to use for the middle of the button (pushed state).
	static const utf8	RightPushedImageName[];			//!< Name of the image to use for the right end of the button (pushed state).
	static const utf8	MouseCursorImageName[];			//!< Name of the image to use for the mouse cursor.

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look Button objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLButton(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLButton objects.
	*/
	virtual ~TLButton(void);
	
protected:
	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		render the button in the normal state.
	*/
	virtual void	drawNormal(float z);

	/*!
	\brief
		render the button in the hover / highlighted state.
	*/
	virtual void	drawHover(float z);

	/*!
	\brief
		render the button in the pushed state.
	*/
	virtual void	drawPushed(float z);

	/*!
	\brief
		render the button in the disabled state
	*/
	virtual void	drawDisabled(float z);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// standard button rendering images
	const Image*	d_leftSectionNormal;			//!< Image to use when rendering the button left section (normal state).
	const Image*	d_middleSectionNormal;			//!< Image to use when rendering the button middle section (normal state).
	const Image*	d_rightSectionNormal;			//!< Image to use when rendering the button right section (normal state).
	const Image*	d_leftSectionHover;				//!< Image to use when rendering the button left section (hover state).
	const Image*	d_middleSectionHover;			//!< Image to use when rendering the button middle section (hover state).
	const Image*	d_rightSectionHover;			//!< Image to use when rendering the button right section (hover state).
	const Image*	d_leftSectionPushed;			//!< Image to use when rendering the button left section (pushed state).
	const Image*	d_middleSectionPushed;			//!< Image to use when rendering the button middle section (pushed state).
	const Image*	d_rightSectionPushed;			//!< Image to use when rendering the button right section (pushed state).
};


/*!
\brief
	Factory class for producing TLButton objects
*/
class TAHAREZLOOK_API TLButtonFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLButtonFactory(void) : WindowFactory(TLButton::WidgetTypeName) { }
	~TLButtonFactory(void){}


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


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _TLButton_h_
