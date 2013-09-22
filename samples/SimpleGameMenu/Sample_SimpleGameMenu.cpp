/***********************************************************************
    filename:   Sample_SimpleGameMenu.cpp
    created:    05/9/2013
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
#include "Sample_SimpleGameMenu.h"
#include "CEGUI/CEGUI.h"

#include <iostream>
#include <sstream>

static CEGUI::String MainMenu = "MainContainer";
static CEGUI::String OptionsMenu = "OptionsContainer";
static CEGUI::String DifficultyMenu = "DifficultyContainer";
static CEGUI::String GeneralOptionsMenu = "GeneralOptionsContainer";
static CEGUI::String VideoOptionsMenu = "VideoOptionsContainer";
static CEGUI::String SoundOptionsMenu = "SoundOptionsContainer";

/** This sample uses most of the code from the 'HelloWorld' sample.
    Thus, most of the clarifying comments have been removed for brevity. **/

SimpleGameMenu::SimpleGameMenu() :
    d_currentMenu(0),
    d_layout(0),
    d_root(0),
    d_windowNavigator(0)
{
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool SimpleGameMenu::initialise(CEGUI::GUIContext* gui_context)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    gui_context->getPointerIndicator().setDefaultImage("TaharezLook/MouseArrow");

    WindowManager& win_mgr = WindowManager::getSingleton();

    if(!ImageManager::getSingleton().isDefined("SpaceBackgroundImage"))
        ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    d_root = win_mgr.createWindow("TaharezLook/StaticImage", "root_wnd");
    // set position and size
    d_root->setPosition(UVector2(cegui_reldim(0), cegui_reldim(0)));
    d_root->setSize(USize(cegui_reldim(1), cegui_reldim(1)));
    // disable frame and standard background
    d_root->setProperty("BackgroundEnabled", "false");
    // set the background image
    d_root->setProperty("Image", "SpaceBackgroundImage");
    // install this as the root GUI sheet
    gui_context->setRootWindow(d_root);

    d_navigationStrategy = new WindowChildrenNavigationStrategy(0);

    d_windowNavigator = new WindowNavigator(createNavigationMappings(),
        d_navigationStrategy);
    gui_context->setWindowNavigator(d_windowNavigator);

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    gui_context->setDefaultFont(&defaultFont);

    d_layout = win_mgr.loadLayoutFromFile("SimpleGameMenu.layout");
    d_root->addChild(d_layout);
    showMenu(MainMenu);

    createEventHandlers();

    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void SimpleGameMenu::deinitialise()
{
    delete d_windowNavigator;
}

std::map<int, CEGUI::String> SimpleGameMenu::createNavigationMappings()
{
    using namespace CEGUI;

    std::map<int, String> mappings;

    mappings[SV_GoDown] = NavigationStrategiesPayloads::NAVIGATE_NEXT;
    mappings[SV_GoUp] = NavigationStrategiesPayloads::NAVIGATE_PREVIOUS;

    return mappings;
}

void SimpleGameMenu::createEventHandlers()
{
    using namespace CEGUI;

    d_layout->getChild(MainMenu)->getChild("NewGameButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleNewGameButtonClick, this));
    d_layout->getChild(MainMenu)->getChild("ContinueGameButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleContinueGameButtonClick, this));
    d_layout->getChild(MainMenu)->getChild("OptionsButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleOptionsButtonClick, this));

    d_layout->getChild(OptionsMenu)->getChild("GeneralOptionsButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleGeneralOptionsButtonClick, this));
    d_layout->getChild(OptionsMenu)->getChild("VideoOptionsButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleVideoOptionsButtonClick, this));
    d_layout->getChild(OptionsMenu)->getChild("SoundOptionsButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleSoundOptionsButtonClick, this));

    d_layout->getChild(OptionsMenu)->getChild("OptionsBackButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleGoToMainMenu, this));
    d_layout->getChild(DifficultyMenu)->getChild("DifficultyBackButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleGoToMainMenu, this));

    d_layout->getChild(VideoOptionsMenu)->getChild("VideoOptionsBackButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleGoToOptionsMenu, this));
    d_layout->getChild(SoundOptionsMenu)->getChild("SoundOptionsBackButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleGoToOptionsMenu, this));
    d_layout->getChild(GeneralOptionsMenu)->getChild("GeneralOptionsBackButton")->subscribeEvent(
        PushButton::EventClicked, Event::Subscriber(&SimpleGameMenu::handleGoToOptionsMenu, this));
}

void SimpleGameMenu::showMenu(CEGUI::String menu_name)
{
    if (d_currentMenu != 0)
        d_currentMenu->setVisible(false);

    d_currentMenu = d_layout->getChild(menu_name);
    d_currentMenu->setVisible(true);
    d_navigationStrategy->d_targetWindow = d_currentMenu;
}

bool SimpleGameMenu::handleNewGameButtonClick(const CEGUI::EventArgs& e)
{
    showMenu(DifficultyMenu);
    return true;
}

bool SimpleGameMenu::handleContinueGameButtonClick(const CEGUI::EventArgs& e)
{
    return true;
}

bool SimpleGameMenu::handleOptionsButtonClick(const CEGUI::EventArgs& e)
{
    showMenu(OptionsMenu);
    return true;
}

bool SimpleGameMenu::handleGoToOptionsMenu(const CEGUI::EventArgs& e)
{
    showMenu(OptionsMenu);
    return true;
}

bool SimpleGameMenu::handleSoundOptionsButtonClick(const CEGUI::EventArgs& e)
{
    showMenu(SoundOptionsMenu);
    return true;
}

bool SimpleGameMenu::handleVideoOptionsButtonClick(const CEGUI::EventArgs& e)
{
    showMenu(VideoOptionsMenu);
    return true;
}

bool SimpleGameMenu::handleGoToMainMenu(const CEGUI::EventArgs& e)
{
    showMenu(MainMenu);
    return true;
}

bool SimpleGameMenu::handleGeneralOptionsButtonClick(const CEGUI::EventArgs& e)
{
    showMenu(GeneralOptionsMenu);
    return true;
}


/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static SimpleGameMenu sample;
    return sample;
}
