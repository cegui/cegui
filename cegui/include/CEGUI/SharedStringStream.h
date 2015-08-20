/***********************************************************************
	created:	16th August 2015
	author:		Lukas Meindl
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
#ifndef _SharedStringStream_h_
#define _SharedStringStream_h_

#include "Base.h"

#include <sstream>

namespace CEGUI
{

class SharedStringstream
{
public:
    SharedStringstream();

 
    /*!
    \brief
        Changes the precision of the stream (the amount of decimal numbers after the decimal
        separator, in case of std::fixed, which is our default)
    */
    void setPrecision(int precision);

    std::stringstream d_sharedStream;
    
    // TODO: make this "static thread_local" in the future once supported on all major compilers,
    // currently only supported on VS 2015 apparently
    static SharedStringstream s_sharedStreamInstance;
};

}


#endif