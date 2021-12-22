/***********************************************************************
    created:    15th August 2015
    author:     Lukas Meindl
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
#ifndef _StreamHelper_h_
#define _StreamHelper_h_

#include "CEGUI/Base.h"
#include <istream>

namespace CEGUI
{

/*!
\brief
    Helper method to skip over an optional single character in an istream. First, this method discards leading whitespaces,
    then it skips over the provided optional character, in the case that it matches the subsequent character in the stream.
    Usage, for example: strStream >> optionalChar<'x'>;
*/
template<char matchingCharacter>
std::istream& optionalChar(std::istream& inputStream)
{
    if (inputStream.fail())
        return inputStream;

    // Discard whitespaces, this is necessary because unformatted input functions like peek(), read() and so on
    // are not affected by the skipws/noskipws flag, only formatted output is affected.
    inputStream >> std::ws;
    // Peek into the next character, if it is equal to the skipping-character, extract/ignore this character
    if (inputStream.peek() == matchingCharacter)
        inputStream.ignore();
    else
        // If peek is executed but no further characters remain, the failbit will be set, we want to undo this
        inputStream.clear(inputStream.rdstate() & ~std::ios::failbit);

    return inputStream;
}


/*!
\brief
    Helper method to check for a match with a mandatory character in an istream.
    First, it discards leading whitespaces, then it  checks if the subsequent
    character of the stream matches the provided one. If so, then the character is
    extracted/ignored, else we set the fail bit for this stream.
    Usage, for example: strStream >> mandatoryChar<'x'>;
*/
template<char matchingCharacter>
std::istream& mandatoryChar(std::istream& inputStream)
{
    if (inputStream.fail())
        return inputStream;

    // Discard whitespaces
    inputStream >> std::ws;
    // Peek into the next character, if it matches the mandatory character we extract/ignore
    // the character of the stream, else we set the fail bit.
    if (inputStream.peek() == matchingCharacter)
        inputStream.ignore();
    else
        inputStream.setstate(std::ios_base::failbit);
        
    return inputStream;
}


/*!
\brief
    A class that can be used to match and extract a mandatory sequence of characters from a string
    using the stream operator ">>". If a space is supplied in the string supplied through the
    constructor, all whitespaces will be discarded starting at the current position in the stream
    until a non-whitespace character is found. If a character mismatch is encountered, the fail bit
    will be set for the input stream.
    Usage for example: strStream >> MandatoryString("mustMatch");
*/
class CEGUIEXPORT MandatoryString
{
public:
    /*!
    \brief
        Constructor for the class that can be used to match and extract a mandatory sequence of
        characters from a string using the stream operator ">>". If a space is supplied in the
        string supplied through the constructor, all whitespaces will be discarded starting at the
        current position in the stream until a non-whitespace character is found. If a character
        mismatch is encountered, the fail bit will be set for the input stream.
        Usage for example: strStream >> MandatoryString("mustMatch");
    */
    MandatoryString(char const* mandatoryString);

    CEGUIEXPORT friend std::istream& operator>> (std::istream& inputStream,
                                                 const MandatoryString& mandatoryString);  

private:
    //! Mandatory characters, as provided in the constructor
    char const* m_chars;
};

}

#endif
