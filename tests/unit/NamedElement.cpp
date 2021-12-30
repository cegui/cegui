/***********************************************************************
 *    created:    30/10/2011
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

BOOST_AUTO_TEST_SUITE(NamedElement)

BOOST_AUTO_TEST_CASE(Children)
{
    CEGUI::Window* root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
    BOOST_CHECK_EQUAL(root->getName(), "root");
    BOOST_CHECK_EQUAL(root->getProperty("Name"), "root");
    CEGUI::Window* child1 = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "child");
    CEGUI::Window* child2 = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "child");
    root->addChild(child1);
    BOOST_CHECK_THROW(root->addChild(child2), CEGUI::AlreadyExistsException);
    root->removeChild("child");
    BOOST_CHECK_THROW(root->removeChild("child"), CEGUI::UnknownObjectException);
    root->addChild(child2);
    BOOST_CHECK_THROW(root->addChild(child1), CEGUI::AlreadyExistsException);

    delete child2;
    delete child1;
    delete root;
}

BOOST_AUTO_TEST_CASE(NamePath)
{
    CEGUI::Window* root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
    CEGUI::Window* child = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "child");
    CEGUI::Window* inner_child = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "inner_child");
    root->addChild(child);
    child->addChild(inner_child);

    BOOST_CHECK_EQUAL(child->getNamePath(), "root/child");
    BOOST_CHECK_EQUAL(child->getProperty("NamePath"), "root/child");
    BOOST_CHECK_EQUAL(inner_child->getNamePath(), "root/child/inner_child");

    BOOST_CHECK_EQUAL(root->getChild("child"), child);
    BOOST_CHECK_EQUAL(root->getChild("child/inner_child"), inner_child);
    BOOST_CHECK_EQUAL(child->getChild("inner_child"), inner_child);
    BOOST_CHECK_THROW(child->getChild("nonexistant"), CEGUI::UnknownObjectException);

    delete inner_child;
    delete child;
    delete root;
}

BOOST_AUTO_TEST_CASE(FindRecursive)
{
    CEGUI::Window* root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
    CEGUI::Window* child = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "child");
    CEGUI::Window* inner_child = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "inner_child");
    root->addChild(child);
    child->addChild(inner_child);

    BOOST_CHECK_EQUAL(child, root->getChildRecursive("child"));
    BOOST_CHECK_EQUAL(inner_child, root->getChildRecursive("inner_child"));
    BOOST_CHECK_EQUAL(inner_child, child->getChildRecursive("inner_child"));

    BOOST_CHECK(0 == root->getChildRecursive("ChIlD")); // case sensitive
    BOOST_CHECK(0 == root->getChildRecursive("InNeR_ChIlD"));
    BOOST_CHECK(0 == child->getChildRecursive("InNeR_ChIlD"));

    BOOST_CHECK(0 == root->getChildRecursive("blah")); // blah-tantly wrong

    delete inner_child;
    delete child;
    delete root;
}

BOOST_AUTO_TEST_SUITE_END()
