/***********************************************************************
filename:   SamplesWindowManager.cpp
created:    11/6/2012
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
#include "SamplesWindowManager.h"

#include "CEGUI/Window.h"
#include "CEGUI/SchemeManager.h"
#include "CEGUI/WindowManager.h"

using namespace CEGUI;

SamplesWindowManager::SamplesWindowManager(CEGUI::Window* samplesWindow)
    : d_root(samplesWindow),
    d_childCount(0),
    d_widthToHeightFactor(1.f)
{
}


CEGUI::Window* SamplesWindowManager::getWindow()
{
    return d_root;
}

void SamplesWindowManager::addSampleWindow(CEGUI::Window* sampleWindow)
{
    d_root->addChild(sampleWindow);
    d_sampleWindows.push_back(sampleWindow);

    ++d_childCount;

    CEGUI::UVector2 position(cegui_reldim(0.f), cegui_reldim(0.f));

    position.d_y.d_scale += int((d_childCount - 1) / 2) * 0.5f;

    sampleWindow->setPosition(position);
    sampleWindow->setSize(USize(UDim(1.f, -10.f), cegui_reldim(0.4f)));

    CEGUI::ColourRect colRect(CEGUI::Colour(argb_t(0xFF659d96)));
    sampleWindow->setProperty("FrameColours", CEGUI::PropertyHelper<ColourRect>::toString(colRect));
}

void SamplesWindowManager::setWindowRatio(float widthToHeight)
{
    d_widthToHeightFactor = widthToHeight;

    updateWindows();
}

void SamplesWindowManager::updateWindows()
{
    float vertOffset = 0.f;

    int max = d_sampleWindows.size();
    for(int i = 0; i < max; ++i)
    {
        CEGUI::Window* window(d_sampleWindows[i]);

        float width = window->getOuterRectClipper().getWidth();
        float height = width * d_widthToHeightFactor;

        window->setSize(CEGUI::USize(cegui_absdim(width), cegui_absdim(height)));

        window->setPosition(CEGUI::UVector2(cegui_absdim(0.f), cegui_absdim(vertOffset)));

        vertOffset += height + 15;
    }
}