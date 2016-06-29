/***********************************************************************
    created:    Sat Feb 18 2012
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUISimpleTimer_h_
#define _CEGUISimpleTimer_h_

#include "CEGUI/Base.h"

#include <chrono>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
//! Simple timer class that returns the elapsed time since the last start in seconds.
class CEGUIEXPORT SimpleTimer
{
public:
#if ! (defined(_MSC_VER) && (_MSC_VER == 1800))
    typedef std::chrono::time_point<std::chrono::steady_clock> timePoint;
#else
    // Once again, Visual Studio 2013 does the wrong thing regarding C++11
    typedef std::chrono::time_point<std::chrono::system_clock> timePoint;
#endif

    SimpleTimer() : d_lastStartTime(std::chrono::steady_clock::now()) {}

    /*!
    \brief Restarts the timer, setting it to the current time.
    */
    void restart() { d_lastStartTime = std::chrono::steady_clock::now(); }

    /*!
    \brief Returns the elapsed time in seconds.
    */
    double elapsedTime() const
    {
        std::chrono::duration<double> elapsedTime = std::chrono::steady_clock::now() - d_lastStartTime;
        return elapsedTime.count();
    }
private:
    timePoint d_lastStartTime;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif

