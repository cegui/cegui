/************************************************************************
	filename: 	TLCheckbox.h
	created:	21/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez Checkbox widget
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
#ifndef _TLCheckbox_h_
#define _TLCheckbox_h_

#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUICheckbox.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Checkbox class for the TaharezLook GUI scheme
*/
class TAHAREZLOOK_API TLCheckbox : public Checkbox
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	static const utf8	ImagesetName[];				//!< Name of the imageset to use for rendering.
	static const utf8	NormalImageName[];			//!< Name of the image to use for the normal state.
	static const utf8	HighlightImageName[];		//!< Name of the image to use for the highlighted state.
	static const utf8	CheckMarkImageName[];		//!< Name of the image to use for the check / selected mark.

	static const float	LabelPadding;				//!< Pixel padding value for text label (space between image and text label).


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez Look Checkbox objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLCheckbox(const String& type, const String& name);


	/*!
	\brief
		Destructor for TLCheckbox objects.
	*/
	virtual ~TLCheckbox(void);

	
protected:
	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		render the Checkbox in the normal state.
	*/
	virtual void	drawNormal(float z);

	/*!
	\brief
		render the Checkbox in the hover / highlighted state.
	*/
	virtual void	drawHover(float z);

	/*!
	\brief
		render the Checkbox in the pushed state.
	*/
	virtual void	drawPushed(float z);

	/*!
	\brief
		render the Checkbox in the disabled state
	*/
	virtual void	drawDisabled(float z);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// rendering images
	const Image*	d_normalImage;			//!< Image to use when rendering in normal state.
	const Image*	d_hoverImage;			//!< Image to use when rendering in hover  / highlighted state.
	const Image*	d_checkMarkImage;		//!< Image to use when rendering the check-mark.
};


/*!
\brief
	Factory class for producing TLCheckbox objects
*/
class TAHAREZLOOK_API TLCheckboxFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLCheckboxFactory(void) : WindowFactory(TLCheckbox::WidgetTypeName) { }
	~TLCheckboxFactory(void){}


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


#endif	// end of guard _TLCheckbox_h_
