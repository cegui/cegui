/***********************************************************************
    filename:   OgreCEGUIOgreRenderTargetTarget.h
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
#ifndef _OgreCEGUIOgreRenderTargetTarget_h_
#define _OgreCEGUIOgreRenderTargetTarget_h_

#include "OgreCEGUIRenderTarget.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/**
 * RenderTarget allowing rendering of GUI content to an arbitary Ogre render
 * target.
 * @note use the specialised OgreTextureTarget as your caching RenderTarget
 * type since it advertises its caching ability and performs additional
 * management tasks (whereas, in both cases, OgreRenderTargetTarget does not).
 */
class OGRE_GUIRENDERER_API OgreRenderTargetTarget : public OgreRenderTarget
{
public:
    /**
     * Constructor for OgreRenderTargetTarget objects.
     * @param target pointer to the Ogre::RenderTarget object that will receive
     * CEGUI rendering output send to this CEGUI::RenderTarget.  If this is 0
     * the RenderTarget will default to using the auto-created rendering
     * window.
     */
    OgreRenderTargetTarget(Ogre::RenderTarget* target = 0);

    /**
     * Destructor for OgreRenderTargetTarget objects
     */
    virtual ~OgreRenderTargetTarget();

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
};

} // End of  CEGUI namespace section

#endif  // end of guard _OgreCEGUIOgreRenderTargetTarget_h_
