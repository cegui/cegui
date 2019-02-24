/***********************************************************************
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
#ifndef _Sample_SimpleGameMenu_h_
#define _Sample_SimpleGameMenu_h_

#include "Sample.h"
#include "NavigationStrategies.h"
#include <vector>

class SimpleGameMenuSample : public Sample
{
public:
    SimpleGameMenuSample();
    virtual ~SimpleGameMenuSample() {}

    bool initialise(CEGUI::GUIContext* gui_context) override;
    void deinitialise() override;

private:
    CEGUI::Window*      d_layout;
    CEGUI::Window*      d_root;
    CEGUI::Window*      d_currentMenu;

    WindowChildrenNavigationStrategy*   d_navigationStrategy;
    CEGUI::WindowNavigator*             d_windowNavigator;

    static std::map<CEGUI::SemanticValue, CEGUI::String> createNavigationMappings();

    bool handleContinueGameButtonClick(const CEGUI::EventArgs& e);
    bool handleNewGameButtonClick(const CEGUI::EventArgs& e);
    bool handleOptionsButtonClick(const CEGUI::EventArgs& e);
    bool handleVideoOptionsButtonClick(const CEGUI::EventArgs& e);
    bool handleGeneralOptionsButtonClick(const CEGUI::EventArgs& e);
    bool handleSoundOptionsButtonClick(const CEGUI::EventArgs& e);

    bool handleGoToOptionsMenu(const CEGUI::EventArgs& e);
    bool handleGoToMainMenu(const CEGUI::EventArgs& e);

    void showMenu(CEGUI::String MainMenu);
    void createEventHandlers();
};

#endif  // end of guard _Sample_SimpleGameMenuSample_h_
