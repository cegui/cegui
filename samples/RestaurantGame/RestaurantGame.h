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
#ifndef _Restaurant_Game_h
#define _Restaurant_Game_h

#include "Sample.h"
#include <glm/glm.hpp>
#include <vector>

namespace CEGUI
{
    class GUIContext;
}

struct GamePlate;

enum SelectedWeapon
{
    SW_Spoon,
    SW_Fork,
    SW_Knife,

    SW_Count
};

// Sample class
class RestaurantGameSample : public Sample
{
public:
    RestaurantGameSample();
    virtual ~RestaurantGameSample() {}

    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext) override;

    void initGame();
    void setupCursor();
    // method to perform any required cleanup operations.
    void deinitialise() override;

    void onEnteringSample() override;

    void update(float timeSinceLastUpdate) override;

    CEGUI::Window* spawnPlate();

    static const CEGUI::String s_imageNamePlate;
    static const CEGUI::String s_imageNameBread;
    static const CEGUI::String s_imageNamePoo;
    static const CEGUI::String s_imageNamePrizza;
    static const CEGUI::String s_imageNameSteak;
    static const CEGUI::String s_imageNameVegPeople;
    static const CEGUI::String s_imageNameVegFruits;


protected:
    static const CEGUI::String& getRandomGameImage();

    void updateCursor();
    void updatePlates(float timeSinceLastUpdate);

    bool handlePlateWindowActivated(const CEGUI::EventArgs& args);
    bool handleScorePopupAnimationEnded(const CEGUI::EventArgs& args);
    bool handleWeaponLeftArrowClicked(const CEGUI::EventArgs& args);
    bool handleWeaponRightArrowClicked(const CEGUI::EventArgs& args);
    bool handleRestartButtonClicked(const CEGUI::EventArgs& args);

    void updateScoreWindow();
    void createScorePopup(const glm::vec2& cursor_pos, std::int32_t points);

    void handleLivesChanged();

    void delayDestroyWindows();
    void selectedWeapon(SelectedWeapon weapon);

    CEGUI::GUIContext* d_guiContext;
    CEGUI::Window* d_root;
    CEGUI::Window* d_rootIngame;
    CEGUI::Window* d_rootGameOver;

    CEGUI::Window* d_cursorWnd;

    std::int32_t d_score;

    std::vector<GamePlate*> d_gamePlates;
    std::vector<CEGUI::Window*> d_delayDestroyWindows;

    CEGUI::ProgressBar* d_lifeBar;
    int d_lives;
    SelectedWeapon d_selectedWeapon;
};

#endif
