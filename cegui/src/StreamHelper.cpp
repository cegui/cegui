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

#include "CEGUI/StreamHelper.h"

#include <istream>
#include <locale>


namespace CEGUI
{

MandatoryString::MandatoryString(char const* mandatoryString)
    : m_chars(mandatoryString)
{}

std::istream& operator>> (std::istream& inputStream, const MandatoryString& mandatoryString) 
{
    //  If the input stream already has a fail/bad bit, we avoid overwriting the error message by doing nothing
    if (inputStream.fail())
        return inputStream;
     
    char const* currentMandatoryChar = mandatoryString.m_chars;

    while (*currentMandatoryChar != '\0')
    {
        static const std::locale spaceLocale("C");

        if (std::isspace(*currentMandatoryChar, spaceLocale))
        {
            // Extract all whitespaces until there is a non-whitespace character
            inputStream >> std::ws;
        }
        else
        {
            // Extract the next character
            int peekedChar = inputStream.get();
            // Check for mismatch
            if (peekedChar != *currentMandatoryChar)
            {
                inputStream.setstate(std::ios::failbit); 
                break;
            }
        }

        ++currentMandatoryChar;
    }
    
    return inputStream;
}


}

