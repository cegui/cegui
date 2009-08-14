/***********************************************************************
    filename:   CEGUIIrrlichtRenderer.h
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
#ifndef _CEGUIIrrlichtRenderer_h_
#define _CEGUIIrrlichtRenderer_h_

#include "CEGUIIrrlichtRendererDef.h"
#include "../../CEGUIRenderer.h"
#include "../../CEGUISize.h"
#include "../../CEGUIVector.h"

#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// forward reference irrlicht classes
namespace irr
{
class IrrlichtDevice;
struct SEvent;

namespace video
{
class IVideoDriver;
}

namespace io
{
class IFileSystem;
}

}

// Start of CEGUI namespace section
namespace CEGUI
{
class IrrlichtTexture;
class IrrlichtGeometryBuffer;
class IrrlichtResourceProvider;
class IrrlichtEventPusher;
class RenderTarget;

//! CEGUI::Renderer implementation for the Irrlicht engine.
class IRR_GUIRENDERER_API IrrlichtRenderer : public Renderer
{
public:
    //! Function to create and return IrrlichtRenderer objects
    static IrrlichtRenderer& create(irr::IrrlichtDevice& device);

    //! Function to destroy IrrlichtRenderer objects.
    static void destroy(IrrlichtRenderer& renderer);

    //! Create a IrrlichtResourceProvider object.
    static IrrlichtResourceProvider&
        createIrrlichtResourceProvider(irr::io::IFileSystem& fs);

    //! Destroy a IrrlichtResourceProvider object.
    static void destroyIrrlichtResourceProvider(IrrlichtResourceProvider& rp);

    //! inject irrlicht event to CEGUI system
    bool injectEvent(const irr::SEvent& event);

    // implement Renderer interface
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
    //! Constructor
    IrrlichtRenderer(irr::IrrlichtDevice& device);
    //! Destructor
    ~IrrlichtRenderer();

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! The IrrlichtDevide that we'll be using.
    irr::IrrlichtDevice& d_device;
    //! Irrlicht video driver (as obtained from the device)
    irr::video::IVideoDriver* d_driver;
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
    typedef std::vector<IrrlichtGeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::vector<IrrlichtTexture*> TextureList;
    //! Container used to track textures.
    TextureList d_textures;
    //! What the renderer thinks the max texture size is.
    uint d_maxTextureSize;
    //! ptr to helper object that aids in injection of events from Irrlicht.
    IrrlichtEventPusher* d_eventPusher;

};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIIrrlichtRenderer_h_
