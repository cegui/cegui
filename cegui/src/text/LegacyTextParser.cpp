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
#include "CEGUI/Logger.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/Image.h"
#include "CEGUI/falagard/XMLEnumHelper.h"

namespace CEGUI
{
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
// Internal helper to parse part of a string, using backslash as an escape char
static bool parseSection(String::const_iterator pos, const String::const_iterator end,
    String::value_type delim, String& out)
{
    const String::value_type escape('\\');

    out.resize(0);

    auto start_iter(pos);
    for (; pos != end; ++pos)
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
bool LegacyTextParser::parse(const String& inText, std::u32string& outText,
    std::vector<size_t>& outOriginalIndices, std::vector<uint16_t>& outElementIndices,
    std::vector<RenderedTextElementPtr>& outElements)
{
    //???TODO: use stack struct for tag params to make the parser itself stateless?

    // Initialize formatting parameters with default values
    d_colours = Colour(0xFFFFFFFF);
    d_font = nullptr;
    d_padding = Rectf(0.f, 0.f, 0.f, 0.f);
    d_imageSize = Sizef(0.f, 0.f);
    d_vertFormatting = VerticalImageFormatting::BottomAligned;

    outText.reserve(inText.size());

    std::u32string tagString;
    tagString.reserve(64);

    bool escaped = false;
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    for (auto itIn = inText.begin(); itIn != inText.end(); /**/)
    {
#else
    String::codepoint_iterator itIn(inText.begin(), inText.begin(), inText.end());
    while (itIn)
    {
#endif
        const char32_t codePoint = *itIn;

        if (tagString.empty())
        {
            if (!escaped && codePoint == '[')
                tagString.push_back(codePoint);
            else if (!escaped && codePoint == '\\')
                escaped = true;
            else
            {
                // ensure that the style is prepared, use its index (or skip default style creation and use special element index for it?)
                // before creating a new style, try to find the same one
                //RenderedStringTextComponent rtc(curr_section, d_font);
                //rtc.setPadding(d_padding);
                //rtc.setColours(d_colours);
                //rtc.setVerticalFormatting(d_vertFormatting);
                //rs.appendComponent(rtc);

                if (escaped)
                {
                    switch (codePoint)
                    {
                        case '\n':
                        case 'n':  outText.push_back('\n'); break;
                        case '\r':
                        case 'r':  outText.push_back('\r'); break;
                        case 't':  outText.push_back('\t'); break;
                        case '\\': outText.push_back('\\'); break;
                        default:   outText.push_back('\\'); outText.push_back(codePoint); break;
                    };

                    escaped = false;
                }
                else
                {
                    outText.push_back(codePoint);
                }
            }
        }
        else
        {
            if (codePoint == ']')
            {
                processControlString(tagString);
                tagString.clear();
            }
            else
                tagString.push_back(codePoint);
        }

        ++itIn;
    }

    outText.shrink_to_fit();

    if (!tagString.empty())
    {
        Logger::getSingleton().logEvent(
            "LegacyTextParser::parse: ignoring unterminated tag : " + String(tagString));
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------//
void LegacyTextParser::processControlString(const std::u32string& ctrlStr)
{
    // All our default strings are of the form <var>='<val>'
    // so let's get the variables using '=' as delimiter:
    const size_t findPos = ctrlStr.find_first_of('=');
    if (findPos == String::npos)
    {
        Logger::getSingleton().logEvent(
            "LegacyTextParser::processControlString: invalid control string declared"
            " (format must be <var> = '<val>'): '" + String(ctrlStr) + "'.  Ignoring!");
        return;
    }

    // Skip leading '['
    auto i = d_tagHandlers.find(ctrlStr.substr(1, findPos - 1));
    if (i == d_tagHandlers.end())
    {
        Logger::getSingleton().logEvent(
            "LegacyTextParser::processControlString: unknown "
            "control variable in string: '" + String(ctrlStr) + "'.  Ignoring!");
        return;
    }

    // We were able to split the variable and value, let's see if we get a valid value.
    // Since the handler was found, we are sure that if the second variable couldn't be read,
    // it is empty. We will supply an empty string.
    const auto valueStart = findPos + 2;
    const auto valueEnd = ctrlStr.size() - 1;
    if (valueStart < valueEnd && ctrlStr[findPos + 1] == '\'' && ctrlStr[valueEnd] == '\'')
        (this->*(*i).second)(ctrlStr.substr(valueStart, valueEnd - valueStart));
    else
        (this->*(*i).second)(String::GetEmpty());
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleColour(const String& value)
{
    d_colours.setColours(PropertyHelper<Colour>::fromString(value));
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleFont(const String& value)
{
    d_font = value.empty() ? nullptr : &FontManager::getSingleton().get(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImage(const String& value)
{
    //!!!need to insert placeholder to outText!

    //RenderedStringImageComponent ric(PropertyHelper<Image*>::fromString(value));
    //ric.setPadding(d_padding);
    //ric.setColours(d_colours);
    //ric.setVerticalFormatting(d_vertFormatting);
    //ric.setSize(d_imageSize);
    //rs.appendComponent(ric);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleWindow(const String& value)
{
    //!!!need to insert placeholder to outText!

    //RenderedStringWidgetComponent rwc(value);
    //rwc.setPadding(d_padding);
    //rwc.setVerticalFormatting(d_vertFormatting);
    //rs.appendComponent(rwc);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleVertFormatting(const String& value)
{
    d_vertFormatting = FalagardXMLHelper<VerticalImageFormatting>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handlePadding(const String& value)
{
    d_padding = PropertyHelper<Rectf>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleTopPadding(const String& value)
{
    d_padding.d_min.y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleBottomPadding(const String& value)
{
    d_padding.d_max.y = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleLeftPadding(const String& value)
{
    d_padding.d_min.x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleRightPadding(const String& value)
{
    d_padding.d_max.x = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageSize(const String& value)
{
    d_imageSize = PropertyHelper<Sizef>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageWidth(const String& value)
{
    d_imageSize.d_width = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageHeight(const String& value)
{
    d_imageSize.d_height = PropertyHelper<float>::fromString(value);
}

}
