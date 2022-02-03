/***********************************************************************
	created:	14/11/2015
	author:		Lukas Meindl
	
	purpose:	Implements String class
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
#include "CEGUI/String.h"
#include "CEGUI/Exceptions.h"

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
#include <istream>
#endif

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)

namespace CEGUI
{
static const String emptyString;
const String& String::GetEmpty()
{
    return emptyString;
}

std::u32string String::convertUtf8ToUtf32(const char* utf8String, const size_t stringLength, std::vector<size_t>* mapping)
{
    if (mapping)
    {
        mapping->clear();
        mapping->reserve(stringLength); // May be excessive
    }

    std::u32string utf32String;

    if (!utf8String || !stringLength)
        return utf32String;

    // Go through every UTF-8 code unit
    size_t currentCharIndex = 0;
    while (currentCharIndex < stringLength)
    {
        const size_t remainingCodeUnits = stringLength - currentCharIndex;
        size_t usedCodeUnits;
        const char32_t utf32CodePoint = getCodePointFromCodeUnits(
            utf8String + currentCharIndex, remainingCodeUnits, usedCodeUnits);

        utf32String.push_back(utf32CodePoint);
        if (mapping)
            mapping->push_back(currentCharIndex);

        currentCharIndex += usedCodeUnits;
    }

    return utf32String;
}

std::string String::convertUtf32ToUtf8(const char32_t* utf32String)
{
    if (utf32String == nullptr)
        return std::string();

    size_t codeUnitCount = std::char_traits<char32_t>::length(utf32String);
    return convertUtf32ToUtf8(utf32String, codeUnitCount);
}

std::string String::convertUtf32ToUtf8(const char32_t* utf32StringStart, const char32_t* utf32StringEnd)
{
    if (utf32StringStart == nullptr)
        return std::string();

    return convertUtf32ToUtf8(utf32StringStart, utf32StringEnd - utf32StringStart);
}

std::string String::convertUtf32ToUtf8(const std::u32string& utf32String)
{
    return convertUtf32ToUtf8(utf32String.data(), utf32String.size());
}

std::string String::convertUtf32ToUtf8(const char32_t utf32Char)
{
    return convertUtf32ToUtf8(&utf32Char, 1);
}

std::string String::convertUtf32ToUtf8(const char32_t* utf32String, const size_t stringLength)
{
    if (utf32String == nullptr)
        return std::string();

    std::string utf8EncodedString;
    
    // Go through every UTF-32 code unit
    for (size_t currentCharIndex = 0; currentCharIndex < stringLength; ++currentCharIndex)
    {
        const char32_t currentCodeUnit = utf32String[currentCharIndex];

        // Check if the UTF-32 code unit can be represented by a single UTF-8 code-unit
        if (currentCodeUnit < 0x80) 
            utf8EncodedString.push_back(static_cast<char>(currentCodeUnit));
        // Check if the UTF-32 code unit can be represented by two UTF-8 code-units
        else if (currentCodeUnit < 0x800)
        {
            utf8EncodedString.push_back(static_cast<char>((currentCodeUnit >> 6)   | 0xC0));
            utf8EncodedString.push_back(static_cast<char>((currentCodeUnit & 0x3F) | 0x80));
        }
        // Check if the UTF-32 code unit can be represented by three UTF-8 code-units
        else if (currentCodeUnit < 0x10000)
        {
            utf8EncodedString.push_back(static_cast<char>((currentCodeUnit  >> 12)         | 0xE0));
            utf8EncodedString.push_back(static_cast<char>(((currentCodeUnit >> 6)  & 0x3F) | 0x80));
            utf8EncodedString.push_back(static_cast<char>((currentCodeUnit         & 0x3F) | 0x80));
        }
        // Otherwise the UTF-32 code unit can only be represented by four UTF-8 code-units
        else
        {
            utf8EncodedString.push_back(static_cast<char>((currentCodeUnit  >> 18)         | 0xF0));
            utf8EncodedString.push_back(static_cast<char>(((currentCodeUnit >> 12) & 0x3F) | 0x80));
            utf8EncodedString.push_back(static_cast<char>(((currentCodeUnit >> 6)  & 0x3F) | 0x80));
            utf8EncodedString.push_back(static_cast<char>((currentCodeUnit         & 0x3F) | 0x80));
        }
    }

    return utf8EncodedString;
}

char32_t String::convertCodePoint(const char firstCodeUnit, const char secondCodeUnit)
{
    char32_t utf32CodePoint     =  ((firstCodeUnit & 0x1F) << 6);
    utf32CodePoint              |= (secondCodeUnit & 0x3F);
    return utf32CodePoint;
}

char32_t String::convertCodePoint(const char firstCodeUnit, const char secondCodeUnit,
                                  const char thirdCodeUnit)
{
    char32_t utf32CodePoint =  ((firstCodeUnit  & 0x0F) << 12);
    utf32CodePoint          |= ((secondCodeUnit & 0x3F) << 6);
    utf32CodePoint          |= (thirdCodeUnit   & 0x3F);
    return utf32CodePoint;
}

char32_t String::convertCodePoint(const char firstCodeUnit, const char secondCodeUnit,
                                  const char thirdCodeUnit, const char fourthCodeUnit)
{
    char32_t utf32CodePoint =  ((firstCodeUnit  & 0x07) << 18);
    utf32CodePoint          |= ((secondCodeUnit & 0x3F) << 12);
    utf32CodePoint          |= ((thirdCodeUnit  & 0x3F) << 6);
    utf32CodePoint          |= (fourthCodeUnit  & 0x3F);
    return utf32CodePoint;
}


bool String::isContinuingUTF8CodeUnit(const char codeUnit)
{
    return (static_cast<unsigned char>(codeUnit) & 0xC0) == 0x80;
}

char32_t String::getCodePointFromCodeUnits(const char* firstCodeUnit,
                                           const size_t remainingCodeUnits)
{
    size_t consumedCodeUnits;
    return getCodePointFromCodeUnits(firstCodeUnit,
                                     remainingCodeUnits,
                                     consumedCodeUnits);
}

char32_t String::getCodePointFromCodeUnits(const char* firstCodeUnit,
                                           const size_t remainingCodeUnits,
                                           size_t& consumedCodeUnits)
{
    /* Number of code units that the code point consists
       of according to the initial code unit */
    size_t codeUnitsInCodePoint = getCodePointSize(*firstCodeUnit);

    checkUtf8CodePointSizeForValidity(codeUnitsInCodePoint,
                                      remainingCodeUnits);
    

    consumedCodeUnits = codeUnitsInCodePoint;
    switch (codeUnitsInCodePoint)
    {
    case 1:
        return static_cast<char32_t>(firstCodeUnit[0]);
    case 2:
        return convertCodePoint(firstCodeUnit[0], firstCodeUnit[1]);
    case 3:
        return convertCodePoint(
            firstCodeUnit[0],
            firstCodeUnit[1],
            firstCodeUnit[2]);
    case 4:
        return convertCodePoint(
            firstCodeUnit[0],
            firstCodeUnit[1],
            firstCodeUnit[2],
            firstCodeUnit[3]);
    default:
    {
        throw CEGUI::UnicodeStringException(
            "String conversion from UTF-8 to UTF-32 failed due to an "
            "invalid amount of bytes specified for the code point.");
    }
    }
}

char32_t String::getCodePointFromCodeUnits(std::string::const_iterator currentCodeUnit,
                                           std::string::const_iterator codeUnitRangeEnd)
{
    size_t consumedCodeUnits;
    return getCodePointFromCodeUnits(currentCodeUnit,
                                     codeUnitRangeEnd,
                                     consumedCodeUnits);
}

char32_t String::getCodePointFromCodeUnits(std::string::const_iterator currentCodeUnit,
                                           std::string::const_iterator codeUnitRangeEnd,
                                           size_t& consumedCodeUnits)
{
    /* Number of code units that the code point consists
       of according to the initial code unit */
    size_t codeUnitsInCodePoint = getCodePointSize(*currentCodeUnit);

    size_t remainingCodeUnits = codeUnitRangeEnd - currentCodeUnit;

    checkUtf8CodePointSizeForValidity(codeUnitsInCodePoint,
                                      remainingCodeUnits);

    const char& firstCodeUnit = *currentCodeUnit;
    
    consumedCodeUnits = codeUnitsInCodePoint;
    switch (codeUnitsInCodePoint)
    {
    case 1:
        return static_cast<char32_t>(firstCodeUnit);
    case 2:
    {
        const char& secondCodeUnit = *(++currentCodeUnit);
        return convertCodePoint(
            firstCodeUnit,
            secondCodeUnit);
    }
    case 3:
    {
        const char& secondCodeUnit = *(++currentCodeUnit);
        const char& thirdCodeUnit = *(++currentCodeUnit);
        return convertCodePoint(
            firstCodeUnit,
            secondCodeUnit,
            thirdCodeUnit);
    }
    case 4:
    {
        const char& secondCodeUnit = *(++currentCodeUnit);
        const char& thirdCodeUnit = *(++currentCodeUnit);
        const char& fourthCodeUnit = *(++currentCodeUnit);
        return convertCodePoint(
            firstCodeUnit,
            secondCodeUnit,
            thirdCodeUnit,
            fourthCodeUnit);
    }
    default:
    {
        throw CEGUI::UnicodeStringException(
            "String conversion from UTF-8 to UTF-32 failed due to an "
            "invalid amount of bytes specified for the code point.");
    }
    }
}

void String::checkUtf8CodePointSizeForValidity(
    const size_t codeUnitsInCodePoint,
    const size_t remainingCodeUnits)
{
    if (codeUnitsInCodePoint == 0)
    {
        throw UnicodeStringException(
            "Invalid initial byte detected for a code point in the string");
    }

    if (codeUnitsInCodePoint > remainingCodeUnits)
    {
        throw UnicodeStringException(
            "String conversion from UTF-8 to UTF-32 failed due to the "
            "start byte not being followed by the expected amount of "
            "continuation bytes");
    }
}

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
bool String::isUtf8StringValid() const
{
    String::codepoint_iterator codePointIter(
        d_string.begin(), d_string.begin(), d_string.end());

    size_t totalCodeUnits = d_string.size();

    while(!codePointIter.isAtEnd())
    {
        size_t codeUnitsInCodePoint = getCodePointSize(*(codePointIter.getCodeUnitIterator()));

        size_t remainingCodeUnits = totalCodeUnits - codePointIter.getCodeUnitIndexFromStart();

        if(codeUnitsInCodePoint > remainingCodeUnits || codeUnitsInCodePoint == 0)
        {
            return false;
        }

        ++codePointIter;   
    }  

    return true;
}

bool String::codepoint_iterator::operator==(const codepoint_iterator& rhs) const
{
    if (m_rangeStart != rhs.m_rangeEnd || m_rangeEnd != rhs.m_rangeEnd)
    {
        throw UnicodeStringException(
            "The code point iterators to be compared are defined "
            "within different ranges");
    }
    return (m_iter == rhs.m_iter);
}
#endif

bool operator==(const String& str1, const String& str2)
{
    return (str1.compare(str2) == 0);
}

bool operator==(const String& str, const std::string& std_str)
{
    return (str.compare(std_str) == 0);
}

bool operator==(const String& str, const std::u32string& std_u32str)
{
    return (str.compare(std_u32str) == 0);
}

bool operator==(const std::string& std_str, const String& str)
{
    return (str.compare(std_str) == 0);
}

bool operator==(const std::u32string& std_u32str, const String& str)
{
    return (str.compare(std_u32str) == 0);
}

bool operator!=(const String& str1, const String& str2)
{
    return (str1.compare(str2) != 0);
}

bool operator!=(const String& str, const std::string& std_str)
{
    return (str.compare(std_str) != 0);
}

bool operator!=(const String& str, const std::u32string& std_u32str)
{
    return (str.compare(std_u32str) != 0);
}

bool operator!=(const std::string& std_str, const String& str)
{
    return (str.compare(std_str) != 0);
}

bool operator!=(const std::u32string& std_u32str, const String& str)
{
    return (str.compare(std_u32str) != 0);
}

bool operator==(const char32_t* lhs, const String& rhs)
{
    return (rhs.compare(lhs) == 0);
}

bool operator==(const String& lhs, const char32_t* rhs)
{
    return (lhs.compare(rhs) == 0);
}

bool operator==(const char* lhs, const String& rhs)
{
    return (rhs.compare(lhs) == 0);
}

bool operator==(const String& lhs, const char* rhs)
{
    return (lhs.compare(rhs) == 0);
}

bool operator!=(const char32_t* lhs, const String& rhs)
{
    return (rhs.compare(lhs) != 0);
}

bool operator!=(const String& lhs, const char32_t* rhs)
{
    return (lhs.compare(rhs) != 0);
}

bool operator!=(const char* lhs, const String& rhs)
{
    return (rhs.compare(lhs) != 0);
}

bool operator!=(const String& lhs, const char* rhs)
{
    return (lhs.compare(rhs) != 0);
}

String operator+(const String& lhs, const String& rhs)
{
    return String(lhs).append(rhs);
}

String operator+(const std::string& lhs, const String& rhs)
{
    return String(lhs).append(rhs);
}

String operator+(const String& lhs, const std::string& rhs)
{
    return String(lhs).append(rhs);
}

String operator+(const std::u32string& lhs, const String& rhs)
{
    return String(lhs).append(rhs);
}

String operator+(const char32_t* lhs, const String& rhs)
{
    return String(lhs) + rhs;
}

String operator+(const String& lhs, const std::u32string& rhs)
{
    return String(lhs).append(rhs);
}

String operator+(const char* lhs, const String& rhs)
{
    return String(lhs) + rhs;
}

String operator+(char32_t lhs, const String& rhs)
{
    return String(1, lhs) + rhs;
}

String operator+(char lhs, const String& rhs)
{
    return String(1, lhs) + rhs;
}

String operator+(const String& lhs, const char32_t* rhs)
{
    return lhs + String(rhs);
}

String operator+(const String& lhs, const char* rhs)
{
    return lhs + String(rhs);
}

String operator+(const String& lhs, char32_t rhs)
{
    return lhs + String(1, rhs);
}

String operator+(const String& lhs, char rhs)
{
    return lhs + String(1, rhs);
}

String operator+(String&& lhs, const String& rhs)
{
    return std::move(lhs.append(rhs));
}

String operator+(String&& lhs, const std::string& rhs)
{
    return std::move(lhs.append(rhs));
}

String operator+(String&& lhs, const std::u32string& rhs)
{
    return std::move(lhs.append(rhs));
}

String operator+(const String& lhs, String&& rhs)
{
    return std::move(rhs.insert(0, lhs));
}

String operator+(const std::string& lhs, String&& rhs)
{
    return std::move(rhs.insert(0, lhs));
}

String operator+(const std::u32string& lhs, String&& rhs)
{
    return std::move(rhs.insert(0, lhs));
}

String operator+(String&& lhs, String&& rhs)
{
    return std::move(lhs.append(std::move(rhs)));
}

String operator+(const char32_t* lhs, String&& rhs)
{
    return std::move(rhs.insert(0, lhs));
}

String operator+(const char* lhs, String&& rhs)
{
    return std::move(rhs.insert(0, lhs));
}

String operator+(char32_t lhs, String&& rhs)
{
    return std::move(rhs.insert(0, 1, lhs));
}

String operator+(char lhs, String&& rhs)
{
    return std::move(rhs.insert(0, 1, lhs));
}

String operator+(String&& lhs, const char32_t* rhs)
{
    return std::move(lhs.append(rhs));
}

String operator+(String&& lhs, const char* rhs)
{
    return std::move(lhs.append(rhs));
}

String operator+(String&& lhs, char32_t rhs)
{
    return std::move(lhs.append(1, rhs));
}

String operator+(String&& lhs, char rhs)
{
    return std::move(lhs.append(1, rhs));
}

bool operator<(const String& str1, const String& str2)
{
    return (str1.compare(str2) < 0);
}

bool operator<(const String& str, const std::string& std_str)
{
    return (str.compare(std_str) < 0);
}

bool operator<(const String& str, const std::u32string& std_u32str)
{
    return (str.compare(std_u32str) < 0);
}

bool operator<(const std::string& std_str, const String& str)
{
    return (str.compare(std_str) >= 0);
}

bool operator<(const std::u32string& std_u32str, const String& str)
{
    return (str.compare(std_u32str) >= 0);
}

bool operator<=(const String& str1, const String& str2)
{
    return (str1.compare(str2) <= 0);
}

bool operator<=(const String& str, const std::string& std_str)
{
    return (str.compare(std_str) <= 0);
}

bool operator<=(const String& str, const std::u32string& std_u32str)
{
    return (str.compare(std_u32str) <= 0);
}

bool operator<=(const std::string& std_str, const String& str)
{
    return (str.compare(std_str) > 0);
}

bool operator<=(const std::u32string& std_u32str, const String& str)
{
    return (str.compare(std_u32str) > 0);
}

bool operator<(const char32_t* lhs, const String& rhs)
{
    return (rhs.compare(lhs) >= 0);
}

bool operator<(const String& lhs, const char32_t* rhs)
{
    return (lhs.compare(rhs) < 0);
}

bool operator<(const char* lhs, const String& rhs)
{
    return (rhs.compare(lhs) >= 0);
}

bool operator<(const String& lhs, const char* rhs)
{
    return (lhs.compare(rhs) < 0);
}

bool operator<=(const char32_t* lhs, const String& rhs)
{
    return (rhs.compare(lhs) > 0);
}

bool operator<=(const String& lhs, const char32_t* rhs)
{
    return (lhs.compare(rhs) <= 0);
}

bool operator<=(const char* lhs, const String& rhs)
{
    return (rhs.compare(lhs) > 0);
}

bool operator<=(const String& lhs, const char* rhs)
{
    return (lhs.compare(rhs) <= 0);
}

bool operator>(const String& str1, const String& str2)
{
    return (str1.compare(str2) > 0);
}

bool operator>(const String& str, const std::string& std_str)
{
    return (str.compare(std_str) > 0);
}

bool operator>(const String& str, const std::u32string& std_u32str)
{
    return (str.compare(std_u32str) > 0);
}

bool operator>(const std::string& std_str, const String& str)
{
    return (str.compare(std_str) <= 0);
}

bool operator>(const std::u32string& std_u32str, const String& str)
{
    return (str.compare(std_u32str) <= 0);
}

bool operator>=(const String& str1, const String& str2)
{
    return (str1.compare(str2) >= 0);
}

bool operator>=(const String& str, const std::string& std_str)
{
    return (str.compare(std_str) >= 0);
}

bool operator>=(const String& str, const std::u32string& std_u32str)
{
    return (str.compare(std_u32str) >= 0);
}

bool operator>=(const std::string& std_str, const String& str)
{
    return (str.compare(std_str) < 0);
}

bool operator>=(const std::u32string& std_u32str, const String& str)
{
    return (str.compare(std_u32str) < 0);
}

bool operator>(const char32_t* lhs, const String& rhs)
{
    return (rhs.compare(lhs) <= 0);
}

bool operator>(const String& lhs, const char32_t* rhs)
{
    return (lhs.compare(rhs) > 0);
}

bool operator>(const char* lhs, const String& rhs)
{
    return (rhs.compare(lhs) <= 0);
}

bool operator>(const String& lhs, const char* rhs)
{
    return (lhs.compare(rhs) > 0);
}

bool operator>=(const char32_t* lhs, const String& rhs)
{
    return (rhs.compare(lhs) < 0);
}

bool operator>=(const String& lhs, const char32_t* rhs)
{
    return (lhs.compare(rhs) >= 0);
}

bool operator>=(const char* lhs, const String& rhs)
{
    return (rhs.compare(lhs) < 0);
}

bool operator>=(const String& lhs, const char* rhs)
{
    return (lhs.compare(rhs) >= 0);
}

void swap(String& str1, String& str2)
{
    str1.swap(str2);
}

std::basic_ostream<char>& operator<<(std::basic_ostream<char>& outputStream, const String& str)
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    outputStream << str.d_string;
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    outputStream << String::convertUtf32ToUtf8(str.getString());
#endif
    return outputStream;
}

std::basic_istream<char>& operator>>(std::basic_istream<char>& inputStream, String& str)
{
#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
    inputStream >> str.d_string;
#elif (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32)
    std::string inputString;
    inputStream >> inputString;
    str.assign(inputString);
#endif
    return inputStream;
}

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
void String::codepoint_iterator::decrementByOneCodePoint()
{
    if (m_iter == m_rangeStart)
    {
        throw UnicodeStringException(
            "Error: Attempting to decrement a code point iterator, which"
            "already points to the beginning of the String.");
    }

    String::const_iterator endRangeIter = m_iter;

    // Decrement the iterator while this is an continuing code unit
    // starting from the code unit preceding the code point we
    // were pointing to previously
    --m_iter;
    size_t codeUnitCount = 0;
    while (isContinuingUTF8CodeUnit(*m_iter))
    {
        if (m_iter == m_rangeStart)
        {
            throw UnicodeStringException(
                "No leading byte found in this String for the current "
                "continuing code unit.");
        }
        else
        {
            --m_iter;
            ++codeUnitCount;
        }
    }

    if (getCodePointSize(*m_iter) != (codeUnitCount + 1))
    {
        throw UnicodeStringException(
            "Invalid code point encountered while decrementing "
            "the code point iterator.");
    }
}

void String::codepoint_iterator::incrementByOneCodePoint()
{
    size_t size = getCodePointSize(*m_iter);
    for (size_t i = 0; i < size; ++i)
    {
        if (m_iter < m_rangeEnd)
        {
            ++m_iter;
        }
        if (m_iter > m_rangeEnd)
        {
            throw UnicodeStringException(
                "The code point iterators to be compared are defined "
                "within different ranges");
        }
    }
}
#endif

}

#endif
