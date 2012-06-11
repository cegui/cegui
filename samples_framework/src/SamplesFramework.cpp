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
#include "SamplesWindowManager.h"

#include "CEGUI/CEGUI.h"
#include "CEGUI/Logger.h"


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
    d_samplesWinMgr(0)
{
}

SamplesFramework::~SamplesFramework()
{
    if(d_metaDataWinMgr)
        delete d_metaDataWinMgr;
}



bool SamplesFramework::initialiseSample()
{
    using namespace CEGUI;

    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();

    initialiseFrameworkLayout();

    loadSamplesDataFromXML("samples.samps", s_defaultResourceGroup);

    // return true to signalize the initialisation was sucessful and run the SamplesFramework
    return true;
}

void SamplesFramework::cleanupSample()
{
    unloadSamples();
}

void SamplesFramework::initialiseFrameworkLayout()
{
    SchemeManager::getSingleton().createFromFile("SampleBrowser.scheme");

    System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("SampleBrowserSkin/MouseArrow");

    WindowManager& winMgr = WindowManager::getSingleton();

    d_root = static_cast<DefaultWindow*>(winMgr.createWindow("DefaultWindow", "Root"));
    
    d_root = winMgr.loadLayoutFromFile("SampleBrowser.layout");
    System::getSingleton().getDefaultGUIContext().setRootWindow(d_root);

    CEGUI::Window* metaDataWindow = d_root->getChild("SampleBrowserMetaData");
    d_metaDataWinMgr = new MetaDataWindowManager(metaDataWindow);

    CEGUI::Window* samplesScrollablePane = d_root->getChild("SamplesFrameWindow/SamplesScrollablePane");
    d_samplesWinMgr = new SamplesWindowManager(this, samplesScrollablePane);
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

/*!
\brief
Sets the default resource group to be used when loading samples xml
data

\param resourceGroup
String describing the default resource group identifier to be used.
*/
void SamplesFramework::setDefaultResourceGroup(const String& resourceGroup)
{
    s_defaultResourceGroup = resourceGroup;
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
    std::vector<SampleData*>::iterator iter = d_samples.begin();
    std::vector<SampleData*>::iterator end = d_samples.end();
    for(; iter != end; ++iter)
    {
        SampleData* sampleData = *iter;

        sampleData->handleNewWindowSize(width, height);
    }

    d_samplesWinMgr->setWindowRatio(height/width);
}

CEGUI::FrameWindow* SamplesFramework::createSampleWindow(const CEGUI::String& name, const CEGUI::Image& image)
{
    WindowManager& winMgr = WindowManager::getSingleton();

    FrameWindow* wnd = static_cast<FrameWindow*>(winMgr.createWindow("SampleBrowserSkin/SampleWindow", name));

    
    CEGUI::String imageName = image.getName();
    wnd->setProperty("Image", imageName);

    d_samplesWinMgr->addSampleWindow(wnd);

    return wnd;
}

void SamplesFramework::addSample(SampleData* sampleData)
{
    d_samples.push_back(sampleData);

    sampleData->initialise();
    CEGUI::FrameWindow* sampleWindow= createSampleWindow(sampleData->getName(), sampleData->getRTTImage());;
    sampleData->setSampleWindow(sampleWindow);
}


void SamplesFramework::drawGUIContexts()
{
    std::vector<SampleData*>::iterator iter = d_samples.begin();
    std::vector<SampleData*>::iterator end = d_samples.end();
    for(; iter != end; ++iter)
    {
        SampleData* sampleData = *iter;

        bool isContextDirty = sampleData->getGuiContext()->isDirty();
        if(isContextDirty)
        {
            sampleData->getGuiContext()->draw();
            sampleData->getSampleWindow()->getRenderingSurface()->invalidate();
        }
    }
}

void SamplesFramework::handleNewSampleSelection(CEGUI::Window* sampleWindow)
{
    SampleData* correspondingSampleData = findSampleData(sampleWindow);

    d_metaDataWinMgr->setSampleInfo(correspondingSampleData);
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