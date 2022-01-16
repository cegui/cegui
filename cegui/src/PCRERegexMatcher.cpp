/***********************************************************************
    created:    Mon Jul 27 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/PCRERegexMatcher.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/PropertyHelper.h"
#include <cstring>

namespace CEGUI
{

//----------------------------------------------------------------------------//
PCRERegexMatcher::~PCRERegexMatcher()
{
    if (d_regex)
        pcre_free(d_regex);
}

//----------------------------------------------------------------------------//
void PCRERegexMatcher::setRegexString(const String& regex)
{
    if (d_regex)
        pcre_free(d_regex);

    d_string.clear();
    // try to compile this new regex string
    const char* prce_error;
    int pcre_erroff;
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    d_regex = pcre_compile(regex.c_str(), PCRE_UTF8,
        &prce_error, &pcre_erroff, 0);
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    d_regex = pcre_compile(String::convertUtf32ToUtf8(regex.getString()).c_str(), PCRE_UTF8,
        &prce_error, &pcre_erroff, nullptr);
#endif

    // handle failure
    if (!d_regex)
        throw InvalidRequestException(
            "Bad RegEx set: '" + regex + "'.  Additional Information: " +
            prce_error);

    // set this last so that upon failure object is in consistent state.
    d_string = regex;
}

//----------------------------------------------------------------------------//
RegexMatchState PCRERegexMatcher::getMatchStateOfString(const String& str) const
{
    // if the regex is not valid, then an exception is thrown
    if (!d_regex)
        throw InvalidRequestException(
            "Attempt to use invalid RegEx '" + d_string + "'.");

    int match[3];
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    const char* utf8_str = str.c_str();
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    std::string utf8StdStr = String::convertUtf32ToUtf8(str.getString());
    const char* utf8_str = utf8StdStr.c_str();
#endif
    const int len = static_cast<int>(std::strlen(utf8_str));

#ifdef PCRE_PARTIAL_SOFT
    // we are using a new version of pcre
    const std::int32_t result = pcre_exec(d_regex, nullptr, utf8_str, len, 0,
                                 PCRE_PARTIAL_SOFT | PCRE_ANCHORED, match, 3);
#else
    // PCRE_PARTIAL is a backwards compatible synonym for PCRE_PARTIAL_SOFT
    // using it is a requirement if we want to support pcre < 8.0

    // Older versions of pcre have problems doing partial matching of
    // single repeated characters if using pcre_exec,
    // It is suggested to use pcre_dfa_exec instead.
    int workspace[100]; // FIXME: persist the workspace between match attempts
    const int result = pcre_dfa_exec(d_regex, 0, utf8_str, len, 0,
                                     PCRE_PARTIAL | PCRE_ANCHORED, match, 3, workspace, 100);
#endif

    if (result == PCRE_ERROR_PARTIAL)
        return RegexMatchState::Partial;

    // a match must be for the entire string
    if (result >= 0)
        return (match[1] - match[0] == len) ? RegexMatchState::Valid : RegexMatchState::Invalid;

    // no match found or if test string or regex is 0
    if (result == PCRE_ERROR_NOMATCH || result == PCRE_ERROR_NULL)
        return RegexMatchState::Invalid;

    // anything else is an error
    throw InvalidRequestException(
        "PCRE Error: " + PropertyHelper<std::int32_t>::toString(result) +
        " occurred while attempting to match the RegEx '" +
        d_string + "'.");
}

}
