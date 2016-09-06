/***********************************************************************
created:    05/08/2006
author:     Olivier Delannoy (Dalfy)
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
#include "SampleBase.h"
#include "CEGUI/CEGUI.h"
#include "Minesweeper_Timer.h"
#include <ctime>
#include <cstdlib>
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
    // method to initialse the samples windows and events.
    virtual bool initialise(CEGUI::GUIContext* guiContext);

    // method to perform any required cleanup operations.
    virtual void deinitialise();

protected:
    // Handle new game
    bool handleGameStartClicked(const CEGUI::EventArgs& event);
    // Handle click on a button of the board
    bool handleMineButtonClicked(const CEGUI::EventArgs& event);
    // Handle mouse button down on a button of the board
    bool handleMineButtonDown(const CEGUI::EventArgs& event);
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

//////////////////////////////////////////////////////////////////////////
/*************************************************************************

MinesweeperSample class

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
Sample specific initialisation goes here.
*************************************************************************/
bool MinesweeperSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_usedFiles = CEGUI::String(__FILE__);

    // Register Timer Window
    WindowFactoryManager::getSingleton().addFactory( &getTimerFactory() );

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    d_gameStarted = false;

    // Get window manager which we wil use for a few jobs here.
    WindowManager& winMgr = WindowManager::getSingleton();

    // Load the scheme to initialse the VanillaSkin which we use in this sample
    SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    guiContext->setDefaultTooltipType("TaharezLook/Tooltip");

    // set default mouse image
    guiContext->getMouseCursor().setDefaultImage("Vanilla-Images/MouseArrow");

    // load an image to use as a background
    if( !ImageManager::getSingleton().isDefined("SpaceBackgroundImage") )
        ImageManager::getSingleton().addFromImageFile("SpaceBackgroundImage", "SpaceBackground.jpg");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("Vanilla/StaticImage");

    // set area rectangle
    background->setArea(URect(cegui_reldim(0), cegui_reldim(0), cegui_reldim(1), cegui_reldim(1)));

    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");

    // set the background image
    background->setProperty("Image", "SpaceBackgroundImage");

    // install this as the root GUI sheet
    guiContext->setRootWindow(background);
    d_alarm = (Timer*)winMgr.createWindow("Timer");
    background->addChild(d_alarm);
    d_alarm->setDelay(0.5); // Tick each 0.5 seconds

    // create the game frame
    Window* frame = winMgr.createWindow("Vanilla/FrameWindow");
    d_alarm->addChild(frame);
    frame->setXPosition(UDim(0.3f, 0.0f));
    frame->setYPosition(UDim(0.15f, 0.0f));
    frame->setWidth(UDim(0.4f, 0.0f)); 
    frame->setHeight(UDim(0.7f, 0.0f)); 
    frame->setText("CEGUI Minesweeper");

    // create the action panel
    Window* action = winMgr.createWindow("DefaultWindow");
    frame->addChild(action);
    action->setXPosition(UDim(0.03f, 0.0f));
    action->setYPosition(UDim(0.10f, 0.0f));
    action->setWidth(UDim(0.94f, 0.0f));
    action->setHeight(UDim(0.1f, 0.0f));
    d_counter = (Editbox*)winMgr.createWindow("Vanilla/Editbox", "mine_counter");
    action->addChild(d_counter);
    d_counter->setText("0");
    d_counter->setTooltipText("Number of mine");
    d_counter->setReadOnly(true);
    d_counter->setXPosition(UDim(0.0f, 0.0f));
    d_counter->setYPosition(UDim(0.0f, 0.0f));
    d_counter->setWidth(UDim(0.3f, 0.0f));
    d_counter->setHeight(UDim(1.0f, 0.0f));

    Window* newGame = winMgr.createWindow("Vanilla/Button", "new_game");
    action->addChild(newGame);
    newGame->setText("Start");
    newGame->setTooltipText("Start a new game");
    newGame->setXPosition(UDim(0.35f, 0.0f));
    newGame->setYPosition(UDim(0.0f, 0.0f));
    newGame->setWidth(UDim(0.3f, 0.0f));
    newGame->setHeight(UDim(1.0f, 0.0f));
    newGame->subscribeEvent(PushButton::EventClicked,  Event::Subscriber(&MinesweeperSample::handleGameStartClicked, this));

    d_timer = (Editbox*)winMgr.createWindow("Vanilla/Editbox", "timer");
    action->addChild(d_timer);
    d_timer->setText("0");
    d_timer->setTooltipText("Time elapsed");
    d_timer->setReadOnly(true);
    d_timer->setXPosition(UDim(0.7f, 0.0f));
    d_timer->setYPosition(UDim(0.0f, 0.0f));
    d_timer->setWidth(UDim(0.3f, 0.0f));
    d_timer->setHeight(UDim(1.0f, 0.0f));
    d_alarm->subscribeEvent(Timer::EventTimerAlarm, Event::Subscriber(&MinesweeperSample::handleUpdateTimer, this));

    // Board button grid
    Window* grid = winMgr.createWindow("DefaultWindow");
    frame->addChild(grid);
    grid->setXPosition(UDim(0.03f, 0.0f));
    grid->setYPosition(UDim(0.23f, 0.0f));
    grid->setWidth(    UDim(0.94f, 0.0f));
    grid->setHeight(   UDim(0.74f, 0.0f));
    const float d_inc = 1.0f / MinesweeperSize; 
    for(size_t i = 0 ; i < MinesweeperSize ; ++i)
    {
        // create a container for each row
        Window* row = winMgr.createWindow("DefaultWindow");
        row->setArea(URect(UDim(0,0), UDim(d_inc * i, 0),
            UDim(1,0), UDim(d_inc * (i + 1), 0)));
        grid->addChild(row);
        for(size_t j = 0 ; j < MinesweeperSize ; ++j)
        {
            // Initialize buttons coordinate
            d_buttonsMapping[i][j].d_col = j;
            d_buttonsMapping[i][j].d_row = i;
            d_buttons[i][j] = (PushButton*)winMgr.createWindow("Vanilla/Button");
            row->addChild(d_buttons[i][j]);
            d_buttons[i][j]->setArea(URect(UDim(d_inc * j, 0), UDim(0,0),
                UDim(d_inc * (j + 1), 0), UDim(1,0)));
            d_buttons[i][j]->setEnabled(false);
            // Associate user data
            d_buttons[i][j]->setUserData(&(d_buttonsMapping[i][j]));
            d_buttons[i][j]->setID(0);
            // Connect event handlers
            d_buttons[i][j]->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&MinesweeperSample::handleMineButtonClicked, this));
            d_buttons[i][j]->subscribeEvent(Window::EventMouseButtonDown, Event::Subscriber(&MinesweeperSample::handleMineButtonDown, this));
        }
    }
    d_result = winMgr.createWindow("Vanilla/StaticText");
    grid->addChild(d_result);
    d_result->setXPosition(UDim(0.0, 0.0));
    d_result->setYPosition(UDim(0.0, 0.0));
    d_result->setWidth(UDim(1.0, 0.0));
    d_result->setHeight(UDim(1.0, 0.0));
    d_result->setAlwaysOnTop(true);
    d_result->setProperty("HorzFormatting", "HorzCentred");
    d_result->setVisible(false);
    d_result->setAlpha(0.67f);
    // activate the background window
    background->activate();
    // success!
    return true;
}


/*************************************************************************
Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void MinesweeperSample::deinitialise()
{
    //delete d_console;
}
/*************************************************************************
Handle new game started event
*************************************************************************/
bool MinesweeperSample::handleGameStartClicked(const CEGUI::EventArgs&)
{
    d_result->setVisible(false);
    boardReset();
    boardPositionMines();
    for (size_t i = 0 ; i < MinesweeperSize ; ++i)
    {
        for(size_t j = 0 ; j < MinesweeperSize ; ++j)
        {
            d_buttons[i][j]->setProperty("DisabledTextColour", "FF000000");
            d_buttons[i][j]->setText("");
            d_buttons[i][j]->setEnabled(true);
        }
    }
    d_counter->setText(CEGUI::PropertyHelper<CEGUI::uint>::toString(MineCount));
    // Handle timer
    d_timerStartTime = ::clock();
    d_timerValue = 0;
    d_timer->setText("0");
    d_gameStarted = true;
    d_alarm->start();
    return true;
}
/************************************************************************
Handle click on a mine button
************************************************************************/
bool MinesweeperSample::handleMineButtonClicked(const CEGUI::EventArgs& event)
{
    const CEGUI::WindowEventArgs* evt = static_cast<const CEGUI::WindowEventArgs*>(&event);
    CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(evt->window);
    Location* buttonLoc = static_cast<Location*>(button->getUserData());
    if (button->getID() > 0)
    {
        // dont touch flagged buttons
        return true;
    }
    if (boardDiscover(*buttonLoc))
    {
        // We did not find a mine
        button->setText(CEGUI::PropertyHelper<CEGUI::uint>::toString(d_board[buttonLoc->d_row][buttonLoc->d_col]));
        if (isGameWin())
            gameEnd(true);
    }
    else
    {
        for(size_t i = 0 ; i < MinesweeperSize ; ++i)
        {
            for (size_t j = 0 ;  j < MinesweeperSize ; ++j)
            {
                if (! d_buttons[i][j]->isDisabled())
                {
                    if (d_board[i][j] > 8)
                    {
                        d_buttons[i][j]->setText("B");
                        d_buttons[i][j]->setProperty("DisabledTextColour", "FFFF1010");
                    }
                    else
                    {
                        d_buttons[i][j]->setText(CEGUI::PropertyHelper<CEGUI::uint>::toString(d_board[i][j]));
                    }
                }
                d_buttons[i][j]->setEnabled(false);
            }
        }
        gameEnd(false);
    }
    return true;
}
/************************************************************************
Handle click on a mine button (any mouse button)
************************************************************************/
bool MinesweeperSample::handleMineButtonDown(const CEGUI::EventArgs& event)
{
    const CEGUI::MouseEventArgs& me = static_cast<const CEGUI::MouseEventArgs&>(event);
    if (me.button == CEGUI::RightButton)
    {
        CEGUI::Window* button = me.window;
        if (!button->isDisabled())
        {
            if (button->getID() == 0)
            {
                button->setID(1);
                button->setText("F");
            }
            else
            {
                button->setID(0);
                button->setText("");
            }
            return true;
        }
    }
    return false;
}
/***********************************************************************
Handle timer refresh
***********************************************************************/
bool MinesweeperSample::handleUpdateTimer(const CEGUI::EventArgs&)
{
    if (d_gameStarted)
    {
        clock_t time = ::clock();
        time -= d_timerStartTime;
        if (time != d_timerValue)
        {
            d_timer->setText(CEGUI::PropertyHelper<CEGUI::uint>::toString(time /  CLOCKS_PER_SEC));
            d_timerValue = time;
        }
    }
    return true;
}
/************************************************************************
Create the board
************************************************************************/
void MinesweeperSample::boardReset()
{
    d_boardCellDiscovered = 0;
    for(size_t i = 0 ; i < MinesweeperSize ; ++i)
    {
        for(size_t j = 0 ; j < MinesweeperSize ; ++j)
        {
            d_board[i][j] = 0;
        }
    }
}
/************************************************************************
Position mine on the board
************************************************************************/
void MinesweeperSample::boardPositionMines()
{
    size_t x = 0 ;
    size_t y = 0 ;
    ::srand(::clock());
    for(size_t i = 0 ; i < MineCount ; ++i)
    {
        do
        {
            x = (size_t) ((float)MinesweeperSize * (::rand() / (RAND_MAX + 1.0)));
            y = (size_t) ((float)MinesweeperSize * (::rand() / (RAND_MAX + 1.0)));
        }
        while(d_board[x][y] > 8);

        d_board[x][y] += 10;
        if (x > 0)
        {
            if (y > 0)
                ++ d_board[x - 1][y - 1];

            ++ d_board[x - 1][y    ];

            if (y < MinesweeperSize - 1)
                ++ d_board[x - 1][y + 1];
        }

        if (y > 0)
            ++ d_board[x    ][y - 1];

        if (y < MinesweeperSize - 1)
            ++ d_board[x    ][y + 1];

        if (x < MinesweeperSize - 1)
        {
            if (y > 0)
                ++ d_board[x + 1][y - 1];

            ++ d_board[x + 1][y    ];

            if (y < MinesweeperSize - 1)
                ++ d_board[x + 1][y + 1];
        }
    }
}
/************************************************************************
Check wether the game is won or not
************************************************************************/
bool MinesweeperSample::isGameWin()
{
    return d_boardCellDiscovered + MineCount == MinesweeperSize * MinesweeperSize;
}


void MinesweeperSample::gameEnd(bool victory)
{
    d_gameStarted = false;
    d_alarm->stop();
    CEGUI::String message;
    if (victory)
    {
        message = CEGUI::String("You win");
    }
    else
    {
        message = CEGUI::String("You lose");
    }
    // Display a message to the user
    d_result->setText(message);
    d_result->setVisible(true);
}

bool MinesweeperSample::boardDiscover(const Location& loc)
{
    CEGUI::PushButton* btn = d_buttons[loc.d_row][loc.d_col];
    if (btn->isDisabled() || btn->getID() > 0)
        return true;

    if (d_board[loc.d_row][loc.d_col] > 8)
        return false;
    d_buttons[loc.d_row][loc.d_col]->setText(CEGUI::PropertyHelper<CEGUI::uint>::toString(d_board[loc.d_row][loc.d_col]));
    d_buttons[loc.d_row][loc.d_col]->setEnabled(false);
    ++d_boardCellDiscovered;
    // Discover surrounding case
    if (d_board[loc.d_row][loc.d_col] == 0)
    {
        Location l;
        if (loc.d_row > 0)
        {
            l.d_row = loc.d_row - 1;
            if ( loc.d_col > 0)
            {
                l.d_col = loc.d_col - 1;
                boardDiscover(l);
            }
            l.d_col = loc.d_col;
            boardDiscover(l);
            if ( loc.d_col < MinesweeperSize - 1)
            {
                l.d_col = loc.d_col + 1;
                boardDiscover(l);
            }
        }
        l.d_row = loc.d_row;
        if ( loc.d_col > 0)
        {
            l.d_col = loc.d_col - 1;
            boardDiscover(l);
        }
        if ( loc.d_col < MinesweeperSize  - 1)
        {
            l.d_col = loc.d_col + 1;
            boardDiscover(l);
        }
        if (loc.d_row < MinesweeperSize - 1)
        {
            l.d_row = loc.d_row + 1;
            if ( loc.d_col > 0)
            {
                l.d_col = loc.d_col - 1;
                boardDiscover(l);
            }
            l.d_col = loc.d_col;
            boardDiscover(l);
            if ( loc.d_col < MinesweeperSize - 1)
            {
                l.d_col = loc.d_col + 1;
                boardDiscover(l);
            }
        }
    }
    return true;
}


extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static MinesweeperSample sample;
    return sample;
}