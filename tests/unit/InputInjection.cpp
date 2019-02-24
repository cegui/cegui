/***********************************************************************
 *    created:    21/5/2013
 *    author:     Timotei Dolean
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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

#include <boost/test/unit_test.hpp>

#include <vector>
#include <iostream>

#include "CEGUI/CEGUI.h"

using namespace CEGUI;

struct InputInjectionFixture
{
    InputInjectionFixture() :
        d_buttonHandledCount(0),
        d_windowHandledCount(0),
        d_guiContext(&System::getSingleton().getDefaultGUIContext()),
        d_inputAggregator(new InputAggregator(d_guiContext))
    {
        d_inputAggregator->initialise();
        // reset the cursor to (0,0)
        d_guiContext->getCursor().setPosition(glm::vec2(0, 0));

        System::getSingleton().notifyDisplaySizeChanged(Sizef(100, 100));
        Rectf constraint_area(0, 0, 100, 100);
        d_guiContext->getCursor().setConstraintArea(&constraint_area);

        d_window = WindowManager::getSingleton().createWindow("DefaultWindow");
        d_window->setPosition(UVector2(cegui_reldim(0.0f), cegui_reldim(0.0f)));
        d_window->setSize(USize(cegui_reldim(1.0f), cegui_reldim(1.0f)));

        d_button = WindowManager::getSingleton().createWindow("CEGUI/PushButton");
        d_button->setPosition(UVector2(cegui_reldim(0.2f), cegui_reldim(0.2f)));
        d_button->setSize(USize(cegui_reldim(0.2f), cegui_reldim(0.2f)));

        d_editbox = static_cast<Editbox*>(WindowManager::getSingleton().createWindow("TaharezLook/Editbox"));
        d_editbox->setPosition(UVector2(cegui_reldim(0.9f), cegui_reldim(0.9f)));
        d_editbox->setSize(USize(cegui_reldim(0.1f), cegui_reldim(0.1f)));

        d_window->addChild(d_editbox);
        d_window->addChild(d_button);

        System::getSingleton().getDefaultGUIContext().setRootWindow(d_window);

        d_windowConnections.push_back(
            d_window->subscribeEvent(Window::EventCursorActivate,
                Event::Subscriber(&InputInjectionFixture::handleWindowEvent, this))
            );
        d_buttonConnections.push_back(
            d_button->subscribeEvent(PushButton::EventClicked,
                Event::Subscriber(&InputInjectionFixture::handleButtonEvent, this))
            );
    }

    ~InputInjectionFixture()
    {
        disconnectConnections(d_windowConnections);
        disconnectConnections(d_buttonConnections);

        delete d_inputAggregator;

        System::getSingleton().getDefaultGUIContext().setRootWindow(nullptr);

        WindowManager::getSingleton().destroyWindow(d_window);
    }

    bool handleButtonEvent(const EventArgs&)
    {
        ++d_buttonHandledCount;
        return true;
    }

    bool handleWindowEvent(const EventArgs&)
    {
        ++d_windowHandledCount;
        return true;
    }

    void disconnectConnections(std::vector<Event::Connection>& connections)
    {
        for(std::vector<Event::Connection>::iterator itor = connections.begin();
            itor != connections.end(); ++itor)
        {
            (*itor)->disconnect();
        }
    }

    int d_buttonHandledCount;
    int d_windowHandledCount;

    Window* d_window;
    Window* d_button;
    Editbox* d_editbox;

    GUIContext* d_guiContext;
    InputAggregator* d_inputAggregator;

    std::vector<Event::Connection> d_windowConnections;
    std::vector<Event::Connection> d_buttonConnections;
};

//----------------------------------------------------------------------------//
static void doClick(InputAggregator* input_aggregator, float position_x, float position_y)
{
    BOOST_REQUIRE_EQUAL(input_aggregator->injectMousePosition(position_x, position_y), true);
    BOOST_REQUIRE_EQUAL(input_aggregator->injectMouseButtonDown(MouseButton::Left), true);
    BOOST_REQUIRE_EQUAL(input_aggregator->injectMouseButtonUp(MouseButton::Left), true);
}

static void pressKey(InputAggregator* input_aggregator, Key::Scan key)
{
    BOOST_REQUIRE_EQUAL(input_aggregator->injectKeyDown(key), true);
    BOOST_REQUIRE_EQUAL(input_aggregator->injectKeyUp(key), true);
}

//----------------------------------------------------------------------------//
BOOST_FIXTURE_TEST_SUITE(InputInjection, InputInjectionFixture)

BOOST_AUTO_TEST_CASE(OneClickOnWindow)
{
    // we check for both: a) being handled, b) have the expected final result
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectMouseButtonClick(MouseButton::Left), true);

    BOOST_REQUIRE_EQUAL(d_windowHandledCount, 1);
    BOOST_REQUIRE_EQUAL(d_buttonHandledCount, 0);
}

BOOST_AUTO_TEST_CASE(MultipleClicksOnWindow)
{
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectMouseButtonClick(MouseButton::Left), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectMouseButtonClick(MouseButton::Left), true);

    BOOST_REQUIRE_EQUAL(d_windowHandledCount, 2);
    BOOST_REQUIRE_EQUAL(d_buttonHandledCount, 0);
}

BOOST_AUTO_TEST_CASE(OneClickOnButton)
{
    doClick(d_inputAggregator, 30.0f, 30.0f);

    BOOST_REQUIRE_EQUAL(d_windowHandledCount, 0);
    BOOST_REQUIRE_EQUAL(d_buttonHandledCount, 1);
}

BOOST_AUTO_TEST_CASE(InsertSimpleText)
{
    // focus the editbox
    doClick(d_inputAggregator, 91.0f, 91.0f);

    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('W'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('o'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('W'), true);

    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "WoW");
}

BOOST_AUTO_TEST_CASE(DeleteTextWithBackspace)
{
    // focus the editbox
    doClick(d_inputAggregator, 91.0f, 91.0f);

    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('W'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('o'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('k'), true);
    pressKey(d_inputAggregator, Key::Scan::Backspace);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('W'), true);

    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "WoW");
}

BOOST_AUTO_TEST_CASE(DeleteTextWithDelete)
{
    // focus the editbox
    doClick(d_inputAggregator, 91.0f, 91.0f);

    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('W'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('o'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('k'), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectChar('W'), true);
    pressKey(d_inputAggregator, Key::Scan::ArrowLeft);
    pressKey(d_inputAggregator, Key::Scan::ArrowLeft);
    pressKey(d_inputAggregator, Key::Scan::DeleteKey);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectKeyDown(Key::Scan::DeleteKey), true);

    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "WoW");
}

BOOST_AUTO_TEST_CASE(MouseSelectAllTextCopyAndPaste)
{
    // focus the editbox
    doClick(d_inputAggregator, 91.0f, 91.0f);

    d_editbox->setText("WoW rocks");

    // select all text
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectMouseButtonTripleClick(MouseButton::Left), true);
    BOOST_REQUIRE_EQUAL(d_editbox->getSelectionLength(), 9);

   
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectCopyRequest(), true);

    // deselect the text
    d_editbox->setSelection(0, 0);
    
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectPasteRequest(), true);
   
    //CEGUI::String editboxText = d_editbox->getText();
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32
    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "WoW rocksWoW rocks");
#else
    //BOOST_REQUIRE_EQUAL(String::convertUtf32ToUtf8(d_editbox->getText().getString()).c_str(), "WoW rocksWoW rocks");
#endif
}

BOOST_AUTO_TEST_CASE(MouseSelectWordAndDelete)
{
    // focus the editbox
    doClick(d_inputAggregator, 92.0f, 92.0f);

    d_editbox->setText("WoW rocks");

    // select all text
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectMouseButtonDoubleClick(MouseButton::Left), true);
    BOOST_REQUIRE_EQUAL(d_editbox->getSelectionLength(), 4);
    pressKey(d_inputAggregator, Key::Scan::DeleteKey);
    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "rocks");
}

BOOST_AUTO_TEST_CASE(KeyboardSelectCharactersAndDelete)
{
    // focus the editbox
    doClick(d_inputAggregator, 91.0f, 91.0f);

    d_editbox->setText("WoW rocks");

    // select 'WoW'
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectKeyDown(Key::Scan::LeftShift), true);
    pressKey(d_inputAggregator, Key::Scan::ArrowRight);
    pressKey(d_inputAggregator, Key::Scan::ArrowRight);
    pressKey(d_inputAggregator, Key::Scan::ArrowRight);
    pressKey(d_inputAggregator, Key::Scan::ArrowRight);

    BOOST_REQUIRE_EQUAL(d_editbox->getSelectionLength(), 4);
    pressKey(d_inputAggregator, Key::Scan::DeleteKey);
    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "rocks");
}

BOOST_AUTO_TEST_CASE(KeyboardSelectWordAndDelete)
{
    // focus the editbox
    doClick(d_inputAggregator, 91.0f, 91.0f);

    d_editbox->setText("WoW rocks");

    // select 'WoW'
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectKeyDown(Key::Scan::LeftShift), true);
    BOOST_REQUIRE_EQUAL(d_inputAggregator->injectKeyDown(Key::Scan::LeftControl), true);
    pressKey(d_inputAggregator, Key::Scan::ArrowRight);

    BOOST_REQUIRE_EQUAL(d_editbox->getSelectionLength(), 4);
    pressKey(d_inputAggregator, Key::Scan::DeleteKey);
    BOOST_REQUIRE_EQUAL(d_editbox->getText(), "rocks");
}
BOOST_AUTO_TEST_SUITE_END()
