/************************************************************************
	filename: 	CEGUIWindowProperties.h
	created:	5/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to available window base class properties
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
	RelativeMinSize() : Property(
		"RelativeMinSize",
		"Property to get/set the minimum size for the Window.  Value is \"w:[float] h:[float]\" using relative metrics (this setting is relative to the display size).",
		"w:0.000000 h:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeMaxSize() : Property(
		"RelativeMaxSize",
		"Property to get/set the maximum size for the Window.  Value is \"w:[float] h:[float]\" using relative metrics (this setting is relative to the display size).",
		"w:1.000000 h:1.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteMinSize() : Property(
		"AbsoluteMinSize",
		"Property to get/set the minimum size for the Window.  Value is \"w:[float] h:[float]\" using absolute (pixel) metrics.",
		"w:0.000000 h:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteMaxSize() : Property(
		"AbsoluteMaxSize",
		"Property to get/set the maximum size for the Window.  Value is \"w:[float] h:[float]\" using absolute (pixel) metrics.",
		"")
		{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);

	// default depends upon current size of display.
	bool	isDefault(const PropertyReceiver* receiver) const;
	String	getDefault(const PropertyReceiver* receiver) const;
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
	MetricsMode() : Property(
		"MetricsMode",
		"Property to get/set the metrics mode for the Window.  Value is \"Relative\", \"Absolute\", or \"Inherited\".",
		"Relative")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	ID() : Property(
		"ID",
		"Property to get/set the ID value of the Window.  Value is an unsigned integer number.",
		"0")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Alpha() : Property(
		"Alpha",
		"Property to get/set the alpha value of the Window.  Value is floating point number.",
		"1.000000") 
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Font() : Property(
		"Font",
		"Property to get/set the font for the Window.  Value is the name of the font to use (must be loaded already).",
		"")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Text() : Property(
		"Text", 
		"Property to get/set the text / caption for the Window.  Value is the text string to use.",
		"")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	MouseCursorImage() : Property(
		"MouseCursorImage",
		"Property to get/set the mouse cursor image for the Window.  Value should be \"set:<imageset name> image:<image name>\".",
		"")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	ClippedByParent() : Property(
		"ClippedByParent",
		"Property to get/set the 'clipped by parent' setting for the Window.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	InheritsAlpha() : Property(
		"InheritsAlpha",
		"Property to get/set the 'inherits alpha' setting for the Window.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AlwaysOnTop() : Property(
		"AlwaysOnTop",
		"Property to get/set the 'always on top' setting for the Window.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Disabled() : Property(
		"Disabled",
		"Property to get/set the 'disabled state' setting for the Window.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Visible() : Property(
		"Visible",
		"Property to get/set the 'visible state' setting for the Window.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RestoreOldCapture() : Property(
		"RestoreOldCapture",
		"Property to get/set the 'restore old capture' setting for the Window.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	DestroyedByParent() : Property(
		"DestroyedByParent",
		"Property to get/set the 'destroyed by parent' setting for the Window.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Width() : Property(
		"Width",
		"Property to get/set the width of the Window.  Value is floating point using the active metrics mode.",
		"0.000000") {}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeWidth() : Property(
		"RelativeWidth",
		"Property to get/set the width of the Window.  Value is floating point using relative metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteWidth() : Property(
		"AbsoluteWidth",
		"Property to get/set the width of the Window.  Value is floating point using absolute metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Height() : Property(
		"Height",
		"Property to get/set the height of the Window.  Value is floating point using the active metrics mode.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeHeight() : Property(
		"RelativeHeight",
		"Property to get/set the height of the Window.  Value is floating point using relative metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteHeight() : Property(
		"AbsoluteHeight",
		"Property to get/set the height of the Window.  Value is floating point using absolute metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Size() : Property(
		"Size",
		"Property to get/set the size of the Window.  Value is \"w:[float] h:[float]\" using the active metrics mode.",
		"w:0.000000 h:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeSize() : Property(
		"RelativeSize",
		"Property to get/set the size of the Window.  Value is \"w:[float] h:[float]\" using relative metrics.",
		"w:0.000000 h:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteSize() : Property(
		"AbsoluteSize",
		"Property to get/set the size of the Window.  Value is \"w:[float] h:[float]\" using absolute metrics.",
		"w:0.000000 h:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	XPosition() : Property(
		"XPosition",
		"Property to get/set the x co-ordinate position of the Window.  Value is a floating point number using the active metrics mode.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeXPosition() : Property(
		"RelativeXPosition",
		"Property to get/set the x co-ordinate position of the Window.  Value is a floating point number using relative metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteXPosition() : Property(
		"AbsoluteXPosition",
		"Property to get/set the x co-ordinate position of the Window.  Value is a floating point number using absolute metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	YPosition() : Property(
		"YPosition",
		"Property to get/set the y co-ordinate position of the Window.  Value is a floating point number using the active metrics mode.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeYPosition() : Property(
		"RelativeYPosition",
		"Property to get/set the y co-ordinate position of the Window.  Value is a floating point number using relative metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteYPosition() : Property(
		"AbsoluteYPosition",
		"Property to get/set the y co-ordinate position of the Window.  Value is a floating point number using absolute metrics.",
		"0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Position() : Property(
		"Position",
		"Property to get/set the position of the Window.  Value is \"x:[float] y:[float]\" using the active metrics mode.",
		"x:0.000000 y:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativePosition() : Property(
		"RelativePosition",
		"Property to get/set the position of the Window.  Value is \"x:[float] y:[float]\" using relative metrics.",
		"x:0.000000 y:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsolutePosition() : Property(
		"AbsolutePosition",
		"Property to get/set the position of the Window.  Value is \"x:[float] y:[float]\" using absolute metrics.",
		"x:0.000000 y:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	Rect() : Property(
		"Rect",
		"Property to get/set the area rectangle of the Window.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom) using the active metrics system.",
		"l:0.000000 t:0.000000 r:0.000000 b:0.000000")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	RelativeRect() : Property(
		"RelativeRect",
		"Property to get/set the area rectangle of the Window.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom) using relative metrics.",
		"l:0.000000 t:0.000000 r:0.000000 b:0.000000") 
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
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
	AbsoluteRect() : Property(
		"AbsoluteRect",
		"Property to get/set the area rectangle of the Window.  Value is \"l:[float] t:[float] r:[float] b:[float]\" (where l is left, t is top, r is right, and b is bottom) using absolute metrics.",
		"l:0.000000 t:0.000000 r:0.000000 b:0.000000") 
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access window Z-Order changing enabled setting.

	This property offers access to the setting that controls whether z-order changes are enabled for the window.

	\par Usage:
		- Name: ZOrderChangeEnabled
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window should respect requests to change z-order.
		- "False" to indicate the Window should not change it's z-order.
*/
class ZOrderChangeEnabled : public Property
{
public:
	ZOrderChangeEnabled() : Property(
		"ZOrderChangeEnabled",
		"Property to get/set the 'z-order changing enabled' setting for the Window.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to control whether the window will receive double/triple-click events.

    This property offers access to the setting that controls whether a window will receive double-click and
    triple-click events, or whether the window will receive multiple single mouse button down events instead.

    \par Usage:
        - Name: WantsMultiClickEvents
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the Window wants double-click and triple-click events.
        - "False" to indicate the Window wants multiple single mouse button down events.
*/
class WantsMultiClickEvents : public Property
{
public:
    WantsMultiClickEvents() : Property(
        "WantsMultiClickEvents",
        "Property to get/set whether the window will receive double-click and triple-click events.  Value is either \"True\" or \"False\".",
        "True")
    {}

    String	get(const PropertyReceiver* receiver) const;
    void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to control whether the window will receive autorepeat mouse button down events.

    This property offers access to the setting that controls whether a window will receive autorepeat
    mouse button down events.

    \par Usage:
        - Name: MouseButtonDownAutoRepeat
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the Window will receive autorepeat mouse button down events.
        - "False" to indicate the Window will not receive autorepeat mouse button down events.
*/
class MouseButtonDownAutoRepeat : public Property
{
public:
    MouseButtonDownAutoRepeat() : Property(
        "MouseButtonDownAutoRepeat",
        "Property to get/set whether the window will receive autorepeat mouse button down events.  Value is either \"True\" or \"False\".",
        "False")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access window autorepeat delay value.

    This property offers access to the value that controls the initial delay for autorepeat mouse button down events.

    \par Usage:
        - Name: AutoRepeatDelay
        - Format: "[float]".

    \par Where:
        - [float]   specifies the delay in seconds.
*/
class AutoRepeatDelay : public Property
{
public:
    AutoRepeatDelay() : Property(
        "AutoRepeatDelay",
        "Property to get/set the autorepeat delay.  Value is a floating point number indicating the delay required in seconds.",
        "0.3")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access window autorepeat rate value.

    This property offers access to the value that controls the generation rate for autorepeat mouse button down events.

    \par Usage:
        - Name: AutoRepeatRate
        - Format: "[float]".

    \par Where:
        - [float]   specifies the rate at which autorepeat events will be generated in seconds.
*/
class AutoRepeatRate : public Property
{
public:
    AutoRepeatRate() : Property(
        "AutoRepeatRate",
        "Property to get/set the autorepeat rate.  Value is a floating point number indicating the rate required in seconds.",
        "0.06")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};


} // End of  WindowProperties namespace section


} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindowProperties_h_
