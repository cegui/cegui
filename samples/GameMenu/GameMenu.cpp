/***********************************************************************
filename:   GameMenu.cpp
created:    31/7/2012
author:     Lukas E Meindl
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2012 Paul D Turner & Thce CEGUI Development Team
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
#include "GameMenu.h"

#include "CEGUI/CEGUI.h"

#include <cmath>
#include <algorithm>


using namespace CEGUI;


bool GameMenuDemo::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);
    d_guiContext = guiContext;

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("GameMenu.scheme");
    d_guiContext->getMouseCursor().setDefaultImage("GameMenuImages/MouseCursor");

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("Jura-13.font");
    // Set default font for the gui context
    d_guiContext->setDefaultFont(&defaultFont);

    FontManager::getSingleton().createFromFile("DejaVuSans-12.font");

    CEGUI::WindowManager& winMgr = CEGUI::WindowManager::getSingleton();

    d_root = winMgr.loadLayoutFromFile("GameMenu.layout");
    d_guiContext->setRootWindow(d_root);

    setupWindows();

    setupAnimations();

    d_interactiveElementsAreActivated = true;

    return true;
}

/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void GameMenuDemo::deinitialise()
{
}

void GameMenuDemo::setupAnimations()
{
    AnimationManager& animMgr = AnimationManager::getSingleton();
    animMgr.loadAnimationsFromXML("GameMenu.anims");

    CEGUI::Animation* startButtAnim = animMgr.getAnimation("StartButtonPulsating");
    CEGUI::AnimationInstance* startButtAnimInstance = animMgr.instantiateAnimation(startButtAnim);
    CEGUI::Window* startButtWindow = d_root->getChild("InnerPartContainer/InsideStartClickArea/StartButtonImage");
    startButtAnimInstance->setTargetWindow(startButtWindow);
    startButtAnimInstance->start();

    CEGUI::Animation* insideImg1Anim = animMgr.getAnimation("InsideImage1Pulsating");
    CEGUI::AnimationInstance* insideImg1AnimInst = animMgr.instantiateAnimation(insideImg1Anim);
    CEGUI::Window* insideImg1 = d_root->getChild("InnerPartContainer/InsideImage1");
    insideImg1AnimInst->setTargetWindow(insideImg1);
    insideImg1AnimInst->start();

    CEGUI::Animation* topBarAnim = animMgr.getAnimation("TopBarMoveInAnimation");
    d_topBarAnimInst = animMgr.instantiateAnimation(topBarAnim);
    CEGUI::Window* topBarWindow = d_root->getChild("TopBar");
    d_topBarAnimInst->setTargetWindow(topBarWindow);

    CEGUI::Animation* botBarAnim = animMgr.getAnimation("BotBarMoveInAnimation");
    d_botBarAnimInst = animMgr.instantiateAnimation(botBarAnim);
    CEGUI::Window* botBarWindow = d_root->getChild("BotBar");
    d_botBarAnimInst->setTargetWindow(botBarWindow);

    CEGUI::Animation* insideBlendInAnim = animMgr.getAnimation("InsideBlendIn");
    d_insideBlendInAnimInst = animMgr.instantiateAnimation(insideBlendInAnim);
    CEGUI::Window* innerPartContainer = d_root->getChild("InnerPartContainer");
    d_insideBlendInAnimInst->setTargetWindow(innerPartContainer);

    CEGUI::Animation* insideImage3RotateIn = animMgr.getAnimation("InsideImage3RotateIn");
    d_insideImage3RotateInInst = animMgr.instantiateAnimation(insideImage3RotateIn);
    CEGUI::Window* insideImage3 = d_root->getChild("InnerPartContainer/OuterRingsContainer/InsideImage3");
    d_insideImage3RotateInInst->setTargetWindow(insideImage3);

    CEGUI::Animation* insideImage4RotateIn = animMgr.getAnimation("InsideImage4RotateIn");
    d_insideImage4RotateInInst = animMgr.instantiateAnimation(insideImage4RotateIn);
    CEGUI::Window* insideImage4 = d_root->getChild("InnerPartContainer/OuterRingsContainer/InsideImage4");
    d_insideImage4RotateInInst->setTargetWindow(insideImage4);

    CEGUI::Animation* insideImageRingsContainerSizeIn = animMgr.getAnimation("RingsContainerSizeIn");
    d_insideImageRingsContainerSizeInInst = animMgr.instantiateAnimation(insideImageRingsContainerSizeIn);
    CEGUI::Window* insideImageContainer = d_root->getChild("InnerPartContainer/OuterRingsContainer");
    d_insideImageRingsContainerSizeInInst->setTargetWindow(insideImageContainer);

    CEGUI::Animation* buttonFadeInAnim = animMgr.getAnimation("ButtonFadeIn");
    d_buttonFadeInAnimInst1 = animMgr.instantiateAnimation(buttonFadeInAnim);
    CEGUI::Window* window = d_root->getChild("InnerButtonsContainer/ButtonOptions");
    d_buttonFadeInAnimInst1->setTargetWindow(window);
    d_buttonFadeInAnimInst2 = animMgr.instantiateAnimation(buttonFadeInAnim);
    window = d_root->getChild("InnerButtonsContainer/ButtonLoad");
    d_buttonFadeInAnimInst2->setTargetWindow(window);
    d_buttonFadeInAnimInst3 = animMgr.instantiateAnimation(buttonFadeInAnim);
    window = d_root->getChild("InnerButtonsContainer/ButtonSave");
    d_buttonFadeInAnimInst3->setTargetWindow(window);
    d_buttonFadeInAnimInst4 = animMgr.instantiateAnimation(buttonFadeInAnim);
    window = d_root->getChild("InnerButtonsContainer/ButtonCharacters");
    d_buttonFadeInAnimInst4->setTargetWindow(window);
    d_buttonFadeInAnimInst5 = animMgr.instantiateAnimation(buttonFadeInAnim);
    window = d_root->getChild("InnerButtonsContainer/ButtonQuit");
    d_buttonFadeInAnimInst5->setTargetWindow(window);

    CEGUI::Animation* loginContainerMoveInAnim = animMgr.getAnimation("LoginMoveIn");
    d_loginContainerMoveInInst = animMgr.instantiateAnimation(loginContainerMoveInAnim);
    d_loginContainerMoveInInst->setTargetWindow(d_loginContainer);

    CEGUI::Animation* naviButtonRightMoveInAnim = animMgr.getAnimation("NaviButtonRightMoveIn");
    d_naviButtonRightMoveInInst = animMgr.instantiateAnimation(naviButtonRightMoveInAnim);
    d_naviButtonRightMoveInInst->setTargetWindow(d_botNaviRightArrow);

    CEGUI::Animation* naviButtonLeftMoveInAnim = animMgr.getAnimation("NaviButtonLeftMoveIn");
    d_naviButtonLeftMoveInInst = animMgr.instantiateAnimation(naviButtonLeftMoveInAnim);
    d_naviButtonLeftMoveInInst->setTargetWindow(d_botNaviLeftArrow);

    CEGUI::Animation* naviBotMoveInAnim = animMgr.getAnimation("NaviBotCenterMoveIn");
    d_naviBotMoveInInst = animMgr.instantiateAnimation(naviBotMoveInAnim);
    d_naviBotMoveInInst->setTargetWindow(d_botNaviCenter);

    CEGUI::Animation* blendInAnim = animMgr.getAnimation("BlendIn");
    d_startButtonBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
    d_startButtonBlendInAnimInst->setTargetWindow(d_innerPartStartClickArea);

    d_navigationLabelBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
    d_navigationLabelBlendInAnimInst->setTargetWindow(d_navigationDestinationLabel);

    CEGUI::Animation* partialBlendOutAnim = animMgr.getAnimation("PartialBlendOut");

    d_leftArrowPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_leftArrowPartialBlendOutInst->setTargetWindow(d_botNaviLeftArrow);
    d_leftArrowBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_leftArrowBlendInInst->setTargetWindow(d_botNaviLeftArrow);
    d_rightArrowPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_rightArrowPartialBlendOutInst->setTargetWindow(d_botNaviRightArrow);
    d_rightArrowBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_rightArrowBlendInInst->setTargetWindow(d_botNaviRightArrow);
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    d_naviIconPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_naviIconPartialBlendOutInst->setTargetWindow(window);
    d_naviIconBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_naviIconBlendInInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer/ButtonOptions");
    d_buttonOptionsPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_buttonOptionsPartialBlendOutInst->setTargetWindow(window);
    d_buttonOptionsBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_buttonOptionsBlendInInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer/ButtonLoad");
    d_buttonLoadPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_buttonLoadPartialBlendOutInst->setTargetWindow(window);
    d_buttonLoadBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_buttonLoadBlendInInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer/ButtonSave");
    d_buttonSavePartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_buttonSavePartialBlendOutInst->setTargetWindow(window);
    d_buttonSaveBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_buttonSaveBlendInInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer/ButtonCharacters");

    d_buttonCharactersBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_buttonCharactersBlendInInst->setTargetWindow(window);
    d_buttonCharactersPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_buttonCharactersPartialBlendOutInst->setTargetWindow(window);
}


void GameMenuDemo::onEnteringSample()
{
    d_timeSinceStart = 0.0f;

    d_interactiveElementsAreActivated = true;

    //Reset labels and visibility
    d_topBarLabel->setText("");
    d_botBarLabel->setText("");

    //Set animations to end and apply their changes to the windows
    //For every animation instance that uses source properties
    //so the values will be taken correctly after starting them again
    d_buttonFadeInAnimInst1->setPosition(d_buttonFadeInAnimInst1->getDefinition()->getDuration());
    d_buttonFadeInAnimInst1->apply();
    d_buttonFadeInAnimInst2->setPosition(d_buttonFadeInAnimInst2->getDefinition()->getDuration());
    d_buttonFadeInAnimInst2->apply();
    d_buttonFadeInAnimInst3->setPosition(d_buttonFadeInAnimInst3->getDefinition()->getDuration());
    d_buttonFadeInAnimInst3->apply();
    d_buttonFadeInAnimInst4->setPosition(d_buttonFadeInAnimInst4->getDefinition()->getDuration());
    d_buttonFadeInAnimInst4->apply();
    d_buttonFadeInAnimInst5->setPosition(d_buttonFadeInAnimInst5->getDefinition()->getDuration());
    d_buttonFadeInAnimInst5->apply();
    d_loginContainerMoveInInst->setPosition(d_loginContainerMoveInInst->getDefinition()->getDuration());
    d_loginContainerMoveInInst->apply();
    d_naviButtonRightMoveInInst->setPosition(d_naviButtonRightMoveInInst->getDefinition()->getDuration());
    d_naviButtonRightMoveInInst->apply();
    d_naviButtonLeftMoveInInst->setPosition(d_naviButtonLeftMoveInInst->getDefinition()->getDuration());
    d_naviButtonLeftMoveInInst->apply();
    d_naviBotMoveInInst->setPosition(d_naviBotMoveInInst->getDefinition()->getDuration());
    d_naviBotMoveInInst->apply();

    d_loginContainer->setVisible(false);
    d_innerPartStartClickArea->setVisible(false);
    d_navigationDestinationLabel->setVisible(false);

    //Start the animations
    d_topBarAnimInst->start();
    d_botBarAnimInst->start();
    d_insideBlendInAnimInst->start();
    d_insideImage3RotateInInst->start();
    d_insideImage4RotateInInst->start();
    d_insideImageRingsContainerSizeInInst->start();
    d_buttonFadeInAnimInst1->start();
    d_buttonFadeInAnimInst2->start();
    d_buttonFadeInAnimInst3->start();
    d_buttonFadeInAnimInst4->start();
    d_buttonFadeInAnimInst5->start();
    d_naviButtonRightMoveInInst->start();
    d_naviButtonLeftMoveInInst->start();
    d_naviBotMoveInInst->start();
}

void GameMenuDemo::update(float passedTime)
{
    d_timeSinceStart += passedTime;

    static const CEGUI::String firstPart = "Connection established...";
    static const CEGUI::String secondPart = "Warning! User Authentication required!";
    const float firstStartDelay = 5.f;
    const float secondStartDelay = 11.f;
    const float loginDisplayStartDelay = 15.77f;

    CEGUI::String finalText;

    int firstPartTypeProgress = static_cast<int>((d_timeSinceStart - firstStartDelay) / 0.08f);
    if(firstPartTypeProgress > 0)
        finalText += firstPart.substr(0, std::min<unsigned int>(firstPart.length(), firstPartTypeProgress));

    int secondPartTypeProgress = static_cast<int>((d_timeSinceStart - secondStartDelay) / 0.08f);
    if(secondPartTypeProgress > 0)
        finalText += "\n" + secondPart.substr(0, std::min<unsigned int>(secondPart.length(), secondPartTypeProgress));

    finalText += "[font='DejaVuSans-12']";

    static double blinkStartDelay = 3.6f;
    double blinkPeriod = 0.8;
    double blinkTime = std::modf(static_cast<double>(d_timeSinceStart), &blinkPeriod);
    if(blinkTime > 0.55 || d_timeSinceStart < blinkStartDelay)
        finalText += "[colour='00000000']";

    finalText += reinterpret_cast<const encoded_char*>("❚");

    d_botBarLabel->setText(finalText);


    if(d_timeSinceStart >= loginDisplayStartDelay && !d_loginContainer->isVisible())
        d_loginContainerMoveInInst->start();

    if(d_timeSinceStart >= secondStartDelay && d_interactiveElementsAreActivated)
    {
        blendOutInteractiveElements();
    }
}


bool GameMenuDemo::handleRootWindowUpdate(const CEGUI::EventArgs& args)
{
    const CEGUI::UpdateEventArgs& updateArgs = static_cast<const CEGUI::UpdateEventArgs&>(args);
    float passedTime = updateArgs.d_timeSinceLastFrame;

    update(passedTime);

    return false;
}

bool GameMenuDemo::handleLoginAcceptButtonClicked(const CEGUI::EventArgs& args)
{
    d_innerPartStartClickArea->setAlpha(0.f);
    d_startButtonBlendInAnimInst->start();

    blendInInteractiveElements();

    return false;
}

bool GameMenuDemo::handleInnerPartStartClickAreaClick(const CEGUI::EventArgs& args)
{
    d_navigationLabelBlendInAnimInst->start();

    return false;
}

void GameMenuDemo::setupWindows()
{
    d_botBarLabel = d_root->getChild("BotBar/BotBarLabel");
    d_topBarLabel = d_root->getChild("TopBar/TopBarLabel");

    d_loginContainer = d_root->getChild("LoginContainer");

    d_botNaviLeftArrow = d_root->getChild("BotNavigationContainer/NaviLeft");
    d_botNaviRightArrow = d_root->getChild("BotNavigationContainer/NaviRight");

    d_botNaviCenter = d_root->getChild("BotNavigationContainer/NaviCenterContainer");

    d_loginAcceptButton = d_loginContainer->getChild("AcceptButton");
    d_loginAcceptButton->subscribeEvent(CEGUI::Window::EventMouseClick, Event::Subscriber(&GameMenuDemo::handleLoginAcceptButtonClicked, this));

    d_innerPartStartClickArea = d_root->getChild("InnerPartContainer/InsideStartClickArea");   
    d_innerPartStartClickArea->subscribeEvent(CEGUI::Window::EventMouseClick, Event::Subscriber(&GameMenuDemo::handleInnerPartStartClickAreaClick, this));

    d_navigationDestinationLabel = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");

    d_root->subscribeEvent(CEGUI::Window::EventUpdated, Event::Subscriber(&GameMenuDemo::handleRootWindowUpdate, this));
}

void GameMenuDemo::blendOutInteractiveElements()
{
    d_leftArrowPartialBlendOutInst->start();
    d_rightArrowPartialBlendOutInst->start();
    d_naviIconPartialBlendOutInst->start();

    d_buttonLoadPartialBlendOutInst->start();
    d_buttonOptionsPartialBlendOutInst->start();
    d_buttonSavePartialBlendOutInst->start();
    d_buttonCharactersPartialBlendOutInst->start();

    d_interactiveElementsAreActivated = false;
}

void GameMenuDemo::blendInInteractiveElements()
{
    d_leftArrowPartialBlendOutInst->pause();

    d_leftArrowBlendInInst->start();
    d_rightArrowBlendInInst->start();
    d_naviIconBlendInInst->start();

    d_buttonLoadBlendInInst->start();
    d_buttonOptionsBlendInInst->start();
    d_buttonSaveBlendInInst->start();
    d_buttonCharactersBlendInInst->start();

    d_interactiveElementsAreActivated = true;
}

/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static GameMenuDemo sample;
    return sample;
}