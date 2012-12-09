/***********************************************************************
    filename:   CEGUIDirect3D10Renderer.h
    created:    Sat Mar 7 2009
    author:     Paul D Turner (parts based on code by Rajko Stojadinovic)
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
#ifndef _CEGUIDirect3D10Renderer_h_
#define _CEGUIDirect3D10Renderer_h_

#include "../../CEGUIRenderer.h"
#include "../../CEGUISize.h"
#include "../../CEGUIVector.h"
#include <vector>

#if (defined( __WIN32__ ) || defined( _WIN32 )) && !defined(CEGUI_STATIC)
#   ifdef DIRECT3D10_GUIRENDERER_EXPORTS
#       define D3D10_GUIRENDERER_API __declspec(dllexport)
#   else
#       define D3D10_GUIRENDERER_API __declspec(dllimport)
#   endif
#else
#   define D3D10_GUIRENDERER_API
#endif

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// D3D forward refs
struct ID3D10Device;
struct ID3D10Effect;
struct ID3D10EffectTechnique;
struct ID3D10InputLayout;
struct ID3D10EffectShaderResourceVariable;
struct ID3D10EffectMatrixVariable;
struct ID3D10ShaderResourceView;
struct D3DXMATRIX;

// Start of CEGUI namespace section
namespace CEGUI
{
class Direct3D10GeometryBuffer;
class Direct3D10Texture;

//! Renderer implementation using Direct3D 10.
class D3D10_GUIRENDERER_API Direct3D10Renderer : public Renderer
{
public:
    /*!
    \brief
        Convenience function that creates the required objects to initialise the
        CEGUI system.

        This will create and initialise the following objects for you:
        - CEGUI::Direct3D10Renderer
        - CEGUI::DefaultResourceProvider
        - CEGUI::System

    \param device
        Pointer to the ID3D10Device interface that is to be used for CEGUI
        rendering operations.

    \return
        Reference to the CEGUI::Direct3D10Renderer object that was created.
    */
    static Direct3D10Renderer& bootstrapSystem(ID3D10Device* device);

    /*!
    \brief
        Convenience function to cleanup the CEGUI system and related objects
        that were created by calling the bootstrapSystem function.

        This function will destroy the following objects for you:
        - CEGUI::System
        - CEGUI::DefaultResourceProvider
        - CEGUI::Direct3D10Renderer

    \note
        If you did not initialise CEGUI by calling the bootstrapSystem function,
        you should \e not call this, but rather delete any objects you created
        manually.
    */
    static void destroySystem();

    /*!
    \brief
        Create an Direct3D10Renderer object.
    */
    static Direct3D10Renderer& create(ID3D10Device* device);

    /*!
    \brief
        Destroy an Direct3D10Renderer object.

    \param renderer
        The Direct3D10Renderer object to be destroyed.
    */
    static void destroy(Direct3D10Renderer& renderer);

    //! return the ID3D10Device used by this renderer object.
    ID3D10Device& getDirect3DDevice() const;

    //! low-level function that binds the technique pass ready for use
    void bindTechniquePass(const BlendMode mode);
    //! low-level function to set the texture shader resource view to be used.
    void setCurrentTextureShaderResource(ID3D10ShaderResourceView* srv); 
    //! low-level function to set the projection matrix to be used.
    void setProjectionMatrix(D3DXMATRIX& matrix);
    //! low-level function to set the world matrix to be used.
    void setWorldMatrix(D3DXMATRIX& matrix);

    // Implement interface from Renderer
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
    //! constructor
    Direct3D10Renderer(ID3D10Device* device);

    //! destructor.
    ~Direct3D10Renderer();

    //! return size of the D3D device viewport.
    Size getViewportSize();

    //! String holding the renderer identification text.
    static String d_rendererID;
    //! The D3D device we're using to render with.
    ID3D10Device* d_device;
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
    typedef std::vector<Direct3D10GeometryBuffer*> GeometryBufferList;
    //! Container used to track geometry buffers.
    GeometryBufferList d_geometryBuffers;
    //! container type used to hold Textures we create.
    typedef std::vector<Direct3D10Texture*> TextureList;
    //! Container used to track textures.
    TextureList d_textures;
    //! Effect (shader) used when rendering.
    ID3D10Effect* d_effect;
    //! Rendering technique that supplies BM_NORMAL type rendering
    ID3D10EffectTechnique* d_normalTechnique;
    //! Rendering technique that supplies BM_RTT_PREMULTIPLIED type rendering
    ID3D10EffectTechnique* d_premultipliedTechnique;
    //! D3D10 input layout describing the vertex format we use.
    ID3D10InputLayout* d_inputLayout;
    //! Variable to access current texture (actually shader resource view)
    ID3D10EffectShaderResourceVariable* d_boundTextureVariable;
    //! Variable to access world matrix used in geometry transformation.
    ID3D10EffectMatrixVariable* d_worldMatrixVariable;
    //! Variable to access projection matrix used in geometry transformation.
    ID3D10EffectMatrixVariable* d_projectionMatrixVariable;
};


} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIDirect3D10Renderer_h_
