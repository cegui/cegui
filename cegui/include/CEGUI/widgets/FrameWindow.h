/***********************************************************************
    created:    13/4/2004
    author:     Paul D Turner

    purpose:    Interface to base class for FrameWindow
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
#pragma once
#include "../Window.h"

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! \brief Abstract base class for a movable, sizable, window with a title-bar and a frame.
class CEGUIEXPORT FrameWindow : public Window
{
public:
    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name

    /** Event fired when the rollup (shade) state of the window is changed.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the FrameWindow whose rolled up state
     * has been changed.
     */
    static const String EventRollupToggled;
    /** Event fired when the close button for the window is clicked.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the FrameWindow whose close button was
     * clicked.
     */
    static const String EventCloseClicked;
    /** Event fired when drag-sizing of the window starts.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the FrameWindow that has started to be
     * drag sized.
     */
    static const String EventDragSizingStarted;
    /** Event fired when drag-sizing of the window ends.
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the FrameWindow for which drag sizing has
     * ended.
     */
    static const String EventDragSizingEnded;

    static constexpr float DefaultSizingBorderSize = 8.f;    //!< Default size for the sizing border (in pixels)

    static const String TitlebarName;      //!< Widget name for the titlebar component.
    static const String CloseButtonName;   //!< Widget name for the close button component.

    /*!
    \brief
        Enumeration that defines the set of possible locations for the cursor
        on a frame windows sizing border.
    */
    enum class SizingLocation : int
    {
        Invalid,        //!< Position is not a sizing location.
        TopLeft,        //!< Position will size from the top-left.
        TopRight,       //!< Position will size from the top-right.
        BottomLeft,     //!< Position will size from the bottom left.
        BottomRight,    //!< Position will size from the bottom right.
        Top,            //!< Position will size from the top.
        Left,           //!< Position will size from the left.
        Bottom,         //!< Position will size from the bottom.
        Right           //!< Position will size from the right.
    };

    FrameWindow(const String& type, const String& name);

    /*!
    \brief
        Initialises the Window based object ready for use.

    \note
        This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.
    */
    void initialiseComponents() override;

    /*!
    \brief
        Return whether this window is sizable.  Note that this requires that the window have an enabled frame and that sizing itself is enabled

    \return
        true if the window can be sized, false if the window can not be sized
    */
    bool isSizingEnabled() const { return d_sizingEnabled && isFrameEnabled(); }

    /*!
    \brief
        Return whether the frame for this window is enabled.

    \return
        true if the frame for this window is enabled, false if the frame for this window is disabled.
    */
    bool isFrameEnabled() const { return d_frameEnabled; }

    /*!
    \brief
        Return whether the title bar for this window is enabled.

    \return
        true if the window has a title bar and it is enabled, false if the window has no title bar or if the title bar is disabled.
    */
    bool isTitleBarEnabled() const;

    /*!
    \brief
        Return whether this close button for this window is enabled.

    \return
        true if the window has a close button and it is enabled, false if the window either has no close button or if the close button is disabled.
    */
    bool isCloseButtonEnabled() const;

    /*!
    \brief
        Return whether roll up (a.k.a shading) is enabled for this window.

    \return
        true if roll up is enabled, false if roll up is disabled.
    */
    bool isRollupEnabled() const { return d_rollupEnabled; }

    /*!
    \brief
        Sets whether the window is currently rolled up (a.k.a shaded).

    \see
        Window::isRolledup
    */
    void setRolledup(bool val);

    /*!
    \brief
        Return whether the window is currently rolled up (a.k.a shaded).

    \return
        true if the window is rolled up, false if the window is not rolled up.
    */
    bool isRolledup() const { return d_rolledup; }

    /*!
    \brief
        Return the thickness of the sizing border.

    \return
        float value describing the thickness of the sizing border in screen pixels.
    */
    float getSizingBorderThickness() const { return d_borderSize; }

    /*!
    \brief
        Enables or disables sizing for this window.

    \param setting
        set to true to enable sizing (also requires frame to be enabled), or false to disable sizing.
    */
    void setSizingEnabled(bool setting) { d_sizingEnabled = setting; }

    /*!
    \brief
        Enables or disables the frame for this window.

    \param setting
        set to true to enable the frame for this window, or false to disable the frame for this window.
    */
    void setFrameEnabled(bool setting);

    /*!
    \brief
        Enables or disables the title bar for the frame window.

    \param setting
        set to true to enable the title bar (if one is attached), or false to disable the title bar.
    */
    void setTitleBarEnabled(bool setting);

    /*!
    \brief
        Enables or disables the close button for the frame window.

    \param setting
        Set to true to enable the close button (if one is attached), or false to disable the close button.
    */
    void setCloseButtonEnabled(bool setting);

    /*!
    \brief
        Enables or disables roll-up (shading) for this window.

    \param setting
        Set to true to enable roll-up for the frame window, or false to disable roll-up.
    */
    void setRollupEnabled(bool setting);

    /*!
    \brief
        Toggles the state of the window between rolled-up (shaded) and normal sizes.  This requires roll-up to be enabled.
    */
    void toggleRollup();

    /*!
    \brief
        Set the size of the sizing border for this window.

    \param pixels
        float value specifying the thickness for the sizing border in screen pixels.
    */
    void setSizingBorderThickness(float pixels) { d_borderSize = pixels; }

    /*!
    \brief
        Return whether this FrameWindow can be moved by dragging the title bar.

    \return
        true if the Window will move when the user drags the title bar, false if the window will not move.
    */
    bool isDragMovingEnabled() const { return d_dragMovable; }

    /*!
    \brief
        Set whether this FrameWindow can be moved by dragging the title bar.

    \param setting
        true if the Window should move when the user drags the title bar, false if the window should not move.
    */
    void setDragMovingEnabled(bool setting);

    /*!
    \brief
        Return a cursor to the currently set Image to be used for the north-south
        sizing cursor.

    \return
        Pointer to an Image object, or nullptr for none.
    */
    const Image* getNSSizingIndicatorImage() const { return d_nsSizingCursor; }

    /*!
    \brief
        Return a cursor to the currently set Image to be used for the east-west
        sizing cursor.

    \return
        Pointer to an Image object, or nullptr for none.
    */
    const Image* getEWSizingIndicatorImage() const { return d_ewSizingCursor; }

    /*!
    \brief
        Return a cursor to the currently set Image to be used for the northwest-southeast
        sizing cursor.

    \return
        Pointer to an Image object, or nullptr for none.
    */
    const Image* getNWSESizingIndicatorImage() const { return d_nwseSizingCursor; }

    /*!
    \brief
        Return a cursor to the currently set Image to be used for the northeast-southwest
        sizing cursor.

    \return
        Pointer to an Image object, or nullptr for none.
    */
    const Image* getNESWSizingIndicatorImage() const { return d_neswSizingCursor; }

    /*!
    \brief
        Set the Image to be used for the north-south sizing cursor.

    \param image
        Pointer to an Image object, or nullptr for none.
    */
    void setNSSizingIndicatorImage(const Image* image) { d_nsSizingCursor = image; }

    /*!
    \brief
        Set the Image to be used for the east-west sizing cursor.

    \param image
        Pointer to an Image object, or nullptr for none.
    */
    void setEWSizingIndicatorImage(const Image* image) { d_ewSizingCursor = image; }

    /*!
    \brief
        Set the Image to be used for the northwest-southeast sizing cursor.

    \param image
        Pointer to an Image object, or nullptr for none.
    */
    void setNWSESizingIndicatorImage(const Image* image) { d_nwseSizingCursor = image; }

    /*!
    \brief
        Set the Image to be used for the northeast-southwest sizing cursor.

    \param image
        Pointer to an Image object, or nullptr for none.
    */
    void setNESWSizingIndicatorImage(const Image* image) { d_neswSizingCursor = image; }

    // overridden from Window class
    bool isHit(const glm::vec2& position, bool allow_disabled) const override;

    /*!
    \brief
        Return a pointer to the Titlebar component widget for this FrameWindow.

    \return
        Pointer to a Titlebar object.

    \exception UnknownObjectException
        Thrown if the Titlebar component does not exist.
    */
    Titlebar* getTitlebar() const;

    /*!
    \brief
        Return a pointer to the close button component widget for this
        FrameWindow.

    \return
        Pointer to a PushButton object.

    \exception UnknownObjectException
        Thrown if the close button component does not exist.
    */
    PushButton* getCloseButton() const;

    /*!
    \brief
        check local pixel co-ordinate point 'pt' and return one of the
        SizingLocation enumerated values depending where the point falls on
        the sizing border.

    \param pt
        vec2 object describing, in pixels, the window relative offset to check.

    \return
        One of the SizingLocation enumerated values that describe which part of
        the sizing border that \a pt corresponded to, if any.
    */
    SizingLocation getSizingBorderAtPoint(const glm::vec2& pt) const;

protected:

    /*!
    \brief
        move the window's left edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in which direction.  Positive values make window smaller.
    */
    void moveLeftEdge(float delta, URect& outArea);

    /*!
    \brief
        move the window's right edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in which direction.  Positive values make window larger.
    */
    void moveRightEdge(float delta, URect& outArea);

    /*!
    \brief
        move the window's top edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in which direction.  Positive values make window smaller.
    */
    void moveTopEdge(float delta, URect& outArea);

    /*!
    \brief
        move the window's bottom edge by 'delta'.  The rest of the window does not move, thus this changes the size of the Window.

    \param delta
        float value that specifies the amount to move the window edge, and in which direction.  Positive values make window larger.
    */
    void moveBottomEdge(float delta, URect& outArea);

    /*!
    \brief
        return true if given SizingLocation is on left edge.

    \param loc
        SizingLocation value to be checked.

    \return
        true if \a loc is on the left edge.  false if \a loc is not on the left edge.
    */
    bool isLeftSizingLocation(SizingLocation loc) const { return ((loc == SizingLocation::Left) || (loc == SizingLocation::TopLeft) || (loc == SizingLocation::BottomLeft)); }

    /*!
    \brief
        return true if given SizingLocation is on right edge.

    \param loc
        SizingLocation value to be checked.

    \return
        true if \a loc is on the right edge.  false if \a loc is not on the right edge.
    */
    bool isRightSizingLocation(SizingLocation loc) const { return ((loc == SizingLocation::Right) || (loc == SizingLocation::TopRight) || (loc == SizingLocation::BottomRight)); }

    /*!
    \brief
        return true if given SizingLocation is on top edge.

    \param loc
        SizingLocation value to be checked.

    \return
        true if \a loc is on the top edge.  false if \a loc is not on the top edge.
    */
    bool isTopSizingLocation(SizingLocation loc) const { return ((loc == SizingLocation::Top) || (loc == SizingLocation::TopLeft) || (loc == SizingLocation::TopRight)); }

    /*!
    \brief
        return true if given SizingLocation is on bottom edge.

    \param loc
        SizingLocation value to be checked.

    \return
        true if \a loc is on the bottom edge.  false if \a loc is not on the bottom edge.
    */
    bool isBottomSizingLocation(SizingLocation loc) const { return ((loc == SizingLocation::Bottom) || (loc == SizingLocation::BottomLeft) || (loc == SizingLocation::BottomRight)); }

    //! Set the appropriate cursor for the given sizing location.
    void setCursorForSizingLocation(SizingLocation location) const;
    //! Return a Rect that describes, in window relative pixel co-ordinates, the outer edge of the sizing area for this window.
    virtual Rectf getSizingRect() const { return Rectf(0, 0, d_pixelSize.d_width, d_pixelSize.d_height); }

    //! Event generated internally whenever the roll-up / shade state of the window changes.
    virtual void onRollupToggled(WindowEventArgs& e);
    //! Event generated internally whenever the close button is clicked.
    virtual void onCloseClicked(WindowEventArgs& e);
    //! Handler called when drag-sizing of the FrameWindow starts.
    virtual void onDragSizingStarted(WindowEventArgs& e);
    //! Handler called when drag-sizing of the FrameWindow ends.
    virtual void onDragSizingEnded(WindowEventArgs& e);

    void onCursorMove(CursorMoveEventArgs& e) override;
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onMouseButtonUp(MouseButtonEventArgs& e) override;
    void onCaptureLost(WindowEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;
    void onActivated(ActivationEventArgs& e) override;
    void onDeactivated(ActivationEventArgs& e) override;

    const Image* d_nsSizingCursor = nullptr;      //!< North/South sizing cursor image.
    const Image* d_ewSizingCursor = nullptr;      //!< East/West sizing cursor image.
    const Image* d_nwseSizingCursor = nullptr;    //!< North-West/South-East cursor image.
    const Image* d_neswSizingCursor = nullptr;    //!< North-East/South-West cursor image.

    glm::vec2 d_dragPoint;                        //!< point window is being dragged at.
    float d_borderSize = DefaultSizingBorderSize; //!< thickness of the sizing border around this window

    bool d_dragMovable = true;                    //!< true if the window will move when dragged by the title bar.
    bool d_frameEnabled = true;                   //!< true if window frame should be drawn.
    bool d_rollupEnabled = true;                  //!< true if roll-up of window is allowed.
    bool d_rolledup = false;                      //!< true if window is rolled up.
    bool d_sizingEnabled = true;                  //!< true if sizing is enabled for this window.
    bool d_beingSized = false;                    //!< true if window is being sized.

private:

    void addFrameWindowProperties();
};

}

#if defined(_MSC_VER)
#    pragma warning(pop)
#endif
