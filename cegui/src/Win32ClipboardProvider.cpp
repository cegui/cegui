/***********************************************************************
    created:    19/09/2013
    author:     Alexander Gladis

    purpose:    Implement a clipboard provider for Windows
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2013 Paul D Turner & The CEGUI Development Team
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
#include <windows.h>
#include "CEGUI/Win32ClipboardProvider.h"
#include <CEGUI/System.h>

namespace CEGUI
{

Win32ClipboardProvider::Win32ClipboardProvider() :
    d_buffer(nullptr),
    d_bufferSize(0)
{
}

Win32ClipboardProvider::~Win32ClipboardProvider()
{
   deallocateBuffer();
}

void Win32ClipboardProvider::allocateBuffer(size_t size)
{
   if(d_buffer)
      delete[] d_buffer;

   d_buffer = new char[size];
   d_bufferSize = size;
}

void Win32ClipboardProvider::deallocateBuffer()
{
   delete[] d_buffer;
   d_buffer = nullptr;
   d_bufferSize = 0;
}

void Win32ClipboardProvider::sendToClipboard(const String& mime_type, void* buffer, size_t size)
{
    if (mime_type != "text/plain")
        return;

    if (!::OpenClipboard(nullptr))
        return;

    // Transcode buffer to UTF-16
    String str(static_cast<char*>(buffer), size);

    const char16_t* utf16str = System::getSingleton().getStringTranscoder().stringToUTF16(str);
    const size_t size_in_bytes = (str.size() + 1) * sizeof(char16_t);

    // Copy to clipboard
    ::EmptyClipboard();
    if (HGLOBAL clipboard_data = ::GlobalAlloc(GMEM_DDESHARE, size_in_bytes))
    {
        if (LPWSTR clipboard = static_cast<LPWSTR>(::GlobalLock(clipboard_data)))
            memcpy(clipboard, utf16str, size_in_bytes);
        ::GlobalUnlock(clipboard_data);
        // CF_UNICODETEXT is for UTF-16 encoded strings
        ::SetClipboardData(CF_UNICODETEXT, clipboard_data);
        ::CloseClipboard();
    }

    delete[] utf16str;
}

void Win32ClipboardProvider::retrieveFromClipboard(String& mime_type, void*& buffer, size_t& size)
{
   if(OpenClipboard(nullptr))
   {
      // Open & read UTF-16 clipboard data
      HGLOBAL clipboard_data = GetClipboardData(CF_UNICODETEXT);
      const char16_t* clipboard = static_cast<const char16_t*>(GlobalLock(clipboard_data));
      if(clipboard)
      {
         // Transcode UTF-16 to native format and copy to local buffer
         String str = System::getSingleton().getStringTranscoder().stringFromUTF16(clipboard);

#if (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_ASCII) || (CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8)
         // We need the actual byte count which can be different from str.size() when using UTF-8!
         const char* utf8Characters = str.c_str();
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
         // We need the actual byte count which can be different from str.size() when using UTF-8!
         std::string utf8String = String::convertUtf32ToUtf8(str.getString());
         const char* utf8Characters = utf8String.c_str();
#endif
         const size_t characterCount = std::char_traits<char>::length(utf8Characters);
         allocateBuffer(characterCount); 
         memcpy(d_buffer, utf8Characters, d_bufferSize);

         mime_type = "text/plain";
         buffer = d_buffer;
         size = d_bufferSize;
      }

      // Close clipboard
      GlobalUnlock(clipboard_data);
      CloseClipboard();
   }
}

}
