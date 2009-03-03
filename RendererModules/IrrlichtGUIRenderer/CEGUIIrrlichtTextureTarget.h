/***********************************************************************
    filename:   CEGUIIrrlichtTextureTarget.h
    created:    Tue Mar 3 2009
    author:     Paul D Turner (parts based on original code by Thomas Suter)
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
#ifndef _CEGUIIrrlichtTextureTarget_h_
#define _CEGUIIrrlichtTextureTarget_h_

#include "CEGUIIrrlichtRendererDef.h"
#include "CEGUITextureTarget.h"
#include "CEGUIIrrlichtRenderTarget.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4250)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! CEGUI::TextureTarget implementation for the Irrlicht engine.
class IRR_GUIRENDERER_API IrrlichtTextureTarget : public IrrlichtRenderTarget,
                                                  public TextureTarget
{
public:
    //! Constructor.
    IrrlichtTextureTarget(IrrlichtRenderer& owner);
    //! Destructor.
    virtual ~IrrlichtTextureTarget();

    // implementation of RenderTarget interface
    bool isImageryCache() const;
    // implement CEGUI::TextureTarget interface.
    void clear();
    Texture& getTexture() const;
    void declareRenderSize(const Size& sz);
    bool isRenderingInverted() const;

protected:
};

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIIrrlichtTextureTarget_h_
