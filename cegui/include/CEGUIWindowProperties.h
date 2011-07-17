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
        "\"w:[w_float] x:[x_float] y:[y_float] z:[z_float] (Quaternion)\".",
		"w:1 x:0 y:0 z:0")
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
