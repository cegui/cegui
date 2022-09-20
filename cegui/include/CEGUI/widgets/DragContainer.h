/***********************************************************************
created:    14/2/2005
author:        Paul D Turner
*************************************************************************/
/***************************************************************************
*   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIDragContainer_h_
#define _CEGUIDragContainer_h_

#include "../Window.h"
#include "../WindowFactory.h"

// FIXME: add CEGUIEXPORT to UVector2?
#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4251)
#endif

namespace CEGUI
{
/*!
\brief
    Generic drag & drop enabled window class
*/
class CEGUIEXPORT DragContainer : public Window
{
public:
    /*************************************************************************
        Constants
    *************************************************************************/
    static const String WidgetTypeName;     //!< Type name for DragContainer.
    static const String EventNamespace;     //!< Namespace for global events
    /** Event fired when the user begins dragging the DragContainer.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the DragContainer that the user
        * has started to drag.
        */
    static const String EventDragStarted;
    /** Event fired when the user releases the DragContainer.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the DragContainer that the user has
        * released.
        */
    static const String EventDragEnded;
    /** Event fired when the drag position has changed.
        * Handlers are passed a const CursorMoveEventArgs reference with
        * window set to the DragContainer and moveDelta set to movement amount.
        */
    static const String EventDragPositionChanged;
    /** Event fired when dragging is enabled or disabled.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the DragContainer whose setting has
        * been changed.
        */
    static const String EventDragEnabledChanged;
    /** Event fired when the alpha value used when dragging is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the DragContainer whose drag alpha
        * value has been changed.
        */
    static const String EventDragAlphaChanged;
    /** Event fired when the cursor used when dragging is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the DragContainer whose dragging
        * cursor image has been changed.
        */
    static const String EventDragCursorChanged;
    /** Event fired when the drag pixel threshold is changed.
        * Handlers are passed a const WindowEventArgs reference with
        * WindowEventArgs::window set to the DragContainer whose dragging pixel
        * threshold has been changed.
        */
    static const String EventDragThresholdChanged;
    /** Event fired when the drop target changes.
        * Handlers are passed a const DragDropEventArgs reference with
        * WindowEventArgs::window set to the Window that is now the target
        * window  and DragDropEventArgs::dragDropItem set to the DragContainer
        * whose target has changed.
        */
    static const String EventDragDropTargetChanged;

    DragContainer(const String& type, const String& name);

    /*************************************************************************
        Public Interface to DragContainer
    *************************************************************************/
    /*!
    \brief
        Return whether the ragging is enabled, i.e. this DragContainer can be dragged.
    */
    bool isDraggingEnabled() const { return d_draggingEnabled; }

    /*!
    \brief
        Set whether dragging is currently enabled for this DragContainer.
    */
    void setDraggingEnabled(bool setting);

    /*!
    \brief
        Return whether the DragContainer is currently being dragged.
    */
    bool isBeingDragged() const { return d_dragging; }

    /*!
    \brief
        Return the current drag threshold in pixels.

        The drag threshold is the number of pixels that the cursor must be
        moved with the left source held pressed in order to commence a drag
        operation.
    */
    float getPixelDragThreshold() const { return d_dragThreshold; }

    /*!
    \brief
        Set the current drag threshold in pixels.

        The drag threshold is the number of pixels that the cursor must be
        moved with the left source held pressed in order to commence a drag
        operation.
    */
    void setPixelDragThreshold(float pixels);

    /*!
    \brief
        Return the alpha value that will be set on the DragContainer while a drag operation is
        in progress.
    */
    float getDragAlpha() const { return d_dragAlpha; }

    /*!
    \brief
        Set the alpha value to be set on the DragContainer when a drag operation is
        in progress.

        This method can be used while a drag is in progress to update the alpha.  Note that
        the normal setAlpha method does not affect alpha while a drag is in progress, but
        once the drag operation has ended, any value set via setAlpha will be restored.
    */
    void setDragAlpha(float alpha);

    /*!
    \brief
        Return the Image currently set to be used for the cursor
        when a drag operation is in progress.
    */
    const Image* getDragIndicatorImage() const;

    /*!
    \brief
        Set the Image to be used for the cursor when a drag
        operation is in progress.

        This method may be used during a drag operation to update the current
        cursor image.
    */
    void setDragIndicatorImage(const Image* image);

    /*!
    \brief
        Set the Image to be used for the cursor when a drag
        operation is in progress.

        This method may be used during a drag operation to update the current
        cursor image.

    \exception UnknownObjectException thrown if Image \name is unknown.
    */
    void setDragIndicatorImage(const String& name);

    /*!
    \brief
        Return the Window object that is the current drop target for the DragContainer.

        The drop target for a DragContainer is basically the Window that the DragContainer
        is within while being dragged.  The drop target may be 0 to indicate no target.

    \return
        Pointer to a Window object that contains the DragContainer whilst being dragged, or
        nullptr to indicate no current target.
    */
    Window* getCurrentDropTarget() const { return d_dropTarget; }

    /*!
    \brief
        Return whether sticky mode is enable or disabled.
    */
    bool isStickyModeEnabled() const { return d_stickyMode; }

    /*!
    \brief
        Enable or disable sticky mode.
    */
    void setStickyModeEnabled(bool setting) { d_stickyMode = setting; }

    /*!
    \brief
        Immediately pick up the DragContainer and optionally set the sticky
        mode in order to allow this to happen. Any current interaction
        (i.e. cursor capture) will be interrupted.

    \param force_sticky
        - true to automatically enable the sticky mode in order to
        facilitate picking up the DragContainer.
        - false to ignore the pick up request if the sticky mode is not
        already enabled (default).

    \return
        true if the DragContainer was successfully picked up, false otherwise.
    */
    bool pickUp(bool force_sticky = false);

    /*!
    \brief
        Cancels dragging of this container without dropping it
    */
    void cancelDragging();

    /*!
    \brief
        Set the fixed cursor dragging offset to be used when dragging
        this DragContainer.

    \note
        This offset is only used if it's use is enabled via the
        setUsingFixedDragOffset function.
    */
    void setFixedDragOffset(const UVector2& offset) { d_fixedDragOffset = offset; }

    /*!
    \brief
        Return the fixed cursor dragging offset to be used when dragging
        this DragContainer.

    \note
        This offset is only used if it's use is enabled via the
        setUsingFixedDragOffset function.
    */
    const UVector2& getFixedDragOffset() const { return d_fixedDragOffset; }

    /*!
    \brief
        Set whether the fixed dragging offset (as set with the
        setFixedDragOffset function) will be used, or whether the built-in
        positioning will be used.

    \param enable
        - true to enabled the use of the fixed offset.
        - false to use the regular logic.
    */
    void setUsingFixedDragOffset(bool enable) { d_usingFixedDragOffset = enable; }

    /*!
    \brief
        Return whether the fixed dragging offset (as set with the
        setFixedDragOffset function) will be used, or whether the built-in
        positioning will be used.
    */
    bool isUsingFixedDragOffset() const { return d_usingFixedDragOffset; }

protected:
    /*************************************************************************
        Protected Implementation Methods
    *************************************************************************/

    // Window class overrides.
    void getRenderingContext_impl(RenderingContext& ctx) const override;

    /*!
    \brief
        Return whether the required minimum movement threshold before initiating dragging
        has been exceeded.

    \param local_cursor
        Cursor position as a pixel offset from the top-left corner of this window.

    \return
        - true if the threshold has been exceeded and dragging should be initiated.
        - false if the threshold has not been exceeded.
    */
    bool isDraggingThresholdExceeded(const glm::vec2& local_cursor);

    void doDragging(const CursorInputEventArgs& e);

    void endDragging(bool restorePosition);

    void updateActiveCursorImage() const;

    void updateDropTarget();

    /*************************************************************************
        Overrides for Event handler methods
    *************************************************************************/
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onMouseButtonUp(MouseButtonEventArgs& e) override;
    void onClick(MouseButtonEventArgs& e) override;
    void onCursorMove(CursorMoveEventArgs& e) override;
    void onCaptureLost(WindowEventArgs& e) override;
    void onAlphaChanged(WindowEventArgs& e) override;
    void onClippingChanged(WindowEventArgs& e) override;

    /*************************************************************************
        New Event handler methods
    *************************************************************************/
    /*!
    \brief
        Method called when dragging commences

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onDragStarted(CursorInputEventArgs& e);

    /*!
    \brief
        Method called when dragging ends.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onDragEnded(WindowEventArgs& e);

    /*!
    \brief
        Method called when the dragged object position is changed.

    \param e
        WindowEventArgs object containing any relevant data.
    */
    virtual void onDragPositionChanged(CursorMoveEventArgs& e);

    /*!
    \brief
        Method called when the dragging state is enabled or disabled
    \param e
        WindowEventArgs object.
    */
    virtual void onDragEnabledChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the alpha value to use when dragging is changed.
    \param e
        WindowEventArgs object.
    */
    virtual void onDragAlphaChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the cursor to use when dragging is changed.
    \param e
        WindowEventArgs object.
    */
    virtual void onDragCursorChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the movement threshold required to trigger dragging is changed.
    \param e
        WindowEventArgs object.
    */
    virtual void onDragThresholdChanged(WindowEventArgs& e);

    /*!
    \brief
        Method called when the current drop target of this DragContainer changes.
    \note
        This event fires just prior to the target field being changed.  The default implementation
        changes the drop target, you can examine the old and new targets before calling the default
        implementation to make the actual change (and fire appropriate events for the Window objects
        involved).
    \param e
        DragDropEventArgs object initialised as follows:
        - dragDropItem is initialised to the DragContainer triggering the event (typically 'this').
        - window is initialised to point to the Window which will be the new drop target.
    */
    virtual void onDragDropTargetChanged(DragDropEventArgs& e);

    /*************************************************************************
        Data
    *************************************************************************/

    Event::ScopedConnection d_targetDestroyConnection;

    Window*      d_dropTarget;               //!< Target window for possible drop operation.
    const Image* d_dragIndicatorImage;       //!< Image to use for cursor when dragging.
    UVector2     d_dragPoint;                //!< point we are being dragged at.
    UVector2     d_startPosition;            //!< position prior to dragging.
    UVector2     d_fixedDragOffset;          //!< current fixed cursor offset value.
    float        d_dragThreshold;            //!< Pixels cursor must move before dragging commences.
    float        d_dragAlpha;                //!< Alpha value to set when dragging.
    float        d_storedAlpha;              //!< Alpha value to re-set when dragging ends.
    bool         d_draggingEnabled : 1;      //!< True when dragging is enabled.
    bool         d_leftPointerHeld : 1;      //!< True when left cursor source is held.
    bool         d_dragging : 1;             //!< true when being dragged.
    bool         d_storedClipState : 1;      //!< Parent clip state to re-set.
    bool         d_stickyMode : 1;           //!< true when we're in 'sticky' mode.
    bool         d_pickedUp : 1;             //!< true after been picked-up / dragged via sticky mode
    bool         d_usingFixedDragOffset : 1; //!< true if fixed cursor offset is used for dragging position.

private:
    /*!
    \brief
        Adds properties specific to the DragContainer base class.
    */
    void addDragContainerProperties();
};

} // End of  CEGUI namespace section

// FIXME: add CEGUIEXPORT to UVector2?
#if defined(_MSC_VER)
#    pragma warning(pop)
#endif

#endif    // end of guard _CEGUIDragContainer_h_
