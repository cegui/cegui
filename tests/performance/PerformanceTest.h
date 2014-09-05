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
#include <fstream>

#include "CEGUI/WindowManager.h"

/*!
\brief
    General structure of a performance test.

    It will print to console the total running time in seconds, and output the
    result(s), by appending them in the performance-test-results.csv file.
*/
class PerformanceTest
{
public:
    explicit PerformanceTest(CEGUI::String test_name) : d_testName(test_name) { }
    virtual ~PerformanceTest() {}


    CEGUI::String d_testName;

    void execute()
    {
        std::cout
            << "Running performance test " << d_testName << "..." << std::endl;

        boost::timer::auto_cpu_timer timer;

        doTest();

        logRunningTime(boost::timer::format(timer.elapsed(), timer.places(), "%u, %w"));
    }

protected:
    virtual void doTest() = 0;

private:
    void logRunningTime(std::string result)
    {
        std::ofstream fout("performance-test-results.csv",
            std::ofstream::out | std::ofstream::app);

        // fill column names if file is empty.
        fout.seekp(0, std::ios::end);
        const std::streamoff length = fout.tellp();
        if (length == 0)
        {
            fout << "test name, user time (seconds), wall time (seconds)" << std::endl;
        }

        fout << d_testName << ", " << result << std::endl;
        fout.close();
    }
};

/*!
\brief
    Generic test on a Window. It will automatically create the specified
    window type and set the renderer on it, so that the rendering phase's logic
    is tested as well (using the NullRenderer - so no actual rendering is done).
*/
template<typename TWindow>
class WindowPerformanceTest : public PerformanceTest
{
public:
    WindowPerformanceTest(CEGUI::String window_type, CEGUI::String renderer) :
        PerformanceTest(window_type),
        d_window(0)
    {
        d_window = static_cast<TWindow*>(
            CEGUI::WindowManager::getSingleton().createWindow(
                window_type, window_type + "-perf-test"));
        d_window->setWindowRenderer(renderer);
    }

    virtual void render()
    {
        d_window->render();
    }

    TWindow* d_window;
};

/*!
\brief
    This represents a generic test made on list widgets. It adds items, deletes
    some, then inserts items in the middle, and finally sorts them.

    To add a new list test, you just inherit from this class, and implement the
    required methods that do the actual item addition/removal/etc
*/
template<typename TWindow>
class BaseListPerformanceTest : public WindowPerformanceTest<TWindow>
{
public:
    BaseListPerformanceTest(CEGUI::String windowType, CEGUI::String renderer) :
        WindowPerformanceTest<TWindow>(windowType, renderer)
    {
    }

    virtual void doTest()
    {
        addItemsWithRender(500);

        clearItems();
        this->render();

        addItemsWithRender(1000);

        deleteFirstItems(150);
        this->render();

        for (size_t step = 0; step < 17; ++step)
        {
            deleteFirstItems(3);
            this->render();
        }

        deleteLastItems(123);
        this->render();

        clearItems();
        this->render();

        addItemsWithRender(100);

        for (size_t i = 0; i < 50; ++i)
        {
            addItems(1, 50);
            this->render();
        }

        sortItems();
        this->render();
    }

    //! Adds the specified number of items, rendering after each addition.
    virtual void addItemsWithRender(size_t count)
    {
        for (size_t i = 0; i < count; ++i)
        {
            addItems(1);
            this->render();
        }
    }

    virtual void clearItems() = 0;
    virtual void addItems(size_t count) = 0;
    virtual void addItems(size_t count, size_t at_position) = 0;
    virtual void deleteFirstItems(size_t count) = 0;
    virtual void deleteLastItems(size_t count) = 0;
    virtual void sortItems() = 0;
};

#endif
