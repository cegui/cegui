/***********************************************************************
	filename: 	CEGUIMouseCursor.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for the MouseCursor class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _CEGUIMouseCursor_h_
#define _CEGUIMouseCursor_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIVector.h"
#include "CEGUIRect.h"
#include "CEGUIEventSet.h"
#include "CEGUIInputEvent.h"
#include "CEGUIUDim.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Enumeration of special values used for mouse cursor settings in Window objects.
*/
enum MouseCursorImage
{		
	BlankMouseCursor	= 0,		//!< No image should be displayed for the mouse cursor.
	DefaultMouseCursor	= -1		//!< The default mouse cursor image should be displayed.
};


/*!
\brief
	Class that allows access to the GUI system mouse cursor.

	The MouseCursor provides functionality to access the position and imagery of the mouse cursor / pointer
*/
class CEGUIEXPORT MouseCursor : public EventSet, public Singleton<MouseCursor>
{
public:
	static const String EventNamespace;				//!< Namespace for global events

	/*************************************************************************
		Event name constants
	*************************************************************************/
	// generated internally by MouseCursor
	static const String EventImageChanged;			//!< The cursor image of the widget has changed.


	/*!
	\brief
		Constructor for MouseCursor objects
	*/
	MouseCursor(void);


	/*!
	\brief
		Destructor for MouseCursor objects
	*/
	~MouseCursor(void);


	/*!
	\brief
		Return singleton MouseCursor object

	\return
		Singleton MouseCursor object
	*/
	static	MouseCursor&	getSingleton(void);


	/*!
	\brief
		Return pointer to singleton MouseCursor object

	\return
		Pointer to singleton MouseCursor object
	*/
	static	MouseCursor*	getSingletonPtr(void);


	/*!
	\brief
		Set the current mouse cursor image

	\param imageset
		String object holding the name of the Imageset that contains the desired Image.

	\param image_name
		String object holding the name of the desired Image on Imageset \a imageset.

	\return
		Nothing.

	\exception UnknownObjectException	thrown if \a imageset is not known, or if \a imageset contains no Image named \a image_name.
	*/
	void	setImage(const String& imageset, const String& image_name);


	/*!
	\brief
		Set the current mouse cursor image
	*/
	void	setImage(const Image* image);


	/*!
	\brief
		Get the current mouse cursor image
	\return
		The current image used to draw mouse cursor.
	*/
	const Image*	getImage(void) const	{return d_cursorImage;}


	/*!
	\brief
		Makes the cursor draw itself

	\return
		Nothing
	*/
	void	draw(void) const;


	/*!
	\brief
		Set the current mouse cursor position

	\param position
		Point object describing the new location for the mouse.  This will be clipped to within the renderer screen area.
	*/
	void	setPosition(const Point& position);


	/*!
	\brief
		Offset the mouse cursor position by the deltas specified in \a offset.

	\param offset
		Point object which describes the amount to move the cursor in each axis.

	\return
		Nothing.
	*/
	void	offsetPosition(const Point& offset);


	/*!
	\brief
		Set the area that the mouse cursor is constrained to.

	\param area
		Pointer to a Rect object that describes the area of the display that the mouse is allowed to occupy.  The given area will be clipped to
		the current Renderer screen area - it is never possible for the mouse to leave this area.  If this parameter is NULL, the
		constraint is set to the size of the current Renderer screen area.

	\return
		Nothing.
	*/
	void	setConstraintArea(const Rect* area);


	/*!
	\brief
		Set the area that the mouse cursor is constrained to.

	\param area
		Pointer to a URect object that describes the area of the display that the mouse is allowed to occupy.  The given area will be clipped to
		the current Renderer screen area - it is never possible for the mouse to leave this area.  If this parameter is NULL, the
		constraint is set to the size of the current Renderer screen area.

	\return
		Nothing.
	*/
	void	setUnifiedConstraintArea(const URect* area);


	/*!
	\brief
		Hides the mouse cursor.

	\return
		Nothing.
	*/
	void	hide(void)		{d_visible = false;}


	/*!
	\brief
		Shows the mouse cursor.

	\return
		Nothing.
	*/
	void	show(void)		{d_visible = true;}


    /*!
    \brief
        Set the visibility of the mouse cursor.

    \param visible
        'true' to show the mouse cursor, 'false' to hide it.

    \return
        Nothing.
    */
    void    setVisible(bool visible)    {d_visible = visible;}


	/*!
	\brief
		return whether the mouse cursor is visible.

	\return
		true if the mouse cursor is visible, false if the mouse cursor is hidden.
	*/
	bool	isVisible(void) const	{return d_visible;}


	/*!
	\brief
		Return the current mouse cursor position as a pixel offset from the top-left corner of the display.

	\return
		Point object describing the mouse cursor position in screen pixels.
	*/
	Point	getPosition(void) const		{return Point(d_position.d_x, d_position.d_y);}


	/*!
	\brief
		return the current constraint area of the mouse cursor.

	\return
		Rect object describing the active area that the mouse cursor is constrained to.
	*/
	Rect	getConstraintArea(void) const;


	/*!
	\brief
		return the current constraint area of the mouse cursor.

	\return
		URect object describing the active area that the mouse cursor is constrained to.
	*/
	const URect& getUnifiedConstraintArea(void) const;


	/*!
	\brief
		Return the current mouse cursor position as display resolution independant values.

	\return
		Point object describing the current mouse cursor position as resolution independant values that
		range from 0.0f to 1.0f, where 0.0f represents the left-most and top-most positions, and 1.0f
		represents the right-most and bottom-most positions.
	*/
	Point	getDisplayIndependantPosition(void) const;


protected:
	/*************************************************************************
		New event handlers
	*************************************************************************/
	/*!
	\brief
		event triggered internally when image of mouse cursor changes
	*/
	virtual void	onImageChanged(MouseCursorEventArgs& e);


private:
	/*************************************************************************
		Implementation Methods
	*************************************************************************/
	/*!
	\brief
		Checks the mouse cursor position is within the current 'constrain' Rect and adjusts as required.
	*/
	void	constrainPosition(void);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Image*	d_cursorImage;		//!< Image that is currently set as the mouse cursor.
	Vector3	d_position;					//!< Current location of the cursor
	bool	d_visible;					//!< true if the cursor will be drawn, else false.
	URect	d_constraints;				//!< Specifies the area (in screen pixels) that the mouse can move around in.
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIMouseCursor_h_
