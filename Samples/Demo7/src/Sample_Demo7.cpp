/************************************************************************
    filename:   Sample_Demo7.cpp
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
#include "Sample_Demo7.h"
#include "CEGUI.h"
#include "CEGuiBaseApplication.h"

#include <cstdlib>

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
    Demo7Sample app;
    return app.run();
}


/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool Demo7Sample::initialiseSample()
{
    using namespace CEGUI;

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().loadScheme("TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook", "MouseArrow");
    FontManager::getSingleton().createFont("Commonwealth-10.font");

    // load an image to use as a background
    ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setWindowPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setWindowSize(UVector2(cegui_reldim(1), cegui_reldim( 1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "set:BackgroundImage image:full_image");
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

    // do demo stuff
    createDemoWindows();
    initDemoEventWiring();

    // success!
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void Demo7Sample::cleanupSample()
{
    // nothing to do here!
}

/*************************************************************************
    create the windows & widgets for this demo
*************************************************************************/
void Demo7Sample::createDemoWindows(void)
{
    using namespace CEGUI;

    WindowManager& winMgr = WindowManager::getSingleton();
    Window* root = winMgr.getWindow("root_wnd");

    // Create first frame window
    FrameWindow* fwnd1 = static_cast<FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "Demo7/Window1"));
    root->addChildWindow(fwnd1);
    fwnd1->setWindowMinSize(UVector2(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    fwnd1->setWindowMaxSize(UVector2(cegui_reldim(0.8f), cegui_reldim( 0.8f)));
    fwnd1->setWindowPosition(UVector2(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    fwnd1->setWindowSize(UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f)));
    fwnd1->setText("Demo 7 - Window 1");
    fwnd1->setCloseButtonEnabled(false);

    // create second frame window
    FrameWindow* fwnd2 = static_cast<FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "Demo7/Window2"));
    root->addChildWindow(fwnd2);
    fwnd2->setWindowMinSize(UVector2(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    fwnd2->setWindowMaxSize(UVector2(cegui_reldim(0.8f), cegui_reldim( 0.8f)));
    fwnd2->setWindowPosition(UVector2(cegui_reldim(0.4), cegui_reldim( 0.1f)));
    fwnd2->setWindowSize(UVector2(cegui_reldim(0.5f), cegui_reldim( 0.6f)));
    fwnd2->setText("Demo 7 - Window 2");
    fwnd2->setCloseButtonEnabled(false);

    // create third and final frame window.
    FrameWindow* fwnd3 = static_cast<FrameWindow*>(winMgr.createWindow("TaharezLook/FrameWindow", "Demo7/Window3"));
    root->addChildWindow(fwnd3);
    fwnd3->setWindowMinSize(UVector2(cegui_reldim(0.2f), cegui_reldim( 0.2f)));
    fwnd3->setWindowMaxSize(UVector2(cegui_reldim(0.8f), cegui_reldim( 0.8f)));
    fwnd3->setWindowPosition(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.4f)));
    fwnd3->setWindowSize(UVector2(cegui_reldim(0.5f), cegui_reldim( 0.5f)));
    fwnd3->setText("Demo 7 - Window 3");
    fwnd3->setCloseButtonEnabled(false);

    // create quit button
    PushButton* btn = static_cast<PushButton*>(winMgr.createWindow("TaharezLook/Button", "Demo7/Window1/Quit"));
    fwnd1->addChildWindow(btn);
    btn->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    btn->setWindowPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.1f)));
    btn->setWindowSize(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.1f)));
    btn->setText("Exit Demo");

    // create first radio-button for StaticImage image selection
    RadioButton* rbtn = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "Demo7/Window1/Radio1"));
    fwnd1->addChildWindow(rbtn);
    rbtn->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    rbtn->setWindowPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.23f)));
    rbtn->setWindowSize(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.075f)));
    rbtn->setText("Option 1");
    rbtn->setID(0);

    // create second radio-button for StaticImage image selection
    rbtn = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "Demo7/Window1/Radio2"));
    fwnd1->addChildWindow(rbtn);
    rbtn->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    rbtn->setWindowPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.306f)));
    rbtn->setWindowSize(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.075f)));
    rbtn->setText("Option 2");
    rbtn->setID(1);

    // create third radio-button for StaticImage image selection
    rbtn = static_cast<RadioButton*>(winMgr.createWindow("TaharezLook/RadioButton", "Demo7/Window1/Radio3"));
    fwnd1->addChildWindow(rbtn);
    rbtn->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    rbtn->setWindowPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.382f)));
    rbtn->setWindowSize(UVector2(cegui_reldim(0.25f), cegui_reldim( 0.075f)));
    rbtn->setText("Option 3");
    rbtn->setID(2);

    // create checkbox to show / hide frame window 3 (with the multi-line editbox in it)
    Checkbox* cbox = static_cast<Checkbox*>(winMgr.createWindow("TaharezLook/Checkbox", "Demo7/Window1/Checkbox"));
    fwnd1->addChildWindow(cbox);
    cbox->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    cbox->setWindowPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.48f)));
    cbox->setWindowSize(UVector2(cegui_reldim(0.45f), cegui_reldim( 0.075f)));
    cbox->setText("Enable some option");
    cbox->setSelected(true);

    // create a static text with some information in.
    Window* stxt = winMgr.createWindow("TaharezLook/StaticText", "Demo7/Window1/Text1");
    fwnd1->addChildWindow(stxt);
    stxt->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    stxt->setWindowPosition(UVector2(cegui_reldim(0.5f), cegui_reldim( 0.1f)));
    stxt->setWindowSize(UVector2(cegui_reldim(0.45f), cegui_reldim( 0.5f)));
    stxt->setText("This is a re-creation of the original \"Demo7\" preview application, it shows many of the CEGUI widgets.");
    stxt->setProperty("FrameEnabled", "false");
    stxt->setProperty("BackgroundEnabled", "false");
    stxt->setProperty("HorzFormatting", "WordWrapCentred");

    // single line edit box widget
    Editbox* ebox = static_cast<Editbox*>(winMgr.createWindow("TaharezLook/Editbox", "Demo7/Window1/Editbox"));
    fwnd1->addChildWindow(ebox);
    ebox->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    ebox->setWindowPosition(UVector2(cegui_reldim(0.2f), cegui_reldim( 0.6f)));
    ebox->setWindowSize(UVector2(cegui_reldim(0.6f), cegui_reldim( 0.1f)));
    ebox->setText("Single-line editbox");

    // large vertical 'Taharez' scrollbar
    Scrollbar* sbar = static_cast<Scrollbar*>(winMgr.createWindow("TaharezLook/LargeVerticalScrollbar", "Demo7/Window1/Scrollbar1"));
    fwnd1->addChildWindow(sbar);
    sbar->setWindowMinSize(UVector2(cegui_reldim(0.01f), cegui_reldim( 0.1f)));
    sbar->setWindowMaxSize(UVector2(cegui_reldim(0.03f), cegui_reldim( 1.0f)));
    sbar->setWindowPosition(UVector2(cegui_reldim(0.06f), cegui_reldim( 0.6f)));
    sbar->setWindowSize(UVector2(cegui_reldim(0.06f), cegui_reldim( 0.38f)));

    // Slider to affect display alpha and progress bars
    Slider* sldr = static_cast<Slider*>(winMgr.createWindow("TaharezLook/Slider", "Demo7/Window1/Slider1"));
    fwnd1->addChildWindow(sldr);
    sldr->setWindowMinSize(UVector2(cegui_reldim(0.01f), cegui_reldim( 0.1f)));
    sldr->setWindowMaxSize(UVector2(cegui_reldim(0.03f), cegui_reldim( 1.0f)));
    sldr->setWindowPosition(UVector2(cegui_reldim(0.9f), cegui_reldim( 0.625f)));
    sldr->setWindowSize(UVector2(cegui_reldim(0.03f), cegui_reldim( 0.28f)));
    sldr->setCurrentValue(1.0f);

    // First progress bar
    ProgressBar* pbar = static_cast<ProgressBar*>(winMgr.createWindow("TaharezLook/ProgressBar", "Demo7/Window2/Progbar1"));
    fwnd2->addChildWindow(pbar);
    pbar->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    pbar->setWindowPosition(UVector2(cegui_reldim(0.04f), cegui_reldim( 0.1f)));
    pbar->setWindowSize(UVector2(cegui_reldim(0.94f), cegui_reldim( 0.05f)));
    pbar->setProgress(0.33f);

    // second progress bar in the alternative style
    pbar = static_cast<ProgressBar*>(winMgr.createWindow("TaharezLook/AlternateProgressBar", "Demo7/Window2/Progbar2"));
    fwnd2->addChildWindow(pbar);
    pbar->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    pbar->setWindowPosition(UVector2(cegui_reldim(0.04f), cegui_reldim( 0.16f)));
    pbar->setWindowSize(UVector2(cegui_reldim(0.94f), cegui_reldim( 0.05f)));
    pbar->setProgress(0.7f);

    // standard listbox
    Listbox* lbox = static_cast<Listbox*>(winMgr.createWindow("TaharezLook/Listbox", "Demo7/Window2/Listbox"));
    fwnd2->addChildWindow(lbox);
    lbox->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    lbox->setWindowPosition(UVector2(cegui_reldim(0.04f), cegui_reldim( 0.25f)));
    lbox->setWindowSize(UVector2(cegui_reldim(0.42f), cegui_reldim( 0.3f)));

    // combobox widget
    Combobox* cbobox = static_cast<Combobox*>(winMgr.createWindow("TaharezLook/Combobox", "Demo7/Window2/Combobox"));
    fwnd2->addChildWindow(cbobox);
    cbobox->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    cbobox->setWindowPosition(UVector2(cegui_reldim(0.5f), cegui_reldim( 0.25f)));
    cbobox->setWindowSize(UVector2(cegui_reldim(0.42f), cegui_reldim( 0.45f)));

    // multi-column list (MCL / Grid) widget
    MultiColumnList* mclbox = static_cast<MultiColumnList*>(winMgr.createWindow("TaharezLook/MultiColumnList", "Demo7/Window2/MultiColumnList"));
    fwnd2->addChildWindow(mclbox);
    mclbox->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    mclbox->setWindowPosition(UVector2(cegui_reldim(0.05f), cegui_reldim( 0.6f)));
    mclbox->setWindowSize(UVector2(cegui_reldim(0.9f), cegui_reldim( 0.38f)));
    mclbox->setSelectionMode(MultiColumnList::RowSingle);

    // StaticImage
    Window* simg = winMgr.createWindow("TaharezLook/StaticImage", "Demo7/Window2/Image1");
    fwnd2->addChildWindow(simg);
    simg->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    simg->setWindowPosition(UVector2(cegui_reldim(0.56f), cegui_reldim( 0.35f)));
    simg->setWindowSize(UVector2(cegui_reldim(0.3f), cegui_reldim( 0.2f)));
    simg->setProperty("FrameEnabled", "false");
    simg->setProperty("BackgroundEnabled", "false");
    simg->setProperty("Image", "set:BackgroundImage image:full_image");

    // Multi-line editbox widget
    MultiLineEditbox* mleb = static_cast<MultiLineEditbox*>(winMgr.createWindow("TaharezLook/MultiLineEditbox", "Demo7/Window3/MLEditbox"));
    fwnd3->addChildWindow(mleb);
    mleb->setWindowMaxSize(UVector2(cegui_reldim(1.0f), cegui_reldim( 1.0f)));
    mleb->setWindowPosition(UVector2(cegui_reldim(0.02f), cegui_reldim( 0.1f)));
    mleb->setWindowSize(UVector2(cegui_reldim(0.96f), cegui_reldim( 0.85f)));
    mleb->setText("Multi-line edit box.  Edit me!\nCa va marcher que je te dis\nComme ca\nLigne 4");

    //
    // List box setup
    //
    // Add items to normal listbox
    lbox->addItem(new MyListItem("Listbox Item 1"));
    lbox->addItem(new MyListItem("Listbox Item 2"));
    lbox->addItem(new MyListItem("Listbox Item 3"));
    lbox->addItem(new MyListItem("Listbox Item 4"));
    lbox->addItem(new MyListItem("Listbox Item 5"));
    lbox->addItem(new MyListItem("Listbox Item 6"));
    lbox->addItem(new MyListItem("Listbox Item 7"));

    // add items to the combobox list
    cbobox->addItem(new MyListItem("Combobox Item 1"));
    cbobox->addItem(new MyListItem("Combobox Item 2"));
    cbobox->addItem(new MyListItem("Combobox Item 3"));
    cbobox->addItem(new MyListItem("Combobox Item 4"));
    cbobox->addItem(new MyListItem("Combobox Item 5"));
    cbobox->addItem(new MyListItem("Combobox Item 6"));
    cbobox->addItem(new MyListItem("Combobox Item 7"));
    cbobox->addItem(new MyListItem("Combobox Item 8"));
    cbobox->addItem(new MyListItem("Combobox Item 9"));
    cbobox->addItem(new MyListItem("Combobox Item 10"));

    // Add columns to MCL
    mclbox->addColumn("Server Name", 0, cegui_reldim(0.33f));
    mclbox->addColumn("Address", 1, cegui_reldim(0.5f));
    mclbox->addColumn("Ping", 2, cegui_reldim(0.2f));

    // Add some empty rows to the MCL
    mclbox->addRow();
    mclbox->addRow();
    mclbox->addRow();
    mclbox->addRow();
    mclbox->addRow();

    // Set first row item texts for the MCL
    mclbox->setItem(new MyListItem("Laggers World"), 0, 0);
    mclbox->setItem(new MyListItem("yourgame.some-server.com"), 1, 0);
    mclbox->setItem(new MyListItem("1000ms"), 2, 0);

    // Set second row item texts for the MCL
    mclbox->setItem(new MyListItem("Super-Server"), 0, 1);
    mclbox->setItem(new MyListItem("whizzy.fakenames.net"), 1, 1);
    mclbox->setItem(new MyListItem("8ms"), 2, 1);

    // Set third row item texts for the MCL
    mclbox->setItem(new MyListItem("Cray-Z-Eds"), 0, 2);
    mclbox->setItem(new MyListItem("crayzeds.notarealserver.co.uk"), 1, 2);
    mclbox->setItem(new MyListItem("43ms"), 2, 2);

    // Set fourth row item texts for the MCL
    mclbox->setItem(new MyListItem("Fake IPs"), 0, 3);
    mclbox->setItem(new MyListItem("123.320.42.242"), 1, 3);
    mclbox->setItem(new MyListItem("63ms"), 2, 3);

    // Set fifth row item texts for the MCL
    mclbox->setItem(new MyListItem("Yet Another Game Server"), 0, 4);
    mclbox->setItem(new MyListItem("abc.abcdefghijklmn.org"), 1, 4);
    mclbox->setItem(new MyListItem("284ms"), 2, 4);
}


/*************************************************************************
    Perform required event hook-ups for this demo.
*************************************************************************/
void Demo7Sample::initDemoEventWiring(void)
{
    using namespace CEGUI;

    // Subscribe handler that quits the application
    WindowManager::getSingleton().getWindow("Demo7/Window1/Quit")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo7Sample::handleQuit, this));

    // Subscribe handler that processes changes to the slider position.
    WindowManager::getSingleton().getWindow("Demo7/Window1/Slider1")->
        subscribeEvent(Slider::EventValueChanged, Event::Subscriber(&Demo7Sample::handleSlider, this));

    // Subscribe handler that processes changes to the checkbox selection state.
    WindowManager::getSingleton().getWindow("Demo7/Window1/Checkbox")->
        subscribeEvent(Checkbox::EventCheckStateChanged, Event::Subscriber(&Demo7Sample::handleCheck, this));

    // Subscribe handler that processes changes to the radio button selection state.
    WindowManager::getSingleton().getWindow("Demo7/Window1/Radio1")->
        subscribeEvent(RadioButton::EventSelectStateChanged, Event::Subscriber(&Demo7Sample::handleRadio, this));

    // Subscribe handler that processes changes to the radio button selection state.
    WindowManager::getSingleton().getWindow("Demo7/Window1/Radio2")->
        subscribeEvent(RadioButton::EventSelectStateChanged, Event::Subscriber(&Demo7Sample::handleRadio, this));

    // Subscribe handler that processes changes to the radio button selection state.
    WindowManager::getSingleton().getWindow("Demo7/Window1/Radio3")->
        subscribeEvent(RadioButton::EventSelectStateChanged, Event::Subscriber(&Demo7Sample::handleRadio, this));
}

bool Demo7Sample::handleQuit(const CEGUI::EventArgs& e)
{
    // signal quit
    d_sampleApp->setQuitting();

    // event was handled
    return true;
}

bool Demo7Sample::handleSlider(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get the current slider value
    float val = static_cast<Slider*>(static_cast<const WindowEventArgs&>(e).window)->getCurrentValue();

    // set the progress for the first bar according to the slider value
    static_cast<ProgressBar*>(WindowManager::getSingleton().getWindow("Demo7/Window2/Progbar1"))->setProgress(val);
    // set second bar's progress - this time the reverse of the first one
    static_cast<ProgressBar*>(WindowManager::getSingleton().getWindow("Demo7/Window2/Progbar2"))->setProgress(1.0f - val);
    // set the alpha on the window containing all the controls.
    WindowManager::getSingleton().getWindow("root_wnd")->setAlpha(val);

    // event was handled.
    return true;
}

bool Demo7Sample::handleRadio(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get the ID of the selected radio button
    CEGUI::uint id = static_cast<RadioButton*>(static_cast<const WindowEventArgs&>(e).window)->getSelectedButtonInGroup()->getID();
    // get the StaticImage window
    Window* img = WindowManager::getSingleton().getWindow("Demo7/Window2/Image1");

    // set an image into the StaticImage according to the ID of the selected radio button.
    switch (id)
    {
    case 0:
        img->setProperty("Image", "set:BackgroundImage image:full_image");
        break;

    case 1:
        img->setProperty("Image", "set:TaharezLook image:MouseArrow");
        break;

    default:
        img->setProperty("Image", "");
        break;
    }

    // event was handled
    return true;
}

bool Demo7Sample::handleCheck(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // show or hide the FrameWindow containing the multi-line editbox according to the state of the
    // checkbox widget
    WindowManager::getSingleton().getWindow("Demo7/Window3")->
        setVisible(static_cast<Checkbox*>(static_cast<const WindowEventArgs&>(e).window)->isSelected());

    // event was handled.
    return true;
}
