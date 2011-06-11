/***********************************************************************
    filename:   Entry.cpp
    created:    11/6/2011
    author:     Martin Preisler
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

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CEGUITests

#include <boost/test/unit_test.hpp>
#include <iostream>

#include "RendererModules/Null/CEGUINullRenderer.h"

/**
\brief This fixture sets CEGUI up with NullRenderer
*/
class CEGUIInstanceFixture
{
public:
    CEGUIInstanceFixture()
    {
        // BOOST_TEST_MESSAGE is not available here
        std::cout << "Bringing CEGUI up using NullRenderer" << std::endl;
        std::cout << "************************************" << std::endl;
        std::cout << std::endl;
        
        CEGUI::NullRenderer::bootstrapSystem();
    }
    
    ~CEGUIInstanceFixture()
    {
        // BOOST_TEST_MESSAGE is not available here
        std::cout << std::endl;
        std::cout << "Destroying CEGUI instance" << std::endl;
        
        CEGUI::NullRenderer::destroySystem();
    }
};

BOOST_GLOBAL_FIXTURE( CEGUIInstanceFixture );
