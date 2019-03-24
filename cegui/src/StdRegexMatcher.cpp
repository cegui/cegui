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
#include "CEGUI/StdRegexMatcher.h"
#include "CEGUI/Exceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
StdRegexMatcher::StdRegexMatcher()
{
}

//----------------------------------------------------------------------------//
StdRegexMatcher::~StdRegexMatcher()
{
}

//----------------------------------------------------------------------------//
void StdRegexMatcher::setRegexString(const String& regex)
{
    d_regex = std::regex(std::string(regex.c_str()));
    d_string = regex;
}

//----------------------------------------------------------------------------//
const String& StdRegexMatcher::getRegexString() const
{
    return d_string;
}

//----------------------------------------------------------------------------//
bool StdRegexMatcher::matchRegex(const String& str) const
{
    std::smatch smatch;
    std::string temp = str.c_str();
    if (std::regex_match(temp, smatch, d_regex) == false)
        return false;
    return smatch.empty() == false;
}

//----------------------------------------------------------------------------//
RegexMatcher::MatchState StdRegexMatcher::getMatchStateOfString(const String& str) const
{
    if (matchRegex(str))
        return MS_VALID;
    return MS_INVALID;
}

//----------------------------------------------------------------------------//
} // End of  CEGUI namespace section
