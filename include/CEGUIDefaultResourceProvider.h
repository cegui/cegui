/************************************************************************
	filename: 	CEGUIDefaultResourceProvider.h
	created:	8/7/2004
	author:		James '_mental_' O'Sullivan
	
	purpose:	Defines abstract base class for CEGUIData objects
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _CEGUIDefaultResourceProvider_h_
#define _CEGUIDefaultResourceProvider_h_

#include "CEGUIBase.h"
#include "CEGUIResourceProvider.h"

#include <map>

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT DefaultResourceProvider : public ResourceProvider
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	DefaultResourceProvider() {}
	~DefaultResourceProvider(void) {}

    /*!
    \brief
        Set the directory associated with a given resource group identifier.

    \param resourceGroup
        The resource group identifier whose directory is to be set.

    \param directory
        The directory to be associated with resource group identifier
        \a resourceGroup

    \return
        Nothing.
    */
    void setResourceGroupDirectory(const String& resourceGroup, const String& directory);

    /*!
    \brief
        Return the directory associated with the specified resource group
        identifier.

    \param resourceGroup
        The resource group identifier for which the associated directory is to
        be returned.

    \return
        String object describing the directory currently associated with resource
        group identifier \a resourceGroup.

    \note
        This member is not defined as being const because it may cause
        creation of an 'empty' directory specification for the resourceGroup
        if the resourceGroup has not previously been accessed.
    */
    const String& getResourceGroupDirectory(const String& resourceGroup);

    /*!
    \brief
        clears any currently set directory for the specified resource group
        identifier.

    \param resourceGroup
        The resource group identifier for which the associated directory is to
        be cleared.
    */
    void clearResourceGroupDirectory(const String& resourceGroup);

    void loadRawDataContainer(const String& filename, RawDataContainer& output, const String& resourceGroup);

protected:
    /*!
    \brief
        Return the final path and filename, taking into account the given
        resource group identifier that should be used when attempting to
        load the data.
    */
    String getFinalFilename(const String& filename, const String& resourceGroup) const;

    typedef std::map<String, String, String::FastLessCompare> ResourceGroupMap;
    ResourceGroupMap    d_resourceGroups;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIDefaultResourceProvider_h_
