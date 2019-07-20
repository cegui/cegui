/***********************************************************************
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

#include "CEGUI/Renderer.h"
#include "CEGUI/Sizef.h"
#include "CEGUI/Vector.h"
#include <directfb.h>
#include <vector>
#include <unordered_map>

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
                                    IDirectFBSurface& surface,
                                    const int abi = CEGUI_VERSION_ABI);
    //! destroy a DirectFBRenderer object created by the \a create call.
    static void destroy(DirectFBRenderer& renderer);

    //! Return the current target DirectFB surface.
    IDirectFBSurface& getTargetSurface() const;

    //! Set the target DirectFB surface.
    void setTargetSurface(IDirectFBSurface& surface);

    // Implementation of Renderer interface.
    virtual RenderTarget& getDefaultRenderTarget();
    virtual GeometryBuffer& createGeometryBuffer();
    virtual void destroyGeometryBuffer(const GeometryBuffer& buffer);
    virtual void destroyAllGeometryBuffers();
    virtual TextureTarget* createTextureTarget(bool addStencilBuffer);
    virtual void destroyTextureTarget(TextureTarget* target);
    virtual void destroyAllTextureTargets();
    virtual Texture& createTexture(const CEGUI::String& name);
    virtual Texture& createTexture(const CEGUI::String& name,
                           const String& filename,
                           const String& resourceGroup);
    virtual Texture& createTexture(const CEGUI::String& name, const Sizef& size);
    virtual void destroyTexture(Texture& texture);
    virtual void destroyTexture(const CEGUI::String& name);
    virtual void destroyAllTextures();
    virtual Texture& getTexture(const String&) const;
    virtual bool isTextureDefined(const String& name) const;
    virtual void beginRendering();
    virtual void endRendering();
    virtual void setDisplaySize(const Sizef& sz);
    virtual const Sizef& getDisplaySize() const;
    virtual unsigned int getMaxTextureSize() const;
    virtual const String& getIdentifierString() const;
    virtual bool isTexCoordSystemFlipped() const;

protected:
    //! Constructor.
    DirectFBRenderer(IDirectFB& directfb, IDirectFBSurface& surface);
    //! Destructor.
    ~DirectFBRenderer();

    //! helper to safely log the creation of a named texture
    static void logTextureCreation(DirectFBTexture* texture);
    //! helper to safely log the destruction of a named texture
    static void logTextureDestruction(DirectFBTexture* texture);

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! DirectFB interface we were given when constructed.
    IDirectFB& d_directfb;
    //! The DirectFB surface to be used as the default root target.
    IDirectFBSurface& d_rootSurface;
    //! The current target DirectFB surface.
    IDirectFBSurface* d_targetSurface;
    //! What the renderer considers to be the current display size.
    Sizef d_displaySize;
    //! The default RenderTarget
    RenderTarget* d_defaultTarget;
    //! container type used to hold TextureTargets we create.
    typedef std::vector<TextureTarget*> TextureTargetList;
    //! Container used to track texture targets.
    TextureTargetList d_textureTargets;
    //! container type used to hold GeometryBuffers we create.
    typedef std::vector<DirectFBGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::unordered_map<String, DirectFBTexture*> TextureMap;
    //! Container used to track textures.
    TextureMap d_textures;
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIDirectFBRenderer_h_
