/***********************************************************************
    created:    28/05/2009
    author:     Paul Turner
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
#include "CEGUI/BasicRenderedStringParser.h"
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
const String BasicRenderedStringParser::ColourTagName("colour");
const String BasicRenderedStringParser::FontTagName("font");
const String BasicRenderedStringParser::ImageTagName("image");
const String BasicRenderedStringParser::WindowTagName("window");
const String BasicRenderedStringParser::VertFormattingTagName("vert-formatting");
const String BasicRenderedStringParser::VertImageFormattingTagName("vert-image-formatting");
const String BasicRenderedStringParser::VertTextFormattingTagName("vert-text-formatting");
const String BasicRenderedStringParser::PaddingTagName("padding");
const String BasicRenderedStringParser::TopPaddingTagName("top-padding");
const String BasicRenderedStringParser::BottomPaddingTagName("bottom-padding");
const String BasicRenderedStringParser::LeftPaddingTagName("left-padding");
const String BasicRenderedStringParser::RightPaddingTagName("right-padding");
const String BasicRenderedStringParser::ImageSizeTagName("image-size");
const String BasicRenderedStringParser::ImageWidthTagName("image-width");
const String BasicRenderedStringParser::ImageHeightTagName("image-height");

//----------------------------------------------------------------------------//
RenderedString BasicRenderedStringParser::parse(
    const String& input_string, const Font* initial_font,
    const ColourRect* initial_colours, DefaultParagraphDirection defaultParagraphDir)
{
    // first-time initialisation (due to issues with static creation order)
    if (!d_initialised)
        initialiseTagHandlers();

    initialiseDefaultState();

    // Override active font if necessary
    d_font = initial_font;

    // Override active font if necessary
    if (initial_colours)
        d_colours = *initial_colours;

    RenderedString rs;
    String curr_section, tag_string;

    for (auto itIn = input_string.begin(); itIn != input_string.end(); /* no-op*/)
    {
        const bool found_tag = parse_section(itIn, input_string.end(), '[', curr_section);
        appendRenderedText(rs, curr_section, defaultParagraphDir);

        if (!found_tag)
            return rs;

        if (!parse_section(itIn, input_string.end(), ']', tag_string))
        {
            Logger::getSingleton().logEvent(
                "BasicRenderedStringParser::parse: Ignoring unterminated tag : [" +
                tag_string);

            return rs;
        }

        processControlString(rs, tag_string);
    }

    return rs;
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
void BasicRenderedStringParser::appendRenderedText(RenderedString& rs, const String& text, DefaultParagraphDirection dir) const
{
    if (text.empty())
        return;

    size_t cpos = 0;
    // split the given string into lines based upon the newline character
    while (text.length() > cpos)
    {
        // find next newline
        const size_t nlpos = text.find('\n', cpos);
        // calculate length of this substring
        const size_t len =
            ((nlpos != String::npos) ? nlpos : text.length()) - cpos;

        // construct new text component and append it.
        RenderedStringTextComponent rtc(text.substr(cpos, len), d_font);
        rtc.setPadding(d_padding);
        rtc.setColours(d_colours);
        rtc.setVerticalFormatting(d_vertFormatting);
        rtc.setDefaultParagraphDirection(dir);
        rs.appendComponent(rtc);

        // break line if needed
        if (nlpos != String::npos)
            rs.appendLineBreak();

        // advance current position.  +1 to skip the \n char
        cpos += len + 1;
    }
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::processControlString(RenderedString& rs, const String& ctrl_str)
{
    // All our default strings are of the form <var> = '<val>'
    // so let's get the variables using '=' as delimiter:
    const size_t findPos = ctrl_str.find_first_of('=');
    if (findPos == String::npos)
    {
        Logger::getSingleton().logEvent(
            "BasicRenderedStringParser::processControlString: invalid "
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
            "BasicRenderedStringParser::processControlString:  unknown "
            "control variable in string: '" + value + "'.  Ignoring!");
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::initialiseDefaultState()
{
    d_colours = Colour(0xFFFFFFFF);
    d_font = nullptr;
    d_padding = Rectf(0, 0, 0, 0);
    d_imageSize.d_width = d_imageSize.d_height = 0.0f;
    d_vertFormatting = VerticalImageFormatting::BottomAligned;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::initialiseTagHandlers()
{
    d_tagHandlers[ColourTagName] = &BasicRenderedStringParser::handleColour;
    d_tagHandlers[FontTagName] = &BasicRenderedStringParser::handleFont;
    d_tagHandlers[ImageTagName] = &BasicRenderedStringParser::handleImage;
    d_tagHandlers[WindowTagName] = &BasicRenderedStringParser::handleWindow;

    // FIXME: legacy separation on Image and Text, left for compatibility but may behave unexpectedly!
    d_tagHandlers[VertFormattingTagName] = &BasicRenderedStringParser::handleVertFormatting;
    d_tagHandlers[VertImageFormattingTagName] = &BasicRenderedStringParser::handleVertFormatting;
    d_tagHandlers[VertTextFormattingTagName] = &BasicRenderedStringParser::handleVertFormatting;

    d_tagHandlers[PaddingTagName] = &BasicRenderedStringParser::handlePadding;
    d_tagHandlers[TopPaddingTagName] = &BasicRenderedStringParser::handleTopPadding;
    d_tagHandlers[BottomPaddingTagName] = &BasicRenderedStringParser::handleBottomPadding;
    d_tagHandlers[LeftPaddingTagName] = &BasicRenderedStringParser::handleLeftPadding;
    d_tagHandlers[RightPaddingTagName] = &BasicRenderedStringParser::handleRightPadding;
    d_tagHandlers[ImageSizeTagName] = &BasicRenderedStringParser::handleImageSize;
    d_tagHandlers[ImageWidthTagName] = &BasicRenderedStringParser::handleImageWidth;
    d_tagHandlers[ImageHeightTagName] = &BasicRenderedStringParser::handleImageHeight;

    d_initialised = true;
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleColour(RenderedString&, const String& value)
{
    d_colours.setColours(PropertyHelper<Colour>::fromString(value));
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleFont(RenderedString&, const String& value)
{
    d_font = value.empty() ? nullptr : &FontManager::getSingleton().get(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImage(RenderedString& rs, const String& value)
{
    RenderedStringImageComponent ric(PropertyHelper<Image*>::fromString(value));
    ric.setPadding(d_padding);
    ric.setColours(d_colours);
    ric.setVerticalFormatting(d_vertFormatting);
    ric.setSize(d_imageSize);
    rs.appendComponent(ric);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleWindow(RenderedString& rs, const String& value)
{
    RenderedStringWidgetComponent rwc(value);
    rwc.setPadding(d_padding);
    rwc.setVerticalFormatting(d_vertFormatting);
    rs.appendComponent(rwc);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleVertFormatting(RenderedString&, const String& value)
{
    d_vertFormatting = FalagardXMLHelper<VerticalImageFormatting>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handlePadding(RenderedString&, const String& value)
{
    d_padding = PropertyHelper<Rectf>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleTopPadding(RenderedString&, const String& value)
{
    d_padding.d_min.y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleBottomPadding(RenderedString&, const String& value)
{
    d_padding.d_max.y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleLeftPadding(RenderedString&, const String& value)
{
    d_padding.d_min.x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleRightPadding(RenderedString&, const String& value)
{
    d_padding.d_max.x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImageSize(RenderedString&, const String& value)
{
    d_imageSize = PropertyHelper<Sizef >::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImageWidth(RenderedString&, const String& value)
{
    d_imageSize.d_width = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void BasicRenderedStringParser::handleImageHeight(RenderedString&, const String& value)
{
    d_imageSize.d_height = PropertyHelper<float>::fromString(value);
}

}
