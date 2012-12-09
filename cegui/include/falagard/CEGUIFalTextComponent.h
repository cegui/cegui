/***********************************************************************
    filename:   CEGUIFalTextComponent.h
    created:    Sun Jun 19 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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

#include "CEGUIFalComponentBase.h"
#include "../CEGUIRenderedString.h"
#include "../CEGUIRefCounted.h"
#include "../CEGUIFormattedRenderedString.h"

#if defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates information for a text component.
    */
    class CEGUIEXPORT TextComponent : public FalagardComponentBase
    {
    public:
        /*!
        \brief
            Constructor
        */
        TextComponent();

        //! Destructor
        ~TextComponent();

        //! Copy constructor
        TextComponent(const TextComponent& obj);

        //! Assignment
        TextComponent& operator=(const TextComponent& other);

        /*!
        \brief
            Return the text object that will be rendered by this TextComponent.

        \return
            String object containing the text that will be rendered.
        */
        const String& getText() const;

        //! return text string with \e visual ordering of glyphs.
        const String& getTextVisual() const;

        /*!
        \brief
            Set the text that will be rendered by this TextComponent.

            Note that setting this to the empty string ("") will cause the text from the
            base window passed when rendering to be used instead.

        \param text
            String containing text to render, or "" to render text from window.

        \return
            Nothing.
        */
        void setText(const String& text);

        /*!
        \brief
            Return the name of the font to be used when rendering this TextComponent.

        \return
            String object containing the name of a font, or "" if the window font is to be used.
        */
        const String& getFont() const;

        /*!
        \brief
            Set the name of the font to be used when rendering this TextComponent.

            Note that setting this to the empty string ("") will cause the font from the
            base window passed when rendering to be used instead.

        \param font
            String containing name of a font

        \return
            Nothing.
        */
        void setFont(const String& font);

        /*!
        \brief
            Return the current vertical formatting setting for this TextComponent.

        \return
            One of the VerticalTextFormatting enumerated values.
        */
        VerticalTextFormatting getVerticalFormatting() const;

        /*!
        \brief
            Set the vertical formatting setting for this TextComponent.

        \param fmt
            One of the VerticalTextFormatting enumerated values.

        \return
            Nothing.
        */
        void setVerticalFormatting(VerticalTextFormatting fmt);

        /*!
        \brief
            Return the current horizontal formatting setting for this TextComponent.

        \return
            One of the HorizontalTextFormatting enumerated values.
        */
        HorizontalTextFormatting getHorizontalFormatting() const;

        /*!
        \brief
            Set the horizontal formatting setting for this TextComponent.

        \param fmt
            One of the HorizontalTextFormatting enumerated values.

        \return
            Nothing.
        */
        void setHorizontalFormatting(HorizontalTextFormatting fmt);

        /*!
        \brief
            Writes an xml representation of this TextComponent to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

        /*!
        \brief
            Return whether this TextComponent fetches it's text string via a property on the target window.

        \return
            - true if the text string comes via a Propery.
            - false if the text string is defined explicitly, or will come from the target window.
        */
        bool isTextFetchedFromProperty() const;

        /*!
        \brief
            Return the name of the property that will be used to determine the text string to render
            for this TextComponent.

        \return
            String object holding the name of a Propery.
        */
        const String& getTextPropertySource() const;

        /*!
        \brief
            Set the name of the property that will be used to determine the text string to render
            for this TextComponent.

        \param property
            String object holding the name of a Propery.  The property can contain any text string to render.

        \return
            Nothing.
        */
        void setTextPropertySource(const String& property);
        
        /*!
        \brief
            Return whether this TextComponent fetches it's font via a property on the target window.

        \return
            - true if the font comes via a Propery.
            - false if the font is defined explicitly, or will come from the target window.
        */
        bool isFontFetchedFromProperty() const;

        /*!
        \brief
            Return the name of the property that will be used to determine the font to use for rendering
            the text string for this TextComponent.

        \return
            String object holding the name of a Propery.
        */
        const String& getFontPropertySource() const;

        /*!
        \brief
            Set the name of the property that will be used to determine the font to use for rendering
            the text string of this TextComponent.

        \param property
            String object holding the name of a Propery.  The property should access a valid font name.

        \return
            Nothing.
        */
        void setFontPropertySource(const String& property);

        //! return the horizontal pixel extent of the formatted rendered string.
        float getHorizontalTextExtent() const;

        //! return the vertical pixel extent of the formatted rendered string.
        float getVerticalTextExtent() const;

    protected:
        // implemets abstract from base
        void render_impl(Window& srcWindow, Rect& destRect, const CEGUI::ColourRect* modColours, const Rect* clipper, bool clipToDisplay) const;
        //! helper to set up an appropriate FormattedRenderedString
        void setupStringFormatter(const Window& window,
                                  const RenderedString& rendered_string) const;
    private:
        String               d_textLogical;            //!< text rendered by this component.
        //! pointer to bidirection support object
        BiDiVisualMapping* d_bidiVisualMapping;
        //! whether bidi visual mapping has been updated since last text change.
        mutable bool d_bidiDataValid;
        //! RenderedString used when not using the one from the target Window.
        mutable RenderedString d_renderedString;
        //! FormattedRenderedString object that applies formatting to the string
        mutable RefCounted<FormattedRenderedString> d_formattedRenderedString;
        //! Tracks last used horizontal formatting (in order to detect changes)
        mutable HorizontalTextFormatting d_lastHorzFormatting;

        String               d_font;            //!< name of font to use.
        VerticalTextFormatting   d_vertFormatting;  //!< Vertical formatting to be applied when rendering the component.
        HorizontalTextFormatting d_horzFormatting;  //!< Horizontal formatting to be applied when rendering the component.
        String  d_textPropertyName;             //!< Name of the property to access to obtain the text string to render.
        String  d_fontPropertyName;             //!< Name of the property to access to obtain the font to use for rendering.
    };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalTextComponent_h_
