/***********************************************************************
    filename:   FalMultiLineEditbox.cpp
    created:    Thu Jul 7 2005
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
#include "FalMultiLineEditbox.h"
#include "falagard/CEGUIFalWidgetLookManager.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include "CEGUIWindowManager.h"
#include "elements/CEGUIScrollbar.h"
#include "CEGUIPropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    const utf8 FalagardMultiLineEditbox::TypeName[] = "Falagard/MultiLineEditbox";
    const String FalagardMultiLineEditbox::UnselectedTextColourPropertyName( "NormalTextColour" );
    const String FalagardMultiLineEditbox::SelectedTextColourPropertyName( "SelectedTextColour" );
    const String FalagardMultiLineEditbox::ActiveSelectionColourPropertyName( "ActiveSelectionColour" );
    const String FalagardMultiLineEditbox::InactiveSelectionColourPropertyName( "InactiveSelectionColour" );

    FalagardMultiLineEditbox::FalagardMultiLineEditbox(const String& type) :
        MultiLineEditboxWindowRenderer(type)
    {
    }

    Rect FalagardMultiLineEditbox::getTextRenderArea(void) const
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        const WidgetLookFeel& wlf = getLookNFeel();
        bool v_visible = w->getVertScrollbar()->isVisible(true);
        bool h_visible = w->getHorzScrollbar()->isVisible(true);

        // if either of the scrollbars are visible, we might want to use another text rendering area
        if (v_visible || h_visible)
        {
            String area_name("TextArea");

            if (h_visible)
            {
                area_name += "H";
            }
            if (v_visible)
            {
                area_name += "V";
            }
            area_name += "Scroll";

            if (wlf.isNamedAreaDefined(area_name))
            {
                return wlf.getNamedArea(area_name).getArea().getPixelRect(*w);
            }
        }

        // default to plain TextArea
        return wlf.getNamedArea("TextArea").getArea().getPixelRect(*w);
    }

    void FalagardMultiLineEditbox::cacheEditboxBaseImagery()
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        const StateImagery* imagery;

        // get WidgetLookFeel for the assigned look.
        const WidgetLookFeel& wlf = getLookNFeel();
        // try and get imagery for our current state
        imagery = &wlf.getStateImagery(w->isDisabled() ? "Disabled" : (w->isReadOnly() ? "ReadOnly" : "Enabled"));
        // peform the rendering operation.
        imagery->render(*w);
    }

    void FalagardMultiLineEditbox::cacheCaratImagery(const Rect& textArea)
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        Font* fnt = w->getFont();

        // require a font so that we can calculate carat position.
        if (fnt)
        {
            // get line that carat is in
            size_t caratLine = w->getLineNumberFromIndex(w->getCaratIndex());

            const MultiLineEditbox::LineList& d_lines = w->getFormattedLines();

            // if carat line is valid.
            if (caratLine < d_lines.size())
            {
                // calculate pixel offsets to where carat should be drawn
                size_t caratLineIdx = w->getCaratIndex() - d_lines[caratLine].d_startIdx;
                float ypos = caratLine * fnt->getLineSpacing();
                float xpos = fnt->getTextExtent(w->getText().substr(d_lines[caratLine].d_startIdx, caratLineIdx));

                // get base offset to target layer for cursor.
                Renderer* renderer = System::getSingleton().getRenderer();
                float baseZ = renderer->getZLayer(7) - renderer->getCurrentZ();

                // get WidgetLookFeel for the assigned look.
                const WidgetLookFeel& wlf = getLookNFeel();
                // get carat imagery
                const ImagerySection& caratImagery = wlf.getImagerySection("Carat");

                // calculate finat destination area for carat
                Rect caratArea;
                caratArea.d_left    = textArea.d_left + xpos;
                caratArea.d_top     = textArea.d_top + ypos;
                caratArea.setWidth(caratImagery.getBoundingRect(*w).getSize().d_width);
                caratArea.setHeight(fnt->getLineSpacing());
                caratArea.offset(Point(-w->getHorzScrollbar()->getScrollPosition(), -w->getVertScrollbar()->getScrollPosition()));

                // cache the carat image for rendering.
                caratImagery.render(*w, caratArea, baseZ, 0, &textArea);
            }
        }
    }

    void FalagardMultiLineEditbox::render()
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        // render general frame and stuff before we handle the text itself
        cacheEditboxBaseImagery();

        // Render edit box text
        Rect textarea(getTextRenderArea());
        cacheTextLines(textarea);

        // draw caret
        if (w->hasInputFocus() && !w->isReadOnly())
            cacheCaratImagery(textarea);
    }

    void FalagardMultiLineEditbox::cacheTextLines(const Rect& dest_area)
    {
        MultiLineEditbox* w = (MultiLineEditbox*)d_window;
        // text is already formatted, we just grab the lines and render them with the required alignment.
        Rect drawArea(dest_area);
        float vertScrollPos = w->getVertScrollbar()->getScrollPosition();
        drawArea.offset(Point(-w->getHorzScrollbar()->getScrollPosition(), -vertScrollPos));

        Renderer* renderer = System::getSingleton().getRenderer();
        Font* fnt = w->getFont();

        if (fnt)
        {
            // get layers to use for rendering
            float textZ = renderer->getZLayer(4) - renderer->getCurrentZ();
            float selZ  = renderer->getZLayer(3) - renderer->getCurrentZ();

            // calculate final colours to use.
            ColourRect colours;
            float alpha = w->getEffectiveAlpha();
            colour normalTextCol = getUnselectedTextColour();
            normalTextCol.setAlpha(normalTextCol.getAlpha() * alpha);
            colour selectTextCol = getSelectedTextColour();
            selectTextCol.setAlpha(selectTextCol.getAlpha() * alpha);
            colour selectBrushCol = w->hasInputFocus() ? getActiveSelectionColour() :
                                                      getInactiveSelectionColour();
            selectBrushCol.setAlpha(selectBrushCol.getAlpha() * alpha);

            const MultiLineEditbox::LineList& d_lines = w->getFormattedLines();
            const size_t numLines = d_lines.size();

            // calculate the range of visible lines
            size_t sidx,eidx;
            sidx = static_cast<size_t>(vertScrollPos / fnt->getLineSpacing());
            eidx = 1 + sidx + static_cast<size_t>(dest_area.getHeight() / fnt->getLineSpacing());
            eidx = ceguimin(eidx, numLines);
            drawArea.d_top += fnt->getLineSpacing()*static_cast<float>(sidx);

            // for each formatted line.
            for (size_t i = sidx; i < eidx; ++i)
            {
                Rect lineRect(drawArea);
                const MultiLineEditbox::LineInfo& currLine = d_lines[i];
                String lineText(w->getText().substr(currLine.d_startIdx, currLine.d_length));

                // offset the font little down so that it's centered within its own spacing
                float old_top = lineRect.d_top;
                lineRect.d_top += (fnt->getLineSpacing() - fnt->getFontHeight()) * 0.5f;

                // if it is a simple 'no selection area' case
                if ((currLine.d_startIdx >= w->getSelectionEndIndex()) ||
                    ((currLine.d_startIdx + currLine.d_length) <= w->getSelectionStartIndex()) ||
                    (w->getSelectionBrushImage() == 0))
                {
                    colours.setColours(normalTextCol);
                    // render the complete line.
                    w->getRenderCache().cacheText(lineText, fnt, LeftAligned, lineRect, textZ, colours, &dest_area);
                }
                // we have at least some selection highlighting to do
                else
                {
                    // Start of actual rendering section.
                    String sect;
                    size_t sectIdx = 0, sectLen;
                    float selStartOffset = 0.0f, selAreaWidth = 0.0f;

                    // render any text prior to selected region of line.
                    if (currLine.d_startIdx < w->getSelectionStartIndex())
                    {
                        // calculate length of text section
                        sectLen = w->getSelectionStartIndex() - currLine.d_startIdx;

                        // get text for this section
                        sect = lineText.substr(sectIdx, sectLen);
                        sectIdx += sectLen;

                        // get the pixel offset to the beginning of the selection area highlight.
                        selStartOffset = fnt->getTextExtent(sect);

                        // draw this portion of the text
                        colours.setColours(normalTextCol);
                        w->getRenderCache().cacheText(sect, fnt, LeftAligned, lineRect, textZ, colours, &dest_area);

                        // set position ready for next portion of text
                        lineRect.d_left += selStartOffset;
                    }

                    // calculate the length of the selected section
                    sectLen = ceguimin(w->getSelectionEndIndex() - currLine.d_startIdx, currLine.d_length) - sectIdx;

                    // get the text for this section
                    sect = lineText.substr(sectIdx, sectLen);
                    sectIdx += sectLen;

                    // get the extent to use as the width of the selection area highlight
                    selAreaWidth = fnt->getTextExtent(sect);

                    // draw the text for this section
                    colours.setColours(selectTextCol);
                    w->getRenderCache().cacheText(sect, fnt, LeftAligned, lineRect, textZ, colours, &dest_area);

                    // render any text beyond selected region of line
                    if (sectIdx < currLine.d_length)
                    {
                        // update render position to the end of the selected area.
                        lineRect.d_left += selAreaWidth;

                        // calculate length of this section
                        sectLen = currLine.d_length - sectIdx;

                        // get the text for this section
                        sect = lineText.substr(sectIdx, sectLen);

                        // render the text for this section.
                        colours.setColours(normalTextCol);
                        w->getRenderCache().cacheText(sect, fnt, LeftAligned, lineRect, textZ, colours, &dest_area);
                    }

                    lineRect.d_top = old_top;

                    // calculate area for the selection brush on this line
                    lineRect.d_left = drawArea.d_left + selStartOffset;
                    lineRect.d_right = lineRect.d_left + selAreaWidth;
                    lineRect.d_bottom = lineRect.d_top + fnt->getLineSpacing();

                    // render the selection area brush for this line
                    colours.setColours(selectBrushCol);
                    w->getRenderCache().cacheImage(*w->getSelectionBrushImage(), lineRect, selZ, colours, &dest_area);
                }

                // update master position for next line in paragraph.
                drawArea.d_top += fnt->getLineSpacing();
            }
        }
    }

    colour FalagardMultiLineEditbox::getOptionalPropertyColour(const String& propertyName) const
    {
        if (d_window->isPropertyPresent(propertyName))
            return PropertyHelper::stringToColour(d_window->getProperty(propertyName));
        else
            return colour(0,0,0);
    }

    colour FalagardMultiLineEditbox::getUnselectedTextColour() const
    {
        return getOptionalPropertyColour(UnselectedTextColourPropertyName);
    }

    colour FalagardMultiLineEditbox::getSelectedTextColour() const
    {
        return getOptionalPropertyColour(SelectedTextColourPropertyName);
    }

    colour FalagardMultiLineEditbox::getActiveSelectionColour() const
    {
        return getOptionalPropertyColour(ActiveSelectionColourPropertyName);
    }

    colour FalagardMultiLineEditbox::getInactiveSelectionColour() const
    {
        return getOptionalPropertyColour(InactiveSelectionColourPropertyName);
    }

} // End of  CEGUI namespace section
