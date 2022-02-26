/***********************************************************************
    created:    Sun Jun 19 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIFalTextComponent_h_
#define _CEGUIFalTextComponent_h_

#include "./ComponentBase.h"
#include "CEGUI/text/RenderedText.h"
#include "CEGUI/falagard/FormattingSetting.h"

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! \brief Class that encapsulates information for a Falagard text component.
class CEGUIEXPORT TextComponent : public FalagardComponentBase
{
public:

    TextComponent();
    TextComponent(const TextComponent& obj) = delete;
    TextComponent(TextComponent&& obj) noexcept;
    TextComponent& operator =(const TextComponent& other) = delete;
    TextComponent& operator =(TextComponent&& other) noexcept;
    ~TextComponent() override;

    /*!
    \brief
        Return the text set for this TextComponent.

    \note
        This returns the text string set directly to the TextComponent,
        which may or may not be the actual string that will be used -
        since the actual string may be sourced from a property or the main
        text string from a window that the TextComponent is rendered to.
        To get the actual string, call the getEffectiveText function
        instead.
    */
    const String& getText() const { return d_text; }

    /*
    \brief
        Return a copy of the actual text string that will be used when
        rendering this TextComponent.
    */
    String getEffectiveText(const Window& wnd) const;

    /*!
    \brief
        Set the text string for this TextComponent.

    \note
        Setting this string may not set the actual string that will be used
        when rendering the TextComponent.  The acutal string used will
        depend upon whether a text source property is set and whether this
        string is set to the empty string or not.

    \param text
        String containing text to set on the TextComponent.
    */
    void setText(const String& text) { d_text = text; d_textFromProperty = false; }

    /*!
    \brief
        Return the name of the font set to be used when rendering this
        TextComponent.

    \note
        This returns the name of the font set directly to the TextComponent,
        which may or may not be the actual font that will be used -
        since the actual font may be sourced from a property or the main
        font setting on a window that the TextComponent is rendered to, or
        the default font. To get the actual font name that will be used,
        call the getEffectiveFont function instead.

    \return
        String object containing the name of a font
    */
    const String& getFont() const { return d_font; }

    /*!
    \brief
        Set the name of a font to be used when rendering this TextComponent.

    \note
        Setting this may not set the actual font that will be used
        when rendering the TextComponent.  The acutal font used will
        depend upon whether a font source property is set and whether the
        font name set here is set to the empty string or not.

    \param font
        String containing name of a font
    */
    void setFont(const String& font) { d_font = font; d_fontFromProperty = false; }

    /*!
    \brief
        Return the current horizontal formatting setting for this TextComponent.

    \return
        One of the HorizontalTextFormatting enumerated values.
    */
    HorizontalTextFormatting getHorizontalFormatting(const Window& wnd) const { return d_horzFormatting.get(wnd); }

    /*!
    \brief
        Directly returns the horizontal formatting which was set for the component.

    \return
        The HorizontalTextFormatting enum.
    */
    HorizontalTextFormatting getHorizontalFormattingFromComponent() const { return d_horzFormatting.getValue(); }

    /*!
    \brief
        Set the horizontal formatting setting for this TextComponent.

    \param fmt
        One of the HorizontalTextFormatting enumerated values.
    */
    void setHorizontalFormatting(HorizontalTextFormatting fmt) { d_horzFormatting.set(fmt); }

    /*!
    \brief
        Returns the name of the property that will be used to obtain the horizontal
        formatting to use for this component or an empty string if none is set.

    \return
        A String containing the name of the property
    */
    const String& getHorizontalFormattingPropertySource() const { return d_horzFormatting.getPropertySource(); }

    /*!
    \brief
        Set the name of a property that will be used to obtain the horizontal
        formatting to use for this component.
    */
    void setHorizontalFormattingPropertySource(const String& property_name) { d_horzFormatting.setPropertySource(property_name); }

    /*!
    \brief
        Return the current vertical formatting setting for this TextComponent.

    \return
        One of the VerticalTextFormatting enumerated values.
    */
    VerticalTextFormatting getVerticalFormatting(const Window& wnd) const { return d_vertFormatting.get(wnd); }

    /*!
    \brief
        Directly returns the vertical formatting which was set for the component.

    \return
        The VerticalTextFormatting enum.
    */
    VerticalTextFormatting getVerticalFormattingFromComponent() const { return d_vertFormatting.getValue(); }

    /*!
    \brief
        Set the vertical formatting setting for this TextComponent.

    \param fmt
        One of the VerticalTextFormatting enumerated values.
    */
    void setVerticalFormatting(VerticalTextFormatting fmt) { d_vertFormatting.set(fmt); }

    /*!
    \brief
        Returns the name of the property that will be used to obtain the vertical
        formatting to use for this component or an empty string if none is set.

    \return
        A String containing the name of the property
    */
    const String& getVerticalFormattingPropertySource() const { return d_vertFormatting.getPropertySource(); }

    /*!
    \brief
        Set the name of a property that will be used to obtain the vertical
        formatting to use for this component.
    */
    void setVerticalFormattingPropertySource(const String& property_name) { d_vertFormatting.setPropertySource(property_name); }

    DefaultParagraphDirection getParagraphDir(const Window& wnd) const { return d_paragraphDir.get(wnd); }
    DefaultParagraphDirection getParagraphDirFromComponent() const { return d_paragraphDir.getValue(); }
    void setParagraphDir(DefaultParagraphDirection dir) { d_paragraphDir.set(dir); }
    const String& getParagraphDirPropertySource() const { return d_paragraphDir.getPropertySource(); }
    void setParagraphDirPropertySource(const String& property_name) { d_paragraphDir.setPropertySource(property_name); }

    /*!
    \brief
        Writes an xml representation of this TextComponent to \a out_stream.

    \param xml_stream
        Stream where xml data should be output.
    */
    void writeXMLToStream(XMLSerializer& xml_stream) const;

    /*!
    \brief
        Return whether this TextComponent fetches its text string via a property on the target window.

    \return
        - true if the text string comes via a Propery.
        - false if the text string is defined explicitly, or will come from the target window.
    */
    bool isTextFetchedFromProperty() const { return d_textFromProperty; }

    /*!
    \brief
        Return the name of the property that will be used to determine the text string to render
        for this TextComponent.

    \return
        String object holding the name of a Propery.
    */
    const String& getTextPropertySource() const { return d_textFromProperty ? d_text : String::GetEmpty(); }

    /*!
    \brief
        Set the name of the property that will be used to determine the text string to render
        for this TextComponent.

    \param property
        String object holding the name of a Propery.  The property can contain any text string to render.
    */
    void setTextPropertySource(const String& property);
        
    /*!
    \brief
        Return whether this TextComponent fetches its font via a property on the target window.

    \return
        - true if the font comes via a Propery.
        - false if the font is defined explicitly, or will come from the target window.
    */
    bool isFontFetchedFromProperty() const { return d_fontFromProperty; }

    /*!
    \brief
        Return the name of the property that will be used to determine the font to use for rendering
        the text string for this TextComponent.

    \return
        String object holding the name of a Propery.
    */
    const String& getFontPropertySource() const { return d_fontFromProperty ? d_font : String::GetEmpty(); }

    void setWordWrapEnabled(bool wrap) { d_wordWrapProperty.clear(); d_wordWrap = wrap; }
    void setWordWrapProperty(const String& name) { d_wordWrapProperty = name; }

    /*!
    \brief
        Set the name of the property that will be used to determine the font to use for rendering
        the text string of this TextComponent.

    \param property
        String object holding the name of a Propery.  The property should access a valid font name.
    */
    void setFontPropertySource(const String& property);

    //! return the pixel extent of the formatted rendered string.
    const Sizef& getTextExtent(const Window& window) const;

    virtual bool handleFontRenderSizeChange(Window& window, const Font* font) const override;

protected:

    void addImageRenderGeometryToWindow_impl(Window& srcWindow, Rectf& destRect,
        const CEGUI::ColourRect* modColours, const Rectf* clipper) const override;

    Font* getFontObject(const Window& window) const;
    TextParser* getTextParser(const Window& window) const;
    void updateRenderedText(const Window& srcWindow, const Sizef& size) const;

private:

    mutable RenderedText d_renderedText;

    // Cache for avoiding redundant calulations
    mutable const Font* d_lastFont = nullptr;
    mutable const TextParser* d_lastParser = nullptr;
    mutable String d_lastText;

    String d_text; //!< text rendered by this component, either string or property name.
    String d_font; //!< font to use, either font name or property name.

    String d_wordWrapProperty;

    // Cache for avoiding redundant calulations
    mutable DefaultParagraphDirection d_lastBidiDir = DefaultParagraphDirection::Automatic;

    FormattingSetting<VerticalTextFormatting> d_vertFormatting = VerticalTextFormatting::TopAligned;
    FormattingSetting<HorizontalTextFormatting> d_horzFormatting = HorizontalTextFormatting::LeftAligned;
    FormattingSetting<DefaultParagraphDirection> d_paragraphDir = DefaultParagraphDirection::LeftToRight;

    bool d_wordWrap = false;
    bool d_fontFromProperty = false; //!< d_font is a property name in a source window
    bool d_textFromProperty = false; //!< d_text is a property name in a source window
};

}

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif
