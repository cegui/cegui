/***********************************************************************
    filename:   CEGUIOpenGLTextureTarget.cpp
    created:    Thu Oct 15 2009
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "CEGUIOpenGLTextureTarget.h"
#include "CEGUIOpenGLTexture.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
OpenGLTextureTarget::OpenGLTextureTarget(OpenGLRenderer& owner) :
    OpenGLRenderTarget(owner),
    d_texture(0)
{
    // this essentially creates a 'null' CEGUI::Texture
    d_CEGUITexture = &static_cast<OpenGLTexture&>(
        d_owner.createTexture(d_texture, d_area.getSize()));
}

//----------------------------------------------------------------------------//
OpenGLTextureTarget::~OpenGLTextureTarget()
{
    d_owner.destroyTexture(*d_CEGUITexture);
}

//----------------------------------------------------------------------------//
bool OpenGLTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//
Texture& OpenGLTextureTarget::getTexture() const
{
    return *d_CEGUITexture;
}

//----------------------------------------------------------------------------//
bool OpenGLTextureTarget::isRenderingInverted() const
{
    return true;
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::grabTexture()
{
    if (d_CEGUITexture)
    {
        d_owner.destroyTexture(*d_CEGUITexture);
        d_texture = 0;
        d_CEGUITexture = 0;
    }
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::restoreTexture()
{
    if (d_CEGUITexture)
        return;

    d_CEGUITexture = &static_cast<OpenGLTexture&>(
        d_owner.createTexture(d_texture, d_area.getSize()));
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
