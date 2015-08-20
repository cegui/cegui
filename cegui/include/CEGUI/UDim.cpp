/***********************************************************************
    created:    20th August 2015
    author:     Lukas Meindl

    purpose:    Implements UDim class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2015 Paul D Turner & The CEGUI Development Team
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
#include "UDim.h"

#include "CEGUI/PropertyHelper.h"
#include "CEGUI/StreamHelper.h"

namespace CEGUI
{

std::istream& operator>>(std::istream& s, UDim& val)
{
    // Format is: " { %g , %g } " but we are lenient regarding the format, so this is also allowed: " %g %g "
    s >> optionalChar<'{'> >> val.d_scale >> optionalChar<','> >> val.d_offset >> optionalChar<'}'>;
    return s;
}

std::istream& operator>>(std::istream& inStream, UBox& val)
{
    // Format is:  { top: { %g , %g } , left: { %g , %g } , bottom: { %g , %g } , right: { %g , %g } }",
    // but we are lenient regarding the format, so this is also allowed: " top : { %g %g } left : { %g %g } bottom : { %g %g } right : { %g %g } "
    inStream >> optionalChar<'{'> >> MandatoryString(" top : {") >> val.d_top.d_scale >> optionalChar<','> >> val.d_top.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> MandatoryString(" left : {") >> val.d_left.d_scale >> optionalChar<','> >> val.d_left.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> MandatoryString(" bottom : {") >> val.d_bottom.d_scale >> optionalChar<','> >> val.d_bottom.d_offset >>
        mandatoryChar<'}'> >> optionalChar<','> >> MandatoryString(" right : {") >> val.d_right.d_scale >> optionalChar<','> >> val.d_right.d_offset;
    return inStream;
}

}