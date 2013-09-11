/***********************************************************************
    filename:   SVGSample.cpp
    created:    11th August 2013
    author:     Lukas Meindl
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
#include "SVGSample.h"

#include "CEGUI/SchemeManager.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/widgets/DefaultWindow.h"
#include "CEGUI/ImageManager.h"

#include <iostream>

using namespace CEGUI;

/*************************************************************************
    Constructor.
*************************************************************************/
SVGSample::SVGSample()
{
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool SVGSample::initialise(CEGUI::GUIContext* guiContext)
{
    d_usedFiles = CEGUI::String(__FILE__);

    //CEGUI setup
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    SchemeManager::getSingleton().createFromFile("Generic.scheme");
    guiContext->getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");
    WindowManager& winMgr = WindowManager::getSingleton();

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");

    // Create a DefaultWindow called 'Root'.
    d_root = static_cast<DefaultWindow*>(winMgr.createWindow("DefaultWindow", "Root"));

    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // Set the root window as root of our GUI Context
    guiContext->setRootWindow(d_root);

    // Load our SVG-based imageset via the ImageManager
    CEGUI::ImageManager& imageManager = CEGUI::ImageManager::getSingleton();
    ImageManager::getSingleton().loadImageset("SVGSampleImageset.imageset");

    Window* svgSampleFrameWindow = winMgr.createWindow("WindowsLook/FrameWindow", "SvgSampleFrameWindow");
    svgSampleFrameWindow->setSize(CEGUI::USize(cegui_absdim(300.0f), cegui_absdim(300.0f)));
    d_root->addChild(svgSampleFrameWindow);

    Window* svgImageWindow1 = winMgr.createWindow("Generic/Image");
    svgImageWindow1->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    svgSampleFrameWindow->addChild(svgImageWindow1);
    svgImageWindow1->setProperty("Image", "SVGSampleImageset/SVGTestImage1");

    CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();

    // return true so that the samples framework knows that initialisation was a
    // success, and that it should now run the sample.
    return true;
}


void SVGSample::update(float timeSinceLastUpdate)
{
}

void SVGSample::deinitialise()
{
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static SVGSample sample;
    return sample;
}