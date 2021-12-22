/***********************************************************************
	created:	16th August 2015
	author:		Lukas Meindl
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
#ifndef _SharedStringstream_h_
#define _SharedStringstream_h_

#include "CEGUI/String.h"
#include <sstream>

namespace CEGUI
{
//! For library-internal usage only
class SharedStringstream
{
public:
    SharedStringstream();

    /*!
    \brief
        Helper function for getting the stringstream with its value set to the
        supplied String value.
    \param initialValue
        The String this stringstream should be initialised to.
    \return
        The shared stringstream.
    */
    static std::stringstream& GetPreparedStream(const String& initialValue);

#if CEGUI_STRING_CLASS != CEGUI_STRING_CLASS_ASCII
    /*!
    \brief
        Helper function for getting the stringstream with its value set to the
        supplied string value.
    \param initialValue
        The string this stringstream should be initialised to.
    \return
        The shared stringstream.
    */
    static std::stringstream& GetPreparedStream(const std::string& initialValue);
#endif

    /*!
    \brief
        Helper function for getting the shared stringstream set to an empty
        String value.
    \return
        The shared stringstream.
    */
    static std::stringstream& GetPreparedStream();

    /*!
    \brief
        Helper function for retrieving the hexadecimal representation of a
        memory address as String based on a passed pointer. 
    \param addressPointer
        The pointer to the memory address that we want to retrieve as String.
    \return
        A String containing the memory address representation.
    */
    static String GetPointerAddressAsString(const void* addressPointer);

    static CEGUI::String::value_type GetDecimalPoint() { return s_sharedStreamInstance.d_decimalPoint; }
    static CEGUI::String::value_type GetNegativeSign() { return s_sharedStreamInstance.d_negativeSign; }
    static CEGUI::String::value_type GetPositiveSign() { return s_sharedStreamInstance.d_positiveSign; }

private:
    thread_local static SharedStringstream s_sharedStreamInstance;

    std::stringstream d_sharedStream;
    CEGUI::String::value_type d_decimalPoint;
    CEGUI::String::value_type d_negativeSign;
    CEGUI::String::value_type d_positiveSign;
};

}


#endif
