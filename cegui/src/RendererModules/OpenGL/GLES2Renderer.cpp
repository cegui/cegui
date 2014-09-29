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
#include "CEGUI/RendererModules/OpenGL/Texture.h"
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
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"
#include "CEGUI/RenderMaterial.h"
#include "CEGUI/RendererModules/OpenGL/GLES2ShaderWrapper.h"

#include <sstream>
#include <algorithm>
#include <cstring>

#ifdef __ANDROID__
#include <android/log.h>
#endif

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
        { return new T(static_cast<OpenGLES2Renderer&>(r)); }
};

//----------------------------------------------------------------------------//
OpenGLES2Renderer& OpenGLES2Renderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGLES2Renderer& renderer(create());
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGLES2Renderer& OpenGLES2Renderer::bootstrapSystem(const Sizef& display_size,
                                                  const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGLES2Renderer& renderer(create(display_size));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    OpenGLES2Renderer* renderer = static_cast<OpenGLES2Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGLES2Renderer& OpenGLES2Renderer::create(const int abi)
{
    __android_log_write (ANDROID_LOG_ERROR, "EGL", "create version test");

    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    __android_log_write (ANDROID_LOG_ERROR, "EGL", "create obj");

    return *new OpenGLES2Renderer();
}

//----------------------------------------------------------------------------//
OpenGLES2Renderer& OpenGLES2Renderer::create(const Sizef& display_size,
                                         const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLES2Renderer(display_size);
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::destroy(OpenGLES2Renderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OpenGLES2Renderer::OpenGLES2Renderer() :
    d_shaderWrapperTextured(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    __android_log_write (ANDROID_LOG_ERROR, "EGL", "id string ");
    initialiseRendererIDString();

    __android_log_write (ANDROID_LOG_ERROR, "EGL", "gl extensions");
    initialiseGLExtensions();
    __android_log_write (ANDROID_LOG_ERROR, "EGL", "gl state change wrapper");
    d_openGLStateChanger = new OpenGL3StateChangeWrapper();
    __android_log_write (ANDROID_LOG_ERROR, "EGL", "gl tex target factory");
    initialiseTextureTargetFactory();
    __android_log_write (ANDROID_LOG_ERROR, "EGL", "gl shaders");
    initialiseOpenGLShaders();
    __android_log_write (ANDROID_LOG_ERROR, "EGL", "shaders complete");
}

//----------------------------------------------------------------------------//
OpenGLES2Renderer::OpenGLES2Renderer(const Sizef& display_size) :
    OpenGLRendererBase(display_size),
    d_shaderWrapperTextured(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    initialiseRendererIDString();
    initialiseGLExtensions();
    d_openGLStateChanger = new OpenGL3StateChangeWrapper();
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();
}

//----------------------------------------------------------------------------//
OpenGLES2Renderer::~OpenGLES2Renderer()
{
    delete d_textureTargetFactory;
    delete d_openGLStateChanger;
    delete d_shaderManager;

    delete d_shaderWrapperTextured;
    delete d_shaderWrapperSolid;
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::initialiseRendererIDString()
{
    d_rendererID =
        "CEGUI::OpenGLES2Renderer - Official OpenGL 3.2 core based "
        "renderer module.";
}
//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase* OpenGLES2Renderer::createGeometryBuffer_impl(CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    return new OpenGLES2GeometryBuffer(*this, renderMaterial);
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGLES2Renderer::createTextureTarget_impl()
{
    return d_textureTargetFactory->create(*this);
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::beginRendering()
{
    // if enabled, restores a subset of the GL state back to default values.
    if (d_initExtraStates)
        setupExtraStates();

    d_openGLStateChanger->reset();

    // Setup initial states
    d_openGLStateChanger->enable(GL_BLEND);

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BM_NORMAL, true);
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::endRendering()
{
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::setupExtraStates()
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
void OpenGLES2Renderer::initialiseTextureTargetFactory()
{
    //Use OGL core implementation for FBOs
    d_rendererID += "  TextureTarget support enabled via FBO OGL 3.2 core implementation.";
    d_textureTargetFactory = new OGLTemplateTargetFactory<OpenGLES2FBOTextureTarget>;
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::setupRenderingBlendMode(const BlendMode mode,
                                              const bool force)
{
    // exit if mode is already set up (and update not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;

    if (d_activeBlendMode == BM_RTT_PREMULTIPLIED)
    {
        d_openGLStateChanger->blendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        d_openGLStateChanger->blendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    }
}

//----------------------------------------------------------------------------//
Sizef OpenGLES2Renderer::getAdjustedTextureSize(const Sizef& sz) 
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
OpenGL3StateChangeWrapper* OpenGLES2Renderer::getOpenGLStateChanger()
{
    return d_openGLStateChanger;
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::initialiseOpenGLShaders()
{
    checkGLErrors();
    d_shaderManager = new OpenGL3ShaderManager(d_openGLStateChanger, SHADER_GLSLES3);
    d_shaderManager->initialiseShaders();

    initialiseStandardTexturedShaderWrapper();
    initialiseStandardColouredShaderWrapper();
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::initialiseGLExtensions()
{
    //glewExperimental = GL_TRUE;

    //GLenum err = glewInit();
    //if(err != GLEW_OK)
    //{
    //    std::ostringstream err_string;
        //Problem: glewInit failed, something is seriously wrong.
    //    err_string << "failed to initialise the GLEW library. "
    //        << glewGetErrorString(err);

    //    CEGUI_THROW(RendererException(err_string.str().c_str()));
    //}
    //Clear the useless error glew produces as of version 1.7.0, when using OGLES2.2 Core Profile
    //glGetError();

    const GLubyte* pcExt = glGetString(GL_EXTENSIONS);
    String extensions = String((const char*)pcExt);
    
    if (extensions.find("GL_EXT_texture_compression_s3tc"))
    {
        d_s3tcSupported = true;
    }
}

//----------------------------------------------------------------------------//
bool OpenGLES2Renderer::isS3TCSupported() const
{
    return d_s3tcSupported;
}

//----------------------------------------------------------------------------//
RefCounted<RenderMaterial> OpenGLES2Renderer::createRenderMaterial(const DefaultShaderType shaderType) const
{
    if(shaderType == DS_TEXTURED)
    {
        RefCounted<RenderMaterial> render_material(new RenderMaterial(d_shaderWrapperTextured));

        return render_material;
    }
    else if(shaderType == DS_SOLID)
    {
        RefCounted<RenderMaterial> render_material(new RenderMaterial(d_shaderWrapperSolid));

        return render_material;
    }
    else
    {
        CEGUI_THROW(RendererException(
            "A default shader of this type does not exist."));

        return RefCounted<RenderMaterial>();
    }
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::initialiseStandardTexturedShaderWrapper()
{
    OpenGL3Shader* shader_standard_textured =  d_shaderManager->getShader(SHADER_ID_STANDARD_TEXTURED);
    d_shaderWrapperTextured = new OpenGL3ShaderWrapper(*shader_standard_textured, d_openGLStateChanger);

    d_shaderWrapperTextured->addTextureUniformVariable("texture0", 0);

    d_shaderWrapperTextured->addUniformVariable("modelViewProjMatrix");
    d_shaderWrapperTextured->addUniformVariable("alphaPercentage");

    d_shaderWrapperTextured->addAttributeVariable("inPosition");
    d_shaderWrapperTextured->addAttributeVariable("inTexCoord");
    d_shaderWrapperTextured->addAttributeVariable("inColour");
}

//----------------------------------------------------------------------------//
void OpenGLES2Renderer::initialiseStandardColouredShaderWrapper()
{
    OpenGL3Shader* shader_standard_solid =  d_shaderManager->getShader(SHADER_ID_STANDARD_SOLID);
    d_shaderWrapperSolid = new OpenGL3ShaderWrapper(*shader_standard_solid, d_openGLStateChanger);

    d_shaderWrapperSolid->addUniformVariable("modelViewProjMatrix");
    d_shaderWrapperSolid->addUniformVariable("alphaPercentage");

    d_shaderWrapperSolid->addAttributeVariable("inPosition");
    d_shaderWrapperSolid->addAttributeVariable("inColour");
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
