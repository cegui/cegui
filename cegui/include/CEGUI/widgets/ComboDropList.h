/***********************************************************************
    created:    13/6/2004
    author:        Paul D Turner

    purpose:    Interface for the Combobox Drop-List widget base class
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
#include "CEGUI/widgets/ListWidget.h"

namespace CEGUI
{
/*!
\brief
    Base class for the combo box drop down list.
    This is a specialisation of the ListWidget class.
*/
class CEGUIEXPORT ComboDropList : public ListWidget
{
public:
    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name

    /** Event fired when the user confirms the selection by activation (of the cursor).
     * Handlers are passed a const WindowEventArgs reference with
     * WindowEventArgs::window set to the ComboDropList whose selection has been
     * confirmed by the user.
     */
    static const String EventListSelectionAccepted;

    ComboDropList(const String& type, const String& name);

    /*!
    \brief
        Initialise the Window based object ready for use.

    \note
        This must be called for every window created.  Normally this is handled automatically by the WindowFactory for each Window type.
    */
    void initialiseComponents() override;

    /*!
    \brief
        Set whether the drop-list is 'armed' for selection.

    \note
        This setting is not exclusively under client control; the ComboDropList will auto-arm in
        response to certain cursor left source events.  This is also dependent upon the autoArm
        setting of the ComboDropList.

    \param setting
        - true to arm the box; items will be highlighted and the next left button up event
        will cause dismissal and possible item selection.

        - false to disarm the box; items will not be highlighted or selected until the box is armed.
    */
    void setArmed(bool setting) { d_armed = setting; }

    /*!
    \brief
        Return the 'armed' state of the ComboDropList.

    \return
        - true if the box is armed; items will be highlighted and the next left button up event
        will cause dismissal and possible item selection.

        - false if the box is not armed; items will not be highlighted or selected until the box is armed.
    */
    bool isArmed() const { return d_armed; }

    /*!
    \brief
        Set the mode of operation for the ComboDropList.

    \param setting
        - true if the ComboDropList auto-arms when the cursor enters the box.
        - false if the user must activate to arm the box.
    */
    void setAutoArmEnabled(bool setting) { d_autoArm = setting; }

    /*!
    \brief
        returns the mode of operation for the drop-list

    \return
        - true if the ComboDropList auto-arms when the cursor enters the box.
        - false if the user must click to arm the box.
    */
    bool isAutoArmEnabled() const { return d_autoArm; }

    //! resize the widget such that the content is shown without scrollbars.
    void resizeToContent(bool fit_width, bool fit_height);

protected:

    //! Handler for when list selection is confirmed.
    void onListSelectionAccepted(WindowEventArgs& e);

    void onCursorMove(CursorMoveEventArgs& e) override;
    void onMouseButtonDown(MouseButtonEventArgs& e) override;
    void onClick(MouseButtonEventArgs& e) override;
    void onCaptureLost(WindowEventArgs& e) override;
    void onViewContentsChanged(WindowEventArgs& e) override;
    void onSelectionChanged(ItemViewEventArgs& e) override;

    StandardItem* d_lastItemSelected = nullptr; //!< Item last accepted by user.
    bool d_autoArm = false;                        //!< true if the box auto-arms when the cursor enters it.
    bool d_armed = false;                        //!< true when item selection has been armed.
};

}
