/************************************************************************
	filename: 	TLMiniHorzScrollbarThumb.h
	created:	2/6/2004
	author:		Paul D Turner
	
	purpose:	Interface to thumb for Taharez mini horizontal scroll bar.
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
#ifndef _TLMiniHorzScrollbarThumb_h_
#define _TLMiniHorzScrollbarThumb_h_

#include "TLModule.h"
#include "elements/CEGUIThumb.h"
#include "CEGUIWindowFactory.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Thumb class as used in the Taharez mini horizontal scroll bar.
*/
class TAHAREZLOOK_API TLMiniHorzScrollbarThumb : public Thumb
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	// Image names
	static const utf8	ImagesetName[];					//!< Name of the imageset to use for rendering.
	static const utf8	NormalImageName[];				//!< Name of the image to use for normal rendering (used for size only now).
	static const utf8	NormalLeftImageName[];			//!< Name of the image to use for normal rendering (left end).
	static const utf8	NormalMiddleImageName[];		//!< Name of the image to use for normal rendering (mid section).
	static const utf8	NormalRightImageName[];			//!< Name of the image to use for normal rendering (right end).
	static const utf8	HighlightLeftImageName[];		//!< Name of the image to use for hover / highlighted rendering (left end).
	static const utf8	HighlightMiddleImageName[];		//!< Name of the image to use for hover / highlighted rendering (mid section).
	static const utf8	HighlightRightImageName[];		//!< Name of the image to use for hover / highlighted rendering (right end).


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	TLMiniHorzScrollbarThumb(const String& type, const String& name);
	virtual ~TLMiniHorzScrollbarThumb(void);


protected:
	/*************************************************************************
		Implementation Rendering Functions
	*************************************************************************/
	/*!
	\brief
		render the thumb in the normal state.
	*/
	virtual void	drawNormal(float z);

	/*!
	\brief
		render the thumb in the hover / highlighted state.
	*/
	virtual void	drawHover(float z);

	/*!
	\brief
		render the thumb in the pushed state.
	*/
	virtual void	drawPushed(float z)		{drawHover(z);}

	/*!
	\brief
		render the thumb in the disabled state
	*/
	virtual void	drawDisabled(float z);


	/*************************************************************************
		Overridden events
	*************************************************************************/
	virtual void	onSized(WindowEventArgs& e);


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	const Image*	d_normalImage;			//!< Image to render in normal state (used for measuring only)
	const Image*	d_normalLeftImage;		//!< Image to render in normal state (left end)
	const Image*	d_normalMiddleImage;	//!< Image to render in normal state (middle section)
	const Image*	d_normalRightImage;		//!< Image to render in normal state (right end)
	const Image*	d_highlightLeftImage;	//!< Image to render in highlighted state (left end).
	const Image*	d_highlightMiddleImage;	//!< Image to render in highlighted state (middle section).
	const Image*	d_highlightRightImage;	//!< Image to render in highlighted state (right end).
};


/*!
\brief
	Factory class for producing TLMiniHorzScrollbarThumb objects
*/
class TAHAREZLOOK_API TLMiniHorzScrollbarThumbFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLMiniHorzScrollbarThumbFactory(void) : WindowFactory(TLMiniHorzScrollbarThumb::WidgetTypeName) { }
	~TLMiniHorzScrollbarThumbFactory(void){}


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

#endif	// end of guard _TLMiniHorzScrollbarThumb_h_
