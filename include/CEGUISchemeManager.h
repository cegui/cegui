/************************************************************************
	filename: 	CEGUISchemeManager.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines the interface to the SchemeManager class
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
#ifndef _CEGUISchemeManager_h_
#define _CEGUISchemeManager_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISingleton.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	A class that manages the creation of, access to, and destruction of GUI Scheme objects
*/
class CEGUIBASE_API SchemeManager : public Singleton<SchemeManager>
{
public:
	/*!
	\brief
		Constructor for SchemeManager objects
	*/
	SchemeManager(void);


	/*!
	\brief
		Destructor for SchemeManager objects
	*/
	virtual ~SchemeManager(void);


	/*!
	\brief
		Return singleton SchemeManager object

	\return
		Singleton SchemeManager object
	*/
	static	SchemeManager&	getSingleton(void)			{return Singleton<SchemeManager>::getSingleton();}


	/*!
	\brief
		Return pointer to singleton SchemeManager object

	\return
		Pointer to singleton SchemeManager object
	*/
	static	SchemeManager*	getSingletonPtr(void)			{return Singleton<SchemeManager>::getSingletonPtr();}


};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUISchemeManager_h_
