/***********************************************************************
    created:    Sun Jan 11 2009
    authors:    Paul D Turner <paul@cegui.org.uk>
                Lukas E Meindl
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
#include "CEGUI/RendererModules/OpenGL/GL.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/RendererModules/OpenGL/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGL/GLGeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGL/GLFBOTextureTarget.h"
#include "CEGUI/RendererModules/OpenGL/GLRenderer.h"
#include "CEGUI/RendererModules/OpenGL/GLTexture.h"
#include "CEGUI/RendererModules/OpenGL/GLShaderWrapper.h"

#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"
#include "CEGUI/Logger.h"

#include <glm/gtc/type_ptr.hpp>

#include <sstream>
#include <algorithm>

#if defined(__linux__)  || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
#   include "CEGUI/RendererModules/OpenGL/GLXPBTextureTarget.h"
#elif defined(_WIN32) || defined(__WIN32__)
#   include "CEGUI/RendererModules/OpenGL/WGLPBTextureTarget.h"
#elif defined(__APPLE__)
#   include "CEGUI/RendererModules/OpenGL/ApplePBTextureTarget.h"
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
// The following are some GL extension / version dependent related items.
// This is all done totally internally here; no need for external interface
// to show any of this.
//----------------------------------------------------------------------------//
// we only really need this with MSVC / Windows(?) and by now it should already
// be defined on that platform, so we just define it as empty macro so the
// compile does not break on other systems.
#ifndef APIENTRY
#   define APIENTRY
#endif
//! Pointer to a function to use as glActiveTexture
static PFNGLACTIVETEXTUREPROC CEGUI_activeTexture;
//! Pointer to a function to use as glClientActiveTexture
static PFNGLCLIENTACTIVETEXTUREPROC CEGUI_clientActiveTexture;
//! Dummy function for if real ones are not present (saves testing each render)
static void APIENTRY activeTextureDummy(GLenum) {}

//----------------------------------------------------------------------------//
// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGLTextureTargetFactory
{
    TextureTarget* create(OpenGLRendererBase& r, bool addStencilBuffer) const override
    { return new T(r, addStencilBuffer); }
};

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::bootstrapSystem(const TextureTargetType tt_type,
                                                const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

    OpenGLRenderer& renderer(create(tt_type));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::bootstrapSystem(const Sizef& display_size,
                                                const TextureTargetType tt_type,
                                                const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        throw InvalidRequestException(
            "CEGUI::System object is already initialised.");

    OpenGLRenderer& renderer(create(display_size, tt_type));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroySystem()
{
    System* sys = System::getSingletonPtr();
    if (sys == nullptr)
    {
        throw InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed.");
    }

    OpenGLRenderer* renderer = static_cast<OpenGLRenderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::create(const TextureTargetType tt_type,
                                       const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLRenderer(tt_type);
}

//----------------------------------------------------------------------------//
OpenGLRenderer& OpenGLRenderer::create(const Sizef& display_size,
                                       const TextureTargetType tt_type,
                                       const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLRenderer(display_size, tt_type);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::destroy(OpenGLRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(const TextureTargetType tt_type) :
    OpenGLRendererBase(false)
{
    initialiseRendererIDString();
    initialiseGLExtensions();
    initialiseTextureTargetFactory(tt_type);
    initialiseShaderWrappers();

    // we _really_ need separate rgb/alpha blend modes, if this support is not
    // available, add a note to the renderer ID string so that this fact is
    // logged.
    if (!GLEW_VERSION_1_4 && !GLEW_EXT_blend_func_separate)
        d_rendererID += "  No glBlendFuncSeparate(EXT) support.";
}

//----------------------------------------------------------------------------//
OpenGLRenderer::OpenGLRenderer(const Sizef& display_size,
                               const TextureTargetType tt_type) :
    OpenGLRendererBase(display_size, false)
{
    initialiseRendererIDString();
    initialiseGLExtensions();
    initialiseTextureTargetFactory(tt_type);

    // we _really_ need separate rgb/alpha blend modes, if this support is not
    // available, add a note to the renderer ID string so that this fact is
    // logged.
    if (!GLEW_VERSION_1_4 && !GLEW_EXT_blend_func_separate)
        d_rendererID += "  No glBlendFuncSeparate(EXT) support.";
}

//----------------------------------------------------------------------------//
OpenGLRenderer::~OpenGLRenderer()
{
    delete d_textureTargetFactory;
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::initialiseRendererIDString()
{
    d_rendererID = 
        "CEGUI::OpenGLRenderer - Official OpenGL based 2nd generation "
        "renderer module.";
}

//----------------------------------------------------------------------------//
OpenGLGeometryBufferBase* OpenGLRenderer::createGeometryBuffer_impl(CEGUI::RefCounted<RenderMaterial> renderMaterial)
{
    return new OpenGLGeometryBuffer(*this, renderMaterial);
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGLRenderer::createTextureTarget_impl(bool addStencilBuffer)
{
    return d_textureTargetFactory->create(*this, addStencilBuffer);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::beginRendering()
{
    //save current attributes
    glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    glPushAttrib(GL_ALL_ATTRIB_BITS);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    // do required set-up.  yes, it really is this minimal ;)
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    // force set blending ops to get to a known state.
    setupRenderingBlendMode(BlendMode::Normal, true);

    // enable arrays that we'll be using in the batches
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_SECONDARY_COLOR_ARRAY);
    glDisableClientState(GL_INDEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_FOG_COORDINATE_ARRAY);
    glDisableClientState(GL_EDGE_FLAG_ARRAY);

    // if enabled, restores a subset of the GL state back to default values.
    if (d_isStateResettingEnabled)
        restoreOpenGLStatesToDefaults();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::endRendering()
{
    if (d_isStateResettingEnabled)
        cleanupMatrixStack();

    // restore former matrices
    // FIXME: If the push ops failed, the following could mess things up!
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    //restore former attributes
    glPopAttrib();
    glPopClientAttrib();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::restoreOpenGLStatesToDefaults()
{
    CEGUI_activeTexture(GL_TEXTURE0);
    CEGUI_clientActiveTexture(GL_TEXTURE0);

    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();

    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_TEXTURE_GEN_R);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::cleanupMatrixStack()
{
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::initialiseTextureTargetFactory(
    const TextureTargetType tt_type)
{
    // prefer FBO

    if (((tt_type == TextureTargetType::Auto) || (tt_type == TextureTargetType::Fbo)) &&
        GLEW_EXT_framebuffer_object)
    {
        d_rendererID += "  TextureTarget support enabled via FBO extension.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLFBOTextureTarget>;
    }

#if defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__HAIKU__)
    // on linux (etc), we can try for GLX pbuffer support
    else if (((tt_type == TextureTargetType::Auto) || (tt_type == TextureTargetType::Pbuffer)) &&
             GLXEW_VERSION_1_3)
    {
        d_rendererID += "  TextureTarget support enabled via GLX pbuffers.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLGLXPBTextureTarget>;
    }
#elif defined(_WIN32) || defined(__WIN32__)
    // on Windows, we can try for WGL based pbuffer support
    else if (((tt_type == TextureTargetType::Auto) || (tt_type == TextureTargetType::Pbuffer)) &&
             WGLEW_ARB_pbuffer)
    {
        d_rendererID += "  TextureTarget support enabled via WGL_ARB_pbuffer.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLWGLPBTextureTarget>;
    }
#elif defined(__APPLE__)
    // on Apple Mac, we can try for Apple's pbuffer support
    else if (((tt_type == TextureTargetType::Auto) || (tt_type == TextureTargetType::Pbuffer)) &&
             GLEW_APPLE_pixel_buffer)
    {
        d_rendererID += "  TextureTarget support enabled via "
                        "GL_APPLE_pixel_buffer.";
        d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLApplePBTextureTarget>;
    }
#endif
    // Nothing suitable available, try to carry on without TextureTargets
    else
    {
        d_rendererID += "  TextureTarget support is not available :(";
        d_textureTargetFactory = new OGLTextureTargetFactory;
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::setupRenderingBlendMode(const BlendMode mode,
                                             const bool force)
{
    // exit if mode is already set up (and update not forced)
    if ((d_activeBlendMode == mode) && !force)
        return;

    d_activeBlendMode = mode;

    if (d_activeBlendMode == BlendMode::RttPremultiplied)
    {
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        if (GLEW_VERSION_1_4)
            glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                GL_ONE_MINUS_DST_ALPHA, GL_ONE);
        else if (GLEW_EXT_blend_func_separate)
            glBlendFuncSeparateEXT(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
                                   GL_ONE_MINUS_DST_ALPHA, GL_ONE);
        else
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
}

//----------------------------------------------------------------------------//
void OpenGLRenderer::initialiseGLExtensions()
{
    // GL 1.3 has multi-texture support natively
    if (GLEW_VERSION_1_3)
    {
        CEGUI_activeTexture = glActiveTexture;
        CEGUI_clientActiveTexture = glClientActiveTexture;
    }
    // Maybe there is the ARB_multitexture extension version?
    else if (GLEW_ARB_multitexture)
    {
        CEGUI_activeTexture = glActiveTextureARB;
        CEGUI_clientActiveTexture = glClientActiveTextureARB;
    }
    // assign dummy functions if no multitexture possibilities
    else
    {
        CEGUI_activeTexture = activeTextureDummy;
        CEGUI_clientActiveTexture = activeTextureDummy;
    }
}

//----------------------------------------------------------------------------//
RefCounted<RenderMaterial> OpenGLRenderer::createRenderMaterial(const DefaultShaderType shaderType) const
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
void OpenGLRenderer::initialiseShaderWrappers()
{
    d_shaderWrapperTextured = new OpenGLShaderWrapper();

    d_shaderWrapperSolid = new OpenGLShaderWrapper();
}

//----------------------------------------------------------------------------//
Sizef OpenGLRenderer::getAdjustedTextureSize(const Sizef& sz)
{
    Sizef out(sz);

    // if we can't support non power of two sizes, get appropriate POT values.
    if (!OpenGLInfo::getSingleton().isNpotTextureSupported())
    {
        out.d_width = getNextPOTSize(out.d_width);
        out.d_height = getNextPOTSize(out.d_height);
    }

    return out;
}

//----------------------------------------------------------------------------//
OpenGLTexture* OpenGLRenderer::createTexture_impl(const String& name)
{
    return new OpenGL1Texture(*this, name);
}  

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

