/***********************************************************************
created:    24/5/2012
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
#ifndef _Samples_Browser_h_
#define _Samples_Browser_h_

#include "SampleBrowserBase.h"
#include "SampleHandler.h"
#include "Sample.h"

#include <vector>

// forward declarations
#include "CEGUI/ForwardRefs.h"

using namespace CEGUI;

class SampleHandler;
class Sample;
class MetaDataWindowManager;
class SampleBrowserManager;

/*!
\brief
This is 
*/
class SampleBrowser : public SampleBrowserBase
{
public:
    SampleBrowser();
    virtual ~SampleBrowser();

    typedef std::vector<SampleHandler*> SampleList;

    bool initialise(const CEGUI::String& logFile,
                    const CEGUI::String& dataPathPrefixOverride) override;
    void cleanup() override;

    void loadSamples();

    void handleSampleSelection(CEGUI::Window* sampleWindow);
    void handleStartDisplaySample(CEGUI::Window* sampleWindow);

    void update(float passedTime) override;

    void handleNewWindowSize(float width, float height) override;

    void renderGUIContexts() override;

    SampleHandler* findSampleData(CEGUI::Window* sampleWindow);

    bool injectKeyDown(const CEGUI::Key::Scan& ceguiKey) override;
    bool injectKeyUp(const CEGUI::Key::Scan& ceguiKey) override;
    bool injectChar(int character) override;
    bool injectMouseButtonDown(const CEGUI::MouseButton& ceguiMouseButton) override;
    bool injectMouseButtonUp(const CEGUI::MouseButton& ceguiMouseButton) override;
    bool injectMouseWheelChange(float position) override;
    bool injectMousePosition(float x, float y) override;

protected:
    void initialiseLoadScreenLayout();

    void initialiseSampleBrowserLayout();

    bool initialiseSampleStepwise(std::int32_t sampleNumber);

    void displaySampleLoadProgress(std::int32_t sampleNumber);
    void initialisationFinalisation();
    void unloadSamples();

    void stopDisplaySample();

    void updateSamples(float passedTime);
    bool updateInitialisationStep();

    void displaySampleBrowserLayoutLoadProgress();
    bool handleSampleExitButtonClicked(const CEGUI::EventArgs& args);
    void renderSampleGUIContexts();

    bool areWindowsIntersecting(CEGUI::Window* window1, CEGUI::Window* window2);

    CEGUI::InputAggregator* getCurrentInputAggregator();

    CEGUI::Window*          d_root;

    CEGUI::PushButton*      d_sampleExitButton;

    SampleList              d_samples;

    MetaDataWindowManager*  d_metaDataWinMgr;
    SampleBrowserManager*       d_samplesWinMgr;

    SampleHandler*             d_selectedSampleData;

    CEGUI::ProgressBar*     d_loadingProgressBar;
    CEGUI::Window*          d_loadingScreenText;
    CEGUI::Window*          d_loadScreenChunkProgressText;

    bool                            d_quittingSampleView;
    CEGUI::InputAggregator*         d_systemInputAggregator;
};

#endif
