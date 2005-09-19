/************************************************************************
	filename: 	FalagardStaticTextProperties.h
	created:	17/9/2005
	author:		Tomas L Olsen (based on code by Paul D Turner)
	
	purpose:	Interface for properties for the FalagardStaticText class.
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
#ifndef _FalagardStaticImageProperties_h_
#define _FalagardStaticImageProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of FalagardStaticImageProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the FalagardStaticText class
*/
namespace FalagardStaticImageProperties
{

/*!
\brief
    Property to access the image for the FalagardStaticImage widget.

    \par Usage:
        - Name: Image
        - Format: "set:[text] image:[text]".

    \par Where:
        - set:[text] is the name of the Imageset containing the image.  The Imageset name should not contain spaces.  The Imageset specified must already be loaded.
        - image:[text] is the name of the Image on the specified Imageset.  The Image name should not contain spaces.
*/
class Image : public Property
{
public:
    Image() : Property(
        "Image",
        "Property to get/set the image for the FalagardStaticImage widget.  Value should be \"set:[imageset name] image:[image name]\".",
        "")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

} // End of  FalagardStaticImageProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _FalagardStaticImageProperties_h_
