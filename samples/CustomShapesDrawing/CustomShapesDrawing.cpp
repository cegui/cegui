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
#include "CEGUI/BitmapImage.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/svg/SVGDataManager.h"
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/svg/SVGData.h"
#include "CEGUI/svg/SVGBasicShape.h"

#include "glm/glm.hpp"

#include <iostream>

using namespace CEGUI;
using namespace glm;

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
    d_usedFiles = CEGUI::String(__FILE__);

    //CEGUI setup
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    SchemeManager::getSingleton().createFromFile("Generic.scheme");
    guiContext->getMouseCursor().setDefaultImage("WindowsLook/MouseArrow");
    WindowManager& winMgr = WindowManager::getSingleton();


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

    // Create an SVGImage using the ImageManager
    CEGUI::ImageManager& imageManager = CEGUI::ImageManager::getSingleton();
    SVGImage& fpsSVGImage = static_cast<SVGImage&>(imageManager.create(CEGUI::String("SVGImage"), "FPSGraphSVG"));
    // Create an SVGData object
    CEGUI::SVGDataManager& svgDataManager = CEGUI::SVGDataManager::getSingleton();
    SVGData& fpsSVGData = svgDataManager.create(CEGUI::String("FPSGraphCustomShape"));
    //Fill in line points
    SVGPolyline* polyLine = new SVGPolyline(); //TODO maybe create a creation function for this
    polyLine->d_points.push_back(glm::vec2(20.0f, 20.0f));
    polyLine->d_points.push_back(glm::vec2(40.0f, 20.0f));
    polyLine->d_points.push_back(glm::vec2(20.0f, 40.0f));
    polyLine->d_points.push_back(glm::vec2(60.0f, 60.0f));
    polyLine->d_points.push_back(glm::vec2(110.0f, 60.0f));
    fpsSVGData.addShape(polyLine);
    // ...and set the pointer to the SVGData for the SVGImage
    fpsSVGImage.setSVGData(&fpsSVGData);
 
    Window* fpsSVGFrameWindow = winMgr.createWindow("WindowsLook/FrameWindow");
    d_root->addChild(fpsSVGFrameWindow);
    Window* fpsSVGImageWindow = winMgr.createWindow("Generic/Image");
    fpsSVGFrameWindow->addChild(fpsSVGImageWindow);
    fpsSVGImageWindow->setProperty("Image", "FPSGraphSVG");


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

    std::vector<glm::vec2> linePositions;
    
    for(unsigned int i = 0; i < d_FPSGraphSamplesCount; ++i)
    {
        float currentOffset = i * sampleDisplaySpacing;

        float scale = static_cast<float>(d_lastFPSValues.at(i)) / d_FPSMaxGraphValue;
        float pointHeight = maxHeight - scale * maxHeight;

        glm::vec2 currentPosition(currentOffset, pointHeight);
        linePositions.push_back(currentPosition);
    }

    drawLineStrip(linePositions, lineWidth, lineColour);
}

void CustomShapesDrawing::drawLineStrip(std::vector<glm::vec2> &linePositions, const float lineWidth, const CEGUI::Colour lineColour)
{
    unsigned int size = linePositions.size();
    for (unsigned int j = 1; j < size; ++j)
    {
        const glm::vec2& prevPos = linePositions.at(j - 1);
        const glm::vec2& currentPos = linePositions.at(j);

        // Normalize and tilt the 2D direction vector by 90° to get the vector pointing in the offset direction
        glm::vec2 offsetVector = currentPos - prevPos;
        offsetVector = glm::normalize(offsetVector);
        offsetVector = glm::vec2(offsetVector.y, -offsetVector.x) * 0.5f;

        CEGUI::ColouredVertex linePositionVertex;
        glm::vec2 vertexPosition;
        linePositionVertex.colour_val = lineColour;

        linePositionVertex.position = glm::vec3(prevPos - offsetVector, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos - offsetVector, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos + offsetVector, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(currentPos + offsetVector, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(prevPos - offsetVector, 0.0f);
        d_FPSGraphGeometry->appendVertex(linePositionVertex);

        linePositionVertex.position = glm::vec3(prevPos + offsetVector, 0.0f);
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