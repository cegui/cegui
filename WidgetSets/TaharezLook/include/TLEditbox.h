/************************************************************************
	filename: 	TLEditbox.h
	created:	29/5/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez Look Editbox widget class
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
#ifndef _TLEditbox_h_
#define _TLEditbox_h_

#include "TLModule.h"
#include "elements/CEGUIEditbox.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Single line edit box class for the Taharez look Gui Scheme
*/
class TAHAREZLOOK_API TLEditbox : public Editbox
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// image name constants
	static const utf8	ImagesetName[];						//!< Name of the Imageset containing the imagery to use.
	static const utf8	ContainerLeftImageName[];			//!< Name of the image to use for the left end of the container.
	static const utf8	ContainerMiddleImageName[];			//!< Name of the image to use for the middle of the container.
	static const utf8	ContainerRightImageName[];			//!< Name of the image to use for the right end of the container.
	static const utf8	CaratImageName[];					//!< Name of the image to use for the carat.
	static const utf8	SelectionBrushImageName[];			//!< Name of the image to use for the selection brush.
	static const utf8	MouseCursorImageName[];				//!< Name of the image used for the mouse cursor.

	// layout values
	static const float	TextPaddingRatio;				//!< Used to generate padding distance for text.


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Taharez edit box widgets
	*/
	TLEditbox(const String& type, const String& name);


	/*!
	\brief
		Destructor for Taharez edit box widgets
	*/
	virtual ~TLEditbox(void);


protected:
	/*************************************************************************
		Rendering layers
	*************************************************************************/
	static const uint	SelectionLayer;		//!< Layer to use for selection rendering.
	static const uint	TextLayer;			//!< Layer to use for text.
	static const uint	CaratLayer;			//!< Layer to use for carat.


	/*************************************************************************
		Implementation functions
	*************************************************************************/
	/*!
	\brief
		Return the text code point index that is rendered closest to screen position \a pt.

	\param pt
		Point object describing a position on the screen in pixels.

	\return
		Code point index into the text that is rendered closest to screen position \pt.
	*/
	virtual	size_t	getTextIndexFromPosition(const Point& pt) const;


	/*!
	\brief
		return text padding value to use in pixels
	*/
	float	getTextPaddingPixels(void) const;


	/*************************************************************************
		Overridden Rendering Functions
	*************************************************************************/
	/*!
	\brief
		Perform the actual rendering for this Window.

	\param z
		float value specifying the base Z co-ordinate that should be used when rendering

	\return
		Nothing
	*/
	virtual	void	drawSelf(float z);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	// images
	const Image*	d_left;			//!< Image to use for left end of the edit box.
	const Image*	d_middle;		//!< Image to use for the  middle section of the edit box.
	const Image*	d_right;		//!< Image to use for the right end of the edit box.
	const Image*	d_carat;		//!< Image to use for the carat.
	const Image*	d_selection;	//!< Image to use for selection highlight brush.

	// rendering internal vars
	float	d_lastTextOffset;		//!< x rendering offset used last time we drew the widget.
};


/*!
\brief
	Factory class for producing TLEditbox objects
*/
class TAHAREZLOOK_API TLEditboxFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLEditboxFactory(void) : WindowFactory(TLEditbox::WidgetTypeName) { }
	~TLEditboxFactory(void){}


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


#endif	// end of guard _TLEditbox_h_
