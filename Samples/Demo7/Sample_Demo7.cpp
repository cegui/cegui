/***********************************************************************
    filename:   Sample_Demo7.cpp
    created:    20/8/2005
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "Sample_Demo7.h"
#include "CEGUI/CEGUI.h"
#include "CEGuiBaseApplication.h"

#include <cstdlib>

int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    Demo7Sample app;
    return app.run();
}

//----------------------------------------------------------------------------//
// The following are related to the RenderEffect
//
// Note: This be the land of magic numbers and compound hacks upon hacks :-p
//       Any final version of this we might provide will likely be cleaned up
//       considerably.
//----------------------------------------------------------------------------//
const float MyEffect::tess_x = 8;
const float MyEffect::tess_y = 8;

//----------------------------------------------------------------------------//
MyEffect::MyEffect() :
    initialised(false),
    dragX(0), dragY(0),
    elasX(0), elasY(0)
{
}

//----------------------------------------------------------------------------//
int MyEffect::getPassCount() const
{
    return 1;
}

//----------------------------------------------------------------------------//
void MyEffect::performPreRenderFunctions(const int /*pass*/)
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
void MyEffect::performPostRenderFunctions()
{
    // nothing we need here
}

//----------------------------------------------------------------------------//
bool MyEffect::realiseGeometry(CEGUI::RenderingWindow& window,
                               CEGUI::GeometryBuffer& geometry)
{
    using namespace CEGUI;
    Texture& tex = window.getTextureTarget().getTexture();

    static const CEGUI::Colour c(1, 1, 1, 1);

    const float qw = window.getSize().d_width / tess_x;
    const float qh = window.getSize().d_height / tess_y;
    const float tcx = qw * tex.getTexelScaling().d_x;
    const float tcy =
        (window.getTextureTarget().isRenderingInverted() ? -qh : qh) *
            tex.getTexelScaling().d_y;

    for (int j = 0; j < tess_y; ++j)
    {
        for (int i = 0; i < tess_x; ++i)
        {
            int idx = (j * tess_x + i) * 6;

            float top_adj = dragX * ((1.0f / tess_x) * j);
            float bot_adj = dragX * ((1.0f / tess_x) * (j+1));
            top_adj = ((top_adj*top_adj) / 3) * (dragX < 0 ? -1 : 1);
            bot_adj = ((bot_adj*bot_adj) / 3) * (dragX < 0 ? -1 : 1);

            float lef_adj = dragY * ((1.0f / tess_y) * i);
            float rig_adj = dragY * ((1.0f / tess_y) * (i+1));
            lef_adj = ((lef_adj*lef_adj) / 3) * (dragY < 0 ? -1 : 1);
            rig_adj = ((rig_adj*rig_adj) / 3) * (dragY < 0 ? -1 : 1);

            // vertex 0
            vb[idx + 0].position   = Vector3f(i * qw - top_adj, j * qh - lef_adj, 0.0f);
            vb[idx + 0].colour_val = c;
            vb[idx + 0].tex_coords = Vector2f(i * tcx, j*tcy);

            // vertex 1
            vb[idx + 1].position   = Vector3f(i * qw - bot_adj, j * qh + qh - lef_adj, 0.0f);
            vb[idx + 1].colour_val = c;
            vb[idx + 1].tex_coords = Vector2f(i*tcx, j*tcy+tcy);

            // vertex 2
            vb[idx + 2].position   = Vector3f(i * qw + qw - bot_adj, j * qh + qh - rig_adj, 0.0f);
            vb[idx + 2].colour_val = c;
            vb[idx + 2].tex_coords = Vector2f(i*tcx+tcx, j*tcy+tcy);

            // vertex 3
            vb[idx + 3].position   = Vector3f(i * qw + qw - bot_adj, j * qh + qh - rig_adj, 0.0f);
            vb[idx + 3].colour_val = c;
            vb[idx + 3].tex_coords = Vector2f(i*tcx+tcx, j*tcy+tcy);

            // vertex 4
            vb[idx + 4].position   = Vector3f(i * qw + qw - top_adj, j * qh - rig_adj, 0.0f);
            vb[idx + 4].colour_val = c;
            vb[idx + 4].tex_coords = Vector2f(i*tcx+tcx, j*tcy);

            // vertex 5
            vb[idx + 5].position   = Vector3f(i * qw - top_adj, j * qh - lef_adj, 0.0f);
            vb[idx + 5].colour_val = c;
            vb[idx + 5].tex_coords = Vector2f(i * tcx, j*tcy);
        }
    }

    geometry.setActiveTexture(&tex);
    geometry.appendGeometry(vb, buffsize);

    // false, because we do not want the default geometry added!
    return false;
}

//----------------------------------------------------------------------------//
bool MyEffect::update(const float elapsed, CEGUI::RenderingWindow& window)
{
    using namespace CEGUI;
    
    // initialise ourself upon the first update call.
    if (!initialised)
    {
        initialised=true;
        lastX = window.getPosition().d_x;
        lastY = window.getPosition().d_y;
        return true;
    }

    const Vector2f pos(window.getPosition());

    //
    // Set up for X axis animation.
    //
    if (pos.d_x != lastX)
    {
        dragX += (pos.d_x - lastX) * 0.2;
        elasX = 0.05f;
        lastX = pos.d_x;

        if (dragX > 25)
            dragX = 25;
        else if (dragX < -25)
            dragX = -25;
    }

    //
    // Set up for y axis animation
    //
    if (pos.d_y != lastY)
    {
        dragY += (pos.d_y - lastY) * 0.2f;
        elasY = 0.05f;
        lastY = pos.d_y;

        if (dragY > 25)
            dragY = 25;
        else if (dragY < -25)
            dragY = -25;
    }

    //
    // Perform required animation steps
    //
    if ((dragX != 0) || (dragY != 0))
    {
        if (dragX < 0)
        {
            dragX += (elasX * 800 * elapsed);
            elasX += 0.075 * elapsed;
            if (dragX >0)
                dragX = 0;
        }
        else
        {
            dragX -= (elasX * 800 * elapsed);
            elasX += 0.075 * elapsed;
            if (dragX < 0)
                dragX = 0;
        }

        if (dragY < 0)
        {
            dragY += elasY * 800 * elapsed;
            elasY += 0.075 * elapsed;
            if (dragY >0)
                dragY = 0;
        }
        else
        {
            dragY -= elasY * 800 * elapsed;
            elasY += 0.075 * elapsed;
            if (dragY < 0)
                dragY = 0;
        }

        // note we just need system to redraw the geometry; we do not need a
        // full redraw of all window/widget content - which is unchanged.
        System::getSingleton().signalRedraw();
        return false;
    }

    return true;
}


//----------------------------------------------------------------------------//
// The following are for the main Demo7Sample class.
//----------------------------------------------------------------------------//

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool Demo7Sample::initialiseSample()
{
    using namespace CEGUI;

    // Register our effect with the system
    RenderEffectManager::getSingleton().addEffect<MyEffect>("WobblyWindow");

    // Now we make a Falagard mapping for a frame window that uses this effect.
    // We create a type "TaharezLook/WobblyFrameWindow", which is subsequently
    // referenced in the layout file loaded below.  Note that it wold be more
    // usual for this mapping to be specified in the scheme xml file, though
    // here we are doing in manually to save from needing either multiple
    // versions of the schemes or from having demo specific definitions in
    // the schemes.
    WindowFactoryManager::getSingleton().addFalagardWindowMapping(
        "TaharezLook/WobblyFrameWindow",    // type to create
        "CEGUI/FrameWindow",                // 'base' widget type
        "TaharezLook/FrameWindow",          // WidgetLook to use.
        "Falagard/FrameWindow",             // WindowRenderer to use.
        "WobblyWindow");                    // effect to use.

    // we will use of the WindowManager.
    WindowManager& winMgr = WindowManager::getSingleton();

    // load scheme and set up defaults
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    System::getSingleton().getDefaultGUIRoot().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

    // load an image to use as a background
    ImageManager::getSingleton().addFromImageFile("BackgroundImage", "GPN-2000-001437.png");

    // here we will use a StaticImage as the root, then we can use it to place a background image
    Window* background = winMgr.createWindow("TaharezLook/StaticImage", "background_wnd");
    // set position and size
    background->setPosition(UVector2(cegui_reldim(0), cegui_reldim( 0)));
    background->setSize(USize(cegui_reldim(1), cegui_reldim( 1)));
    // disable frame and standard background
    background->setProperty("FrameEnabled", "false");
    background->setProperty("BackgroundEnabled", "false");
    // set the background image
    background->setProperty("Image", "BackgroundImage");
    // install this as the root GUI sheet
    System::getSingleton().getDefaultGUIRoot().setRootWindow(background);

    // load the windows for Demo7 from the layout file.
    Window* sheet = winMgr.loadLayoutFromFile("Demo7Windows.layout");
    // attach this to the 'real' root
    background->addChild(sheet);
    // set-up the contents of the list boxes.
    createListContent(sheet);
    // initialise the event handling.
    initDemoEventWiring(sheet);

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
void Demo7Sample::createListContent(CEGUI::Window* root)
{
    using namespace CEGUI;

    WindowManager& winMgr = WindowManager::getSingleton();

    //
    // Combobox setup
    //
    Combobox* cbobox = static_cast<Combobox*>(root->getChild("Window2/Combobox"));
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

    //
    // Multi-Column List setup
    //
    MultiColumnList* mclbox = static_cast<MultiColumnList*>(root->getChild("Window2/MultiColumnList"));
    // Add some empty rows to the MCL
    mclbox->addRow();
    mclbox->addRow();
    mclbox->addRow();
    mclbox->addRow();
    mclbox->addRow();

    // Set first row item texts for the MCL
    mclbox->setItem(new MyListItem("Laggers World"), 0, 0);
    mclbox->setItem(new MyListItem("yourgame.some-server.com"), 1, 0);
    mclbox->setItem(new MyListItem("[colour='FFFF0000']1000ms"), 2, 0);

    // Set second row item texts for the MCL
    mclbox->setItem(new MyListItem("Super-Server"), 0, 1);
    mclbox->setItem(new MyListItem("whizzy.fakenames.net"), 1, 1);
    mclbox->setItem(new MyListItem("[colour='FF00FF00']8ms"), 2, 1);

    // Set third row item texts for the MCL
    mclbox->setItem(new MyListItem("Cray-Z-Eds"), 0, 2);
    mclbox->setItem(new MyListItem("crayzeds.notarealserver.co.uk"), 1, 2);
    mclbox->setItem(new MyListItem("[colour='FF00FF00']43ms"), 2, 2);

    // Set fourth row item texts for the MCL
    mclbox->setItem(new MyListItem("Fake IPs"), 0, 3);
    mclbox->setItem(new MyListItem("123.320.42.242"), 1, 3);
    mclbox->setItem(new MyListItem("[colour='FFFFFF00']63ms"), 2, 3);

    // Set fifth row item texts for the MCL
    mclbox->setItem(new MyListItem("Yet Another Game Server"), 0, 4);
    mclbox->setItem(new MyListItem("abc.abcdefghijklmn.org"), 1, 4);
    mclbox->setItem(new MyListItem("[colour='FFFF6600']284ms"), 2, 4);
}


/*************************************************************************
    Perform required event hook-ups for this demo.
*************************************************************************/
void Demo7Sample::initDemoEventWiring(CEGUI::Window* root)
{
    using namespace CEGUI;

    // Subscribe handler that quits the application
    root->getChild("Window1/Quit")->
        subscribeEvent(PushButton::EventClicked, Event::Subscriber(&Demo7Sample::handleQuit, this));

    // Subscribe handler that processes changes to the slider position.
    root->getChild("Window1/Slider1")->
        subscribeEvent(Slider::EventValueChanged, Event::Subscriber(&Demo7Sample::handleSlider, this));

    // Subscribe handler that processes changes to the checkbox selection state.
    root->getChild("Window1/Checkbox")->
        subscribeEvent(Checkbox::EventCheckStateChanged, Event::Subscriber(&Demo7Sample::handleCheck, this));

    // Subscribe handler that processes changes to the radio button selection state.
    root->getChild("Window1/Radio1")->
        subscribeEvent(RadioButton::EventSelectStateChanged, Event::Subscriber(&Demo7Sample::handleRadio, this));

    // Subscribe handler that processes changes to the radio button selection state.
    root->getChild("Window1/Radio2")->
        subscribeEvent(RadioButton::EventSelectStateChanged, Event::Subscriber(&Demo7Sample::handleRadio, this));

    // Subscribe handler that processes changes to the radio button selection state.
    root->getChild("Window1/Radio3")->
        subscribeEvent(RadioButton::EventSelectStateChanged, Event::Subscriber(&Demo7Sample::handleRadio, this));
}

bool Demo7Sample::handleQuit(const CEGUI::EventArgs&)
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
    static_cast<ProgressBar*>(static_cast<const WindowEventArgs&>(e).window->getRootWindow()->getChild("root/Window2/Progbar1"))->setProgress(val);
    // set second bar's progress - this time the reverse of the first one
    static_cast<ProgressBar*>(static_cast<const WindowEventArgs&>(e).window->getRootWindow()->getChild("root/Window2/Progbar2"))->setProgress(1.0f - val);
    // set the alpha on the window containing all the controls.
    static_cast<const WindowEventArgs&>(e).window->getRootWindow()->getChild("root")->setAlpha(val);

    // event was handled.
    return true;
}

bool Demo7Sample::handleRadio(const CEGUI::EventArgs& e)
{
    using namespace CEGUI;

    // get the ID of the selected radio button
    CEGUI::uint id = static_cast<RadioButton*>(static_cast<const WindowEventArgs&>(e).window)->getSelectedButtonInGroup()->getID();
    // get the StaticImage window
    Window* img = static_cast<const WindowEventArgs&>(e).window->getRootWindow()->getChild("root/Window2/Image1");

    // set an image into the StaticImage according to the ID of the selected radio button.
    switch (id)
    {
    case 0:
        img->setProperty("Image", "BackgroundImage");
        break;

    case 1:
        img->setProperty("Image", "TaharezLook/MouseArrow");
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
    static_cast<const WindowEventArgs&>(e).window->getRootWindow()->getChild("root/Window3")->
        setVisible(static_cast<Checkbox*>(static_cast<const WindowEventArgs&>(e).window)->isSelected());

    // event was handled.
    return true;
}
