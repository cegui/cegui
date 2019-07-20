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
#include "CEGUI/RendererModules/OpenGL/ShaderManager.h"
#include "CEGUI/RendererModules/OpenGL/GLES2Renderer.h"
#include "CEGUI/RendererModules/OpenGL/GLES2Texture.h"
#include "CEGUI/RendererModules/OpenGL/Shader.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/RendererModules/OpenGL/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGL/GLES2GeometryBuffer.h"
#include "CEGUI/GUIContext.h"
#include "CEGUI/RendererModules/OpenGL/GLES2FBOTextureTarget.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"
#include "CEGUI/RendererModules/OpenGL/GLES2StateChangeWrapper.h"
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/RendererModules/OpenGL/GLBaseShaderWrapper.h"

#include <sstream>
#include <algorithm>
#include <cstring>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// The following are some GL extension / version dependant related items.
// This is all done totally internally here; no need for external interface
// to show any of this.
//----------------------------------------------------------------------------//
// we only really need this with MSVC / Windows(?) and by now it should already
// be defined on that platform, so we just define it as empty macro so the
// compile does not break on other systems.
#ifndef APIENTRY
#   define APIENTRY
#endif
//! Dummy function for if real ones are not present (saves testing each render)
static void APIENTRY activeTextureDummy(GLenum) {}

//----------------------------------------------------------------------------//
// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGLTextureTargetFactory
{
    TextureTarget* create(OpenGLRendererBase& r) const
        { return new T(static_cast<GLES2Renderer&>(r)); }
};

//----------------------------------------------------------------------------//
GLES2Renderer& GLES2Renderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

    GLES2Renderer& renderer(create());
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
GLES2Renderer& GLES2Renderer::bootstrapSystem(const Sizef& display_size,
                                                  const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

    GLES2Renderer& renderer(create(display_size));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void GLES2Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        throw InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed.");

    GLES2Renderer* renderer = static_cast<GLES2Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
GLES2Renderer& GLES2Renderer::create(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new GLES2Renderer();
}

//----------------------------------------------------------------------------//
GLES2Renderer& GLES2Renderer::create(const Sizef& display_size,
                                         const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new GLES2Renderer(display_size);
}

//----------------------------------------------------------------------------//
void GLES2Renderer::destroy(GLES2Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
GLES2Renderer::GLES2Renderer() :
    d_shaderWrapperTextured(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    initialiseRendererIDString();
    initialiseGLExtensions();
    d_openGLStateChanger = new GLES2StateChangeWrapper();
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();
}

//----------------------------------------------------------------------------//
GLES2Renderer::GLES2Renderer(const Sizef& display_size) :
    OpenGLRendererBase(display_size),
    d_shaderWrapperTextured(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    initialiseRendererIDString();
    initialiseGLExtensions();
    d_openGLStateChanger = new GLES2StateChangeWrapper();
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();
}

//----------------------------------------------------------------------------//
GLES2Renderer::~GLES2Renderer()
{
    delete d_textureTargetFactory;
    delete d_openGLStateChanger;
    delete d_shaderManager;

    delete d_shaderWrapperTextured;
    delete d_shaderWrapperSolid;
}

//----------------------------------------------------------------------------//
void GLES2Renderer::initialiseRendererIDString()
{
    d_rendererID =
#ifdef CEGUI_GLES3_SUPPORT
        "CEGUI::GLES2Renderer - OpenGL ES 3.0 Renderer"
#else
        "CEGUI::GLES2Renderer - OpenGL ES 2.0 Renderer"
#endif
        "renderer module.";
}
//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase* GLES2Renderer::createGeometryBuffer_impl(CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    return new GLES2GeometryBuffer(*this, renderMaterial);
}

//----------------------------------------------------------------------------//
TextureTarget* GLES2Renderer::createTextureTarget_impl(bool addStencilBuffer)
{
    return d_textureTargetFactory->create(*this);
}

//----------------------------------------------------------------------------//
void GLES2Renderer::beginRendering()
{
    // if enabled, restores a subset of the GL state back to default values.
    if (d_initExtraStates)
        setupExtraStates();

    d_openGLStateChanger->reset();

    // Setup initial states
    d_openGLStateChanger->enable(GL_BLEND);

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BlendMode::Normal, true);
}

//----------------------------------------------------------------------------//
void GLES2Renderer::endRendering()
{
}

//----------------------------------------------------------------------------//
void GLES2Renderer::setupExtraStates()
{
    glActiveTexture(GL_TEXTURE0);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    glUseProgram(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------------//
void GLES2Renderer::initialiseTextureTargetFactory()
{
    //Use OGL core implementation for FBOs
    d_rendererID += "  TextureTarget support enabled via FBO OGL 3.2 core implementation.";
    d_textureTargetFactory = new OGLTemplateTargetFactory<GLES2FBOTextureTarget>;
}

//----------------------------------------------------------------------------//
void GLES2Renderer::setupRenderingBlendMode(const BlendMode mode,
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
Sizef GLES2Renderer::getAdjustedTextureSize(const Sizef& sz) 
{
    Sizef out(sz);

    const GLubyte* pcExt = glGetString(GL_EXTENSIONS);
    String extensions = String((const char*)pcExt);
    
    // if we can't support non power of two sizes, get appropriate POT values.
    if (!extensions.find("GL_OES_texture_npot"))
    {
        out.d_width = getNextPOTSize(out.d_width);
        out.d_height = getNextPOTSize(out.d_height);
    }

    return out;

}

//----------------------------------------------------------------------------//
OpenGLBaseStateChangeWrapper* GLES2Renderer::getOpenGLStateChanger()
{
    return d_openGLStateChanger;
}

//----------------------------------------------------------------------------//
void GLES2Renderer::initialiseOpenGLShaders()
{
    checkGLErrors();
#ifdef CEGUI_GLES3_SUPPORT
    d_shaderManager = new OpenGLBaseShaderManager(d_openGLStateChanger, SHADER_GLSLES3);
#else
    d_shaderManager = new OpenGLBaseShaderManager(d_openGLStateChanger, SHADER_GLSLES1);
#endif
    d_shaderManager->initialiseShaders();

    initialiseStandardTexturedShaderWrapper();
    initialiseStandardColouredShaderWrapper();
}

//----------------------------------------------------------------------------//
void GLES2Renderer::initialiseGLExtensions()
{
    const GLubyte* pcExt = glGetString(GL_EXTENSIONS);
    String extensions = String((const char*)pcExt);
    
    if (extensions.find("GL_EXT_texture_compression_s3tc"))
    {
        d_s3tcSupported = true;
    }
}

//----------------------------------------------------------------------------//
RefCounted<RenderMaterial> GLES2Renderer::createRenderMaterial(const DefaultShaderType shaderType) const
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
void GLES2Renderer::initialiseStandardTexturedShaderWrapper()
{
    OpenGLBaseShader* shader_standard_textured =  d_shaderManager->getShader(SHADER_ID_STANDARD_TEXTURED);
    d_shaderWrapperTextured = new OpenGLBaseShaderWrapper(*shader_standard_textured, d_openGLStateChanger);

    d_shaderWrapperTextured->addTextureUniformVariable("texture0", 0);

    d_shaderWrapperTextured->addUniformVariable("modelViewProjMatrix");
    d_shaderWrapperTextured->addUniformVariable("alphaPercentage");

    d_shaderWrapperTextured->addAttributeVariable("inPosition");
    d_shaderWrapperTextured->addAttributeVariable("inTexCoord");
    d_shaderWrapperTextured->addAttributeVariable("inColour");
}

//----------------------------------------------------------------------------//
void GLES2Renderer::initialiseStandardColouredShaderWrapper()
{
    OpenGLBaseShader* shader_standard_solid =  d_shaderManager->getShader(SHADER_ID_STANDARD_SOLID);
    d_shaderWrapperSolid = new OpenGLBaseShaderWrapper(*shader_standard_solid, d_openGLStateChanger);

    d_shaderWrapperSolid->addUniformVariable("modelViewProjMatrix");
    d_shaderWrapperSolid->addUniformVariable("alphaPercentage");

    d_shaderWrapperSolid->addAttributeVariable("inPosition");
    d_shaderWrapperSolid->addAttributeVariable("inColour");
}

//----------------------------------------------------------------------------//
OpenGLTexture* GLES2Renderer::createTexture_impl(const String& name)
{
    return new GLES2Texture(*this, name);
}  

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
