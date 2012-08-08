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

#define _USE_MATH_DEFINES
#include "math.h"


using namespace CEGUI;


const float GameMenuDemo::s_firstStartDelay = 5.f;
const float GameMenuDemo::s_secondStartDelay = 11.f;
const float GameMenuDemo::s_loginDisplayStartDelay = 15.77f;

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

    d_interactiveElementsWereInitiallyBlendedOut = false;
    d_interactivePlanetElementsAreEnabled = false;
    d_navigationIsEnabled = false;
    d_loginWasAccepted = false;
    d_mouseIsHoveringNavi = false;

    d_currentWriteFocus = WF_BotBar;

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

    CEGUI::Animation* loginContainerMoveOutAnim = animMgr.getAnimation("LoginMoveOut");
    d_loginContainerMoveOutInst = animMgr.instantiateAnimation(loginContainerMoveOutAnim);
    d_loginContainerMoveOutInst->setTargetWindow(d_loginContainer);


    CEGUI::Animation* naviButtonRightMoveInAnim = animMgr.getAnimation("NaviButtonRightMoveIn");
    d_naviButtonRightMoveInInst = animMgr.instantiateAnimation(naviButtonRightMoveInAnim);
    window = d_root->getChild("BotNavigationContainer/RightArrow");
    d_naviButtonRightMoveInInst->setTargetWindow(window);

    CEGUI::Animation* naviButtonLeftMoveInAnim = animMgr.getAnimation("NaviButtonLeftMoveIn");
    d_naviButtonLeftMoveInInst = animMgr.instantiateAnimation(naviButtonLeftMoveInAnim);
    window = d_root->getChild("BotNavigationContainer/LeftArrow");
    d_naviButtonLeftMoveInInst->setTargetWindow(window);

    CEGUI::Animation* naviBotMoveInAnim = animMgr.getAnimation("NaviBotCenterMoveIn");
    d_naviBotMoveInInst = animMgr.instantiateAnimation(naviBotMoveInAnim);
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer");
    d_naviBotMoveInInst->setTargetWindow(window);

    CEGUI::Animation* blendInAnim = animMgr.getAnimation("BlendIn");
    d_startButtonBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
    d_startButtonBlendInAnimInst->setTargetWindow(d_startButtonClickArea);

    CEGUI::Animation* partialBlendOutAnim = animMgr.getAnimation("PartialBlendOut");

    window = d_root->getChild("BotNavigationContainer");
    d_naviPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_naviPartialBlendOutInst->setTargetWindow(window);
    d_naviBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_naviBlendInInst->setTargetWindow(window);
    window = d_root->getChild("InnerButtonsContainer");
    d_centerButtonsPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_centerButtonsPartialBlendOutInst->setTargetWindow(window);
    d_centerButtonsBlendInInst = animMgr.instantiateAnimation(blendInAnim);
    d_centerButtonsBlendInInst->setTargetWindow(window);

    
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");
    d_navigationLabelPartialBlendOutInst = animMgr.instantiateAnimation(partialBlendOutAnim);
    d_navigationLabelPartialBlendOutInst->setTargetWindow(window);
    d_navigationLabelBlendInAnimInst = animMgr.instantiateAnimation(blendInAnim);
    d_navigationLabelBlendInAnimInst->setTargetWindow(window);


    CEGUI::Animation* travelRotateInAnim = animMgr.getAnimation("TravelRotateIn");
    CEGUI::AnimationInstance* travelRotateInInst = animMgr.instantiateAnimation(travelRotateInAnim);
    travelRotateInInst->setTargetWindow(d_navigationTravelIcon);
    CEGUI::Animation* travelRotateOutAnim = animMgr.getAnimation("TravelRotateOut");
    CEGUI::AnimationInstance* travelRotateOutInst = animMgr.instantiateAnimation(travelRotateOutAnim);
    travelRotateOutInst->setTargetWindow(d_navigationTravelIcon);

    CEGUI::Animation* blendOutAnim = animMgr.getAnimation("FullBlendOut");
    d_botBarLabelBlendOutInst = animMgr.instantiateAnimation(blendOutAnim);
    d_botBarLabelBlendOutInst->setTargetWindow(d_botBarLabel);

    CEGUI::Animation* loopRotateRightAnim = animMgr.getAnimation("LoopRotateRight");
    CEGUI::Animation* stopRotateAnim = animMgr.getAnimation("StopRotate");
    CEGUI::AnimationInstance* loopRotateRightAnimInst = animMgr.instantiateAnimation(loopRotateRightAnim);
    loopRotateRightAnimInst->setTargetWindow(d_navigationSelectionIcon);
    CEGUI::AnimationInstance* loopRotateLeftAnimInst = animMgr.instantiateAnimation(stopRotateAnim);
    loopRotateLeftAnimInst->setTargetWindow(d_navigationSelectionIcon);

    window = d_root->getChild("BotNavigationContainer/RightArrow");
    CEGUI::Animation* naviArrowRightTwitch = animMgr.getAnimation("NaviArrowRightTwitch");
    CEGUI::AnimationInstance* naviArrowRightTwitchInst = animMgr.instantiateAnimation(naviArrowRightTwitch);
    naviArrowRightTwitchInst->setTargetWindow(window);
    CEGUI::Animation* naviArrowRightReturn = animMgr.getAnimation("NaviArrowRightReturn");
    CEGUI::AnimationInstance* naviArrowRightReturnInst = animMgr.instantiateAnimation(naviArrowRightReturn);
    naviArrowRightReturnInst->setTargetWindow(window);

    window = d_root->getChild("BotNavigationContainer/LeftArrow");
    CEGUI::Animation* naviArrowLeftTwitch = animMgr.getAnimation("NaviArrowLeftTwitch");
    CEGUI::AnimationInstance* naviArrowLeftTwitchInst = animMgr.instantiateAnimation(naviArrowLeftTwitch);
    naviArrowLeftTwitchInst->setTargetWindow(window);
    CEGUI::Animation* naviArrowLeftReturn = animMgr.getAnimation("NaviArrowLeftReturn");
    CEGUI::AnimationInstance* naviArrowLeftReturnInst = animMgr.instantiateAnimation(naviArrowLeftReturn);
    naviArrowLeftReturnInst->setTargetWindow(window);

    window = d_root->getChild("InnerButtonsContainer/PopupLinesSave");
    CEGUI::Animation* sizeGrowth = animMgr.getAnimation("SizeGrowth");
    d_popupLinesSaveAnimInst = animMgr.instantiateAnimation(sizeGrowth);
    d_popupLinesSaveAnimInst->setTarget(window);
}


void GameMenuDemo::onEnteringSample()
{
    d_navigationTravelIcon->setEnabled(false);

    d_timeSinceStart = 0.0f;

    d_interactiveElementsWereInitiallyBlendedOut = false;
    d_interactivePlanetElementsAreEnabled = false;
    d_loginWasAccepted = false;

    d_botBarLabel->setAlpha(1.f);
    d_botNaviContainer->setAlpha(1.f);
    d_botNaviContainer->setEnabled(false);

    d_centerButtonsBlendInInst->setPosition(d_centerButtonsBlendInInst->getDefinition()->getDuration());
    d_centerButtonsBlendInInst->apply();

    d_currentWriteFocus = WF_BotBar;

    //Reset labels and visibility
    d_topBarLabel->setText("");
    d_botBarLabel->setText("");


    d_root->getChild("InnerButtonsContainer/PopupLinesSave")->setVisible(false);

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
    d_startButtonClickArea->setVisible(false);

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

    updateIntroText();
    if(d_loginWasAccepted)
        updateLoginWelcomeText(passedTime);

    if(d_timeSinceStart >= s_loginDisplayStartDelay && !d_loginContainer->isVisible())
        d_loginContainerMoveInInst->start();

    if(d_timeSinceStart >= s_secondStartDelay && !d_interactiveElementsWereInitiallyBlendedOut)
    {
        disableInteractivePlanetElements();
        disableNavigationBarElements();
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
    d_startButtonClickArea->setAlpha(0.f);
    d_startButtonBlendInAnimInst->start();

    enableNavigationBarElements();

    d_loginContainerMoveOutInst->start();
    d_navigationTravelIcon->setEnabled(true);

    CEGUI::Editbox* loginEditbox = static_cast<CEGUI::Editbox*>(d_root->getChild("LoginContainer/LoginEditbox"));
    d_userName = loginEditbox->getText();
    
    d_timeSinceLoginAccepted = 0.f;
    d_loginWasAccepted = true;
    d_currentWriteFocus = WF_TopBar;

    d_botBarLabelBlendOutInst->start();

    d_botNaviContainer->setEnabled(true);

    return false;
}

bool GameMenuDemo::handleInnerPartStartClickAreaClick(const CEGUI::EventArgs& args)
{
    if(!d_interactivePlanetElementsAreEnabled)
        enableInteractivePlanetElements();

    d_startButtonBlendInAnimInst->pause();
    d_startButtonClickArea->setVisible(false);

    return false;
}


bool GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange(const CEGUI::EventArgs& args)
{
    bool mouseIsHovering = false;
    CEGUI::Window* window;

    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");
    mouseIsHovering |= window->isMouseContainedInArea();
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    mouseIsHovering |= window->isMouseContainedInArea();
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NaviBotSelectionIcon");
    mouseIsHovering |= window->isMouseContainedInArea();
    mouseIsHovering |= d_botNaviRightArrowArea->isMouseContainedInArea();
    mouseIsHovering |= d_botNaviLeftArrowArea->isMouseContainedInArea();

    //We fire an event to trigger the animation depending on if the mouse hovers a critical
    //window or not. Additionally we perform a check to not fire an event for an animation that is already running
    if(d_mouseIsHoveringNavi != mouseIsHovering)
    {
        d_mouseIsHoveringNavi = mouseIsHovering;

        if(mouseIsHovering)
        {
            d_navigationSelectionIcon->fireEvent("StartRotate", CEGUI::EventArgs());
            d_navigationTravelIcon->fireEvent("StartRotate", CEGUI::EventArgs());
        }
        else
        {
            d_navigationSelectionIcon->fireEvent("StopRotate", CEGUI::EventArgs());
            d_navigationTravelIcon->fireEvent("StopRotate", CEGUI::EventArgs());
        }
    }

    return false;
}


float getAngle(const CEGUI::Quaternion& quat)
{
    return 2.f * std::acos(quat.d_w);
}


bool GameMenuDemo::handleNaviSelectionIconAnimStart(const CEGUI::EventArgs& args)
{
    const CEGUI::AnimationEventArgs& animArgs = static_cast<const CEGUI::AnimationEventArgs&>(args);

    CEGUI::AnimationInstance* animInst = animArgs.instance;

    if(animInst->getDefinition() == CEGUI::AnimationManager::getSingleton().getAnimation("LoopRotateRight"))
    {
        CEGUI::Quaternion curRotation = animInst->getTarget()->getProperty<CEGUI::Quaternion>("Rotation");

        float curAngle = getAngle(curRotation);
        float progress = curAngle / (2.f * static_cast<float>(M_PI));
        animInst->setPosition(progress * animInst->getDefinition()->getDuration());
    }

    return false;
}

bool GameMenuDemo::handleMouseEntersLeftArrowArea(const CEGUI::EventArgs& args)
{
    d_root->getChild("BotNavigationContainer/LeftArrow")->fireEvent("StartTwitching", CEGUI::EventArgs());

    return false;
}

bool GameMenuDemo::handleMouseLeavesLeftArrowArea(const CEGUI::EventArgs& args)
{
    d_root->getChild("BotNavigationContainer/LeftArrow")->fireEvent("EndTwitching", CEGUI::EventArgs());

    return false;
}

bool GameMenuDemo::handleMouseEntersRightArrowArea(const CEGUI::EventArgs& args)
{
    d_root->getChild("BotNavigationContainer/RightArrow")->fireEvent("StartTwitching", CEGUI::EventArgs());

    return false;
}

bool GameMenuDemo::handleMouseLeavesRightArrowArea(const CEGUI::EventArgs& args)
{
    d_root->getChild("BotNavigationContainer/RightArrow")->fireEvent("EndTwitching", CEGUI::EventArgs());

    return false;
}


bool GameMenuDemo::handleStartPopupLinesSaveDisplay(const CEGUI::EventArgs& args)
{
    d_popupLinesSaveAnimInst->start();
    
    return false;
}

void GameMenuDemo::setupWindows()
{
    d_botBarLabel = d_root->getChild("BotBar/BotBarLabel");
    d_topBarLabel = d_root->getChild("TopBar/TopBarLabel");

    d_loginContainer = d_root->getChild("LoginContainer");

    d_botNaviLeftArrowArea = d_root->getChild("BotNavigationContainer/LeftArrowArea");
    d_botNaviRightArrowArea = d_root->getChild("BotNavigationContainer/RightArrowArea");
    d_botNaviLeftArrowArea->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleMouseEntersLeftArrowArea, this));
    d_botNaviLeftArrowArea->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleMouseLeavesLeftArrowArea, this));
    d_botNaviRightArrowArea->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleMouseEntersRightArrowArea, this));
    d_botNaviRightArrowArea->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleMouseLeavesRightArrowArea, this));

    CEGUI::Window* buttonSave = d_root->getChild("InnerButtonsContainer/ButtonSave");
    buttonSave->subscribeEvent(CEGUI::Window::EventMouseClick, Event::Subscriber(&GameMenuDemo::handleStartPopupLinesSaveDisplay, this));

    d_botNaviContainer = d_root->getChild("BotNavigationContainer");

    d_botNaviCenter = d_root->getChild("BotNavigationContainer/NaviCenterContainer");

    d_loginAcceptButton = d_loginContainer->getChild("AcceptButton");
    d_loginAcceptButton->subscribeEvent(CEGUI::Window::EventMouseClick, Event::Subscriber(&GameMenuDemo::handleLoginAcceptButtonClicked, this));

    d_startButtonClickArea = d_root->getChild("InnerPartContainer/InsideStartClickArea");   
    d_startButtonClickArea->subscribeEvent(CEGUI::Window::EventMouseClick, Event::Subscriber(&GameMenuDemo::handleInnerPartStartClickAreaClick, this));

    d_navigationTravelIcon = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    d_navigationSelectionIcon = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NaviBotSelectionIcon");

    d_root->subscribeEvent(CEGUI::Window::EventUpdated, Event::Subscriber(&GameMenuDemo::handleRootWindowUpdate, this));

    d_navigationSelectionIcon->subscribeEvent(CEGUI::AnimationInstance::EventAnimationStarted, Event::Subscriber(&GameMenuDemo::handleNaviSelectionIconAnimStart, this));

    setupNaviIconAnimationEventHandlers();
}

void GameMenuDemo::disableInteractivePlanetElements()
{
    d_centerButtonsBlendInInst->pause();
    d_centerButtonsPartialBlendOutInst->start();


    d_interactiveElementsWereInitiallyBlendedOut = true;
}

void GameMenuDemo::enableInteractivePlanetElements()
{
    d_centerButtonsPartialBlendOutInst->pause();
    d_centerButtonsBlendInInst->start();
   
    d_interactivePlanetElementsAreEnabled = true;
}

void GameMenuDemo::disableNavigationBarElements()
{
    d_naviBlendInInst->pause();
    d_naviPartialBlendOutInst->start();

    d_navigationLabelBlendInAnimInst->stop();
    d_navigationLabelPartialBlendOutInst->start();

    d_navigationIsEnabled = false;
}

void GameMenuDemo::enableNavigationBarElements()
{
    d_naviPartialBlendOutInst->pause();
    d_naviBlendInInst->start();

    d_navigationLabelPartialBlendOutInst->stop();
    d_navigationLabelBlendInAnimInst->start();

    d_navigationIsEnabled = true;
}

void GameMenuDemo::updateIntroText()
{
    static const CEGUI::String firstPart = "Connection established...";
    static const CEGUI::String secondPart = "Warning! User Authentication required!";

    CEGUI::String finalText;

    int firstPartTypeProgress = static_cast<int>((d_timeSinceStart - s_firstStartDelay) / 0.08f);
    if(firstPartTypeProgress > 0)
        finalText += firstPart.substr(0, std::min<unsigned int>(firstPart.length(), firstPartTypeProgress));

    int secondPartTypeProgress = static_cast<int>((d_timeSinceStart - s_secondStartDelay) / 0.08f);
    if(secondPartTypeProgress > 0)
        finalText += "\n" + secondPart.substr(0, std::min<unsigned int>(secondPart.length(), secondPartTypeProgress));

    finalText += "[font='DejaVuSans-12']";

    static double blinkStartDelay = 3.6f;
    double blinkPeriod = 0.8;
    double blinkTime = std::modf(static_cast<double>(d_timeSinceStart), &blinkPeriod);
    if(blinkTime > 0.55 || d_timeSinceStart < blinkStartDelay || d_currentWriteFocus == WF_TopBar)
        finalText += "[colour='00000000']";

    finalText += reinterpret_cast<const encoded_char*>("❚");

    d_botBarLabel->setText(finalText);
}

void GameMenuDemo::updateLoginWelcomeText(float passedTime)
{
    d_timeSinceLoginAccepted += passedTime;
    if(d_timeSinceLoginAccepted <= 0.f)
        return;

    static const CEGUI::String firstPart = "Welcome ";
    CEGUI::String displayText = firstPart + d_userName;
    CEGUI::String finalText;

    int progress = static_cast<int>(d_timeSinceLoginAccepted / 0.08f);
    if(progress > 0)
        finalText += displayText.substr(0, std::min<unsigned int>(displayText.length(), progress));

    finalText += "[font='DejaVuSans-12']";

    static double blinkStartDelay = 3.6f;
    double blinkPeriod = 0.8;
    double blinkTime = std::modf(static_cast<double>(d_timeSinceStart), &blinkPeriod);
    if(blinkTime > 0.55 || d_timeSinceStart < blinkStartDelay || d_currentWriteFocus == WF_BotBar)
        finalText += "[colour='00000000']";

    finalText += reinterpret_cast<const encoded_char*>("❚");

    d_topBarLabel->setText(finalText);
}

void GameMenuDemo::setupNaviIconAnimationEventHandlers()
{
    d_botNaviLeftArrowArea->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    d_botNaviLeftArrowArea->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    d_botNaviRightArrowArea->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    d_botNaviRightArrowArea->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    CEGUI::Window* window;
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationLabel");
    window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NavigationIcon");
    window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    window = d_root->getChild("BotNavigationContainer/NaviCenterContainer/NaviBotSelectionIcon");
    window->subscribeEvent(CEGUI::Window::EventMouseEntersArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
    window->subscribeEvent(CEGUI::Window::EventMouseLeavesArea, Event::Subscriber(&GameMenuDemo::handleCheckIfNaviIconAnimationNeedsChange, this));
}

/*************************************************************************
Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static GameMenuDemo sample;
    return sample;
}