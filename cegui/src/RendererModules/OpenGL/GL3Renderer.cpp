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
#include "CEGUI/RendererModules/OpenGL/Texture.h"
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
#include "CEGUI/RendererModules/OpenGL/StateChangeWrapper.h"

#include <algorithm>

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

//----------------------------------------------------------------------------//
// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGLTextureTargetFactory
{
    TextureTarget* create(OpenGLRendererBase& r) const
        { return CEGUI_NEW_AO T(static_cast<OpenGL3Renderer&>(r)); }
};

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::bootstrapSystem(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGL3Renderer& renderer(create());
    DefaultResourceProvider* rp = CEGUI_NEW_AO CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::bootstrapSystem(const Sizef& display_size,
                                                  const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGL3Renderer& renderer(create(display_size));
    DefaultResourceProvider* rp = CEGUI_NEW_AO CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    OpenGL3Renderer* renderer = static_cast<OpenGL3Renderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    CEGUI_DELETE_AO rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::create(const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *CEGUI_NEW_AO OpenGL3Renderer();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer& OpenGL3Renderer::create(const Sizef& display_size,
                                         const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *CEGUI_NEW_AO OpenGL3Renderer(display_size);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::destroy(OpenGL3Renderer& renderer)
{
    CEGUI_DELETE_AO &renderer;
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::OpenGL3Renderer() :
    OpenGLRendererBase(true),
    d_shaderStandard(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    init();
    CEGUI_UNUSED(d_s3tcSupported);
    // d_s3tcSupported is unused, but must be preserved to avoid breaking ABI
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::OpenGL3Renderer(const Sizef& display_size) :
    OpenGLRendererBase(display_size, true),
    d_shaderStandard(0),
    d_openGLStateChanger(0),
    d_shaderManager(0)
{
    init();
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::init()
{
    if (      OpenGLInfo::getSingleton().isUsingOpenglEs()
          &&  OpenGLInfo::getSingleton().verMajor() < 2)
        CEGUI_THROW(RendererException("Only version 2 and up of OpenGL ES is "
                                      "supported by this type of renderer."));
    initialiseRendererIDString();
    initialiseTextureTargetFactory();
    initialiseOpenGLShaders();
    d_openGLStateChanger = CEGUI_NEW_AO OpenGL3StateChangeWrapper();
}

//----------------------------------------------------------------------------//
OpenGL3Renderer::~OpenGL3Renderer()
{
    CEGUI_DELETE_AO d_textureTargetFactory;
    CEGUI_DELETE_AO d_openGLStateChanger;
    CEGUI_DELETE_AO d_shaderManager;
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
OpenGLGeometryBufferBase* OpenGL3Renderer::createGeometryBuffer_impl()
{
    return CEGUI_NEW_AO OpenGL3GeometryBuffer(*this);
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGL3Renderer::createTextureTarget_impl()
{
    return d_textureTargetFactory->create(*this);
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
    if (d_initExtraStates)
        setupExtraStates();

    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BM_NORMAL, true);

    d_shaderStandard->bind();
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::endRendering()
{
    glUseProgram(0);

    if (d_initExtraStates)
        setupExtraStates();

    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::setupExtraStates()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (OpenGLInfo::getSingleton().isPolygonModeSupported())
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    
    d_openGLStateChanger->blendFunc(GL_ONE, GL_ZERO);

    if (OpenGLInfo::getSingleton().isVaoSupported())
        d_openGLStateChanger->bindVertexArray(0);
    glUseProgram(0);
    d_openGLStateChanger->bindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    d_openGLStateChanger->bindBuffer(GL_ARRAY_BUFFER, 0);
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseTextureTargetFactory()
{
    //Use OGL core implementation for FBOs
    d_rendererID += "  TextureTarget support enabled via FBO OGL 3.2 core implementation.";
    d_textureTargetFactory = CEGUI_NEW_AO OGLTemplateTargetFactory<OpenGL3FBOTextureTarget>;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::setupRenderingBlendMode(const BlendMode mode,
                                             const bool force)
{
    // exit if mode is already set up (and update not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;

    if (d_activeBlendMode == BM_RTT_PREMULTIPLIED)
    {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);
    }
}

//----------------------------------------------------------------------------//
Sizef OpenGL3Renderer::getAdjustedTextureSize(const Sizef& sz) const
{
    return Sizef(sz);
}

//----------------------------------------------------------------------------//
OpenGL3Shader*& OpenGL3Renderer::getShaderStandard()
{
    return d_shaderStandard;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardPositionLoc()
{
    return d_shaderStandardPosLoc;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardTexCoordLoc()
{
    return d_shaderStandardTexCoordLoc;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardColourLoc()
{
    return d_shaderStandardColourLoc;
}

//----------------------------------------------------------------------------//
GLint OpenGL3Renderer::getShaderStandardMatrixUniformLoc()
{
    return d_shaderStandardMatrixLoc;
}

//----------------------------------------------------------------------------//
OpenGL3StateChangeWrapper* OpenGL3Renderer::getOpenGLStateChanger()
{
    return d_openGLStateChanger;
}

//----------------------------------------------------------------------------//
void OpenGL3Renderer::initialiseOpenGLShaders()
{
    checkGLErrors();
    d_shaderManager = CEGUI_NEW_AO OpenGL3ShaderManager();
    d_shaderManager->initialiseShaders();
    d_shaderStandard = d_shaderManager->getShader(SHADER_ID_STANDARDSHADER);
    GLuint texLoc = d_shaderStandard->getUniformLocation("texture0");
    d_shaderStandard->bind();
    glUniform1i(texLoc, 0);
    d_shaderStandard->unbind();

    d_shaderStandardPosLoc = d_shaderStandard->getAttribLocation("inPosition");
    d_shaderStandardTexCoordLoc = d_shaderStandard->getAttribLocation("inTexCoord");
    d_shaderStandardColourLoc = d_shaderStandard->getAttribLocation("inColour");

    d_shaderStandardMatrixLoc = d_shaderStandard->getUniformLocation("modelViewPerspMatrix");
}

//----------------------------------------------------------------------------//
bool OpenGL3Renderer::isS3TCSupported() const
{
    return OpenGLInfo::getSingleton().isS3tcSupported();
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section
