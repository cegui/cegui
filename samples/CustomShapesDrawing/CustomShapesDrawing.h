/***********************************************************************
    filename:   CustomShapesDrawing.h
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
#ifndef _Sample_Custom_Shapes_Drawing_h_
#define _Sample_Custom_Shapes_Drawing_h_

#include "SampleBase.h"

#include <deque>

namespace CEGUI
{
    class Window;
    class DefaultWindow;
}

class CustomShapesDrawing : public Sample
{
public:
    CustomShapesDrawing();

    virtual bool initialise(CEGUI::GUIContext* guiContext);

    void updateFPSGraphGeometry();

    virtual void deinitialise();

    void positionGeometryBuffer();

    void update(float timeSinceLastUpdate);

protected:
    
    /*!
    \brief
        Updates the counted FPS and triggers the recreation of FPS Graph geometry
        if needed.
    */
    void updateFPS(float timeSinceLastUpdate);
    
    /*!
    \brief
        Triggers the drawing of our FPS graph after everything else was rendered.
    */
    bool drawFPSGraphOverlay(const CEGUI::EventArgs& args);

    CEGUI::DefaultWindow*       d_root;

    //! The GeometryBuffer that will be used to draw the FPS graph.
    CEGUI::GeometryBuffer*      d_FPSGraphGeometry;

    //! The FPS values we want to display.
    std::deque<unsigned int>    d_lastFPSValues;

    //! The number of bars we want to display in our FPS graph.
    unsigned int d_FPSGraphBarsCount;

    //! Number of frames drawn so far.
    int d_FPSFrames;

    //! Fraction of second elapsed (used for counting frames per second).
    float d_FPSElapsed;

    //! The maximum FPS value the graph bars will be normalised to.
    int d_FPSMaxGraphValue;
};

#endif  // end of guard _Sample_FirstWindow_h_
