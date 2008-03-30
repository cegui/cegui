/***********************************************************************
    filename:   OgreCEGUIRenderQueueListener.cpp
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
#include "OgreCEGUIRenderQueueListener.h"
#include <CEGUI/CEGUISystem.h>
#include <CEGUI/CEGUIRenderTarget.h>

// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
CEGUIRQListener::CEGUIRQListener(Ogre::uint8 queue_id, bool post_queue) :
        d_queue_id(queue_id),
        d_post_queue(post_queue)
{
}

//----------------------------------------------------------------------------//
CEGUIRQListener::~CEGUIRQListener()
{
}

//----------------------------------------------------------------------------//
void CEGUIRQListener::renderQueueStarted(Ogre::uint8 id,
    const Ogre::String& invocation, bool& skipThisQueue)
{
    if (!d_post_queue && (d_queue_id == id))
        CEGUI::System::getSingleton().renderGUI();
}

//----------------------------------------------------------------------------//
void CEGUIRQListener::renderQueueEnded(Ogre::uint8 id,
    const Ogre::String& invocation, bool& repeatThisQueue)
{
    if (d_post_queue && (d_queue_id == id))
        CEGUI::System::getSingleton().renderGUI();
}

//----------------------------------------------------------------------------//
void CEGUIRQListener::setTargetRenderQueue(Ogre::uint8 queue_id)
{
    d_queue_id = queue_id;
}

//----------------------------------------------------------------------------//
void CEGUIRQListener::setPostRenderQueue(bool post_queue)
{
    d_post_queue = post_queue;
}

//----------------------------------------------------------------------------//
Ogre::uint8 CEGUIRQListener::getTargetRenderQueue() const
{
    return d_queue_id;
}

//----------------------------------------------------------------------------//
bool CEGUIRQListener::getPostRenderQueue() const
{
    return d_post_queue;
}

//----------------------------------------------------------------------------//
} // End of  CEGUI namespace section
