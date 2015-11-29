/***********************************************************************
created:    17/6/2006
author:     Paul D Turner
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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

// NOTE: if you don't have Asian fonts installed, the Japanese and Korean
// sample texts might show up unreadable in this source file. The sample
// should look okay though when running.

#include "Sample_FontDemo.h"
#include "LanguageStrings.h"
#include "CEGUI/CEGUI.h"

using namespace CEGUI;
using namespace LanguageStrings;

struct FontOption
{
    CEGUI::String   d_font;
    bool            d_isEditable;
};

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public ListboxTextItem
{
public:
    MyListItem(const String& text, CEGUI::uint item_id = 0) :
      ListboxTextItem(text, item_id)
      {
          setSelectionBrushImage("Vanilla-Images/GenericBrush");
      }
};


bool FontDemo::initialise(CEGUI::GUIContext* guiContext)
{
    d_usedFiles = CEGUI::String(__FILE__);

    d_guiContext = guiContext;

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    d_guiContext->getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // Create a custom font which we use to draw the list items. This custom
    // font won't get effected by the scaler and such.
    FontManager& fontManager(FontManager::getSingleton());
    CEGUI::Font& font(fontManager.createFromFile("DejaVuSans-12.font"));
    // Set it as the default
    d_guiContext->setDefaultFont(&font);

    // load all the fonts (if they are not loaded yet)
    fontManager.createAll("*.font", "fonts");

    // Fill list with all loaded font names
    retrieveLoadedFontNames(false);

    // Now that we know about all existing fonts we will initialise
    // the fonts we wanna use for this demo hardcoded
    initialiseDemoFonts();

    // Fill list with all new and thus editable font names
    retrieveLoadedFontNames(true);

    // Fill list with all available font type file names
    retrieveFontFileNames();

    // set tooltip styles (by default there is none)
    d_guiContext->setDefaultTooltipType("Vanilla/Tooltip");

    // Load the GUI layout and attach it to the context as root window
    d_root = winMgr.loadLayoutFromFile("FontDemo.layout");
    d_guiContext->setRootWindow(d_root);

    //Here we create a font and apply it to the renew font name button
    CEGUI::Font& buttonFont = fontManager.createFreeTypeFont("DejaVuSans-14", 14.f, true, "DejaVuSans.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));
    d_renewFontNameButton =  static_cast<CEGUI::PushButton*>(d_root->getChild("FontDemoWindow/FontCreator/RenewNameButton"));
    d_renewFontNameButton->setFont(&buttonFont);

    d_root->getChild("FontDemoWindow/FontCreator/FontSizeLabel");

    //Subscribe click event for renewing font name based on font file name and size
    d_renewFontNameButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontDemo::handleRenewFontNameButtonClicked, this));

    // Get the editbox where we display the text
    d_textDisplayMultiLineEditbox = static_cast<CEGUI::MultiLineEditbox*>(d_root->getChild("FontDemoWindow/MultiLineTextWindow"));
    d_textDisplayMultiLineEditbox->subscribeEvent(CEGUI::MultiLineEditbox::EventTextChanged, Event::Subscriber(&FontDemo::handleTextMultiLineEditboxTextChanged, this));

    //Get the font editor info label and apply an animation to it for blending out
    initialiseFontEditorInfoLabel();

    //Initialise the options we have for setting autoScale for fonts as strings
    initialiseAutoScaleOptionsArray();

    //Initialise the Map that connects Language/Text strings with the actual text we will display in the MultiLineEditbox
    initialiseLangToTextMap();

    // Initialise the font creator window + its subwindows
    initialiseFontCreator();

    // Initialise the widget to select fonts and its items
    initialiseFontSelector();

    // Initialise the widget to select the different language texts the relative items for each
    initialiseTextSelector();

    //Subscribe font selection event
    d_fontSelector->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, Event::Subscriber(&FontDemo::handleFontSelectionChanged, this));

    d_textSelector->setItemSelectState(size_t(0), true);

    return true;
}

// method to perform any required cleanup operations.
void FontDemo::deinitialise()
{
}

bool FontDemo::handleFontCreationButtonClicked(const EventArgs&)
{
    FontManager& fontMgr(FontManager::getSingleton());

    CEGUI::String fontName = d_fontNameEditbox->getText();
    bool fontNameExists = fontMgr.isDefined(fontName);
    if(fontNameExists ||fontName.size() == 0)
    {
        d_fontEditorInfoLabel->setText("Font name already in use.");
        return true;
    }

    CEGUI::String fontFileName = d_fontFileNameSelector->getSelectedItem()->getText();

    CEGUI::String fontSizeString = d_fontSizeEditbox->getText();
    float fontSize = CEGUI::PropertyHelper<float>::fromString(fontSizeString);
    if(fontSize == 0.0f)
        return true;

    bool antiAlias = d_fontAntiAliasCheckbox->isSelected();

    
    AutoScaledMode autoScaleMode = static_cast<AutoScaledMode>(getAutoScaleMode());


    String::size_type pos = fontFileName.rfind(".imageset");
    if(pos != String::npos)
    {
        fontMgr.createPixmapFont(fontName, fontFileName, Font::getDefaultResourceGroup(), autoScaleMode,
            CEGUI::Sizef(1280.0f, 720.0f), XREA_THROW);
    }
    else
    {
        fontMgr.createFreeTypeFont(fontName, fontSize, antiAlias, fontFileName, Font::getDefaultResourceGroup(), autoScaleMode, 
            CEGUI::Sizef(1280.0f, 720.0f), XREA_THROW);
    }

    ListboxItem* item = new MyListItem(fontName, 0);
    d_fontSelector->addItem(item);
    d_fontSelector->setItemSelectState(item, true);

    return true;
}


bool FontDemo::handleFontEditButtonClicked(const EventArgs&)
{
    FontManager& fontMgr(FontManager::getSingleton());

    CEGUI::String fontName = d_fontNameEditbox->getText();
    bool fontNameExists = fontMgr.isDefined(fontName);
    if(!fontNameExists)
    {
        d_fontEditorInfoLabel->setText("A font with this name does not exist.");
        return true;
    }

    CEGUI::Font& font(fontMgr.get(fontName));


    if(font.isPropertyPresent("PointSize"))
    {
        CEGUI::String fontSizeString = d_fontSizeEditbox->getText();
        float fontSize = CEGUI::PropertyHelper<float>::fromString(fontSizeString);
        if(fontSize != 0.0f)
            font.setProperty<float>("PointSize", fontSize);
    }

    if(font.isPropertyPresent("Antialiased"))
    {
        bool antiAlias = d_fontAntiAliasCheckbox->isSelected();
        font.setProperty("Antialiased", PropertyHelper<bool>::toString(antiAlias));
    }


    AutoScaledMode autoScaleMode = static_cast<AutoScaledMode>(getAutoScaleMode());
    font.setAutoScaled(autoScaleMode);

    return true;
}

bool FontDemo::handleFontSelectionChanged(const EventArgs&)
{
    //Change font of the selected language/text sample
    if(d_textSelector->getFirstSelectedItem() && d_fontSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getID();
        
        d_languageToFontMap[LangList[index].Language] = d_fontSelector->getFirstSelectedItem()->getText();
    }

    //Change the font creatore fields according to the font
    if(d_fontSelector->getFirstSelectedItem())
    {
        CEGUI::String fontName = d_fontSelector->getFirstSelectedItem()->getText();

        if(FontManager::getSingleton().isDefined(fontName))
        {
            CEGUI::Font& font(FontManager::getSingleton().get(fontName));

            d_textDisplayMultiLineEditbox->setFont(&font);

            d_fontNameEditbox->setText(font.getName());

            AutoScaledMode autoScaleMode = font.getAutoScaled();
            CEGUI::String autoScaleString = d_autoScaleOptionsArray.at(autoScaleMode);
            d_fontAutoScaleCombobox->getEditbox()->setText(autoScaleString);
            d_fontAutoScaleCombobox->selectListItemWithEditboxText();

            if(font.isPropertyPresent("Antialiased"))
            {
                bool fontAntiAlias = font.getProperty<bool>("Antialiased");
                d_fontAntiAliasCheckbox->enable();
                d_fontAntiAliasCheckbox->setSelected(fontAntiAlias);
            }
            else
            {
                d_fontAntiAliasCheckbox->disable();
            }

            if(font.isPropertyPresent("PointSize"))
            {
                CEGUI::String fontPointSize = font.getProperty("PointSize");
                d_fontSizeEditbox->enable();
                d_fontSizeEditbox->setText(fontPointSize);
            }
            else
            {
                d_fontSizeEditbox->disable();
            }

            d_fontFileNameSelector->setText(font.getFileName());

            checkIfEditButtonShouldBeDisabled(font);


            //Change the font colour for pixmap fonts
            if(font.getTypeName().compare(Font_xmlHandler::FontTypePixmap) == 0)
                d_textDisplayMultiLineEditbox->setProperty<CEGUI::ColourRect>("NormalTextColour", CEGUI::ColourRect(CEGUI::Colour(1.0f, 1.0f, 1.0f, 1.0f)));
            else
                d_textDisplayMultiLineEditbox->setProperty<CEGUI::ColourRect>("NormalTextColour", CEGUI::ColourRect(CEGUI::Colour(0.0f, 0.0f, 0.0f, 1.0f)));

        }
    }

    return true;
}

bool FontDemo::handleTextSelectionChanged(const EventArgs&)
{
    if(d_textSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getID();

        d_textDisplayMultiLineEditbox->setText(d_languageToTextMap[LangList[index].Language]);

        changeFontSelectorFontSelection(d_languageToFontMap[LangList[index].Language]);
    }

    return true;
}

bool FontDemo::handleTextMultiLineEditboxTextChanged(const EventArgs&)
{
    if(d_textSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getID();

        d_languageToTextMap[LangList[index].Language] = d_textDisplayMultiLineEditbox->getText();
    }

    return true;
}

bool FontDemo::handleFontFileNameSelectionChanged(const EventArgs&)
{
    generateNewFontName();

    return true;
}

bool FontDemo::handleRenewFontNameButtonClicked(const EventArgs&)
{
    generateNewFontName();

    return true;
}

void FontDemo::initialiseAutoScaleOptionsArray()
{
    //AutoScale options in the enum order
    d_autoScaleOptionsArray.push_back("Disabled");
    d_autoScaleOptionsArray.push_back("Vertical");
    d_autoScaleOptionsArray.push_back("Horizontal");
    d_autoScaleOptionsArray.push_back("Minimum");
    d_autoScaleOptionsArray.push_back("Maximum");
    d_autoScaleOptionsArray.push_back("Both");
}


void FontDemo::retrieveLoadedFontNames(bool areEditable)
{
    FontManager& fontManager(FontManager::getSingleton());
    FontManager::FontIterator fi = fontManager.getIterator();

    while (!fi.isAtEnd())
    {
        CEGUI::Font& font(fontManager.get(fi.getCurrentKey()));

        if(d_fontNameOptions.find(font.getName()) == d_fontNameOptions.end())
        {
            d_fontNameOptions[font.getName()] = areEditable;
        }
        ++fi;
    }
}


void FontDemo::retrieveFontFileNames()
{
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.ttf", Font::getDefaultResourceGroup());
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.pcf", Font::getDefaultResourceGroup());
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.otf", Font::getDefaultResourceGroup());
}

void FontDemo::initialiseFontFileNameCombobox()
{
    //Select a font file name if any are present
    if(d_fontFileNameOptions.size() > 0)
    {
        // Add the font file names to the listbox
        for(unsigned int i = 0; i < d_fontFileNameOptions.size(); i++)
        {
            const CEGUI::String& fileName(d_fontFileNameOptions[i]);

            d_fontFileNameSelector->addItem(new MyListItem(fileName, i));  
        }

        d_fontFileNameSelector->getListboxItemFromIndex(0)->setSelected(true);
        d_fontFileNameSelector->getEditbox()->setText(d_fontFileNameSelector->getListboxItemFromIndex(0)->getText());
    }
}

void FontDemo::initialiseFontCreator()
{
    d_fontFileNameSelector = static_cast<CEGUI::Combobox*>(d_root->getChild("FontDemoWindow/FontCreator/FontFileCombobox"));
    d_fontNameEditbox = static_cast<CEGUI::Editbox*>(d_root->getChild("FontDemoWindow/FontCreator/FontNameEditbox"));
    d_fontSizeEditbox = static_cast<CEGUI::Editbox*>(d_root->getChild("FontDemoWindow/FontCreator/FontSizeEditbox"));
    d_fontAutoScaleCombobox = static_cast<CEGUI::Combobox*>(d_root->getChild("FontDemoWindow/FontCreator/AutoScaleCombobox"));
    d_fontAntiAliasCheckbox = static_cast<CEGUI::ToggleButton*>(d_root->getChild("FontDemoWindow/FontCreator/AntiAliasingCheckbox"));
    d_fontCreationButton = static_cast<CEGUI::PushButton*>(d_root->getChild("FontDemoWindow/FontCreator/CreationButton"));
    d_fontEditButton = static_cast<CEGUI::PushButton*>(d_root->getChild("FontDemoWindow/FontCreator/EditButton"));

    d_fontFileNameSelector->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, Event::Subscriber(&FontDemo::handleFontFileNameSelectionChanged, this));

    d_fontCreationButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontDemo::handleFontCreationButtonClicked, this));
    d_fontEditButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontDemo::handleFontEditButtonClicked, this));

    initialiseFontFileNameCombobox();
    initialiseAutoScaleCombobox();

}

void FontDemo::initialiseFontSelector()
{
    d_fontSelector = static_cast<CEGUI::Listbox*>(d_root->getChild("FontDemoWindow/FontSelector"));

    d_fontSelector->setSortingEnabled(true);

    //Select a font file name if any are present
    if(d_fontNameOptions.size() > 0)
    {
        std::map<String, bool>::iterator iter = d_fontNameOptions.begin();
        for(; iter != d_fontNameOptions.end(); ++iter)
        {
            std::pair<const CEGUI::String, bool>& currentPair = *iter;
            d_fontSelector->addItem(new MyListItem(currentPair.first, 0));
        }
    }

    d_fontSelector->handleUpdatedItemData();
}

void FontDemo::initialiseTextSelector()
{
    d_textSelector = static_cast<CEGUI::Listbox*>(d_root->getChild("FontDemoWindow/TextSelector"));
    d_textSelector->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, Event::Subscriber(&FontDemo::handleTextSelectionChanged, this));

    for(unsigned int i = 0; i < LangListSize; ++i)
    {
         d_textSelector->addItem(new MyListItem(LangList[i].Language, i)); 
         d_languageToFontMap[LangList[i].Language] = LangList[i].Font;
    }
}

void FontDemo::changeFontSelectorFontSelection(const CEGUI::String& font)
{
    while(d_fontSelector->getFirstSelectedItem())
    {
        d_fontSelector->setItemSelectState(d_fontSelector->getFirstSelectedItem(), false);
    }

    unsigned int itemCount = d_fontSelector->getItemCount();
    for(unsigned int i = 0; i < itemCount; ++i)
    {
        CEGUI::ListboxItem* item = d_fontSelector->getListboxItemFromIndex(i);

        CEGUI::String itemFontName = item->getText();

        if(itemFontName.compare(font) == 0)
        {
            d_fontSelector->setItemSelectState(item, true);
            return;
        }
    }
}

void FontDemo::initialiseLangToTextMap()
{
    for(unsigned int i = 0; i < LangListSize; ++i)
    {
        d_languageToTextMap[LangList[i].Language] = LangList[i].Text;
    }
}

void FontDemo::generateNewFontName()
{
    CEGUI::String fileName = d_fontFileNameSelector->getText();
    CEGUI::String pointSize = d_fontSizeEditbox->getText();

    CEGUI::String fontName = fileName.substr(0, fileName.find_last_of("."));
    fontName += "-" + pointSize;

    d_fontNameEditbox->setText(fontName);
}

void FontDemo::initialiseAutoScaleCombobox()
{
   for(unsigned int i = 0; i < d_autoScaleOptionsArray.size(); ++i)
    {
        CEGUI::String itemText = d_autoScaleOptionsArray.at(i);
        d_fontAutoScaleCombobox->addItem(new MyListItem(itemText, i));
    }
}

int FontDemo::getAutoScaleMode()
{
    CEGUI::String autoScaleString = d_fontAutoScaleCombobox->getSelectedItem()->getText();

    for(unsigned int i = 0; i < d_autoScaleOptionsArray.size(); ++i)
    {
        if(autoScaleString.compare(d_autoScaleOptionsArray.at(i)) == 0)
            return i;
    }

    return 0;
}

bool FontDemo::findFontOption(CEGUI::String fontName)
{
    std::map<String, bool>::iterator iter = d_fontNameOptions.begin();
    for(; iter != d_fontNameOptions.end(); ++iter)
    {
        std::pair<const CEGUI::String, bool>& currentPair = *iter;

        if(currentPair.first.compare(fontName) == 0)
            return currentPair.second; 
    }
    return true;
}

void FontDemo::initialiseFontEditorInfoLabel()
{
    d_fontEditorInfoLabel = d_root->getChild("FontDemoWindow/FontCreator/InfoLabel");

    //Create fadeout animation
    CEGUI::Animation* anim = CEGUI::AnimationManager::getSingleton().createAnimation("ErrorDisplayAndFadeout");
    anim->setDuration(5.f);
    anim->setReplayMode(CEGUI::Animation::RM_Once);

    // this affector changes YRotation and interpolates keyframes with float interpolator
    CEGUI::Affector* affector = anim->createAffector("Alpha", "float");
    affector->createKeyFrame(0.0f, "1.0");
    affector->createKeyFrame(4.0f, "1.0");
    affector->createKeyFrame(5.0f, "0.0", CEGUI::KeyFrame::P_QuadraticDecelerating);

    //anim->defineAutoSubscription(CEGUI::Window::EventTextChanged, "Stop");
    anim->defineAutoSubscription(CEGUI::Window::EventTextChanged, "Start");


    CEGUI::AnimationInstance* instance = CEGUI::AnimationManager::getSingleton().instantiateAnimation(anim);
    // after we instantiate the animation, we have to set its target window
    instance->setTargetWindow(d_fontEditorInfoLabel);

    // at this point, you can start this instance and see the results
    instance->start();
}

void FontDemo::checkIfEditButtonShouldBeDisabled(CEGUI::Font &font)
{
    bool isEditable = findFontOption(font.getName());

    if(!isEditable)
    {
        d_fontEditButton->disable();
        d_fontEditButton->setTooltipText("This demo won't allow editing of\n"
            "fonts that were created outside the demo or\n"
            "were loaded from .font files");
    }
    else
    {
        d_fontEditButton->enable();
        d_fontEditButton->setTooltipText("");
    }
}

void FontDemo::initialiseDemoFonts()
{
    FontManager& fontManager(FontManager::getSingleton());
    fontManager.createFreeTypeFont("Junicode-14", 14.f, true, "Junicode.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("Klingon-pIqaD-HaSta-24", 24.f, true, "Klingon-pIqaD-HaSta.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("TnuaLibre-12", 12.f, true, "Tnua-Libre.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("RichStyle-22", 22.5f, true, "RichStyle.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("FetteUNZFraktur-20", 20.0f, true, "FetteClassicUNZFraktur.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("GreatVibes-22", 22.f, true, "GreatVibes-Regular.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("FellType-12.5", 12.5f, true, "IMFePIrm29P.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("FutharkAdapted-18", 18.f, true, "Futhark Adapted.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));

    fontManager.createFreeTypeFont("mizufalp-12", 12.f, true, "mizufalp.ttf",
        Font::getDefaultResourceGroup(), ASM_Disabled, CEGUI::Sizef(1280.0f, 720.0f));
}

/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static FontDemo sample;
    return sample;
}
