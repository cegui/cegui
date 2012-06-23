/***********************************************************************
filename:   SampleFramework.cpp
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
#include "SamplesFramework.h"

#include "Samples_xmlHandler.h"
#include "Sample.h"
#include "SampleData.h"

#include "MetaDataWindowManager.h"
#include "SamplesBrowserManager.h"

#include "CEGUI/CEGUI.h"
#include "CEGUI/Logger.h"
#include "CEGUI/widgets/PushButton.h"


#include <string>

using namespace CEGUI;

//platform-dependant DLL delay-loading includes
#if (defined( __WIN32__ ) || defined( _WIN32 )) 
    #include "windows.h"
#endif

// Name of the xsd schema file used to validate animation XML files.
const String SamplesFramework::XMLSchemaName("Animation.xsd");

// String that holds the default resource group for loading samples
String SamplesFramework::s_defaultResourceGroup("");


int main(int /*argc*/, char* /*argv*/[])
{
    // Basic start-up for the sample browser application.
    // Will remain in run() until quitting

    SamplesFramework sampleFramework;
    return sampleFramework.run();
}


SamplesFramework::SamplesFramework()
    : d_metaDataWinMgr(0),
    d_samplesWinMgr(0),
    d_selectedSampleData(0),
    d_sampleExitButton(0)
{
}

SamplesFramework::~SamplesFramework()
{
    if(d_metaDataWinMgr)
        delete d_metaDataWinMgr;
}



bool SamplesFramework::initialise()
{
    using namespace CEGUI;

    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

    initialiseFrameworkLayout();

    loadSamplesDataFromXML("samples.samps", s_defaultResourceGroup);

    initialiseSamples();

    if(d_samples.size() > 0)
    {
        CEGUI::Window* wnd = d_samples[0]->getSampleWindow();
        if(wnd)
        {
            handleSampleSelection(wnd);
            d_samplesWinMgr->selectSampleWindow(wnd);
        }
    }

    // return true to signalize the initialisation was sucessful and run the SamplesFramework
    return true;
}

void SamplesFramework::cleanupSample()
{
    unloadSamples();
}

void SamplesFramework::initialiseFrameworkLayout()
{
    CEGUI::Font& font = FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont(&font);

    SchemeManager::getSingleton().createFromFile("SampleBrowser.scheme");

    System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("SampleBrowserSkin/MouseArrow");

    WindowManager& winMgr(WindowManager::getSingleton());

    d_root = static_cast<DefaultWindow*>(winMgr.createWindow("DefaultWindow", "Root"));
    
    d_root = winMgr.loadLayoutFromFile("SampleBrowser.layout");
    System::getSingleton().getDefaultGUIContext().setRootWindow(d_root);

    CEGUI::Window* metaDataWindow = d_root->getChild("SampleBrowserMetaData");
    d_metaDataWinMgr = new MetaDataWindowManager(metaDataWindow);

    CEGUI::Window* samplesScrollablePane = d_root->getChild("SampleFrameWindowContainer/SamplesFrameWindow/SamplesScrollablePane");
    d_samplesWinMgr = new SamplesBrowserManager(this, samplesScrollablePane);


    d_sampleExitButton = static_cast<CEGUI::PushButton*>(winMgr.createWindow("SampleBrowserSkin/Button", "SampleExitButton"));

    d_sampleExitButton->setSize(CEGUI::USize(cegui_absdim(46.f), cegui_absdim(46.f)));
    d_sampleExitButton->setPosition(CEGUI::UVector2(cegui_absdim(0.f), cegui_absdim(0.f)));
    d_sampleExitButton->setHorizontalAlignment(HA_RIGHT);
    d_sampleExitButton->setVerticalAlignment(VA_TOP);
    d_sampleExitButton->setProperty("NormalImage", "SampleBrowserSkin/ExitButtonNormal");
    d_sampleExitButton->setProperty("HoverImage", "SampleBrowserSkin/ExitButtonHover");
    d_sampleExitButton->setProperty("PushedImage", "SampleBrowserSkin/ExitButtonClicked");
    d_sampleExitButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&SamplesFramework::handleExitSampleView, this));
    d_sampleExitButton->setAlwaysOnTop(true);
}

void SamplesFramework::unloadSamples()
{
    while(d_samples.size() > 0)
    {
        SampleData*& sampleData = d_samples.back();

        sampleData->deinitialise();
        delete sampleData;

        d_samples.pop_back();
    }
}

//----------------------------------------------------------------------------//
void SamplesFramework::loadSamplesDataFromXML(const String& filename,
                                             const String& resourceGroup)
{
    if (filename.empty())
        CEGUI_THROW(InvalidRequestException(
            "SamplesFramework::loadSamplesDataFromXML: "
            "filename supplied for file loading must be valid."));

    Samples_xmlHandler handler(this);

    // do parse (which uses handler to create actual data)
    CEGUI_TRY
    {
        System::getSingleton().getXMLParser()->
            parseXMLFile(handler, filename, XMLSchemaName,
                         resourceGroup.empty() ? resourceGroup : s_defaultResourceGroup);
    }
    CEGUI_CATCH(...)
    {
        Logger::getSingleton().logEvent(
            "SamplesFramework::loadSamplesDataFromXML: "
            "loading of sample data from file '" + filename + "' has failed.",
            Errors);

        CEGUI_RETHROW;
    }
}


void SamplesFramework::addSampleDataCppModule(CEGUI::String sampleName, CEGUI::String summary, CEGUI::String description, SampleType sampleTypeEnum)
{
    SampleData* sampleData = new SampleDataModule(sampleName, summary, description, sampleTypeEnum);

    addSample(sampleData);
}

void SamplesFramework::setDefaultResourceGroup(const String& resourceGroup)
{
    s_defaultResourceGroup = resourceGroup;
}


void SamplesFramework::injectKeyDown(const CEGUI::Key::Scan& ceguiKey)
{
    if(d_selectedSampleData)
    {
        if(Key::Escape != ceguiKey)
            d_selectedSampleData->getGuiContext()->injectKeyDown(ceguiKey);
        else
            handleStopDisplaySample();
    }
    else
    {
        if(Key::Escape != ceguiKey)
            CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(ceguiKey);
        else
            setQuitting(true);
    }
}

void SamplesFramework::injectKeyUp(const CEGUI::Key::Scan& ceguiKey)
{
    if(d_selectedSampleData)
    {
        d_selectedSampleData->getGuiContext()->injectKeyUp(ceguiKey);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp(ceguiKey);
    }
}

void SamplesFramework::injectChar(int character)
{
    if(d_selectedSampleData)
    {
        d_selectedSampleData->getGuiContext()->injectChar(character);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(character);
    }
}

void SamplesFramework::injectMouseButtonDown(const CEGUI::MouseButton& ceguiMouseButton)
{
    if(d_selectedSampleData)
    {
        d_selectedSampleData->getGuiContext()->injectMouseButtonDown(ceguiMouseButton);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(ceguiMouseButton);
    }
}

void SamplesFramework::injectMouseButtonUp(const CEGUI::MouseButton& ceguiMouseButton)
{
    if(d_selectedSampleData)
    {
        d_selectedSampleData->getGuiContext()->injectMouseButtonUp(ceguiMouseButton);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(ceguiMouseButton);
    }
}

void SamplesFramework::injectMouseWheelChange(float position)
{
    if(d_selectedSampleData)
    {
        d_selectedSampleData->getGuiContext()->injectMouseWheelChange(position);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseWheelChange(position);
    }
}

void SamplesFramework::injectMousePosition(float x, float y)
{
    if(d_selectedSampleData)
    {
        d_selectedSampleData->getGuiContext()->injectMousePosition(x, y);
    }
    else
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().injectMousePosition(x, y);
    }
}



void SamplesFramework::update(float passedTime)
{
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectTimePulse(passedTime);

    std::vector<SampleData*>::iterator iter = d_samples.begin();
    std::vector<SampleData*>::iterator end = d_samples.end();
    for(; iter != end; ++iter)
    {
        SampleData* sampleData = *iter;

        GUIContext* guiContext = sampleData->getGuiContext();
        guiContext->injectTimePulse(passedTime);
    }
}

void SamplesFramework::handleNewWindowSize(float width, float height)
{
    d_appWindowWidth = (int)width;
    d_appWindowHeight = (int)height;

    std::vector<SampleData*>::iterator iter = d_samples.begin();
    std::vector<SampleData*>::iterator end = d_samples.end();
    for(; iter != end; ++iter)
    {
        SampleData* sampleData = *iter;

        sampleData->handleNewWindowSize(width, height);
    }

    if(d_samplesWinMgr)
        d_samplesWinMgr->setWindowRatio(width / height);
}

void SamplesFramework::addSample(SampleData* sampleData)
{
    d_samples.push_back(sampleData);
}


void SamplesFramework::drawGUIContexts()
{
    if(!d_selectedSampleData)
    {
        CEGUI::System& gui_system(CEGUI::System::getSingleton());
        gui_system.getDefaultGUIContext().draw();

        std::vector<SampleData*>::iterator iter = d_samples.begin();
        std::vector<SampleData*>::iterator end = d_samples.end();
        for(; iter != end; ++iter)
        {
            SampleData* sampleData = *iter;

            if(!sampleData->getGuiContext())
                continue;

            bool isContextDirty = sampleData->getGuiContext()->isDirty();
            if(isContextDirty)
            {
                sampleData->clearRTTTexture();
                sampleData->getGuiContext()->draw();

                sampleData->getSampleWindow()->invalidate();
            }
        }
    }
    else
    {
        d_selectedSampleData->getGuiContext()->draw();
    }

}

void SamplesFramework::handleSampleSelection(CEGUI::Window* sampleWindow)
{
    SampleData* correspondingSampleData = findSampleData(sampleWindow);

    d_metaDataWinMgr->setSampleInfo(correspondingSampleData);
}


void SamplesFramework::handleStartDisplaySample(CEGUI::Window* sampleWindow)
{
    SampleData* correspondingSampleData = findSampleData(sampleWindow);

    CEGUI::RenderTarget& defaultRenderTarget = CEGUI::System::getSingleton().getRenderer()->getDefaultRenderTarget();
    CEGUI::GUIContext* sampleContext(correspondingSampleData->getGuiContext());
    sampleContext->setRenderTarget(defaultRenderTarget);

    sampleContext->getRootWindow()->addChild(d_sampleExitButton);
    sampleContext->getMouseCursor().setPosition(CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition());

    d_selectedSampleData = correspondingSampleData;
}

void SamplesFramework::handleStopDisplaySample()
{
    GUIContext* sampleGUIContext = d_selectedSampleData->getGuiContext();
    sampleGUIContext->getRootWindow()->removeChild(d_sampleExitButton);
    d_selectedSampleData->setGUIContextRTT();

    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setPosition(sampleGUIContext->getMouseCursor().getPosition());

    d_selectedSampleData = 0;
}


SampleData* SamplesFramework::findSampleData(CEGUI::Window* sampleWindow)
{
    //Find corresponding SampleData
    std::vector<SampleData*>::iterator iter = d_samples.begin();
    std::vector<SampleData*>::iterator end = d_samples.end();
    for(; iter != end; ++iter)
    {
        SampleData* sampleData = *iter;

        if(sampleData->getSampleWindow() == sampleWindow)
            return sampleData;
    }

    return 0;
}

bool SamplesFramework::handleExitSampleView(const CEGUI::EventArgs& args)
{
    handleStopDisplaySample();

    return true;
}

void SamplesFramework::initialiseSamples()
{
    std::vector<SampleData*>::iterator iter = d_samples.begin();
    std::vector<SampleData*>::iterator end = d_samples.end();
    for(; iter != end; ++iter)
    {
        SampleData* sampleData = *iter;

        sampleData->initialise(d_appWindowWidth, d_appWindowHeight);
        CEGUI::FrameWindow* sampleWindow = d_samplesWinMgr->createSampleWindow(sampleData->getName(), sampleData->getRTTImage());
        sampleData->setSampleWindow(sampleWindow);
    }

    d_samplesWinMgr->setWindowRatio(d_appWindowWidth / (float)d_appWindowHeight);
}