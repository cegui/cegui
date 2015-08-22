/***********************************************************************
    created:    Fri Jun 01 2012
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/Win32StringTranscoder.h"
#include "CEGUI/Exceptions.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace CEGUI
{
//----------------------------------------------------------------------------//
Win32StringTranscoder::Win32StringTranscoder()
{
}

//----------------------------------------------------------------------------//
std::uint16_t* Win32StringTranscoder::stringToUTF16(const String& input) const
{
    const int len = MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1,
                                        0, 0);
    if (!len)
        throw CEGUI::InvalidRequestException(
            "MultiByteToWideChar failed");

    std::uint16_t* buff = new std::uint16_t[len];
    MultiByteToWideChar(CP_UTF8, 0, input.c_str(), -1,
                        reinterpret_cast<LPWSTR>(buff), len);

    return buff;
}

//----------------------------------------------------------------------------//
std::wstring Win32StringTranscoder::stringToStdWString(const String& input) const
{
    std::uint16_t* tmp = stringToUTF16(input);
    std::wstring result(reinterpret_cast<wchar_t*>(tmp));
    deleteUTF16Buffer(tmp);

    return result;
}

//----------------------------------------------------------------------------//
// Templatised helper so code within our string type conditional compile does
// not need to be duplicated.
template<typename T>
static CEGUI::String stringFromUTF16(UINT codepage, const std::uint16_t* input)
{
    const int len =
        WideCharToMultiByte(codepage, 0, reinterpret_cast<LPCWSTR>(input), -1,
                            0, 0, 0, 0);
    if (!len)
        throw CEGUI::InvalidRequestException(
            "WideCharToMultiByte failed");

    T* buff = new T[len];

    WideCharToMultiByte(codepage, 0, reinterpret_cast<LPCWSTR>(input), -1,
                        reinterpret_cast<char*>(buff), len, 0, 0);

    const CEGUI::String result(buff);

    delete[] buff;

    return result;
}

//----------------------------------------------------------------------------//
String Win32StringTranscoder::stringFromUTF16(const std::uint16_t* input) const
{
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
    return CEGUI::stringFromUTF16<utf8>(CP_UTF8, input);
#else
    return CEGUI::stringFromUTF16<String::value_type>(CP_ACP, input);
#endif
}

//----------------------------------------------------------------------------//
String Win32StringTranscoder::stringFromStdWString(const std::wstring& input) const
{
    return stringFromUTF16(reinterpret_cast<const std::uint16_t*>(input.c_str()));
}

//----------------------------------------------------------------------------//
void Win32StringTranscoder::deleteUTF16Buffer(std::uint16_t* input) const
{
    const std::uint16_t* b = input;
    while (*b++);

    delete[] input;
}

//----------------------------------------------------------------------------//

}

