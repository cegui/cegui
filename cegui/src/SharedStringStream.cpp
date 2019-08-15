/***********************************************************************
    created:    16th August 2015
    author:     Lukas Meindl
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
#include "CEGUI/SharedStringStream.h"

namespace CEGUI
{
thread_local SharedStringstream SharedStringstream::s_sharedStreamInstance;

SharedStringstream::SharedStringstream()
{
    // We only want to use the "C" locale for our stringstreams internally in CEGUI
    d_sharedStream.imbue(std::locale("C"));
    d_sharedStream.setf(std::ios_base::skipws);

    d_sharedStream.unsetf(std::ios_base::floatfield);
    d_sharedStream.precision(8);
    d_sharedStream.unsetf(std::ios_base::showpoint);

    d_decimalPoint = static_cast<CEGUI::String::value_type>(
        std::use_facet<std::numpunct<char>>(d_sharedStream.getloc()).decimal_point());
    d_negativeSign = static_cast<CEGUI::String::value_type>('-');
    d_positiveSign = static_cast<CEGUI::String::value_type>('+');
}

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_ASCII
std::stringstream& SharedStringstream::GetPreparedStream(const String& initialValue)
{
    std::stringstream& sstream = s_sharedStreamInstance.d_sharedStream;
#if CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_32
    sstream.str(String::convertUtf32ToUtf8(initialValue.getString()));
#elif CEGUI_STRING_CLASS == CEGUI_STRING_CLASS_UTF_8
        sstream.str(initialValue.getString());
#endif
    sstream.clear();

    return sstream;
}
#endif

std::stringstream& SharedStringstream::GetPreparedStream(const std::string& initialValue)
{
    std::stringstream& sstream = s_sharedStreamInstance.d_sharedStream;
    sstream.str(initialValue);
    sstream.clear();

    return sstream;
}

std::stringstream& SharedStringstream::GetPreparedStream()
{
    std::stringstream& sstream = s_sharedStreamInstance.d_sharedStream;
    sstream.str(std::string());
    sstream.clear();
    return sstream;
}

CEGUI::String SharedStringstream::GetPointerAddressAsString(const void* addressPointer)
{
    std::stringstream& sstream = GetPreparedStream();
    sstream << addressPointer;
    return CEGUI::String(sstream.str());
}
}
