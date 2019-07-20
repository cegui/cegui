/***********************************************************************
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
#include "CEGUI/widgets/ToggleButton.h"
#include "CEGUI/GeometryBuffer.h"
#include "CEGUI/Vertex.h"
#include "CEGUI/ImageManager.h"
#include "CEGUI/svg/SVGDataManager.h"
#include "CEGUI/svg/SVGImage.h"
#include "CEGUI/svg/SVGData.h"
#include "CEGUI/svg/SVGBasicShape.h"

#include <glm/glm.hpp>

#include <iostream>
#include <ctime>
#include <stddef.h>

using namespace CEGUI;
using namespace glm;

/*************************************************************************
    Constructor.
*************************************************************************/
CustomShapesDrawingSample::CustomShapesDrawingSample() :
    d_root(nullptr),
    d_customSVGImageFrameWindow(nullptr),
    d_customPolyline(nullptr),
    d_FPSGraphGeometryBuffer(nullptr),
    d_FPSGraphSamplesCount(30),
    d_FPSFrames(),
    d_FPSElapsed(0.0f),
    d_FPSMaxGraphValue(1),
    d_useRealFPS(false),
    d_customSVGImageWidth(300.0f),
    d_customSVGImageHeight(100.0f),
    d_customGeometryGraphWidth(300.0f),
    d_customGeometryGraphHeight(100.0f),
    d_lastFPSLabel(nullptr),
    d_customGeometryFPSLabel1(nullptr),
    d_customGeometryFPSLabel2(nullptr),
    d_customGeometryFPSLabel3(nullptr),
    d_customSVGImageFPSLabel1(nullptr),
    d_customSVGImageFPSLabel2(nullptr),
    d_customSVGImageFPSLabel3(nullptr),
    d_customSVGImage(nullptr),
    d_customSVGData(nullptr)
{
    Sample::d_name = "CustomShapesDrawingSample";
    Sample::d_credits = "Lukas \"Ident\" Meindl";
    Sample::d_description =
        "In CEGUI the GeometryBuffer can be used to render vertices directly on the screen. However, this comes with limitations, "
        "since CEGUI is not a Rendering Engine.For example only textured and coloured vertices are available out of the box.The vertices for all lines and "
        "the quad of the background of the graph had to be calculated with extra code.The other possibility to render a graph, as display in this sample, "
        "is by using an SVGImage.Each SVGImage points to a(possibly shared) SVGData object that contains its data.This data can be modified by adding shapes based "
        "on the SVG BasicShapes.The vertices do not have to be calculated in this case and the rendering specifics like colours and line widths can be specified "
        "more conveniently.Additionally, with SVGImages it is possible to switch on anti - aliasing, which is implemented by adding additional blending geometry to all shapes. "
        "Also, it is easily possible to add the SVGImage to a window, by referring to it via a property, which is not possible with the GeometryBuffer method.This is "
        "displayed in the sample with a WindowsLook / FrameWindow that contains an Generic / Image window.The FrameWindow is sizeable and will also scale its child window when "
        "sized.The Generic / Image window has an 'Image' property that is pointing to our SVGImage.When the FrameWindow is sized it can thus be seen that the image is "
        "being scaled too.";
    Sample::d_summary =
        "This sample displays rendering possibilities provided with CEGUI's GeometryBuffer and SVGImage/SVGData class. "
        "The goal is to render an FPS graph that displays the last recorded FPS values.SVGImage provides optional anti - aliasing and the possibility to "
        "use the Image as part of a window.";

    // Initialising the FPS values
    for(unsigned int i = 0; i < d_FPSGraphSamplesCount; ++i)
        d_lastFPSValues.push_front(0);
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool CustomShapesDrawingSample::initialise(CEGUI::GUIContext* guiContext)
{
    d_usedFiles = CEGUI::String(__FILE__);

    // CEGUI setup
    SchemeManager::getSingleton().createFromFile("WindowsLook.scheme");
    SchemeManager::getSingleton().createFromFile("Generic.scheme");
    guiContext->getCursor().setDefaultImage("WindowsLook/MouseArrow");
    WindowManager& winMgr = WindowManager::getSingleton();

    // Create a Generic/image called 'Root' with a white image as Image property.
    d_root = static_cast<DefaultWindow*>(winMgr.createWindow("Generic/Image", "Root"));
    d_root->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));
    d_root->setProperty("Image", "WindowsLook/Background");

    // load font and setup default if not loaded via scheme
    FontManager::FontList loadedFonts = FontManager::getSingleton().createFromFile("DejaVuSans-12.font");
    Font* defaultFont = loadedFonts.empty() ? 0 : loadedFonts.front();

    FontManager::getSingleton().createFromFile("DejaVuSans-10.font");
    // Set default font for the gui context
    guiContext->setDefaultFont(defaultFont);

    // Set the root window as root of our GUI Context
    guiContext->setRootWindow(d_root);

    // Initialise random-seed
    srand(static_cast<unsigned int>(time(nullptr)));

    // We create a checkbox to switch between showing randomly generated FPS and real FPS
    createCheckboxShowRealFPS();

    // Create a label displaying the last FPS value
    createLastFPSLabel();

    // Create a label with description of the Sample
    createDescriptionLabel();

    /* We will render custom geometry in two different ways. The first uses an SVGImage with attached customly defined
    SVGData. The SVGImage will be part of a window inside a Framewindow which can be moved around. The second directly
    modifies a Geometrybuffer to render user-defined vertices directly on the screen. */

    /* Our first method uses the SVGImage class of CEGUI. The SVGImage references an SVGData object that we will create.
    This SVGData object commonly contains what is parsed from an SVG file. In our case we will manually create SVG BasicShape
    objects, such as a polyline, which connects several points with lines, and add them to the SVGData. The SVGImage can be used
    in the same way as a regular raster graphics Image (BitmapImage) in CEGUI and can therefore be set as a property of a window
    that will then render the Image automatically, with appropriate scaling etc.
    */
    setupCustomSVGImage();
    // We create a drag- and sizable FrameWindow and add a child-window to it that will render our SVGImage
    createCustomSVGImageWindows();
    // We add labels to our graph using windows
    createCustomSVGImageFPSLabels();


    /* Our second method directly modifies a GeometryBuffer of CEGUI. This has the disadvantage that we will have to calculate all
    vertex positions of all triangles forming the geometry ourselves before filling them in. Compared to using SVGData and creating
    and modifying SVG BasicShapes this can be inconvenient. Additionally, we cannot make our geometry be used in connection with a window, so that it could be moved around etc. wever this can be of great
    use for overlays or backgrounds that use custom geometry that is calculated by the user. */
    setupCustomGeometryGraph(guiContext);
    // We add labels to our graph using windows
    createCustomGeometryFPSLabels();


    // Subscribe handler to reposition our labels when the window size is changed
    CEGUI::System::getSingleton().subscribeEvent(
        CEGUI::System::EventDisplaySizeChanged,
        CEGUI::Event::Subscriber(&CustomShapesDrawingSample::handleDisplaySizeChange,
        this));

    // return true so that the samples framework knows that initialisation was a
    // success, and that it should now run the sample.
    return true;
}

/*************************************************************************
    Positions the GeometryBuffer based graph
*************************************************************************/
void CustomShapesDrawingSample::positionCustomGeometryFPSGraph()
{
    CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();
    const CEGUI::Rectf scrn(glm::vec2(0, 0), renderer->getDisplaySize());

    d_FPSGraphGeometryBuffer->setClippingRegion(scrn);
    d_FPSGraphGeometryBuffer->setTranslation(glm::vec3(250.0f, 250.0f, 0.0f));
}

/*************************************************************************
    Triggers the drawing of our FPS graph after everything else was rendered
*************************************************************************/
bool CustomShapesDrawingSample::drawFPSGraphOverlay(const CEGUI::EventArgs& args)
{
    if (static_cast<const CEGUI::RenderQueueEventArgs&>(args).queueID != CEGUI::RenderQueueID::Overlay)
        return false;

    // draw FPS value
    d_FPSGraphGeometryBuffer->draw();

    return true;
}

/*************************************************************************
    Update the FPS graph geometry when necessary
*************************************************************************/
void CustomShapesDrawingSample::update(float timeSinceLastUpdate)
{
    updateFPS(timeSinceLastUpdate);
    positionCustomGeometryFPSGraph();
}

/*************************************************************************
    Update the FPS value
*************************************************************************/
void CustomShapesDrawingSample::updateFPS(const float elapsed)
{
    // another frame
    ++d_FPSFrames;

    // Add FPS count if a second has passed
    if ((d_FPSElapsed += elapsed) >= 1.0f)
    {
        if(d_useRealFPS)
            updateFPSData(d_FPSFrames);
        else
        {
            int randomFPS = 50 + rand() % 60;
            updateFPSData(randomFPS);
        }

        // reset counter state
        d_FPSFrames = 0;
        float modValue = 1.0f;
        d_FPSElapsed = std::modf(d_FPSElapsed, &modValue);
    }
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void CustomShapesDrawingSample::deinitialise()
{
    // Destroy the GeometryBuffer created for this sample
    CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();
    renderer->destroyGeometryBuffer(*d_FPSGraphGeometryBuffer);
    // Destroy the SVGImage created for this sample
    CEGUI::ImageManager& imageManager = CEGUI::ImageManager::getSingleton();
    imageManager.destroy("FPSGraphSVG");
    // Destroy the created custom SVGData
    CEGUI::SVGDataManager& svgDataManager = CEGUI::SVGDataManager::getSingleton();
    svgDataManager.destroy(*d_customSVGData);

    // This destroys the window and its child windows
    WindowManager& winMgr = WindowManager::getSingleton();
    winMgr.destroyWindow(d_root);
}

/*************************************************************************
    Update the geometry used for the FPS graph
*************************************************************************/
void CustomShapesDrawingSample::updateFPSGraphs()
{
    d_FPSGraphGeometryBuffer->reset();

    std::vector<glm::vec2> linePositions;

    for(unsigned int i = 0; i < d_FPSGraphSamplesCount; ++i)
    {
        float currentOffset = i / static_cast<float>(d_FPSGraphSamplesCount - 1);

        float relative_height = d_lastFPSValues.at(i) / static_cast<float>(d_FPSMaxGraphValue);

        glm::vec2 currentPosition(currentOffset, relative_height);
        linePositions.push_back(currentPosition);
    }

    // We will update the SVG Polyline object by simply passing the linepoints to it and triggering
    // a re-draw of the Image.
    updateCustomSVGImagePolyline(linePositions);

    // Update the FPS labels for the SVGImage
    updateCustomSVGImageFPSLabels();

    // We will update the GeometryBuffer which contains our line vertices. We will need to calculate
    // the vertices of a line with a width of 1.0 and which will be rendered in green colour.
    updateCustomGeometryGraph(linePositions);

    // Update the FPS Labels for the GeometryBuffer
    updateCustomGeometryFPSLabels();

}

/*************************************************************************
    Update the FPS graph geometry
*************************************************************************/
void CustomShapesDrawingSample::updateCustomGeometryGraph(std::vector<glm::vec2> linePositions)
{
    // Reset all geometry data
    d_FPSGraphGeometryBuffer->reset();

    // Add the background vertices
    d_FPSGraphGeometryBuffer->appendGeometry(d_customGeometryGraphBackgroundVertices);

    //Precalculate y and y coordinate of first point
    linePositions.at(0).x = 0.0f;
    linePositions.at(0).y = d_customGeometryGraphHeight - linePositions.at(0).y * d_customGeometryGraphHeight;

    //We will draw a simple quad for each line segment
    size_t linePositionsCount = linePositions.size();
    for (size_t j = 1; j < linePositionsCount; ++j)
    {
        const glm::vec2& prevPos = linePositions.at(j - 1);
        glm::vec2& currentPos = linePositions.at(j);

        currentPos.x = currentPos.x * d_customGeometryGraphWidth * 0.95f;
        currentPos.y = d_customGeometryGraphHeight - currentPos.y * d_customGeometryGraphHeight;

        // Normalize and tilt the 2D direction vector by 90� to get the vector pointing in the offset direction
        glm::vec2 offsetVector = currentPos - prevPos;
        offsetVector = glm::normalize(offsetVector);
        offsetVector = glm::vec2(offsetVector.y, -offsetVector.x) * 1.0f;

        CEGUI::ColouredVertex linePositionVertex;
        static const CEGUI::Colour lineColour(0.0f, 1.0f, 0.0f, 1.0f);

        linePositionVertex.setColour(lineColour);

        linePositionVertex.d_position = glm::vec3(prevPos - offsetVector, 0.0f);
        d_FPSGraphGeometryBuffer->appendVertex(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(currentPos - offsetVector, 0.0f);
        d_FPSGraphGeometryBuffer->appendVertex(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(currentPos + offsetVector, 0.0f);
        d_FPSGraphGeometryBuffer->appendVertex(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(currentPos + offsetVector, 0.0f);
        d_FPSGraphGeometryBuffer->appendVertex(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(prevPos - offsetVector, 0.0f);
        d_FPSGraphGeometryBuffer->appendVertex(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(prevPos + offsetVector, 0.0f);
        d_FPSGraphGeometryBuffer->appendVertex(linePositionVertex);
    }
}

/*************************************************************************
    Update the FPS graph SVGImage's Polyline
*************************************************************************/
void CustomShapesDrawingSample::updateCustomSVGImagePolyline(std::vector<glm::vec2> linePositions)
{
    //! We need to scale and offset our linepoints to match the chosen SVGImage size
    size_t linePosCount = linePositions.size();
    for(size_t i = 0; i < linePosCount; ++i)
    {
        // Calculating the position with 95% horizontal scale
        linePositions[i].x = linePositions[i].x * d_customSVGImageWidth * 0.95f;
        // Calculating the position with 90% vertical scale and 5% vertical offset
        linePositions[i].y = d_customSVGImageHeight * 0.975f - linePositions[i].y * d_customSVGImageHeight * 0.95f;
    }

   d_customPolyline->d_points = linePositions;

   d_customSVGImageWindow->invalidate();
}

/*************************************************************************
    Sets up everything we need to draw our graph into the GeometryBuffer
*************************************************************************/
void CustomShapesDrawingSample::setupCustomGeometryGraph(CEGUI::GUIContext* guiContext)
{
    CEGUI::Renderer* renderer = CEGUI::System::getSingleton().getRenderer();

    // GeometryBuffer used for drawing in this Sample
    d_FPSGraphGeometryBuffer = &renderer->createGeometryBufferColoured(renderer->createRenderMaterial(DefaultShaderType::Solid));

    // Calculate and save our custom graph background
    setupCustomGeometryGraphBackground();

    // Clearing this queue actually makes sure it's created(!)
    guiContext->clearGeometry(CEGUI::RenderQueueID::Overlay);

    // Subscribe handler to render overlay items
    guiContext->subscribeEvent(CEGUI::RenderingSurface::EventRenderQueueStarted,
        CEGUI::Event::Subscriber(&CustomShapesDrawingSample::drawFPSGraphOverlay,
        this));
}

/*************************************************************************
    Sets up everything we need to draw our graph using an SVGImage object
*************************************************************************/
void CustomShapesDrawingSample::setupCustomSVGImage()
{
    // Create an SVGImage using the ImageManager
    CEGUI::ImageManager& imageManager = CEGUI::ImageManager::getSingleton();
    d_customSVGImage = static_cast<SVGImage*>(&imageManager.create("SVGImage", "FPSGraphSVG"));
    // Create an SVGData object
    CEGUI::SVGDataManager& svgDataManager = CEGUI::SVGDataManager::getSingleton();
    d_customSVGData = &svgDataManager.create(CEGUI::String("FPSGraphCustomShape"));

    // Set the desired size of the SVGData
    d_customSVGData->setWidth(d_customSVGImageWidth);
    d_customSVGData->setHeight(d_customSVGImageHeight);

    // Set the pointer to the SVGData for the SVGImage
    d_customSVGImage->setSVGData(d_customSVGData);
    // We make our SVGImage the same size as the SVGData
    const Rectf imageArea(glm::vec2(0, 0), CEGUI::Sizef(d_customSVGData->getWidth(), d_customSVGData->getHeight()));
    d_customSVGImage->setImageArea(imageArea);

    // We create a graph background consisting of a grey background and some lines for better readability
    setupCustomSVGImageGraphBackground(*d_customSVGData);

    // We create polyline object for the visualisation of the last FPS values. It contains no points yet.
    std::vector<glm::vec2> pointsList;
    d_customPolyline = new SVGPolyline(SVGPaintStyle(), glm::mat3x3(1.0f), pointsList);
    d_customPolyline->d_paintStyle.d_stroke.d_colour = glm::vec3(0.0f, 1.0f, 0.0f);
    d_customPolyline->d_paintStyle.d_strokeLinejoin = SVGPaintStyle::SVGLinejoin::Round;
    d_customPolyline->d_paintStyle.d_strokeWidth = 2.0f;
    //By default the SVG standard has the default fill set to black. We do not want any fill so we switch it to "none".
    d_customPolyline->d_paintStyle.d_fill.d_none = true;

    //We add the created shape to the SVGData object
    d_customSVGData->addShape(d_customPolyline);
}

/*************************************************************************
    Refreshes the maximum value that will be visualised in the graph
*************************************************************************/
void CustomShapesDrawingSample::refreshFPSMaxGraphValue()
{
    d_FPSMaxGraphValue = 0;

    size_t lastFPSValuesCount = d_lastFPSValues.size();
    for(size_t i = 0; i < lastFPSValuesCount; ++i)
        d_FPSMaxGraphValue = std::max(d_FPSMaxGraphValue, d_lastFPSValues[i]);
}

/*************************************************************************
    Creates a checkbox to let the user choose to display randomised or real FPS value
*************************************************************************/
void CustomShapesDrawingSample::createCheckboxShowRealFPS()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    // We create a button and subscribe to its click events
    ToggleButton* checkboxShowRealFPS = static_cast<CEGUI::ToggleButton*>(winMgr.createWindow("WindowsLook/Checkbox"));
    checkboxShowRealFPS->setSize(CEGUI::USize(cegui_reldim(0.25f), cegui_reldim(0.035f)));
    checkboxShowRealFPS->setHorizontalAlignment(HorizontalAlignment::Centre);
    checkboxShowRealFPS->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.13f)));
    checkboxShowRealFPS->setText("Show randomly generated FPS values");
    checkboxShowRealFPS->subscribeEvent(ToggleButton::EventSelectStateChanged, Event::Subscriber(&CustomShapesDrawingSample::handleToggleButtonShowRandomisedFpsSelectionChanged, this));
    checkboxShowRealFPS->setSelected(true);
    d_root->addChild(checkboxShowRealFPS);
}

/*************************************************************************
    Creates a label that will display our last FPS value
*************************************************************************/
void CustomShapesDrawingSample::createLastFPSLabel()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    // We create a button and subscribe to its click events
    d_lastFPSLabel = winMgr.createWindow("Generic/Label");
    d_lastFPSLabel->setSize(CEGUI::USize(cegui_reldim(1.0f), cegui_reldim(0.035f)));
    d_lastFPSLabel->setHorizontalAlignment(HorizontalAlignment::Centre);
    d_lastFPSLabel->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_reldim(0.18f)));
    d_root->addChild(d_lastFPSLabel);
}

/*************************************************************************
    Creates a label that contains a description of what is seen in the sample
*************************************************************************/
void CustomShapesDrawingSample::createDescriptionLabel()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    // We create a button and subscribe to its click events
    CEGUI::Window* descriptionLabel = winMgr.createWindow("Generic/Label");
    descriptionLabel->setSize(CEGUI::USize(cegui_reldim(0.8f), cegui_reldim(0.25f)));
    descriptionLabel->setHorizontalAlignment(HorizontalAlignment::Centre);
    descriptionLabel->setProperty("HorzFormatting", "WordWrapCentreAligned");
    descriptionLabel->setPosition(CEGUI::UVector2(cegui_reldim(0.0f), cegui_absdim(400.0f)));
    d_root->addChild(descriptionLabel);
    descriptionLabel->setText("The left graph is rendered directly into a GeometryBuffer and rendered as overlay."
        "The right graph is created using an SVGImage with modified SVGData and is then used in the same way as a regular CEGUI image"
        " - it is set as a property for a window that renders the image. \n\n"
        "The benefit of creating custom geometry by using an SVGImage and its SVGData is that it can be conveniently added to a CEGUI Window "
        "and that defining primitive objects, such as lines, rectangles etc., is easier. Additionally, the internal CEGUI SVG classes which "
        "create the geometry offer an anti-aliasing option that delivers anti-aliasing at almost no extra performance cost for the GPU.");
}

/*************************************************************************
    Event handler for the FPS value checkbox selection change
*************************************************************************/
bool CustomShapesDrawingSample::handleToggleButtonShowRandomisedFpsSelectionChanged(const CEGUI::EventArgs& args)
{
    const CEGUI::WindowEventArgs& winArgs = static_cast<const CEGUI::WindowEventArgs&>(args);
    CEGUI::ToggleButton* checkbox = static_cast<CEGUI::ToggleButton*>(winArgs.window);

    d_useRealFPS = !checkbox->isSelected();

    return true;
}

/*************************************************************************
    Updates everything needed for the graphs and then the graphs themselves
*************************************************************************/
void CustomShapesDrawingSample::updateFPSData(int newFPSValue)
{
    //Update the window displaying the current FPS as text
    std::stringstream sstream;
    sstream << newFPSValue;
    d_lastFPSLabel->setText(CEGUI::String("Currently recorded FPS: " + sstream.str()));

    d_lastFPSValues.pop_front();
    d_lastFPSValues.push_back(newFPSValue);
    refreshFPSMaxGraphValue();

    //Update the graphs
    updateFPSGraphs();
}

/*************************************************************************
    Sets up the background of the GeometryBuffer-based graph
*************************************************************************/
void CustomShapesDrawingSample::setupCustomGeometryGraphBackground()
{
    // Add a grey background quad
    static const glm::vec2 topLeft(0.0f, 0.0f);
    static const glm::vec2 bottomLeft(0.0f, d_customGeometryGraphHeight);
    static const glm::vec2 topRight(d_customGeometryGraphWidth * 0.9f + d_customGeometryGraphWidth * 0.05f, 0.0f);
    static const glm::vec2 bottomRight(d_customGeometryGraphWidth * 0.9f + d_customGeometryGraphWidth * 0.05f, d_customGeometryGraphHeight);

    // Defining some colours we will use
    static const CEGUI::Colour backgroundQuadColour(0.3f, 0.3f, 0.3f, 1.0f);
    static const CEGUI::Colour lineColour(0.5f, 0.5f, 0.5f, 1.0f);

    CEGUI::ColouredVertex backgroundQuadVertex;
    backgroundQuadVertex.setColour(backgroundQuadColour);

    backgroundQuadVertex.d_position = glm::vec3(topLeft, 0.0f);
    d_customGeometryGraphBackgroundVertices.push_back(backgroundQuadVertex);

    backgroundQuadVertex.d_position = glm::vec3(bottomLeft, 0.0f);
    d_customGeometryGraphBackgroundVertices.push_back(backgroundQuadVertex);

    backgroundQuadVertex.d_position = glm::vec3(topRight, 0.0f);
    d_customGeometryGraphBackgroundVertices.push_back(backgroundQuadVertex);

    backgroundQuadVertex.d_position = glm::vec3(topRight, 0.0f);
    d_customGeometryGraphBackgroundVertices.push_back(backgroundQuadVertex);

    backgroundQuadVertex.d_position = glm::vec3(bottomLeft, 0.0f);
    d_customGeometryGraphBackgroundVertices.push_back(backgroundQuadVertex);

    backgroundQuadVertex.d_position = glm::vec3(bottomRight, 0.0f);
    d_customGeometryGraphBackgroundVertices.push_back(backgroundQuadVertex);

    // We save some horizontical line vertices that we will use for our graph's background.
    CEGUI::ColouredVertex linePositionVertex;
    linePositionVertex.setColour(lineColour);

    size_t lineCount = 7;
    for (size_t i = 0; i < lineCount; ++i)
    {
        const float currentHeight = d_customGeometryGraphHeight * i / (lineCount - 1);
        const glm::vec2 linePointBegin(0.0f, currentHeight);
        const glm::vec2 linePointEnd(d_customGeometryGraphWidth, currentHeight);

        glm::vec2 offsetVector(0.0f, 0.5f);

        linePositionVertex.d_position = glm::vec3(linePointBegin - offsetVector, 0.0f);
        d_customGeometryGraphBackgroundVertices.push_back(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(linePointEnd - offsetVector, 0.0f);
        d_customGeometryGraphBackgroundVertices.push_back(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(linePointEnd + offsetVector, 0.0f);
        d_customGeometryGraphBackgroundVertices.push_back(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(linePointEnd + offsetVector, 0.0f);
        d_customGeometryGraphBackgroundVertices.push_back(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(linePointBegin - offsetVector, 0.0f);
        d_customGeometryGraphBackgroundVertices.push_back(linePositionVertex);

        linePositionVertex.d_position = glm::vec3(linePointBegin + offsetVector, 0.0f);
        d_customGeometryGraphBackgroundVertices.push_back(linePositionVertex);
    }
}

/*************************************************************************
    Creates the labels that we will use to display the FPS values next to the GeometryBuffer-based graph
*************************************************************************/
void CustomShapesDrawingSample::createCustomGeometryFPSLabels()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    // We create a button and subscribe to its click events
    d_customGeometryFPSLabel1 = winMgr.createWindow("Generic/Label");
    d_customGeometryFPSLabel1->setSize(CEGUI::USize(cegui_absdim(100.0f), cegui_reldim(0.035f)));
    d_customGeometryFPSLabel1->setProperty("HorzFormatting", "LeftAligned");
    d_root->addChild(d_customGeometryFPSLabel1);
    d_customGeometryFPSLabel1->setFont("DejaVuSans-10");

    // We create a button and subscribe to its click events
    d_customGeometryFPSLabel2 = winMgr.createWindow("Generic/Label");
    d_customGeometryFPSLabel2->setSize(CEGUI::USize(cegui_absdim(100.0f), cegui_reldim(0.035f)));
    d_customGeometryFPSLabel2->setProperty("HorzFormatting", "LeftAligned");
    d_root->addChild(d_customGeometryFPSLabel2);
    d_customGeometryFPSLabel2->setFont("DejaVuSans-10");

    // We create a button and subscribe to its click events
    d_customGeometryFPSLabel3 = winMgr.createWindow("Generic/Label");
    d_customGeometryFPSLabel3->setSize(CEGUI::USize(cegui_absdim(100.0f), cegui_reldim(0.035f)));
    d_customGeometryFPSLabel3->setProperty("HorzFormatting", "LeftAligned");
    d_root->addChild(d_customGeometryFPSLabel3);
    d_customGeometryFPSLabel3->setFont("DejaVuSans-10");
}

/*************************************************************************
    Updates the labels we will display next to the graph made with the GeometryBuffer
*************************************************************************/
void CustomShapesDrawingSample::updateCustomGeometryFPSLabels()
{
    //Calculate our top position with the half of the window's height as offset for centering
    float absoluteHeight = d_customGeometryFPSLabel1->getUnclippedOuterRect().get().getHeight();
    float centeredPositionTop = 250.0f - absoluteHeight * 0.5f;
    std::stringstream sstream;

    d_customGeometryFPSLabel1->setPosition(CEGUI::UVector2(cegui_absdim(550.0f + 2.0f), cegui_absdim(centeredPositionTop + 100.0f)));
    d_customGeometryFPSLabel1->setText(CEGUI::String("0"));

    d_customGeometryFPSLabel2->setPosition(CEGUI::UVector2(cegui_absdim(550.0f + 2.0f), cegui_absdim(centeredPositionTop + 100.f * 0.5f)));
    unsigned int halfMaxValue = d_FPSMaxGraphValue / 2;
    sstream << halfMaxValue;
    d_customGeometryFPSLabel2->setText(sstream.str());
    sstream.str("");

    d_customGeometryFPSLabel3->setPosition(CEGUI::UVector2(cegui_absdim(550.0f + 2.0f), cegui_absdim(centeredPositionTop)));
    sstream << d_FPSMaxGraphValue;
    d_customGeometryFPSLabel3->setText(sstream.str());
}

/*************************************************************************
    Creates the labels we will display next to the graph using the SVGImage
*************************************************************************/
void CustomShapesDrawingSample::createCustomSVGImageFPSLabels()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    // We create a button and subscribe to its click events
    d_customSVGImageFPSLabel1 = winMgr.createWindow("Generic/Label");
    d_customSVGImageFPSLabel1->setSize(CEGUI::USize(cegui_reldim(0.3f), cegui_reldim(0.15f)));
    d_customSVGImageFPSLabel1->setProperty("HorzFormatting", "LeftAligned");
    d_customSVGImageFrameWindow->addChild(d_customSVGImageFPSLabel1);
    d_customSVGImageFPSLabel1->setFont("DejaVuSans-10");

    // We create a button and subscribe to its click events
    d_customSVGImageFPSLabel2 = winMgr.createWindow("Generic/Label");
    d_customSVGImageFPSLabel2->setSize(CEGUI::USize(cegui_reldim(0.3f), cegui_reldim(0.15f)));
    d_customSVGImageFPSLabel2->setProperty("HorzFormatting", "LeftAligned");
    d_customSVGImageFrameWindow->addChild(d_customSVGImageFPSLabel2);
    d_customSVGImageFPSLabel2->setFont("DejaVuSans-10");

    // We create a button and subscribe to its click events
    d_customSVGImageFPSLabel3 = winMgr.createWindow("Generic/Label");
    d_customSVGImageFPSLabel3->setSize(CEGUI::USize(cegui_reldim(0.3f), cegui_reldim(0.15f)));
    d_customSVGImageFPSLabel3->setProperty("HorzFormatting", "LeftAligned");
    d_customSVGImageFrameWindow->addChild(d_customSVGImageFPSLabel3);
    d_customSVGImageFPSLabel3->setFont("DejaVuSans-10");
}

/*************************************************************************
    Updates the labels we will display next to the graph using the SVGImage
*************************************************************************/
void CustomShapesDrawingSample::updateCustomSVGImageFPSLabels()
{
    //Calculate our top position with the half of the window's height as offset for centering
    float absoluteHeight = d_customSVGImageFPSLabel1->getUnclippedOuterRect().get().getHeight();
    float centeredPositionOffset = -absoluteHeight * 0.5f;
    std::stringstream sstream;

    //We horizontically position the windows after the graph SVGImage window
    CEGUI::UDim labelPosX = CEGUI::UDim(0.75f, 2.0f);

    //The vertical position is calculated from the image and window heights and results in a fixed value for the relative dimension.
    d_customSVGImageFPSLabel1->setPosition(CEGUI::UVector2(labelPosX, CEGUI::UDim(0.9275f, centeredPositionOffset)));
    d_customSVGImageFPSLabel1->setText(CEGUI::String("0"));

    d_customSVGImageFPSLabel2->setPosition(CEGUI::UVector2(labelPosX, CEGUI::UDim(0.5f, centeredPositionOffset)));
    unsigned int halfMaxValue = d_FPSMaxGraphValue / 2;
    sstream << halfMaxValue;
    d_customSVGImageFPSLabel2->setText(sstream.str());
    sstream.str("");

    d_customSVGImageFPSLabel3->setPosition(CEGUI::UVector2(labelPosX, CEGUI::UDim(0.0725f, centeredPositionOffset)));
    sstream << d_FPSMaxGraphValue;
    d_customSVGImageFPSLabel3->setText(sstream.str());
}

/*************************************************************************
    Sets up the background of the SVGImage-based graph
*************************************************************************/
void CustomShapesDrawingSample::setupCustomSVGImageGraphBackground(CEGUI::SVGData &fpsSVGData)
{
    // We make a grey quad background for the graph. We offset its position by 2.5% of the height and
    // make the total height of the rect 95% of the image height. We will later do the same with our lines
    // so that their strokes won't be clipped by the image's area.
    SVGRect* svg_rect = new SVGRect(SVGPaintStyle(), glm::mat3x3(1.0f),
        0.0f, d_customSVGImageHeight * 0.025f,
        d_customSVGImageWidth * 0.95f, d_customSVGImageHeight * 0.95f);
    svg_rect->d_paintStyle.d_fill.d_colour = glm::vec3(0.3f, 0.3f, 0.3f);
    // We don't want a stroke to be rendered. SVG offers a black stroke with 1 width by default.
    svg_rect->d_paintStyle.d_stroke.d_none = true;
    fpsSVGData.addShape(svg_rect);

    //We create our background line's style
    SVGPaintStyle graphBGLineStyle;
    graphBGLineStyle.d_stroke.d_colour = glm::vec3(0.5f, 0.5f, 0.5f);
    graphBGLineStyle.d_strokeWidth = 0.35f;

    // We don't fill the full height because the strokes need some extra space
    float adjustedHeight = d_customSVGImageHeight * 0.95f;

    // We save some horizontical line vertices that we will use for our graph's background.
    size_t lineCount = 7;
    for (size_t i = 0; i < lineCount; ++i)
    {
        // We calculate the current height of the line vertex using an offset from the top and the modified height, so that the stroke won't get clipped by the image
        const float currentHeight = d_customSVGImageHeight * 0.025f + adjustedHeight * i / static_cast<float>(lineCount - 1);
        const glm::vec2 linePointBegin(0.0f, currentHeight);
        const glm::vec2 linePointEnd(d_customSVGImageWidth, currentHeight);

        SVGLine* currentLine = new SVGLine(graphBGLineStyle, glm::mat3x3(1.0f), linePointBegin, linePointEnd);
        fpsSVGData.addShape(currentLine);
    }
}

/*************************************************************************
    Creates the windows which will be used to display the SVGImage graph
*************************************************************************/
void CustomShapesDrawingSample::createCustomSVGImageWindows()
{
    WindowManager& winMgr = WindowManager::getSingleton();

    //We create a draggable and resizable frame window that will contain our Image window.
    d_customSVGImageFrameWindow = winMgr.createWindow("WindowsLook/FrameWindow");
    d_customSVGImageFrameWindow->setSize(CEGUI::USize(cegui_absdim(490.0f), cegui_absdim(160.0f)));
    d_customSVGImageFrameWindow->setPosition(CEGUI::UVector2(cegui_reldim(0.5f), cegui_reldim(0.29f)));
    d_root->addChild(d_customSVGImageFrameWindow);
    d_customSVGImageFrameWindow->subscribeEvent(CEGUI::Window::EventSized,
        CEGUI::Event::Subscriber(&CustomShapesDrawingSample::handleSVGImageFrameWindowSizeChanged,
        this));

    //We create the image window through which our custom SVGImage will be rendered.
    d_customSVGImageWindow = winMgr.createWindow("Generic/Image");
    d_customSVGImageWindow->setSize(CEGUI::USize(cegui_reldim(0.7f), cegui_reldim(0.9f)));
    d_customSVGImageWindow->setPosition(CEGUI::UVector2(cegui_reldim(0.05f), cegui_reldim(0.05f)));
    d_customSVGImageFrameWindow->addChild(d_customSVGImageWindow);
    d_customSVGImageWindow->setProperty("Image", "FPSGraphSVG");
}

/*************************************************************************
    Display size change handler
*************************************************************************/
bool CustomShapesDrawingSample::handleDisplaySizeChange(const CEGUI::EventArgs&)
{
    updateCustomGeometryFPSLabels();

    updateCustomSVGImageFPSLabels();

    return false;
}

/*************************************************************************
    Handler for size changes of the custom-SVGImage FrameWindow
*************************************************************************/
bool CustomShapesDrawingSample::handleSVGImageFrameWindowSizeChanged
  (const CEGUI::EventArgs&)
{
    updateCustomSVGImageFPSLabels();

    return false;
}
