/************************************************************************
	filename: 	TLStatic.h
	created:	5/6/2004
	author:		Paul D Turner
	
	purpose:	Interface to Taharez look static widgets & factories
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
#ifndef _TLStatic_h_
#define _TLStatic_h_

#include "TLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIStaticText.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Routine to do some common initialisation of static widgets
*/
static void initTaharezStatic(Static* s);


/*!
\brief
	StaticText class for the TaharezLook Gui Scheme
*/
class TAHAREZLOOK_API TLStaticText : public StaticText
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// component widget type names
	static const utf8	HorzScrollbarTypeName[];	//!< Type name of widget to be created as horizontal scroll bar.
	static const utf8	VertScrollbarTypeName[];	//!< Type name of widget to be created as vertical scroll bar.


	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for TaharezLook StaticText objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	TLStaticText(const String& type, const String& name) : StaticText(type, name) {}


	/*!
	\brief
		Destructor for WLStaticText objects.
	*/
	virtual ~TLStaticText(void) {}


protected:
	/*************************************************************************
		Implementation Methods (abstract)
	*************************************************************************/
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
};


/*!
\brief
	Factory class for producing StaticText objects for the Taharez GUI Scheme
*/
class TAHAREZLOOK_API TLStaticTextFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLStaticTextFactory(void) : WindowFactory((utf8*)"Taharez StaticText") { }
	~TLStaticTextFactory(void){}


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


/*!
\brief
	Factory class for producing StaticImage objects for the Taharez GUI Scheme
*/
class TAHAREZLOOK_API TLStaticImageFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	TLStaticImageFactory(void) : WindowFactory((utf8*)"Taharez StaticImage") { }
	~TLStaticImageFactory(void){}


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


#endif	// end of guard _TLStatic_h_
