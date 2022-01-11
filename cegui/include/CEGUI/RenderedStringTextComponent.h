/***********************************************************************
    created:    25/05/2009
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
#ifndef _CEGUIRenderedStringTextComponent_h_
#define _CEGUIRenderedStringTextComponent_h_

#include "CEGUI/RenderedStringComponent.h"
#include "CEGUI/ColourRect.h"
#include "CEGUI/String.h"
#include "CEGUI/DefaultParagraphDirection.h"
#include <vector>

namespace CEGUI
{

//! String component that draws a text.
class CEGUIEXPORT RenderedStringTextComponent : public RenderedStringComponent
{
public:

    RenderedStringTextComponent() = default;
    RenderedStringTextComponent(const String& text, const Font* font = nullptr);

    //! Set the text to be rendered by this component.
    void setText(const String& text) { d_text = text; }
    //! return the text that will be rendered by this component
    const String& getText() const { return d_text; }
    //! set the font to use when rendering the text.
    void setFont(const Font* font) { d_font = font; }
    //! return the font set to be used.  If 0 the default font will be used.
    const Font* getFont() const { return d_font; }
    //! Set the colour values used when rendering this component.
    void setColours(const ColourRect& cr) { d_colours = cr; }
    //! Set the colour values used when rendering this component.
    void setColours(const Colour& c) { d_colours.setColours(c); }
    //! return the ColourRect object used when drawing this component.
    const ColourRect& getColours() const { return d_colours; }
    //! set the default paragraph direction to use when rendering the text.
    void setDefaultParagraphDirection(DefaultParagraphDirection dir) { d_defaultParagraphDir = dir; }
    //! return the default paragraph direction to use when rendering the text.
    DefaultParagraphDirection getDefaultParagraphDirection() const { return d_defaultParagraphDir; }

    // implementation of abstract base interface
    std::vector<GeometryBuffer*> createRenderGeometry(
        const Window* ref_wnd,
        const glm::vec2& position, const ColourRect* mod_colours,
        const Rectf* clip_rect, const float vertical_space,
        const float space_extra) const override;
    Sizef getPixelSize(const Window* ref_wnd) const override;
    bool canSplit() const override { return d_text.length() > 1; }
    RenderedStringTextComponent* split(const Window* ref_wnd,
      float split_point, bool first_component, bool& was_word_split) override;
    RenderedStringTextComponent* clone() const override;
    size_t getSpaceCount() const override;
    void setSelection(const Window* ref_wnd,
                      const float start, const float end) override;

protected:
    const Font* getEffectiveFont(const Window* window) const;
    void handleFormattingOptions(const Window* ref_wnd, const float vertical_space, glm::vec2& final_pos) const;
    void createSelectionRenderGeometry(const glm::vec2& position, const Rectf* clip_rect, const float vertical_space, const Font* fnt) const;
    static size_t getNextTokenLength(const String& text, size_t start_idx);

    //! text string drawn by the component.
    String d_text;
    //! Font to use for text rendering, 0 for system default.
    const Font* d_font = nullptr;
    //! ColourRect object describing the colours to use when rendering.
    ColourRect d_colours = 0xFFFFFFFF;
    //! last set selection
    size_t d_selectionStart = 0;
    size_t d_selectionLength = 0;
    DefaultParagraphDirection d_defaultParagraphDir = DefaultParagraphDirection::Automatic;
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIRenderedStringTextComponent_h_
