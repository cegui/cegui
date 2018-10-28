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
#ifndef _Sample_Form_Navigation_h_
#define _Sample_Form_Navigation_h_

#include "Sample.h"
#include "NavigationStrategies.h"

class FormNavigationSample : public Sample
{
public:
    FormNavigationSample();
    virtual ~FormNavigationSample() {}

    bool initialise(CEGUI::GUIContext* gui_context) override;
    void deinitialise() override;

private:
    void createForm(CEGUI::FrameWindow* wnd);

    CEGUI::Window* createWidget(const CEGUI::String& type, float position_x, float position_y, const CEGUI::String& text = "");
    bool resetForm(const CEGUI::EventArgs& e);
    bool disableConfirmButton(const CEGUI::EventArgs& e);
    std::map<CEGUI::SemanticValue, CEGUI::String> getNavigationMappings();

    CEGUI::DefaultWindow*       d_root;
    CEGUI::ToggleButton*        d_isGameMasterCheckbox;
    CEGUI::Window*              d_confirmButton;
    std::vector<CEGUI::Window*> d_editboxes;
    LinearNavigationStrategy*   d_navigationStrategy;
    CEGUI::WindowNavigator*     d_windowNavigator;
};

#endif  // end of guard _Sample_Form_Navigation_h_
