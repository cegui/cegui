/***********************************************************************
filename:   FontDemo.cpp
created:    17/6/2006
author:     Paul D Turner
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

// NOTE: if you don't have Asian fonts installed, the Japanese and Korean
// sample texts might show up unreadable in this source file. The sample
// should look okay though when running.

#include "Sample_FontDemo.h"
#include "CEGUI/CEGUI.h"

using namespace CEGUI;

static const unsigned int LangListSize = 10;

static struct
{
    encoded_char* Language;
    encoded_char* Font;
    encoded_char* Text;
} LangList [LangListSize] =
{
    // A list of strings in different languages
    // Feel free to add your own language here (UTF-8 ONLY!)...
    {
        (encoded_char*)"English",
            (encoded_char*)"DejaVuSans-10",
            (encoded_char*)"THIS IS SOME TEXT IN UPPERCASE\n"
            "and this is lowercase...\n"
            "Try Catching The Brown Fox While It's Jumping Over The Lazy Dog"
    },
    {
        (encoded_char*)"Русский",
            (encoded_char*)"DejaVuSans-10",
            (encoded_char*)"Всё ускоряющаяся эволюция компьютерных технологий "
            "предъявила жёсткие требования к производителям как "
            "собственно вычислительной техники, так и периферийных устройств.\n"
            "\nЗавершён ежегодный съезд эрудированных школьников, "
            "мечтающих глубоко проникнуть в тайны физических явлений "
            "и химических реакций.\n"
            "\nавтор панграмм -- Андрей Николаев\n"
        },
        {
            (encoded_char*)"Română",
                (encoded_char*)"DejaVuSans-10",
                (encoded_char*)"CEI PATRU APOSTOLI\n"
                "au fost trei:\n"
                "Luca şi Matfei\n"
        },
        {
            (encoded_char*)"Dansk",
                (encoded_char*)"DejaVuSans-10",
                (encoded_char*)"FARLIGE STORE BOGSTAVER\n"
                "og flere men små...\n"
                "Quizdeltagerne spiste jordbær med fløde, mens "
                "cirkusklovnen Walther spillede på xylofon\n"
            },
            {
                (encoded_char*)"Japanese",
                    (encoded_char*)"Batang-26",
                    (encoded_char*)"日本語を選択\n"
                    "トリガー検知\n"
                    "鉱石備蓄不足\n"
            },
            {
                (encoded_char*)"Korean",
                    (encoded_char*)"Batang-26",
                    (encoded_char*)"한국어를 선택\n"
                    "트리거 검지\n"
                    "광석 비축부족\n"
                },
                {
                    (encoded_char*)"Việt",
                        (encoded_char*)"DejaVuSans-10",
                        (encoded_char*)"Chào CrazyEddie !\n"
                        "Mình rất hạnh phúc khi nghe bạn nói điều đó\n"
                        "Hy vọng sớm được thấy CEGUI hỗ trợ đầy đủ tiếng Việt\n"
                        "Cám ơn bạn rất nhiều\n"
                        "Chúc bạn sức khoẻ\n"
                        "Tạm biệt !\n"
                },
                {
                    (encoded_char*)"Việt",
                        (encoded_char*)"DejaVuSans-10",
                        (encoded_char*)"Chào CrazyEddie !\n"
                        "Mình rất hạnh phúc khi nghe bạn nói điều đó\n"
                        "Hy vọng sớm được thấy CEGUI hỗ trợ đầy đủ tiếng Việt\n"
                        "Cám ơn bạn rất nhiều\n"
                        "Chúc bạn sức khoẻ\n"
                        "Tạm biệt !\n"
                },
                {
                    (encoded_char*)"Việt",
                        (encoded_char*)"DejaVuSans-10",
                        (encoded_char*)"Chào CrazyEddie !\n"
                        "Mình rất hạnh phúc khi nghe bạn nói điều đó\n"
                        "Hy vọng sớm được thấy CEGUI hỗ trợ đầy đủ tiếng Việt\n"
                        "Cám ơn bạn rất nhiều\n"
                        "Chúc bạn sức khoẻ\n"
                        "Tạm biệt !\n"
                },  
                {
                    (encoded_char*)"Klingon",
                        (encoded_char*)"DejaVuSans-10",
                        (encoded_char*)"  \n"
                        "         \n"
                        "         \n"
                        "  "
                }
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
    d_guiContext = guiContext;

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    d_guiContext->getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // Create a custom font which we use to draw the list items. This custom
    // font won't get effected by the scaler and such.
    FontManager& fontManager(FontManager::getSingleton());
    CEGUI::Font& font(fontManager.createFromFile("DejaVuSans-10.font"));
    // Set it as the default
    d_guiContext->setDefaultFont(&font);


    // load all the fonts (if they are not loaded yet)
    fontManager.createAll("*.font", "fonts");

    //Fill list with all loaded font names
    retrieveFontNames();

    //Fill list with all available font type file names
    retrieveFontFileNames();

    // set tooltip styles (by default there is none)
    //d_guiContext->setDefaultTooltipType("Vanilla/StaticText");

    // Load the GUI layout and attach it to the context as root window
    d_root = winMgr.loadLayoutFromFile("FontDemo.layout");
    d_guiContext->setRootWindow(d_root);

    //Here we create a font and apply it to the renew font name button
    CEGUI::Font& buttonFont = fontManager.createFreeTypeFont("DejaVuSans-14", 14.f, true, "DejaVuSans.ttf",
        Font::getDefaultResourceGroup(), ASM_Vertical, CEGUI::Sizef(1280.f, 720.f));
    d_renewFontNameButton =  static_cast<CEGUI::PushButton*>(d_root->getChild("FontDemoWindow/FontCreator/RenewNameButton"));
    d_renewFontNameButton->setFont(&buttonFont);

    d_root->getChild("FontDemoWindow/FontCreator/FontSizeLabel");

    //Subscribe click event for renewing font name based on font file name and size
    d_renewFontNameButton->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&FontDemo::handleRenewFontNameButtonClicked, this));

    // Get the editbox where we display the text
    d_textDisplayMultiLineEditbox = static_cast<CEGUI::MultiLineEditbox*>(d_root->getChild("FontDemoWindow/MultiLineTextWindow"));
    d_textDisplayMultiLineEditbox->subscribeEvent(CEGUI::MultiLineEditbox::EventTextChanged, Event::Subscriber(&FontDemo::handleTextMultiLineEditboxTextChanged, this));

    initialiseAutoScaleOptionsArray();

    initialiseLangToTextMap();

    // Initialise the font creator window + its subwindows
    initialiseFontCreator();

    // Initialise the widget to select fonts and its items
    initialiseFontSelector();

    // Initialise the widget to select the different language texts the relative items for each
    initialiseTextSelector();

    d_fontSelector->subscribeEvent(CEGUI::Listbox::EventSelectionChanged, Event::Subscriber(&FontDemo::handleFontSelectionChanged, this));
    d_fontSelector->setItemSelectState(size_t(0), true);


    return true;
}

// method to perform any required cleanup operations.
void FontDemo::deinitialise()
{
}

bool FontDemo::handleFontCreationButtonClicked(const EventArgs& e)
{
    FontManager& fontMgr(FontManager::getSingleton());

    CEGUI::String fontName = d_fontNameEditbox->getText();
    bool fontNameExists = fontMgr.isDefined(fontName);
    if(fontNameExists ||fontName.size() == 0)
        return true; // todo : display error message

    CEGUI::String fontFileName = d_fontFileNameSelector->getSelectedItem()->getText();

    CEGUI::String fontSizeString = d_fontSizeEditbox->getText();
    float fontSize = CEGUI::PropertyHelper<float>::fromString(fontSizeString);
    if(fontSize == 0.f)
        return true;

    bool antiAlias = d_fontAntiAliasCheckbox->isSelected();

    
    AutoScaledMode autoScaleMode = static_cast<AutoScaledMode>(getAutoScaleMode());


    String::size_type pos = fontFileName.rfind(".imageset");
    if(pos != -1)
    {
        CEGUI::Font& createdFont = fontMgr.createPixmapFont(fontName, fontFileName, Font::getDefaultResourceGroup(), autoScaleMode,
            CEGUI::Sizef(1280.f, 720.f), XREA_THROW);
    }
    else
    {
        CEGUI::Font& createdFont = fontMgr.createFreeTypeFont(fontName, fontSize, antiAlias, fontFileName, Font::getDefaultResourceGroup(), autoScaleMode, 
            CEGUI::Sizef(1280.f, 720.f), XREA_THROW);
    }

    d_fontSelector->addItem(new MyListItem(fontName, 0));

    return true;
}


bool FontDemo::handleFontEditButtonClicked(const EventArgs& e)
{
    FontManager& fontMgr(FontManager::getSingleton());

    CEGUI::String fontName = d_fontNameEditbox->getText();
    bool fontNameExists = fontMgr.isDefined(fontName);
    if(!fontNameExists)
        return true; // todo : display error message

    CEGUI::Font& font(fontMgr.get(fontName));

  /*
    if(font.isPropertyPresent("PointSize"))
    {
        CEGUI::String fontSizeString = d_fontSizeEditbox->getText();
        float fontSize = CEGUI::PropertyHelper<float>::fromString(fontSizeString);
        if(fontSize != 0.f)
           // font.setProperty<float>("PointSize", fontSize);
            font.setProperty("PointSize", PropertyHelper<int>::toString(int(fontSize)));
    }
      */
    if(font.isPropertyPresent("Antialiased"))
    {
        bool antiAlias = d_fontAntiAliasCheckbox->isSelected();
       // font.setProperty<bool>("Antialiased", antiAlias);

        font.setProperty("Antialiased", PropertyHelper<bool>::toString(antiAlias));
    }


/*
    AutoScaledMode autoScaleMode = static_cast<AutoScaledMode>(getAutoScaleMode());
    font.setAutoScaled(autoScaleMode);*/



    return true;
}

bool FontDemo::handleFontSelectionChanged(const EventArgs& e)
{
    if(d_fontSelector->getFirstSelectedItem())
    {
        d_textDisplayMultiLineEditbox->setFont(d_fontSelector->getFirstSelectedItem()->getText());
    }

    if(d_textSelector->getFirstSelectedItem() && d_fontSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getID();
        
        d_languageToFontMap[LangList[index].Language] = d_fontSelector->getFirstSelectedItem()->getText();
    }

    if(d_fontSelector->getFirstSelectedItem())
    {
        CEGUI::String fontName = d_fontSelector->getFirstSelectedItem()->getText();

        if(FontManager::getSingleton().isDefined(fontName))
        {
            CEGUI::Font& font(FontManager::getSingleton().get(fontName));

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
        }
    }

    return true;
}

bool FontDemo::handleTextSelectionChanged(const EventArgs& e)
{
    if(d_textSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getID();

        d_textDisplayMultiLineEditbox->setText(d_languageToTextMap[LangList[index].Language]);

        changeFontSelectorFontSelection(d_languageToFontMap[LangList[index].Language]);
    }

    return true;
}

bool FontDemo::handleTextMultiLineEditboxTextChanged(const EventArgs& e)
{
    if(d_textSelector->getFirstSelectedItem())
    {
        unsigned int index = d_textSelector->getFirstSelectedItem()->getID();

        d_languageToTextMap[LangList[index].Language] = d_textDisplayMultiLineEditbox->getText();
    }

    return true;
}

bool FontDemo::handleFontFileNameSelectionChanged(const EventArgs& e)
{
    generateNewFontName();

    return true;
}

bool FontDemo::handleRenewFontNameButtonClicked(const EventArgs& e)
{
    generateNewFontName();

    return true;
}

void FontDemo::initialiseAutoScaleOptionsArray()
{
    d_autoScaleOptionsArray.push_back("Disabled");
    d_autoScaleOptionsArray.push_back("Vertical");
    d_autoScaleOptionsArray.push_back("Horizontal");
    d_autoScaleOptionsArray.push_back("Minimum");
    d_autoScaleOptionsArray.push_back("Maximum");
    d_autoScaleOptionsArray.push_back("Both");
}


void FontDemo::retrieveFontNames()
{
    FontManager& fontManager(FontManager::getSingleton());
    FontManager::FontIterator fi = fontManager.getIterator();

    while (!fi.isAtEnd())
    {
        CEGUI::Font& font(fontManager.get(fi.getCurrentKey()));

        d_fontNameOptions.push_back(font.getName());
        ++fi;
    }
}


void FontDemo::retrieveFontFileNames()
{
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.ttf", Font::getDefaultResourceGroup());
    System::getSingleton().getResourceProvider()->getResourceGroupFileNames(d_fontFileNameOptions, "*.pcf", Font::getDefaultResourceGroup());
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

    //Select a font file name if any are present
    if(d_fontNameOptions.size() > 0)
    {
        for(unsigned int i = 0; i < d_fontNameOptions.size(); i++)
        {
            const CEGUI::String& fontName(d_fontNameOptions[i]);

            d_fontSelector->addItem(new MyListItem(fontName, i));  
        }
    }
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

    d_textSelector->setItemSelectState(size_t(0), true);
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
        {
            return i;
        }
    }

    return 0;
}

/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static FontDemo sample;
    return sample;
}
