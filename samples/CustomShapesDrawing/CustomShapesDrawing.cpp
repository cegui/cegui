/***********************************************************************
    filename:   CustomShapesDrawing.cpp
    created:    16/7/2013
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
#include "CustomShapesDrawing.h"

#include "CEGUI/SchemeManager.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/WindowManager.h"
#include "CEGUI/FontManager.h"
#include "CEGUI/Window.h"
#include "CEGUI/widgets/DefaultWindow.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/BasicImage.h"
#include "CEGUI/ImageManager.h"



#include <iostream>


/*************************************************************************
    Constructor.
*************************************************************************/
CustomShapesDrawing::CustomShapesDrawing()
    : d_FPSMaxGraphValue(1)
    , d_FPSGraphSamplesCount(30)
{
    // Initialising the FPS values
    for(unsigned int i = 0; i < d_FPSGraphSamplesCount; ++i)
        d_lastFPSValues.push_front(0);
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool CustomShapesDrawing::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

     d_usedFiles = CEGUI::String(__FILE__);

    // CEGUI relies on various systems being set-up, so this is what we do
    // here first.
    //
    // The first thing to do is load a CEGUI 'scheme' this is basically a file
    // that groups all the required resources and definitions for a particular
    // skin so they can be loaded / initialised easily
    //
    // So, we use the SchemeManager singleton to load in a scheme that loads the
    // imagery and registers widgets for the TaharezLook skin.  This scheme also
    // loads in a font that gets used as the system default.
    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");

    // The next thing we do is to set a default mouse cursor image.  This is
    // not strictly essential, although it is nice to always have a visible
    // cursor if a window or widget does not explicitly set one of its own.
    //
    // The TaharezLook Imageset contains an Image named "MouseArrow" which is
    // the ideal thing to have as a defult mouse cursor image.
    guiContext->getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

    // Now the system is initialised, we can actually create some UI elements, for
    // this first example, a full-screen 'root' window is set as the active GUI
    // sheet, and then a simple frame window will be created and attached to it.

    // All windows and widgets are created via the WindowManager singleton.
    WindowManager& winMgr = WindowManager::getSingleton();

    // Here we create a "DefaultWindow".  This is a native type, that is, it does
    // not have to be loaded via a scheme, it is always available.  One common use
    // for the DefaultWindow is as a generic container for other windows.  Its
    // size defaults to 1.0f x 1.0f using the Relative metrics mode, which means
    // when it is set as the root GUI sheet window, it will cover the entire display.
    // The DefaultWindow does not perform any rendering of its own, so is invisible.
    //
    // Create a DefaultWindow called 'Root'.
    d_root = (DefaultWindow*)winMgr.createWindow("DefaultWindow", "Root");

    // load font and setup default if not loaded via scheme
    Font& defaultFont = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(&defaultFont);

    // Set the root window as root of our GUI Context
    guiContext->setRootWindow(d_root);

    CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();

    // GeometryBuffer used for drawing in this demo
    d_FPSGraphGeometry = &renderer->createGeometryBufferColoured(renderer->createRenderMaterial(DS_SOLID));
    // Size and position have to be set
    positionFPSGraphGeometry();
    updateFPSGraphGeometry();


    // clearing this queue actually makes sure it's created(!)
    guiContext->clearGeometry(CEGUI::RQ_OVERLAY);

    // subscribe handler to render overlay items
    guiContext->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
            CEGUI::Event::Subscriber(&CustomShapesDrawing::drawFPSGraphOverlay,
                                     this));

    // return true so that the samples framework knows that initialisation was a
    // success, and that it should now run the sample.
    return true;
}

/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void CustomShapesDrawing::positionFPSGraphGeometry()
{
    CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();
    const CEGUI::Rectf scrn(CEGUI::Vector2f(0, 0), renderer->getDisplaySize());

    d_FPSGraphGeometry->setClippingRegion(scrn);
    d_FPSGraphGeometry->setTranslation(
        CEGUI::Vector3f(250.0f, 250.f, 0.0f));
}

/*************************************************************************
    Triggers the drawing of our FPS graph after everything else was rendered
*************************************************************************/
bool CustomShapesDrawing::drawFPSGraphOverlay(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RQ_OVERLAY)
        return false;

    // draw FPS value
    d_FPSGraphGeometry->draw();

    return true;
}

/*************************************************************************
    Update the FPS graph geometry when necessary
*************************************************************************/
void CustomShapesDrawing::update(float timeSinceLastUpdate)
{
    updateFPS(timeSinceLastUpdate);
    positionFPSGraphGeometry();
}

void CustomShapesDrawing::updateFPS(const float elapsed)
{
    // another frame
    ++d_FPSFrames;

    // Add FPS count if a second has passed
    if ((d_FPSElapsed += elapsed) >= 1.0f)
    {
        d_FPSMaxGraphValue = std::max(d_FPSMaxGraphValue, d_FPSFrames);
        d_FPSGraphGeometry->reset();

        d_lastFPSValues.pop_front();
        d_lastFPSValues.push_back(d_FPSFrames);
        updateFPSGraphGeometry();

        // reset counter state
        d_FPSFrames = 0;
        float modValue = 1.f;
        d_FPSElapsed = std::modf(d_FPSElapsed, &modValue);
    }
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void CustomShapesDrawing::deinitialise()
{
}

/*************************************************************************
    Update the geometry used for the FPS graph
*************************************************************************/
void CustomShapesDrawing::updateFPSGraphGeometry()
{
    static const float maxHeight = 100.0f;
    static const float sampleDisplaySpacing = 7.0f;
    static const float lineWidth = 1.f;
    static const CEGUI::Colour lineColour = CEGUI::Colour(0.0f, 1.0f, 0.0f);

    d_FPSGraphGeometry->reset();

    std::vector<CEGUI::Vector3f> linePositions;
    
    for(unsigned int i = 0; i < d_FPSGraphSamplesCount; ++i)
    {
        float currentOffset = i * sampleDisplaySpacing;

        float scale = static_cast<float>(d_lastFPSValues.at(i)) / d_FPSMaxGraphValue;
        float pointHeight = maxHeight - scale * maxHeight;

        CEGUI::Vector3f currentPosition(currentOffset, pointHeight, 0.0f);
        linePositions.push_back(currentPosition);
    }

    drawLineStrip(linePositions, lineWidth, lineColour);
}

void CustomShapesDrawing::drawLineStrip(std::vector<CEGUI::Vector3<float> > &linePositions, const float lineWidth, const CEGUI::Colour lineColour)
{
    unsigned int size = linePositions.size();
    for (unsigned int j = 1; j < size; ++j)
    {
        const CEGUI::Vector3f& previousPos = linePositions.at(j - 1);
        const CEGUI::Vector3f& currentPos = linePositions.at(j);

        const float& x1 = previousPos.d_x;
        const float& y1 = previousPos.d_y;
        const float& x2 = currentPos.d_x;
        const float& y2 = currentPos.d_y;

        //Todo: Replace trigonometry  with 2D vector flipping calculations to save speed
        float angle = std::atan2(y2 - y1, x2 - x1);
        float xLineOffset = lineWidth / 2.0f * sin(angle);
        float yLineOffset = lineWidth / 2.0f * cos(angle);

        CEGUI::ColouredVertex linePositionVertex;
        linePositionVertex.colour_val = lineColour;

        linePositionVertex.position = CEGUI::Vector3f(x1 + xLineOffset, y1 - yLineOffset, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = CEGUI::Vector3f(x2 + xLineOffset, y2 - yLineOffset, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = CEGUI::Vector3f(x2 - xLineOffset, y2 + yLineOffset, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = CEGUI::Vector3f(x2 - xLineOffset, y2 + yLineOffset, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = CEGUI::Vector3f(x1 - xLineOffset, y1 + yLineOffset, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = CEGUI::Vector3f(x1 + xLineOffset, y1 - yLineOffset, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);
    }
}

/*************************************************************************
    Define the module function that returns an instance of the sample
*************************************************************************/
extern "C" SAMPLE_EXPORT Sample& getSampleInstance()
{
    static CustomShapesDrawing sample;
    return sample;
}