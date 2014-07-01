/***********************************************************************
created:    23/6/2012
author:     Lukas E Meindl
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
#ifndef _Sample_FontDemo_h_
#define _Sample_FontDemo_h_

#include "SampleBase.h"

#include <vector>
#include <map>

#include "CEGUI/ForwardRefs.h"

struct FontOption;

// Sample class
class FontDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext);

    void initialiseDemoFonts();
    void initialiseFontEditorInfoLabel();
    void deinitialise();

protected:
    void initialiseLangToTextMap();
    void initialiseFontSelector();
    void initialiseTextSelector();
    void initialiseFontCreator();

    void initialiseAutoScaleCombobox();
    void initialiseFontFileNameCombobox();
    void retrieveLoadedFontNames(bool areEditable);
    void retrieveFontFileNames();

    bool handleFontCreationButtonClicked(const CEGUI::EventArgs& e);

    bool handleFontEditButtonClicked(const CEGUI::EventArgs& e);
    bool handleFontSelectionChanged(const CEGUI::EventArgs& e);

    void checkIfEditButtonShouldBeDisabled(CEGUI::Font &font);
    bool findFontOption(CEGUI::String fontName);
    bool handleTextSelectionChanged(const CEGUI::EventArgs& e);
    bool handleTextMultiLineEditboxTextChanged(const CEGUI::EventArgs& e);
    bool handleFontFileNameSelectionChanged(const CEGUI::EventArgs& e);
    bool handleRenewFontNameButtonClicked(const CEGUI::EventArgs& e);

    void generateNewFontName();
    void changeFontSelectorFontSelection(const CEGUI::String& font);

    void initialiseAutoScaleOptionsArray();

    int getAutoScaleMode();

    CEGUI::GUIContext*      d_guiContext;

    CEGUI::Window*          d_root;

    CEGUI::Editbox*         d_fontNameEditbox;
    CEGUI::Combobox*        d_fontFileNameSelector;
    CEGUI::Editbox*         d_fontSizeEditbox;
    CEGUI::Combobox*        d_fontAutoScaleCombobox;
    CEGUI::ToggleButton*    d_fontAntiAliasCheckbox;
    CEGUI::PushButton*      d_fontCreationButton;
    CEGUI::PushButton*      d_fontEditButton;
    CEGUI::PushButton*      d_renewFontNameButton;
    CEGUI::Window*          d_fontEditorInfoLabel;

    CEGUI::Listbox*         d_fontSelector;
    CEGUI::Listbox*         d_textSelector;

    CEGUI::MultiLineEditbox*        d_textDisplayMultiLineEditbox;

    std::vector<CEGUI::String>              d_fontFileNameOptions;
    std::map<CEGUI::String, bool>  d_fontNameOptions;

    std::map<CEGUI::String, CEGUI::String>  d_languageToFontMap;
    std::map<CEGUI::String, CEGUI::String>  d_languageToTextMap;

    // A vector for Auto Scale Strings in the order of the AutoScale enum
    std::vector<CEGUI::String> d_autoScaleOptionsArray;
};


#endif