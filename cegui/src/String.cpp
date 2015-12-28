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

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE

namespace CEGUI
{

std::u32string String::convertUtf8ToUtf32(const char* utf8String)
{
    if(utf8String == nullptr)
        return std::u32string();

    std::size_t codeUnitCount = std::char_traits<char>::length(utf8String);
    return convertUtf8ToUtf32(utf8String, codeUnitCount);
}

std::u32string String::convertUtf8ToUtf32(const char* utf8StringStart, const char* utf8StringEnd)
{
    if (utf8StringStart == nullptr)
        return std::u32string();

    return convertUtf8ToUtf32(utf8StringStart, utf8StringEnd - utf8StringStart);
}

std::u32string String::convertUtf8ToUtf32(const std::string& utf8String)
{
    return convertUtf8ToUtf32(utf8String.data(), utf8String.size());
}

std::u32string String::convertUtf8ToUtf32(const char utf8Char)
{
    return convertUtf8ToUtf32(&utf8Char, 1);
}

std::u32string String::convertUtf8ToUtf32(const char* utf8String, const size_t stringLength)
{
    if (utf8String == nullptr)
        return std::u32string();

    std::u32string utf32String;

    // Go through every UTF-8 code unit
    size_t currentCharIndex = 0;
    while (currentCharIndex < stringLength)
    {
        const unsigned char currentCodeUnit = static_cast<const unsigned char>(utf8String[currentCharIndex]);

        char32_t utf32CodeUnit;

        // Check if the code point consists of a single code unit
        if (currentCodeUnit < 0x80)
        {
            utf32CodeUnit = static_cast<char32_t>(currentCodeUnit);
        }
        else if (currentCodeUnit < 0xE0)
        {
            if (currentCharIndex + 1 >= stringLength)
            {
                throw CEGUI::InvalidRequestException("String conversion from UTF-8 to UTF-32 failed due to the "
                                                     "start byte not being follwed by enough continuation bytes");
                break;
            }
            utf32CodeUnit = ((currentCodeUnit                & 0x1F) << 6);
            utf32CodeUnit |= (utf8String[++currentCharIndex] & 0x3F);
        }
        else if (currentCodeUnit < 0xF0)
        {
            if (currentCharIndex + 2 >= stringLength)
            {
                throw CEGUI::InvalidRequestException("String conversion from UTF-8 to UTF-32 failed due to the "
                                                      "start byte not being follwed by enough continuation bytes");
                break;
            }
            utf32CodeUnit = ((currentCodeUnit                   & 0x0F) << 12);
            utf32CodeUnit |= ((utf8String[++currentCharIndex]   & 0x3F) << 6);
            utf32CodeUnit |= (utf8String[++currentCharIndex]    & 0x3F);
        }
        else
        {
            if (currentCharIndex + 3 >= stringLength)
            {
                throw CEGUI::InvalidRequestException("String conversion from UTF-8 to UTF-32 failed due to the "
                                                     "start byte not being follwed by enough continuation bytes");
                break;
            }
            utf32CodeUnit = ((currentCodeUnit                   & 0x07) << 18);
            utf32CodeUnit |= ((utf8String[++currentCharIndex]   & 0x3F) << 12);
            utf32CodeUnit |= ((utf8String[++currentCharIndex]   & 0x3F) << 6);
            utf32CodeUnit |= (utf8String[++currentCharIndex]    & 0x3F);
        }

        utf32String.push_back(utf32CodeUnit);

        ++currentCharIndex;
    }

    return utf32String;
}

std::string String::convertUtf32ToUtf8(const char32_t* utf32String)
{
    if (utf32String == nullptr)
        return std::string();

    std::size_t codeUnitCount = std::char_traits<char32_t>::length(utf32String);
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
        const char32_t& currentCodeUnit = utf32String[currentCharIndex];

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
    return std::move(lhs.append(rhs));
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
    outputStream << str.toUtf8String();
    return outputStream;
}

std::basic_istream<char>& operator>>(std::basic_istream<char>& inputStream, String& str)
{
    std::string inputString;
    inputStream >> inputString;
    str.assign(inputString);
    return inputStream;
}

}

#endif
