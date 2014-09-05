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

#include "CEGUI/NamedElement.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(NamedElement)

BOOST_AUTO_TEST_CASE(Children)
{
    CEGUI::NamedElement* root = new CEGUI::NamedElement("root");
    BOOST_CHECK_EQUAL(root->getName(), "root");
    BOOST_CHECK_EQUAL(root->getProperty("Name"), "root");
    CEGUI::NamedElement* child1 = new CEGUI::NamedElement("child");
    CEGUI::NamedElement* child2 = new CEGUI::NamedElement("child");
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
    CEGUI::NamedElement* root = new CEGUI::NamedElement("root");
    CEGUI::NamedElement* child = new CEGUI::NamedElement("child");
    CEGUI::NamedElement* inner_child = new CEGUI::NamedElement("inner_child");
    root->addChild(child);
    child->addChild(inner_child);

    BOOST_CHECK_EQUAL(child->getNamePath(), "root/child");
    BOOST_CHECK_EQUAL(child->getProperty("NamePath"), "root/child");
    BOOST_CHECK_EQUAL(inner_child->getNamePath(), "root/child/inner_child");

    BOOST_CHECK_EQUAL(root->getChildElement("child"), child);
    BOOST_CHECK_EQUAL(root->getChildElement("child/inner_child"), inner_child);
    BOOST_CHECK_EQUAL(child->getChildElement("inner_child"), inner_child);
    BOOST_CHECK_THROW(child->getChildElement("nonexistant"), CEGUI::UnknownObjectException);

    delete inner_child;
    delete child;
    delete root;
}

BOOST_AUTO_TEST_CASE(FindRecursive)
{
    CEGUI::NamedElement* root = new CEGUI::NamedElement("root");
    CEGUI::NamedElement* child = new CEGUI::NamedElement("child");
    CEGUI::NamedElement* inner_child = new CEGUI::NamedElement("inner_child");
    root->addChild(child);
    child->addChild(inner_child);

    BOOST_CHECK_EQUAL(child, root->getChildElementRecursive("child"));
    BOOST_CHECK_EQUAL(inner_child, root->getChildElementRecursive("inner_child"));
    BOOST_CHECK_EQUAL(inner_child, child->getChildElementRecursive("inner_child"));

    BOOST_CHECK(0 == root->getChildElementRecursive("ChIlD")); // case sensitive
    BOOST_CHECK(0 == root->getChildElementRecursive("InNeR_ChIlD"));
    BOOST_CHECK(0 == child->getChildElementRecursive("InNeR_ChIlD"));

    BOOST_CHECK(0 == root->getChildElementRecursive("blah")); // blah-tantly wrong

    delete inner_child;
    delete child;
    delete root;
}

BOOST_AUTO_TEST_SUITE_END()
