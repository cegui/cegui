/***********************************************************************
 *    filename:   Element.cpp
 *    created:    28/10/2011
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

#include "CEGUI/Element.h"

#include <boost/test/unit_test.hpp>
#include <boost/timer.hpp>

BOOST_AUTO_TEST_SUITE(Element)

BOOST_AUTO_TEST_CASE(RelativeSizing)
{
    // in this test, everything is top left aligned
    
    CEGUI::Element* root = new CEGUI::Element();
    root->setSize(CEGUI::USize(100.0f * CEGUI::UDim::px(), 100 * CEGUI::UDim::px()));
    CEGUI::Element* child = new CEGUI::Element();
    child->setSize(CEGUI::USize(50.0f * CEGUI::UDim::percent(), 25.0f * CEGUI::UDim::percent()));
    root->addChild(child);
    
    BOOST_CHECK_EQUAL(root->getPixelSize(), CEGUI::Sizef(100.0f, 100.0f));
    BOOST_CHECK_EQUAL(child->getPixelSize(), CEGUI::Sizef(50.0f, 25.0f));
    
    CEGUI::Element* innerChild = new CEGUI::Element();
    child->addChild(innerChild);
    innerChild->setSize(CEGUI::USize(200.0f * CEGUI::UDim::percent(), 100.0f * CEGUI::UDim::percent()));
    BOOST_CHECK_EQUAL(innerChild->getPixelSize(), CEGUI::Sizef(100.0f, 25.0f));
    
    delete innerChild;
    delete child;
    delete root;
}

BOOST_AUTO_TEST_CASE(TopLeftAlignment)
{
    // in this test, everything is top left aligned
    
    CEGUI::Element* root = new CEGUI::Element();
    root->setSize(CEGUI::USize(100.0f * CEGUI::UDim::px(), 100 * CEGUI::UDim::px()));
    CEGUI::Element* child = new CEGUI::Element();
    child->setSize(CEGUI::USize(50.0f * CEGUI::UDim::px(), 25.0f * CEGUI::UDim::px()));
    root->addChild(child);
    
    // even though it is the default at the point of writing the test, we have to make sure this fact doesn't change!
    BOOST_CHECK_EQUAL(root->getHorizontalAlignment(), CEGUI::HA_LEFT);
    BOOST_CHECK_EQUAL(root->getVerticalAlignment(), CEGUI::VA_TOP);
    BOOST_CHECK_EQUAL(child->getHorizontalAlignment(), CEGUI::HA_LEFT);
    BOOST_CHECK_EQUAL(child->getVerticalAlignment(), CEGUI::VA_TOP);
    
    BOOST_CHECK_EQUAL(root->getUnclippedOuterRect().get(), CEGUI::Rectf(0, 0, 100, 100));
    BOOST_CHECK_EQUAL(child->getUnclippedOuterRect().get(), CEGUI::Rectf(0, 0, 50, 25));
    
    child->setPosition(CEGUI::UVector2(10.0f * CEGUI::UDim::px(), 5.0f * CEGUI::UDim::px()));
    
    BOOST_CHECK_EQUAL(child->getUnclippedOuterRect().get(), CEGUI::Rectf(10, 5, 60, 30));
}

BOOST_AUTO_TEST_SUITE_END()
