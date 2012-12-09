/***********************************************************************
	filename: 	CEGUITitlebar.h
	created:	25/4/2004
	author:		Paul D Turner
	
	purpose:	Interface for a Titlebar Widget
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
#ifndef _CEGUITitlebar_h_
#define _CEGUITitlebar_h_

#include "../CEGUIWindow.h"
#include "CEGUITitlebarProperties.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Class representing the title bar for Frame Windows.

*/
class CEGUIEXPORT Titlebar : public Window
{
public:
	static const String EventNamespace;				//!< Namespace for global events
    static const String WidgetTypeName;             //!< Window factory name

	/*!
	\brief
		Return whether this title bar will respond to dragging.

	\return
		true if the title bar will respond to dragging, false if the title bar will not respond.
	*/
	bool	isDraggingEnabled(void) const;


	/*!
	\brief
		Set whether this title bar widget will respond to dragging.

	\param setting
		true if the title bar should respond to being dragged, false if it should not respond.

	\return
		Nothing.
	*/
	void	setDraggingEnabled(bool setting);


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for Titlebar base class.
	*/
	Titlebar(const String& type, const String& name);


	/*!
	\brief
		Destructor for Titlebar base class.
	*/
	virtual ~Titlebar(void);


protected:
	/*************************************************************************
		Overridden event handler functions
	*************************************************************************/
	virtual void	onMouseMove(MouseEventArgs& e);
	virtual void	onMouseButtonDown(MouseEventArgs& e);
	virtual void	onMouseButtonUp(MouseEventArgs& e);
	virtual void	onMouseDoubleClicked(MouseEventArgs& e);
	virtual void	onCaptureLost(WindowEventArgs& e);
	virtual void	onFontChanged(WindowEventArgs &e);


	/*************************************************************************
		New event handlers for title bar
	*************************************************************************/
	/*!
	\brief
		Event handler called when the 'draggable' state for the title bar is changed.
		
		Note that this is for 'internal' use at the moment and as such does not add or
		fire a public Event that can be subscribed to.
	*/
	virtual void	onDraggingModeChanged(WindowEventArgs&) {}


	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Return whether this window was inherited from the given class name at some point in the inheritance hierarchy.

	\param class_name
		The class name that is to be checked.

	\return
		true if this window was inherited from \a class_name. false if not.
	*/
	virtual bool	testClassName_impl(const String& class_name) const
	{
		if (class_name=="Titlebar")	return true;
		return Window::testClassName_impl(class_name);
	}


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	bool	d_dragging;			//!< set to true when the window is being dragged.
	Point	d_dragPoint;		//!< Point at which we are being dragged.
	bool	d_dragEnabled;		//!< true when dragging for the widget is enabled.

	Rect	d_oldCursorArea;	//!< Used to backup cursor restraint area.

private:
	/*************************************************************************
		Static Properties for this class
	*************************************************************************/
	static TitlebarProperties::DraggingEnabled	d_dragEnabledProperty;


	/*************************************************************************
		Private methods
	*************************************************************************/
	void	addTitlebarProperties(void);
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUITitlebar_h_
