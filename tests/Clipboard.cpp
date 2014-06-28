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
    CEGUI::uint8* buffer = new CEGUI::uint8[BUFFER_SIZE];
    for (unsigned int i = 0; i < BUFFER_SIZE; ++i)
    {
        // I want to have something in the memory, not just zeroes,
        // this could avoid false positives in some cases
        buffer[i] = static_cast<CEGUI::uint8>(i % std::numeric_limits<CEGUI::uint8>::max());
    }

    cb.setData(MIME_TYPE, buffer, BUFFER_SIZE);

    CEGUI::String retrievedMimeType;
    const void* retrievedBuffer;
    size_t retrievedBufferSize;

    cb.getData(retrievedMimeType, retrievedBuffer, retrievedBufferSize);

    BOOST_CHECK_EQUAL(MIME_TYPE, retrievedMimeType);
    BOOST_CHECK_EQUAL(BUFFER_SIZE, retrievedBufferSize);
    BOOST_CHECK(memcmp(buffer, retrievedBuffer, BUFFER_SIZE) == 0); // 0 indicates that both blocks of memory are equal
}

static CEGUI::String g_MimeType = "";
static CEGUI::uint8* g_ClipboardBuffer = 0;
static size_t g_ClipboardSize = 0;

class TestNativeClipboardProvider : public CEGUI::NativeClipboardProvider
{
public:
    virtual ~TestNativeClipboardProvider()
    {}

    virtual void sendToClipboard(const CEGUI::String& mimeType, void* buffer, size_t size)
    {
        delete g_ClipboardBuffer;

        g_MimeType = mimeType;
        g_ClipboardSize = size;
        g_ClipboardBuffer = new CEGUI::uint8[size];
        memcpy(g_ClipboardBuffer, buffer, size);
    }

    virtual void retrieveFromClipboard(CEGUI::String& mimeType, void*& buffer, size_t& size)
    {
        mimeType = g_MimeType;
        size = g_ClipboardSize;

        // we have to allocate buffer for the user of the native clipboard provider!
        buffer = new CEGUI::uint8[g_ClipboardSize];
        memcpy(buffer, g_ClipboardBuffer, g_ClipboardSize);
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
    BOOST_CHECK(memcmp(g_ClipboardBuffer, CEGUI::String(asciiTest).c_str(), g_ClipboardSize) == 0);

#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UNICODE
    // Unicode string set, get
    const CEGUI::utf8* utf8Test = (const CEGUI::utf8*)"(・。・;)";
    cb.setText(utf8Test);
    BOOST_CHECK_EQUAL(cb.getText(), utf8Test);
    BOOST_CHECK(memcmp(g_ClipboardBuffer, CEGUI::String(utf8Test).c_str(), g_ClipboardSize) == 0);
#endif

    delete provider;
}

BOOST_AUTO_TEST_SUITE_END()
