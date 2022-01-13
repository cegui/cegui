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

namespace CEGUI
{

//----------------------------------------------------------------------------//
void StdRegexMatcher::setRegexString(const String& regex)
{
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8
    const std::string& regex8bit = regex.getString();
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    std::string regex8bit = String::convertUtf32ToUtf8(regex.getString());
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII
    const std::string& regex8bit = regex;
#endif
    d_regex = std::regex(regex8bit);
    d_string = regex;
}

//----------------------------------------------------------------------------//
const String& StdRegexMatcher::getRegexString() const
{
    return d_string;
}

//----------------------------------------------------------------------------//
RegexMatchState StdRegexMatcher::getMatchStateOfString(const String& str) const
{
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8
    const std::string& str8bit = str.getString();
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    std::string str8bit = String::convertUtf32ToUtf8(str.getString());
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII
    const std::string& str8bit = str;
#endif

    // There is no partial regex matching in std yet.
    // TODO: try to use std::smatch to detect partial match?
    std::smatch smatch;
    const bool match = std::regex_match(str8bit, smatch, d_regex) && !smatch.empty();
    return match ? RegexMatchState::Valid : RegexMatchState::Invalid;
}

}
