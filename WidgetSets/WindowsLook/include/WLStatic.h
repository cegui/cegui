/************************************************************************
	filename: 	WLStatic.h
	created:	8/8/2004
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
#ifndef _WLStatic_h_
#define _WLStatic_h_

#include "WLModule.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIStaticText.h"
#include "elements/CEGUIStaticImage.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Routine to do some common initialisation of static widgets
*/
static void initWinLookStatic(Static* s);


/*!
\brief
	StaticText class for the WindowsLook Gui Scheme
*/
class WINDOWSLOOK_API WLStaticText : public StaticText
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WIndowsLook StaticText objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	WLStaticText(const String& type, const String& name) : StaticText(type, name) {}


	/*!
	\brief
		Destructor for WLStaticText objects.
	*/
	virtual ~WLStaticText(void) {}
};



/*!
\brief
	StaticImage class for the WindowsLook Gui Scheme
*/
class WINDOWSLOOK_API WLStaticImage : public StaticImage
{
public:
	/*************************************************************************
		Constants
	*************************************************************************/
	// type name for this widget
	static const utf8	WidgetTypeName[];				//!< The unique typename of this widget

	/*************************************************************************
		Construction / Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for WindowsLook StaticImage objects.

	\param type
		String object that specifies a type for this window, usually provided by a factory class.

	\param name
		String object that specifies a unique name that will be used to identify the new Window object
	*/
	WLStaticImage(const String& type, const String& name) : StaticImage(type, name) {}


	/*!
	\brief
		Destructor for WLStaticImage objects.
	*/
	virtual ~WLStaticImage(void) {}
};


/*!
\brief
	Factory class for producing StaticText objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLStaticTextFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLStaticTextFactory(void) : WindowFactory(WLStaticText::WidgetTypeName) { }
	~WLStaticTextFactory(void){}


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
	Factory class for producing StaticImage objects for the WindowsLook GUI Scheme
*/
class WINDOWSLOOK_API WLStaticImageFactory : public WindowFactory
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	WLStaticImageFactory(void) : WindowFactory(WLStaticImage::WidgetTypeName) { }
	~WLStaticImageFactory(void){}


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


#endif	// end of guard _WLStatic_h_
