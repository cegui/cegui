/***********************************************************************
    created:    Mon Jan 12 2009
    author:     Paul D Turner
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIRenderingSurface_h_
#define _CEGUIRenderingSurface_h_

#include "CEGUI/EventSet.h"
#include "CEGUI/EventArgs.h"
#include "CEGUI/RenderQueue.h"

#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
//----------------------------------------------------------------------------//
//! Enumerated type for valid render queue IDs.
enum class RenderQueueID : int
{
    User0,
    //! Queue for rendering that appears beneath base imagery.
    Underlay,
    User1,
    //! Queue for base level rendering by the surface owner.
    Base,
    User2,
    //! Queue for first level of 'content' rendering.
    Content1,
    User3,
    //! Queue for second level of 'content' rendering.
    Content2,
    User4,
    //! Queue for overlay rendering that appears above other regular rendering.
    Overlay,
    User5
};

//----------------------------------------------------------------------------//

/*!
\brief
    EventArgs based class that is passed to handlers subcribed to hear about
    begin/end events on rendering queues that are part of a RenderingSurface
    object.
*/
class CEGUIEXPORT RenderQueueEventArgs : public EventArgs
{
public:
    /*!
    \brief
        Constructor for RenderQueueEventArgs objects.

    \param id
        RenderQueueID value indicating the queue that the event is being
        generated for.
    */
    RenderQueueEventArgs(const RenderQueueID id);

    //! ID of the queue that this event has been fired for.
    RenderQueueID queueID;
};

//----------------------------------------------------------------------------//

/*!
\brief
    Class that represents a surface that can have geometry based imagery drawn
    to it.

    A RenderingSurface has a number of queues that can be used for rendering;
    normal window rendering will typically be done on RenderQueueID::BASE queue, things that
    are overlaid everything else are rendered to RenderQueueID::Overlay.
    \par
    The event EventRenderQueueStarted is fired before each queue is rendered and
    the event EventRenderQueueEnded is fired after each queue is rendered.
    \note
    For performance reasons, events are only fired for queues that are in use;
    these are queues that have had some interaction - such as clearing or adding
    geometry.
*/
class CEGUIEXPORT RenderingSurface :
    public EventSet
{
public:
    //! Namespace for global events from RenderingSurface objects.
    static const String EventNamespace;
    /** Event fired when rendering of a RenderQueue begins for the
     * RenderingSurface.
     * Handlers are passed a const RenderQueueEventArgs reference with
     * RenderQueueEventArgs::queueID set to one of the ::RenderQueueID
     * enumerated values indicating the queue that is about to start
     * rendering.
     */
    static const String EventRenderQueueStarted;
    /** Event fired when rendering of a RenderQueue completes for the
     * RenderingSurface.
     * Handlers are passed a const RenderQueueEventArgs reference with
     * RenderQueueEventArgs::queueID set to one of the ::RenderQueueID
     * enumerated values indicating the queue that has completed rendering.
     */
    static const String EventRenderQueueEnded;

    /*!
    \brief
        Constructor for RenderingSurface objects.

    \param target
        RenderTarget object that will receive rendered output from the
        RenderingSurface being created.

    \note
        The RenderingSurface does not take ownership of \a target.  When the
        RenderingSurface is finally destroyed, the RenderTarget will not have
        been destroyed, and it should be destoyed by whover created it, if that
        is desired.  One reason for this is that there is not an exclusive one
        to one mapping from RenderingSurface to RenderTarget objects; it's
        entirely feasable that multiple RenderingSurface objects could be
        targetting a shared RenderTarget).
    */
    RenderingSurface(RenderTarget& target);

    //! Destructor for RenderingSurface objects.
    virtual ~RenderingSurface();

    /*!
    \brief
        Add the specified GeometryBuffers to the specified queue for rendering
        when the RenderingSurface is drawn.

    \param queue
        One of the RenderQueueID enumerated values indicating which prioritised
        queue the GeometryBuffer should be added to.

    \param geometryBuffers
        List of GeometryBuffers to be added to the specified rendering queue.

    \note
        The RenderingSurface does not take ownership of the GeometryBuffers, and
        does not destroy it when the RenderingSurface geometry is cleared.
        Rather, the RenderingSurface is just maintaining a list of things to be
        drawn; the actual GeometryBuffers can be re-used by whichever object
        does own them, and even changed or updated while still "attached" to
        a RenderingSurface.
    */
    void addGeometryBuffers(const RenderQueueID queue,
                            const std::vector<GeometryBuffer*>& geometry_buffers);

    /*!
    \brief
        Add the specified GeometryBuffer to the specified queue for rendering
        when the RenderingSurface is drawn.

    \param queue
        One of the RenderQueueID enumerated values indicating which prioritised
        queue the GeometryBuffer should be added to.

    \param geometry_buffers
        The GeometryBuffer to be added to the specified rendering queue.

    \note
        The RenderingSurface does not take ownership of the GeometryBuffer, and
        does not destroy it when the RenderingSurface geometry is cleared.
        Rather, the RenderingSurface is just maintaining a list of things to be
        drawn; the actual GeometryBuffers can be re-used by whichever object
        does own them, and even changed or updated while still "attached" to
        a RenderingSurface.
    */
    void addGeometryBuffer(const RenderQueueID queue,
                           const GeometryBuffer& geometry_buffer);

    /*!
    \brief
        Remove the specified GeometryBuffer from the specified queue.

    \param queue
        One of the RenderQueueID enumerated values indicating which prioritised
        queue the GeometryBuffer should be removed from.

    \param geometry_buffer
        GeometryBuffer object to be removed from the specified rendering queue.
    */
    void removeGeometryBuffer(const RenderQueueID queue,
                              const GeometryBuffer& geometry_buffer);

    /*!
    \brief
        Clears all GeometryBuffers from the specified rendering queue.

    \param queue
        One of the RenderQueueID enumerated values indicating which prioritised
        queue is to to be cleared.

    \note
        Clearing a rendering queue does not destroy the attached GeometryBuffers,
        which remain under thier original ownership.
    */
    void clearGeometry(const RenderQueueID queue);

    /*!
    \brief
        Clears all GeometryBuffers from all rendering queues.

    \note
        Clearing the rendering queues does not destroy the attached GeometryBuffers,
        which remain under their original ownership.
    */
    void clearGeometry();

    /*!
    \brief
        Draw the GeometryBuffers for all rendering queues to the RenderTarget
        that this RenderingSurface is targetting.

        The GeometryBuffers remain in the rendering queues after the draw
        operation is complete.  This allows the next draw operation to occur
        without needing to requeue all the GeometryBuffers (if for instance the
        sequence of buffers to be drawn remains unchanged).
    */
    virtual void draw();

    /*!
    \brief
        Marks the RenderingSurface as invalid, causing the geometry to be
        rerendered to the RenderTarget next time draw is called.

    \brief
        Note that some surface types can never be in a 'valid' state and so
        rerendering occurs whenever draw is called.  This function mainly exists
        as a means to hint to other surface types - those that physically cache
        the rendered output - that geometry content has changed and the cached
        imagery should be cleared and redrawn.
    */
    virtual void invalidate();

    /*!
    \brief
        Return whether this RenderingSurface is invalidated.

    \return
        - true to indicate the RenderingSurface is invalidated and will be
        rerendered the next time the draw member function is called.
        - false to indicate the RenderingSurface is valid, and will not be
        rerendered the next time the draw member function is called, since it's
        cached imagery is up-to-date.

    \brief
        Note that some surface types can never be in a 'valid' state and so
        will always return true.
    */
    bool isInvalidated() const;

    /*!
    \brief
        Return whether this RenderingSurface is actually an instance of the
        RenderingWindow subclass.

    \return
        - true to indicate the RenderingSurface is a RenderingWindow instance.
        - false to indicate the RenderingSurface is not a RenderingWindow.
    */
    virtual bool isRenderingWindow() const;

    /*!
    \brief
        Create and return a reference to a child RenderingWindow object that
        will render back onto this RenderingSurface when it's draw member
        function is called.

        The RenderingWindow returned is initially owned by the RenderingSurface
        that created it.

    \param target
        TextureTarget object that will receive rendered output from the
        RenderingWindow being creatd.

    \return
        Reference to a RenderingWindow object.

    \note
        Since RenderingWindow is a RenderingSurface, the same note from the
        constructor applies here, and that is the passed in TextureTarget
        remains under the ownership of whichever part of the system created
        it.
    */
    virtual RenderingWindow& createRenderingWindow(TextureTarget& target);

    /*!
    \brief
        Destroy a RenderingWindow we own.  If we are not the present owner of
        the given RenderingWindow, nothing happens.

    \param window
        RenderingWindow object that is to be destroyed.

    \note
        Destroying a RenderingWindow will not also destroy the TextureTarget
        that was given when the RenderingWindow was created.  The TextureTarget
        should be destoyed elsewhere.
    */
    virtual void destroyRenderingWindow(RenderingWindow& window);

    //! transfer ownership of the RenderingWindow to this RenderingSurface.
    /*!
    \brief
        Transfer ownership of the given RenderingWindow to this
        RenderingSurface.  The result is \e generally the same as if this
        RenderingSurface had created the RenderingWindow in the first place.

    \param window
        RenderingWindow object that this RenderingSurface is to take ownership
        of.
    */
    virtual void transferRenderingWindow(RenderingWindow& window);

    /*!
    \brief
        Return the RenderTarget object that this RenderingSurface is drawing
        to.

    \return
        RenderTarget object that the RenderingSurface is using to draw it's
        output.
    */
    const RenderTarget& getRenderTarget() const;
    RenderTarget& getRenderTarget();

protected:
    /** draw the surface content. Default impl draws the render queues.
     * NB: Called between RenderTarget activate and deactivate calls.
     */
    virtual void drawContent();

    //! draw a rendering queue, firing events before and after.
    void draw(const RenderQueue& queue, RenderQueueEventArgs& args);

    //! detatch ReneringWindow from this RenderingSurface
    void detatchWindow(RenderingWindow& w);

    //! attach ReneringWindow from this RenderingSurface
    void attachWindow(RenderingWindow& w);

    //! collection type for the queues
    typedef std::map<RenderQueueID, RenderQueue> RenderQueueList;
    //! collection type for created RenderingWindow objects
    typedef std::vector<RenderingWindow*> RenderingWindowList;
    //! the collection of RenderQueue objects.
    RenderQueueList d_queues;
    //! collection of RenderingWindow object we own
    RenderingWindowList d_windows;
    //! RenderTarget that this surface actually draws to.
    RenderTarget* d_target;
    //! holds invalidated state of target (as far as we are concerned)
    bool d_invalidated;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIRenderingSurface_h_
