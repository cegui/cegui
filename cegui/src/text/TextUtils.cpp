/***********************************************************************
    created:    30/5/2004
    author:     Paul D Turner

    purpose:    Implementation of text support class
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/text/TextUtils.h"

namespace CEGUI
{
/*************************************************************************
    Constants
*************************************************************************/
const String    TextUtils::DefaultWhitespace(" \n\t\r");
const String    TextUtils::DefaultAlphanumerical("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890");
const String    TextUtils::DefaultWrapDelimiters(" \n\t\r");

/*************************************************************************
    return a String containing the the next word in a String.
*************************************************************************/
String TextUtils::getNextWord(const String& str, String::size_type start_idx, const String& delimiters)
{
    String::size_type   word_start = str.find_first_not_of(delimiters, start_idx);

    if (word_start == String::npos)
    {
        word_start = start_idx;
    }

    String::size_type   word_end = str.find_first_of(delimiters, word_start);

    if (word_end == String::npos)
    {
        word_end = str.length();
    }

    return str.substr(start_idx, (word_end - start_idx));
}

//----------------------------------------------------------------------------//
String::size_type TextUtils::getWordStartIndex(const String& str, String::size_type idx)
{
    if (!idx)
        return 0;

    // Find last non-delimiter before idx
    String::size_type trimIdx = idx - 1;
    for (; trimIdx > 0; --trimIdx)
        if (String::npos == DefaultWhitespace.find(str[trimIdx]))
            break;

    // Single character line has only one valid index anyway
    if (!trimIdx)
        return 0;

    // FIXME: write manual iteration or use string view instead of copying
    const String temp = str.substr(0, trimIdx);

    if (String::npos != DefaultAlphanumerical.find(temp.back()))
        idx = temp.find_last_not_of(DefaultAlphanumerical);
    else
        idx = temp.find_last_of(DefaultAlphanumerical + DefaultWhitespace);

    return (idx == String::npos) ? 0 : idx + 1;
}

//----------------------------------------------------------------------------//
String::size_type TextUtils::getWordEndIndex(const String& str, String::size_type idx)
{
    const auto strLen = str.size();
    if (idx >= strLen)
        return strLen;

    if (String::npos != DefaultAlphanumerical.find(str[idx]))
        idx = str.find_first_not_of(DefaultAlphanumerical, idx);
    else if (String::npos == DefaultWhitespace.find(str[idx]))
        idx = str.find_first_of(DefaultAlphanumerical + DefaultWhitespace, idx);

    return (String::npos == idx) ? strLen : idx;
}

//----------------------------------------------------------------------------//
String::size_type TextUtils::getNextWordStartIndex(const String& str, String::size_type idx)
{
    const auto wordEndIdx = getWordEndIndex(str, idx);

    const auto strLen = str.size();
    if (wordEndIdx >= strLen)
        return strLen;

    if (String::npos == DefaultWhitespace.find(str[wordEndIdx]))
        return wordEndIdx;

    idx = str.find_first_not_of(DefaultWhitespace, wordEndIdx);
    return (String::npos == idx) ? strLen : idx;
}


/*************************************************************************
    Trim all characters from the set specified in \a chars from the
    beginning of 'str'.
*************************************************************************/
void TextUtils::trimLeadingChars(String& str, const String& chars)
{
    String::size_type idx = str.find_first_not_of(chars);

    if (idx != String::npos)
    {
        str.erase(0, idx);
    }
    else
    {
        str.erase();
    }

}


/*************************************************************************
    Trim all characters from the set specified in \a chars from the end
    of 'str'.
*************************************************************************/
void TextUtils::trimTrailingChars(String& str, const String& chars)
{
    const auto idx = str.find_last_not_of(chars);
    if (idx != String::npos)
        str.resize(idx + 1);
    else
        str.erase();
}

}
