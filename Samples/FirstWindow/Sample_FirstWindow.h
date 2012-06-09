/***********************************************************************
    filename:   Sample_FirstWindow.h
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
#ifndef _Sample_FirstWindow_h_
#define _Sample_FirstWindow_h_

#include "SampleBase.h"


namespace CEGUI
{
    class Window;
    class DefaultWindow;
}

class FirstWindowSample : public Sample
{
public:
    FirstWindowSample() {}
    virtual ~FirstWindowSample() {}

    virtual bool initialise(CEGUI::GUIContext* guiContext);
    virtual void deinitialise();

    virtual CEGUI::Window* getGUIRoot();
private:
    FirstWindowSample(const FirstWindowSample&) {}
    FirstWindowSample& operator=(const FirstWindowSample&) {}

    CEGUI::DefaultWindow*      d_root;
};

#endif  // end of guard _Sample_FirstWindow_h_
