/***********************************************************************
    created:    Sun Jan 11 2009
    author:     Paul D Turner
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
#ifndef _CEGUIOpenGL1Texture_h_
#define _CEGUIOpenGL1Texture_h_

#include "CEGUI/RendererModules/OpenGL/Texture.h"
#include "CEGUI/RendererModules/OpenGL/RendererBase.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! Texture implementation for the OpenGLRenderer.
class OPENGL_GUIRENDERER_API OpenGL1Texture : public OpenGLTexture
{
public:
    OpenGL1Texture(OpenGLRendererBase& owner, const String& name);
    virtual ~OpenGL1Texture();

    void blitToMemory(void* targetData) override;

protected:
    //! OpenGL method to set glTexEnv which is deprecated in GL 3.2 and GLES 2.0 and above
    void setTextureEnvironment() override;

    //! initialise the internal format flags for the given CEGUI::PixelFormat.
    void initInternalPixelFormatFields(const PixelFormat fmt) override;

    //! internal texture resize function (does not reset format or other fields)
    void setTextureSize_impl(const Sizef& sz) override;

    GLsizei getCompressedTextureSize(const Sizef& pixel_size) const override;

};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif // end of guard _CEGUIOpenGL1Texture_h_
