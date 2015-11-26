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

#include "CEGUI/Window.h"
#include "CEGUI/WindowManager.h"

#include <boost/test/unit_test.hpp>

/*
 * Used to bring some Windows up for testing
 *
 * This is for exception safety, no matter what happens in the tests,
 * its destructor will be called
 */
struct LayoutSetupFixture
{
    LayoutSetupFixture()
    {
        d_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
        d_root->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
        d_root->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));

        d_insideRoot = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
        d_insideRoot->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 100), CEGUI::UDim(0, 50)));
        d_insideRoot->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.5f, 0)));
        d_root->addChild(d_insideRoot);

        d_insideInsideRoot = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow");
        d_insideInsideRoot->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 100), CEGUI::UDim(0, 50)));
        d_insideInsideRoot->setSize(CEGUI::USize(CEGUI::UDim(0.5f, 0), CEGUI::UDim(0.5f, 0)));
        d_insideRoot->addChild(d_insideInsideRoot);

        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(d_root);
        CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Sizef(800, 600));
    }

    ~LayoutSetupFixture()
    {
        CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(0);

        CEGUI::WindowManager::getSingleton().destroyWindow(d_root);
    }

    CEGUI::Window* d_root;
    CEGUI::Window* d_insideRoot;
    CEGUI::Window* d_insideInsideRoot;
};

BOOST_FIXTURE_TEST_SUITE(Window, LayoutSetupFixture)

BOOST_AUTO_TEST_CASE(Defaults)
{
    /*
     * We check these to ensure we don't change them between releases, as people very likely depend on them!
     */

    BOOST_CHECK(!d_root->isActive());

    BOOST_CHECK(!d_root->isDisabled());
    BOOST_CHECK(!d_root->isEffectiveDisabled());

    BOOST_CHECK(d_root->isVisible());
    BOOST_CHECK(d_root->isEffectiveVisible());
}

BOOST_AUTO_TEST_CASE(PropertyInheritance)
{
    /*
     * Alpha and Disabled state should get "inherited"/propagated to children via effective alpha/disabled
     */

    d_root->setAlpha(0.5f);
    d_insideRoot->setAlpha(0.5f);

    BOOST_CHECK_EQUAL(d_insideInsideRoot->getEffectiveAlpha(), 0.25f);

    d_root->setAlpha(1.0f);
    d_insideRoot->setAlpha(1.0f);

    d_root->setDisabled(true);

    BOOST_CHECK_EQUAL(d_insideRoot->isEffectiveDisabled(), true);
    BOOST_CHECK_EQUAL(d_insideInsideRoot->isEffectiveDisabled(), true);

    d_root->setDisabled(false);

    BOOST_CHECK_EQUAL(d_insideRoot->isEffectiveDisabled(), false);
    BOOST_CHECK_EQUAL(d_insideInsideRoot->isEffectiveDisabled(), false);
}

BOOST_AUTO_TEST_CASE(UnifiedDimensions)
{
    /*
     * Basic relative UDim tests
     */
    BOOST_CHECK_EQUAL(d_insideInsideRoot->getPixelSize(), CEGUI::Sizef(200, 150));
    BOOST_CHECK_EQUAL(d_insideInsideRoot->getUnclippedOuterRect().get(), CEGUI::Rectf(200, 100, 400, 250));
}

BOOST_AUTO_TEST_CASE(HitTesting)
{
    /*
     * We know where the windows are so lets check whether CEGUI reports correct hits for them
     */

    BOOST_CHECK(d_insideInsideRoot->isHit(CEGUI::Vector2f(300, 150)));

    d_insideInsideRoot->setDisabled(true);
    BOOST_CHECK(!d_insideInsideRoot->isHit(CEGUI::Vector2f(300, 150), false));
    BOOST_CHECK(d_insideInsideRoot->isHit(CEGUI::Vector2f(300, 150), true));
    d_insideInsideRoot->setDisabled(false);

    d_root->setDisabled(true);
    BOOST_CHECK(!d_insideInsideRoot->isHit(CEGUI::Vector2f(300, 150), false));
    BOOST_CHECK(d_insideInsideRoot->isHit(CEGUI::Vector2f(300, 150), true));
    d_root->setDisabled(false);
}

BOOST_AUTO_TEST_CASE(Hierarchy)
{
    CEGUI::Window* child = d_insideInsideRoot->createChild("DefaultWindow");
    BOOST_CHECK(d_insideInsideRoot->isChild(child));
    d_insideInsideRoot->destroyChild(child);

    child = d_insideInsideRoot->createChild("DefaultWindow");
    child->setDestroyedByParent(false);
    d_insideInsideRoot->removeChild(child);
    BOOST_CHECK(!d_insideInsideRoot->isChild(child));
    CEGUI::WindowManager::getSingleton().destroyWindow(child);
}

BOOST_AUTO_TEST_CASE(RecursiveSearch)
{
    int previousID[3];
    previousID[0] = d_root->getID();
    previousID[1] = d_insideRoot->getID();
    previousID[2] = d_insideInsideRoot->getID();

    d_root->setID(2);
    d_insideRoot->setID(3);
    d_insideInsideRoot->setID(5);

    BOOST_CHECK_EQUAL(d_insideInsideRoot, d_root->getChildRecursive(5)); // finding the correct window
    BOOST_CHECK_EQUAL(d_insideRoot, d_root->getChildRecursive(3));
    BOOST_CHECK_EQUAL(d_insideInsideRoot, d_insideRoot->getChildRecursive(5));

    BOOST_CHECK(0 == d_root->getChildRecursive(10)); // not finding these
    BOOST_CHECK(0 == d_root->getChildRecursive(6));
    BOOST_CHECK(0 == d_insideRoot->getChildRecursive(10));

    d_root->setID(previousID[0]);
    d_insideRoot->setID(previousID[1]);
    d_insideInsideRoot->setID(previousID[2]);
}

BOOST_AUTO_TEST_SUITE_END()
