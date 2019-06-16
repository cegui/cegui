/***********************************************************************
    created:    25/05/2009
    author:     Paul Turner
 *************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/FormattedRenderedString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
FormattedRenderedString::FormattedRenderedString(const RenderedString& string) :
    d_renderedString(&string),
    d_wasWordSplit(false)
{
}

//----------------------------------------------------------------------------//
FormattedRenderedString::~FormattedRenderedString()
{
}

//----------------------------------------------------------------------------//
void FormattedRenderedString::setRenderedString(const RenderedString& string)
{
    d_renderedString = &string;
}

//----------------------------------------------------------------------------//
const RenderedString& FormattedRenderedString::getRenderedString() const
{
    return *d_renderedString;
}

//----------------------------------------------------------------------------//
bool FormattedRenderedString::wasWordSplit() const
{
    return d_wasWordSplit;
}

//----------------------------------------------------------------------------//
void FormattedRenderedString::setWasWordSplit(bool value)
{
    d_wasWordSplit = value;
}

//----------------------------------------------------------------------------//
std::size_t FormattedRenderedString::getNumOfOriginalTextLines() const
{
    return getRenderedString().getLineCount();
}
    
//----------------------------------------------------------------------------//
std::size_t FormattedRenderedString::getNumOfFormattedTextLines() const
{
    return getNumOfOriginalTextLines();
}

} // End of  CEGUI namespace section
