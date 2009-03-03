/***********************************************************************
    filename:   CEGUIIrrlichtTexture.cpp
    created:    Tue Mar 3 2009
    author:     Paul D Turner (based on original code by Thomas Suter)
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
#include "CEGUIIrrlichtTexture.h"
#include "CEGUIIrrlichtRenderer.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const Size& IrrlichtTexture::getSize() const
{
}

//----------------------------------------------------------------------------//
const Size& IrrlichtTexture::getOriginalDataSize() const
{
}

//----------------------------------------------------------------------------//
const Vector2& IrrlichtTexture::getTexelScaling() const
{
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::loadFromFile(const String& filename,
                                   const String& resourceGroup)
{
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::loadFromMemory(const void* buffer,
                                     const Size& buffer_size,
                                     PixelFormat pixel_format)
{
}

//----------------------------------------------------------------------------//
void IrrlichtTexture::saveToMemory(void* buffer)
{
}

//----------------------------------------------------------------------------//
IrrlichtTexture::IrrlichtTexture(IrrlichtRenderer& owner) :
    Texture(&owner)
{
}

//----------------------------------------------------------------------------//
IrrlichtTexture::~IrrlichtTexture()
{
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
