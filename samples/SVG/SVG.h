/***********************************************************************
    created:    11th August 2013
    author:     Lukas Meindl
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
#ifndef _SVG_Sample_h_
#define _SVG_Sample_h_

#include "Sample.h"

#include "CEGUI/ForwardRefs.h"

namespace CEGUI
{
    class Window;
    class DefaultWindow;
}

class SVGSample : public Sample
{
public:
    SVGSample();
    virtual ~SVGSample() {}

    bool initialise(CEGUI::GUIContext* guiContext) override;
    void deinitialise() override;

    void update(float timeSinceLastUpdate) override;

protected:  
    CEGUI::DefaultWindow*       d_root;

private:
    //! An event handler that handles clicks on the anti-aliasing on/off button
    bool handleAntialiasingButtonClicked(const CEGUI::EventArgs& args);

    //! A pointer to the window that will display the sample SVGImage
    CEGUI::Window* d_svgImageWindow;
    //! A pointer to our sample SVGImage object
    CEGUI::SVGImage*    d_svgSampleImage;
};

#endif
