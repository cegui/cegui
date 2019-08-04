/***********************************************************************
created:    24/11/2015
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

#ifndef _LanguageStrings_h_
#define _LanguageStrings_h_

#include "CEGUI/DefaultParagraphDirection.h"

namespace LanguageStrings
{

static const unsigned int LangListSize = 12;

static struct
{
    const char* language;
    const char* font;
    const char* text;
    CEGUI::DefaultParagraphDirection direction;
    CEGUI::HorizontalTextFormatting horizontalAlignment;
} LangList[LangListSize] =
{
    // A list of strings in different languages stored as hexadecimal UTF-8 code units
    // Feel free to add your own language here.
    // Since this is part of the source code we escape the characters to ensure greater
    // support and so that we do not have to encode the file itself. Normally you would load
    // the text from an UTF-8 encoded file, where you could regular unescaped unicode characters.
    // So do not be afraid by the looks of this ;) We just want to avoid having to extra load any
    // files in the samples to keep it simple.
    {
        "Sandbox",
        "DejaVuSans-12",
        "Try out writing text in any language here. The used font can be changed on the right.\n"
        "You can create new fonts if needed. The new font will be set as font for the selected text/language...\n"
        "You can also edit the fonts that are part of this demo or the ones you created here "
        "(however, this demo won't allow to change the fonts of the other Samples)\n"
        "Important: When you switch to a font the FIRST time, it takes some time to load it. "
        "Especially for fonts with a lot of non-ASCII characters this load time might be "
        "noticeable.!\n"
        "The quick brown fox jumps over the lazy dog. 1234567890",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "European characters using DejaVuSans font",
        "DejaVuSans-12",
        "Try Catching The Brown Fox While It's Jumping Over The Lazy Dog\n\n"
        "Supports most European unicode characters, but for example no cyrillic:\n"
        "\x62\x6F\x6B\x6D\xC3\xA5\x6C\x2C\x20\xC4\x8D\x65\x73\x6B\x79\x2C\x20\x73"
        "\x6C\x6F\x76\x65\x6E\xC5\xA1\xC4\x8D\x69\x6E\x61\x2C\x20\x6C\x61\x74\x76"
        "\x69\x65\xC5\xA1\x75\x2C\x20\x54\x69\xE1\xBA\xBF\x6E\x67\x20\x56\x69\xE1"
        "\xBB\x87\x74\x2C\x20\x65\x74\x63\x2E"
        "\xC2\xA5\xC2\xA4\xC2\xA3\xC2\xA2\xC2\xA9\xC2\xAE\xC3\x90\xC3\x86\x5C"
        "\x6E\xC3\x84\xC3\x9C\xC3\x96\xC3\xA4\xC3\xBC\xC3\xB6\xC3\x9F\x5C\x6E"
        "\xC4\x93\xC4\x92\xC5\x97\xC5\x96\xC5\xAB\xC5\xAA\xC4\xAB\xC4\xAA\xC4"
        "\x81\xC4\x80\xC5\xA1\xC5\xA0\xC4\xA3\xC4\xA2\xC4\xB7\xC4\xB6\xC4\xBC"
        "\xC4\xBB\xC5\xBE\xC5\xBD\xC4\x8D\xC4\x8C\xC5\x86\xC5\x85\x5C\x6E\xC3"
        "\xB8\xC3\x98\xC3\xA6\xC3\x86\xC3\xA5\xC3\x85\xC3\xA8\xC3\xA2\xC3\xAF"
        "\xC3\x80\xC3\x81\xC3\x82\xC6\x92\xC3\xA9\xC3\xAD\xC3\xB3\xC3\xBA\xC3"
        "\x89\xC3\x8D\xC3\x93\xC3\x9A\xC3\xB5\xC3\xA7\xC2\xBF\xC3\xB1\xC3\x91",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "European characters using Junicode font",
        "Junicode-14",
        "Try Catching The Brown Fox While It's Jumping Over The Lazy Dog\n\n"
        "Supports most European unicode characters, but for example no cyrillic:\n"
        "\x62\x6F\x6B\x6D\xC3\xA5\x6C\x2C\x20\xC4\x8D\x65\x73\x6B\x79\x2C\x20\x73"
        "\x6C\x6F\x76\x65\x6E\xC5\xA1\xC4\x8D\x69\x6E\x61\x2C\x20\x6C\x61\x74\x76"
        "\x69\x65\xC5\xA1\x75\x2C\x20\x54\x69\xE1\xBA\xBF\x6E\x67\x20\x56\x69\xE1"
        "\xBB\x87\x74\x2C\x20\x65\x74\x63\x2E"
        "\xC2\xA5\xC2\xA4\xC2\xA3\xC2\xA2\xC2\xA9\xC2\xAE\xC3\x90\xC3\x86\x5C"
        "\x6E\xC3\x84\xC3\x9C\xC3\x96\xC3\xA4\xC3\xBC\xC3\xB6\xC3\x9F\x5C\x6E"
        "\xC4\x93\xC4\x92\xC5\x97\xC5\x96\xC5\xAB\xC5\xAA\xC4\xAB\xC4\xAA\xC4"
        "\x81\xC4\x80\xC5\xA1\xC5\xA0\xC4\xA3\xC4\xA2\xC4\xB7\xC4\xB6\xC4\xBC"
        "\xC4\xBB\xC5\xBE\xC5\xBD\xC4\x8D\xC4\x8C\xC5\x86\xC5\x85\x5C\x6E\xC3"
        "\xB8\xC3\x98\xC3\xA6\xC3\x86\xC3\xA5\xC3\x85\xC3\xA8\xC3\xA2\xC3\xAF"
        "\xC3\x80\xC3\x81\xC3\x82\xC6\x92\xC3\xA9\xC3\xAD\xC3\xB3\xC3\xBA\xC3"
        "\x89\xC3\x8D\xC3\x93\xC3\x9A\xC3\xB5\xC3\xA7\xC2\xBF\xC3\xB1\xC3\x91",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "Korean/Chinese/Japanese using Batang font",
        "Batang-18",

        //Japanese
        "\xE6\x97\xA5\xE6\x9C\xAC\xE8\xAA\x9E\xE3\x82\x92\xE9\x81\xB8\xE6\x8A\x9E\n"
        "\xE3\x83\x88\xE3\x83\xAA\xE3\x82\xAC\xE3\x83\xBC\xE6\xA4\x9C\xE7\x9F\xA5\n"
        "\xE9\x89\xB1\xE7\x9F\xB3\xE5\x82\x99\xE8\x93\x84\xE4\xB8\x8D\xE8\xB6\xB3\n\n\n\n\n"

        //Chinese
        "\xE6\x97\xA9\xE7\x99\xBC\xE7\x99\xBD\xE5\xB8\x9D\xE5\x9F\x8E\x20\x28\xE6\x9D\x8E\xE7\x99\xBD\x29\n\n"

        "\xE6\x9C\x9D\xE8\xBE\xAD\xE7\x99\xBD\xE5\xB8\x9D\xE5\xBD\xA9\xE9\x9B\xB2\xE9\x96\x93\xEF\xBC\x8C\n"
        "\xE5\x8D\x83\xE9\x87\x8C\xE6\xB1\x9F\xE9\x99\xB5\xE4\xB8\x80\xE6\x97\xA5\xE9\x82\x84\xE3\x80\x82\n"
        "\xE5\x85\xA9\xE5\xB2\xB8\xE7\x8C\xBF\xE8\x81\xB2\xE5\x95\xBC\xE4\xB8\x8D\xE4\xBD\x8F\xEF\xBC\x8C\n"
        "\xE8\xBC\x95\xE8\x88\x9F\xE5\xB7\xB1\xE9\x81\x8E\xE8\x90\xAC\xE9\x87\x8D\xE5\xB1\xB1\xE3\x80\x82\n\n\n\n\n"

        //Korean
        "\xEC\x9D\xB4\xEB\xAA\xB8\xEC\x9D\xB4\x2F\xEC\xA3\xBD\xEA\xB3\xA0\xEC\xA3\xBD\xEC\x96\xB4\x2F\xEC\x9D"
        "\xBC\xEB\xB0\xB1\xEB\xB2\x88\x2F\xEA\xB3\xA0\xEC\xB3\x90\xEC\xA3\xBD\xEC\x96\xB4\x2F\n"
        "\xEB\xB0\xB1\xEA\xB3\xA8\xEC\x9D\xB4\x2F\xEC\xA7\x84\xED\x87\xB4\xEB\x90\x98\xEC\x96\xB4"
        "\x2F\xEB\x84\x8B\xEC\x9D\xB4\xEB\x9D\xBC\xEB\x8F\x84\x2F\xEC\x9E\x88\xEA\xB3\xA0\xEC\x97\x86\xEA\xB3\xA0\x2F\n"
        "\xEC\x9E\x84\x20\xED\x96\xA5\xED\x95\x9C\x2F\xEC\x9D\xBC\xED\x8E\xB8\xEB\x8B\xA8\xEC\x8B\xAC\xEC"
        "\x9D\xB4\xEC\x95\xBC\x2F\xEA\xB0\x80\xEC\x8B\xA4\x20\xEC\xA4"
        "\x84\xEC\x9D\xB4\x2F\xEC\x9E\x88\xEC\x9C\xBC\xEB\x9E\xB4/",

        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "Hebrew using TnuaLibre font",
        "TnuaLibre-12",
        "1234 \xD7\xAA\xD7\xA0\xD7\x95\xD7\xA2\xD7\x94 5678\n"
        "1234 \xD7\x97\xD7\x95\xD7\xA4\xD7\xA9\xD7\x99 **\xD7\x90\xD7\x91\xD7\x92\xD7\x93 5678",

        CEGUI::DefaultParagraphDirection::RightToLeft,
        CEGUI::HorizontalTextFormatting::WordWrapRightAligned
    },
    {
        "Old German using Fette UNZ Fraktur font",
        "FetteUNZFraktur-20",
        "Heute back ich, morgen brau ich,\n"
        "\xC3\x9C  bermorgen hol ich mir der K\xC3\xB6 nigin ihr Kind;\n"
        "Ach, wie gut, dass niemand wei\xC3\x9F ,\n"
        "dass ich Rumpelstilzchen hei\xC3\x9F",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapCentreAligned
    },
    {
        "Latin using Old Fell Type font",
        "FellType-12.5",
        "\xE2\x98\x9E Non nobis, non nobis, Domine\n"
        "Sed nomini tuo da gloriam.\xE2\x98\x9C\n"
        "\xEE\x80\x9C\x20\xEE\x81\x87\x20\xEE\x81\x80\x20\xEE\x81\x81\x20\xC3\x90",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "Handwriting font",
        "GreatVibes-22",
        "Dear Gooby,\n\n"
        "Handwriting is nice when you don't have to do it yourself.\n\n"
        "Regards, Uncle Dolan.",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "RichStyle Icons font",
        "RichStyle-22",
        "\x2B\x20\x2D\x20\x3F\x20\x42\x20\x49\x20\x57\x20\x59\x20\x66\x20\x6E\x20\x74\x20\xE2"
        "\x84\xB9\x20\xE2\x87\xA6\x20\xE2\x87\xA7\x20\xE2\x87\xA8\x20\xE2\x87\xA9\x20\xE2\x8C"
        "\x98\x20\xE2\x98\x8E\x20\xE2\x98\x90\x20\xE2\x98\x91\x20\xE2\x9A\x96\x20\xE2\x9A\x99"
        "\x20\xE2\x9A\xA0\x20\xE2\x9B\x8F\x20\xE2\x9C\x8E\x20\xE2\x9C\x91\x20\xE2\x9C\x93\x20"
        "\xE2\x9C\x94\x20\xE2\x9C\x95\x20\xE2\x9C\x96\x20\xE2\x9D\x9D\x20\xE2\x9D\x9E\x20\xE2"
        "\x9E\xA1\x20\xE2\xAC\x80\x20\xE2\xAC\x81\x20\xE2\xAC\x82\x20\xE2\xAC\x83\x20\xE2\xAC"
        "\x85\x20\xE2\xAC\x86\x20\xE2\xAC\x87\x20\xE2\xAC\x88\x20\xE2\xAC\x89\x20\xE2\xAC\x8A"
        "\x20\xE2\xAC\x8B\x20\xEE\x80\x80\x20\xEE\x80\x81\x20\xEE\x80\x82\x20\xEE\x80\x83\x20"
        "\xEE\x80\x84\x20\xEE\x80\x85\x20\xEE\x80\x86\x20\xEE\x80\x87\x20\xEE\x80\x88\x20\xEE"
        "\x80\x89\x20\xEE\x80\x90\x20\xEE\x80\x91\x20\xEE\x80\x92\x20\xEE\x80\x93\x20\xEE\x80"
        "\x94\x20\xEE\x80\x95\x20\xEE\x80\x96\x20\xEE\x80\x97\x20\xEE\x80\x98\x20\xEE\x80\x99"
        "\x20\xEE\x80\xA0\x20\xEE\x80\xA1\x20\xEE\x80\xA2",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "Old Runic writing using Futhark Adapted font",
        "FutharkAdapted-18",
        "Somehow, although he is the smallest office boy around the place, none of the other"
        "lads pick on him. Scuffling and fighting almost has ceased since Kerensky came to work."
        "That's only one of the nicknames of Leo Kobreen, and was assigned to him because of a"
        "considerable facial resemblance to the perpetually fleeing Russian statesman, and, "
        "too, because both wore quite formal standing collars.",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "Klingon using pIqaD HaSta font",
        "Klingon-pIqaD-HaSta-24",
        "\xEF\xA3\xA3\xEF\xA3\x9D\xEF\xA3\x93\xEF\xA3\xA5\xEF\xA3\x98\x20\xEF\xA3\xA9\xEF"
        "\xA3\x9D\xEF\xA3\xA2\x20\xEF\xA3\xA1\xEF\xA3\x9D\xEF\xA3\x99\xEF\xA3\xBE\n\n\n"
        "\xEF\xA3\xA0\x20\xEF\xA3\xA1\x20\xEF\xA3\xA2\x20\xEF\xA3\xA3\x20\xEF\xA3\xA4\x20"
        "\xEF\xA3\xA5\x20\xEF\xA3\xA6\x20\xEF\xA3\xA7\x20\xEF\xA3\xA8\x20\xEF\xA3\xA9\n"
        "\xEF\xA3\xB0\x20\xEF\xA3\xB1\x20\xEF\xA3\xB2\x20\xEF\xA3\xB3\x20\xEF\xA3\xB4\x20"
        "\xEF\xA3\xB5\x20\xEF\xA3\xB6\x20\xEF\xA3\xB7\x20\xEF\xA3\xB8\x20\xEF\xA3\xB9\n"
        "\xEF\xA3\xBD\x20\xEF\xA3\xBE\x20\xEF\xA3\xBF",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    },
    {
        "Pixel style font using Mizufalp font",
        "mizufalp-12",
        "Mechanic: Somebody set up us the bomb.\n"
        "Operator: Main screen turn on.\n"
        "CATS: All your base are belong to us.\n"
        "CATS: You have no chance to survive make your time.",
        CEGUI::DefaultParagraphDirection::LeftToRight,
        CEGUI::HorizontalTextFormatting::WordWrapLeftAligned
    }
};

}


#endif