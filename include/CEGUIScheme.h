/************************************************************************
	filename: 	CEGUIScheme.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines abstract base class for the GUI Scheme object.
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
#ifndef _CEGUIScheme_h_
#define _CEGUIScheme_h_

#include "CEGUIBase.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	A class that groups a set of GUI elements and initialises the system to access those elements.

	A GUI Scheme is a high-level construct that loads and initialises various lower-level objects
	and registers them within the system for usage.  So, for example, a Scheme might create some
	Imageset objects, some Font objects, and register a collection of WindowFactory objects within
	the system which would then be in a state to serve those elements to client code.
*/
class CEGUIBASE_API Scheme
{
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIScheme_h_
