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

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE

namespace CEGUI
{

std::u32string String::convertUtf8ToUtf32(const char* utf8String)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.from_bytes(utf8String);
}

std::u32string String::convertUtf8ToUtf32(const char* utf8StringStart, const char* utf8StringEnd)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.from_bytes(utf8StringStart, utf8StringEnd);
}

std::u32string String::convertUtf8ToUtf32(const std::string& utf8String)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.from_bytes(utf8String);
}

std::u32string String::convertUtf8ToUtf32(const char utf8Char)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.from_bytes(utf8Char);
}

std::string String::convertUtf32ToUtf8(const char32_t* utf32String)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.to_bytes(utf32String);
}

std::string convertUtf32ToUtf8(const char32_t* utf32StringStart, const char32_t* utf32StringEnd)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.to_bytes(utf32StringStart, utf32StringEnd);
}

std::string String::convertUtf32ToUtf8(const std::u32string& utf32String)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.to_bytes(utf32String);
}

std::string String::convertUtf32ToUtf8(const char32_t utf32Char)
{
    //! The UTF-8 / UTF-32 standard conversion facet
    // TODO: Add thread_local if available
    static std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> utf8Converter;
    return utf8Converter.to_bytes(utf32Char);
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
