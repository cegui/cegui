/************************************************************************
	filename: 	CEGUIStatic.h
	created:	13/4/2004
	author:		Paul D Turner
	
	purpose:	Interface to base class for Static widget
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
#ifndef _CEGUIStatic_h_
#define _CEGUIStatic_h_

#include "CEGUIBase.h"
#include "CEGUIWindow.h"
#include "CEGUIRenderableFrame.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class for static widgets.
*/
class CEGUIBASE_API Static : public Window
{
public:
	/*************************************************************************
		Accessors
	*************************************************************************/
	/*!
	\brief
		Return whether the frame for this static widget is enabled or disabled.

	\return
		true if the frame is enabled and will be rendered.  false is the frame is disabled and will not be rendered.
	*/
	bool	isFrameEnabled(void) const		{return d_frameEnabled;}


	/*!
	\brief
		Return whether the background for this static widget is enabled to disabled.

	\return
		true if the background is enabled and will be rendered.  false if the background is disabled and will not be rendered.
	*/
	bool	isBackgroundEnabled(void) const		{return d_backgroundEnabled;}


	/*!
	\brief
		Return a ColourRect object containing the colours used when rendering this widget.

	\return
		ColourRect object initialised with the colours used when rendering the frame for this widget.
	*/
	ColourRect	getFrameColours(void) const		{return d_frameCols;}


	/*!
	\brief
		Return a ColourRect object containing the colours used when rendering this widget.

	\return
		ColourRect object initialised with the colours used when rendering the background for this widget.
	*/
	ColourRect	getBackgroundColours(void) const		{return d_backgroundCols;}


	// overridden so derived classes are auto-clipped to within the inner area of the frame when it's active.
	virtual Rect	getUnclippedInnerRect(void) const;


	/*************************************************************************
		Manipulators
	*************************************************************************/
	/*!
	\brief
		Enable or disable rendering of the frame for this static widget.

		Frame imagery can be customised via the setFrameImages method.

	\param setting
		true to enable rendering of a frame.  false to disable rendering of a frame.
	*/
	void	setFrameEnabled(bool setting);


	/*!
	\brief
		specify the Image objects to use for each part of the frame.  A NULL may be used to omit any part.

	\param topleft
		Pointer to an Image object to render as the top-left corner of the frame.  Specify NULL to omit this part of the frame.

	\param topright
		Pointer to an Image object to render as the top-right corner of the frame.  Specify NULL to omit this part of the frame.

	\param bottomleft
		Pointer to an Image object to render as the bottom-left corner of the frame.  Specify NULL to omit this part of the frame.

	\param bottomright
		Pointer to an Image object to render as the bottom-right corner of the frame.  Specify NULL to omit this part of the frame.

	\param left
		Pointer to an Image object to render as the left edge of the frame.  Specify NULL to omit this part of the frame.

	\param top
		Pointer to an Image object to render as the top edge of the frame.  Specify NULL to omit this part of the frame.

	\param right
		Pointer to an Image object to render as the right edge of the frame.  Specify NULL to omit this part of the frame.

	\param bottom
		Pointer to an Image object to render as the bottom edge of the frame.  Specify NULL to omit this part of the frame.

	\return
		Nothing
	*/
	void	setFrameImages(const Image* topleft, const Image* topright, const Image* bottomleft, const Image* bottomright, const Image* left, const Image* top, const Image* right, const Image* bottom);


	/*!
	\brief
		Sets the colours to be applied when rendering the frame.

	\param colours
		ColourRect object describing the colours to be used.

	\return 
		Nothing.
	*/
	void	setFrameColours(const ColourRect& colours);


	/*!
	\brief
		Sets the colours to be applied when rendering the frame.

	\param top_left_colour
		Colour (as ARGB value) to be applied to the top-left corner of each Image used in the frame.

	\param top_right_colour
		Colour (as ARGB value) to be applied to the top-right corner of each Image used in the frame.

	\param bottom_left_colour
		Colour (as ARGB value) to be applied to the bottom-left corner of each Image used in the frame.

	\param bottom_right_colour
		Colour (as ARGB value) to be applied to the bottom-right corner of each Image used in the frame.

	\return 
		Nothing.
	*/
	void	setFrameColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour);


	/*!
	\brief
		Sets the colour to be applied when rendering the frame.

	\param col
		colour value to be used when rendering.

	\return
		Nothing.
	*/
	void	setFrameColours(colour col)		{setFrameColours(col, col, col, col);}


	/*!
	\brief
		Enable or disable rendering of the background for this static widget.

	\param setting
		true to enable rendering of the background.  false to disable rendering of the background.
	*/
	void	setBackgroundEnabled(bool setting);


	/*!
	\brief
		Set the image to use as the background for the static widget.

	\param image
		Pointer to the Image object to be rendered.  Can be NULL to specify no image is to be rendered.
	
	\return
		Nothing.
	*/
	void	setBackgroundImage(const Image* image);


	/*!
	\brief
		Set the image to use as the background for the static widget.

	\param imageset
		String object specifying the name of the Imageset that holds the required image.

	\param image
		Name of the Image on the specified Imageset that is to be used.
	
	\return
		Nothing.

	\exception UnknownObjectException	thrown if Imageset \a imageset does not exist in the system or if \a imageset contains no Image named \a image.
	*/
	void	setBackgroundImage(const String& imageset, const String& image);


	/*!
	\brief
		Sets the colours to be applied when rendering the background.

	\param colours
		ColourRect object describing the colours to be used.

	\return 
		Nothing.
	*/
	void	setBackgroundColours(const ColourRect& colours);


	/*!
	\brief
		Sets the colours to be applied when rendering the background.

	\param top_left_colour
		Colour (as ARGB value) to be applied to the top-left corner of the background.

	\param top_right_colour
		Colour (as ARGB value) to be applied to the top-right corner of the background.

	\param bottom_left_colour
		Colour (as ARGB value) to be applied to the bottom-left corner of the background.

	\param bottom_right_colour
		Colour (as ARGB value) to be applied to the bottom-right corner of the background.

	\return 
		Nothing.
	*/
	void	setBackgroundColours(colour top_left_colour, colour top_right_colour, colour bottom_left_colour, colour bottom_right_colour);


	/*!
	\brief
		Sets the colour to be applied when rendering the background.

	\param col
		colour value to be used when rendering.

	\return
		Nothing.
	*/
	void	setBackgroundColours(colour col)		{setBackgroundColours(col, col, col, col);}


protected:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for static widget base class
	*/
	Static(const String& type, const String& name);


	/*!
	\brief
		Destructor for static widget base class.
	*/
	virtual ~Static(void);


	/*************************************************************************
		Overridden from base class
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
		Event handling
	*************************************************************************/
	virtual void	onSized(WindowEventArgs& e);
	virtual void	onAlphaChanged(WindowEventArgs& e);


	/*************************************************************************
		Implementation methods
	*************************************************************************/
	/*!
	\brief
		update the internal RenderableFrame with currently set colours and
		alpha settings.
	*/
	void	updateRenderableFrameColours(void);


	/*!
	\brief
		return ARGB colour value \a col, with its alpha component modulated by the value specified in float \a alpha.
	*/
	colour	calculateModulatedAlphaColour(colour col, float alpha) const;


	/*!
	\brief
		This is used internally to indicate that the frame for the static widget has been modified, and as such
		derived classes may need to adjust their layouts or reconfigure their rendering somehow.
	\note
		This does not currently fire an external event.
	*/
	virtual void	onStaticFrameChanged(WindowEventArgs& e)	{}


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool				d_frameEnabled;		//!< True when the frame is enabled.
	ColourRect			d_frameCols;		//!< Used to store frame colours.
	RenderableFrame		d_frame;			//!< Frame object used for rendering a frame for the static element.
	bool				d_backgroundEnabled;//!< true when the background is enabled.
	ColourRect			d_backgroundCols;	//!< Colours to use when drawing background.
	const Image*		d_background;		//!< Image to use for widget background.

	// cache of frame edge sizes
	float	d_left_width;			//!< Width of the left edge image for the current frame.
	float	d_right_width;			//!< Width of the right edge image for the current frame.
	float	d_top_height;			//!< Height of the top edge image for the current frame.
	float	d_bottom_height;		//!< Height of the bottom edge image for the current frame.
};


} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIStatic_h_
