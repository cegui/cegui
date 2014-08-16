/***********************************************************************
 *    created:    11/6/2011
 *    author:     Martin Preisler
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/Base.h"

#ifdef CEGUI_HAS_PCRE_REGEX

#include "CEGUI/widgets/Spinner.h"
#include "CEGUI/WindowManager.h"

#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

/*
 * Used to bring some Spinners up for testing
 *
 * This is for exception safety, no matter what happens in the tests,
 * its destructor will be called
 */
struct SpinnerFixture
{
    SpinnerFixture()
    {
        d_spinner = static_cast<CEGUI::Spinner*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Spinner"));
        d_defaultSpinner = static_cast<CEGUI::Spinner*>(CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/Spinner"));
    }

    ~SpinnerFixture()
    {
        CEGUI::WindowManager::getSingleton().destroyWindow(d_defaultSpinner);
        CEGUI::WindowManager::getSingleton().destroyWindow(d_spinner);
    }

    CEGUI::Spinner* d_spinner;
    CEGUI::Spinner* d_defaultSpinner;
};

BOOST_FIXTURE_TEST_SUITE(Spinner, SpinnerFixture)

BOOST_AUTO_TEST_CASE(Defaults)
{
    // we can't use d_spinner because we have no idea what has been done to it
    BOOST_CHECK_EQUAL(d_defaultSpinner->getCurrentValue(), 0);

    BOOST_CHECK_EQUAL(d_defaultSpinner->getMinimumValue(), -32768);
    BOOST_CHECK_EQUAL(d_defaultSpinner->getMaximumValue(), 32767);

    BOOST_CHECK_EQUAL(d_defaultSpinner->getTextInputMode(), CEGUI::Spinner::Integer);
}

BOOST_AUTO_TEST_CASE(MinMax)
{
    d_spinner->setMinimumValue(0.0f);
    BOOST_CHECK_EQUAL(d_spinner->getMinimumValue(), 0.0f);

    d_spinner->setMaximumValue(10.0f);
    BOOST_CHECK_EQUAL(d_spinner->getMaximumValue(), 10.0f);

    d_spinner->setCurrentValue(10.0f);
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), 10.0f);

    d_spinner->setMaximumValue(9.0f);
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), 9.0f);

    d_spinner->setCurrentValue(5.0f);
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), 5.0f);

    d_spinner->setMinimumValue(6.0f);
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), 6.0f);
}

BOOST_AUTO_TEST_CASE(MinMaxStringProperty)
{
    d_spinner->setProperty("MinimumValue", "-123.0");
    BOOST_CHECK_EQUAL(d_spinner->getMinimumValue(), -123.0f);
    BOOST_CHECK_EQUAL(d_spinner->getProperty("MinimumValue"), "-123");

    d_spinner->setProperty("MaximumValue", "123.0");
    BOOST_CHECK_EQUAL(d_spinner->getMaximumValue(), 123.0f);
    BOOST_CHECK_EQUAL(d_spinner->getProperty("MaximumValue"), "123");
}

BOOST_AUTO_TEST_CASE(CurrentValueStringProperty)
{
    d_spinner->setProperty("CurrentValue", "0");
    BOOST_CHECK_EQUAL(d_spinner->getProperty("CurrentValue"), "0");
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), 0);

    d_spinner->setProperty("CurrentValue", "123");
    BOOST_CHECK_EQUAL(d_spinner->getProperty("CurrentValue"), "123");
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), 123);

    d_spinner->setProperty("CurrentValue", "-123");
    BOOST_CHECK_EQUAL(d_spinner->getProperty("CurrentValue"), "-123");
    BOOST_CHECK_EQUAL(d_spinner->getCurrentValue(), -123);
}

BOOST_AUTO_TEST_SUITE_END()

#endif
