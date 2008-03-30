/***********************************************************************
    filename:   CEGUIOpenGLTextureTarget.cpp
    created:    Mon Feb 18 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#include "CEGUIExceptions.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
const float OpenGLTextureTarget::DEFAULT_SIZE(128.0f);

//----------------------------------------------------------------------------//
OpenGLTextureTarget::OpenGLTextureTarget() :
    d_texture(0)
{
    discoverTextureCapabilities();
}

//----------------------------------------------------------------------------//
OpenGLTextureTarget::~OpenGLTextureTarget()
{
    if (d_texture)
        glDeleteTextures(1, &d_texture);
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::queueToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    if (!d_queueEnabled)
    {
        renderToTarget(src, dst, z, cols, quad_split_mode, target);
        return;
    }

    Rect tex_rect;
    initTexRectFromPixRect(src, tex_rect);

    Quad quad;
    // _initialise_quad must be called on the destination to ensure that the
    // geometry is set up in terms of the destination's area and not our own.
    static_cast<OpenGLRenderTarget&>(target).
        _initialise_quad(quad, dst, z, d_texture, tex_rect,
                         quad_split_mode, cols);

    static_cast<OpenGLRenderTarget&>(target)._queue_quad(quad);
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::renderToTarget(const Rect& src, const Rect& dst,
    float z, const ColourRect& cols, const QuadSplitMode quad_split_mode,
    RenderTarget& target)
{
    Rect tex_rect;
    initTexRectFromPixRect(src, tex_rect);

    Quad quad;
    // _initialise_quad must be called on the destination to ensure that the
    // geometry is set up in terms of the destination's area and not our own.
    static_cast<OpenGLRenderTarget&>(target).
        _initialise_quad(quad, dst, z, d_texture, tex_rect,
                         quad_split_mode, cols);

    static_cast<OpenGLRenderTarget&>(target)._render_quad(quad);
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::setArea(const Rect& area)
{
    // do nothing for this at the moment.
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::initTexRectFromPixRect(const Rect& pix_rect,
    Rect& tex_rect) const
{
    tex_rect.d_left = pix_rect.d_left / d_area.getWidth();
    tex_rect.d_right = pix_rect.d_right / d_area.getWidth();
    tex_rect.d_top = 1 - pix_rect.d_top / d_area.getHeight();
    tex_rect.d_bottom = 1 - pix_rect.d_bottom / d_area.getHeight();
}

//----------------------------------------------------------------------------//
void OpenGLTextureTarget::discoverTextureCapabilities()
{
    // get (roughly) the maximum supported texture size
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &d_maxTextureSize);

    d_supportsNPOT =
        OpenGLRenderer::isExtensionSupported("GL_ARB_texture_non_power_of_two");
    d_mustBeSquare = false;
}

//----------------------------------------------------------------------------//
Size OpenGLTextureTarget::getAdjustedTextureSize(const Size& sz) const
{
    // add some room to manoeuvre (this maybe a little hackish)
    int new_width = static_cast<int>(sz.d_width) + 128;
    int new_height = static_cast<int>(sz.d_height) + 128;

    // make dimensions power of two if that is required
    if (!d_supportsNPOT)
    {
        new_width = OpenGLRenderer::getNextPowerOfTwo(new_width);
        new_height = OpenGLRenderer::getNextPowerOfTwo(new_height);
    }

    // make dimensions square if that is required
    if (d_mustBeSquare)
        new_width = new_height = ceguimax(new_width, new_height);

    // see if such a texture is supported (probably)
    if ((new_width > d_maxTextureSize) || (new_height > d_maxTextureSize))
        throw InvalidRequestException("Requested texture size is greater than "
                                      "maximum allowed as reported by OpenGL.");

    return Size(static_cast<float>(new_width), static_cast<float>(new_height));
}

//----------------------------------------------------------------------------//
bool OpenGLTextureTarget::isImageryCache() const
{
    return true;
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
