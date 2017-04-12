/***********************************************************************
    created:    Wed, 8th Feb 2012
    author:     Lukas E Meindl (based on code by Paul D Turner)
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2012 Paul D Turner & The CEGUI Development Team
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/RendererModules/OpenGL/ShaderManager.h"
#include "CEGUI/RendererModules/OpenGL/GL3Renderer.h"
#include "CEGUI/RendererModules/OpenGL/GL3Texture.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/RendererModules/OpenGL/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGL/GL3GeometryBuffer.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/RendererModules/OpenGL/GL3FBOTextureTarget.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"
#include "CEGUI/RendererModules/OpenGL/GL3StateChangeWrapper.h"
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/RendererModules/OpenGL/GLBaseShaderWrapper.h"

#include <algorithm>

// Start of CEGUI namespace section
namespace CEGUI
{

//----------------------------------------------------------------------------//
// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGLTextureTargetFactory
{
    TextureTarget* create(OpenGLRendererBase& r, bool addStencilBuffer) const override
    { return new T(static_cast<OpenGL3Renderer&>(r), addStencilBuffer); }
};

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

    OpenGL3Renderer& renderer(create());
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::bootstrapSystem(const Sizef& display_size,
                                                  const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

    OpenGL3Renderer& renderer(create(display_size));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroySystem()
{
    System* sys = System::getSingletonPtr();
    if (sys == nullptr)
    {
        throw InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed.");
    }

    OpenGL3Renderer* renderer = static_cast<OpenGL3Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::create(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGL3Renderer();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::create(const Sizef& display_size,
                                         const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGL3Renderer(display_size);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroy(OpenGL3Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::OpenGL3Renderer() :
    OpenGLRendererBase(true),
    d_shaderWrapperTextured(nullptr),
    d_openGLStateChanger(nullptr),
    d_shaderManager(nullptr)
{
    init();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::OpenGL3Renderer(const Sizef& display_size) :
    OpenGLRendererBase(display_size, true),
    d_shaderWrapperTextured(nullptr),
    d_openGLStateChanger(nullptr),
    d_shaderManager(nullptr)
{
    init();
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::init()
{
    if (OpenGLInfo::getSingleton().isUsingOpenglEs()
        &&  OpenGLInfo::getSingleton().verMajor() < 2)
        throw RendererException("Only version 2 and up of OpenGL ES is "
                                "supported by this type of renderer.");
    initialiseRendererIDString();
    d_openGLStateChanger = new OpenGL3StateChangeWrapper();
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::~OpenGL3Renderer()
{
    delete d_textureTargetFactory;
    delete d_openGLStateChanger;
    delete d_shaderManager;

    delete d_shaderWrapperTextured;
    delete d_shaderWrapperSolid;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseRendererIDString()
{
    d_rendererID = OpenGLInfo::getSingleton().isUsingDesktopOpengl()
        ?  "CEGUI::OpenGL3Renderer - Official OpenGL 3.2 core based "
           "renderer module."
        :  "CEGUI::OpenGL3Renderer - OpenGL ES 2 renderer module.";
}
//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase* OpenGL3Renderer::createGeometryBuffer_impl(CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    return new OpenGL3GeometryBuffer(*this, renderMaterial);
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGL3Renderer::createTextureTarget_impl(bool addStencilBuffer)
{
    return d_textureTargetFactory->create(*this, addStencilBuffer);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::beginRendering()
{
    // Deprecated OpenGL 2 client states may mess up rendering. They are not added here
    // since they are deprecated and thus do not fit in a OpenGL Core renderer. However
    // this information may be relevant for people combining deprecated and modern
    // functions. In that case disable client states like this: glDisableClientState(GL_VERTEX_ARRAY);

    d_openGLStateChanger->reset();

    // if enabled, restores a subset of the GL state back to default values.
    if (d_isStateResettingEnabled)
        restoreChangedStatesToDefaults(false);

    d_openGLStateChanger->enable(GL_SCISSOR_TEST);
    d_openGLStateChanger->enable(GL_BLEND);

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BlendMode::Normal, true);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::endRendering()
{
    if (d_isStateResettingEnabled)
        restoreChangedStatesToDefaults(true);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::restoreChangedStatesToDefaults(bool isAfterRendering)
{
    //Resetting to initial values of the functions
    d_openGLStateChanger->activeTexture(0);
    d_openGLStateChanger->bindTexture(GL_TEXTURE_2D, 0);

    if (OpenGLInfo::getSingleton().isPolygonModeSupported())
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
 
    d_openGLStateChanger->disable(GL_CULL_FACE);
    d_openGLStateChanger->disable(GL_DEPTH_TEST);

    // During the preparation before rendering, these states will be changed anyways
    // so we do not want to change them extra
    if(isAfterRendering)
    { 
        d_openGLStateChanger->disable(GL_BLEND);
        d_openGLStateChanger->disable(GL_SCISSOR_TEST);
    }

    d_openGLStateChanger->blendFunc(GL_ONE, GL_ZERO);

    d_openGLStateChanger->useProgram(0);
    if (OpenGLInfo::getSingleton().isVaoSupported())
        d_openGLStateChanger->bindVertexArray(0);
    d_openGLStateChanger->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    d_openGLStateChanger->bindBuffer(GL_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseTextureTargetFactory()
{
    //Use OGL core implementation for FBOs
    d_rendererID += "  TextureTarget support enabled via FBO OGL 3.2 core implementation.";
    d_textureTargetFactory = new OGLTemplateTargetFactory<OpenGL3FBOTextureTarget>;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::setupRenderingBlendMode(const BlendMode mode,
                                              const bool force)
{
    // exit if mode is already set up (and update not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;

    if (d_activeBlendMode == BlendMode::RttPremultiplied)
    {
        d_openGLStateChanger->blendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        d_openGLStateChanger->blendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    }
}

//----------------------------------------------------------------------------//
Sizef OpenGL3Renderer::getAdjustedTextureSize(const Sizef& sz)
{
    return Sizef(sz);
}

//----------------------------------------------------------------------------//
OpenGLBaseStateChangeWrapper* OpenGL3Renderer::getOpenGLStateChanger()
{
    return d_openGLStateChanger;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseOpenGLShaders()
{
    checkGLErrors(__FILE__, __LINE__, CEGUI_FUNCTION_NAME);
    d_shaderManager = new OpenGLBaseShaderManager(d_openGLStateChanger, ShaderVersion::Glsl);
    d_shaderManager->initialiseShaders();

    initialiseStandardTexturedShaderWrapper();
    initialiseStandardColouredShaderWrapper();
}

//----------------------------------------------------------------------------//
RefCounted<RenderMaterial> OpenGL3Renderer::createRenderMaterial(const DefaultShaderType shaderType) const
{
    if(shaderType == DefaultShaderType::Textured)
    {
        RefCounted<RenderMaterial> render_material(new RenderMaterial(d_shaderWrapperTextured));

        return render_material;
    }
    else if(shaderType == DefaultShaderType::Solid)
    {
        RefCounted<RenderMaterial> render_material(new RenderMaterial(d_shaderWrapperSolid));

        return render_material;
    }
    else
    {
        throw RendererException(
            "A default shader of this type does not exist.");

        return RefCounted<RenderMaterial>();
    }
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseStandardTexturedShaderWrapper()
{
    OpenGLBaseShader* shader_standard_textured =  d_shaderManager->getShader(OpenGLBaseShaderID::StandardTextured);
    d_shaderWrapperTextured = new OpenGLBaseShaderWrapper(*shader_standard_textured, d_openGLStateChanger);

    d_shaderWrapperTextured->addTextureUniformVariable("texture0", 0);

    d_shaderWrapperTextured->addUniformVariable("modelViewProjMatrix");
    d_shaderWrapperTextured->addUniformVariable("alphaFactor");

    d_shaderWrapperTextured->addAttributeVariable("inPosition");
    d_shaderWrapperTextured->addAttributeVariable("inTexCoord");
    d_shaderWrapperTextured->addAttributeVariable("inColour");
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseStandardColouredShaderWrapper()
{
    OpenGLBaseShader* shader_standard_solid =  d_shaderManager->getShader(OpenGLBaseShaderID::StandardSolid);
    d_shaderWrapperSolid = new OpenGLBaseShaderWrapper(*shader_standard_solid, d_openGLStateChanger);

    d_shaderWrapperSolid->addUniformVariable("modelViewProjMatrix");
    d_shaderWrapperSolid->addUniformVariable("alphaFactor");

    d_shaderWrapperSolid->addAttributeVariable("inPosition");
    d_shaderWrapperSolid->addAttributeVariable("inColour");
}

//----------------------------------------------------------------------------//
OpenGLTexture* OpenGL3Renderer::createTexture_impl(const String& name)
{
    return new OpenGL3Texture(*this, name);
}  

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
