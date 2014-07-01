/***********************************************************************
    created:    Sun Jan 11 2009
    author:     Paul D Turner
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
#include "CEGUI/RendererModules/OpenGLES/Renderer.h"
#include "CEGUI/RendererModules/OpenGLES/Texture.h"
#include "CEGUI/Exceptions.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/ImageCodec.h"
#include "CEGUI/DynamicModule.h"
#include "CEGUI/RendererModules/OpenGLES/ViewportTarget.h"
#include "CEGUI/RendererModules/OpenGLES/GeometryBuffer.h"
#include "CEGUI/RendererModules/OpenGLES/FBOTextureTarget.h"
#include "CEGUI/Logger.h"
#include "CEGUI/System.h"
#include "CEGUI/DefaultResourceProvider.h"

#include <sstream>
#include <algorithm>

#include "CEGUI/RendererModules/OpenGLES/FBOTextureTarget.h"

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
void APIENTRY activeTextureDummy(GLenum) {}

//----------------------------------------------------------------------------//
//
// Here we have an internal class that allows us to implement a factory template
// for creating / destroying any type of TextureTarget.  The code that detects
// the computer's abilities will generate an appropriate factory for a
// TextureTarget based on what the host system can provide - or use the default
// 'null' factory if no suitable TextureTargets are available.
//
// base factory class - mainly used as a polymorphic interface
class OGLTextureTargetFactory
{
public:
    OGLTextureTargetFactory() {}
    virtual ~OGLTextureTargetFactory() {}
    virtual TextureTarget* create(OpenGLESRenderer&) const
        { return 0; }
    virtual void destory(TextureTarget* target) const
        { delete target; }
};

// template specialised class that does the real work for us
template<typename T>
class OGLTemplateTargetFactory : public OGLTextureTargetFactory
{
    virtual TextureTarget* create(OpenGLESRenderer& r) const
        { return new T(r); }
};

//----------------------------------------------------------------------------//
String OpenGLESRenderer::d_rendererID(
"CEGUI::OpenGLESRenderer - Official OpenGLES based 2nd generation renderer module.");

//----------------------------------------------------------------------------//
OpenGLESRenderer& OpenGLESRenderer::bootstrapSystem(
                                        const TextureTargetType tt_type,
                                        const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGLESRenderer& renderer(create(tt_type));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
OpenGLESRenderer& OpenGLESRenderer::bootstrapSystem(
                                        const Sizef& display_size,
                                        const TextureTargetType tt_type,
                                        const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    if (System::getSingletonPtr())
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is already initialised."));

    OpenGLESRenderer& renderer(create(display_size, tt_type));
    DefaultResourceProvider* rp = new CEGUI::DefaultResourceProvider();
    System::create(renderer, rp);

    return renderer;
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroySystem()
{
    System* sys;
    if (!(sys = System::getSingletonPtr()))
        CEGUI_THROW(InvalidRequestException(
            "CEGUI::System object is not created or was already destroyed."));

    OpenGLESRenderer* renderer = 
        static_cast<OpenGLESRenderer*>(sys->getRenderer());
    DefaultResourceProvider* rp =
        static_cast<DefaultResourceProvider*>(sys->getResourceProvider());

    System::destroy();
    delete rp;
    destroy(*renderer);
}

//----------------------------------------------------------------------------//
OpenGLESRenderer& OpenGLESRenderer::create(const TextureTargetType tt_type,
                                           const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLESRenderer(tt_type);
}

//----------------------------------------------------------------------------//
OpenGLESRenderer& OpenGLESRenderer::create(const Sizef& display_size,
                                           const TextureTargetType tt_type,
                                           const int abi)
{
    System::performVersionTest(CEGUI_VERSION_ABI, abi, CEGUI_FUNCTION_NAME);

    return *new OpenGLESRenderer(display_size, tt_type);
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroy(OpenGLESRenderer& renderer)
{
    delete &renderer;
}

//----------------------------------------------------------------------------//
bool OpenGLESRenderer::isGLExtensionSupported( const char* extension )
{
	const GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;

	/* Extension names should not have spaces. */
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;

	extensions = glGetString(GL_EXTENSIONS);

	start = extensions;
	for (;;)
	{
		where = (GLubyte *) strstr((const char *) start, extension);
		if (!where)
			break;
		terminator = where + strlen(extension);
		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;
		start = terminator;
	}
	return false;
}

//----------------------------------------------------------------------------//
OpenGLESRenderer::OpenGLESRenderer(const TextureTargetType tt_type) :
    d_displayDPI(96, 96),
    d_initExtraStates(false)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    // initialise display size
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp);
    d_displaySize = Sizef(static_cast<float>(vp[2]), static_cast<float>(vp[3]));

    initialiseTextureTargetFactory(tt_type);

    d_defaultTarget = new OpenGLESViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLESRenderer::OpenGLESRenderer(const Sizef& display_size,
                                   const TextureTargetType tt_type) :
    d_displaySize(display_size),
    d_displayDPI(96, 96),
    d_initExtraStates(false)
{
    // get rough max texture size
    GLint max_tex_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_tex_size);
    d_maxTextureSize = max_tex_size;

    initialiseTextureTargetFactory(tt_type);

    d_defaultTarget = new OpenGLESViewportTarget(*this);
}

//----------------------------------------------------------------------------//
OpenGLESRenderer::~OpenGLESRenderer()
{
    destroyAllGeometryBuffers();
    destroyAllTextureTargets();
    destroyAllTextures();

    delete d_defaultTarget;
    delete d_textureTargetFactory;
}

//----------------------------------------------------------------------------//
RenderTarget& OpenGLESRenderer::getDefaultRenderTarget()
{
    return *d_defaultTarget;
}

//----------------------------------------------------------------------------//
GeometryBuffer& OpenGLESRenderer::createGeometryBuffer()
{
    OpenGLESGeometryBuffer* b= new OpenGLESGeometryBuffer;
    d_geometryBuffers.push_back(b);
    return *b;
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyGeometryBuffer(const GeometryBuffer& buffer)
{
    GeometryBufferList::iterator i = std::find(d_geometryBuffers.begin(),
                                               d_geometryBuffers.end(),
                                               &buffer);

    if (d_geometryBuffers.end() != i)
    {
        d_geometryBuffers.erase(i);
        delete &buffer;
    }
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyAllGeometryBuffers()
{
    while (!d_geometryBuffers.empty())
        destroyGeometryBuffer(**d_geometryBuffers.begin());
}

//----------------------------------------------------------------------------//
TextureTarget* OpenGLESRenderer::createTextureTarget()
{
    TextureTarget* t = d_textureTargetFactory->create(*this);
    d_textureTargets.push_back(t);
    return t;
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyTextureTarget(TextureTarget* target)
{
    TextureTargetList::iterator i = std::find(d_textureTargets.begin(),
                                              d_textureTargets.end(),
                                              target);

    if (d_textureTargets.end() != i)
    {
        d_textureTargets.erase(i);
        d_textureTargetFactory->destory(target);
    }
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyAllTextureTargets()
{
    while (!d_textureTargets.empty())
        destroyTextureTarget(*d_textureTargets.begin());
}

//----------------------------------------------------------------------------//
Texture& OpenGLESRenderer::createTexture(const String& name)
{
    OpenGLESTexture* tex = new OpenGLESTexture(*this, name);
    d_textures[name] = tex;
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLESRenderer::createTexture(const String& name,
                                         const String& filename,
                                         const String& resourceGroup)
{
    OpenGLESTexture* tex = new OpenGLESTexture(*this, name, filename,
                                               resourceGroup);
    d_textures[name] = tex;
    return *tex;
}

//----------------------------------------------------------------------------//
Texture& OpenGLESRenderer::createTexture(const String& name, const Sizef& size)
{
    OpenGLESTexture* tex = new OpenGLESTexture(*this, name, size);
    d_textures[name] = tex;
    return *tex;
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyTexture(const String& name)
{
    TextureMap::iterator i = d_textures.find(name);

    if (d_textures.end() != i)
    {
        logTextureDestruction(name);
        delete i->second;
        d_textures.erase(i);
    }
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::logTextureDestruction(const String& name)
{
    Logger* logger = Logger::getSingletonPtr();
    if (logger)
        logger->logEvent("[OpenGLESRenderer] Destroyed texture: " + name);
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyTexture(Texture& texture)
{
    destroyTexture(texture.getName());
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::destroyAllTextures()
{
    while (!d_textures.empty())
        destroyTexture(d_textures.begin()->first);
}

//----------------------------------------------------------------------------//
Texture& OpenGLESRenderer::getTexture(const String& name) const
{
    TextureMap::const_iterator i = d_textures.find(name);
    
    if (i == d_textures.end())
        CEGUI_THROW(UnknownObjectException(
            "No texture named '" + name + "' is available."));

    return *i->second;
}

//----------------------------------------------------------------------------//
bool OpenGLESRenderer::isTextureDefined(const String& name) const
{
    return d_textures.find(name) != d_textures.end();
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::beginRendering()
{
    //save current attributes

	// Unsupported by OpenGL ES
	//glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);
    //glPushAttrib(GL_ALL_ATTRIB_BITS);

    // save current matrices
    //glMatrixMode(GL_PROJECTION);
    //glPushMatrix();  -- causes gl stack overflow error

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

	// Save at least something
	glPreRenderStates.glScissorTest = glIsEnabled(GL_SCISSOR_TEST);
	glPreRenderStates.texturing = glIsEnabled(GL_TEXTURE_2D);
	glPreRenderStates.blend = glIsEnabled(GL_BLEND);
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &glPreRenderStates.arrayBuffer);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &glPreRenderStates.texture);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &glPreRenderStates.texEnvParam);

    // do required set-up.  yes, it really is this minimal ;)
	glEnable(GL_SCISSOR_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    // enable arrays that we'll be using in the batches
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);

    // if enabled, restores a subset of the GL state back to default values.
    if (d_initExtraStates)
        setupExtraStates();
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::endRendering()
{
    if (d_initExtraStates)
        cleanupExtraStates();

    // restore former matrices
    // FIXME: If the push ops failed, the following could mess things up!
    //glMatrixMode(GL_PROJECTION);
    //glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

	if (!glPreRenderStates.glScissorTest)
		glDisable(GL_SCISSOR_TEST);
	if (!glPreRenderStates.texturing)
		glDisable(GL_TEXTURE_2D);
	if (!glPreRenderStates.blend)
		glDisable(GL_BLEND);

	if (glPreRenderStates.arrayBuffer)
		glBindBuffer(GL_ARRAY_BUFFER, glPreRenderStates.arrayBuffer);
	glBindTexture(GL_TEXTURE_2D, glPreRenderStates.texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, glPreRenderStates.texEnvParam);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    //restore former attributes
	//still unsupported
    //glPopAttrib();
    //glPopClientAttrib();
}

//----------------------------------------------------------------------------//
const Sizef& OpenGLESRenderer::getDisplaySize() const
{
    return d_displaySize;
}

//----------------------------------------------------------------------------//
const Vector2f& OpenGLESRenderer::getDisplayDPI() const
{
    return d_displayDPI;
}

//----------------------------------------------------------------------------//
uint OpenGLESRenderer::getMaxTextureSize() const
{
    return d_maxTextureSize;
}

//----------------------------------------------------------------------------//
const String& OpenGLESRenderer::getIdentifierString() const
{
    return d_rendererID;
}

//----------------------------------------------------------------------------//
Texture& OpenGLESRenderer::createTexture(const String& name, GLuint tex,
                                         const Sizef& sz)
{
    OpenGLESTexture* t = new OpenGLESTexture(*this, name, tex, sz);
    d_textures[name] = t;
    return *t;
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::setDisplaySize(const Sizef& sz)
{
    if (sz != d_displaySize)
    {
        d_displaySize = sz;

        // update the default target's area
        Rectf area(d_defaultTarget->getArea());
        area.setSize(sz);
        d_defaultTarget->setArea(area);
    }
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::enableExtraStateSettings(bool setting)
{
    d_initExtraStates = setting;
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::setupExtraStates()
{
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glLoadIdentity();

    glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);

    //glPolygonMode(GL_FRONT, GL_FILL);
    //glPolygonMode(GL_BACK, GL_FILL);

    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    //glDisable(GL_TEXTURE_GEN_S);
    //glDisable(GL_TEXTURE_GEN_T);
    //glDisable(GL_TEXTURE_GEN_R);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::cleanupExtraStates()
{
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::grabTextures()
{
    for(TextureMap::iterator i = d_textures.begin();
        i != d_textures.end();
        ++i)
        i->second->grabTexture();
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::restoreTextures()
{
    for(TextureMap::iterator i = d_textures.begin();
        i != d_textures.end();
        ++i)
        i->second->restoreTexture();
}

//----------------------------------------------------------------------------//
void OpenGLESRenderer::initialiseTextureTargetFactory(
    const TextureTargetType tt_type)
{
    if (isGLExtensionSupported("GL_OES_framebuffer_object"))
    {
        d_rendererID += "  TextureTarget support enabled via"
						"GL_OES_framebuffer_object extension.";
		OpenGLESFBOTextureTarget::initializedFBOExtension();
		d_textureTargetFactory =
            new OGLTemplateTargetFactory<OpenGLESFBOTextureTarget>;
    }
    else
    {
        d_rendererID += "  TextureTarget support is not available :(";
        d_textureTargetFactory = new OGLTextureTargetFactory;
    }
}

//----------------------------------------------------------------------------//
Sizef OpenGLESRenderer::getAdjustedTextureSize(const Sizef& sz) const
{
    Sizef out(sz);
    out.d_width = getNextPOTSize(out.d_width);
    out.d_height = getNextPOTSize(out.d_height);
    return out;
}

//----------------------------------------------------------------------------//
float OpenGLESRenderer::getNextPOTSize(const float f)
{
    uint size = static_cast<uint>(f);

    // if not power of 2
    if ((size & (size - 1)) || !size)
    {
        int log = 0;

        // get integer log of 'size' to base 2
        while (size >>= 1)
            ++log;

        // use log to calculate value to use as size.
        size = (2 << log);
    }

    return static_cast<float>(size);
}

//----------------------------------------------------------------------------//

} // End of  CEGUI namespace section

