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
#ifndef _SampleCustomShapesDrawingSample_h_
#define _SampleCustomShapesDrawingSample_h_

#include "Sample.h"

#include "CEGUI/ForwardRefs.h"
#include <glm/glm.hpp>

#include <deque>
#include <vector>

namespace CEGUI
{
    class Window;
    class DefaultWindow;
}

class CustomShapesDrawingSample : public Sample
{
public:
    CustomShapesDrawingSample();
    virtual ~CustomShapesDrawingSample() {}

    //! Initialises our sample
    bool initialise(CEGUI::GUIContext* guiContext) override;

    void deinitialise() override;

private:
    //! The general update function of our sample
    void update(float timeSinceLastUpdate) override;

    //! Updates the counted FPS and triggers the recreation of FPS Graph geometry if needed.
    void updateFPS(float timeSinceLastUpdate);

    //! Updates everything needed for the graphs including  the graphs themselves
    void updateFPSData(int randomFPS);

    //! Refreshes the maximum value that will be visualised in the graph
    void refreshFPSMaxGraphValue();

    //! Update the geometry used for the FPS graph
    void updateFPSGraphs();

    //! Event handler that triggers the redrawing of our GeometryBuffer-based graph.
    bool drawFPSGraphOverlay(const CEGUI::EventArgs& args);

    //! Sets up everything necessary to use and render our custom SVGImage.
    void setupCustomSVGImage();

    //! Sets up the background of the SVGImage-based graph
    void setupCustomSVGImageGraphBackground(CEGUI::SVGData& fpsSVGData);

    //! Sets up everything necessary to use and render our custom GeometryBuffer.
    void setupCustomGeometryGraph(CEGUI::GUIContext* guiContext);

    //! Positions the GeometryBuffer based graph
    void positionCustomGeometryFPSGraph();

    //! Sets up the background of the GeometryBuffer-based graph 
    void setupCustomGeometryGraphBackground();

    //! Update the FPS graph geometry
    void updateCustomGeometryGraph(std::vector<glm::vec2> linePositions);

    //! Update the FPS graph SVGImage's Polyline
    void updateCustomSVGImagePolyline(std::vector<glm::vec2> linePositions);

    //! Creates the labels we will display next to the graph using the SVGImage
    void createCustomSVGImageFPSLabels();

    //! Updates the labels we will display next to the graph made with the GeometryBuffer
    void updateCustomGeometryFPSLabels();

    //! Creates the windows which will be used to display the SVGImage graph
    void createCustomSVGImageWindows();

    //! Creates the labels that we will use to display the FPS values next to the GeometryBuffer-based graph
    void createCustomGeometryFPSLabels();

    //! Updates the labels we will display next to the graph using the SVGImage
    void updateCustomSVGImageFPSLabels();

    //! Creator function for our Random-FPS checkbox
    void createCheckboxShowRealFPS();

    //! Creates a label that will display our last FPS value
    void createLastFPSLabel();

    //! Creates a label that contains a description of what is seen in the sample
    void createDescriptionLabel();

    //! The event handler for clicks on the Random-FPS checkbox.
    bool handleToggleButtonShowRandomisedFpsSelectionChanged(const CEGUI::EventArgs& args);

    //! Display size change handler
    bool handleDisplaySizeChange(const CEGUI::EventArgs& args);

    //! Handler for size changes of the custom-SVGImage FrameWindow
    bool handleSVGImageFrameWindowSizeChanged(const CEGUI::EventArgs& args);



    //! Our sample's root window.
    CEGUI::DefaultWindow*           d_root;

    //! Our window that renders the custom SVGImage.
    CEGUI::Window*                  d_customSVGImageWindow;

    //! A Framewindow to size and drag around. It contains our custom SVGImage window.
    CEGUI::Window*                  d_customSVGImageFrameWindow;

    //! The Polyline used to define our FPS graph lines. It will be added in the custom SVGImage's SVGData in our sample.
    CEGUI::SVGPolyline*             d_customPolyline;

    //! The GeometryBuffer that will be used to draw the FPS graph.
    CEGUI::GeometryBuffer*          d_FPSGraphGeometryBuffer;

    //! The FPS values we want to display.
    std::deque<unsigned int>        d_lastFPSValues;

    //! The number of samples we want to display in our FPS graph.
    unsigned int                    d_FPSGraphSamplesCount;

    //! Number of frames drawn so far.
    int                             d_FPSFrames;

    //! Fraction of second elapsed (used for counting frames per second).
    float                           d_FPSElapsed;

    //! The maximum FPS value the graph will be normalised to.
    unsigned int                    d_FPSMaxGraphValue;

    //! Defines if we should use actual FPS values or use random generated ones of a certain range for better display.
    bool                            d_useRealFPS;

    //! Width of the sample SVGImage
    float                           d_customSVGImageWidth;

    //! Height of the sample SVGImage
    float                           d_customSVGImageHeight;

    //! The vertices we generated to draw the static background of our GeometryBuffer based graph
    std::vector<CEGUI::ColouredVertex> d_customGeometryGraphBackgroundVertices;

    //! The width of the graph made using a GeometryBuffer
    float                           d_customGeometryGraphWidth;
    //! The height of the graph made using a GeometryBuffer
    float                           d_customGeometryGraphHeight;

    //! The window displaying the last FPS
    CEGUI::Window*                  d_lastFPSLabel;

    //! Labels that we will use to display the FPS next to the GeometryBuffer-based graph
    CEGUI::Window*                  d_customGeometryFPSLabel1;
    CEGUI::Window*                  d_customGeometryFPSLabel2;
    CEGUI::Window*                  d_customGeometryFPSLabel3;

    //! Labels that we will use to display the FPS next to the SVGImage-based graph
    CEGUI::Window*                  d_customSVGImageFPSLabel1;
    CEGUI::Window*                  d_customSVGImageFPSLabel2;
    CEGUI::Window*                  d_customSVGImageFPSLabel3;

    //! The custom SVGImage we will create to render one of the graphs
    CEGUI::SVGImage*                d_customSVGImage;
    //! The custom SVGData we will use for our custom SVGImage
    CEGUI::SVGData*                 d_customSVGData;
};

#endif  // end of guard _Sample_FirstWindow_h_
