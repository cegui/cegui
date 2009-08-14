/***********************************************************************
	filename: 	CEGUIListHeaderSegmentProperties.h
	created:	11/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for ListHeaderSegment class
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
#ifndef _CEGUIListHeaderSegmentProperties_h_
#define _CEGUIListHeaderSegmentProperties_h_

#include "../CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of ListHeaderSegmentProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the ListHeaderSegment class
*/
namespace ListHeaderSegmentProperties
{
/*!
\brief
	Property to access the sizable setting of the header segment.

	\par Usage:
		- Name: Sizable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the segment can be sized by the user.
		- "False" to indicate the segment can not be sized by the user.
*/
class Sizable : public Property
{
public:
	Sizable() : Property(
		"Sizable",
		"Property to get/set the sizable setting of the header segment.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the click-able setting of the header segment.

	\par Usage:
		- Name: Clickable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the segment can be clicked by the user.
		- "False" to indicate the segment can not be clicked by the user.
*/
class Clickable : public Property
{
public:
	Clickable() : Property(
		"Clickable",
		"Property to get/set the click-able setting of the header segment.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the drag-able setting of the header segment.

	\par Usage:
		- Name: Dragable
		- Format: "[text]"

	\par Where [Text] is:
		- "True" to indicate the segment can be dragged by the user.
		- "False" to indicate the segment can not be dragged by the user.
*/
class Dragable : public Property
{
public:
	Dragable() : Property(
		"Dragable",
		"Property to get/set the drag-able setting of the header segment.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the sort direction setting of the header segment.

	\par Usage:
		- Name: SortDirection
		- Format: "[text]"

	\par Where [Text] is one of:
		- "Ascending"
		- "Descending"
		- "None"
*/
class SortDirection : public Property
{
public:
	SortDirection() : Property(
		"SortDirection",
		"Property to get/set the sort direction setting of the header segment.  Value is the text of one of the SortDirection enumerated value names.",
		"None")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
   Property to access the segment sizing cursor image

   \par Usage:
      - Name: SizingCursorImage
      - Format: "set:<imageset> image:<imagename>".

*/
class SizingCursorImage : public Property
{
public:
   SizingCursorImage() : Property(
       "SizingCursorImage",
       "Property to get/set the sizing cursor image for the List Header Segment.  Value should be \"set:[imageset name] image:[image name]\".",
       "")
   {}

   String   get(const PropertyReceiver* receiver) const;
   void   set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
   Property to access the segment moving cursor image

   \par Usage:
      - Name: MovingCursorImage
      - Format: "set:<imageset> image:<imagename>".

*/
class MovingCursorImage : public Property
{
public:
   MovingCursorImage() : Property(
       "MovingCursorImage",
       "Property to get/set the moving cursor image for the List Header Segment.  Value should be \"set:[imageset name] image:[image name]\".",
       "")
   {}

   String   get(const PropertyReceiver* receiver) const;
   void   set(PropertyReceiver* receiver, const String& value);
};

} // End of  ListHeaderSegmentProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIListHeaderSegmentProperties_h_
