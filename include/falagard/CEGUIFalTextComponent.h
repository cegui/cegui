/************************************************************************
    filename:   CEGUIFalTextComponent.h
    created:    Sun Jun 19 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#ifndef _CEGUIFalTextComponent_h_
#define _CEGUIFalTextComponent_h_

#include "falagard/CEGUIFalDimensions.h"
#include "CEGUIWindow.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates information for a text component.
    */
    class TextComponent
    {
    public:
        /*!
        \brief
            Constructor
        */
        TextComponent();

        /*!
        \brief
            Render this TextComponent.  More correctly, the TextComponent is cached for rendering.

        \param srcWindow
            Window to use as the base for translating the TextComponent's ComponentArea into pixel values.

        \param base_z
            The z value to use for rendering the text.  Note that this is not the final z value to use, but
            some z offset from a currently unknown starting value.

        \param modColours
            ColourRect describing colours that are to be modulated with the TextComponent's stored colour values
            to calculate a set of 'final' colour values to be used.  May be 0.

        \return
            Nothing.
        */
        void render(Window& srcWindow, float base_z, const CEGUI::ColourRect* modColours = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Render this TextComponent.  More correctly, the TextComponent is cached for rendering.

        \param srcWindow
            Window to use as the base for translating the TextComponent's ComponentArea into pixel values.

        \param baseRect
            Rect to use as the base for translating the TextComponent's ComponentArea into pixel values.

        \param base_z
            The z value to use for rendering the text.  Note that this is not the final z value to use, but
            some z offset from a currently unknown starting value.

        \param modColours
            ColourRect describing colours that are to be modulated with the TextComponent's stored colour values
            to calculate a set of 'final' colour values to be used.  May be 0.

        \return
            Nothing.
        */
        void render(Window& srcWindow, const Rect& baseRect, float base_z, const CEGUI::ColourRect* modColours = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Return the ComponentArea of this TextComponent.

        \return
            ComponentArea object describing the TextComponent's current target area.
        */
        const ComponentArea& getComponentArea() const;

        /*!
        \brief
            Set the TextComponent's ComponentArea.

        \param area
            ComponentArea object describing a new target area for the TextComponent.

        \return
            Nothing.
        */
        void setComponentArea(const ComponentArea& area);

        /*!
        \brief
            Return the text object that will be rendered by this TextComponent.

        \return
            String object containing the text that will be rendered.
        */
        const String& getText() const;

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
            Return the ColourRect set for use by this TextComponent.

        \return
            ColourRect object holding the colours currently in use by this TextComponent.
        */
        const ColourRect& getColours() const;

        /*!
        \brief
            Set the colours to be used by this TextComponent.

        \param cols
            ColourRect object describing the colours to be used by this TextComponent.
        */
        void setColours(const ColourRect& cols);

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
            Set the name of the property where colour values can be obtained.

        \param property
            String containing the name of the property.

        \return
            Nothing.
        */
        void setColoursPropertySource(const String& property);

        /*!
        \brief
            Set whether the colours property source represents a full ColourRect.

        \param setting
            - true if the colours property will access a ColourRect object.
            - false if the colours property will access a colour object.

        \return
            Nothing.
        */
        void setColoursPropertyIsColourRect(bool setting = true);

    protected:
        /*!
        \brief
            Helper method to initialise a ColourRect with appropriate values according to the way the
            TextComponent is set up.

            This will try and get values from multiple places:
                - a property attached to \a wnd
                - or the integral d_colours value.
        */
        void initColoursRect(const Window& wnd, ColourRect& cr) const;

        /*!
        \brief
            Method to perform the main render caching work.
        */
        void render_impl(Window& srcWindow, Rect& destRect, float base_z, const CEGUI::ColourRect* modColours, const Rect* clipper, bool clipToDisplay) const;

    private:
        ComponentArea        d_area;            //!< Destination area for this component.
        String               d_text;            //!< text rendered by this component.
        String               d_font;            //!< name of font to use.
        ColourRect           d_colours;         //!< base colours to be applied when rendering the component.
        VerticalTextFormatting   d_vertFormatting;  //!< Vertical formatting to be applied when rendering the component.
        HorizontalTextFormatting d_horzFormatting;  //!< Horizontal formatting to be applied when rendering the component.
        String              d_colourPropertyName;   //!< name of property to fetch colours from.
        bool                d_colourProperyIsRect;  //!< true if the colour property will fetch a full ColourRect.
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalTextComponent_h_
