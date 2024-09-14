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
#include "CEGUI/text/RenderedTextStyle.h"
#include "CEGUI/text/RenderedTextImage.h"
#include "CEGUI/text/RenderedTextWidget.h"
#include "CEGUI/Logger.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/text/Font.h"
#include "CEGUI/Image.h"
#include "CEGUI/falagard/XMLEnumHelper.h"

namespace CEGUI
{
//----------------------------------------------------------------------------//
const String LegacyTextParser::ColourTagName("colour");
const String LegacyTextParser::ColorTagName("color");
const String LegacyTextParser::BgColourTagName("bg-colour");
const String LegacyTextParser::BgColorTagName("bg-color");
const String LegacyTextParser::FontTagName("font");
const String LegacyTextParser::UnderlineTagName("underline");
const String LegacyTextParser::StrikeoutTagName("strikeout");
const String LegacyTextParser::OutlineColourTagName("outline-colour");
const String LegacyTextParser::OutlineColorTagName("outline-color");
const String LegacyTextParser::OutlineSizeTagName("outline-size");
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
const String LegacyTextParser::ImageAspectLockTagName("aspect-lock");
const String LegacyTextParser::ResetTagName("reset");
const String LegacyTextParser::NewLineTagName("br");

//----------------------------------------------------------------------------//
LegacyTextParser::LegacyTextParser()
{
    d_tagHandlers[ColourTagName] = &LegacyTextParser::handleColour;
    d_tagHandlers[ColorTagName] = &LegacyTextParser::handleColour;
    d_tagHandlers[BgColourTagName] = &LegacyTextParser::handleBgColour;
    d_tagHandlers[BgColorTagName] = &LegacyTextParser::handleBgColour;
    d_tagHandlers[FontTagName] = &LegacyTextParser::handleFont;
    d_tagHandlers[UnderlineTagName] = &LegacyTextParser::handleUnderline;
    d_tagHandlers[StrikeoutTagName] = &LegacyTextParser::handleStrikeout;
    d_tagHandlers[OutlineColourTagName] = &LegacyTextParser::handleOutlineColour;
    d_tagHandlers[OutlineColorTagName] = &LegacyTextParser::handleOutlineColour;
    d_tagHandlers[OutlineSizeTagName] = &LegacyTextParser::handleOutlineSize;

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
    d_tagHandlers[ImageAspectLockTagName] = &LegacyTextParser::handleImageAspectLock;
    d_tagHandlers[ResetTagName] = &LegacyTextParser::handleReset;
}

//----------------------------------------------------------------------------//
bool LegacyTextParser::parse(const String& inText, std::u32string& outText,
    std::vector<size_t>& outOriginalIndices, std::vector<uint16_t>& outElementIndices,
    std::vector<RenderedTextElementPtr>& outElements)
{
    outText.clear();
    outOriginalIndices.clear();
    outElementIndices.clear();

    if (inText.empty())
        return true;

    //???TODO: use stack struct for tag params to make the parser itself stateless?

    // Initialize formatting parameters with default values
    resetStyle(true, true, true, true);

    outText.reserve(inText.size());
    outOriginalIndices.reserve(inText.size());
    outElementIndices.reserve(inText.size());

    std::u32string tagString;
    tagString.reserve(64);

    uint16_t elementIndex = std::numeric_limits<uint16_t>().max();
    bool escaped = false;
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    for (auto itIn = inText.begin(); itIn != inText.end(); /**/)
    {
#else
    String::codepoint_iterator itIn(inText.begin(), inText.begin(), inText.end());
    while (itIn)
    {
        outOriginalIndices.push_back(itIn.getCodeUnitIndexFromStart());
#endif
        char32_t codePoint = *itIn;

        if (tagString.empty())
        {
            if (!escaped && codePoint == '[')
                tagString.push_back(codePoint);
            else if (!escaped && codePoint == '\\')
                escaped = true;
            else
            {
                if (d_styleChanged)
                {
                    //!!!TODO TEXT: try to find the same style first!

                    auto style = std::make_unique<RenderedTextStyle>(d_font, d_underline, d_strikeout);
                    style->setTextColour(d_colours);
                    style->setBackgroundColour(d_bgColours);
                    style->setUseModulateColour(d_useModColour);
                    style->setPadding(d_padding);
                    style->setVerticalFormatting(d_vertFormatting);
                    style->setOutlineColour(d_outlineColours);
                    style->setOutlineSize(d_outlineSize);
                    elementIndex = static_cast<uint16_t>(outElements.size());
                    outElements.push_back(std::move(style));
                    d_styleChanged = false;
                }

                if (escaped)
                {
                    // If it is not actually escaped, print the slash itself too.
                    // Only slash and opening square bracket support escaping now.
                    if (codePoint != '\\' && codePoint != '[')
                    {
                        outText.push_back('\\');
                        outElementIndices.push_back(elementIndex);
                    }

                    escaped = false;
                }

                outText.push_back(codePoint);
                outElementIndices.push_back(elementIndex);
            }
        }
        else
        {
            if (codePoint == ']')
            {
                processControlString(tagString, outText, outElementIndices, outElements);
                tagString.clear();
            }
            else
            {
                tagString.push_back(codePoint);
            }
        }

        ++itIn;
    }

    // NB: don't shrink outElementIndices, it will be thrown away soon
    outText.shrink_to_fit();

    // Skip remapping if mapping is 1 to 1
    if (!outOriginalIndices.empty() && outOriginalIndices.back() == outOriginalIndices.size() - 1)
        outOriginalIndices.clear();

    if (outElements.size() >= static_cast<size_t>(std::numeric_limits<uint16_t>().max()))
    {
        Logger::getSingleton().logEvent("LegacyTextParser::parse: too many elements and styles, parsing failed");
        return false;
    }

    if (!tagString.empty())
    {
        Logger::getSingleton().logEvent("LegacyTextParser::parse: unterminated tag : " + String(tagString));
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------//
void LegacyTextParser::processControlString(const std::u32string& ctrlStr, std::u32string& outText,
    std::vector<uint16_t>& outElementIndices, std::vector<RenderedTextElementPtr>& outElements)
{
    if (NewLineTagName.compare(0, String::npos, ctrlStr, 1, String::npos) == 0)
    {
        outText.push_back('\n');
        outElementIndices.push_back(static_cast<uint16_t>(outElements.size()));
        return;
    }

    // All our default strings are of the form <var>='<val>'
    // so let's get the variables using '=' as delimiter:
    const size_t findPos = ctrlStr.find_first_of('=');
    if (findPos == String::npos)
    {
        Logger::getSingleton().logEvent(
            "LegacyTextParser::processControlString: invalid control string declared"
            " (format must be <var>='<val>'): '" + String(ctrlStr) + "'.  Ignoring!");
        return;
    }

    // Skip leading '['
    const String key = ctrlStr.substr(1, findPos - 1);

    const auto valueStart = findPos + 2;
    const auto valueEnd = ctrlStr.size() - 1;
    const bool valueValid = (valueStart < valueEnd&& ctrlStr[findPos + 1] == '\'' && ctrlStr[valueEnd] == '\'');

    // First try registered handlers
    auto it = d_tagHandlers.find(key);
    if (it != d_tagHandlers.end())
    {
        // We were able to split the variable and value, let's see if we get a valid value.
        // Since the handler was found, we are sure that if the second variable couldn't be read,
        // it is empty. We will supply an empty string.
        if (valueValid)
            (this->*(*it).second)(ctrlStr.substr(valueStart, valueEnd - valueStart));
        else
            (this->*(*it).second)(String::GetEmpty());

        // For simplicity, consider all other changes as style changes
        d_styleChanged = true;
        return;
    }

    if (valueValid && key == ImageTagName)
    {
        const String val = ctrlStr.substr(valueStart, valueEnd - valueStart);
        if (!ImageManager::getSingleton().isDefined(val))
        {
            ImageManager::getSingleton().addBitmapImageFromFile(val, val);
        }

        //!!!TODO TEXT: try to find the same image first!

        auto element = std::make_unique<RenderedTextImage>( PropertyHelper<Image*>::fromString(val) );
        element->setColour(d_colours);
        element->setBackgroundColour(d_bgColours);
        element->setUseModulateColour(false);
        element->setSize(d_imageSize, d_imageAspectLock);
        element->setFont(d_font);
        element->setPadding(d_padding);
        element->setVerticalFormatting(d_vertFormatting);
        const auto elementIndex = static_cast<uint16_t>(outElements.size());
        outElements.push_back(std::move(element));

        // Zero width space as a placeholder
        outText.push_back(0x200B);
        outElementIndices.push_back(elementIndex);
        return;
    }

    if (valueValid && key == WindowTagName)
    {
        //!!!TODO TEXT: try to find the same widget first!
        //fail if found, can't use the same widget twice!

        auto element = std::make_unique<RenderedTextWidget>(ctrlStr.substr(valueStart, valueEnd - valueStart));
        element->setFont(d_font);
        element->setBackgroundColour(d_bgColours);
        element->setPadding(d_padding);
        element->setVerticalFormatting(d_vertFormatting);
        const auto elementIndex = static_cast<uint16_t>(outElements.size());
        outElements.push_back(std::move(element));

        // Zero width space as a placeholder
        outText.push_back(0x200B);
        outElementIndices.push_back(elementIndex);
        return;
    }

    Logger::getSingleton().logEvent("LegacyTextParser::processControlString: unknown "
        "control variable in string: '" + String(ctrlStr) + "'.  Ignoring!", LoggingLevel::Warning);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleColour(const String& value)
{
    d_colours.setColours(PropertyHelper<Colour>::fromString(value));
    d_useModColour = false;
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleBgColour(const String& value)
{
    d_bgColours.setColours(PropertyHelper<Colour>::fromString(value));
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleOutlineColour(const String& value)
{
    d_outlineColours.setColours(PropertyHelper<Colour>::fromString(value));
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleOutlineSize(const String& value)
{
    d_outlineSize = PropertyHelper<float>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleFont(const String& value)
{
    d_font = value.empty() ? nullptr : &FontManager::getSingleton().get(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleUnderline(const String& value)
{
    d_underline = PropertyHelper<bool>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::handleStrikeout(const String& value)
{
    d_strikeout = PropertyHelper<bool>::fromString(value);
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

//----------------------------------------------------------------------------//
void LegacyTextParser::handleImageAspectLock(const String& value)
{
    d_imageAspectLock = PropertyHelper<bool>::fromString(value);
}

//----------------------------------------------------------------------------//
void LegacyTextParser::resetStyle(bool color, bool font, bool image, bool others)
{
    if (color) {
        d_colours = Colour(0xFFFFFFFF);
        d_bgColours = Colour(0x00000000);
        d_outlineColours = Colour();
        d_useModColour = true;
    }
    if (font) {
        d_font = nullptr;
        d_underline = false;
        d_strikeout = false;
    }
    if (image) {
        d_imageSize = Sizef(0.f, 0.f);
        d_imageAspectLock = false;
    }
    if (others) {
        d_padding = Rectf(0.f, 0.f, 0.f, 0.f);
        d_vertFormatting = VerticalImageFormatting::BottomAligned;
        d_outlineSize = 0.f;
    }
    d_styleChanged = true;
}

void LegacyTextParser::handleReset(const String& value)
{
   resetStyle(
       value == "full" || value == "colour",
       value == "full" || value == "font",
       value == "full" || value == "image",
       value == "full"
   );
}

}
