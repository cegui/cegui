/***********************************************************************
created:    11/8/2012
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
#include "RestaurantGame.h"

#include "CEGUI/CEGUI.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace CEGUI;


struct GamePlate
{
    GamePlate(RestaurantGameSample* RestaurantGameSample);
    ~GamePlate();

    void update(float timeSinceLastUpdate);
    int getPoints();

    CEGUI::Window* d_window;
    bool d_isComingFromRight;
    bool d_isDestroyed;
};

GamePlate::GamePlate(RestaurantGameSample* RestaurantGameSample)
    : d_isDestroyed(false)
{
    d_window = RestaurantGameSample->spawnPlate();

    int randumNumber = rand() % 2;
    d_isComingFromRight = (randumNumber == 0 ? false : true);

    if(d_isComingFromRight)
    {
        d_window->setHorizontalAlignment(HorizontalAlignment::Right);
        d_window->setPosition(d_window->getPosition() + CEGUI::UVector2(cegui_reldim(0.2f), cegui_absdim(0.0f)));
    }
    else
        d_window->setPosition(d_window->getPosition() + CEGUI::UVector2(cegui_reldim(-0.2f), cegui_absdim(0.0f)));
}

GamePlate::~GamePlate()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    winMgr.destroyWindow(d_window);
}

void GamePlate::update(float timeSinceLastUpdate)
{
    CEGUI::UVector2 positionOffset;

    if(d_isComingFromRight)
        positionOffset = CEGUI::UVector2(cegui_reldim(timeSinceLastUpdate * -0.24f), cegui_absdim(0.f));
    else
        positionOffset = CEGUI::UVector2(cegui_reldim(timeSinceLastUpdate * 0.24f), cegui_absdim(0.f));

    d_window->setPosition(d_window->getPosition() + positionOffset);

    const CEGUI::UVector2& position = d_window->getPosition();

    if(d_isComingFromRight)
    {
        if(position.d_x.d_scale < -1.2f)
            d_isDestroyed = true;
    }
    else
    {
        if(position.d_x.d_scale > 1.2f)
            d_isDestroyed = true;
    }
}

int GamePlate::getPoints()
{
    CEGUI::Window* window = d_window->getChild("ImageWindowObject");

    CEGUI::String objectImage = window->getProperty("Image");

    if(objectImage.compare(RestaurantGameSample::s_imageNameBread) == 0)
        return 2;
    else if(objectImage.compare(RestaurantGameSample::s_imageNamePoo) == 0)
        return -6;
    else if(objectImage.compare(RestaurantGameSample::s_imageNameSteak) == 0)
        return -13;
    else if(objectImage.compare(RestaurantGameSample::s_imageNamePrizza) == 0)
        return 3;
    else if(objectImage.compare(RestaurantGameSample::s_imageNameVegPeople) == 0)
        return 1;
    else if(objectImage.compare(RestaurantGameSample::s_imageNameVegFruits) == 0)
        return 88;

    return 0;
}

const CEGUI::String RestaurantGameSample::s_imageNamePlate = "RestaurantGameSample/Plate";
const CEGUI::String RestaurantGameSample::s_imageNameBread = "RestaurantGameSample/Bread";
const CEGUI::String RestaurantGameSample::s_imageNamePoo = "RestaurantGameSample/Poo";
const CEGUI::String RestaurantGameSample::s_imageNamePrizza = "RestaurantGameSample/Prizza";
const CEGUI::String RestaurantGameSample::s_imageNameSteak = "RestaurantGameSample/Steak";
const CEGUI::String RestaurantGameSample::s_imageNameVegPeople = "RestaurantGameSample/VegetablePeople";
const CEGUI::String RestaurantGameSample::s_imageNameVegFruits = "RestaurantGameSample/VegetablesAndFruits";

RestaurantGameSample::RestaurantGameSample() :
    Sample(99)
{
    Sample::d_name = "RestaurantGameSample";
    Sample::d_credits = "Lukas \"Ident\" Meindl, graphics: Charles \"Syg\" Mattei";
    Sample::d_description =
        "The RestaurantGame presents a game that uses a head-up display. This shows that CEGUI has "
        "full capabilities for rendering HUD's for first person shooters or other any other types of games. "
        "The interface consists of a health bar, lives, switchable weapons and power-ups. "
        "The game itself consists of moving dishes that have to be clicked on to receive points. "
        "When clicked, the points appear in red or green text as a pop-up text that moves "
        "upwards and vanishes after some seconds.";
    Sample::d_summary =
        "The HUD consists of basic \"Generic/Label\" and \"Generic/Image\" windows to display its elements. "
        "For some windows event handlers are registered to react to user interaction. "
        "A CEGUI/Progressbar with custom skinning was made for this demo to display the life bar. "
        "For the pop-up points a window with animations directly defined in the look n' feel file "
        "was made. The animations are triggered by firing events and when the pop-up animation is "
        "over the event handler will react to this and subsequently the window will be "
        "deleted as defined in the C++ code.";
}

bool RestaurantGameSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);
    d_guiContext = guiContext;

    SchemeManager::getSingleton().createFromFile("RestaurantGameSample.scheme");
    SchemeManager::getSingleton().createFromFile("Generic.scheme");

    FontManager::getSingleton().createFromFile("DejaVuSans-14.font");

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();
    // Load the RestaurantGameSample Layout
    d_rootIngame = winMgr.loadLayoutFromFile("RestaurantGameSampleIngame.layout");
    d_rootGameOver = winMgr.loadLayoutFromFile("RestaurantGameSampleGameOver.layout");
    d_root = winMgr.createWindow("DefaultWindow", "RestaurantGameSampleRoot");
    d_root->addChild(d_rootIngame);
    d_guiContext->setRootWindow(d_root);

    if(!ImageManager::getSingleton().isDefined("RestaurantGameSampleGameOver"))
        ImageManager::getSingleton().addBitmapImageFromFile("RestaurantGameSampleGameOver", "RestaurantGameSampleGameOver.png");
    d_rootGameOver->getChild("GameOverImage")->setProperty("Image", "RestaurantGameSampleGameOver");

    setupCursor();

    srand(static_cast<unsigned int >(time(nullptr)));

    d_lifeBar = static_cast<CEGUI::ProgressBar*>(d_rootIngame->getChild("TopBar/LifeBar"));

    initGame();

    d_rootIngame->getChild("BotBar/WeaponBGImage/LeftArrowArea")->subscribeEvent(CEGUI::Window::EventCursorActivate, Event::Subscriber(&RestaurantGameSample::handleWeaponLeftArrowClicked, this));
    d_rootIngame->getChild("BotBar/WeaponBGImage/RightArrowArea")->subscribeEvent(CEGUI::Window::EventCursorActivate, Event::Subscriber(&RestaurantGameSample::handleWeaponRightArrowClicked, this));

    d_rootGameOver->getChild("ButtonRestart")->subscribeEvent(CEGUI::PushButton::EventClicked, Event::Subscriber(&RestaurantGameSample::handleRestartButtonClicked, this));


    return true;
}

/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void RestaurantGameSample::deinitialise()
{
    while(!d_gamePlates.empty())
    {
        GamePlate* curPlate = d_gamePlates.back();
        delete curPlate;

        d_gamePlates.pop_back();
    }
}


void RestaurantGameSample::onEnteringSample()
{
    initGame();
}

void RestaurantGameSample::update(float timeSinceLastUpdate)
{
    static float timeSinceLastSpawn(0.0f);

    timeSinceLastSpawn += timeSinceLastUpdate;

    updateCursor();


    if(timeSinceLastSpawn> 1.2f)
    {
        d_gamePlates.push_back(new GamePlate(this));
        timeSinceLastSpawn -= 1.2f;
    }

    updatePlates(timeSinceLastUpdate);

    delayDestroyWindows();

    d_guiContext->markAsDirty();
}

void RestaurantGameSample::setupCursor()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    d_cursorWnd = winMgr.createWindow("Generic/Image");
    d_cursorWnd->setProperty("Image", "RestaurantGameSample/Spoon");
    d_cursorWnd->setAspectMode(AspectMode::Expand);
    d_cursorWnd->setAspectRatio(1.0f);
    d_cursorWnd->setSize(CEGUI::USize(cegui_absdim(0.0f), cegui_reldim(0.1f)));
    d_cursorWnd->setAlwaysOnTop(true);
    d_cursorWnd->setCursorPassThroughEnabled(true);
    d_rootIngame->addChild(d_cursorWnd);
}

void RestaurantGameSample::updateCursor()
{
    glm::vec2 position = d_guiContext->getCursor().getPosition();

    // We want to position the image-window right top of the actual
    // cursor point so we add its height
    float absHeight = d_cursorWnd->getPixelSize().d_height;
    position.y -= absHeight;

    d_cursorWnd->setPosition(
        CEGUI::UVector2(
        cegui_absdim(position.x - 5.0f), cegui_absdim(position.y + 5.0f))
        );
}


CEGUI::Window* RestaurantGameSample::spawnPlate()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    CEGUI::Window* plateRoot = winMgr.createWindow("DefaultWindow");
    plateRoot->setSize(CEGUI::USize(cegui_absdim(0.0f), cegui_reldim(0.16f)));
    plateRoot->setAspectMode(CEGUI::AspectMode::Expand);
    plateRoot->setAspectRatio(1.0f);
    plateRoot->setRiseOnClickEnabled(false);
    plateRoot->setPixelAligned(false);
    plateRoot->subscribeEvent(CEGUI::Window::EventCursorActivate, Event::Subscriber(&RestaurantGameSample::handlePlateWindowActivated, this));
    d_rootIngame->addChild(plateRoot);

    CEGUI::Window* plateImgWnd = winMgr.createWindow("Generic/Image", "ImageWindowPlate");
    plateImgWnd->setProperty("Image", s_imageNamePlate);
    plateImgWnd->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_absdim(0.0f)));
    plateImgWnd->setAspectRatio(3.308f);
    plateImgWnd->setAspectMode(CEGUI::AspectMode::Expand);
    plateImgWnd->setVerticalAlignment(CEGUI::VerticalAlignment::Bottom);
    plateImgWnd->setCursorPassThroughEnabled(true);
    plateImgWnd->setPixelAligned(false);
    plateRoot->addChild(plateImgWnd);

    CEGUI::String image = getRandomGameImage();

    CEGUI::Window* plateTopping = winMgr.createWindow("Generic/Image", "ImageWindowObject");
    plateTopping->setProperty("Image", image);
    plateTopping->setSize(CEGUI::USize(cegui_reldim(0.88f), cegui_absdim(0.0f)));
    plateTopping->setAspectRatio(1.0f);
    plateTopping->setAspectMode(CEGUI::AspectMode::Expand);
    plateTopping->setHorizontalAlignment(CEGUI::HorizontalAlignment::Centre);
    plateTopping->setCursorPassThroughEnabled(true);
    plateTopping->setPixelAligned(false);
    plateRoot->addChild(plateTopping);


    int randumNumber = rand() % 10000;
    float posY = randumNumber / 10000.0f;

    plateRoot->setPosition(CEGUI::UVector2(cegui_absdim(0.0f), cegui_reldim(0.1f + 0.6f * posY)));

    return plateRoot;
}

const CEGUI::String& RestaurantGameSample::getRandomGameImage()
{
    int randumNumber = rand() % 100;

    if(randumNumber > 88)
        return s_imageNamePoo;
    else if(randumNumber > 72)
        return s_imageNamePrizza;
    else if(randumNumber > 55)
        return s_imageNameSteak;
    else if(randumNumber > 52)
        return s_imageNameVegFruits;
    else if(randumNumber > 25)
        return s_imageNameVegPeople;
    else if(randumNumber > 0)
        return s_imageNameBread;

    return s_imageNameBread;
}


void RestaurantGameSample::updatePlates(float timeSinceLastUpdate)
{
    unsigned int vectorSize = d_gamePlates.size();

    for(unsigned int i = 0; i < vectorSize;)
    {
        GamePlate* currentPlate = d_gamePlates[i];

        currentPlate->update(timeSinceLastUpdate);
        if(currentPlate->d_isDestroyed)
        {
            delete currentPlate;
            d_gamePlates[i] = d_gamePlates.back();
            d_gamePlates.pop_back();
            vectorSize = d_gamePlates.size();
        }
        else
            ++i;
    }
}

void RestaurantGameSample::updateScoreWindow()
{
    CEGUI::Window* scoreWnd = d_rootIngame->getChild("TopBar/ScoreBGImage/Score");
    scoreWnd->setText(CEGUI::PropertyHelper<std::int32_t>::toString(d_score));
}

bool RestaurantGameSample::handlePlateWindowActivated(const CEGUI::EventArgs& args)
{
    const CEGUI::CursorInputEventArgs& cursor_args = static_cast<const CEGUI::CursorInputEventArgs&>(args);

    for(unsigned int i = 0; i < d_gamePlates.size(); ++i)
    {
        GamePlate* gamePlate = d_gamePlates[i];

        if(gamePlate->d_window == cursor_args.window)
        {
            int points = gamePlate->getPoints();
            d_score += points;
            updateScoreWindow();

            if(points < 0)
            {
                float newProgress = d_lifeBar->getProgress() - 0.35f;


                if(newProgress < 0.0f)
                {
                    d_lives--;
                    handleLivesChanged();
                    newProgress = 1.0f;
                }

                d_lifeBar->setProgress(newProgress);
            }

            gamePlate->d_isDestroyed = true;
            createScorePopup(cursor_args.position, points);
        }
    }

    return false;
}

void RestaurantGameSample::createScorePopup(const glm::vec2& cursor_pos, std::int32_t points)
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    CEGUI::Window* popupWindow = winMgr.createWindow("RestaurantGameSample/PopupLabel");
    d_rootIngame->addChild(popupWindow);
    popupWindow->setPosition(CEGUI::UVector2(cegui_absdim(cursor_pos.x), cegui_absdim(cursor_pos.y)));
    popupWindow->setText(CEGUI::PropertyHelper<std::int32_t>::toString(points));
    popupWindow->setRiseOnClickEnabled(false);
    popupWindow->subscribeEvent(AnimationInstance::EventAnimationEnded, Event::Subscriber(&RestaurantGameSample::handleScorePopupAnimationEnded, this));
    popupWindow->setPixelAligned(false);
    popupWindow->setFont("DejaVuSans-14");

    popupWindow->setPosition(popupWindow->getPosition() + CEGUI::UVector2(cegui_reldim(0.03f), cegui_reldim(-0.02f)));

    if(points < 0)
        popupWindow->setProperty("NormalTextColour", "FF880000");
    else
    {
        popupWindow->setText( "+" + popupWindow->getText());
        popupWindow->setProperty("NormalTextColour", "FF006600");
    }

    CEGUI::EventArgs args;
    popupWindow->fireEvent("StartAnimation", args);
}

bool RestaurantGameSample::handleScorePopupAnimationEnded(const CEGUI::EventArgs& args)
{
    const CEGUI::AnimationEventArgs& animArgs = static_cast<const CEGUI::AnimationEventArgs&>(args);

    CEGUI::Window* window = static_cast<CEGUI::Window*>( animArgs.instance->getTarget());
    d_delayDestroyWindows.push_back(window);

    return false;
}

void RestaurantGameSample::delayDestroyWindows()
{
    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    while(!d_delayDestroyWindows.empty())
    {
        CEGUI::Window* curWindow = d_delayDestroyWindows.back();
        winMgr.destroyWindow(curWindow);

        d_delayDestroyWindows.pop_back();
    }
}


void RestaurantGameSample::handleLivesChanged()
{
    bool life1Visible = (d_lives >= 1);
    bool life2Visible = (d_lives >= 2);
    bool life3Visible = (d_lives >= 3);


    d_rootIngame->getChild("TopBar/Life1")->setAlpha( life1Visible ? 1.0f : 0.5f );
    d_rootIngame->getChild("TopBar/Life2")->setAlpha( life2Visible ? 1.0f : 0.5f );
    d_rootIngame->getChild("TopBar/Life3")->setAlpha( life3Visible ? 1.0f : 0.5f );

    if(d_lives <= 0)
    {
        d_root->addChild(d_rootGameOver);
        d_rootGameOver->addChild(d_cursorWnd);
    }
}

void RestaurantGameSample::initGame()
{
    d_lives = 3;
    handleLivesChanged();

    selectedWeapon(SW_Spoon);

    d_score = 0;
    updateScoreWindow();

    d_lifeBar->setProgress(1.0f);
}

void RestaurantGameSample::selectedWeapon(SelectedWeapon weapon)
{
    d_selectedWeapon = weapon;

    switch(d_selectedWeapon)
    {
    case SW_Spoon:
        d_rootIngame->getChild("BotBar/WeaponSpoon")->setAlpha(1.0f);
        d_rootIngame->getChild("BotBar/WeaponKnife")->setAlpha(0.5f);
        d_rootIngame->getChild("BotBar/WeaponFork")->setAlpha(0.5f);
        d_rootIngame->getChild("BotBar/WeaponBGImage/WeaponLabel")->setText("Le Spoon");

        break;
    case SW_Fork:
        d_rootIngame->getChild("BotBar/WeaponSpoon")->setAlpha(0.5f);
        d_rootIngame->getChild("BotBar/WeaponKnife")->setAlpha(0.5f);
        d_rootIngame->getChild("BotBar/WeaponFork")->setAlpha(1.0f);
        d_rootIngame->getChild("BotBar/WeaponBGImage/WeaponLabel")->setText("Le Fork");
        break;
    case SW_Knife:
        d_rootIngame->getChild("BotBar/WeaponSpoon")->setAlpha(0.5f);
        d_rootIngame->getChild("BotBar/WeaponKnife")->setAlpha(1.0f);
        d_rootIngame->getChild("BotBar/WeaponFork")->setAlpha(0.5f);
        d_rootIngame->getChild("BotBar/WeaponBGImage/WeaponLabel")->setText("Le Knife");
        break;
    default:
        break;
    }
}

bool RestaurantGameSample::handleWeaponRightArrowClicked(const CEGUI::EventArgs&)
{
    int weaponIndex = static_cast<int>(d_selectedWeapon);
    weaponIndex = (weaponIndex - 1) % 3;
    if(weaponIndex < 0)
        weaponIndex += 3;
    selectedWeapon( static_cast<SelectedWeapon>(weaponIndex) );

    return false;
}

bool RestaurantGameSample::handleRestartButtonClicked(const CEGUI::EventArgs&)
{
    d_root->removeChild(d_rootGameOver);
    d_rootIngame->addChild(d_cursorWnd);

    initGame();

    return false;
}

bool RestaurantGameSample::handleWeaponLeftArrowClicked(const CEGUI::EventArgs&)
{
    int weaponIndex = static_cast<int>(d_selectedWeapon);
    selectedWeapon( static_cast<SelectedWeapon>(++weaponIndex % 3) );

    return false;
}