/***********************************************************************
    filename:   CEGUIPixmapFont.cpp
    created:    14/6/2006
    author:     Andrew Zabolotny
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
#include "CEGUIPixmapFont.h"
#include "CEGUIImageset.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIFont_xmlHandler.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUILogger.h"

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
static const String BuiltInResourceGroup ("*");

//----------------------------------------------------------------------------//
PixmapFont::PixmapFont(const String& font_name, const String& imageset_filename,
                       const String& resource_group, const bool auto_scaled,
                       const float native_horz_res,
                       const float native_vert_res) :
    Font(font_name, Font_xmlHandler::FontTypePixmap, imageset_filename,
         resource_group, auto_scaled, native_horz_res, native_vert_res),
    d_glyphImages(0),
    d_origHorzScaling(1.0f),
    d_imagesetOwner(false)
{
    addPixmapFontProperties();

    reinit();
    updateFont();
}

//----------------------------------------------------------------------------//
PixmapFont::~PixmapFont ()
{
    if (d_imagesetOwner)
        ImagesetManager::getSingleton ().destroy(*d_glyphImages);
}

//----------------------------------------------------------------------------//
void PixmapFont::reinit()
{
    if (d_imagesetOwner)
        ImagesetManager::getSingleton().destroy(*d_glyphImages);

    if (d_resourceGroup == BuiltInResourceGroup)
    {
        d_glyphImages = &ImagesetManager::getSingleton().get(d_filename);
        d_imagesetOwner = false;
    }
    else
    {
        d_glyphImages = &ImagesetManager::getSingleton().create(d_filename,
                                                                d_resourceGroup);
        d_imagesetOwner = true;
    }
}

//----------------------------------------------------------------------------//
void PixmapFont::updateFont()
{
    const float factor = (d_autoScale ? d_horzScaling : 1.0f) / d_origHorzScaling;

    d_ascender = 0;
    d_descender = 0;
    d_height = 0;
    d_maxCodepoint = 0;

    d_glyphImages->setAutoScalingEnabled(d_autoScale);
    d_glyphImages->setNativeResolution(Size(d_nativeHorzRes, d_nativeVertRes));

    for (CodepointMap::const_iterator i = d_cp_map.begin(); i != d_cp_map.end(); ++i)
    {
        if (i->first > d_maxCodepoint)
            d_maxCodepoint = i->first;

        ((FontGlyph&)i->second).setAdvance(i->second.getAdvance() * factor);

        const Image* img = i->second.getImage();

        if (img->getOffsetY() < d_ascender)
            d_ascender = img->getOffsetY();
        if (img->getHeight() + img->getOffsetY() > d_descender)
            d_descender = img->getHeight() + img->getOffsetY();
    }

    d_ascender = -d_ascender;
    d_descender = -d_descender;
    d_height = d_ascender - d_descender;

    d_origHorzScaling = d_autoScale ? d_horzScaling : 1.0f;
}

//----------------------------------------------------------------------------//
void PixmapFont::writeXMLToStream_impl (XMLSerializer& xml_stream) const
{
    float advscale = 1.0f / d_origHorzScaling;
    for (CodepointMap::const_iterator i = d_cp_map.begin(); i != d_cp_map.end(); ++i)
    {
        xml_stream.openTag("Mapping")
            .attribute(Font_xmlHandler::MappingCodepointAttribute,
                       PropertyHelper<uint>::toString(i->first))
            .attribute(Font_xmlHandler::MappingHorzAdvanceAttribute,
                       PropertyHelper<float>::toString(i->second.getAdvance() * advscale))
            .attribute(Font_xmlHandler::MappingImageAttribute,
                       i->second.getImage()->getName());

        xml_stream.closeTag();
    }
}

//----------------------------------------------------------------------------//
void PixmapFont::defineMapping(const utf32 codepoint, const String& image_name,
                               const float horz_advance)
{
    const Image& image(d_glyphImages->getImage(image_name));

    float adv = (horz_advance == -1.0f) ?
        (float)(int)(image.getWidth() + image.getOffsetX()) :
        horz_advance;

    if (d_autoScale)
        adv *= d_origHorzScaling;

    if (codepoint > d_maxCodepoint)
        d_maxCodepoint = codepoint;

    // create a new FontGlyph with given character code
    const FontGlyph glyph(adv, &image);

    if (image.getOffsetY() < -d_ascender)
        d_ascender = -image.getOffsetY();
    if (image.getHeight() + image.getOffsetY() > -d_descender)
        d_descender = -(image.getHeight() + image.getOffsetY());

    d_height = d_ascender - d_descender;

    // add glyph to the map
    d_cp_map[codepoint] = glyph;
}

//----------------------------------------------------------------------------//
const String& PixmapFont::getImageset() const
{
    return d_glyphImages->getName();
}

//----------------------------------------------------------------------------//
void PixmapFont::setImageset(const String& imageset)
{
    d_resourceGroup = "*";
    d_filename = imageset;
    reinit();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
