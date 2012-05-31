/***********************************************************************
    filename:   IconvStringTranscoder.cpp
    created:    Thu May 31 2012
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
#include "CEGUI/IconvStringTranscoder.h"
#include "CEGUI/Exceptions.h"
#include <vector>
#include <iconv.h>
#include <errno.h>

namespace CEGUI
{
//----------------------------------------------------------------------------//
class IconvHelper
{
    std::string d_fromCode;
    std::string d_toCode;
    iconv_t d_cd;

public:
    //------------------------------------------------------------------------//
    IconvHelper(const std::string& tocode, const std::string& fromcode) :
        d_fromCode(fromcode),
        d_toCode(tocode),
        d_cd(iconv_open(d_toCode.c_str(), d_fromCode.c_str()))
    {
        if (d_cd == reinterpret_cast<iconv_t>(-1))
            CEGUI_THROW(InvalidRequestException(String(
                "[ICONVHelper] Failed to create conversion descriptor from \"") +
                d_fromCode + "\" to \"" + d_toCode + "\"."));
    }

    //------------------------------------------------------------------------//
    ~IconvHelper()
    {
        iconv_close(d_cd);
    }

    //------------------------------------------------------------------------//
    size_t iconv(const char** inbuf, size_t* inbytesleft,
                 char** outbuf, size_t* outbytesleft)
    {
        return ::iconv(d_cd, const_cast<char**>(inbuf), inbytesleft,
                       outbuf, outbytesleft);
    }

    //------------------------------------------------------------------------//
    void throwErrorException(int err)
    {
        std::string reason;

        if (errno == EINVAL)
            reason = "Incomplete " + d_fromCode + " sequence.";
        else if (errno == EILSEQ)
            reason = "Invalid " + d_fromCode + " sequence.";
        else
            reason = "Unknown error.";

        CEGUI_THROW(InvalidRequestException(String(
            "[ICONVHelper] Failed to convert from \"") + d_fromCode +
            "\" to \"" + d_toCode + "\": " + reason));
    }

    //------------------------------------------------------------------------//
};

//----------------------------------------------------------------------------//
// Helper to use iconv to perform some transcode operation.
template<typename T>
static T* iconvTranscode(IconvHelper& ich, const char* in_buf, size_t in_len)
{
    std::vector<T CEGUI_VECTOR_ALLOC(T)> out_vec;
    out_vec.resize(in_len);
    size_t out_count = 0;

    while (true)
    {
        char* out_buf =
            reinterpret_cast<char*>(&out_vec[out_count]);
        const size_t start_out_bytes_left =
            (out_vec.size() - out_count) * sizeof(T);
        size_t out_bytes_left = start_out_bytes_left;

        const size_t result = ich.iconv(&in_buf, &in_len,
                                        &out_buf, &out_bytes_left);

        out_count +=
            (start_out_bytes_left - out_bytes_left) / sizeof(T);

        if (result != static_cast<size_t>(-1))
        {
            T* ret_buff = CEGUI_NEW_ARRAY_PT(T, out_count + 1,
                                             CEGUI::BufferAllocator);
            memcpy(ret_buff, &out_vec[0], out_count * sizeof(T));
            ret_buff[out_count] = 0;
            return ret_buff;
        }

        if (errno != E2BIG)
            break;

        out_vec.resize(out_vec.size() + 8); // this is some arbitrary number
    }

    ich.throwErrorException(errno);
}

//----------------------------------------------------------------------------//
// Helper tp return length of zero terminated string of Ts
template<typename T>
static size_t getStringLength(const T* buffer)
{
    const T* b = buffer;
    while (*b++);

    return b - buffer;
}

//----------------------------------------------------------------------------//
// Helper to correctly delete a buffer returned from iconvTranscode
template<typename T>
static void deleteTranscodeBuffer(const T* buffer)
{
    CEGUI_DELETE_ARRAY_PT(
        buffer, T, getStringLength(buffer), CEGUI::BufferAllocator);
}

//----------------------------------------------------------------------------//
// Helper to transcode a buffer and return a string class built from it.
template<typename String_T, typename CodeUnit_T>
static String_T iconvTranscode(IconvHelper& ich,
                               const char* in_buf, size_t in_len)
{
    CodeUnit_T* tmp = iconvTranscode<CodeUnit_T>(ich, in_buf, in_len);
    String_T result(tmp);
    deleteTranscodeBuffer(tmp);

    return result;
}

//----------------------------------------------------------------------------//
uint16* IconvStringTranscoder::stringToUTF16(const String& input)
{
    IconvHelper ich("UTF-16", "UTF-8");
    return iconvTranscode<uint16>(
        ich, input.c_str(), getStringLength(input.c_str()));
}

//----------------------------------------------------------------------------//
std::wstring IconvStringTranscoder::stringToStdWString(const String& input)
{
    IconvHelper ich("WCHAR_T", "UTF-8");
    return iconvTranscode<std::wstring, wchar_t>(
        ich, input.c_str(), getStringLength(input.c_str()));
}

//----------------------------------------------------------------------------//
String IconvStringTranscoder::stringFromUTF16(const uint16* input)
{
    IconvHelper ich("UTF-8", "UTF-16");
    return iconvTranscode<String, utf8>(
        ich, reinterpret_cast<const char*>(input),
        getStringLength(input) * sizeof(uint16));
}

//----------------------------------------------------------------------------//
String IconvStringTranscoder::stringFromStdWString(const std::wstring& input)
{
    IconvHelper ich("UTF-8", "WCHAR_T");
    return iconvTranscode<String, utf8>(
        ich, reinterpret_cast<const char*>(input.c_str()),
        input.length() * sizeof(wchar_t));
}

//----------------------------------------------------------------------------//
void IconvStringTranscoder::deleteUTF16Buffer(const uint16* input)
{
    deleteTranscodeBuffer(input);
}

//----------------------------------------------------------------------------//

}

