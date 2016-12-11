/***********************************************************************
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
#include "MenuNavigation.h"
#include "CEGUI/CEGUI.h"

#include <iostream>
#include <sstream>

using namespace CEGUI;
using namespace NavigationStrategiesPayloads;

/** This sample uses most of the code from the 'HelloWorld' sample.
    Thus, most of the clarifying comments have been removed for brevity. **/

MenuNavigationSample::MenuNavigationSample()
{
    Sample::d_name = "MenuNavigationSample";
    Sample::d_credits = "Timotei Dolean";
    Sample::d_description =
        "A demo that shows a 2D menu which can be navigated using the keyboard or other devices.";
    Sample::d_summary =
        "The demo uses the WindowManager to create from code a window "
        "with a menu that contains multiple buttons arranged in a 4x4 matrix.";
}
/*************************************************************************ech
Sample specific initialisation goes here.
*************************************************************************/
bool MenuNavigationSample::initialise(CEGUI::GUIContext* gui_context)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    gui_context->getCursor().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();
    d_root = win_mgr.loadLayoutFromFile("MenuNavigationSample.layout");

    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();
    gui_context->setDefaultFont(defaultFont);

    gui_context->setRootWindow(d_root);

    MatrixNavigationStrategy* d_matrixNavigationStrategy = new MatrixNavigationStrategy();
    d_matrixWindowNavigator = new WindowNavigator(createMatrixNavigationMappings(),
        d_matrixNavigationStrategy);
    gui_context->setWindowNavigator(d_matrixWindowNavigator);

    LinearNavigationStrategy* d_linearNavigatorStrategy = new LinearNavigationStrategy();
    d_linearWindowNavigator = new WindowNavigator(createLinearNavigationMappings(),
        d_linearNavigatorStrategy);

    TabControl* tabControl = static_cast<TabControl*>(d_root->getChild("FrameWindow/TabControl"));
    tabControl->subscribeEvent(TabControl::EventSelectionChanged,
        Event::Subscriber(&MenuNavigationSample::handleTabSelectionChanged, this));

    Window* page1Window = win_mgr.loadLayoutFromFile("MenuNavigationSampleTabPage1.layout");
    d_logWidget1 = page1Window->getChild("StaticText");
    d_logWidget1->setText("OK");

    // 4 rows
    d_matrixNavigationStrategy->d_windows.resize(4);
    for (int i = 1; i <= 16; ++i)
    {
        std::ostringstream os;
        os << "Button" << i;

        PushButton* button = static_cast<PushButton*>(page1Window->getChild(os.str()));
        button->subscribeEvent(PushButton::EventClicked,
            Event::Subscriber(&MenuNavigationSample::handleNumberButtonClicked, this));

        d_matrixNavigationStrategy->d_windows.at((i - 1) % 4).push_back(button);
    }

    tabControl->addTab(page1Window);

    Window* page2Window = win_mgr.loadLayoutFromFile("MenuNavigationSampleTabPage2.layout");
    d_logWidget2 = page2Window->getChild("StaticText");
    d_logWidget2->setText("OK");

    Window* selectButton = page2Window->getChild("SelectButton");
    selectButton->subscribeEvent(PushButton::EventClicked,
        Event::Subscriber(&MenuNavigationSample::handleSelectButtonClicked, this));

    tabControl->addTab(page2Window);

    d_classesList = static_cast<ListWidget*>(page2Window->getChild("ClassesList"));
    d_classesList->setMultiSelectEnabled(true);
    initialiseClasses(d_classesList);

    d_linearNavigatorStrategy->d_windows.push_back(d_classesList);
    d_linearNavigatorStrategy->d_windows.push_back(selectButton);

    return true;
}

/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void MenuNavigationSample::deinitialise()
{
}

void MenuNavigationSample::initialiseClasses(ListWidget* classes_list_view)
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

    for (int i = 0; i < classes_list_size; ++i)
    {
        classes_list_view->addItem(new StandardItem(classes_list[i]));
    }
}

bool MenuNavigationSample::handleSelectButtonClicked(const CEGUI::EventArgs&)
{
    using namespace CEGUI;

    StandardItem* item = d_classesList->getFirstSelectedItem();
    if (item != nullptr)
    {
        d_logWidget2->setText("Selected " + item->getText() + "\n");
    }

    return true;
}

bool MenuNavigationSample::handleNumberButtonClicked(const CEGUI::EventArgs& e)
{
    d_logWidget1->setText("Button " +
        static_cast<const WindowEventArgs&>(e).window->getText() +
        " pressed\n");

    return true;
}

bool MenuNavigationSample::handleTabSelectionChanged(const CEGUI::EventArgs& e)
{
    TabControl* tabControl = static_cast<TabControl*>(static_cast<const WindowEventArgs&>(e).window);

    // only the first tab has a window navigator
    if (tabControl->getSelectedTabIndex() == 0)
        d_root->getGUIContext().setWindowNavigator(d_matrixWindowNavigator);
    else
        d_root->getGUIContext().setWindowNavigator(d_linearWindowNavigator);
    return true;
}

std::map<SemanticValue, String> MenuNavigationSample::createMatrixNavigationMappings()
{
    std::map<SemanticValue, String> mappings;

    mappings[SemanticValue::NavigateToNext] = NAVIGATE_NEXT;
    mappings[SemanticValue::NavigateToPrevious] = NAVIGATE_PREVIOUS;
    mappings[SemanticValue::GoToPreviousCharacter] = NAVIGATE_LEFT;
    mappings[SemanticValue::GoToNextCharacter] = NAVIGATE_RIGHT;
    mappings[SemanticValue::GoDown] = NAVIGATE_DOWN;
    mappings[SemanticValue::GoUp] = NAVIGATE_UP;

    return mappings;
}

std::map<SemanticValue, CEGUI::String> MenuNavigationSample::createLinearNavigationMappings()
{
    std::map<SemanticValue, String> mappings;

    mappings[SemanticValue::NavigateToNext] = NAVIGATE_NEXT;
    mappings[SemanticValue::NavigateToPrevious] = NAVIGATE_PREVIOUS;

    return mappings;
}
