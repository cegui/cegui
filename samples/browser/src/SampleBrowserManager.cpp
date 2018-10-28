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
#include "SampleBrowserManager.h"

#include "SampleBrowser.h"

#include "CEGUI/Window.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/widgets/DefaultWindow.h"
#include "CEGUI/widgets/VerticalLayoutContainer.h"
#include "CEGUI/widgets/HorizontalLayoutContainer.h"
#include "CEGUI/widgets/FrameWindow.h"
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/Image.h"
#include "CEGUI/InputAggregator.h"
#include "CEGUI/falagard/WidgetLookManager.h"

using namespace CEGUI;

const std::uint32_t SampleBrowserManager::d_sampleWindowFrameNormal(0xFFFFFFFF);
const std::uint32_t SampleBrowserManager::d_sampleWindowFrameSelected(0xFF77FFB6);

SampleBrowserManager::SampleBrowserManager(SampleBrowser* owner, CEGUI::Window* samplesWindow)
    : d_owner(owner),
    d_root(samplesWindow),
    d_childCount(0),
    d_aspectRatio(1.0f),
    d_selectedWindow(nullptr)
{
    init();
}


CEGUI::Window* SampleBrowserManager::getWindow()
{
    return d_root;
}

CEGUI::FrameWindow* SampleBrowserManager::createSampleWindow(const CEGUI::String& name, const CEGUI::Image& image)
{
    CEGUI::VerticalLayoutContainer* root = createPreviewLayoutContainer();

    CEGUI::HorizontalLayoutContainer* header = createPreviewHeader();
    root->addChild(header);

    CEGUI::DefaultWindow* windowName = createPreviewHeaderNameWindow(name);
    header->addChild(windowName);

    CEGUI::PushButton* entryButton = createPreviewHeaderEnterButton();
    windowName->addChild(entryButton);

    FrameWindow* sampleWindow = createPreviewSampleWindow(name, image);
    root->addChild(sampleWindow);

    d_sampleWindows.push_back(sampleWindow);
    d_buttonToSampleWindowMap[entryButton] = sampleWindow;

    d_verticalLayoutContainerSamples->addChild(root);


    ++d_childCount;

    return sampleWindow;
}

void SampleBrowserManager::setWindowRatio(float aspectRatio)
{
    d_aspectRatio = aspectRatio;

    updateWindows();
}

void SampleBrowserManager::updateWindows()
{
    int max = d_sampleWindows.size();
    for(int i = 0; i < max; ++i)
    {
        CEGUI::Window* window(d_sampleWindows[i]);

        window->setAspectRatio(d_aspectRatio);
        window->setSize(USize(UDim(1.0f, -10.0f), cegui_absdim(1.0f)));
    }

    d_root->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
}

bool SampleBrowserManager::handlePointerActivateSampleWindow(const CEGUI::EventArgs& args)
{
    const WindowEventArgs& winArgs(static_cast<const WindowEventArgs&>(args));

    CEGUI::Window* wnd(winArgs.window);

    d_owner->handleStartDisplaySample(wnd);

    return true;
}


bool SampleBrowserManager::handlePointerMoveSampleWindow(const CEGUI::EventArgs& args)
{
    const CursorInputEventArgs& cursor_args(static_cast<const CursorInputEventArgs&>(args));

    CEGUI::Window* wnd(cursor_args.window);

    if(d_selectedWindow != wnd)
    {
        selectSampleWindow(wnd);
        d_owner->handleSampleSelection(wnd);
    }

    const CEGUI::String& lookNFeel(wnd->getLookNFeel());
    CEGUI::Rectf innerRectangle = CEGUI::WidgetLookManager::getSingleton().getWidgetLook(lookNFeel).getNamedArea("InnerArea").getArea().getPixelRect(*wnd);

    const glm::vec2& cursor_pos(cursor_args.position);

    const CEGUI::Rectf& windowDimensions(wnd->getUnclippedOuterRect().get());

    const float relPosX = (cursor_pos.x - windowDimensions.left() - innerRectangle.getPosition().x) / innerRectangle.getWidth();
    const float relPosY = (cursor_pos.y - windowDimensions.top()  - innerRectangle.getPosition().y) / innerRectangle.getHeight();

    if (relPosX >= 0.0f && relPosX <= 1.0f && relPosY >= 0.0f && relPosY <= 1.0f)
    {
        SampleHandler* sampleData = d_owner->findSampleData(wnd);
        const CEGUI::Sizef& contextSize(sampleData->getGuiContext()->getSurfaceSize());

        const float absPosX = relPosX * contextSize.d_width;
        const float absPosY = relPosY * contextSize.d_height;

        sampleData->getInputAggregator()->injectMousePosition(absPosX, absPosY);
        sampleData->getGuiContext()->markAsDirty();

        wnd->setCursor("SampleBrowserSkin/MouseArrowHover");
    }
    else
    {
        wnd->setCursor("SampleBrowserSkin/MouseArrow");
    }

    return true;
}

bool SampleBrowserManager::handleLeaveSampleWindow(const CEGUI::EventArgs& args)
{
    const CursorInputEventArgs& cursor_args(static_cast<const CursorInputEventArgs&>(args));

    CEGUI::Window* wnd(cursor_args.window);
    wnd->setCursor("SampleBrowserSkin/MouseArrow");

    return true;
}

void SampleBrowserManager::selectSampleWindow(CEGUI::Window* wnd)
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

void SampleBrowserManager::init()
{
    WindowManager& winMgr(WindowManager::getSingleton());

    d_verticalLayoutContainerSamples = static_cast<VerticalLayoutContainer*>(winMgr.createWindow("VerticalLayoutContainer"));
    d_verticalLayoutContainerSamples->setSize(USize::zero());

    d_verticalLayoutContainerSamples->setMargin(CEGUI::UBox(cegui_reldim(0.0f), cegui_reldim(0.1f), cegui_absdim(14.f), cegui_reldim(0.1f)));
    d_verticalLayoutContainerSamples->setCursorInputPropagationEnabled(true);

    d_root->addChild(d_verticalLayoutContainerSamples);

}

CEGUI::DefaultWindow* SampleBrowserManager::createPreviewHeaderNameWindow(const CEGUI::String& name)
{
    WindowManager& winMgr(WindowManager::getSingleton());

    CEGUI::DefaultWindow* windowName = static_cast<DefaultWindow*>(winMgr.createWindow("SampleBrowserSkin/StaticText"));
    windowName->setSize(CEGUI::USize(cegui_reldim(0.6f), cegui_absdim(44.f)));
    windowName->setText(name);
    windowName->setFont("DejaVuSans-12");
    windowName->setProperty("HorzFormatting", "Centre");
    windowName->setCursorInputPropagationEnabled(true);

    return windowName;
}

CEGUI::VerticalLayoutContainer* SampleBrowserManager::createPreviewLayoutContainer()
{
    WindowManager& winMgr(WindowManager::getSingleton());

    CEGUI::VerticalLayoutContainer* root = static_cast<VerticalLayoutContainer*>(winMgr.createWindow("VerticalLayoutContainer"));
    root->setSize(CEGUI::USize(cegui_reldim(0.8f), cegui_reldim(1.0f)));
    root->setCursorInputPropagationEnabled(true);
    root->setMargin(CEGUI::UBox(UDim(0.0f, 0.0f),UDim(0.0f, 0.0f),UDim(0.0f, 8.f), UDim(0.0f, 0.0f)));

    return root;
}

CEGUI::FrameWindow* SampleBrowserManager::createPreviewSampleWindow(const CEGUI::String& name, const CEGUI::Image &image)
{
    WindowManager& winMgr(WindowManager::getSingleton());

    FrameWindow* sampleWindow = static_cast<FrameWindow*>(winMgr.createWindow("SampleBrowserSkin/SampleWindow", name));
    CEGUI::String imageName = image.getName();
    sampleWindow->setProperty("Image", imageName);

    sampleWindow->setSize(USize(UDim(1.0f, -10.0f), cegui_absdim(1.0f)));
    sampleWindow->setCursorInputPropagationEnabled(true);

    sampleWindow->subscribeEvent(Window::EventCursorMove, Event::Subscriber(&SampleBrowserManager::handlePointerMoveSampleWindow, this));
    sampleWindow->subscribeEvent(Window::EventCursorActivate, Event::Subscriber(&SampleBrowserManager::handlePointerActivateSampleWindow, this));
    sampleWindow->subscribeEvent(Window::EventCursorLeavesArea, Event::Subscriber(&SampleBrowserManager::handleLeaveSampleWindow, this));

    CEGUI::ColourRect colRect((CEGUI::Colour(d_sampleWindowFrameNormal)));
    sampleWindow->setProperty("FrameColours", CEGUI::PropertyHelper<ColourRect>::toString(colRect));

    sampleWindow->setAspectMode(AspectMode::Expand);

    return sampleWindow;
}

CEGUI::HorizontalLayoutContainer* SampleBrowserManager::createPreviewHeader()
{
    WindowManager& winMgr(WindowManager::getSingleton());

    CEGUI::HorizontalLayoutContainer* header = static_cast<HorizontalLayoutContainer*>(winMgr.createWindow("HorizontalLayoutContainer"));
    header->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_absdim(40.0f)));
    header->setCursorInputPropagationEnabled(true);
    header->setMargin(CEGUI::UBox(UDim(0.0f, 12.f),UDim(0.0f, 0.0f),UDim(0.0f, 0), UDim(0.0f, 0.0f)));
    header->setHorizontalAlignment(HorizontalAlignment::Centre);

    return header;
}

CEGUI::PushButton* SampleBrowserManager::createPreviewHeaderEnterButton()
{
    WindowManager& winMgr(WindowManager::getSingleton());

    CEGUI::PushButton* button = static_cast<CEGUI::PushButton*>(winMgr.createWindow("SampleBrowserSkin/Button", "SampleEntryButton"));

    button->setSize(CEGUI::USize(cegui_absdim(1.0f), cegui_reldim(0.7f)));
    button->setAspectMode(AspectMode::Expand);
    button->setAspectRatio(1.0f);
    button->setPosition(CEGUI::UVector2(cegui_absdim(-7.f), cegui_absdim(0.0f)));
    button->setCursorInputPropagationEnabled(true);
    button->setProperty("NormalImage", "SampleBrowserSkin/EntryButtonNormal");
    button->setProperty("HoverImage", "SampleBrowserSkin/EntryButtonHover");
    button->setProperty("PushedImage", "SampleBrowserSkin/EntryButtonClicked");
    button->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&SampleBrowserManager::handleSampleEnterButtonClicked, this));
    button->setAlwaysOnTop(true);
    button->setHorizontalAlignment(HorizontalAlignment::Right);
    button->setVerticalAlignment(VerticalAlignment::Centre);
    button->setAlwaysOnTop(true);

    return button;
}


bool SampleBrowserManager::handleSampleEnterButtonClicked(const CEGUI::EventArgs& args)
{
    const WindowEventArgs& winArgs(static_cast<const WindowEventArgs&>(args));

    CEGUI::Window* sampleWindow = d_buttonToSampleWindowMap[winArgs.window];
    d_owner->handleStartDisplaySample(sampleWindow);

    return true;
}
