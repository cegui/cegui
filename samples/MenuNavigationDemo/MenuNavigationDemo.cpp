/***********************************************************************
    filename:   MenuNavigationDemo.cpp
    created:    30/5/2013
    author:     Timotei Dolean
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include "MenuNavigationDemo.h"
#include "CEGUI/CEGUI.h"

#include <iostream>

// Sample sub-class for ListboxTextItem that auto-sets the selection brush
// image.  This saves doing it manually every time in the code.
class MyListItem : public CEGUI::ListboxTextItem
{
public:
    MyListItem(const CEGUI::String& text, CEGUI::uint item_id = 0) :
      ListboxTextItem(text, item_id)
      {
          setSelectionBrushImage("TaharezLook/GenericBrush");
      }
};

/** This sample uses most of the code from the 'HelloWorld' sample. 
    Thus, most of the clarifying comments have been removed for brevity. **/

/*************************************************************************ech
    Sample specific initialisation goes here.
*************************************************************************/
bool MenuNavigationDemo::initialise(CEGUI::GUIContext* gui_context)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    gui_context->getPointerIndicator().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();
    d_root = win_mgr.loadLayoutFromFile("MenuNavigationDemo.layout");

    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    gui_context->setDefaultFont(&defaultFont);

    gui_context->setRootWindow(d_root);

    TabControl* tabControl = static_cast<TabControl*>(d_root->getChild("TabControl"));

    tabControl->addTab(win_mgr.loadLayoutFromFile("MenuNavigationDemoTabPage1.layout"));

    Window* page2Window = win_mgr.loadLayoutFromFile("MenuNavigationDemoTabPage2.layout");
    d_logWidget = page2Window->getChild("StaticText");

    Window* selectButton = page2Window->getChild("SelectButton");
    selectButton->subscribeEvent(PushButton::EventClicked, 
        Event::Subscriber(&MenuNavigationDemo::handleSelectButtonClicked, this));

    tabControl->addTab(page2Window);

    d_classesListBox = static_cast<Listbox*>(page2Window->getChild("ClassesListBox"));
    initialiseClasses(d_classesListBox);

    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void MenuNavigationDemo::deinitialise()
{
}

void MenuNavigationDemo::initialiseClasses(CEGUI::Listbox* classesListBox)
{
    static const int classes_list_size = 5;
    static const char* classes_list[] = 
    {
        "Druid",
        "Shaman",
        "Warrior",
        "Priest",
        "Death Knight"
    };
   
    for(int i = 0; i < classes_list_size; ++i)
    {
        classesListBox->addItem(new MyListItem(classes_list[i]));
    }
}

bool MenuNavigationDemo::handleSelectButtonClicked(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    ListboxItem* item = d_classesListBox->getFirstSelectedItem();
    if (item != 0)
    {
        d_logWidget->setText("Selected " + item->getText() + "\n");
    }

    return true;
}


/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static MenuNavigationDemo sample;
    return sample;
}