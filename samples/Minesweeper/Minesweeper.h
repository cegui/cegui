/***********************************************************************
created:    2014-08-23
author:     Luca Ebach <lucaebach@gmail.com>
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _Minesweeper_h_
#define _Minesweeper_h_

#include "Sample.h"
#include "Minesweeper_Timer.h"

#include <ctime>

struct Location
{
    size_t d_row;
    size_t d_col;
};
const size_t MinesweeperSize = 10;
const size_t MineCount = 15;

class MinesweeperSample : public Sample
{
public:
    MinesweeperSample();
    virtual ~MinesweeperSample() {}

    // method to initialse the samples windows and events.
    bool initialise(CEGUI::GUIContext* guiContext) override;

    // method to perform any required cleanup operations.
    void deinitialise() override;

protected:
    // Handle new game
    bool handleGameStartClicked(const CEGUI::EventArgs& event);
    // Handle click on a button of the board
    bool handleMineButtonClicked(const CEGUI::EventArgs& event);
    // Handle cursor press & hold on a button of the board
    bool handleMinePointerPressHold(const CEGUI::EventArgs& event);
    // Update the timer if needed
    bool handleUpdateTimer(const CEGUI::EventArgs& event);
    // reset the board
    void boardReset();
    // place mine and computes mine neighborhood
    void boardPositionMines();
    // Test whether the player wins or not
    bool isGameWin();
    // Call this function if the game is finished
    void gameEnd(bool victory);
    // When a button is clicked
    bool boardDiscover(const Location& location);
    // Store all buttons needed
    CEGUI::PushButton* d_buttons[MinesweeperSize][MinesweeperSize];
    // Store button location
    Location d_buttonsMapping[MinesweeperSize][MinesweeperSize];
    // Store the value of the board itself
    size_t d_board[MinesweeperSize][MinesweeperSize];
    // Store the number of case the user discovered
    size_t d_boardCellDiscovered;
    // Store the number of mine to find
    CEGUI::Editbox* d_counter;
    // Store the number of second elapsed
    CEGUI::Editbox* d_timer;
    // Used to display the result text
    CEGUI::Window* d_result;

    // True if the game is started false otherwise
    bool d_gameStarted;
    // time at the start of the game
    clock_t d_timerStartTime;
    // current value of the timer
    clock_t d_timerValue;
    // Custom window type to force refresh of the timer
    Timer* d_alarm;
};

#endif // _Minesweeper_h_