/************************************************************************
	filename: 	CEGUIScriptModule.h
	created:	16/7/2004
	author:		Paul D Turner
	
	purpose:	Abstract class interface for scripting support
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
#ifndef _CEGUIScriptModule_h_
#define _CEGUIScriptModule_h_

#include "CEGUIBase.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Abstract interface required for all scripting support modules to be used with
	the CEGUI system.
*/
class CEGUIBASE_API ScriptModule
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for ScriptModule base class
	*/
	ScriptModule(void);


	/*!
	\brief
		Destructor for ScriptModule base class.
	*/
	virtual ~ScriptModule(void);


	/*************************************************************************
		Abstract interface
	*************************************************************************/
	/*!
	\brief
		Execute a script file.

	\param filename
		String object holding the filename of the script file that is to be executed
	*/
	virtual	void	executeScriptFile(const String& filename)	= 0;


	/*!
	\brief
		Execute a scripted global function.  The function should not take any parameters and should return an integer.

	\param function_name
		String object holding the name of the function, in the global script environment, that
		is to be executed.

	\return
		The integer value returned from the script function.
	*/
	virtual int	executeScriptGloabl(const String& function_name)	= 0;
};

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIScriptModule_h_
