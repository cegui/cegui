/***********************************************************************
    filename:   Sample_FirstWindow.cpp
    created:    10/3/2005
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "Sample_FirstWindow.h"
#include "CEGUI.h"
#include <iostream>

int main(int /*argc*/, char* /*argv*/[])
{
    // This is a basic start-up for the sample application which is
    // object orientated in nature, so we just need an instance of
    // the CEGuiSample based object and then tell that sample application
    // to run.  All of the samples will use code similar to this in the
    // main/WinMain function.
    FirstWindowSample app;
    return app.run();
}

/*************************************************************************
    Sample specific initialisation goes here.
*************************************************************************/
bool FirstWindowSample::initialiseSample()
{
    using namespace CEGUI;

    WindowFactoryManager::getSingleton().addWindowTypeAlias("TaharezLook/WobblyFrameWindow", "TaharezLook/FrameWindow");

    SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    System::getSingleton().setDefaultMouseCursor("TaharezLook/MouseArrow");
    WindowManager& winMgr = WindowManager::getSingleton();
    Window* root = winMgr.loadLayoutFromFile("Demo7Windows.layout");

    XMLSerializer s(std::cout);
    root->writeXMLToStream(s);

    return true;
}


/*************************************************************************
    Cleans up resources allocated in the initialiseSample call.
*************************************************************************/
void FirstWindowSample::cleanupSample()
{
    // nothing to do here!
}
