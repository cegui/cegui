/***********************************************************************
    filename:   CEGUIFontProperties.h
    created:    12/6/2006
    author:     Andrew Zabolotny

    purpose:    Interface to available font properties
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifdef HAVE_CONFIG_H
#   include "config.h"
#endif

#include "CEGUIPixmapFont.h"
#include "CEGUIXMLSerializer.h" 
#include "CEGUIPropertyHelper.h"
#include "CEGUIExceptions.h"
#include "CEGUILogger.h"

#ifdef CEGUI_HAS_FREETYPE
#   include "CEGUIFreeTypeFont.h"
#endif

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
    NativeRes() : Property(
        "NativeRes",
        "Native screen resolution for this font. Value uses the 'w:# h:#' "
        "format.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<Size<> >::toString(
            static_cast<const Font*>(receiver)->getNativeResolution());
    }

    void set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Font*>(receiver)->
            setNativeResolution(PropertyHelper<Size<> >::fromString(value));
    }
};

class Name : public Property
{
public:
    Name() : Property(
        "Name",
        "This is font name.  Value is a string.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return static_cast<const Font*>(receiver)->getName();
    }

    void set(PropertyReceiver* receiver, const String& /*value*/)
    {
        // Font can not be renamed
        Logger::getSingleton().logEvent("FontProperties::Name::set: "
            "Attempt to set read-only propery 'Name' on Font '" +
            static_cast<const Font*>(receiver)->getName() + "'- ignoring.",
            Errors);
    }
};

class AutoScaled : public Property
{
public:
    AutoScaled() : Property(
        "AutoScaled",
        "This is a flag indicating whether to autoscale font depending on "
        "resolution.  Value is either true or false.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(
            static_cast<const Font*>(receiver)->isAutoScaled());
    }

    void set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<Font*>(receiver)->setAutoScaled(
            PropertyHelper<bool>::fromString(value));
    }
};

#ifdef CEGUI_HAS_FREETYPE
// FreeTypeFont property
class FreeTypePointSize : public Property
{
public:
    FreeTypePointSize() : Property(
        "PointSize",
        "This is the point size of the font.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<float>::toString(
            static_cast<const FreeTypeFont*>(receiver)->getPointSize());
    }

    void set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<FreeTypeFont*>(receiver)->
            setPointSize(PropertyHelper<float>::fromString(value));
    }
};

// FreeTypeFont property
class FreeTypeAntialiased : public Property
{
public:
    FreeTypeAntialiased() : Property(
        "Antialiased",
        "This is a flag indicating whenever to render antialiased font or not. "
        "Value is either true or false.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return PropertyHelper<bool>::toString(
            static_cast<const FreeTypeFont*>(receiver)->isAntiAliased());
    }

    void set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<FreeTypeFont*>(receiver)->
            setAntiAliased(PropertyHelper<bool>::fromString(value));
}
};
#endif // CEGUI_HAS_FREETYPE

// PixmapFont property
class PixmapImageNamePrefix : public Property
{
public:
    PixmapImageNamePrefix() : Property(
        "ImageNamePrefix",
        "This is the name prefix used by the images that contain the glyph "
        "imagery for this font.")
    {}

    String get(const PropertyReceiver* receiver) const
    {
        return static_cast<const PixmapFont*>(receiver)->getImageNamePrefix();
    }

    void set(PropertyReceiver* receiver, const String& value)
    {
        static_cast<PixmapFont*>(receiver)->setImageNamePrefix(value);
    }
};

// PixmapFont property
class PixmapMapping : public Property
{
public:
    PixmapMapping () : Property(
        "Mapping",
        "This is the glyph-to-image mapping font property. It cannot be read. "
        "Format is: codepoint,advance,imagename")
    {}

    String get (const PropertyReceiver*) const
    {
        return String();
    }

    void set (PropertyReceiver* receiver, const String& value)
    {
        char img[33];
        String::value_type codepoint;
        float adv;
        if (sscanf (value.c_str(), " %u , %g , %32s", &codepoint, &adv, img) != 3)
            CEGUI_THROW(InvalidRequestException(
                "Bad glyph Mapping specified: " + value));
        static_cast<PixmapFont*>(receiver)->defineMapping(codepoint, img, adv);
    }
};

} // End of FontProperties namespace section

void Font::addFontProperties ()
{
    static FontProperties::Name Name;
    static FontProperties::NativeRes NativeRes;
    static FontProperties::AutoScaled AutoScaled;

    addProperty(&Name);
    addProperty(&NativeRes);
    addProperty(&AutoScaled);
}

#ifdef CEGUI_HAS_FREETYPE
void FreeTypeFont::addFreeTypeFontProperties ()
{
    static FontProperties::FreeTypePointSize FreeTypePointSize;
    static FontProperties::FreeTypeAntialiased FreeTypeAntialiased;

    addProperty(&FreeTypePointSize);
    addProperty(&FreeTypeAntialiased);
}
#endif

void PixmapFont::addPixmapFontProperties ()
{
    static FontProperties::PixmapImageNamePrefix PixmapImageNamePrefix;
    static FontProperties::PixmapMapping PixmapMapping;

    addProperty(&PixmapImageNamePrefix);
    addProperty(&PixmapMapping);
}

} // End of CEGUI namespace section
