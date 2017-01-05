/***********************************************************************
    created:    Sat Jun 25 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUI/WindowRendererSets/Core/Editbox.h"
#include "CEGUI/falagard/WidgetLookManager.h"
#include "CEGUI/falagard/WidgetLookFeel.h"
#include "CEGUI/falagard/XMLEnumHelper.h"
#include "CEGUI/PropertyHelper.h"
#include "CEGUI/CoordConverter.h"
#include "CEGUI/Font.h"
#include "CEGUI/BidiVisualMapping.h"
#include "CEGUI/TplWindowRendererProperty.h"

#include <stdio.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const String FalagardEditbox::TypeName("Core/Editbox");

const String FalagardEditbox::UnselectedTextColourPropertyName( "NormalTextColour" );
const String FalagardEditbox::SelectedTextColourPropertyName( "SelectedTextColour" );
const String FalagardEditbox::ActiveSelectionColourPropertyName( "ActiveSelectionColour" );
const String FalagardEditbox::InactiveSelectionColourPropertyName( "InactiveSelectionColour" );

const float FalagardEditbox::DefaultCaretBlinkTimeout(0.66f);

//----------------------------------------------------------------------------//
FalagardEditbox::FalagardEditbox(const String& type) :
    EditboxWindowRenderer(type),
    d_lastTextOffset(0),
    d_blinkCaret(false),
    d_caretBlinkTimeout(DefaultCaretBlinkTimeout),
    d_caretBlinkElapsed(0.0f),
    d_showCaret(true),
    d_textFormatting(HorizontalTextFormatting::LeftAligned)
{
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, bool,
        "BlinkCaret", "Property to get/set whether the Editbox caret should blink.  "
        "Value is either \"true\" or \"false\".",
        &FalagardEditbox::setCaretBlinkEnabled, &FalagardEditbox::isCaretBlinkEnabled,
        false);
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, float,
        "BlinkCaretTimeout", "Property to get/set the caret blink timeout / speed.  "
        "Value is a float value indicating the timeout in seconds.",
        &FalagardEditbox::setCaretBlinkTimeout, &FalagardEditbox::getCaretBlinkTimeout,
        DefaultCaretBlinkTimeout);
    CEGUI_DEFINE_WINDOW_RENDERER_PROPERTY(FalagardEditbox, HorizontalTextFormatting,
        "TextFormatting", "Property to get/set the horizontal formatting mode. "
        "Value is one of: LeftAligned, RightAligned or HorzCentred",
        &FalagardEditbox::setTextFormatting, &FalagardEditbox::getTextFormatting,
        HorizontalTextFormatting::LeftAligned);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::createRenderGeometry()
{
    const WidgetLookFeel& wlf = getLookNFeel();

    renderBaseImagery(wlf);

    // no font == no more rendering
    const Font* font = d_window->getFont();
    if (!font)
        return;

    String visual_text;
    setupVisualString(visual_text);

    const ImagerySection& caret_imagery = wlf.getImagerySection("Caret");

    const Rectf text_area(wlf.getNamedArea("TextArea").getArea().getPixelRect(*d_window));
    const size_t caret_index = getCaretIndex(visual_text);
    const float caret_width = caret_imagery.getBoundingRect(*d_window, text_area).getWidth();
    const float text_extent = font->getTextExtent(visual_text);
    const float extent_to_caret_visual = font->getTextAdvance(visual_text.substr(0, caret_index));
    const float extent_to_caret_logical = extentToCarretLogical(extent_to_caret_visual, text_extent, caret_width);
    const float text_offset_logical = calculateTextOffset(text_area, text_extent, caret_width, extent_to_caret_logical);
    d_lastTextOffset = text_offset_logical;
    const float text_offset_visual = textOffsetVisual(text_area, text_extent);

#ifdef CEGUI_BIDI_SUPPORT
    renderTextBidi(wlf, visual_text, text_area, text_offset_visual);
#else
    createRenderGeometryForTextWithoutBidi(wlf, visual_text, text_area, text_offset_visual);
#endif

    renderCaret(caret_imagery, text_area, text_offset_visual, extent_to_caret_visual);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderBaseImagery(const WidgetLookFeel& wlf) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    String state;

    if (w->isEffectiveDisabled())
        state = "Disabled";
    else
    {
        if (w->isReadOnly())
            state = "ReadOnly";
        else
            state = "Enabled";

        if (w->isFocused())
            state += "Focused";
    }

    const StateImagery* imagery = &wlf.getStateImagery(state);

    imagery->render(*w);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setupVisualString(String& visual) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    if (w->isTextMaskingEnabled())
    {
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) 
        visual.assign(w->getText().length(), static_cast<String::value_type>(w->getTextMaskingCodepoint()));
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) 
        visual.assign(w->getText().length(), static_cast<char32_t>(w->getTextMaskingCodepoint()));
#endif
    }
    else
        visual.assign(w->getTextVisual());
}

//----------------------------------------------------------------------------//
size_t FalagardEditbox::getCaretIndex(const String& visual_text) const
{
    Editbox* w = static_cast<Editbox*>(d_window);

    size_t caretIndex = w->getCaretIndex();

#ifdef CEGUI_BIDI_SUPPORT
    // the char before the caret bidi type
    bool currCharIsRtl = false;
    if (!visual_text.empty() && caretIndex > 0)
    {
        size_t curCaretIndex = w->getCaretIndex();
        BidiCharType charBeforeCaretType = w->getBidiVisualMapping()->
            getBidiCharType(visual_text[curCaretIndex - 1]);
        // for neutral chars you decide by the char after
        for (; BidiCharType::NEUTRAL == charBeforeCaretType &&
               (visual_text.size() > curCaretIndex); curCaretIndex++)
        {
            charBeforeCaretType = w->getBidiVisualMapping()->
                getBidiCharType(visual_text[curCaretIndex - 1]);
        }

        currCharIsRtl  = (BidiCharType::RIGHT_TO_LEFT == charBeforeCaretType);
    }

    const bool isFirstChar = caretIndex == 0;

    // the pos is by the char before
    if (!isFirstChar)
        caretIndex--;

    // we need to find the caret pos by the logical to visual map
    if (w->getBidiVisualMapping()->getV2lMapping().size() > caretIndex)
        caretIndex = w->getBidiVisualMapping()->getL2vMapping()[caretIndex];

    // for non RTL char - the caret pos is after the char
    if (!currCharIsRtl)
        caretIndex++;

    // if first char is not rtl - we need to stand at the start of the line
    if (isFirstChar)
    {
        bool firstCharRtl =
            !visual_text.empty() &&
            (BidiCharType::RIGHT_TO_LEFT == w->getBidiVisualMapping()->
                getBidiCharType(visual_text[0]));

        if (!firstCharRtl)
            caretIndex--;
    }
#else
    CEGUI_UNUSED(visual_text);
#endif

    return caretIndex;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::extentToCarretLogical(const float extent_to_caret_visual, const float text_extent,
                                             const float caret_width) const
{
    switch (d_textFormatting)
    {
    case HorizontalTextFormatting::LeftAligned:
        return extent_to_caret_visual;
    case HorizontalTextFormatting::CentreAligned:
        return (text_extent -caret_width) /2;
    case HorizontalTextFormatting::RightAligned:
        return text_extent -extent_to_caret_visual -caret_width;
    default:
        throw InvalidRequestException("Invalid horizontal text formatting.");
    }
}

//----------------------------------------------------------------------------//
float FalagardEditbox::calculateTextOffset(const Rectf& text_area,
                                           const float /*text_extent*/,
                                           const float caret_width,
                                           const float extent_to_caret)
{
    // if caret is to the left of the box
    if ((d_lastTextOffset + extent_to_caret) < 0)
        return -extent_to_caret;

    // if caret is off to the right
    if ((d_lastTextOffset + extent_to_caret) >= (text_area.getWidth() - caret_width))
        return text_area.getWidth() - extent_to_caret - caret_width;

    // no change to text position; re-use last offset value.
    return d_lastTextOffset;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::textOffsetVisual(const Rectf& text_area, const float text_extent) const
{
    switch (d_textFormatting)
    {
    case HorizontalTextFormatting::LeftAligned:
        return d_lastTextOffset;
    case HorizontalTextFormatting::CentreAligned:
        return (text_area.getWidth() - text_extent) / 2;
    case HorizontalTextFormatting::RightAligned:
        return text_area.getWidth() -d_lastTextOffset -text_extent;
    default:
        throw InvalidRequestException("Invalid horizontal text formatting.");
    } 
}

//----------------------------------------------------------------------------//
void FalagardEditbox::createRenderGeometryForTextWithoutBidi(
    const WidgetLookFeel& wlf,
    const String& text,
    const Rectf& text_area,
    float text_offset)
{
    const Font* font = d_window->getFont();

    // setup initial rect for text formatting
    Rectf text_part_rect(text_area);
    // allow for scroll position
    text_part_rect.d_min.x += text_offset;
    // centre text vertically within the defined text area
    text_part_rect.d_min.y += (text_area.getHeight() - font->getFontHeight()) * 0.5f;

    ColourRect colours;
    // get unhighlighted text colour (saves accessing property twice)
    ColourRect unselectedColours;
    setColourRectToUnselectedTextColour(unselectedColours);
    // see if the editbox is active or inactive.
    Editbox* const w = static_cast<Editbox*>(d_window);
    const bool active = editboxIsFocussed();
    DefaultParagraphDirection defaultParagraphDir = w->getDefaultParagraphDirection();

    if (w->getSelectionLength() != 0)
    {
        // calculate required start and end offsets of selection imagery.
        float selStartOffset =
            font->getTextAdvance(text.substr(0, w->getSelectionStart()));
        float selEndOffset =
            font->getTextAdvance(text.substr(0, w->getSelectionEnd()));

        // calculate area for selection imagery.
        Rectf hlarea(text_area);
        hlarea.d_min.x += text_offset + selStartOffset;
        hlarea.d_max.x = hlarea.d_min.x + (selEndOffset - selStartOffset);

        // create render geometry for the selection imagery.
        const String& stateName = active ? "ActiveSelection" : "InactiveSelection";
        wlf.getStateImagery(stateName).render(*w, hlarea, nullptr, &text_area);
    }

    // create render geometry for pre-highlight text
    String sect = text.substr(0, w->getSelectionStart());
    colours = unselectedColours;

    

    auto preHighlightTextGeomBuffers = font->createTextRenderGeometry(
        sect, text_part_rect.d_min.x,
        text_part_rect.getPosition(),
        &text_area, true, colours, defaultParagraphDir);

    w->appendGeometryBuffers(preHighlightTextGeomBuffers);

    // create render geometry for highlight text
    sect = text.substr(w->getSelectionStart(), w->getSelectionLength());
    setColourRectToSelectedTextColour(colours);

    auto highlitTextGeomBuffers = font->createTextRenderGeometry(
        sect, text_part_rect.d_min.x, text_part_rect.getPosition(),
        &text_area, true, colours, defaultParagraphDir);

    w->appendGeometryBuffers(highlitTextGeomBuffers);

    // create render geometry for  post-highlight text
    sect = text.substr(w->getSelectionEnd());
    colours = unselectedColours;

     auto postHighlitTextGeomBuffers = font->createTextRenderGeometry(
         sect, text_part_rect.d_min.x, text_part_rect.getPosition(),
         &text_area, true, colours, defaultParagraphDir);

    w->appendGeometryBuffers(postHighlitTextGeomBuffers);
}

#ifdef CEGUI_BIDI_SUPPORT
//----------------------------------------------------------------------------//
void FalagardEditbox::renderTextBidi(const WidgetLookFeel& wlf,
                                     const String& text,
                                     const Rectf& text_area,
                                     float text_offset)
{
    const Font* const font = d_window->getFont();

    // setup initial rect for text formatting
    Rectf text_part_rect(text_area);
    // allow for scroll position
    text_part_rect.d_min.d_x += text_offset;
    // centre text vertically within the defined text area
    text_part_rect.d_min.d_y += (text_area.getHeight() - font->getFontHeight()) * 0.5f;

    ColourRect colours;
    // get unhighlighted text colour (saves accessing property twice)
    ColourRect unselectedColour;
    setColourRectToUnselectedTextColour(unselectedColour);
    // see if the editbox is active or inactive.
    Editbox* const w = static_cast<Editbox*>(d_window);
    const bool active = editboxIsFocussed();

    if (w->getSelectionLength() == 0)
    {
        // no highlighted text - we can draw the whole thing
        colours = unselectedColour;
        text_part_rect.d_min.d_x =
            font->drawText(w->getGeometryBuffers(), text,
                           text_part_rect.getPosition(), &text_area, colours);
    }
    else
    {
        // there is highlighted text - because of the Bidi support - the
        // highlighted area can be in some cases nonconsecutive.
        // So - we need to draw it char by char (I guess we can optimize it more
        // but this is not that big performance hit because it only happens if
        // we have highlighted text - not that common...)
        for (size_t i = 0 ; i < text.size() ; i++)
        {
            // get the char
            String currChar = text.substr(i, 1);
            size_t realPos = 0;

            // get he visual pos of the char
            if (w->getBidiVisualMapping()->getV2lMapping().size() > i)
            {
                realPos = w->getBidiVisualMapping()->getV2lMapping()[i];
            }

            // check if it is in the highlighted region
            bool highlighted =
                realPos >= w->getSelectionStart() &&
                realPos < w->getSelectionStart() + w->getSelectionLength();

            float charAdvance = font->getGlyphForCodepoint(currChar[0])->getAdvance(1.0f);

            if (highlighted)
            {
                setColourRectToSelectedTextColour(colours);

                {

                    // calculate area for selection imagery.
                    Rectf hlarea(text_area);
                    hlarea.d_min.d_x = text_part_rect.d_min.d_x ;
                    hlarea.d_max.d_x = text_part_rect.d_min.d_x + charAdvance ;

                    // render the selection imagery.
                    wlf.getStateImagery(active ? "ActiveSelection" :
                                                 "InactiveSelection").
                        render(*w, hlarea, 0, &text_area);
                }

            }
            else
            {
                colours = unselectedColour;
            }
            font->drawText(w->getGeometryBuffers(), currChar,
                           text_part_rect.getPosition(), &text_area, colours);

            // adjust rect for next section
            text_part_rect.d_min.d_x += charAdvance;

        }
    }
}
#endif

//----------------------------------------------------------------------------//
bool FalagardEditbox::editboxIsFocussed() const
{
    Editbox* const w = static_cast<Editbox*>(d_window);
    return w->hasInputFocus();
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::editboxIsReadOnly() const
{
    Editbox* const w = static_cast<Editbox*>(d_window);
    return w->isReadOnly();
}

//----------------------------------------------------------------------------//
void FalagardEditbox::renderCaret(const ImagerySection& imagery,
                                  const Rectf& text_area,
                                  const float text_offset,
                                  const float extent_to_caret) const
{
    if ((!d_blinkCaret || d_showCaret) && editboxIsFocussed() && !editboxIsReadOnly())
    {
        Rectf caretRect(text_area);
        caretRect.d_min.x += extent_to_caret + text_offset;

        imagery.render(*d_window, caretRect, nullptr, &text_area);
    }
}

//----------------------------------------------------------------------------//
size_t FalagardEditbox::getTextIndexFromPosition(const glm::vec2& pt) const
{
    Editbox* w = static_cast<Editbox*>(d_window);
    const Font* font = w->getFont();
    if (!font)
        return w->getText().length();
    float wndx = CoordConverter::screenToWindowX(*w, pt.x);
    String visual_text;
    setupVisualString(visual_text);
    const Rectf text_area(getLookNFeel().getNamedArea("TextArea").getArea().getPixelRect(*d_window));
    const float text_extent = font->getTextExtent(visual_text);
    wndx -= textOffsetVisual(text_area, text_extent);
    return w->getFont()->getCharAtPixel(visual_text, wndx);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setColourRectToUnselectedTextColour(ColourRect& cr) const
{
    setColourRectToOptionalPropertyColour(UnselectedTextColourPropertyName, cr);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setColourRectToSelectedTextColour(ColourRect& cr) const
{
    setColourRectToOptionalPropertyColour(SelectedTextColourPropertyName, cr);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setColourRectToOptionalPropertyColour(
    const String& propertyName,
    ColourRect& colour_rect) const
{
    if (d_window->isPropertyPresent(propertyName))
        colour_rect = 
            d_window->getProperty<ColourRect>(propertyName);
    else
        colour_rect.setColours(0);
}

//----------------------------------------------------------------------------//
void FalagardEditbox::update(float elapsed)
{
    // do base class stuff
    WindowRenderer::update(elapsed);

    // only do the update if we absolutely have to
    if (d_blinkCaret &&
        !static_cast<Editbox*>(d_window)->isReadOnly() &&
        static_cast<Editbox*>(d_window)->hasInputFocus())
    {
        d_caretBlinkElapsed += elapsed;

        if (d_caretBlinkElapsed > d_caretBlinkTimeout)
        {
            d_caretBlinkElapsed = 0.0f;
            d_showCaret ^= true;
            // state changed, so need a redraw
            d_window->invalidate();
        }
    }
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::isCaretBlinkEnabled() const
{
    return d_blinkCaret;
}

//----------------------------------------------------------------------------//
float FalagardEditbox::getCaretBlinkTimeout() const
{
    return d_caretBlinkTimeout;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setCaretBlinkEnabled(bool enable)
{
    d_blinkCaret = enable;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setCaretBlinkTimeout(float seconds)
{
    d_caretBlinkTimeout = seconds;
}

//----------------------------------------------------------------------------//
void FalagardEditbox::setTextFormatting(const HorizontalTextFormatting format)
{
    if (isUnsupportedFormat(format))
        throw InvalidRequestException(
            "currently only HorizontalTextFormatting::LEFT_ALIGNED, "
            "HorizontalTextFormatting::RIGHT_ALIGNED and "
            "HorizontalTextFormatting::CENTRE_ALIGNED are accepted for Editbox formatting");

    d_textFormatting = format;
    d_window->invalidate();
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::isUnsupportedFormat(const HorizontalTextFormatting format)
{
    return !(format == HorizontalTextFormatting::LeftAligned ||
             format == HorizontalTextFormatting::RightAligned ||
             format == HorizontalTextFormatting::CentreAligned);
}

//----------------------------------------------------------------------------//
HorizontalTextFormatting FalagardEditbox::getTextFormatting() const
{
    return d_textFormatting;
}

//----------------------------------------------------------------------------//
bool FalagardEditbox::handleFontRenderSizeChange(const Font* const font)
{
    const bool res = WindowRenderer::handleFontRenderSizeChange(font);

    if (d_window->getFont() == font)
    {
        d_window->invalidate();
        return true;
    }

    return res;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
