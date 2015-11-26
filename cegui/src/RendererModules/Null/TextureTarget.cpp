/***********************************************************************
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
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
#include "CEGUI/RendererModules/Null/TextureTarget.h"
#include "CEGUI/RendererModules/Null/Texture.h"
#include "CEGUI/PropertyHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
uint NullTextureTarget::s_textureNumber = 0;
const float NullTextureTarget::DEFAULT_SIZE = 128.0f;

//----------------------------------------------------------------------------//
NullTextureTarget::NullTextureTarget(NullRenderer& owner) :
    NullRenderTarget<TextureTarget>(owner),
    d_CEGUITexture(0)
{
    d_CEGUITexture = static_cast<NullTexture*>(
        &d_owner.createTexture(generateTextureName()));

    // setup area and cause the initial texture to be generated.
    declareRenderSize(Sizef(DEFAULT_SIZE, DEFAULT_SIZE));
}

//----------------------------------------------------------------------------//
NullTextureTarget::~NullTextureTarget()
{
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
bool NullTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
void NullTextureTarget::clear()
{
}

//----------------------------------------------------------------------------//
Texture& NullTextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
void NullTextureTarget::declareRenderSize(const Sizef& sz)
{
	Rectf r;
	r.setSize(sz);
	r.setPosition(Vector2f(0, 0));
    setArea(r);
}

//----------------------------------------------------------------------------//
bool NullTextureTarget::isRenderingInverted() const
{
    return false;
}

//----------------------------------------------------------------------------//
String NullTextureTarget::generateTextureName()
{
    String tmp("_null_tt_tex_");
    tmp.append(PropertyHelper<uint>::toString(s_textureNumber++));

    return tmp;
}
//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

//----------------------------------------------------------------------------//
// Implementation of template base class
#include "./RenderTarget.inl"

