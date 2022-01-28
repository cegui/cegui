/***********************************************************************
    created:    28/01/2022
    author:     Vladimir Orlov
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2022 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/LegacyTextParser.h"
#include "CEGUI/RenderedStringTextComponent.h"
#include "CEGUI/RenderedStringImageComponent.h"
#include "CEGUI/RenderedStringWidgetComponent.h"
#include "CEGUI/Logger.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Font.h"
#include "CEGUI/Image.h"
#include "CEGUI/falagard/XMLEnumHelper.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
// Internal helper to parse part of a string, using backslash as an escape char
static bool parse_section(String::const_iterator& pos,
                          const String::const_iterator& end,
                          String::value_type delim,
                          String& out);

//----------------------------------------------------------------------------//
const String LegacyTextParser::ColourTagName("colour");
const String LegacyTextParser::FontTagName("font");
const String LegacyTextParser::ImageTagName("image");
const String LegacyTextParser::WindowTagName("window");
const String LegacyTextParser::VertFormattingTagName("vert-formatting");
const String LegacyTextParser::VertImageFormattingTagName("vert-image-formatting");
const String LegacyTextParser::VertTextFormattingTagName("vert-text-formatting");
const String LegacyTextParser::PaddingTagName("padding");
const String LegacyTextParser::TopPaddingTagName("top-padding");
const String LegacyTextParser::BottomPaddingTagName("bottom-padding");
const String LegacyTextParser::LeftPaddingTagName("left-padding");
const String LegacyTextParser::RightPaddingTagName("right-padding");
const String LegacyTextParser::ImageSizeTagName("image-size");
const String LegacyTextParser::ImageWidthTagName("image-width");
const String LegacyTextParser::ImageHeightTagName("image-height");

//----------------------------------------------------------------------------//
LegacyTextParser::LegacyTextParser()
{
    d_tagHandlers[ColourTagName] = &LegacyTextParser::handleColour;
    d_tagHandlers[FontTagName] = &LegacyTextParser::handleFont;
    d_tagHandlers[ImageTagName] = &LegacyTextParser::handleImage;
    d_tagHandlers[WindowTagName] = &LegacyTextParser::handleWindow;

    // FIXME: legacy separation on Image and Text, left for compatibility but may behave unexpectedly!
    d_tagHandlers[VertFormattingTagName] = &LegacyTextParser::handleVertFormatting;
    d_tagHandlers[VertImageFormattingTagName] = &LegacyTextParser::handleVertFormatting;
    d_tagHandlers[VertTextFormattingTagName] = &LegacyTextParser::handleVertFormatting;

    d_tagHandlers[PaddingTagName] = &LegacyTextParser::handlePadding;
    d_tagHandlers[TopPaddingTagName] = &LegacyTextParser::handleTopPadding;
    d_tagHandlers[BottomPaddingTagName] = &LegacyTextParser::handleBottomPadding;
    d_tagHandlers[LeftPaddingTagName] = &LegacyTextParser::handleLeftPadding;
    d_tagHandlers[RightPaddingTagName] = &LegacyTextParser::handleRightPadding;
    d_tagHandlers[ImageSizeTagName] = &LegacyTextParser::handleImageSize;
    d_tagHandlers[ImageWidthTagName] = &LegacyTextParser::handleImageWidth;
    d_tagHandlers[ImageHeightTagName] = &LegacyTextParser::handleImageHeight;
}

//----------------------------------------------------------------------------//
bool LegacyTextParser::parse(const String& inText, std::u32string& outText,
    std::vector<size_t>& outOriginalIndices, std::vector<uint16_t>& outElementIndices,
    std::vector<RenderedTextElementPtr>& outElements)
{
    //!!!TODO TEXT: IMPLEMENT!
    // each param is a stack
    // style is assembled from stacks when the first real style user is being processed
    // can try to find already created style instead of creating extra one
    // need to somehow export paragraph-level params from here! horz align, BIDI dir

    d_colours = initial_colours ? *initial_colours : 0xFFFFFFFF;
    d_font = initial_font;
    d_padding = Rectf(0, 0, 0, 0);
    d_imageSize.d_width = d_imageSize.d_height = 0.0f;
    d_vertFormatting = VerticalImageFormatting::BottomAligned;

    String curr_section, tag_string;

    for (auto itIn = inText.begin(); itIn != inText.end(); /* no-op*/)
    {
        const bool found_tag = parse_section(itIn, inText.end(), '[', curr_section);

        RenderedStringTextComponent rtc(curr_section, d_font);
        rtc.setPadding(d_padding);
        rtc.setColours(d_colours);
        rtc.setVerticalFormatting(d_vertFormatting);
        rs.appendComponent(rtc);

        if (!found_tag)
            return true;

        if (!parse_section(itIn, inText.end(), ']', tag_string))
        {
            Logger::getSingleton().logEvent(
                "LegacyTextParser::parse: Ignoring unterminated tag : [" + tag_string);
            return false;
        }

        processControlString(rs, tag_string);
    }

    return true;
}

//----------------------------------------------------------------------------//
bool parse_section(String::const_iterator& pos, const String::const_iterator& end,
                   String::value_type delim, String& out)
{
    const String::value_type escape('\\');
    out.resize(0);

    String::const_iterator start_iter(pos);

    for ( ; pos != end; ++pos)
    {
        if (*pos == delim)
        {
            out.append(start_iter, pos++);
            return true;
        }

        if (*pos == escape)
        {
            out.append(start_iter, pos++);

            if (pos == end)
                return false;

            start_iter = pos;
        }
    }

    out.append(start_iter, pos);
    return false;
}

//----------------------------------------------------------------------------//
void LegacyTextParser::processControlString(RenderedString& rs, const String& ctrl_str)
{
    // All our default strings are of the form <var> = '<val>'
    // so let's get the variables using '=' as delimiter:
    const size_t findPos = ctrl_str.find_first_of('=');
    if (findPos == String::npos)
    {
        Logger::getSingleton().logEvent(
            "LegacyTextParser::processControlString: invalid "
            "control string declared (format must be <var> = '<val>'): "
            "'" + ctrl_str + "'.  Ignoring!");
        return;
    }

    const String variable = ctrl_str.substr(0, findPos);
    String value = ctrl_str.substr(findPos + 1); 

    auto i = d_tagHandlers.find(variable);
    if (i != d_tagHandlers.end())
    {
        // We were able to split the variable and value, let's see if we get a valid value:
        const bool correctValueFormat = (value.front() == '\'') && (value.back() == '\'') && (value.length() > 2);
        if (correctValueFormat)
        {
            value.pop_back();
            value.erase(0, 1);
        }

        // Since the handler was found, we are sure that if the second variable
        // couldn't be read, it is empty. We will supply and empty string
        (this->*(*i).second)(rs, correctValueFormat ? value : String::GetEmpty());
    }
    else
        Logger::getSingleton().logEvent(
            "LegacyTextParser::processControlString:  unknown "
            "control variable in string: '" + value + "'.  Ignoring!");
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleColour(RenderedString&, const String& value)
{
    d_colours.setColours(PropertyHelper<Colour>::fromString(value));
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleFont(RenderedString&, const String& value)
{
    d_font = value.empty() ? nullptr : &FontManager::getSingleton().get(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImage(RenderedString& rs, const String& value)
{
    RenderedStringImageComponent ric(PropertyHelper<Image*>::fromString(value));
    ric.setPadding(d_padding);
    ric.setColours(d_colours);
    ric.setVerticalFormatting(d_vertFormatting);
    ric.setSize(d_imageSize);
    rs.appendComponent(ric);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleWindow(RenderedString& rs, const String& value)
{
    RenderedStringWidgetComponent rwc(value);
    rwc.setPadding(d_padding);
    rwc.setVerticalFormatting(d_vertFormatting);
    rs.appendComponent(rwc);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleVertFormatting(RenderedString&, const String& value)
{
    d_vertFormatting = FalagardXMLHelper<VerticalImageFormatting>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handlePadding(RenderedString&, const String& value)
{
    d_padding = PropertyHelper<Rectf>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleTopPadding(RenderedString&, const String& value)
{
    d_padding.d_min.y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleBottomPadding(RenderedString&, const String& value)
{
    d_padding.d_max.y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleLeftPadding(RenderedString&, const String& value)
{
    d_padding.d_min.x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleRightPadding(RenderedString&, const String& value)
{
    d_padding.d_max.x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageSize(RenderedString&, const String& value)
{
    d_imageSize = PropertyHelper<Sizef >::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageWidth(RenderedString&, const String& value)
{
    d_imageSize.d_width = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageHeight(RenderedString&, const String& value)
{
    d_imageSize.d_height = PropertyHelper<float>::fromString(value);
}

}
