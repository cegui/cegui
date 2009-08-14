/***********************************************************************
    filename:   CEGUIDirectFBRenderer.h
    created:    Tue Mar 10 2009
    author:     Paul D Turner (parts based on code by Keith Mok)
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
#ifndef _CEGUIDirectFBRenderer_h_
#define _CEGUIDirectFBRenderer_h_

#include "../../CEGUIRenderer.h"
#include "../../CEGUISize.h"
#include "../../CEGUIVector.h"
#include <directfb.h>
#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
class DirectFBTexture;
class DirectFBGeometryBuffer;

//! Implementation of CEGUI::Renderer interface using DirectFB.
class DirectFBRenderer : public Renderer
{
public:
    //! create a DirectFBRenderer object.
    static DirectFBRenderer& create(IDirectFB& directfb,
                                    IDirectFBSurface& surface);
    //! destroy a DirectFBRenderer object created by the \a create call.
    static void destroy(DirectFBRenderer& renderer);

    //! Return the current target DirectFB surface.
    IDirectFBSurface& getTargetSurface() const;

    //! Set the target DirectFB surface.
    void setTargetSurface(IDirectFBSurface& surface);

    // Implementation of Renderer interface.
    RenderingRoot& getDefaultRenderingRoot();
    GeometryBuffer& createGeometryBuffer();
    void destroyGeometryBuffer(const GeometryBuffer& buffer);
    void destroyAllGeometryBuffers();
    TextureTarget* createTextureTarget();
    void destroyTextureTarget(TextureTarget* target);
    void destroyAllTextureTargets();
    Texture& createTexture();
    Texture& createTexture(const String& filename, const String& resourceGroup);
    Texture& createTexture(const Size& size);
    void destroyTexture(Texture& texture);
    void destroyAllTextures();
    void beginRendering();
    void endRendering();
    void setDisplaySize(const Size& sz);
    const Size& getDisplaySize() const;
    const Vector2& getDisplayDPI() const;
    uint getMaxTextureSize() const;
    const String& getIdentifierString() const;

protected:
    //! Constructor.
    DirectFBRenderer(IDirectFB& directfb, IDirectFBSurface& surface);
    //! Destructor.
    ~DirectFBRenderer();

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! DirectFB interface we were given when constructed.
    IDirectFB& d_directfb;
    //! The DirectFB surface to be used as the default root target.
    IDirectFBSurface& d_rootSurface;
    //! The current target DirectFB surface.
    IDirectFBSurface* d_targetSurface;
    //! What the renderer considers to be the current display size.
    Size d_displaySize;
    //! What the renderer considers to be the current display DPI resolution.
    Vector2 d_displayDPI;
    //! The default RenderTarget (used by d_defaultRoot)
    RenderTarget* d_defaultTarget;
    //! The default rendering root object
    RenderingRoot* d_defaultRoot;
    //! container type used to hold TextureTargets we create.
     typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<DirectFBGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::vector<DirectFBTexture*> TextureList;
    //! Container used to track textures.
    TextureList d_textures;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIDirectFBRenderer_h_
