/***********************************************************************
	filename: 	FalagardStaticTextProperties.h
	created:	17/9/2005
	author:		Tomas L Olsen (based on code by Paul D Turner)
	
	purpose:	Interface for properties for the FalagardStaticText class.
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
#ifndef _FalagardStaticImageProperties_h_
#define _FalagardStaticImageProperties_h_

#include "../../CEGUIProperty.h"

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
