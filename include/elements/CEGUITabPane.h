/************************************************************************
	filename: 	CEGUITabPane.h
	created:	8/8/2004
	author:		Steve Streeting
	
	purpose:	Defines interface for the content area of a tab control
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
#ifndef _CEGUITabPane_h_
#define _CEGUITabPane_h_

#include "elements/CEGUIStatic.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Base class for a tab pane.
*/
class CEGUIBASE_API TabPane : public Static
{
public:

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for tab pane widgets.
	*/
    TabPane(const String& type, const String& name) : Static(type, name) {}


	/*!
	\brief
		Destructor for tab pane widgets.
	*/
    virtual ~TabPane(void) {}


	/*************************************************************************
		Accessors
	*************************************************************************/

	/*************************************************************************
		Manipulators
	*************************************************************************/

protected:
	/*************************************************************************
		Overridden from base class
	*************************************************************************/

    /*************************************************************************
		Implementation methods
	*************************************************************************/

	/*************************************************************************
		Implementation Data
	*************************************************************************/

private:
	/*************************************************************************
		Static Properties for this class
	*************************************************************************/


	/*************************************************************************
		Private methods
	*************************************************************************/
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUITabPane_h_
