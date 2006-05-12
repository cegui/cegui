/***********************************************************************
	filename: 	CEGUIFrameWindowProperties.h
	created:	10/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to properties for FrameWIndow class
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
#ifndef _CEGUIFrameWindowProperties_h_
#define _CEGUIFrameWindowProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of FrameWindowProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the FrameWindow class
*/
namespace FrameWindowProperties
{
/*!
\brief
	Property to access the state of the sizable setting for the FrameWindow.

	\par Usage:
		- Name: SizingEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the window will be user re-sizable.
		- "False" to indicate the window will not be re-sizable by the user.
*/
class SizingEnabled : public Property
{
public:
	SizingEnabled() : Property(
		"SizingEnabled",
		"Property to get/set the state of the sizable setting for the FrameWindow.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for whether the window frame will be displayed.

	\par Usage:
		- Name: FrameEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the windows frame should be displayed.
		- "False" to indicate the windows frame should not be displayed.
*/
class FrameEnabled : public Property
{
public:
	FrameEnabled() : Property(
		"FrameEnabled",
		"Property to get/set the setting for whether the window frame will be displayed.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for whether the window title-bar will be enabled (or displayed depending upon choice of final widget type).

	\par Usage:
		- Name: TitlebarEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the windows title bar should be enabled (and/or visible)
		- "False" to indicate the windows title bar should be disabled (and/or hidden)
*/
class TitlebarEnabled : public Property
{
public:
	TitlebarEnabled() : Property(
		"TitlebarEnabled",
		"Property to get/set the setting for whether the window title-bar will be enabled (or displayed depending upon choice of final widget type).  Value is either \"True\" or \"False\".",
		"") 
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for whether the window close button will be enabled (or displayed depending upon choice of final widget type).

	\par Usage:
		- Name: CloseButtonEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the windows close button should be enabled (and/or visible)
		- "False" to indicate the windows close button should be disabled (and/or hidden)
*/
class CloseButtonEnabled : public Property
{
public:
	CloseButtonEnabled() : Property(
		"CloseButtonEnabled",
		"Property to get/set the setting for whether the window close button will be enabled (or displayed depending upon choice of final widget type).  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for whether the user is able to roll-up / shade the window.

	\par Usage:
		- Name: RollUpEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the user can roll-up / shade the window.
		- "False" to indicate the user can not roll-up / shade the window.
*/
class RollUpEnabled : public Property
{
public:
	RollUpEnabled() : Property(
		"RollUpEnabled",
		"Property to get/set the setting for whether the user is able to roll-up / shade the window.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the roll-up / shade state of the window.

	\par Usage:
		- Name: RollUpState
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the window is / should be rolled-up.
		- "False" to indicate the window is not / should not be rolled up
*/
class RollUpState : public Property
{
public:
	RollUpState() : Property(
		"RollUpState",
		"Property to get/set the roll-up / shade state of the window.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for whether the user may drag the window around by its title bar.

	\par Usage:
		- Name: DragMovingEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the window may be repositioned by the user via dragging.
		- "False" to indicate the window may not be repositioned by the user.
*/
class DragMovingEnabled : public Property
{
public:
	DragMovingEnabled() : Property(
		"DragMovingEnabled",
		"Property to get/set the setting for whether the user may drag the window around by its title bar.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the setting for the sizing border thickness.

	\par Usage:
		- Name: SizingBorderThickness
		- Format: "[float]".

	\par Where:
		- [float] is the size of the invisible sizing border in screen pixels.
*/
class SizingBorderThickness : public Property
{
public:
	SizingBorderThickness() : Property(
		"SizingBorderThickness",
		"Property to get/set the setting for the sizing border thickness.  Value is a float specifying the border thickness in pixels.",
		"8")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
   Property to access the N-S (up-down) sizing cursor image

   \par Usage:
      - Name: NSSizingCursorImage
      - Format: "set:<imageset> image:<imagename>".

*/
class NSSizingCursorImage : public Property
{
public:
   NSSizingCursorImage() : Property(
	   "NSSizingCursorImage",
	   "Property to get/set the N-S (up-down) sizing cursor image for the FramwWindow.  Value should be \"set:[imageset name] image:[image name]\".",
	   "")
   {}

   String   get(const PropertyReceiver* receiver) const;
   void   set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
   Property to access the E-W (left/right) sizing cursor image

   \par Usage:
      - Name: EWSizingCursorImage
      - Format: "set:<imageset> image:<imagename>".

*/
class EWSizingCursorImage : public Property
{
public:
   EWSizingCursorImage() : Property(
	   "EWSizingCursorImage",
	   "Property to get/set the E-W (left-right) sizing cursor image for the FramwWindow.  Value should be \"set:[imageset name] image:[image name]\".",
	   "")
   {}

   String   get(const PropertyReceiver* receiver) const;
   void   set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
   Property to access the NW-SE diagonal sizing cursor image

   \par Usage:
      - Name: NWSESizingCursorImage
      - Format: "set:<imageset> image:<imagename>".

*/
class NWSESizingCursorImage : public Property
{
public:
   NWSESizingCursorImage() : Property(
	   "NWSESizingCursorImage",
	   "Property to get/set the NW-SE diagonal sizing cursor image for the FramwWindow.  Value should be \"set:[imageset name] image:[image name]\".",
	   "")
   {}

   String   get(const PropertyReceiver* receiver) const;
   void   set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
   Property to access the NE-SW diagonal sizing cursor image

   \par Usage:
      - Name: NESWSizingCursorImage
      - Format: "set:<imageset> image:<imagename>".

*/
class NESWSizingCursorImage : public Property
{
public:
   NESWSizingCursorImage() : Property(
	   "NESWSizingCursorImage",
	   "Property to get/set the NE-SW diagonal sizing cursor image for the FramwWindow.  Value should be \"set:[imageset name] image:[image name]\".",
	   "")
   {}

   String   get(const PropertyReceiver* receiver) const;
   void   set(PropertyReceiver* receiver, const String& value);
};

} // End of  FrameWindowProperties namespace section

} // End of  CEGUI namespace section


#endif	// end of guard _CEGUIFrameWindowProperties_h_
