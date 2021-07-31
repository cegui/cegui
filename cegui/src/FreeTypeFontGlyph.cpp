/***********************************************************************
    created:    7th May 2016
    author:     Lukas Meindl <bitbucket@lukasmeindl.at>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2016 Paul D Turner & The CEGUI Development Team
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

#include "CEGUI/FreeTypeFontGlyph.h"

namespace CEGUI
{

float FreeTypeFontGlyph::getRenderedAdvance() const
{
    const Image* img = getImage();
    if (!img)
        return 0.f;

#ifdef CEGUI_USE_RAQM
    //TODO: This is incorrect, the estimate based on the advance should not be used when raqm is on
    float sizeX = img->getRenderedSize().d_width + img->getRenderedOffset().x;
#else
    float sizeX = img->getRenderedSize().d_width + img->getRenderedOffset().x;
#endif

    return sizeX;
}

void FreeTypeFontGlyph::markAsInitialised()
{
    d_initialised = true;
}

bool FreeTypeFontGlyph::isInitialised() const
{
    return d_initialised;
}

void FreeTypeFontGlyph::setLsbDelta(const long lsbDelta)
{
    d_lsbDelta = lsbDelta;
}

long FreeTypeFontGlyph::getLsbDelta() const
{
    return d_lsbDelta;
}

void FreeTypeFontGlyph::setRsbDelta(const long rsbDelta)
{
    d_rsbDelta = rsbDelta;
}

long FreeTypeFontGlyph::getRsbDelta() const
{
    return d_rsbDelta;
}

unsigned FreeTypeFontGlyph::getGlyphIndex() const
{
    return d_glyphIndex;
}

}
