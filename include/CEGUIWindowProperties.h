/************************************************************************
	filename: 	CEGUIWindowProperties.h
	created:	5/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to available window base class properties
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
#ifndef _CEGUIWindowProperties_h_
#define _CEGUIWindowProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of WindowProperties namespace section
/*!
\brief
	Namespace containing all classes that make up the properties interface for the Window base class
*/
namespace WindowProperties
{
/*!
\brief
	Property to access minimum window size.

	This property offers access to the minimum size setting for the window, using screen relative metrics.

	\par Usage:
		- Name: RelativeMinSize
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the minimum width as a floating point number.
		- h:[float] specifies the minimum height as a floating point number.
*/
class RelativeMinSize : public Property
{
public:
	RelativeMinSize() : Property((utf8*)"RelativeMinSize", (utf8*)"Property to get/set the minimum size for the Window.  Value is \"w:[float] h:[float]\" using relative metrics (this setting is relative to the display size).") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access maximum window size.

	This property offers access to the maximum size setting for the window, using screen relative metrics.

	\par Usage:
		- Name: RelativeMaxSize
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the maximum width as a floating point number.
		- h:[float] specifies the maximum height as a floating point number.
*/
class RelativeMaxSize : public Property
{
public:
	RelativeMaxSize() : Property((utf8*)"RelativeMaxSize", (utf8*)"Property to get/set the maximum size for the Window.  Value is \"w:[float] h:[float]\" using relative metrics (this setting is relative to the display size).") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access minimum window size.

	This property offers access to the minimum size setting for the window, using absolute screen pixel metrics.

	\par Usage:
		- Name: AbsoluteMinSize
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the minimum width as a floating point number.
		- h:[float] specifies the minimum height as a floating point number.
*/
class AbsoluteMinSize : public Property
{
public:
	AbsoluteMinSize() : Property((utf8*)"AbsoluteMinSize", (utf8*)"Property to get/set the minimum size for the Window.  Value is \"w:[float] h:[float]\" using absolute (pixel) metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access maximum window size.

	This property offers access to the maximum size setting for the window, using absolute screen pixel metrics.

	\par Usage:
		- Name: AbsoluteMaxSize
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the maximum width as a floating point number.
		- h:[float] specifies the maximum height as a floating point number.
*/
class AbsoluteMaxSize : public Property
{
public:
	AbsoluteMaxSize() : Property((utf8*)"AbsoluteMaxSize", (utf8*)"Property to get/set the maximum size for the Window.  Value is \"w:[float] h:[float]\" using absolute (pixel) metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the metrics mode setting.

	This property offers access to the metrics mode setting for the window.

	\par Usage:
		- Name: MetricsMode
		- Format: "[text]".

	\par Where [text] is:
		- "Relative" for the relative metrics mode.
		- "Absolute" for the absolute metrics mode.
		- "Inherited" if metrics should be inherited from the parent (only used with set method).
*/
class MetricsMode : public Property
{
public:
	MetricsMode() : Property((utf8*)"MetricsMode", (utf8*)"Property to get/set the metrics mode for the Window.  Value is \"Relative\", \"Absolute\", or \"Inherited\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window ID field.

	This property offers access to the client specified ID for the window.

	\par Usage:
		- Name: ID
		- Format: "[uint]".

	\par Where:
		- [uint] is any unsigned integer value.
*/
class ID : public Property
{
public:
	ID() : Property((utf8*)"ID", (utf8*)"Property to get/set the ID value of the Window.  Value is an unsigned integer number.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window alpha setting.

	This property offers access to the alpha-blend setting for the window.

	\par Usage:
		- Name: Alpha
		- Format: "[float]".

	\par Where:
		- [float] is a floating point number between 0.0 and 1.0.
*/
class Alpha : public Property
{
public:
	Alpha() : Property((utf8*)"Alpha", (utf8*)"Property to get/set the alpha value of the Window.  Value is floating point number.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Font setting.

	This property offers access to the current Font setting for the window.

	\par Usage:
		- Name: Font
		- Format: "[text]".

	\par Where:
		- [text] is the name of the Font to assign for this window.  The Font specified must already be loaded.
*/
class Font : public Property
{
public:
	Font() : Property((utf8*)"Font", (utf8*)"Property to get/set the font for the Window.  Value is the name of the font to use (must be loaded already).") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window text setting.

	This property offers access to the current text for the window.

	\par Usage:
		- Name: Text
		- Format: "[text]".

	\par Where:
		- [text] is the name of the Font to assign for this window.  The Font specified must already be loaded.
*/
class Text : public Property
{
public:
	Text() : Property((utf8*)"Text", (utf8*)"Property to get/set the text / caption for the Window.  Value is the text string to use.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window mouse cursor setting.

	This property offers access to the current mouse cursor image for the window.

	\par Usage:
		- Name: MouseCursorImage
		- Format: "set:[text] image:[text]".

	\par Where:
		- set:[text] is the name of the Imageset containing the image.  The Imageset name should not contain spaces.  The Imageset specified must already be loaded.
		- image:[text] is the name of the Image on the specified Imageset.  The Image name should not contain spaces.
*/
class MouseCursorImage : public Property
{
public:
	MouseCursorImage() : Property((utf8*)"MouseCursorImage", (utf8*)"Property to get/set the mouse cursor image for the Window.  Value should be \"set:<imageset name> image:<image name>\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window "clipped by parent" setting.

	This property offers access to the clipped by parent setting for the window.

	\par Usage:
		- Name: ClippedByParent
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window is clipped by it's parent.
		- "False" to indicate the Window is not clipped by it's parent.
*/
class ClippedByParent : public Property
{
public:
	ClippedByParent() : Property((utf8*)"ClippedByParent", (utf8*)"Property to get/set the 'clipped by parent' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window "Inherits Alpha" setting.

	This property offers access to the inherits alpha setting for the window.

	\par Usage:
		- Name: InheritsAlpha
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window inherits alpha blend values from it's ancestors.
		- "False" to indicate the Window does not inherit alpha blend values from it's ancestors.
*/
class InheritsAlpha : public Property
{
public:
	InheritsAlpha() : Property((utf8*)"InheritsAlpha", (utf8*)"Property to get/set the 'inherits alpha' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window "Always-On-Top" setting.

	This property offers access to the always on top / topmost setting for the window.

	\par Usage:
		- Name: AlwaysOnTop
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window is always on top, and appears above all other non-always on top Windows.
		- "False" to indicate the Window is not always on top, and will appear below all other always on top Windows.
*/
class AlwaysOnTop : public Property
{
public:
	AlwaysOnTop() : Property((utf8*)"AlwaysOnTop", (utf8*)"Property to get/set the 'always on top' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Disabled setting.

	This property offers access to the enabled / disabled setting for the window.

	\par Usage:
		- Name: Disabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window is disabled, and will normally receive no inputs from the user.
		- "False" to indicate the Window is not disabled and will receive inputs from the user as normal.
*/
class Disabled : public Property
{
public:
	Disabled() : Property((utf8*)"Disabled", (utf8*)"Property to get/set the 'disabled state' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Visible setting.

	This property offers access to the visible setting for the window.

	\par Usage:
		- Name: Visible
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window is visible.
		- "False" to indicate the Window is not visible.
*/
class Visible : public Property
{
public:
	Visible() : Property((utf8*)"Visible", (utf8*)"Property to get/set the 'visible state' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Restore Old Capture setting.

	This property offers access to the restore old capture setting for the window.  This setting is of generally limited use, it
	is primary purpose is for certain operations required for compound widgets.

	\par Usage:
		- Name: RestoreOldCapture
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window should restore any previous capture Window when it loses input capture.
		- "False" to indicate the Window should not restore the old capture Window.  This is the default behaviour.
*/
class RestoreOldCapture : public Property
{
public:
	RestoreOldCapture() : Property((utf8*)"RestoreOldCapture", (utf8*)"Property to get/set the 'restore old capture' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Destroyed by Parent setting.

	This property offers access to the destryed by parent setting for the window.

	\par Usage:
		- Name: DestroyedByParent
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window should be automatically destroyed when it's parent Window is destroyed.
		- "False" to indicate the Window should not be destroyed when it's parent Window is destroyed.
*/
class DestroyedByParent : public Property
{
public:
	DestroyedByParent() : Property((utf8*)"DestroyedByParent", (utf8*)"Property to get/set the 'destroyed by parent' setting for the Window.  Value is either \"True\" or \"False\".") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window width.

	This property offers access to the Width setting for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: Width
		- Format: "[float]".

	\par Where:
		- [float]	specifies the width as a floating point number, using the active metrics system for the Window.
*/
class Width : public Property
{
public:
	Width() : Property((utf8*)"Width", (utf8*)"Property to get/set the width of the Window.  Value is floating point using the active metrics mode.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window width.

	This property offers access to the Width setting for the window, using the relative metrics mode.

	\par Usage:
		- Name: RelativeWidth
		- Format: "[float]".

	\par Where:
		- [float]	specifies the width as a floating point number, using the relative metrics system.
*/
class RelativeWidth : public Property
{
public:
	RelativeWidth() : Property((utf8*)"RelativeWidth", (utf8*)"Property to get/set the width of the Window.  Value is floating point using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window width.

	This property offers access to the Width setting for the window, using the absolute metrics mode.

	\par Usage:
		- Name: AbsoluteWidth
		- Format: "[float]".

	\par Where:
		- [float]	specifies the width as a floating point number, using the absolute metrics system.
*/
class AbsoluteWidth: public Property
{
public:
	AbsoluteWidth() : Property((utf8*)"AbsoluteWidth", (utf8*)"Property to get/set the width of the Window.  Value is floating point using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window height.

	This property offers access to the Height setting for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: Height
		- Format: "[float]".

	\par Where:
		- [float]	specifies the height as a floating point number, using the active metrics system for the Window.
*/
class Height : public Property
{
public:
	Height() : Property((utf8*)"Height", (utf8*)"Property to get/set the height of the Window.  Value is floating point using the active metrics mode.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window height.

	This property offers access to the Height setting for the window, using the relative metrics mode.

	\par Usage:
		- Name: RelativeHeight
		- Format: "[float]".

	\par Where:
		- [float]	specifies the height as a floating point number, using the relative metrics system.
*/
class RelativeHeight : public Property
{
public:
	RelativeHeight() : Property((utf8*)"RelativeHeight", (utf8*)"Property to get/set the height of the Window.  Value is floating point using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window height.

	This property offers access to the Height setting for the window, using the absolute metrics mode.

	\par Usage:
		- Name: AbsoluteHeight
		- Format: "[float]".

	\par Where:
		- [float]	specifies the height as a floating point number, using the absolute metrics system.
*/
class AbsoluteHeight : public Property
{
public:
	AbsoluteHeight() : Property((utf8*)"AbsoluteHeight", (utf8*)"Property to get/set the height of the Window.  Value is floating point using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the window size.

	This property offers access to the size setting for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: Size
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the minimum width as a floating point number, using the active metrics system for the Window.
		- h:[float] specifies the minimum height as a floating point number, using the active metrics system for the Window.
*/
class Size : public Property
{
public:
	Size() : Property((utf8*)"Size", (utf8*)"Property to get/set the size of the Window.  Value is \"w:[float] h:[float]\" using the active metrics mode.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the window size.

	This property offers access to the size setting for the window, using the relative metrics system.

	\par Usage:
		- Name: RelativeSize
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the minimum width as a floating point number, using the relative metrics system.
		- h:[float] specifies the minimum height as a floating point number, using the relative metrics system.
*/
class RelativeSize : public Property
{
public:
	RelativeSize() : Property((utf8*)"RelativeSize", (utf8*)"Property to get/set the size of the Window.  Value is \"w:[float] h:[float]\" using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access the window size.

	This property offers access to the size setting for the window, using the absolute metrics system.

	\par Usage:
		- Name: AbsoluteSize
		- Format: "w:[float] h:[float]".

	\par Where:
		- w:[float]	specifies the minimum width as a floating point number, using the absolute metrics system.
		- h:[float] specifies the minimum height as a floating point number, using the absolute metrics system.
*/
class AbsoluteSize : public Property
{
public:
	AbsoluteSize() : Property((utf8*)"AbsoluteSize", (utf8*)"Property to get/set the size of the Window.  Value is \"w:[float] h:[float]\" using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window X position.

	This property offers access to the X position for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: XPosition
		- Format: "[float]".

	\par Where:
		- [float]	specifies the x position co-ordinate as a floating point number, using the active metrics system for the Window.
*/
class XPosition : public Property
{
public:
	XPosition() : Property((utf8*)"XPosition", (utf8*)"Property to get/set the x co-ordinate position of the Window.  Value is a floating point number using the active metrics mode.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window X position.

	This property offers access to the X position for the window, using the relative metrics system.

	\par Usage:
		- Name: RelativeXPosition
		- Format: "[float]".

	\par Where:
		- [float]	specifies the x position co-ordinate as a floating point number, using the relative metrics system.
*/
class RelativeXPosition : public Property
{
public:
	RelativeXPosition() : Property((utf8*)"RelativeXPosition", (utf8*)"Property to get/set the x co-ordinate position of the Window.  Value is a floating point number using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window X position.

	This property offers access to the X position for the window, using the absolute metrics system.

	\par Usage:
		- Name: AbsoluteXPosition
		- Format: "[float]".

	\par Where:
		- [float]	specifies the x position co-ordinate as a floating point number, using the absolute metrics system.
*/
class AbsoluteXPosition : public Property
{
public:
	AbsoluteXPosition() : Property((utf8*)"AbsoluteXPosition", (utf8*)"Property to get/set the x co-ordinate position of the Window.  Value is a floating point number using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Y position.

	This property offers access to the Y position for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: YPosition
		- Format: "[float]".

	\par Where:
		- [float]	specifies the y position co-ordinate as a floating point number, using the active metrics system for the Window.
*/
class YPosition : public Property
{
public:
	YPosition() : Property((utf8*)"YPosition", (utf8*)"Property to get/set the y co-ordinate position of the Window.  Value is a floating point number using the active metrics mode.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Y position.

	This property offers access to the Y position for the window, using the relative metrics system.

	\par Usage:
		- Name: RelativeYPosition
		- Format: "[float]".

	\par Where:
		- [float]	specifies the y position co-ordinate as a floating point number, using the relative metrics system.
*/
class RelativeYPosition : public Property
{
public:
	RelativeYPosition() : Property((utf8*)"RelativeYPosition", (utf8*)"Property to get/set the y co-ordinate position of the Window.  Value is a floating point number using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window Y position.

	This property offers access to the Y position for the window, using the absolute metrics system.

	\par Usage:
		- Name: AbsoluteYPosition
		- Format: "[float]".

	\par Where:
		- [float]	specifies the y position co-ordinate as a floating point number, using the absolute metrics system.
*/
class AbsoluteYPosition : public Property
{
public:
	AbsoluteYPosition() : Property((utf8*)"AbsoluteYPosition", (utf8*)"Property to get/set the y co-ordinate position of the Window.  Value is a floating point number using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window position.

	This property offers access to the position for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: Position
		- Format: "x:[float] y:[float]".

	\par Where:
		- x:[float]	specifies the x position co-ordinate as a floating point number, using the active metrics system for the Window.
		- y:[float]	specifies the y position co-ordinate as a floating point number, using the active metrics system for the Window.
*/
class Position : public Property
{
public:
	Position() : Property((utf8*)"Position", (utf8*)"Property to get/set the position of the Window.  Value is \"x:[float] y:[float]\" using the active metrics mode.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window position.

	This property offers access to the position for the window, using the relative metrics system.

	\par Usage:
		- Name: RelativePosition
		- Format: "x:[float] y:[float]".

	\par Where:
		- x:[float]	specifies the x position co-ordinate as a floating point number, using the relative metrics system.
		- y:[float]	specifies the y position co-ordinate as a floating point number, using the relative metrics system.
*/
class RelativePosition : public Property
{
public:
	RelativePosition() : Property((utf8*)"RelativePosition", (utf8*)"Property to get/set the position of the Window.  Value is \"x:[float] y:[float]\" using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window position.

	This property offers access to the position for the window, using the absolute metrics system.

	\par Usage:
		- Name: AbsolutePosition
		- Format: "x:[float] y:[float]".

	\par Where:
		- x:[float]	specifies the x position co-ordinate as a floating point number, using the absolute metrics system.
		- y:[float]	specifies the y position co-ordinate as a floating point number, using the absolute metrics system.
*/
class AbsolutePosition : public Property
{
public:
	AbsolutePosition() : Property((utf8*)"AbsolutePosition", (utf8*)"Property to get/set the position of the Window.  Value is \"x:[float] y:[float]\" using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window area rectangle.

	This property offers access to the area rectangle (Rect) for the window, using the Windows active metrics mode.

	\par Usage:
		- Name: Rect
		- Format: "l:[float] t:[float] r:[float] b:[float]".

	\par Where:
		- l:[float]	specifies the position of the left edge of the area as a floating point number, using the active metrics system for the Window.
		- t:[float]	specifies the position of the top edge of the area as a floating point number, using the active metrics system for the Window.
		- r:[float]	specifies the position of the right edge of the area as a floating point number, using the active metrics system for the Window.
		- b:[float]	specifies the position of the bottom edge of the area as a floating point number, using the active metrics system for the Window.
*/
class Rect : public Property
{
public:
	Rect() : Property((utf8*)"Rect", (utf8*)"Property to get/set the area rectangle of the Window.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom) using the active metrics system.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window area rectangle.

	This property offers access to the area rectangle (Rect) for the window, using the relative metrics system.

	\par Usage:
		- Name: RelativeRect
		- Format: "l:[float] t:[float] r:[float] b:[float]".

	\par Where:
		- l:[float]	specifies the position of the left edge of the area as a floating point number, using the relative metrics system.
		- t:[float]	specifies the position of the top edge of the area as a floating point number, using the relative metrics system.
		- r:[float]	specifies the position of the right edge of the area as a floating point number, using the relative metrics system.
		- b:[float]	specifies the position of the bottom edge of the area as a floating point number, using the relative metrics system.
*/
class RelativeRect : public Property
{
public:
	RelativeRect() : Property((utf8*)"RelativeRect", (utf8*)"Property to get/set the area rectangle of the Window.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom) using relative metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


/*!
\brief
	Property to access window area rectangle.

	This property offers access to the area rectangle (Rect) for the window, using the absolute metrics system.

	\par Usage:
		- Name: AbsoluteRect
		- Format: "l:[float] t:[float] r:[float] b:[float]".

	\par Where:
		- l:[float]	specifies the position of the left edge of the area as a floating point number, using the absolute metrics system.
		- t:[float]	specifies the position of the top edge of the area as a floating point number, using the absolute metrics system.
		- r:[float]	specifies the position of the right edge of the area as a floating point number, using the absolute metrics system.
		- b:[float]	specifies the position of the bottom edge of the area as a floating point number, using the absolute metrics system.
*/
class AbsoluteRect : public Property
{
public:
	AbsoluteRect() : Property((utf8*)"AbsoluteRect", (utf8*)"Property to get/set the area rectangle of the Window.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom) using absolute metrics.") {}

	String	get(const void* receiver) const;
	void	set(void* receiver, const String& value);
};


} // End of  WindowProperties namespace section


} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindowProperties_h_
