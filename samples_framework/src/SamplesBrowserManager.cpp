/***********************************************************************
filename:   SamplesBrowserManager.cpp
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
#include "SamplesBrowserManager.h"

#include "SamplesFramework.h"

#include "CEGUI/Window.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/widgets/VerticalLayoutContainer.h"
#include "CEGUI/widgets/FrameWindow.h"
#include "CEGUI/Image.h"

using namespace CEGUI;

const CEGUI::uint32 SamplesBrowserManager::d_sampleWindowFrameNormal(0xFFFFFFFF);
const CEGUI::uint32 SamplesBrowserManager::d_sampleWindowFrameSelected(0xFF77FFB6);

SamplesBrowserManager::SamplesBrowserManager(SamplesFramework* owner, CEGUI::Window* samplesWindow)
    : d_owner(owner),
    d_root(samplesWindow),
    d_childCount(0),
    d_aspectRatio(1.f),
    d_selectedWindow(0)
{
    init();
}


CEGUI::Window* SamplesBrowserManager::getWindow()
{
    return d_root;
}

CEGUI::FrameWindow* SamplesBrowserManager::createAndAddSampleWindow(const CEGUI::String& name, const CEGUI::Image& image)
{
    WindowManager& winMgr = WindowManager::getSingleton();

    CEGUI::VerticalLayoutContainer* root = static_cast<VerticalLayoutContainer*>(winMgr.createWindow("VerticalLayoutContainer"));
    root->setMaxSize(CEGUI::USize(cegui_absdim(9999999999.f), cegui_absdim(9999999999.f)));

    CEGUI::Window* windowName = winMgr.createWindow("SampleBrowserSkin/StaticText");
    windowName->setSize(CEGUI::USize(cegui_absdim(260.f), cegui_absdim(40.f)));
    windowName->setText(name);
    windowName->setHorizontalAlignment(HA_CENTRE);
    root->addChild(windowName);

    FrameWindow* sampleWindow;
   sampleWindow = static_cast<FrameWindow*>(winMgr.createWindow("SampleBrowserSkin/SampleWindow", name));
    CEGUI::String imageName = image.getName();
    sampleWindow->setProperty("Image", imageName);

        sampleWindow->setSize(USize(UDim(1.f, -10.f), cegui_absdim(1.f)));
    sampleWindow->setMouseInputPropagationEnabled(true);

    sampleWindow->subscribeEvent(Window::EventMouseClick, Event::Subscriber(&SamplesBrowserManager::handleMouseClickSampleWindow, this));
    sampleWindow->subscribeEvent(Window::EventMouseDoubleClick, Event::Subscriber(&SamplesBrowserManager::handleMouseDoubleClickSampleWindow, this));

    CEGUI::ColourRect colRect((CEGUI::Colour(d_sampleWindowFrameNormal)));
    sampleWindow->setProperty("FrameColours", CEGUI::PropertyHelper<ColourRect>::toString(colRect));

    sampleWindow->setAspectMode(AM_EXPAND);
    root->addChild(sampleWindow);

    d_sampleWindows.push_back(sampleWindow);

    d_verticalLayoutContainerSamples->addChild(root);


    ++d_childCount;

    return sampleWindow;
}

void SamplesBrowserManager::setWindowRatio(float aspectRatio)
{
    d_aspectRatio = aspectRatio;

    updateWindows();
}

void SamplesBrowserManager::updateWindows()
{
    float vertOffset = 0.f;

    int max = d_sampleWindows.size();
    for(int i = 0; i < max; ++i)
    {
        CEGUI::Window* window(d_sampleWindows[i]);

        window->setAspectRatio(d_aspectRatio);
        window->setSize(USize(UDim(1.f, -10.f), cegui_absdim(1.f)));
    }

    d_root->setSize(USize(cegui_reldim(1.f), cegui_reldim(1.f)));
}


bool SamplesBrowserManager::handleMouseClickSampleWindow(const CEGUI::EventArgs& args)
{
    const WindowEventArgs& winArgs(static_cast<const WindowEventArgs&>(args));

    CEGUI::Window* wnd(winArgs.window);

    selectSampleWindow(wnd);
    d_owner->handleSampleSelection(wnd);

    return true;
}


bool SamplesBrowserManager::handleMouseDoubleClickSampleWindow(const CEGUI::EventArgs& args)
{
    const WindowEventArgs& winArgs(static_cast<const WindowEventArgs&>(args));

    CEGUI::Window* wnd(winArgs.window);

    d_owner->handleStartDisplaySample(wnd);

    return true;
}


void SamplesBrowserManager::selectSampleWindow(CEGUI::Window* wnd)
{
    if(d_selectedWindow)
    {
        CEGUI::ColourRect colRectNormal = CEGUI::ColourRect(CEGUI::Colour(d_sampleWindowFrameNormal));
        d_selectedWindow->setProperty("FrameColours", CEGUI::PropertyHelper<ColourRect>::toString(colRectNormal));
    }

    d_selectedWindow = wnd;

    CEGUI::ColourRect colRectSelected = CEGUI::ColourRect(CEGUI::Colour(d_sampleWindowFrameSelected));
    d_selectedWindow->setProperty("FrameColours", CEGUI::PropertyHelper<ColourRect>::toString(colRectSelected));
}

void SamplesBrowserManager::init()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    d_verticalLayoutContainerSamples = static_cast<VerticalLayoutContainer*>(winMgr.createWindow("VerticalLayoutContainer"));

    d_verticalLayoutContainerSamples->setSize(CEGUI::USize(cegui_reldim(1.f), cegui_reldim(1.f)));
    d_verticalLayoutContainerSamples->setMaxSize(CEGUI::USize(cegui_absdim(9999999999.f), cegui_absdim(9999999999.f)));

    d_verticalLayoutContainerSamples->setMouseInputPropagationEnabled(true);

    d_root->addChild(d_verticalLayoutContainerSamples);

}