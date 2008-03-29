/***********************************************************************
    filename:   CEGUIFontProperties.h
    created:    12/6/2006
    author:     Andrew Zabolotny

    purpose:    Interface to available font properties
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
#include "CEGUIFont.h"
#include "CEGUIFreeTypeFont.h"
#include "CEGUIPixmapFont.h"
#include "CEGUIImageset.h"
#include "CEGUIXMLSerializer.h" 
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{

// Start of WindowProperties namespace section
/*!
\brief
    Namespace containing all classes that make up the properties interface
    for the Font base class
*/
namespace FontProperties
{

class NativeRes : public Property
{
public:
    NativeRes () : Property(
        "NativeRes",
        "Native screen resolution for this font. Value uses the 'w:# h:#' format.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        const Font *f = static_cast<const Font*>(receiver);
        Size s (f->d_nativeHorzRes, f->d_nativeVertRes);
        return PropertyHelper::sizeToString (s);
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        Font *f = static_cast<Font*>(receiver);
        f->setNativeResolution (PropertyHelper::stringToSize (value));
    }
};

class Name : public Property
{
public:
    Name () : Property(
        "Name",
        "This is font name.  Value is a string.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        return static_cast<const Font*>(receiver)->d_name;
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        static_cast<Font*>(receiver)->d_name = value;
    }
};

class FileName : public Property
{
public:
    FileName () : Property(
        "FileName",
        "This is the filename from which the font is loaded.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        return static_cast<const Font*>(receiver)->d_fileName;
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        static_cast<Font*>(receiver)->d_fileName = value;
    }
};

class ResourceGroup : public Property
{
public:
    ResourceGroup () : Property(
        "ResourceGroup",
        "This is the resource group for font file name.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        return static_cast<const Font*>(receiver)->d_resourceGroup;
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        static_cast<Font*>(receiver)->d_resourceGroup = value;
    }
};

class AutoScaled : public Property
{
public:
    AutoScaled () : Property(
        "AutoScaled",
        "This is a flag indicating whether to autoscale font depending on resolution.  Value is either true or false.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        return PropertyHelper::boolToString(static_cast<const Font*>(receiver)->d_autoScale);
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        Font *f = static_cast<Font*>(receiver);
        bool v = PropertyHelper::stringToBool (value);
        if (f->d_autoScale != v)
        {
            f->d_autoScale = v;
            f->updateFont ();
        }
    }
};

// FreeTypeFont property
class FreeTypePointSize : public Property
{
public:
    FreeTypePointSize () : Property(
        "PointSize",
        "This is the point size of the font.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper::floatToString(static_cast<const FreeTypeFont*>(receiver)->d_ptSize);
    }

    void set(PropertyReceiver* receiver, const String& value)
    {
        FreeTypeFont *f = static_cast<FreeTypeFont*>(receiver);
        float v = PropertyHelper::stringToFloat (value);
        if (v != f->d_ptSize)
        {
            f->d_ptSize = v;
            f->updateFont ();
        }
    }
};

// FreeTypeFont property
class FreeTypeAntialiased : public Property
{
public:
    FreeTypeAntialiased () : Property(
        "Antialiased",
        "This is a flag indicating whenever to render antialiased font or not.  Value is either true or false.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        return PropertyHelper::boolToString(static_cast<const FreeTypeFont*>(receiver)->d_antiAliased);
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        FreeTypeFont *f = static_cast<FreeTypeFont*>(receiver);
        bool v = PropertyHelper::stringToBool (value);
        if (f->d_antiAliased != v)
        {
            f->d_antiAliased = v;
            f->updateFont ();
        }
    }
};

// PixmapFont property
class PixmapImageset : public Property
{
public:
    PixmapImageset () : Property(
        "Imageset",
        "This is the name of the imageset which contains the glyph images for this font.")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        const PixmapFont *f = static_cast<const PixmapFont*>(receiver);
        return f->d_glyphImages ? f->d_glyphImages->getName () : String ();
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        PixmapFont *f = static_cast<PixmapFont*>(receiver);
        f->d_resourceGroup = "*";
        f->d_fileName = value;
        f->reinit ();
    }
};

// PixmapFont property
class PixmapMapping : public Property
{
public:
    PixmapMapping () : Property(
        "Mapping",
        "This is the glyph-to-image mapping font property. It cannot be read. Format is: codepoint,advance,imagename")
    {}

    String get (const PropertyReceiver* receiver) const
    {
        return String ();
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        char img [33];
        utf32 codepoint;
        float adv;
        if (sscanf (value.c_str(), " %u , %g , %32s", &codepoint, &adv, img) != 3)
            throw InvalidRequestException ("Bad glyph Mapping specified: " + value);
        static_cast<PixmapFont*>(receiver)->defineMapping (img, codepoint, adv);
    }
};

} // End of FontProperties namespace section

void Font::addFontProperties ()
{
    static FontProperties::NativeRes NativeRes;
    static FontProperties::Name Name;
    static FontProperties::FileName FileName;
    static FontProperties::ResourceGroup ResourceGroup;
    static FontProperties::AutoScaled AutoScaled;

    addProperty (&NativeRes);
    addProperty (&Name);
    addProperty (&FileName);
    addProperty (&ResourceGroup);
    addProperty (&AutoScaled);
}

void FreeTypeFont::addFreeTypeFontProperties ()
{
    static FontProperties::FreeTypePointSize FreeTypePointSize;
    static FontProperties::FreeTypeAntialiased FreeTypeAntialiased;

    addProperty (&FreeTypePointSize);
    addProperty (&FreeTypeAntialiased);
}

void PixmapFont::addPixmapFontProperties ()
{
    static FontProperties::PixmapImageset PixmapImageset;
    static FontProperties::PixmapMapping PixmapMapping;

    addProperty (&PixmapImageset);
    addProperty (&PixmapMapping);
}

} // End of CEGUI namespace section
