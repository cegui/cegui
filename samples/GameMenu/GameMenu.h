/***********************************************************************
created:    31/7/2012
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
#ifndef _Game_Menu_Demo_
#define _Game_Menu_Demo_

#include "SampleBase.h"

namespace CEGUI
{
    class GUIContext;
}

enum WriteFocus
{
    WF_TopBar,
    WF_BotBar,

    WF_Count
};


// Sample class
class GameMenuDemo : public Sample
{
public:
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    // method to perform any required cleanup operations.
    virtual void deinitialise();

    virtual void onEnteringSample();

    virtual void update(float timeSinceLastUpdate);


protected:
    void setupWindows();

    void setupInnerButtonsSubOptionsLabels();
    void setupNaviArrowWindows();
    void setupButtonClickHandlers();
    void setupNaviIconAnimationEventHandlers();
    void setupAnimations();

    void setupSelectionIconAnimations();
    void setupPopupLinesAnimations();
    void resetAnimations();
    void startEntranceAnimations();

    void updateIntroText();
    void updateLoginWelcomeText();
    void updateLoginStartButtonText();
    void disableInteractivePlanetElements();
    void enableInteractivePlanetElements();
    void enableNavigationBarElements();
    void disableNavigationBarElements();

    bool handleLoginAcceptButtonClicked(const CEGUI::EventArgs& args);
    bool handleInnerPartStartClickAreaClick(const CEGUI::EventArgs& args);
    bool handleCheckIfNaviIconAnimationNeedsChange(const CEGUI::EventArgs& args);
    bool handleNaviSelectionIconAnimStart(const CEGUI::EventArgs& args);

    bool handleMouseEntersLeftArrowArea(const CEGUI::EventArgs& args);
    bool handleMouseLeavesLeftArrowArea(const CEGUI::EventArgs& args);
    bool handleMouseEntersRightArrowArea(const CEGUI::EventArgs& args);
    bool handleMouseLeavesRightArrowArea(const CEGUI::EventArgs& args);

    bool handleStartPopupLinesSaveDisplay(const CEGUI::EventArgs& args);
    bool handleStartPopupLinesLoadDisplay(const CEGUI::EventArgs& args);
    bool handleStartPopupLinesCharactersDisplay(const CEGUI::EventArgs& args);
    bool handleStartPopupLinesOptionsDisplay(const CEGUI::EventArgs& args);
    bool handleStartPopupLinesQuitDisplay(const CEGUI::EventArgs& args);

    bool handleInnerButtonsLabelEntered(const CEGUI::EventArgs& args);

    CEGUI::Window* getIconWindowFromLabel(CEGUI::Window* window);
    bool handleInnerButtonsLabelLeft(const CEGUI::EventArgs& args);

    void makeAllSelectionIconsInvisible();
    void stopStartPopupLinesAnimations();

    static const float s_firstStartDelay;
    static const float s_secondStartDelay;
    static const float s_loginDisplayStartDelay;


    CEGUI::GUIContext* d_guiContext;
    CEGUI::Window* d_root;

    float d_timeSinceStart;

    float d_timeSinceLoginAccepted;

    bool d_interactiveElementsWereInitiallyBlendedOut;
    bool d_interactivePlanetElementsAreEnabled;
    bool d_navigationIsEnabled;
    bool d_loginWasAccepted;
    bool d_mouseIsHoveringNavi;
    bool d_startButtonClicked;

    WriteFocus d_currentWriteFocus;

    CEGUI::String d_userName;

    CEGUI::AnimationInstance* d_topBarAnimInst;
    CEGUI::AnimationInstance* d_botBarAnimInst;
    CEGUI::AnimationInstance* d_insideBlendInAnimInst;
    CEGUI::AnimationInstance* d_insideImage3RotateInInst;
    CEGUI::AnimationInstance* d_insideImage4RotateInInst;
    CEGUI::AnimationInstance* d_insideImageRingsContainerSizeInInst;

    CEGUI::AnimationInstance* d_buttonFadeInAnimInst1;
    CEGUI::AnimationInstance* d_buttonFadeInAnimInst2;
    CEGUI::AnimationInstance* d_buttonFadeInAnimInst3;
    CEGUI::AnimationInstance* d_buttonFadeInAnimInst4;
    CEGUI::AnimationInstance* d_buttonFadeInAnimInst5;

    CEGUI::AnimationInstance* d_loginContainerMoveInInst;
    CEGUI::AnimationInstance* d_loginContainerMoveOutInst;
    CEGUI::AnimationInstance* d_naviButtonLeftMoveInInst;
    CEGUI::AnimationInstance* d_naviButtonRightMoveInInst;
    CEGUI::AnimationInstance* d_naviBotMoveInInst;
    CEGUI::AnimationInstance* d_startButtonBlendInAnimInst;
    CEGUI::AnimationInstance* d_navigationLabelBlendInAnimInst;
    CEGUI::AnimationInstance* d_navigationLabelPartialBlendOutInst;

    CEGUI::AnimationInstance* d_naviPartialBlendOutInst;
    CEGUI::AnimationInstance* d_naviBlendInInst;
    CEGUI::AnimationInstance* d_centerButtonsPartialBlendOutInst;
    CEGUI::AnimationInstance* d_centerButtonsBlendInInst;

    CEGUI::AnimationInstance* d_botBarLabelBlendOutInst;

    CEGUI::AnimationInstance* d_popupLinesSaveAnimInst;
    CEGUI::AnimationInstance* d_popupLinesLoadAnimInst;
    CEGUI::AnimationInstance* d_popupLinesCharactersAnimInst;
    CEGUI::AnimationInstance* d_popupLinesOptionsAnimInst;
    CEGUI::AnimationInstance* d_popupLinesQuitAnimInst;

    CEGUI::Window* d_topBarLabel;
    CEGUI::Window* d_botBarLabel;
    CEGUI::Window* d_loginContainer;
    CEGUI::Window* d_botNaviLeftArrowArea;
    CEGUI::Window* d_botNaviRightArrowArea;
    CEGUI::Window* d_botNaviCenter;
    CEGUI::Window* d_loginAcceptButton;
    CEGUI::Window* d_startButtonClickArea;
    CEGUI::Window* d_navigationTravelIcon;
    CEGUI::Window* d_navigationSelectionIcon;
    CEGUI::Window* d_botNaviContainer;



};

#endif
