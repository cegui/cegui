/***********************************************************************
    filename:   OgreCEGUIRenderer.h
    created:    Mon Mar 3 2008
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2008 Paul D Turner & The CEGUI Development Team
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
#ifndef _OgreCEGUIRenderer_h_
#define _OgreCEGUIRenderer_h_

#include <CEGUI/CEGUIRenderer.h>
#include <CEGUI/CEGUISize.h>
#include <OgrePlatform.h>
#include <OgreRenderQueueListener.h>
#include <list>

#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && !defined(OGRE_STATIC_LIB)
#   ifdef OGRE_GUIRENDERER_EXPORTS
#       define OGRE_GUIRENDERER_API __declspec(dllexport)
#   else
#       if defined(__MINGW32__)
#           define OGRE_GUIRENDERER_API
#       else
#           define OGRE_GUIRENDERER_API __declspec(dllimport)
#       endif
#   endif
#elif defined ( OGRE_GCC_VISIBILITY )
#    define OGRE_GUIRENDERER_API  __attribute__ ((visibility("default")))
#else
#    define OGRE_GUIRENDERER_API
#endif

// forward declare some ogre things to save including whole files
namespace Ogre
{
    class TexturePtr;
    class SceneManager;
    class RenderTarget;
}

// Start of CEGUI namespace section
namespace CEGUI
{
// forward declare classes from this module to save including whole files.
class OgreCEGUITexture;
class CEGUIRQListener;

/**
 * Official CEGUI::Renderer class interfacing Ogre with Crazy Eddie's Gui
 * System (CEGUI).
 *
 * This 2008 version is a pretty much a complete re-imagining of the original
 * renderer and conforms to the new CEGUI RenderTarget approach to rendering.
 *
 * Questions and support for this module are provided by the CEGUI project at
 * http://www.cegui.org.uk/ (look, it's not the Ogre site!)
*/
class OGRE_GUIRENDERER_API OgreCEGUIRenderer : public Renderer
{
public:
    /**
     * Constructor for OgreCEGUIRenderer.
     * @param target the Ogre::RenderTarget where output from the primary
     * CEGUI RenderTarget should be sent.  If this is 0 the automatically
     * created Ogre window will be used as a default.  Currently this always
     * uses the first viewport (index 0) on the target.
     * @param scene_manager the Ogre::SceneManager that will be used to trigger
     * redraws of CEGUI.  If this is 0 the system will attempt to locate a
     * scene manager to use (basically it will use the first one it can find).
     * @param queue_id id of the render queue that will trigger redraws of
     * CEGUI.  Defaults to RENDER_QUEUE_OVERLAY.
     * @param post_queue specifies whether redraws of CEGUI will be triggered
     * before or after all other rendering has occurred on the render queue
     * identified by \a queue_id.  Defaults to false.
     * @note Under the new RenderTarget system employed by CEGUI, the scene
     * manager and associated render queue are now only used to trigger the
     * CEGUI redraw operation.  The final destination for rendering output from
     * CEGUI will depend upon how the Gui has been setup and what
     * CEGUI::RenderTarget objects are in use.
     * @par One change that may affect people is that Gui output will no longer
     * automagically appear on all ogre render targets - this was kind of a
     * funky thing to happen anyway, so this update addesses that.
     */
    OgreCEGUIRenderer(Ogre::RenderTarget* target = 0,
                      Ogre::SceneManager* scene_manager = 0,
                      Ogre::uint8 queue_id = Ogre::RENDER_QUEUE_OVERLAY,
                      bool post_queue = false);

    /**
     * Destructor for OgreCEGUIRenderer objects.
     */
    virtual ~OgreCEGUIRenderer();

    /**
     * Create a texture from an existing Ogre::TexturePtr object.
     * @param texture Ogre::TexturePtr object to be used as the basis for the
     * new CEGUI::Texture being created.
     * @return Pointer to the newly created CEGUI::TexturePtr object.
     * @note If you want to use an Ogre::RenderTexture (for putting rendered
     * output onto Gui elements or other advanced techniques), you can get the
     * Ogre::TexturePtr to be used by calling Ogre::TextureManager::getByName()
     * passing the name returned from Ogre::RenderTexture::getName() (and
     * casting the result as necessary).
     */
    Texture* createTexture(Ogre::TexturePtr& texture);

    /**
     * Sets the Ogre scene manager that will trigger CEGUI redrawing.  If
     * there is already an associated scene manager, the Renderer will first
     * detach from that scene manager before attaching to the new one.
     * @param scene_manager pointer to an Ogre::SceneManager object that is to
     * be used to trigger CEGUI redrawing.
     */
    void setTriggerSceneManager(Ogre::SceneManager* scene_manager);

    /**
     * Set the render queue and pre/post setting that controls exactly when
     * the CEGUI redraw is triggered.
     * @param queue_id Ogre::uint8 value specifying the render queue id that
     * will trigger the CEGUI redraw.
     * @param post_queue bool value specifying whether CEGUI redraw should be
     * triggered before or after other rendering in the same queue.  Set to true
     * to have the CEGUI redraw triggered after rendering on the queue is
     * performed, or to false to have the CEGUI redraw triggered first.
     */
    void setTriggerRenderQueue(Ogre::uint8 queue_id, bool post_queue);

    /**
     * Notify the renderer of the size of the display in pixels.
     * @note This does not affect much, other than to fire off the
     * EventDisplaySizeChanged event.
     * @param sz Size object describing the size of the display.
     * @return nothing.
     */
    void setDisplaySize(const Size& sz);

    // abstract portion of CEGUI::Renderer interface we must implement
    virtual RenderTarget* getPrimaryRenderTarget() const;
    virtual RenderTarget* createCachingRenderTarget();
    virtual void destroyCachingRenderTarget(RenderTarget* target);
    virtual void doRender();
    virtual void clearRenderList();
    virtual Texture* createTexture();
    virtual Texture* createTexture(const String& filename,
                                   const String& resourceGroup = "General");
    virtual Texture* createTexture(float size);
    virtual void destroyTexture(Texture* texture);
    virtual void destroyAllTextures();
    virtual uint getMaxTextureSize() const;
    virtual uint getHorzScreenDPI() const;
    virtual uint getVertScreenDPI() const;

    // Ogre has a specialised RP, so override this.
    virtual ResourceProvider* createResourceProvider();

protected:
    /// Size object used to track reported changes to the display size.
    Size d_displaySize;
    /// pointer to the primary RenderTarget object.
    RenderTarget* d_primaryTarget;
    /// List used to track textures.
    std::list<OgreCEGUITexture*> d_textureList;
    /// The scene manager we are currently hooked into.
    Ogre::SceneManager* d_ogreSceneManager;
    /// RenderQueueListener based object used to hook Ogre rendering.
    CEGUIRQListener* d_renderQueueListener;
};

} // End of  CEGUI namespace section

#endif  // end of guard _OgreCEGUIRenderer_h_
