/************************************************************************
	filename: 	CEGUIResourceProvider.h
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Defines abstract base class for loading DataContainer objects
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
#ifndef _CEGUIResourceProvider_h_
#define _CEGUIResourceProvider_h_

#include "CEGUIBase.h"
#include "CEGUIDataContainer.h"


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Abstract class that defines the required interface for all resource provider sub-classes.

	A ResourceProvider is used to load both XML and binary data from an external source.  This could be from a filesystem or the resource manager of a specific renderer.
*/
class CEGUIBASE_API ResourceProvider
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
    /*!
    \brief
        Constructor for the ResourceProvider class
    */
	ResourceProvider() { }

    /*!
    \brief
        Destructor for the ResourceProvider class
    */
	virtual ~ResourceProvider(void) { }

    /*************************************************************************
        Accessor functions
    *************************************************************************/

    /*!
    \brief
        Load XML data using InputSource objects.

    \param filename
        String containing a filename of the resource to be loaded.

	\param output
		Reference to a InputSourceContainer object to load the data into.
    */
    virtual void loadInputSourceContainer(const String& filename, InputSourceContainer& output) = 0;

    /*!
    \brief
        Load raw binary data.

    \param filename
        String containing a filename of the resource to be loaded.

	\param output
        Reference to a RawDataContainer object to load the data into.
    */
    virtual void loadRawDataContainer(const String& filename, RawDataContainer& output) = 0;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIResourceProvider_h_
