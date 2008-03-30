/***********************************************************************
    filename:   OgreCEGUITextureTarget.h
    created:    Sat Mar 8 2008
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
#ifndef _OgreCEGUITextureTarget_h_
#define _OgreCEGUITextureTarget_h_

#include "OgreCEGUIRenderTarget.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/**
 * RenderTarget allowing rendering of GUI content to an Ogre texture.
 */
class OGRE_GUIRENDERER_API OgreTextureTarget : public OgreRenderTarget
{
public:
    /**
     * Default constructor for OgreTextureTarget objects.
     */
    OgreTextureTarget();

    /**
     * Destructor for OgreTextureTarget objects
     */
    virtual ~OgreTextureTarget();

    // interface from RenderTarget we are required to implement
    virtual void queueToTarget(const Rect& src, const Rect& dst, float z,
                               const ColourRect& cols,
                               const QuadSplitMode quad_split_mode,
                               RenderTarget& target);
    virtual void renderToTarget(const Rect& src, const Rect& dst, float z,
                                const ColourRect& cols,
                                const QuadSplitMode quad_split_mode,
                                RenderTarget& target);
    virtual void setArea(const Rect& area);
    virtual void declareRenderSize(const Size& sz);
    virtual bool isImageryCache() const;

protected:
    /// Default dimension of textures.
    static const int DEFAULT_SIZE = 128;

    /**
     * Takes the pixel area Rect \a pix_rect and converts its co-ordinates into
     * texture co-ordinates based on the dimensions of this RenderTarget and
     * stores the results in \a tex_rect.
     */
    void initTexRectFromPixRect(const Rect& pix_rect, Rect& tex_rect) const;

    /**
     * Generate a unique name to use when creating the texture to render to.
     */
    Ogre::String generateTextureName();

    /**
     * Create and do any required initialisation on the underlying texture.
     */
    void initialiseTargetTexture();

    /**
     * Discovers some texture related capabilites and stores results.
     */
    void discoverTextureCapabilities();

    /**
     * Given the Size \a sz return a Size object containing the sizes
     * adjusted as required by the limits of the system capabilites.
     */
    Size getAdjustedTextureSize(const Size& sz) const;

    /**
     * If \a size is not a power of 2, return the next power of two up from \a
     * size.  If \a size if a power of 2, return \a size.
     */
    uint getNextPowerOfTwo(uint size) const;

    // overridden from OgreRenderTarget
    virtual void beginRender();

    /// Texture object we use as a target for rendering to.
    Ogre::TexturePtr d_texture;
    /// whether textures can be non-power of two
    bool d_supportsNPOT;
    /// maximum texture size supported.
    int d_maxTextureSize;
    /// static integer used when generating texture names.
    static int s_rttID;
    /// count of the number of OgreTextureTarget objects in existence.
    static int s_instanceCount;
    /// scene manager object shared between instances.
    static Ogre::SceneManager* s_ogreSceneManager;
    /// camera object shared between instances.
    static Ogre::Camera* s_ogreCamera;

private:
    /// Copy not allowed
    OgreTextureTarget(const OgreTextureTarget& o) {}
    /// assignment not allowed
    OgreTextureTarget& operator=(const OgreTextureTarget& o) { return *this; }
};

} // End of  CEGUI namespace section

#endif  // end of guard _OgreCEGUITextureTarget_h_
