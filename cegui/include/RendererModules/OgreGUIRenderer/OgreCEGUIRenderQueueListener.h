/***********************************************************************
    filename:   OgreCEGUIRenderQueueListener.h
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
#ifndef _OgreCEGUIRenderQueueListener_h_
#define _OgreCEGUIRenderQueueListener_h_

#include "OgreCEGUIRenderer.h"
#include <OgreRenderQueueListener.h>

// Start of CEGUI namespace section
namespace CEGUI
{
/**
 * Internally used Ogre::RenderQueueListener based class that enables us to
 * hook into the ogre rendering system.
 */
class _OgrePrivate CEGUIRQListener : public Ogre::RenderQueueListener
{
public:
    /**
     * Constructor the render queue listner.
     * @param queue_id id of target render queue.
     * @param post_queue whether to draw CEGUI after other rendering on queue.
     */
    CEGUIRQListener(Ogre::uint8 queue_id, bool post_queue);

    /**
     * Destructor for CEGUIRQListener objects.
     */
    virtual ~CEGUIRQListener();

    // implement required abstract interface members.
    virtual void renderQueueStarted(Ogre::uint8 id,
                                    const Ogre::String& invocation,
                                    bool& skipThisQueue);
    virtual void renderQueueEnded(Ogre::uint8 id,
                                  const Ogre::String& invocation,
                                  bool& repeatThisQueue);

    /**
     * Sets the render queue to be targetted by CEGUI rendering operations for
     * this RenderTarget.
     * @param queue_id Ogre::uint8 value indicating the ID of the render queue
     * that we wish rendering to be a part of.
     */
    void setTargetRenderQueue(Ogre::uint8 queue_id);

    /**
     * Sets whether rendering will occur before or after other rendering
     * operations in the same queue.
     * @param post_queue bool value specifying when rendering into the queue
     * should be done.  Set to true to have CEGUI rendering occur after other
     * rendering in the queue, or false to have CEGUI rendering occur first.
     */
    void setPostRenderQueue(bool post_queue);

    /**
     * Return the id of the RenderQueue we are targetting for CEGUI rendering.
     * @return Ogre::uint8 value indication the ID of the target render queue.
     */
    Ogre::uint8 getTargetRenderQueue() const;

    /**
     * Return whether we are drawing CEGUI content before other rendering in the
     * same queue or after other rendering.
     * @return true if CEGUI content gets rendered after other rendering, or
     * false if CEGUI content is rendered first.
     */
    bool getPostRenderQueue() const;

protected:
    /// ID of the queue that we are hooked into
    Ogre::uint8 d_queue_id;
    /// true if we render after everything else in our queue.
    bool d_post_queue;
};

} // End of  CEGUI namespace section

#endif  // end of guard _OgreCEGUIRenderQueueListener_h_
