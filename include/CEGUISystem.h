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
	virtual ~System(void);


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


private:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	Renderer*	d_renderer;		//!< Holds the pointer to the Renderer object given to us in the constructor
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUISystem_h_
