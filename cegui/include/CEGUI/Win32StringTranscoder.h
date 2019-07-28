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
#ifndef _CEGUIWin32StringTranscoder_h_
#define _CEGUIWin32StringTranscoder_h_

#include "CEGUI/StringTranscoder.h"

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! Implementation of StringTranscoder that uses Win32
class CEGUIEXPORT Win32StringTranscoder : public StringTranscoder
{
public:
    Win32StringTranscoder();

    // implement abstract interface
    char16_t* stringToUTF16(const String& input) const override;
    std::wstring stringToStdWString(const String& input) const override;
    String stringFromUTF16(const char16_t* input) const override;
    String stringFromStdWString(const std::wstring& input) const override;
};

}

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif


#endif

