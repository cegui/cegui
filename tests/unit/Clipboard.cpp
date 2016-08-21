/***********************************************************************
 *    created:    11/6/2011
 *    author:     Martin Preisler
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/Clipboard.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(Clipboard)

const size_t BUFFER_SIZE = 1024;
const CEGUI::String MIME_TYPE = "cegui/regression-test-data";

BOOST_AUTO_TEST_CASE(InternalClipboard)
{
    CEGUI::Clipboard cb;

    // string set, get
    cb.setText("TestingContents");
    BOOST_CHECK_EQUAL(cb.getText(), "TestingContents");

    // raw data set, get
    std::uint8_t* buffer = new std::uint8_t[BUFFER_SIZE];
    for (unsigned int i = 0; i < BUFFER_SIZE; ++i)
    {
        // I want to have something in the memory, not just zeroes,
        // this could avoid false positives in some cases
        buffer[i] = static_cast<std::uint8_t>(i % std::numeric_limits<std::uint8_t>::max());
    }

    cb.setData(MIME_TYPE, buffer, BUFFER_SIZE);

    CEGUI::String retrievedMimeType;
    const void* retrievedBuffer;
    size_t retrievedBufferSize;

    cb.getData(retrievedMimeType, retrievedBuffer, retrievedBufferSize);

    BOOST_CHECK_EQUAL(MIME_TYPE, retrievedMimeType);
    BOOST_CHECK_EQUAL(BUFFER_SIZE, retrievedBufferSize);
    BOOST_CHECK(memcmp(buffer, retrievedBuffer, BUFFER_SIZE) == 0); // 0 indicates that both blocks of memory are equal
    delete[] buffer;
}

static CEGUI::String g_MimeType = "";
static std::uint8_t* g_ClipboardBuffer = nullptr;
static size_t g_ClipboardSize = 0;

class TestNativeClipboardProvider : public CEGUI::NativeClipboardProvider
{
private:
    std::vector<std::unique_ptr<uint8_t[]>> d_allocatedMemory;
public:
    virtual ~TestNativeClipboardProvider() {}

    void sendToClipboard(const CEGUI::String& mimeType, void* buffer, size_t size) override
    {
        g_MimeType = mimeType;
        g_ClipboardSize = size;
        g_ClipboardBuffer = new std::uint8_t[size];
        memcpy(g_ClipboardBuffer, buffer, size);
        d_allocatedMemory.emplace_back(static_cast<std::uint8_t*>(g_ClipboardBuffer));
    }

    void retrieveFromClipboard(CEGUI::String& mimeType, void*& buffer, size_t& size) override
    {
        mimeType = g_MimeType;
        size = g_ClipboardSize;

        // we have to allocate buffer for the user of the native clipboard provider!
        buffer = new std::uint8_t[g_ClipboardSize];
        memcpy(buffer, g_ClipboardBuffer, g_ClipboardSize);
        d_allocatedMemory.emplace_back(static_cast<std::uint8_t*>(buffer));
    }
};

BOOST_AUTO_TEST_CASE(NativeClipboardProvider)
{
    CEGUI::NativeClipboardProvider* provider = new TestNativeClipboardProvider();
    CEGUI::Clipboard cb;
    cb.setNativeProvider(provider);

    // ASCII string set, get
    const char* asciiTest = "TestingContents";
    cb.setText(asciiTest);
    BOOST_CHECK_EQUAL(cb.getText(), asciiTest);
    BOOST_CHECK_EQUAL(g_ClipboardSize, 15); // it only contains characters from ASCII 7bit
#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_UTF_32
    BOOST_CHECK(memcmp(g_ClipboardBuffer, CEGUI::String(asciiTest).c_str(), g_ClipboardSize) == 0);
#else
    BOOST_CHECK(memcmp(g_ClipboardBuffer, CEGUI::String::convertUtf32ToUtf8(CEGUI::String(asciiTest).getString()).c_str(), g_ClipboardSize) == 0);
#endif

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32 || CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8
    // Unicode string set, get
    const char* utf8Test = "(・。・;)";
    cb.setText(utf8Test);
    BOOST_CHECK_EQUAL(cb.getText(), utf8Test);

#   if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8
    BOOST_CHECK(memcmp(g_ClipboardBuffer, CEGUI::String(utf8Test).c_str(), g_ClipboardSize) == 0);
#   endif

#endif

    delete provider;
}

BOOST_AUTO_TEST_SUITE_END()
