/***********************************************************************
    created:    27/01/2022
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
#include "CEGUI/text/RenderedTextParagraph.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
class TextParser;

struct CEGUIEXPORT SelectionInfo
{
    const Image* bgBrush = nullptr;
    ColourRect   bgColours = 0x800080FF;
    ColourRect   textColours = 0;         //!< Zero means keeping an existing text colour
    size_t       start = 0;
    size_t       end = 0;
};

//! \brief A text prepared to be rendered as graphics. Supports styles and embedded objects.
class CEGUIEXPORT RenderedText final
{
public:

    static constexpr size_t npos = std::numeric_limits<size_t>().max();

    RenderedText();
    RenderedText(const RenderedText& other) = delete;
    RenderedText(RenderedText&&) noexcept;
    virtual ~RenderedText();

    RenderedText& operator =(const RenderedText& rhs) = delete; // Use clone() instead
    RenderedText& operator =(RenderedText&&) noexcept;

    bool renderText(const String& text, TextParser* parser = nullptr, const Font* defaultFont = nullptr,
        DefaultParagraphDirection defaultParagraphDir = DefaultParagraphDirection::LeftToRight);

    void updateDynamicObjectExtents(const Window* hostWindow = nullptr);
    bool updateFormatting(float areaWidth);

    void createRenderGeometry(std::vector<GeometryBuffer*>& out, const glm::vec2& position,
        const ColourRect* modColours = nullptr, const Rectf* clipRect = nullptr,
        const SelectionInfo* selection = nullptr) const;

    //! Explicit cloning method. Used instead of copy constructor and assignment operator.
    RenderedText clone() const;

    void setHorizontalFormatting(HorizontalTextFormatting fmt);
    void setLastJustifiedLineHorizontalFormatting(HorizontalTextFormatting fmt);
    void setWordWrappingEnabled(bool wrap);

    const Sizef& getExtents() const { return d_extents; }
    bool empty() const { return d_paragraphs.empty(); }
    size_t getParagraphCount() const { return d_paragraphs.size(); }
    size_t getLineCount() const;
    float getAreaWidth() const { return d_areaWidth; }
    const Font* getDefaultFont() const { return d_defaultFont; }
    bool isFittingIntoAreaWidth() const;
    size_t getTextIndexAtPoint(const glm::vec2& pt) const;
    Rectf getCodepointBounds(size_t textIndex) const;

protected:

    std::vector<RenderedTextParagraph> d_paragraphs;
    std::vector<RenderedTextElementPtr> d_elements;
    const Font* d_defaultFont = nullptr;
    Sizef d_extents;
    float d_areaWidth = -1.f;

    HorizontalTextFormatting d_horzFormatting = HorizontalTextFormatting::LeftAligned;
    HorizontalTextFormatting d_lastJustifiedLineHorzFormatting = HorizontalTextFormatting::LeftAligned;
    bool d_wordWrap = false;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
