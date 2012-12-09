/***********************************************************************
	filename: 	CEGUIWindowProperties.h
	created:	5/7/2004
	author:		Paul D Turner

	purpose:	Interface to available window base class properties
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
#ifndef _CEGUIWindowProperties_h_
#define _CEGUIWindowProperties_h_

#include "CEGUIProperty.h"
#include "CEGUIXMLSerializer.h"

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
		"1")
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
	bool	isDefault(const PropertyReceiver* receiver) const;
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
	bool	isDefault(const PropertyReceiver* receiver) const;
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
	bool	isDefault(const PropertyReceiver* receiver) const;
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
	bool	isDefault(const PropertyReceiver* receiver) const;
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

/*!
\brief
	Property to access whether inputs are passed to child windows when
    input is captured to this window.

	\par Usage:
		- Name: DistributeCapturedInputs
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate 'captured' inputs should be passed to attached child windows.
		- "False" to indicate 'captured' inputs should be passed to this window only.
*/
class DistributeCapturedInputs : public Property
{
public:
	DistributeCapturedInputs() : Property(
		"DistributeCapturedInputs",
		"Property to get/set whether captured inputs are passed to child windows.  Value is either \"True\" or \"False\".",
		"False")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the custom tooltip for this Window.

    \par Usage:
        - Name: CustomTooltipType
        - Format: "[text]".

    \par Where:
        - [Text] is the typename of the custom tooltip for the Window.
 */
class CustomTooltipType : public Property
{
public:
    CustomTooltipType() : Property(
    "CustomTooltipType",
    "Property to get/set the custom tooltip for the window.  Value is the type name of the custom tooltip.",
    "")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the tooltip text for this Window.

    \par Usage:
        - Name: Tooltip
        - Format: "[text]".

    \par Where:
        - [Text] is the tooltip text for this window.
 */
class Tooltip : public Property
{
public:
    Tooltip() : Property(
    "Tooltip",
    "Property to get/set the tooltip text for the window.  Value is the tooltip text for the window.",
    "")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access whether the window inherits its tooltip text from its
    parent when it has no tooltip text of its own.  Default state: True

    \par Usage:
        - Name: InheritsTooltipText
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the Window inherits its tooltip text from its
          parent.
        - "False" to indicate the Window does not inherit its tooltip text.
*/
class InheritsTooltipText : public Property
{
public:
    InheritsTooltipText() : Property(
        "InheritsTooltipText",
        "Property to get/set whether the window inherits its parents tooltip text when it has none of its own.  Value is either \"True\" or \"False\".",
        "True")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access whether the window rises to the top of the z order when clicked.

	\par Usage:
		- Name: RiseOnClick
		- Format: "[text]".

	\par Where [Text] is:
		- "True" to indicate the Window will rise to the surface when clicked.
		- "False" to indicate the Window will not change z position when clicked.
*/
class RiseOnClick : public Property
{
public:
    RiseOnClick() : Property(
		"RiseOnClick",
		"Property to get/set whether the window will come tot he top of the z order hwn clicked.  Value is either \"True\" or \"False\".",
		"True")
	{}

	String	get(const PropertyReceiver* receiver) const;
	void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access the vertical alignment setting for the window.

    \par Usage:
        - Name: VerticalAlignment
        - Format: "[text]".

    \par Where [Text] is:
        - "Top" to indicate the windows position is an offset of its top edge from its parents top edge.
        - "Centre" to indicate the windows position is an offset of its centre point from its parents centre point.
        - "Bottom" to indicate the windows position is an offset of its bottom edge from its parents bottom edge.
*/
class VerticalAlignment : public Property
{
    public:
        VerticalAlignment() : Property(
        "VerticalAlignment",
        "Property to get/set the windows vertical alignment.  Value is one of \"Top\", \"Centre\" or \"Bottom\".",
        "Top")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access the horizontal alignment setting for the window.

    \par Usage:
        - Name: HorizontalAlignment
        - Format: "[text]".

    \par Where [Text] is:
        - "Left" to indicate the windows position is an offset of its left edge from its parents left edge.
        - "Centre" to indicate the windows position is an offset of its centre point from its parents centre point.
        - "Right" to indicate the windows position is an offset of its right edge from its parents right edge.
*/
class HorizontalAlignment : public Property
{
    public:
        HorizontalAlignment() : Property(
        "HorizontalAlignment",
        "Property to get/set the windows horizontal alignment.  Value is one of \"Left\", \"Centre\" or \"Right\".",
        "Left")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified area rectangle of the window.

	\par Usage:
		- Name: UnifiedAreaRect
		- Format: "{{[ls],[lo]},{[ts],[to]},{[rs],[ro]},{[bs],[bo]}}"

	\par Where:
		- [ls] is a floating point value describing the relative scale value for the left edge.
		- [lo] is a floating point value describing the absolute offset value for the left edge.
		- [ts] is a floating point value describing the relative scale value for the top edge.
		- [to] is a floating point value describing the absolute offset value for the top edge.
		- [rs] is a floating point value describing the relative scale value for the right edge.
		- [ro] is a floating point value describing the absolute offset value for the right edge.
		- [bs] is a floating point value describing the relative scale value for the bottom edge.
		- [bo] is a floating point value describing the absolute offset value for the bottom edge.
*/
class UnifiedAreaRect : public Property
{
	public:
		UnifiedAreaRect() : Property(
		"UnifiedAreaRect",
		"Property to get/set the windows unified area rectangle.  Value is a \"URect\".",
		"{{0,0},{0,0},{0,0},{0,0}}")
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified position of the window.

	\par Usage:
		- Name: UnifiedPosition
		- Format: "{{[xs],[xo]},{[ys],[yo]}}"

	\par Where:
		- [xs] is a floating point value describing the relative scale value for the position x-coordinate.
		- [xo] is a floating point value describing the absolute offset value for the position x-coordinate.
		- [ys] is a floating point value describing the relative scale value for the position y-coordinate.
		- [yo] is a floating point value describing the absolute offset value for the position y-coordinate.
*/
class UnifiedPosition : public Property
{
	public:
		UnifiedPosition() : Property(
		"UnifiedPosition",
		"Property to get/set the windows unified position.  Value is a \"UVector2\".",
		"{{0,0},{0,0}}", false)
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified position x-coordinate of the window.

	\par Usage:
		- Name: UnifiedXPosition
		- Format: "{[s],[o]}"

	\par Where:
		- [s] is a floating point value describing the relative scale value for the position x-coordinate.
		- [o] is a floating point value describing the absolute offset value for the position x-coordinate.
*/
class UnifiedXPosition : public Property
{
	public:
		UnifiedXPosition() : Property(
		"UnifiedXPosition",
		"Property to get/set the windows unified position x-coordinate.  Value is a \"UDim\".",
		"{0,0}", false)
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified position y-coordinate of the window.

	\par Usage:
		- Name: UnifiedYPosition
		- Format: "{[s],[o]}"

	\par Where:
		- [s] is a floating point value describing the relative scale value for the position y-coordinate.
		- [o] is a floating point value describing the absolute offset value for the position y-coordinate.
*/
class UnifiedYPosition : public Property
{
	public:
		UnifiedYPosition() : Property(
		"UnifiedYPosition",
		"Property to get/set the windows unified position y-coordinate.  Value is a \"UDim\".",
		"{0,0}", false)
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified position of the window.

	\par Usage:
		- Name: UnifiedSize
		- Format: "{{[ws],[wo]},{[hs],[ho]}}"

	\par Where:
		- [ws] is a floating point value describing the relative scale value for the width.
		- [wo] is a floating point value describing the absolute offset value for the width.
		- [hs] is a floating point value describing the relative scale value for the height.
		- [ho] is a floating point value describing the absolute offset value for the height.
*/
class UnifiedSize : public Property
{
	public:
		UnifiedSize() : Property(
		"UnifiedSize",
		"Property to get/set the windows unified size.  Value is a \"UVector2\".",
		"{{0,0},{0,0}}", false)
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified width of the window.

	\par Usage:
		- Name: UnifiedWidth
		- Format: "{[s],[o]}"

	\par Where:
		- [s] is a floating point value describing the relative scale value for the width.
		- [o] is a floating point value describing the absolute offset value for the width.
*/
class UnifiedWidth : public Property
{
	public:
		UnifiedWidth() : Property(
		"UnifiedWidth",
		"Property to get/set the windows unified width.  Value is a \"UDim\".",
		"{0,0}", false)
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified height of the window.

	\par Usage:
		- Name: UnifiedHeight
		- Format: "{[s],[o]}"

	\par Where:
		- [s] is a floating point value describing the relative scale value for the height.
		- [o] is a floating point value describing the absolute offset value for the height.
*/
class UnifiedHeight : public Property
{
	public:
		UnifiedHeight() : Property(
		"UnifiedHeight",
		"Property to get/set the windows unified height.  Value is a \"UDim\".",
		"{0,0}", false)
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified minimum size of the window.

	\par Usage:
		- Name: UnifiedMinSize
		- Format: "{{[ws],[wo]},{[hs],[ho]}}"

	\par Where:
		- [ws] is a floating point value describing the relative scale value for the minimum width.
		- [wo] is a floating point value describing the absolute offset value for the minimum width.
		- [hs] is a floating point value describing the relative scale value for the minimum height.
		- [ho] is a floating point value describing the absolute offset value for the minimum height.
*/
class UnifiedMinSize : public Property
{
	public:
		UnifiedMinSize() : Property(
		"UnifiedMinSize",
		"Property to get/set the windows unified minimum size.  Value is a \"UVector2\".",
		"{{0,0},{0,0}}")
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
	Property to access the unified maximum size of the window.

	\par Usage:
		- Name: UnifiedMaxSize
		- Format: "{{[ws],[wo]},{[hs],[ho]}}"

	\par Where:
		- [ws] is a floating point value describing the relative scale value for the maximum width.
		- [wo] is a floating point value describing the absolute offset value for the maximum width.
		- [hs] is a floating point value describing the relative scale value for the maximum height.
		- [ho] is a floating point value describing the absolute offset value for the maximum height.
*/
class UnifiedMaxSize : public Property
{
	public:
		UnifiedMaxSize() : Property(
		"UnifiedMaxSize",
		"Property to get/set the windows unified maximum size.  Value is a \"UVector2\".",
		"{{1,0},{1,0}}")
		{}

		String	get(const PropertyReceiver* receiver) const;
		void	set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access whether the window ignores mouse events and pass them through to any windows behind it.

    \par Usage:
        - Name: MousePassThroughEnabled
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the Window will not respond to mouse events but pass them directly to any children behind it.
        - "False" to indicate the Window will respond to normally to all mouse events (Default).
*/
class MousePassThroughEnabled : public Property
{
    public:
        MousePassThroughEnabled() : Property(
        "MousePassThroughEnabled",
        "Property to get/set whether the window ignores mouse events and pass them through to any windows behind it. Value is either \"True\" or \"False\".",
        "False")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
};


/*!
\brief
    Property to access/change the assigned window renderer object.

    \par Usage:
        - Name: WindowRenderer
        - Format: "[windowRendererName]"

    \par Where [windowRendererName] is the factory name of the window renderer type you wish to assign.
*/
class WindowRenderer : public Property
{
    public:
        WindowRenderer() : Property(
        "WindowRenderer",
        "Property to get/set the windows assigned window renderer objects name.  Value is a string.",
        "")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);

        void writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const;
};


/*!
\brief
    Property to access/change the assigned look'n'feel.

    \par Usage:
        - Name: LookNFeel
        - Format: "[LookNFeelName]"

    \par Where [LookNFeelName] is the name of the look'n'feel you wish to assign.
*/
class LookNFeel : public Property
{
    public:
        LookNFeel() : Property(
        "LookNFeel",
        "Property to get/set the windows assigned look'n'feel.  Value is a string.",
        "")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);

        void writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const;
};

/*!
\brief
    Property to get/set whether the Window will receive drag and drop related
    notifications.

    \par Usage:
        - Name: DragDropTarget
        - Format: "[text]".

    \par Where [Text] is:
        - "True" if Window is will receive drag & drop notifications.
        - "False" if Window is will not receive drag & drop notifications.
*/
class DragDropTarget : public Property
{
public:
    DragDropTarget() : Property(
        "DragDropTarget",
        "Property to get/set whether the Window will receive drag and drop related notifications.  Value is either \"True\" or \"False\".",
        "True")
    {}

    String get(const PropertyReceiver* receiver) const;
    void set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to get/set whether the Window will automatically attempt to use a
    full imagery caching RenderingSurface (if supported by the renderer).  Here,
    "full imagery caching" usually will mean caching a window's representation
    onto a texture (although no such implementation requirement is specified.)

    \par Usage:
        - Name: AutoRenderingSurface
        - Format: "[text]".

    \par Where [Text] is:
        - "True" if Window should automatically use a full imagery caching
          RenderingSurface (aka a RenderingWindow).
        - "False" if Window should not automatically use a full imagery caching
          RenderingSurface.
*/
class AutoRenderingSurface : public Property
{
public:
    AutoRenderingSurface() : Property(
        "AutoRenderingSurface",
        "Property to get/set whether the Window will automatically attempt to "
        "use a full imagery caching RenderingSurface (if supported by the "
        "renderer).  Here, full imagery caching usually will mean caching a "
        "window's representation onto a texture (although no such "
        "implementation requirement is specified.)"
        "  Value is either \"True\" or \"False\".",
        "False")
    {}

    String get(const PropertyReceiver* receiver) const;
    void set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the rotation factors of the window.

    \par Usage:
        - Name: Rotation
        - Format: "x:[x_float] y:[y_float] z:[z_float]"

    \par Where:
        - [x_float] is a floating point value describing the rotation around the
                    x axis, in degrees.
        - [y_float] is a floating point value describing the rotation around the
                    y axis, in degrees.
        - [z_float] is a floating point value describing the rotation around the
                    z axis, in degrees.
*/
class Rotation : public Property
{
    public:
        Rotation() : Property(
        "Rotation",
        "Property to get/set the windows rotation factors.  Value is "
        "\"x:[x_float] y:[y_float] z:[z_float]\".",
        "x:0 y:0 z:0")
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the x axis rotation factor of the window.

    \par Usage:
        - Name: XRotation
        - Format: "[float]"

    \par Where:
        - [float] is a floating point value describing the rotation around the
                  x axis, in degrees.
*/
class XRotation : public Property
{
    public:
        XRotation() : Property(
        "XRotation",
        "Property to get/set the window's x axis rotation factor.  Value is "
        "\"[float]\".",
        "0", false)
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the y axis rotation factor of the window.

    \par Usage:
        - Name: YRotation
        - Format: "[float]"

    \par Where:
        - [float] is a floating point value describing the rotation around the
                  y axis, in degrees.
*/
class YRotation : public Property
{
    public:
        YRotation() : Property(
        "YRotation",
        "Property to get/set the window's y axis rotation factor.  Value is "
        "\"[float]\".",
        "0", false)
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the z axis rotation factor of the window.

    \par Usage:
        - Name: ZRotation
        - Format: "[float]"

    \par Where:
        - [float] is a floating point value describing the rotation around the
                  z axis, in degrees.
*/
class ZRotation : public Property
{
    public:
        ZRotation() : Property(
        "ZRotation",
        "Property to get/set the window's z axis rotation factor.  Value is "
        "\"[float]\".",
        "0", false)
        {}

        String  get(const PropertyReceiver* receiver) const;
        void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access window non-client setting.

    This property offers access to the "non client" setting for the window.

    \par Usage:
        - Name: NonClient
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the Window is a non-client window.
        - "False" to indicate the Window is not a non-client.
*/
class NonClient : public Property
{
public:
    NonClient() : Property(
        "NonClient",
        "Property to get/set the 'non-client' setting for the Window.  "
        "Value is either \"True\" or \"False\".",
        "False")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access window text parsing enabled setting.

    This property offers access to the text parsing setting for the window that
    specifies whether parsing will be done or whether text will be rendered
    verbatim.

    \par Usage:
        - Name: TextParsingEnabled
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate some form of parsing of window text is to occur.
        - "False" to indicate that text should not be parsed, but be rendered
          verbatim.
*/
class TextParsingEnabled : public Property
{
public:
    TextParsingEnabled() : Property(
        "TextParsingEnabled",
        "Property to get/set the text parsing setting for the Window.  "
        "Value is either \"True\" or \"False\".",
        "True")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access window margin.

    This property offers access to the margin property. This property controls
	the margins of the window when it's inserted into a layout container.
	When the window isn't in a layout container, margin doesn't have any effect.

    \par Usage:
        - Name: Margin
        - Format: "{top:{[tops],[topo]},left:{[lefts],[lefto]},bottom:{[bottoms],[bottomo]},right:{[rights],[righto]}}".

    \par Where [Text] is:
		- [tops] is top scale
        - [topo] is top offset
		- [lefts] is left scale
        - [lefto] is left offset
		- [bottoms] is bottom scale
        - [bottomo] is bottom offset
		- [rights] is right scale
        - [righto] is right offset
*/
class Margin : public Property
{
public:
    Margin() : Property(
        "Margin",
		"Property to get/set margin for the Window. Value format:"
        "{top:{[tops],[topo]},left:{[lefts],[lefto]},bottom:{[bottoms],[bottomo]},right:{[rights],[righto]}}.",
        "{top:{0,0},left:{0,0},bottom:{0,0},right:{0,0}}")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the update mode setting for the window.

    \par Usage:
        - Name: UpdateMode
        - Format: "[text]".

    \par Where [Text] is:
        - "Always" to indicate the update function should always be called.
        - "Never" to indicate the update function should never be called.
        - "Visible" to indicate the update function should only be called when
          the window is visible (i.e. State of Visible property set to True).
*/
class UpdateMode : public Property
{
    public:
        UpdateMode() : Property(
        "UpdateMode",
        "Property to get/set the window update mode setting.  "
        "Value is one of \"Always\", \"Never\" or \"Visible\".",
        "Visible")
        {}

        String get(const PropertyReceiver* receiver) const;
        void set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the setting that controls whether mouse input not handled
    directly by the window will be propagated back to the parent window.

    \par Usage:
        - Name: MouseInputPropagationEnabled
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate that unhandled mouse input should be propagated to
          the Window's parent.
        - "False" to indicate that unhandled mouse input should not be
          propagated to the window's parent.
*/
class MouseInputPropagationEnabled : public Property
{
public:
    MouseInputPropagationEnabled() : Property(
        "MouseInputPropagationEnabled",
        "Property to get/set whether unhandled mouse inputs should be "
        "propagated back to the Window's parent.  "
        "Value is either \"True\" or \"False\".",
        "False")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

} // End of  WindowProperties namespace section


} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIWindowProperties_h_
