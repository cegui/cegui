/***********************************************************************
    created:    Sun Feb 24 2019
    author:     Metora Wang
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2019 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIStdRegexMatcher_h_
#define _CEGUIStdRegexMatcher_h_

#include "CEGUI/RegexMatcher.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

#if (__cplusplus >= 201103L) || (_MSC_VER >= 1600)
#include <regex>
#else
#include <tr1/regex>
namespace std { using namespace tr1; }
#endif

namespace CEGUI
{

//! Implementation of RegexMatcher using std::regex
class CEGUIEXPORT StdRegexMatcher : public RegexMatcher
{
public:

    virtual void setRegexString(const String& regex) override;
    virtual const String& getRegexString() const override;
    virtual RegexMatchState getMatchStateOfString(const String& str) const override;

private:

    //! Copy of the regex string assigned.
    String d_string;
    //! Pointer to std::regex compiled RegEx.
    std::regex d_regex;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif
