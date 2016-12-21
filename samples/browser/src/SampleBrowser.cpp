/***********************************************************************
created:    24/5/2012
author:     Lukas E Meindl
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
#include "SampleBrowser.h"

#include "Sample.h"
#include "SampleHandler.h"
#include "CEGuiBaseApplication.h"

#include "MetaDataWindowManager.h"
#include "SampleBrowserManager.h"

#include "CEGUI/CEGUI.h"
#include "CEGUI/Logger.h"
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/widgets/ProgressBar.h"

#include "samples.h"
#include <string>
#include <iostream>
#include <vector>

using namespace CEGUI;

//platform-dependant DLL delay-loading includes
#if (defined( __WIN32__ ) || defined( _WIN32 ))
#include "windows.h"
#endif

#ifdef __ANDROID__
#   include "CEGUI/AndroidUtils.h"
#endif

#if !defined __ANDROID__

//----------------------------------------------------------------------------//
#ifdef __ANDROID__
#   include <android_native_app_glue.h>
void android_main(struct android_app* state) 
{
    app_dummy(); // Make sure glue isn't stripped.
    int argc = 0;
    char* argv[0];
    AndroidUtils::setAndroidApp(state);
#else
int main(int argc, char* argv[])
{
#endif

    CEGUI_UNUSED(argc);
    CEGUI_UNUSED(argv);
    
    // Basic start-up for the sample browser application.
    // Will remain in run() until quitting
    int argidx = 1;
    CEGUI_UNUSED(argidx);
    
#ifdef __APPLE__
    if (argc > 1 && !std::strncmp(argv[argidx], "-psn", 4))
    {
        --argc;
        ++argidx;
    }
#endif

    SampleBrowser sampleBrowser;
#ifdef __ANDROID__
    sampleBrowser.run();
#else
    return sampleBrowser.run();
#endif
}

#endif // !defined __ANDROID__

//----------------------------------------------------------------------------//
SampleBrowser::SampleBrowser() :
    d_sampleExitButton(nullptr),
    d_metaDataWinMgr(nullptr),
    d_samplesWinMgr(nullptr),
    d_selectedSampleData(nullptr),
    d_loadingProgressBar(nullptr),
    d_quittingSampleView(false)
{
}

//----------------------------------------------------------------------------//
SampleBrowser::~SampleBrowser()
{
    if (d_metaDataWinMgr)
        delete d_metaDataWinMgr;
}

//----------------------------------------------------------------------------//
bool SampleBrowser::initialise(const CEGUI::String& logFile,
                               const CEGUI::String& dataPathPrefixOverride)
{
    if (SampleBrowserBase::initialise(logFile, dataPathPrefixOverride))
    {
        initialiseLoadScreenLayout();
        loadSamples();

        d_systemInputAggregator = new InputAggregator(
            &CEGUI::System::getSingletonPtr()->getDefaultGUIContext());
        d_systemInputAggregator->initialise();

        return true;
    }
    return false;
}

//----------------------------------------------------------------------------//
void SampleBrowser::cleanup()
{
    unloadSamples();

    if (d_systemInputAggregator != nullptr)
    {
        delete d_systemInputAggregator;
        d_systemInputAggregator = nullptr;
    }

    SampleBrowserBase::cleanup();
}

//----------------------------------------------------------------------------//
void SampleBrowser::initialiseLoadScreenLayout()
{
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();

    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(defaultFont);

    SchemeManager::getSingleton().createFromFile("SampleBrowser.scheme");

    WindowManager& winMgr(WindowManager::getSingleton());
    Window* loadScreenRoot =
        winMgr.loadLayoutFromFile("SampleBrowserLoadScreen.layout");
    System::getSingleton().getDefaultGUIContext().setRootWindow(loadScreenRoot);

    d_loadingProgressBar = static_cast<CEGUI::ProgressBar*>(
        loadScreenRoot->getChild("LoadScreenProgressBar"));
    d_loadingScreenText = loadScreenRoot->getChild("LoadScreenText");
    d_loadScreenChunkProgressText =
        d_loadingScreenText->getChild("LoadScreenTextChunkProgress");

    d_loadingProgressBar->setProgress(0.0f);
}

//----------------------------------------------------------------------------//
void SampleBrowser::loadSamples()
{
    #include "samples.inc"

    std::vector<Sample*>::iterator iter = samples.begin();
    std::vector<Sample*>::iterator iterEnd = samples.end();

    std::sort(samples.begin(), samples.end(), SamplePointerCompare());

    while (iter != iterEnd)
    {
        SampleHandler* sampleData = new SampleHandler(*iter);
        d_samples.push_back(sampleData);

        ++iter;
    }

}

//----------------------------------------------------------------------------//
void SampleBrowser::unloadSamples()
{
    while (d_samples.size() > 0)
    {
        SampleHandler*& sampleData = d_samples.back();

        sampleData->deinitialise();
        delete sampleData;

        d_samples.pop_back();
    }
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectKeyDown(const CEGUI::Key::Scan& ceguiKey)
{
    if (Key::Scan::Esc != ceguiKey)
        return getCurrentInputAggregator()->injectKeyDown(ceguiKey);
    else
    {
        if (d_selectedSampleData)
            stopDisplaySample();
        else
            setQuitting(true);
    }

    return false;
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectKeyUp(const CEGUI::Key::Scan& ceguiKey)
{
    if (getCurrentInputAggregator() == nullptr)
        return false;

    return getCurrentInputAggregator()->injectKeyUp(ceguiKey);
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectChar(int character)
{
    if (getCurrentInputAggregator() == nullptr)
        return false;

    return getCurrentInputAggregator()->injectChar(character);
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectMouseButtonDown(
                                    const CEGUI::MouseButton& ceguiMouseButton)
{
    if (getCurrentInputAggregator() == nullptr)
        return false;

    return getCurrentInputAggregator()->injectMouseButtonDown(ceguiMouseButton);
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectMouseButtonUp(
                                    const CEGUI::MouseButton& ceguiMouseButton)
{
    if (getCurrentInputAggregator() == nullptr)
        return false;

    return getCurrentInputAggregator()->injectMouseButtonUp(ceguiMouseButton);
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectMouseWheelChange(float position)
{
    if (getCurrentInputAggregator() == nullptr)
        return false;

    return getCurrentInputAggregator()->injectMouseWheelChange(position);
}

//----------------------------------------------------------------------------//
bool SampleBrowser::injectMousePosition(float x, float y)
{
    if (getCurrentInputAggregator() == nullptr)
        return false;

    return getCurrentInputAggregator()->injectMousePosition(x, y);
}

//----------------------------------------------------------------------------//
void SampleBrowser::update(float passedTime)
{
    static bool init(false);

    if (!init)
    {
        init = updateInitialisationStep();

        CEGUI::GUIContext& defaultGUIContext(
            CEGUI::System::getSingleton().getDefaultGUIContext());
        defaultGUIContext.injectTimePulse(passedTime);
    }
    else
    {
        if (d_quittingSampleView)
            stopDisplaySample();

        if (!d_selectedSampleData)
        {
            updateSamples(passedTime);

            CEGUI::GUIContext& defaultGUIContext(
                CEGUI::System::getSingleton().getDefaultGUIContext());
            defaultGUIContext.injectTimePulse(passedTime);
        }
        else
        {
            d_selectedSampleData->getGuiContext()->injectTimePulse(passedTime);

            d_selectedSampleData->update(passedTime);
        }
    }

}

//----------------------------------------------------------------------------//
void SampleBrowser::handleNewWindowSize(float width, float height)
{
    d_appWindowWidth = static_cast<int>(width);
    d_appWindowHeight = static_cast<int>(height);

    SampleList::iterator iter = d_samples.begin();
    SampleList::iterator end = d_samples.end();
    for (; iter != end; ++iter)
    {
        SampleHandler* sampleData = *iter;

        sampleData->handleNewWindowSize(width, height);
    }

    if (d_samplesWinMgr)
        d_samplesWinMgr->setWindowRatio(width / height);
}

//----------------------------------------------------------------------------//
void SampleBrowser::renderGUIContexts()
{
    if (!d_selectedSampleData)
    {
        renderSampleGUIContexts();

        CEGUI::System& gui_system(CEGUI::System::getSingleton());
        gui_system.getDefaultGUIContext().draw();
    }
    else
    {
        d_selectedSampleData->getGuiContext()->draw();
    }

}

//----------------------------------------------------------------------------//
void SampleBrowser::handleSampleSelection(CEGUI::Window* sampleWindow)
{
    SampleHandler* correspondingSampleData = findSampleData(sampleWindow);

    d_metaDataWinMgr->setSampleInfo(correspondingSampleData);
}

//----------------------------------------------------------------------------//
void SampleBrowser::handleStartDisplaySample(CEGUI::Window* sampleWindow)
{
    SampleHandler* correspondingSampleData = findSampleData(sampleWindow);

    CEGUI::RenderTarget& defaultRenderTarget =
        CEGUI::System::getSingleton().getRenderer()->getDefaultRenderTarget();
    CEGUI::GUIContext* sampleContext(correspondingSampleData->getGuiContext());
    sampleContext->setRenderTarget(defaultRenderTarget);

    //! We add the exit button to the sample
    sampleContext->getRootWindow()->addChild(d_sampleExitButton);

    //! We manually set the cursor to where it was in the overview
    sampleContext->getCursor().setPosition(
        CEGUI::System::getSingleton().getDefaultGUIContext().getCursor().
            getPosition());

    d_selectedSampleData = correspondingSampleData;

    d_selectedSampleData->onEnteringSample();
}

//----------------------------------------------------------------------------//
void SampleBrowser::stopDisplaySample()
{
    GUIContext* sampleGUIContext = d_selectedSampleData->getGuiContext();

    // Since we switch our contexts, the mouse release won't be injected if we
    // don't do it manually
    if (getCurrentInputAggregator() != nullptr)
        getCurrentInputAggregator()->injectMouseButtonUp(MouseButton::Left);
    sampleGUIContext->injectTimePulse(0.0f);

    sampleGUIContext->getRootWindow()->removeChild(d_sampleExitButton);
    d_selectedSampleData->setGUIContextRTT();

    CEGUI::System::getSingleton().getDefaultGUIContext().getCursor().
        setPosition(sampleGUIContext->getCursor().getPosition());

    d_selectedSampleData = nullptr;
    d_quittingSampleView = false;
}

//----------------------------------------------------------------------------//
SampleHandler* SampleBrowser::findSampleData(CEGUI::Window* sampleWindow)
{
    //Find corresponding SampleData
    SampleList::iterator iter = d_samples.begin();
    SampleList::iterator end = d_samples.end();
    for (; iter != end; ++iter)
    {
        SampleHandler* sampleData = *iter;

        if (sampleData->getSampleWindow() == sampleWindow)
            return sampleData;
    }

    return nullptr;
}

//----------------------------------------------------------------------------//
bool SampleBrowser::handleSampleExitButtonClicked(const CEGUI::EventArgs&)
{
    d_quittingSampleView = true;

    return false;
}

//----------------------------------------------------------------------------//
bool SampleBrowser::initialiseSampleStepwise(std::int32_t sampleNumber)
{
    if (static_cast<int>(d_samples.size()) <= sampleNumber)
        return true;

    if (static_cast<int>(d_samples.size()) > sampleNumber + 1)
        displaySampleLoadProgress(sampleNumber);

    if (sampleNumber >= 0)
    {
        SampleHandler* sampleData = d_samples[sampleNumber];
        sampleData->initialise(d_appWindowWidth, d_appWindowHeight);
        CEGUI::FrameWindow* sampleWindow = d_samplesWinMgr->createSampleWindow(
            sampleData->getNameText(), sampleData->getRTTImage());

        sampleData->setSampleWindow(sampleWindow);

        d_baseApp->registerSampleOverlayHandler(sampleData->getGuiContext());
    }

    return false;
}

//----------------------------------------------------------------------------//
void SampleBrowser::initialiseSampleBrowserLayout()
{
    CEGUI::FontManager::getSingleton().
        createFreeTypeFont("DejaVuSans-14", 14.f, FontSizeUnit::Pixels,
            true, "DejaVuSans.ttf");

    WindowManager& winMgr(WindowManager::getSingleton());

    CEGUI::SchemeManager::getSingleton().createFromFile("Generic.scheme");

    if (!ImageManager::getSingleton().isDefined("BackgroundSampleBrowser"))
        ImageManager::getSingleton().addBitmapImageFromFile(
            "BackgroundSampleBrowser", "BackgroundSampleBrowser.jpg");

    d_root = winMgr.loadLayoutFromFile("SampleBrowser.layout");
    d_root->getChild("BackgroundImage")->setProperty(
        "Image", "BackgroundSampleBrowser");

    CEGUI::Window* metaDataWindow = d_root->getChild("SampleBrowserMetaData");
    d_metaDataWinMgr = new MetaDataWindowManager(metaDataWindow);

    CEGUI::Window* samplesScrollablePane = d_root->getChild(
        "SampleFrameWindowContainer/SamplesFrameWindow/SamplesScrollablePane");
    d_samplesWinMgr = new SampleBrowserManager(this, samplesScrollablePane);

    d_sampleExitButton = static_cast<CEGUI::PushButton*>(winMgr.createWindow(
        "SampleBrowserSkin/Button", "SampleExitButton"));

    d_sampleExitButton->setSize(CEGUI::USize(cegui_absdim(34.f),
                                             cegui_absdim(34.f)));
    d_sampleExitButton->setPosition(CEGUI::UVector2(cegui_absdim(0.0f),
                                                    cegui_absdim(0.0f)));
    d_sampleExitButton->setHorizontalAlignment(HorizontalAlignment::Right);
    d_sampleExitButton->setVerticalAlignment(VerticalAlignment::Top);
    d_sampleExitButton->setProperty(
        "NormalImage", "SampleBrowserSkin/ExitButtonNormal");
    d_sampleExitButton->setProperty(
        "HoverImage", "SampleBrowserSkin/ExitButtonHover");
    d_sampleExitButton->setProperty(
        "PushedImage", "SampleBrowserSkin/ExitButtonClicked");
    d_sampleExitButton->subscribeEvent(
        PushButton::EventClicked,
        Event::Subscriber(&SampleBrowser::handleSampleExitButtonClicked, this));

    d_sampleExitButton->setAlwaysOnTop(true);
}

//----------------------------------------------------------------------------//
bool SampleBrowser::updateInitialisationStep()
{
    static std::int32_t step(0);

    switch (step)
    {
    case 0:
    {
        ++step;
        displaySampleBrowserLayoutLoadProgress();

        break;
    }

    case 1:
    {
        initialiseSampleBrowserLayout();
        ++step;
        break;
    }

    default:
    {
        bool sampleInitFinished = initialiseSampleStepwise(step - 3); // -2 for the previous 2 steps, -1 for extra step to display the text before actually loading
        if (sampleInitFinished)
        {
            //Loading finished, switching layout to sample browser
            initialisationFinalisation();
            return true;
        }
        else
            ++step;

        break;
    }
    }

    return false;
}

//----------------------------------------------------------------------------//
void SampleBrowser::initialisationFinalisation()
{
    System::getSingleton().getDefaultGUIContext().getCursor().
        setDefaultImage("SampleBrowserSkin/MouseArrow");
    d_samplesWinMgr->setWindowRatio(d_appWindowWidth / static_cast<float>(d_appWindowHeight));

    System::getSingleton().getDefaultGUIContext().setRootWindow(d_root);

    if (d_samples.size() > 0)
    {
        if (CEGUI::Window* wnd = d_samples[0]->getSampleWindow())
        {
            handleSampleSelection(wnd);
            d_samplesWinMgr->selectSampleWindow(wnd);
        }
    }
}

//----------------------------------------------------------------------------//
void SampleBrowser::updateSamples(float passedTime)
{
    SampleList::iterator iter = d_samples.begin();
    SampleList::iterator end = d_samples.end();
    for (; iter != end; ++iter)
    {
        SampleHandler* sampleData = *iter;

        GUIContext* guiContext = sampleData->getGuiContext();
        guiContext->injectTimePulse(passedTime);

        sampleData->update(passedTime);
    }
}

//----------------------------------------------------------------------------//
void SampleBrowser::renderSampleGUIContexts()
{
    SampleList::iterator iter = d_samples.begin();
    SampleList::iterator end = d_samples.end();
    for (; iter != end; ++iter)
    {
        SampleHandler* sampleData = *iter;

        if (!sampleData->getGuiContext())
            continue;

        CEGUI::Window* sampleWindow = sampleData->getSampleWindow();

        if (!areWindowsIntersecting(d_samplesWinMgr->getWindow(), sampleWindow))
            continue;

        if (sampleData->getGuiContext()->isDirty())
        {
            sampleData->getSampleWindow()->invalidate();
            sampleData->clearRTTTexture();
            sampleData->getGuiContext()->draw();
        }
    }
}

//----------------------------------------------------------------------------//
void SampleBrowser::displaySampleBrowserLayoutLoadProgress()
{
    std::int32_t totalNum = d_samples.size() + 2;

    CEGUI::String loadText = CEGUI::String("Loading SampleBrowser skin ...");
    d_loadingScreenText->setText(loadText);

    CEGUI::String progressText =  PropertyHelper<std::int32_t>::toString(1) + "/" +
        PropertyHelper<std::int32_t>::toString(totalNum - 1);
    d_loadScreenChunkProgressText->setText(progressText);

    d_loadingProgressBar->setProgress(1.0f / (totalNum - 1.0f));
}

//----------------------------------------------------------------------------//
void SampleBrowser::displaySampleLoadProgress(std::int32_t sampleNumber)
{
    SampleHandler* sampleData = d_samples[sampleNumber + 1];

    int totalNum = d_samples.size() + 2;
    CEGUI::String loadText = "Loading " + sampleData->getNameText() + " ...";
    d_loadingScreenText->setText(loadText);

    CEGUI::String progressText = PropertyHelper<std::int32_t>::toString(sampleNumber + 3) +
        "/" + PropertyHelper<std::int32_t>::toString(totalNum - 1);
    d_loadScreenChunkProgressText->setText(progressText);

    d_loadingProgressBar->setProgress( (sampleNumber + 3.f) / (totalNum - 1.0f) );
}

//----------------------------------------------------------------------------//
bool SampleBrowser::areWindowsIntersecting(CEGUI::Window* window1,
                                              CEGUI::Window* window2)
{
    const CEGUI::Rectf& clipRect1 = window1->getOuterRectClipper();
    const CEGUI::Rectf& clipRect2 = window2->getOuterRectClipper();

    return  clipRect1.left() < clipRect2.right()
        && clipRect1.right() > clipRect2.left()
        && clipRect1.top() < clipRect2.bottom()
        && clipRect1.bottom() > clipRect2.top();
}

//----------------------------------------------------------------------------//
CEGUI::InputAggregator* SampleBrowser::getCurrentInputAggregator()
{
    if (d_selectedSampleData != nullptr)
        return d_selectedSampleData->getInputAggregator();

    return d_systemInputAggregator;
}

//----------------------------------------------------------------------------//
