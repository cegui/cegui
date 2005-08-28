/************************************************************************
    filename:   Sample_FalagardDemo1.cpp
    created:    19/6/2005
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
#include "Sample_FalagardDemo1.h"
#include "CEGUI.h"
#include "falagard/CEGUIFalWidgetLookManager.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
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
    FalagardDemo1Sample app;
    return app.run();
}

//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    FalagardDemo1Sample class

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool FalagardDemo1Sample::initialiseSample()
{
    using namespace CEGUI;

    // Get window manager which we wil use for a few jobs here.
    WindowManager& winMgr = WindowManager::getSingleton();
    // Load the scheme to initialse the VanillaSkin which we use in this sample
    SchemeManager::getSingleton().loadScheme("../datafiles/schemes/VanillaSkin.scheme");
    // set default mouse image
    System::getSingleton().setDefaultMouseCursor("Vanilla-Images", "MouseArrow");

    // load an image to use as a background
    ImagesetManager::getSingleton().createImagesetFromImageFile("BackgroundImage", "../datafiles/imagesets/GPN-2000-001437.tga");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    StaticImage* background = static_cast<StaticImage*>(winMgr.createWindow("Vanilla/StaticImage"));
    // set area rectangle
    background->setRect(Relative, Rect(0, 0, 1, 1));
    // disable frame and standard background
    background->setFrameEnabled(false);
    background->setBackgroundEnabled(false);
    // set the background image
    background->setImage("BackgroundImage", "full_image");
    // install this as the root GUI sheet
    System::getSingleton().setGUISheet(background);

    FontManager::getSingleton().createFont("../datafiles/fonts/Iconified-12.font");

    // load some demo windows and attach to the background 'root'
    background->addChildWindow(winMgr.loadWindowLayout("../datafiles/layouts/VanillaWindows.layout"));

    // create an instance of the console class.
    d_console = new DemoConsole("Demo");

    // listen for key presses on the root window.
    background->subscribeEvent(Window::EventKeyDown, Event::Subscriber(&FalagardDemo1Sample::handleRootKeyDown, this));

    // activate the background window
    background->activate();

    // success!
    return true;
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void FalagardDemo1Sample::cleanupSample()
{
    delete d_console;
}


bool FalagardDemo1Sample::handleRootKeyDown(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    const KeyEventArgs& keyArgs = static_cast<const KeyEventArgs&>(args);

    switch (keyArgs.scancode)
    {
    case Key::F12:
        d_console->toggleVisibility();
        break;

    default:
        return false;
    }

    return true;
}


//////////////////////////////////////////////////////////////////////////
/*************************************************************************

    DemoConsole class

*************************************************************************/
//////////////////////////////////////////////////////////////////////////
// these must match the IDs assigned in the layout
const unsigned int DemoConsole::SubmitButtonID = 1;
const unsigned int DemoConsole::EntryBoxID     = 2;
const unsigned int DemoConsole::HistoryID      = 3;


DemoConsole::DemoConsole(const CEGUI::String& id_name, CEGUI::Window* parent) :
    d_root(CEGUI::WindowManager::getSingleton().loadWindowLayout("../datafiles/layouts/VanillaConsole.layout", id_name)),
    d_historyPos(0)
{
    using namespace CEGUI;

    // we will destroy the console box windows ourselves
    d_root->setDestroyedByParent(false);

    // Do events wire-up
    d_root->subscribeEvent(Window::EventKeyDown, Event::Subscriber(&DemoConsole::handleKeyDown, this));

    d_root->getChild(SubmitButtonID)->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&DemoConsole::handleSubmit, this));

    d_root->getChild(EntryBoxID)->
        subscribeEvent(Editbox::EventTextAccepted, Event::Subscriber(&DemoConsole::handleSubmit, this));

    // decide where to attach the console main window
    parent = parent ? parent : CEGUI::System::getSingleton().getGUISheet();

    // attach this window if parent is valid
    if (parent)
        parent->addChildWindow(d_root);
}

DemoConsole::~DemoConsole()
{
    // destroy the windows that we loaded earlier
    CEGUI::WindowManager::getSingleton().destroyWindow(d_root);
}

void DemoConsole::toggleVisibility()
{
    d_root->isVisible(true) ? d_root->hide() : d_root->show();
}

bool DemoConsole::isVisible() const
{
    return d_root->isVisible();
}

bool DemoConsole::handleSubmit(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get the text entry editbox
    Editbox* editbox = static_cast<Editbox*>(d_root->getChild(EntryBoxID));
    // get text out of the editbox
    String edit_text(editbox->getText());
    // if the string is not empty
    if (!edit_text.empty())
    {
        // add this entry to the command history buffer
        d_history.push_back(edit_text);
        // reset history position
        d_historyPos = d_history.size();
        // append newline to this entry
        edit_text += '\n';
        // get history window
        MultiLineEditbox* history = static_cast<MultiLineEditbox*>(d_root->getChild(HistoryID));
        // append new text to history output
        history->setText(history->getText() + edit_text);
        // scroll to bottom of history output
        history->setCaratIndex(static_cast<size_t>(-1));
        // erase text in text entry box.
        editbox->setText("");
    }

    // re-activate the text entry box
    editbox->activate();

    return true;
}

bool DemoConsole::handleKeyDown(const CEGUI::EventArgs& args)
{
    using namespace CEGUI;

    // get the text entry editbox
    Editbox* editbox = static_cast<Editbox*>(d_root->getChild(EntryBoxID));

    switch (static_cast<const KeyEventArgs&>(args).scancode)
    {
    case Key::ArrowUp:
        d_historyPos = ceguimax(d_historyPos - 1, -1);
        if (d_historyPos >= 0)
        {
            editbox->setText(d_history[d_historyPos]);
            editbox->setCaratIndex(static_cast<size_t>(-1));
        }
        else
        {
            editbox->setText("");
        }

        editbox->activate();
        break;

    case Key::ArrowDown:
        d_historyPos = ceguimin(d_historyPos + 1, static_cast<int>(d_history.size()));
        if (d_historyPos < static_cast<int>(d_history.size()))
        {
            editbox->setText(d_history[d_historyPos]);
            editbox->setCaratIndex(static_cast<size_t>(-1));
        }
        else
        {
            editbox->setText("");
        }

        editbox->activate();
        break;

    default:
        return false;
    }

    return true;
}
