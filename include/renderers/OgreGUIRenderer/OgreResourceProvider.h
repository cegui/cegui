/************************************************************************
	filename: 	CEGUIOgreResourceProvider.h
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Defines abstract base class for CEGUIData objects
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
#ifndef _CEGUIOgreResourceProvider_h_
#define _CEGUIOgreResourceProvider_h_

#include "CEGUIResourceProvider.h"


// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIBASE_API OgreResourceProvider : public ResourceProvider
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	OgreResourceProvider() : ResourceProvider() {}

	~OgreResourceProvider(void) {}

    void loadInputSourceContainer(const String& filename, InputSourceContainer& output);
    void loadRawDataContainer(const String& filename, RawDataContainer& output);
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIOgreResourceProvider_h_
