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
#pragma once
#include "CEGUI/text/TextParser.h"
#include "CEGUI/Rectf.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/String.h"
#include "CEGUI/falagard/Enums.h"
#include <unordered_map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    Basic text parser class that offers support for the following legacy (v0.8) tags:
    - 'colour' / 'color' value is a CEGUI colour property value.
    - 'bg-colour' / 'bg-color' value is a CEGUI colour property value.
    - 'font' value is the name of a font.
    - 'underline' value is a boolean.
    - 'strikeout' value is a boolean.
    - 'outline-colour' / 'outline-color' value is a CEGUI colour property value.
    - 'outline-size' value is a float.
    - 'image' value is a CEGUI image property value.
    - 'window' value is the name of a window.
    - 'vert-formatting' value is either TopAligned, BottomAligned, CentreAligned or Stretched.
    - 'padding' value is a CEGUI Rect property value.
    - 'top-padding' value is a float.
    - 'bottom-padding' value is a float.
    - 'left-padding' value is a float.
    - 'right-padding' value is a float.
    - 'image-size' value is a CEGUI size property value.
    - 'image-width' value is a float.
    - 'image-height' value is a float.
    - 'aspect-lock' value is a boolean.
    - 'reset' value is "full", "colour", "image" or "font" (reset color, font, image size, etc to default values).
    - 'br' without value.
*/
class CEGUIEXPORT LegacyTextParser : public TextParser
{
public:

    // Strings for supported tags
    static const String ColourTagName;
    static const String ColorTagName;
    static const String BgColourTagName;
    static const String BgColorTagName;
    static const String FontTagName;
    static const String UnderlineTagName;
    static const String StrikeoutTagName;
    static const String OutlineColourTagName;
    static const String OutlineColorTagName;
    static const String OutlineSizeTagName;
    static const String ImageTagName;
    static const String WindowTagName;
    static const String VertFormattingTagName;
    static const String VertImageFormattingTagName;
    static const String VertTextFormattingTagName;
    static const String PaddingTagName;
    static const String TopPaddingTagName;
    static const String BottomPaddingTagName;
    static const String LeftPaddingTagName;
    static const String RightPaddingTagName;
    static const String ImageSizeTagName;
    static const String ImageWidthTagName;
    static const String ImageHeightTagName;
    static const String ImageAspectLockTagName;
    static const String ResetTagName;
    static const String NewLineTagName;

    LegacyTextParser();

    virtual bool parse(const String& inText, std::u32string& outText, std::vector<size_t>& outOriginalIndices,
        std::vector<uint16_t>& outElementIndices, std::vector<RenderedTextElementPtr>& outElements) override;

protected:

    typedef void (LegacyTextParser::* TagHandler)(const String&);

    void processControlString(const std::u32string& ctrlStr, std::u32string& outText,
        std::vector<uint16_t>& outElementIndices, std::vector<RenderedTextElementPtr>& outElements);

    void resetStyle(bool color, bool font, bool image, bool others);

    //! handlers for the various tags supported
    void handleColour(const String& value);
    void handleBgColour(const String& value);
    void handleModulateColour(const String& value);
    void handleFont(const String& value);
    void handleUnderline(const String& value);
    void handleStrikeout(const String& value);
    void handleOutlineColour(const String& value);
    void handleOutlineSize(const String& value);
    void handleVertFormatting(const String& value);
    void handlePadding(const String& value);
    void handleTopPadding(const String& value);
    void handleBottomPadding(const String& value);
    void handleLeftPadding(const String& value);
    void handleRightPadding(const String& value);
    void handleImageSize(const String& value);
    void handleImageWidth(const String& value);
    void handleImageHeight(const String& value);
    void handleImageAspectLock(const String& value);
    void handleReset(const String& value);

    //! Collection to map tag names to their handler functions.
    std::unordered_map<String, TagHandler> d_tagHandlers;

    //! active padding values.
    Rectf d_padding;
    //! active colour values.
    ColourRect d_colours;
    ColourRect d_bgColours;
    ColourRect d_outlineColours;
    //! active font.
    Font* d_font = nullptr;
    //! active vertical image formatting
    VerticalImageFormatting d_vertFormatting = VerticalImageFormatting::BottomAligned;
    //! active image size
    Sizef d_imageSize;
    float d_outlineSize = 0.f;
    bool d_underline = false;
    bool d_strikeout = false;
    bool d_styleChanged = false;
    bool d_useModColour = true;
    bool d_imageAspectLock = false;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
