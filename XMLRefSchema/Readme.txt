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
Font is the root element, it has some attributes and zero or more 'Mapping' elements.  Mapping elements are only of importance for static / bitmap fonts, any Mapping elements defined for a dynamic font should be ignored.

Font attributes
---------------
Name		- the name that will be used to identify the Font within the system. (required).
Filename	- filename for the font.  For Static fonts this is an Imageset.  For Dynamic fonts this is a .ttf file. (required).
Type		- Specifies the type of font.  Valid options are 'Static' or 'Dynamic' (required).
Size		- Specifies the point size for a dynamic font, ignored for Static fonts.  (optional, default=12).
FirstCodepoint	- Specifies the first codepoint to be available on the dynamic font.  (optional, default=32).
LastCodepoint	- Specifies the last codepoint to be available on the dynamic font.  (optional, default=127).
NativeHorzRes	- The horizontal screen resolution that the images were are intended to be displayed at (optional, default=640).
NativeVertRes	- The vertical screen resolution that the images were are intended to be displayed at (optional, default=480).
AutoScaled	- Boolean, states whether to scale imagery so it appears the same size at any resolution (optional, default=false).


Mapping Element
===============
Used for static fonts only, defines a mapping between a code-point and a Image on the associated Imageset.
A Mapping must be supplied for every codepoint that the font is to have available.  A Mapping has attributes bu no sub-elements.

Mapping attributes
------------------
Codepoint	- Unicode codepoint which should be mapped. (required).
Image		- Name of the image to map.  This image shold be defined for the Imageset specified in Filename for the Font.
HorzAdvance	- Number of pixels to advance the 'pen' position after rendering.  -1 indicates the values should be auto-calculated.  (optional, default=-1).



==============
+ GUI Scheme +
==============
A GUI Scheme specifies a collection of elements that should be loaded and initialised.
The root element must be GUIScheme.


GUIScheme Element
=================
Root element.  Has a name attribute, and a collection of sub-elements which can be Imageset, Font, and WindowSet elements.

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


Font Element
============
Specifies a Font to be loaded as part of the scheme.  Has attributes nut no sub-elements.
If a font with the requested name already exists, the file specified is not loaded.

Font attributes
-------------------
Name		- The name of the Font. (required).
Filename	- Filename of the Font file.  If the font created by this file does not = Name above, an exception is thrown. (required).


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

