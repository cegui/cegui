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
#ifndef _CEGUIWin32ClipboardProvider_h_
#define _CEGUIWin32ClipboardProvider_h_

#include "CEGUI/Clipboard.h"

namespace CEGUI
{

class Win32ClipboardProvider : public NativeClipboardProvider
{
public:
    Win32ClipboardProvider();
    ~Win32ClipboardProvider();

    // NativeClipboardProvider implementation
    void sendToClipboard(const String& mime_type, void* buffer, size_t size) override;
    void retrieveFromClipboard(String& mime_type, void*& buffer, size_t& size) override;

private:
   char* d_buffer;
   size_t d_bufferSize;

   void allocateBuffer(size_t size);
   void deallocateBuffer();
};

}

#endif
