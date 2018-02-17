/***********************************************************************
    created:    dd-mm-yyyy
    author:     name <email>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - YYYY Paul D Turner & The CEGUI Development Team
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
#ifndef _WellNamedSample_h_ // change to your belongings (it is always a good idea to use
#define _WellNamedSample_h_ // the sample name because the name of your sample has to be unique)

#include "SampleBase.h"

namespace CEGUI
{
    // forward declarations (if necessary)
}

// the sample class. needs to inherit from "Sample" class
class WellNamedSample : public Sample
{
public:
    // ctor
    WellNamedSample();

    // override member to initialise the sample.
    virtual bool initialise(CEGUI::GUIContext* guiContext);
    // override member to perform cleanup.
    virtual void deinitialise();

private:
    CEGUI::GUIContext* d_guiContext;
};

#endif  // end of guard _Prototype_h_

