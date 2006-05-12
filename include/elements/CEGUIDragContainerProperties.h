/***********************************************************************
	filename: 	CEGUIDragContainerProperties.h
	created:	15/2/2005
	author:		Paul D Turner
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
#ifndef _CEGUIDragContainerProperties_h_
#define _CEGUIDragContainerProperties_h_

#include "CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
// Start of DragContainerProperties namespace section
namespace DragContainerProperties
{
    /*!
    \brief
	    Property to access the state of the dragging enabled setting.

	    \par Usage:
		    - Name: DraggingEnabled
		    - Format: "[text]".

	    \par Where [Text] is:
		    - "True" to indicate that dragging is enabled.
		    - "False" to indicate that dragging is disabled.
    */
    class DraggingEnabled : public Property
    {
    public:
	    DraggingEnabled() : Property(
		    "DraggingEnabled",
		    "Property to get/set the state of the dragging enabled setting for the DragContainer.  Value is either \"True\" or \"False\".",
		    "True")
	    {}

	    String	get(const PropertyReceiver* receiver) const;
	    void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the dragging alpha value.

	    \par Usage:
		    - Name: DragAlpha
		    - Format: "[float]".

	    \par Where:
		    - [float] represents the alpha value to set when dragging.
    */
    class DragAlpha : public Property
    {
    public:
	    DragAlpha() : Property(
		    "DragAlpha",
		    "Property to get/set the dragging alpha value.  Value is a float.",
		    "0.500000")
	    {}

	    String get(const PropertyReceiver* receiver) const;
	    void set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the dragging threshold value.

	    \par Usage:
		    - Name: DragThreshold
		    - Format: "[float]".

	    \par Where:
		    - [float] represents the movement threshold (in pixels) which must be exceeded to commence dragging.
    */
    class DragThreshold : public Property
    {
    public:
	    DragThreshold() : Property(
		    "DragThreshold",
		    "Property to get/set the dragging threshold value.  Value is a float.",
		    "8.000000")
	    {}

	    String get(const PropertyReceiver* receiver) const;
	    void set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
	    Property to access the dragging mouse cursor setting.

	    This property offers access to the mouse cursor image used when dragging the DragContainer.

	    \par Usage:
		    - Name: DragCursorImage
		    - Format: "set:[text] image:[text]".

	    \par Where:
		    - set:[text] is the name of the Imageset containing the image.  The Imageset name should not contain spaces.  The Imageset specified must already be loaded.
		    - image:[text] is the name of the Image on the specified Imageset.  The Image name should not contain spaces.
    */
    class DragCursorImage : public Property
    {
    public:
	    DragCursorImage() : Property(
		    "DragCursorImage",
		    "Property to get/set the mouse cursor image used when dragging.  Value should be \"set:<imageset name> image:<image name>\".",
		    "")
	    {}

	    String	get(const PropertyReceiver* receiver) const;
	    void	set(PropertyReceiver* receiver, const String& value);
    };

} // End of  DragContainerProperties namespace section
} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIDragContainerProperties_h_
