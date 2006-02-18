/************************************************************************
    filename:   FalEditbox.cpp
    created:    Sat Jun 25 2005
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
#include "FalEditbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUICoordConverter.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardEditbox::TypeName[] = "Falagard/Editbox";
    const String FalagardEditbox::UnselectedTextColourPropertyName( "NormalTextColour" );
    const String FalagardEditbox::SelectedTextColourPropertyName( "SelectedTextColour" );

    FalagardEditbox::FalagardEditbox(const String& type) :
        EditboxWindowRenderer(type),
        d_lastTextOffset(0)
    {
    }

    void FalagardEditbox::render()
    {
		Editbox* w = (Editbox*)d_window;
        const StateImagery* imagery;

        // draw container etc
        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for the approprite state.
        imagery = &wlf.getStateImagery(w->isDisabled() ? "Disabled" : (w->isReadOnly() ? "ReadOnly" : "Enabled"));

        // peform the rendering operation for the container.
        imagery->render(*w);

        // get destination area for text
        const Rect textArea(wlf.getNamedArea("TextArea").getArea().getPixelRect(*w));

        //
        // Required preliminary work for text rendering operations
        //
        const Font* font = w->getFont();

        // no font == no more rendering
        if (!font)
            return;

        // This will point to the final string to be used for rendering.  Useful because it means we
        // do not have to have duplicate code or be copying d_text for handling masked/unmasked text.
        String* editText;

        // Create a 'masked' version of the string if needed.
        String maskedText, windowText;
        if (w->isTextMasked())
        {
            maskedText.insert(0, w->getText().length(), w->getMaskCodePoint());
            editText = &maskedText;
        }
        // text not masked to editText will be the windows d_text String.
        else
        {
            windowText = w->getText();
            editText = &windowText;
        }

        // calculate best position to render text to ensure carat is always visible
        float textOffset;
        float extentToCarat = font->getTextExtent(editText->substr(0, w->getCaratIndex()));

        // get carat imagery
        const ImagerySection& caratImagery = wlf.getImagerySection("Carat");
        // store carat width
        float caratWidth = caratImagery.getBoundingRect(*w, textArea).getWidth();

        // if box is inactive
        if (!w->hasInputFocus())
        {
            textOffset = d_lastTextOffset;
        }
        // if carat is to the left of the box
        else if ((d_lastTextOffset + extentToCarat) < 0)
        {
            textOffset = -extentToCarat;
        }
        // if carat is off to the right.
        else if ((d_lastTextOffset + extentToCarat) >= (textArea.getWidth() - caratWidth))
        {
            textOffset = textArea.getWidth() - extentToCarat - caratWidth;
        }
        // else carat is already within the box
        else
        {
            textOffset = d_lastTextOffset;
        }

        ColourRect colours;
        float alpha_comp = w->getEffectiveAlpha();

        //
        // Draw label text
        //
        // setup initial rect for text formatting
        Rect text_part_rect(textArea);
        // allow for scroll position
        text_part_rect.d_left += textOffset;
        // centre text vertically within the defined text area
        text_part_rect.d_top += (textArea.getHeight() - font->getLineSpacing()) * 0.5f;

        // get unhighlighted text colour (saves accessing property twice)
        colour unselectedColour(getUnselectedTextColour());
        // draw pre-highlight text
        String sect = editText->substr(0, w->getSelectionStartIndex());
        colours.setColours(unselectedColour);
        colours.modulateAlpha(alpha_comp);
        w->getRenderCache().cacheText(sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

        // adjust rect for next section
        text_part_rect.d_left += font->getTextExtent(sect);

        // draw highlight text
        sect = editText->substr(w->getSelectionStartIndex(), w->getSelectionLength());
        colours.setColours(getSelectedTextColour());
        colours.modulateAlpha(alpha_comp);
        w->getRenderCache().cacheText(sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

        // adjust rect for next section
        text_part_rect.d_left += font->getTextExtent(sect);

        // draw post-highlight text
        sect = editText->substr(w->getSelectionEndIndex());
        colours.setColours(unselectedColour);
        colours.modulateAlpha(alpha_comp);
        w->getRenderCache().cacheText(sect, font, LeftAligned, text_part_rect, 0, colours, &textArea);

        // remember this for next time.
        d_lastTextOffset = textOffset;

        // see if the editbox is active or inactive.
        bool active = (!w->isReadOnly()) && w->hasInputFocus();

        //
        // Render selection imagery.
        //
        if (w->getSelectionLength() != 0)
        {
            // calculate required start and end offsets of selection imagery.
            float selStartOffset = font->getTextExtent(editText->substr(0, w->getSelectionStartIndex()));
            float selEndOffset   = font->getTextExtent(editText->substr(0, w->getSelectionEndIndex()));

            // calculate area for selection imagery.
            Rect hlarea(textArea);
            hlarea.d_left += textOffset + selStartOffset;
            hlarea.d_right = hlarea.d_left + (selEndOffset - selStartOffset);

            // render the selection imagery.
            wlf.getStateImagery(active ? "ActiveSelection" : "InactiveSelection").render(*w, hlarea, 0, &textArea);
        }

        //
        // Render carat
        //
        if (active)
        {
            Rect caratRect(textArea);
            caratRect.d_left += extentToCarat + textOffset;

            caratImagery.render(*w, caratRect, 0, 0, &textArea);
        }
    }

    size_t FalagardEditbox::getTextIndexFromPosition(const Point& pt) const
    {
        Editbox* w = (Editbox*)d_window;
        //
        // calculate final window position to be checked
        //
        float wndx = CoordConverter::screenToWindowX(*w, pt.d_x);

        wndx -= d_lastTextOffset;

        //
        // Return the proper index
        //
        if (w->isTextMasked())
        {
            return w->getFont()->getCharAtPixel(String(w->getText().length(), w->getMaskCodePoint()), wndx);
        }
        else
        {
            return w->getFont()->getCharAtPixel(w->getText(), wndx);
        }
    }

    colour FalagardEditbox::getOptionalPropertyColour(const String& propertyName) const
    {
        if (d_window->isPropertyPresent(propertyName))
            return PropertyHelper::stringToColour(d_window->getProperty(propertyName));
        else
            return colour(0,0,0);
    }

    colour FalagardEditbox::getUnselectedTextColour() const
    {
        return getOptionalPropertyColour(UnselectedTextColourPropertyName);
    }

    colour FalagardEditbox::getSelectedTextColour() const
    {
        return getOptionalPropertyColour(SelectedTextColourPropertyName);
    }

} // End of  CEGUI namespace section
