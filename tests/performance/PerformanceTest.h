/***********************************************************************
 *    created:    Fri Aug 15 2014
 *    author:     Timotei Dolean <timotei21@gmail.com>
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2014 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUITestsPerformanceTest_h_
#define _CEGUITestsPerformanceTest_h_

#include <boost/timer/timer.hpp>

#include "CEGUI/WindowManager.h"

/*!
\brief
    Generic test on a Window.
*/
template<typename TWindow>
class PerformanceTest
{
public:
    PerformanceTest(CEGUI::String windowType, CEGUI::String renderer) :
        d_window(0)
    {
        d_window = static_cast<TWindow*>(CEGUI::WindowManager::getSingleton().createWindow(windowType, windowType + "perf-test"));
        d_window->setWindowRenderer(renderer);
    }

    virtual ~PerformanceTest() {}

    virtual void render()
    {
        d_window->render();
    }

    void execute()
    {
        std::cout << "Performance test for " << d_window->getType() << std::endl;

        boost::timer::auto_cpu_timer timer;

        doTest();
    }

    TWindow* d_window;

protected:
    virtual void doTest() = 0;
};

#endif
