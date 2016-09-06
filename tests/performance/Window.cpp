/***********************************************************************
 *    created:    Sat Aug 16 2014
 *    author:     Martin Preisler
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
#include "PerformanceTest.h"

#include <boost/test/unit_test.hpp>

#include "CEGUI/Window.h"
#include "CEGUI/Vector.h"

class DefaultWindowPerformanceTest : public PerformanceTest
{
public:
    DefaultWindowPerformanceTest(void (CEGUI::Window::*function)(), CEGUI::String test_name) :
        PerformanceTest(test_name),
        d_function(function)
    {
        d_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
        d_root->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        d_root->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

        for (unsigned int i = 0; i < 100; ++i)
        {
            d_windows.push_back(d_root->createChild("DefaultWindow"));
        }
    }

    virtual void doTest()
    {
        for (unsigned int i = 0; i < 1000; ++i)
        {
            for (unsigned int i = 0; i < 100; ++i)
            {
                (d_windows[i]->*d_function)();
            }
        }
    }

    CEGUI::Window* d_root;
    std::vector<CEGUI::Window*> d_windows;
    void (CEGUI::Window::* d_function)();
};

BOOST_AUTO_TEST_SUITE(WindowPerformance)

BOOST_AUTO_TEST_CASE(MoveToBack)
{
    DefaultWindowPerformanceTest test(&CEGUI::Window::moveToBack,
        "1000x 100 windows moved back (100 windows total)");
    test.execute();
}

BOOST_AUTO_TEST_CASE(MoveToFront)
{
    DefaultWindowPerformanceTest test(&CEGUI::Window::moveToFront,
        "1000x 100 windows moved front (100 windows total)");
    test.execute();
}

BOOST_AUTO_TEST_SUITE_END()
