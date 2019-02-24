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
#ifndef _CEGUINullTextureTarget_h_
#define _CEGUINullTextureTarget_h_

#include "../../TextureTarget.h"
#include "CEGUI/RendererModules/Null/RenderTarget.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4250)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! CEGUI::TextureTarget implementation for the Null engine.
class NULL_GUIRENDERER_API NullTextureTarget : public NullRenderTarget, public TextureTarget
{
public:
    //! Constructor.
    NullTextureTarget(NullRenderer& owner, bool addStencilBuffer);
    //! Destructor.
    virtual ~NullTextureTarget();

    // implementation of RenderTarget interface
    bool isImageryCache() const override;
    // implement CEGUI::TextureTarget interface.
    void clear() override;
    Texture& getTexture() const override;
    void declareRenderSize(const Sizef& sz) override;

protected:
    //! helper to generate unique texture names
    static String generateTextureName();
    //! static data used for creating texture names
    static std::uint32_t s_textureNumber;
    //! default / initial size for the underlying texture.
    static const float DEFAULT_SIZE;
    //! This wraps d_texture so it can be used by the core CEGUI lib.
    NullTexture* d_CEGUITexture;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUINullTextureTarget_h_
