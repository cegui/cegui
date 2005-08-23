Brief guide to the schemas currently defined for the CEGUI data files.

=============
+ Imagesets +
=============
Imagesets define one or more images on a larger image (texture).
The root element must be Imageset.


Imageset Element
================
Has attributes and a collection of one or more Image elements.

Imageset attributes
-------------------
Imagefile	- path to the image file containing the graphics (required).
ResourceGroup	- The resource group identifier to pass to the resource provider when loading the file.
Name		- the name that will be assigned to the Imageset in the GUI system (required).
NativeHorzRes	- The horizontal screen resolution that the images were are intended to be displayed at (optional, default=640).
NativeVertRes	- The vertical screen resolution that the images were are intended to be displayed at (optional, default=480).
AutoScaled	- Boolean, states whether to scale imagery so it appears the same size as any resolution (optional, default=false).


Image Element
=============
Has attributes defining an sub-image area.  Can have no sub-elements.

Image attributes
----------------
Name		- The name that will be used to identify the image within the Imageset.  (required).
XPos		- X pixel co-ordinate of the top-left corner of the image on the texture. (required).
YPos		- Y pixel co-ordinate of the top-left corner of the image on the texture. (required).
Width		- Width of this image in pixels. (required).
Height		- Height of this image in pixels. (required).
XOffset		- Horizontal offset to apply when rendering.  (optional, default=0)
YOffset		- Vertical offset to apply when rendering.  (optional, default=0)



=========
+ Fonts +
=========
Font files can define one of two types of font; a static bitmapped font based on an Imageset stored on disc, or a dynamically created font made from a true-type font file (.ttf).

The Root element must be Font.


Font Element
============
Font is the root element, it has some attributes and optional 'Mapping', 'GlyphSet', 'GlyphRange', and 'Glyph' elements.
Mapping elements are only of importance for static / bitmap fonts, any Mapping elements defined for a dynamic font should be ignored.
GlyphSet, GlyphRange, and Glyph elements are only valid for dynamic fonts.

Font attributes
---------------
Name		- the name that will be used to identify the Font within the system. (required).
Filename	- filename for the font.  For Static fonts this is an Imageset.  For Dynamic fonts this is a .ttf file. (required).
ResourceGroup - The resource group identifier to pass to the resource provider when loading the file.
Type		- Specifies the type of font.  Valid options are 'Static' or 'Dynamic' (required).
Size		- Specifies the point size for a dynamic font, ignored for Static fonts.  (optional, default=12).
FirstCodepoint	- Specifies the first codepoint to be available on the dynamic font.  (optional, default=32).
LastCodepoint	- Specifies the last codepoint to be available on the dynamic font.  (optional, default=127).
NativeHorzRes	- The horizontal screen resolution that the images were are intended to be displayed at (optional, default=640).
NativeVertRes	- The vertical screen resolution that the images were are intended to be displayed at (optional, default=480).
AutoScaled	- Boolean, states whether to scale imagery so it appears the same size at any resolution (optional, default=false).
AntiAlias	- Boolean, states whether the font should be anti-aliased, applies to dynamic fonts only.  (optional, default=true).


Mapping Element
===============
Used for static fonts only, defines a mapping between a code-point and a Image on the associated Imageset.
A Mapping must be supplied for every codepoint that the font is to have available.  A Mapping has attributes but no sub-elements.

Mapping attributes
------------------
Codepoint	- Unicode codepoint which should be mapped. (required).
Image		- Name of the image to map.  This image shold be defined for the Imageset specified in Filename for the Font.
HorzAdvance	- Number of pixels to advance the 'pen' position after rendering.  -1 indicates the values should be auto-calculated.  (optional, default=-1).


GlyphSet Element
================
Used for dynamic fonts only.  Defines a set of codepoints for which glyphs should be made available in the font.
Multiple GlyphSet elements may be specified.  A GlyphSet element has attributes but no sub-elements.

GlyphSet attributes
-------------------
Glyphs		- String defining the character / codepoint glyphs to be available.


GlyphRange Element
==================
Used for dynamic fonts only.  Defines a range of codepoints for which glyphs should be made available in the font.
Multiple GlyphRange elements may be specified.  A GlyphRange element has attributes but no sub-elements.

GlyphRange attributes
---------------------
StartCodepoint	- U+ codepoint of the first glyph that is part of this range.
EndCodepoint	- U+ codepoint of the last glyph that is part of this range.


Glyph Element
=============
Used for dynamic fonts only.  Defines a singal codepoint for which a glyph should be made available in the font.
Multiple Glyph elements may be specified.  A Glyph element has attributes but no sub-elements.

Glyph attributes
----------------
Codepoint	- U+ codepoint of the glyph to be added to the font.



==============
+ GUI Scheme +
==============
A GUI Scheme specifies a collection of elements that should be loaded and initialised.
The root element must be GUIScheme.


GUIScheme Element
=================
Root element.  Has a name attribute, and a collection of sub-elements which can be Imageset, Font, WindowSet, and WindowAlias elements.

GUIScheme attributes
--------------------
Name		- Specifies the name that the scheme will use within the system. (required).


Imageset Element
================
Specifies an Imageset to be loaded as part of this scheme.  Has attributes but no sub-elements.
If an imagesetwith the requested name already exists, the file specified is not loaded.

Imageset attributes
-------------------
Name		- The name of the Imageset. (required).
Filename	- Filename of the Imageset file.  If the imageset created by this file does not = Name above, an exception is thrown. (required).
ResourceGroup - The resource group identifier to pass to the resource provider when loading the file.


ImagesetFromImage Element
=========================
Specifies an Imageset to be created via an image file as part of this scheme.  Has attributes but no sub-elements.
If an imageset with the requested name already exists, the file specified is not loaded.

ImagesetFromImage attributes
-------------------
Name		- The name of the Imageset. (required).
Filename	- Filename of the image file to load in order to create this Imageset. (required).
ResourceGroup - The resource group identifier to pass to the resource provider when loading the image file.


Font Element
============
Specifies a Font to be loaded as part of the scheme.  Has attributes nut no sub-elements.
If a font with the requested name already exists, the file specified is not loaded.

Font attributes
-------------------
Name		- The name of the Font. (required).
Filename	- Filename of the Font file.  If the font created by this file does not = Name above, an exception is thrown. (required).
ResourceGroup - The resource group identifier to pass to the resource provider when loading the file.


WindowSet Element
=================
Specifies a module containing concrete GUI elements and their factories.  Has attribues and one or more WindowFactory sub-elements.

WindowSet attributes
--------------------
Filename	- Specifies the name of the loadable module (dll / .so / etc).  (required).


WindowFactory Element
=====================
Specifies the factory name (GUI window type name) from the loadable module that is to be added to the list of available factories.  Has attributes but no sub-elements.

WindowFactory attributes
------------------------
Name		- Name of the factory / window type which is to be added.


WindowAlias Element
===================
Specifies an alias for a given window factory type.  Has attributes but no sub-elements.

WindowAlias attributes
----------------------
Alias		- Name of the alias.  This is the alternative name that 'Target' will be known by.
Target		- Name of the window factory type or existing alias that is to also be known as 'Alias'.



==============
+ GUI Layout +
==============
A GUI Layout defines a hierachy of Window based objects to be created and a property settings for each window.
The root element is GUILayout.


GUILayout Element
=================
GUILayout is the root element.
The GUILayout must contain a single Window element only.

GUILayout attributes
--------------------
Parent		- Specifies the name of an existing window that this gui layout should be attached to (optional).


Window Element
==============
The Window element is used to specify a new window object to be created.
The Window element has attributes as described below and may contain any number of nested Window elements, any number of Property elements,
any number of LayoutImport elements, and any number of Event elements.

Window attributes
-----------------
Type		- Specifies the type of Window object to be created (required).
Name		- Specifies the unique name for the Window.  If this is ommitted a name will be generated.  If a window with the name already exists, an exception will be thrown. (optional).


Property Element
================
The Property element is used to set properties on the Window created by the containing Window element.
Property has no sub-elements, but has attributes as described below.

Property attributes
-------------------
Name		- The name of the property to be set.  If no such property exists for the target window, an exception may be thrown.  (required).
Value		- The value to be assigned to the property.  This must be in a format expected by the property or an exception may be thrown.  (required).


LayoutImport Element
====================
The LayoutImport element is used to reference (import) a layout file into another.  The root window of the imported layout is attached to the Window where the import occurrs.
The LayoutImport element has attributes but no sub-elements.

LayoutImport attributes
-----------------------
Filename	- Specifies the filename of the layout XML file to be imported.
ResourceGroup - The resource group identifier to pass to the resource provider when loading the file.


Event Element
=============
The Event element is used to create bindings between Gui elements and script functions.
The Event element has attributes but no sub-elements.

Event attributes
----------------
Name		- Specifies the name of the target event.
Function	- Specifies the name of the script function that is to be bound to the event.
