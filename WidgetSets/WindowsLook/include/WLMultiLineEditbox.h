/************************************************************************
	filename: 	WLMultiLineEditbox.h
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
#ifndef _WLMultiLineEditbox_h_
#define _WLMultiLineEditbox_h_

#include "WLModule.h"
#include "elements/CEGUIMultiLineEditbox.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIRenderableFrame.h"
#include "CEGUIRenderableImage.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Multi-line edit box class for the WindowsLook Gui scheme.
*/
class WINDOWSLOOK_API WLMultiLineEditbox : public MultiLineEditbox
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
	static const utf8	CaratImageName[];			//!< Name of the image to use for the carat.
	static const utf8	MouseCursorImageName[];		//!< Name of the image to use for the mouse cursor.

	// component widget type names
	static const utf8*	HorzScrollbarTypeName;		//!< Type name of widget to be created as horizontal scroll bar.
	static const utf8*	VertScrollbarTypeName;		//!< Type name of widget to be created as vertical scroll bar.

	// colours
	static const colour	ReadWriteBackgroundColour;			//!< Colour used for background in read/write mode.
	static const colour	ReadOnlyBackgroundColour;			//!< Colour used for background in read only mode.
	static const colour	NormalTextColour;					//!< Colour applied to normal unselected text.
	static const colour	SelectedTextColour;					//!< Colour applied to selected text.
	static const colour	NormalSelectionColour;				//!< Colour applied to normal selection brush.
	static const colour	InactiveSelectionColour;			//!< Colour applied to selection brush when widget is inactive.


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for the WLMultiLineEditbox class.
	*/
	WLMultiLineEditbox(const String& type, const String& name);


	/*!
	\brief
		Destructor for the WLMultiLineEditbox class.
	*/
	virtual ~WLMultiLineEditbox(void);


protected:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Return a Rect object describing, in un-clipped pixels, the window relative area
		that the text should be rendered in to.

	\return
		Rect object describing the area of the Window to be used for rendering text.
	*/
	virtual	Rect	getTextRenderArea(void) const;


	/*!
	\brief
		create and return a pointer to a Scrollbar widget for use as vertical scroll bar

	\return
		Pointer to a Scrollbar to be used for scrolling vertically.
	*/
	virtual Scrollbar*	createVertScrollbar(void) const;
 

	/*!
	\brief
		create and return a pointer to a Scrollbar widget for use as horizontal scroll bar

	\return
		Pointer to a Scrollbar to be used for scrolling horizontally.
	*/
	virtual Scrollbar*	createHorzScrollbar(void) const;


	/*!
	\brief
		Setup size and position for the component widgets attached to this Editbox

	\return
		Nothing.
	*/
	virtual void	layoutComponentWidgets();


	/*!
	\brief
		Perform rendering of the widget control frame and other 'static' areas.  This
		method should not render the actual text.  Note that the text will be rendered
		to layer 4 and the selection brush to layer 3, other layers can be used for
		rendering imagery behind and infront of the text & selection..

	\param z
		Z co-ordinate for layer 0.

	\return
		Nothing.
	*/
	virtual	void	renderEditboxBaseImagery(float z);


	/*!
	\brief
		Render the carat.

	\return
		Nothing
	*/
	virtual void	renderCarat(float baseX, float baseY, const Rect& clipper);


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
		Implementation Data.
	*************************************************************************/
	RenderableFrame	d_frame;		//!< Used for the frame of the edit box.

	const Image*	d_carat;		//!< Image for the carat.
	const Image*	d_background;	//!< Used for the background area of the edit box.

	// sizes of frame edges
	float	d_frameLeftSize;		//!< Width of the left frame edge in pixels.
	float	d_frameRightSize;		//!< Width of the right frame edge in pixels.
	float	d_frameTopSize;			//!< Height of the top frame edge in pixels.
	float	d_frameBottomSize;		//!< Height of the bottom frame edge in pixels.
};

/*!
\brief
	Factory class for producing WLMultiLineEditbox objects
*/
class WINDOWSLOOK_API WLMultiLineEditboxFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLMultiLineEditboxFactory(void) : WindowFactory(WLMultiLineEditbox::WidgetTypeName) { }
	~WLMultiLineEditboxFactory(void){}


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


#endif	// end of guard _WLMultiLineEditbox_h_
