/***********************************************************************
filename:   SamplesWindowManager.h
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
#ifndef _SamplesWindowManager_h_
#define _SamplesWindowManager_h_

#include <vector>
#include <stdint.h>

namespace CEGUI
{
    class Window;
    class EventArgs;
}

class SamplesFramework;

class SamplesWindowManager
{
public:
    SamplesWindowManager(SamplesFramework* owner, CEGUI::Window* samplesWindow);
    virtual ~SamplesWindowManager() {}

    CEGUI::Window* getWindow();

    void addSampleWindow(CEGUI::Window* sampleWindow);
    void setWindowRatio(float widthToHeight);

    void selectSampleWindow(CEGUI::Window* wnd);

private:
    SamplesWindowManager(const SamplesWindowManager&) {}
    SamplesWindowManager& operator=(const SamplesWindowManager&) {}

    static const uint32_t d_sampleWindowFrameNormal;
    static const uint32_t d_sampleWindowFrameSelected;

    void updateWindows();

    bool handleMouseClickSampleWindow(const CEGUI::EventArgs& args);
    bool handleMouseDoubleClickSampleWindow(const CEGUI::EventArgs& args);

    SamplesFramework* d_owner;

    CEGUI::Window* d_root;
    int d_childCount;
    float d_widthToHeightFactor;
    CEGUI::Window* d_selectedWindow;

    std::vector<CEGUI::Window*> d_sampleWindows;
};

#endif