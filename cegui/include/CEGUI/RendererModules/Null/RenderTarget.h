/***********************************************************************
    created:    Sat Jan 16 2010
    author:     Eugene Marcotte
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUINullRenderTarget_h_
#define _CEGUINullRenderTarget_h_

#include "../../RenderTarget.h"
#include "CEGUI/RendererModules/Null/Renderer.h"
#include "../../Rectf.h"

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//! Intermediate RenderTarget 
class NULL_GUIRENDERER_API NullRenderTarget : virtual public RenderTarget
{
public:
    //! Constructor
    NullRenderTarget(NullRenderer& owner);

    //! Destructor
    virtual ~NullRenderTarget();

    // implement parts of CEGUI::RenderTarget interface
    void activate() override;
    void unprojectPoint(const GeometryBuffer& buff,
                        const glm::vec2& p_in, glm::vec2& p_out) const override;
    bool isImageryCache() const override;
    // implementing the virtual function with a covariant return type
    NullRenderer& getOwner() override;

protected:
    //! NullRenderer object that owns this RenderTarget
    NullRenderer& d_owner;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUINullRenderTarget_h_
