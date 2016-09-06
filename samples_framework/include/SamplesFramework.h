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
#ifndef _Samples_Framework_h_
#define _Samples_Framework_h_

#include "SamplesFrameworkBase.h"
#include "SampleData.h"

#include <vector>

// forward declarations
#include "CEGUI/ForwardRefs.h"

class SampleData;
class Sample;
class MetaDataWindowManager;
class SamplesBrowserManager;


/*!
\brief
This is 
*/
class SamplesFramework : public SamplesFrameworkBase
{
public:
    SamplesFramework(const CEGUI::String& xml_filename);
    virtual ~SamplesFramework();

    static void setDefaultResourceGroup(const CEGUI::String& resourceGroup);

    bool initialise(const CEGUI::String& logFile,
                    const CEGUI::String& dataPathPrefixOverride);
    void cleanup();

    void addSampleDataCppModule(CEGUI::String sampleName, CEGUI::String summary,
                                CEGUI::String description,
                                SampleType sampleTypeEnum, CEGUI::String credits);

    void handleSampleSelection(CEGUI::Window* sampleWindow);
    void handleStartDisplaySample(CEGUI::Window* sampleWindow);

    virtual void update(float passedTime);

    virtual void handleNewWindowSize(float width, float height);

    virtual void renderGUIContexts();

    SampleData* findSampleData(CEGUI::Window* sampleWindow);

    virtual bool injectKeyDown(const CEGUI::Key::Scan& ceguiKey);
    virtual bool injectKeyUp(const CEGUI::Key::Scan& ceguiKey);
    virtual bool injectChar(int character);
    virtual bool injectMouseButtonDown(const CEGUI::MouseButton& ceguiMouseButton);
    virtual bool injectMouseButtonUp(const CEGUI::MouseButton& ceguiMouseButton);
    virtual bool injectMouseWheelChange(float position);
    virtual bool injectMousePosition(float x, float y);

    static const CEGUI::String XMLSchemaName;

protected:
    void initialiseLoadScreenLayout();

    void initialiseSampleBrowserLayout();
    void loadSamplesDataFromXML(const CEGUI::String& filename);
    void getSampleInstanceFromDLL(SampleData& sampleData);

    void addSample(SampleData* sampleData);

    bool initialiseSampleStepwise(int sampleNumber);

    void displaySampleLoadProgress(int sampleNumber);
    void initialisationFinalisation();
    void unloadSamples();

    void stopDisplaySample();

    void updateSamples(float passedTime);
    bool updateInitialisationStep();

    void displaySampleBrowserLayoutLoadProgress();
    bool handleSampleExitButtonClicked(const CEGUI::EventArgs& args);
    void renderSampleGUIContexts();

    bool areWindowsIntersecting(CEGUI::Window* window1, CEGUI::Window* window2);

    CEGUI::Window*              d_root;

    CEGUI::PushButton*          d_sampleExitButton;

    std::vector<SampleData*>    d_samples;

    MetaDataWindowManager*      d_metaDataWinMgr;
    SamplesBrowserManager*      d_samplesWinMgr;

    SampleData*                 d_selectedSampleData;

    CEGUI::ProgressBar*         d_loadingProgressBar;
    CEGUI::Window*              d_loadingScreenText;
    CEGUI::Window*              d_loadScreenChunkProgressText;

    bool                        d_quittingSampleView;

    CEGUI::String d_samplesXMLFilename;
};

#endif
