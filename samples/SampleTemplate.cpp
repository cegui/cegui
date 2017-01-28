/***********************************************************************
    created:    20/5/2008
    author:     name <email>
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
#include "SampleTemplate.h"
#include "CEGUI/CEGUI.h"

//----------------------------------------------------------------------------//
WellNamedSample::WellNamedSample()
{
    // add your info here
    // if you don't know what to write have a look
    // into the other samples
    
    // the name of your sample
    Sample::d_name = "";
    // your name goes here
    Sample::d_credits = "";
    // summarise what your sample wants to demonstrate
    Sample::d_summary = "";
    // explain how you achieve this
    Sample::d_description = "";
}

//----------------------------------------------------------------------------//
bool WellNamedSample::initialise(CEGUI::GUIContext* guiContext)
{
    using namespace CEGUI;

    d_guiContext = guiContext;
    
    // add additional files, if you have some
    d_usedFiles = CEGUI::String(__FILE__);

    // add your initialisation code below here
    // e.g. loading layouts, etc.

    // success!
    return true;
}

//----------------------------------------------------------------------------//
void WellNamedSample::deinitialise()
{
    // add your deinitialisation code below (if there's a need for it)
}