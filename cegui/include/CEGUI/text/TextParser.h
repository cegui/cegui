/***********************************************************************
    created:    28/01/2022
    author:     Vladimir Orlov
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2022 Paul D Turner & The CEGUI Development Team
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
#pragma once
#include "CEGUI/String.h"
#include <memory>

namespace CEGUI
{
using RenderedTextElementPtr = std::unique_ptr<class RenderedTextElement>;

//! Specifies interface for classes that parse text to extract style info and embedded objects
class CEGUIEXPORT TextParser
{
public:

    virtual ~TextParser() = default;

    /*!
    \brief
        parse a text string and fill meta-info extracted from it.

    \param inText
        String object holding the text that is to be parsed.

    \param outText
        UTF-32 text with tags and meta parsed out of it.

    \param outOriginalIndices
        Mapping from outText to inText. Empty if they map 1 to 1.

    \param outElementIndices
        Mapping from outText indices to their affecting element indices.

    \param outElements
        'RenderedTextElement's that control appearance and behaviour of outText parts.

    \return
        Whether parsing was successful.
    */
    virtual bool parse(const String& inText, std::u32string& outText, std::vector<size_t>& outOriginalIndices,
        std::vector<uint16_t>& outElementIndices, std::vector<RenderedTextElementPtr>& outElements) = 0;
};

}
