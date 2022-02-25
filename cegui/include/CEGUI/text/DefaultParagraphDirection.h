/***********************************************************************
	created:	5th January 2016
	author:		Lukas Meindl
	
	purpose:	Defines the DefaultParagraphLayoutDirection enum
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2017 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIDefaultParagraphDirection_h_
#define _CEGUIDefaultParagraphDirection_h_

#include <cstdint>

namespace CEGUI
{

/*!
\brief 
        Enum for specifying the default direction of paragraphs (Also known as "direction" in CSS
        and LayoutDirection in Qt. Relevant for bidirectional text)
        
        Specifies the default order of words in a paragraph, which is relevant when having sentences in
        a RightToLeft language that may start with a word (or to be specific: first character of a word)
        from a LeftToRight language.
        Example: If the mode is set to Automatic and the first word of a paragraph in Hebrew is a German
        company name, written in German alphabet, the German word will end up left, whereas the rest of
        the Hebrew sentences starts from the righ, continuing towards the left. With the setting RightToLeft
        the sentence will start on the very right with the German word, as would be expected in a mainly
        RightToLeft written paragraph. If the language of the UI user is known, then either LeftToRight
        or RightToLeft should be chosen for the paragraphs.
 */
enum class DefaultParagraphDirection : std::uint8_t
{
    //! Default direction is from left to right, as used in most of the modern European and Asian languages .
    LeftToRight,
    //! Default direction is from right to left, as used in Hebrew, Persian and Arabic.
    RightToLeft,
    //! Default direction is specified automatically, depending on the first word (specifically: the first character of the first word).
    Automatic
};

}

#endif
