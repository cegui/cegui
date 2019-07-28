/***********************************************************************
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
#include "SVG.h"

#include "CEGUI/SchemeManager.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/widgets/DefaultWindow.h"
#include "CEGUI/widgets/PushButton.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/svg/SVGImage.h"

#include <iostream>

using namespace CEGUI;

/*************************************************************************
    Constructor.
*************************************************************************/
SVGSample::SVGSample() :
    Sample(-10)
{
    Sample::d_name = "SVGSample";
    Sample::d_credits = "Lukas \"Ident\" Meindl";
    Sample::d_description = "";
    Sample::d_summary = "";
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
    guiContext->getCursor().setDefaultImage("WindowsLook/MouseArrow");
    WindowManager& winMgr = WindowManager::getSingleton();

    // load font and setup default if not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();

    // Create a DefaultWindow called 'Root'.
    d_root = static_cast<DefaultWindow*>(winMgr.createWindow("DefaultWindow", "Root"));

    // Set default font for the gui context
    guiContext->setDefaultFont(defaultFont);

    // Set the root window as root of our GUI Context
    guiContext->setRootWindow(d_root);

    // Load our SVG-based Imageset via the ImageManager
    ImageManager::getSingleton().loadImageset("SVGSampleImageset.imageset");

    // We get our loaded sample SVGImage and save it to a variable
    d_svgSampleImage = static_cast<CEGUI::SVGImage*>( &ImageManager::getSingleton().get("SVGSampleImageset/SVGTestImage1") );

    // We create a sizable and movable FrameWindow that will contain our Image window
    Window* svgSampleFrameWindow = winMgr.createWindow("WindowsLook/FrameWindow", "SvgSampleFrameWindow");

    // This is very important. Since our FrameWindow contains our SVGImage, we want the FrameWindow to have
    // a stencil buffer attached. This allows to render SVGs properly, which have overlapping parts in their polygons.
    // Without it, we cannot ensure that areas that have even-degree polygon overlaps (for example two time overlaps) will
    // be erased properly. Almost all vector graphics formats and programs erase such areas. Only odd-degree overlaps (1 + 2*n)
    // are rendered.
    // The SVG Image renders its polygons directly onto the FrameBuffer (or "RenderSurface", however you wanna call it) it is contained.
    // The SVG Image Window could also act as an AutoRenderingSurface (by switching it on) and lead to the same result by having its Stencil enabled.
    // If you just want the isolated SVG Image to render correctly you can do this. In our case this is not necessary - we already use a FrameWindow with
    // an AutoRenderingSurface so we can just use this one and the SVG Image Window will render its SVG Image into this FrameBuffer with the stencil on.
    // If you want to see how it looks without Stencil, just deactivate it and look at the result.
    svgSampleFrameWindow->setAutoRenderingSurfaceStencilEnabled(true);
    svgSampleFrameWindow->setUsingAutoRenderingSurface(true);

    svgSampleFrameWindow->setPosition(CEGUI::UVector2(cegui_absdim(50.0f), cegui_absdim(50.0f)));
    svgSampleFrameWindow->setSize(CEGUI::USize(cegui_absdim(300.0f), cegui_absdim(300.0f)));
    d_root->addChild(svgSampleFrameWindow);
    
    // We create a window that displays images and apply our SVGImage pointer to its "Image" property. Our sample SVGImage will be displayed by the window.
    d_svgImageWindow = winMgr.createWindow("Generic/Image");
    d_svgImageWindow->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    svgSampleFrameWindow->addChild(d_svgImageWindow);
    d_svgImageWindow->setProperty<CEGUI::Image*>("Image", d_svgSampleImage);

    // We create a button and subscribe to its click events
    Window* svgAntialiasingButton = winMgr.createWindow("WindowsLook/Button");
    svgAntialiasingButton->setSize(CEGUI::USize(cegui_reldim(0.2f), cegui_reldim(0.035f)));
    svgAntialiasingButton->setHorizontalAlignment(HorizontalAlignment::Centre);
    svgAntialiasingButton->setPosition(CEGUI::UVector2(cegui_absdim(0.0f), cegui_reldim(0.03f)));
    svgAntialiasingButton->setText("Switch anti-aliasing mode");
    svgAntialiasingButton->subscribeEvent(PushButton::EventClicked, Event::Subscriber(&SVGSample::handleAntialiasingButtonClicked, this));
    d_root->addChild(svgAntialiasingButton);

    // return true so that the samples framework knows that initialisation was a
    // success, and that it should now run the sample.
    return true;
}


bool SVGSample::handleAntialiasingButtonClicked(const CEGUI::EventArgs&)
{
    d_svgSampleImage->setUseGeometryAntialiasing(!d_svgSampleImage->getUsesGeometryAntialiasing());
    d_svgImageWindow->invalidate();

    return true;
}


void SVGSample::update(float /*timeSinceLastUpdate*/)
{
}

void SVGSample::deinitialise()
{
}
