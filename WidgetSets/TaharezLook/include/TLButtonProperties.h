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
#ifndef _CEGUITLButtonProperties_h_
#define _CEGUITLButtonProperties_h_

#include "CEGUIProperty.h"


// Start of CEGUI namespace section
namespace CEGUI
{

// Start of TLButtonProperties namespace section
/*!
\brief
   Namespace containing all classes that make up the properties interface for the TLButton class
*/
namespace TLButtonProperties
{

/*!
\brief
   Property to access the normal image of the button

   \par Usage:
      - Name: NormalImage
      - Format: "set:<imageset> image:<imagename>".

*/
class NormalImage : public Property
{
public:
   NormalImage() : Property((utf8*)"NormalImage", (utf8*)"Property to get/set the normal image for the TLButton widget.  Value should be \"set:[imageset name] image:[image name]\".") {}

   String   get(const void* receiver) const;
   void   set(void* receiver, const String& value);
};

/*!
\brief
   Property to access the pushed image of the button

   \par Usage:
      - Name: PushedImage
      - Format: "set:<imageset> image:<imagename>".

*/
class PushedImage : public Property
{
public:
   PushedImage() : Property((utf8*)"PushedImage", (utf8*)"Property to get/set the pushed image for the TLButton widget.  Value should be \"set:[imageset name] image:[image name]\".") {}

   String   get(const void* receiver) const;
   void   set(void* receiver, const String& value);
};

/*!
\brief
   Property to access the hover image of the button

   \par Usage:
      - Name: HoverImage
      - Format: "set:<imageset> image:<imagename>".

*/
class HoverImage : public Property
{
public:
   HoverImage() : Property((utf8*)"HoverImage", (utf8*)"Property to get/set the hover image for the TLButton widget.  Value should be \"set:[imageset name] image:[image name]\".") {}

   String   get(const void* receiver) const;
   void   set(void* receiver, const String& value);
};

/*!
\brief
   Property to access whether to use the standard images or the supplied custom ones

   \par Usage:
      - Name: UseStandardImagery
      - Format: "True|False".

*/
class UseStandardImagery : public Property
{
public:
   UseStandardImagery() : Property((utf8*)"UseStandardImagery", (utf8*)"Property to get/set whether to use the standard imagery for the TLButton widget.  Value should be \"set:[imageset name] image:[image name]\".") {}

   String   get(const void* receiver) const;
   void   set(void* receiver, const String& value);
};

/*!
\brief
   Property to access an offset that is used to shift the text in the x-direction.

   \par Usage:
      - Name: UseStandardImagery
      - Format: "<xoffset>".

   \par
      where <xoffset> is a float value specifying the offset relative to the button-size

*/
class TextXOffset : public Property
{
public:
   TextXOffset() : Property((utf8*)"TextXOffset", (utf8*)"Property to get/set a relative x offset for the button's text") {}

   String get(const void* receiver) const;
   void set(void* receiver, const String& value);
};

}

}
#endif
