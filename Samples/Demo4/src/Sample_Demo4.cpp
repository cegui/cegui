/************************************************************************
    filename:   Sample_Demo4.cpp
    created:    20/8/2005
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "Sample_Demo4.h"
#include "CEGUI.h"
#include "CEGuiBaseApplication.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#include "windows.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nCmdShow)
#else
int main(int argc, char *argv[])
#endif
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    Demo4Sample app;
    return app.run();
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool Demo4Sample::initialiseSample()
{
    using namespace CEGUI;

    // we will make extensive use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().loadScheme("../datafiles/schemes/TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    FontManager::getSingleton().createFont("../datafiles/fonts/Commonwealth-10.font");

    // load an image to use as a background
    ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "../datafiles/imagesets/GPN-2000-001437.tga");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    StaticImage* background = static_cast<StaticImage*>(winMgr.createWindow("TaharezLook/StaticImage", "background_wnd"));
    // set position and size
    background->setPosition(Point(0, 0));
    background->setSize(Size(1, 1));
    // disable frame and standard background
    background->setFrameEnabled(false);
    background->setBackgroundEnabled(false);
    // set the background image
    background->setImage("BackgroundImage", "full_image");
    // install this as the root GUI sheet
    System::getSingleton().setGUISheet(background);

    // now we create a DefaultWindow which we will attach all the widgets to.  We could
    // have attached them to the background StaticImage, though we want to be a bit tricky
    // since we do not wish the background to be faded by the slider - so we create this
    // container window so we can affect all the other widgets, but leave the background
    // unchanged.
    Window* sheet = winMgr.createWindow("DefaultWindow", "root_wnd");
    // attach this to the 'real' root
    background->addChildWindow(sheet);

    //
    // widget initialisation
    //
    // quit button
    PushButton* btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "QuitButton"));
    sheet->addChildWindow(btn);
    btn->setText("Quit!");
    btn->setPosition(Point(0.035f, 0.0f));
    btn->setSize(Size(0.1f, 0.036f));
    btn->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo4Sample::handleQuit, this));
    btn->setAlwaysOnTop(true);

    // Alpha-slider
    Slider* slider = static_cast<Slider*>(winMgr.createWindow("TaharezLook/Slider", "my slider"));
    sheet->addChildWindow(slider);
    slider->setPosition(Point(0.136f, 0.0f));
    slider->setSize(Size(0.02f, 0.075f));
    slider->setAlwaysOnTop(true);
    // here we disable inherited alpha so that we will always be able to see the slider
    slider->setInheritsAlpha(false);
    // set up slider config
    slider->setCurrentValue(1.0f);
    slider->setClickStep(0.1f);
    // subscribe handler that will process slider position changes.
    slider->subscribeEvent(Slider::EventValueChanged, &sliderHandler);

    // big scroll bar
    Scrollbar* vscb = static_cast<Scrollbar*>(winMgr.createWindow("TaharezLook/LargeVerticalScrollbar", "Vert Scroll 1"));
    sheet->addChildWindow(vscb);
    vscb->setPosition(Point(0.0f, 0.0f));
    vscb->setMinimumSize(Size(0.01f, 0.01f));
    vscb->setMaximumSize(Size(1.0f, 1.0f));
    vscb->setSize(Size(0.035f, 0.83f));
    vscb->setDocumentSize(100);
    vscb->setPageSize(5);
    vscb->setStepSize(1);
    vscb->setAlwaysOnTop(true);

    // mini vert scroll bar
    Scrollbar* mvsb = static_cast<Scrollbar*>(winMgr.createWindow("TaharezLook/VerticalScrollbar", "MiniVertScroll 1"));
    sheet->addChildWindow(mvsb);
    mvsb->setPosition(Point(0.99f, 0.015f));
    mvsb->setMinimumSize(Size(0.01f, 0.01f));
    mvsb->setMaximumSize(Size(1.0f, 1.0f));
    mvsb->setSize(Size(0.01f, 0.5f));
    mvsb->setDocumentSize(360);
    mvsb->setPageSize(45);
    mvsb->setStepSize(1);
    mvsb->setAlwaysOnTop(true);

    // mini horz scroll bar
    Scrollbar* mhsb = static_cast<Scrollbar*>(winMgr.createWindow("TaharezLook/HorizontalScrollbar", "MiniHorzScroll 1"));
    sheet->addChildWindow(mhsb);
    mhsb->setPosition(Point(0.485f, 0.0f));
    mhsb->setMinimumSize(Size(0.01f, 0.01f));
    mhsb->setMaximumSize(Size(1.0f, 1.0f));
    mhsb->setSize(Size(0.5f, 0.015f));
    mhsb->setDocumentSize(360);
    mhsb->setPageSize(45);
    mhsb->setStepSize(1);
    mhsb->setAlwaysOnTop(true);

    //
    // Build a window with some text and formatting options via radio buttons etc
    //
    FrameWindow* textwnd = static_cast<FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "TextWindow"));
    sheet->addChildWindow(textwnd);
    textwnd->setPosition(Point(0.2f, 0.2f));
    textwnd->setMaximumSize(Size(0.75f, 0.75f));
    textwnd->setMinimumSize(Size(0.1f, 0.1f));
    textwnd->setSize(Size(0.5f, 0.5f));
    textwnd->setCloseButtonEnabled(false);
    textwnd->setText("Crazy Eddie's GUI - Demo 4");

    StaticText* st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Static"));
    textwnd->addChildWindow(st);
    st->setPosition(Point(0.1f, 0.2f));
    st->setSize(Size(0.5f, 0.6));

    st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Group label 1"));
    textwnd->addChildWindow(st);
    st->setPosition(Point(0.65f, 0.23f));
    st->setSize(Size(0.35f, 0.05f));
    st->setText("Horz. Formatting");
    // disable frame and background on static control
    st->setFrameEnabled(false);
    st->setBackgroundEnabled(false);

    st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Group label 2"));
    textwnd->addChildWindow(st);
    st->setPosition(Point(0.65f, 0.53f));
    st->setSize(Size(0.35f, 0.05f));
    st->setText("Vert. Formatting");
    // disable frame and background on static control
    st->setFrameEnabled(false);
    st->setBackgroundEnabled(false);

    st = static_cast<StaticText*>(winMgr.createWindow("TaharezLook/StaticText", "TextWindow/Box label"));
    textwnd->addChildWindow(st);
    st->setPosition(Point(0.12f, 0.13f));
    st->setSize(Size(0.35f, 0.05f));
    st->setText("Formatted Output");
    // disable frame and background on static control
    st->setFrameEnabled(false);
    st->setBackgroundEnabled(false);

    // word-wrap checkbox
    Checkbox* cb = static_cast<Checkbox*>(winMgr.createWindow("TaharezLook/Checkbox", "TextWindow/CB1"));
    textwnd->addChildWindow(cb);
    cb->setPosition(Point(0.65f, 0.13f));
    cb->setSize(Size(0.35f, 0.05f));
    cb->setText("Word Wrap");
    // subscribe a handler to listen for when the check-box button select state changes
    cb->subscribeEvent(Checkbox::EventCheckStateChanged, &formatChangedHandler);

    // horizontal formatting radio group
    RadioButton* rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB1"));
    textwnd->addChildWindow(rb);
    rb->setPosition(Point(0.65f, 0.3f));
    rb->setSize(Size(0.35f, 0.05f));
    rb->setGroupID(1);
    rb->setText("Left Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB2"));
    textwnd->addChildWindow(rb);
    rb->setPosition(Point(0.65f, 0.35f));
    rb->setSize(Size(0.35f, 0.05f));
    rb->setGroupID(1);
    rb->setText("Right Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB3"));
    textwnd->addChildWindow(rb);
    rb->setPosition(Point(0.65f, 0.4f));
    rb->setSize(Size(0.35f, 0.05f));
    rb->setGroupID(1);
    rb->setText("Centred");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    // vertical formatting radio group
    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB4"));
    textwnd->addChildWindow(rb);
    rb->setPosition(Point(0.65f, 0.6f));
    rb->setSize(Size(0.35f, 0.05f));
    rb->setGroupID(2);
    rb->setText("Top Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB5"));
    textwnd->addChildWindow(rb);
    rb->setPosition(Point(0.65f, 0.65f));
    rb->setSize(Size(0.35f, 0.05f));
    rb->setGroupID(2);
    rb->setText("Bottom Aligned");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    rb = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "TextWindow/RB6"));
    textwnd->addChildWindow(rb);
    rb->setPosition(Point(0.65f, 0.7f));
    rb->setSize(Size(0.35f, 0.05f));
    rb->setGroupID(2);
    rb->setText("Centred");
    // subscribe a handler to listen for when the radio button select state changes
    rb->subscribeEvent(RadioButton::EventSelectStateChanged, &formatChangedHandler);

    // Edit box for text entry
    Editbox* eb = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "TextWindow/Editbox1"));
    textwnd->addChildWindow(eb);
    eb->setPosition(Point(0.05f, 0.85f));
    eb->setMaximumSize(Size(1.0f, 0.04f));
    eb->setSize(Size(0.90f, 0.08f));
    // subscribe a handler to listen for when the text changes
    eb->subscribeEvent(Window::EventTextChanged, &textChangedHandler);

    //
    // Controls are set up.  Install initial settings
    //
    static_cast<Checkbox*>(winMgr.getWindow("TextWindow/CB1"))->setSelected(true);
    static_cast<RadioButton*>(winMgr.getWindow("TextWindow/RB1"))->setSelected(true);
    static_cast<RadioButton*>(winMgr.getWindow("TextWindow/RB4"))->setSelected(true);
    winMgr.getWindow("TextWindow/Editbox1")->setText("Come on then, edit me!");

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void Demo4Sample::cleanupSample()
{
    // nothing to do here!
}

/*************************************************************************
    Free function to handle slider position changes
*************************************************************************/
bool sliderHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // we know it's a slider.
    Slider* s = static_cast<Slider*>(static_cast<const WindowEventArgs&>(e).window);

    // get value from slider and set it as the current alpha
    float val = s->getCurrentValue();
    WindowManager::getSingleton().getWindow("root_wnd")->setAlpha(val);

    // indicate the event was handled here.
    return true;
}

/*************************************************************************
    Free function to handle change of format options
*************************************************************************/
bool formatChangedHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // we will use the WindowManager to get access to the widgets
    WindowManager& winMgr = WindowManager::getSingleton();

    // get pointers to all the widgets we need to access
    const RadioButton* rb1 = static_cast<const RadioButton*>(winMgr.getWindow("TextWindow/RB1"));
    const RadioButton* rb2 = static_cast<const RadioButton*>(winMgr.getWindow("TextWindow/RB2"));
    const RadioButton* rb3 = static_cast<const RadioButton*>(winMgr.getWindow("TextWindow/RB3"));
    const RadioButton* rb4 = static_cast<const RadioButton*>(winMgr.getWindow("TextWindow/RB4"));
    const RadioButton* rb5 = static_cast<const RadioButton*>(winMgr.getWindow("TextWindow/RB5"));
    const RadioButton* rb6 = static_cast<const RadioButton*>(winMgr.getWindow("TextWindow/RB6"));
    const Checkbox*    cb1 = static_cast<const Checkbox*>(winMgr.getWindow("TextWindow/CB1"));
    // and also the static text for which we will set the formatting options
    StaticText*  st  = static_cast<StaticText*>(winMgr.getWindow("TextWindow/Static"));

    // handle vertical formatting settings
    if (rb4->isSelected())
        st->setVerticalFormatting(StaticText::TopAligned);
    else if (rb5->isSelected())
        st->setVerticalFormatting(StaticText::BottomAligned);
    else if (rb6->isSelected())
        st->setVerticalFormatting(StaticText::VertCentred);

    // handle horizontal formatting settings
    bool wrap = cb1->isSelected();

    if (rb1->isSelected())
        st->setHorizontalFormatting(wrap ? StaticText::WordWrapLeftAligned : StaticText::LeftAligned);
    else if (rb2->isSelected())
        st->setHorizontalFormatting(wrap ? StaticText::WordWrapRightAligned : StaticText::RightAligned);
    else if (rb3->isSelected())
        st->setHorizontalFormatting(wrap ? StaticText::WordWrapCentred : StaticText::HorzCentred);

    // event was handled
    return true;
}

/*************************************************************************
    Free function handler called when editbox text changes
*************************************************************************/
bool textChangedHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    //find the static box
    StaticText* st = static_cast<StaticText*>(WindowManager::getSingleton().getWindow("TextWindow/Static"));

    // set text from the edit box...
    st->setText(static_cast<const WindowEventArgs&>(e).window->getText());

    return true;
}

/*************************************************************************
    Handler method that signals the application to quit
*************************************************************************/
bool Demo4Sample::handleQuit(const CEGUI::EventArgs& e)
{
    // signal quit
    d_sampleApp->setQuitting();

    // event was handled
    return true;
}

/*************************************************************************
    Handler method that ??
*************************************************************************/
bool Demo4Sample::horzMoveHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    return true;
}

/*************************************************************************
    Handler method that ??
*************************************************************************/
bool Demo4Sample::vertMoveHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    return true;
}

/*************************************************************************
    Handler method that ??
*************************************************************************/
bool Demo4Sample::vscrollHandler(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    return true;
}
