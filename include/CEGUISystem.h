/************************************************************************
	filename: 	CEGUISystem.h
	created:	20/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for main GUI system class
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
#ifndef _CEGUISystem_h_
#define _CEGUISystem_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"
#include "CEGUIRenderer.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
/brief
	Enumeration of mouse buttons
*/
enum MouseButton
{
	LeftButton,
	RightButton,
	MiddleButton,
	X1Button,
	X2Button,
};

/*!
\brief
	The System class is the CEGUI class that provides access to all other elements in this system.

	This object must be created by the client application.  The System object requires that you pass it
	an initialised Renderer object which it can use to interface to whatever rendering system will be
	used to display the GUI imagery.
*/
class CEGUIBASE_API System : public Singleton<System>
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructs a System object.

	\param renderer
		Pointer to the valid Renderer object that will be used to render GUI imagery

	*/
	System(Renderer* renderer);

	/*!
	\brief
		Destructor for System objects.
	*/
	~System(void);


	/*!
	\brief
		Return a pointer to the Renderer object being used by the system

	\return
		Pointer to the Renderer object used by the system.
	*/
	Renderer*	getRenderer(void) const			{return d_renderer;}


	/*!
	\brief
		Return singleton System object

	\return
		Singleton System object
	*/
	static	System&	getSingleton(void)			{return Singleton<System>::getSingleton();}


	/*!
	\brief
		Return pointer to singleton System object

	\return
		Pointer to singleton System object
	*/
	static	System*	getSingletonPtr(void)			{return Singleton<System>::getSingletonPtr();}


	/*!
	\brief
		Set the default font to be used by the system

	\param name
		String object containing the name of the font to be used as the system default.

	\return
		Nothing.
	*/
	void	setDefaultFont(const String& name);


	/*!
	\brief
		Set the default font to be used by the system

	\param font
		Pointer to the font to be used as the system default.

	\return
		Nothing.
	*/
	void	setDefaultFont(Font* font);


	/*!
	\brief
		Return a pointer to the default Font for the GUI system

	\return
		Pointer to a Font object that is the default font in the system.
	*/
	Font*	getDefaultFont(void) const				{return d_defaultFont;}


	/*!
	\brief
		Causes a full re-draw next time renderGUI() is called

	\return
		Nothing
	*/
	void	signalRedraw()		{d_gui_redraw = true;}


	/*!
	\brief
		Render the GUI

		Depending upon the internal state, this may either re-use rendering from last time, or trigger a full re-draw from all elements.

	\return
		Nothing
	*/
	void	renderGUI(void);


	/*!
	\brief
		Set the active GUI sheet (root) window.

	\param sheet
		Pointer to a Window object that will become the new GUI 'root'

	\return
		Pointer to the window that was previously set as the GUI root.
	*/
	Window*	setGUISheet(Window* sheet);


	/*!
	\brief
		Return a pointer to the active GUI sheet (root) window.

	\return
		Pointer to the window object that has been set as the GUI root element.
	*/
	Window*	getGUISheet(void) const		{return d_activeSheet;}


	/*************************************************************************
		Input injection interface
	*************************************************************************/
	/*!
	\brief
		Method that injects a mouse movement event into the system

	\param delta_x
		amount the mouse moved on the x axis.

	\param delta_y
		amount the mouse moved on the y axis.

	\return
		Nothing.
	*/
	void	injectMouseMove(int delta_x, int delta_y);


	/*!
	\brief
		Method that injects a mouse button down event into the system.

	\param button
		One of the MouseButton values indicating which button was pressed.

	\return
		Nothing.
	*/
	void	injectMouseButtonDown(MouseButton button);


	/*!
	\brief
		Method that injects a mouse button up event into the system.

	\param button
		One of the MouseButton values indicating which button was released.

	\return
		Nothing.
	*/
	void	injectMouseButtonUp(MouseButton button);


	/*!
	\brief
		Method that injects a key down event into the system.

	\param key_code
		uint value indicating which key was pressed.

	\return
		Nothing.
	*/
	void	injectKeyDown(uint key_code);


	/*!
	\brief
		Method that injects a key up event into the system.

	\param key_code
		uint value indicating which key was released.

	\return
		Nothing.
	*/
	void	injectKeyUp(uint key_code);


	/*!
	\brief
		Method that injects a typed character event into the system.

	\param code_point
		Unicode code point of the character that was typed.

	\return
		Nothing.
	*/
	void	injectChar(utf32 code_point);

private:
	/*************************************************************************
		Implementation Functions
	*************************************************************************/
	/*!
	\brief
		Given Point \a pt, return a pointer to the Window that should receive a mouse input if \a pt is the mouse location.

	\param pt
		Point object describing a screen location in pixels.

	\return
		Pointer to a Window object that should receive mouse input with the system in its current state and the mouse at location \a pt.
	*/
	Window*	getTargetWindow(const Point& pt) const;


	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Renderer*	d_renderer;			//!< Holds the pointer to the Renderer object given to us in the constructor
	Font*		d_defaultFont;		//!< Holds a pointer to the default GUI font.
	bool		d_gui_redraw;		//!< True if GUI should be re-drawn, false if render should re-use last times queue.

	Window*		d_wndWithMouse;		//!< Pointer to the window that currently contains the mouse.
	Window*		d_activeSheet;		//!< The active GUI sheet (root window)
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUISystem_h_
