/***********************************************************************
created:    11/6/2012
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
#ifndef _SampleBrowserManager_h_
#define _SampleBrowserManager_h_

#include <vector>
#include <map>

#include "CEGUI/Base.h"
#include "CEGUI/String.h"

#include "CEGUI/ForwardRefs.h"

class SampleBrowser;

class SampleBrowserManager
{
public:
    SampleBrowserManager(SampleBrowser* owner, CEGUI::Window* samplesWindow);
    virtual ~SampleBrowserManager() {}

    CEGUI::Window* getWindow();

    CEGUI::FrameWindow* createSampleWindow(const CEGUI::String& name, const CEGUI::Image& image);

    void setWindowRatio(float aspectRatio);

    void selectSampleWindow(CEGUI::Window* wnd);

private:
    SampleBrowserManager(const SampleBrowserManager&) {}

    void init();
    SampleBrowserManager& operator=(const SampleBrowserManager&) { return *this; }

    static const std::uint32_t d_sampleWindowFrameNormal;
    static const std::uint32_t d_sampleWindowFrameSelected;

    void updateWindows();

    bool handlePointerActivateSampleWindow(const CEGUI::EventArgs& args);
    bool handlePointerMoveSampleWindow(const CEGUI::EventArgs& args);
    bool handleLeaveSampleWindow(const CEGUI::EventArgs& args);

    bool handleSampleEnterButtonClicked(const CEGUI::EventArgs& args);

    CEGUI::VerticalLayoutContainer* createPreviewLayoutContainer();
    CEGUI::DefaultWindow* createPreviewHeaderNameWindow(const CEGUI::String& name);
    CEGUI::FrameWindow* createPreviewSampleWindow(const CEGUI::String& name, const CEGUI::Image &image);
    CEGUI::PushButton* createPreviewHeaderEnterButton();
    CEGUI::HorizontalLayoutContainer* createPreviewHeader();

    SampleBrowser* d_owner;

    CEGUI::Window* d_root;
    CEGUI::VerticalLayoutContainer* d_verticalLayoutContainerSamples;
    int d_childCount;
    float d_aspectRatio;
    CEGUI::Window* d_selectedWindow;

    
    std::map<CEGUI::Window*, CEGUI::Window*> d_buttonToSampleWindowMap;


    std::vector<CEGUI::Window*> d_sampleWindows;
};

#endif
