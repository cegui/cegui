/***********************************************************************
    created:    27/3/2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)

    purpose:    Interface to base class for PopupMenu widget
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
#include "MenuBase.h"

namespace CEGUI
{

//! Base class for popup menus
class CEGUIEXPORT PopupMenu : public MenuBase
{
public:

    static const String EventNamespace; //!< Namespace for global events
    static const String WidgetTypeName; //!< Window factory name

    PopupMenu(const String& type, const String& name);

    /*!
    \brief
        Get the fade in time for this popup menu.

    \return
        The time in seconds that it takes for the popup to fade in.
        0 if fading is disabled.
    */
    float getFadeInTime() const { return d_fadeInTime; }

    /*!
    \brief
        Set the fade in time for this popup menu.

    \param fadetime
        The time in seconds that it takes for the popup to fade in.
        If this parameter is zero, fading is disabled.
    */
    void setFadeInTime(float fadetime) { d_fadeInTime = fadetime; }

    /*!
    \brief
        Get the fade out time for this popup menu.

    \return
        The time in seconds that it takes for the popup to fade out.
        0 if fading is disabled.
    */
    float getFadeOutTime() const { return d_fadeOutTime; }

    /*!
    \brief
        Set the fade out time for this popup menu.

    \param fadetime
        The time in seconds that it takes for the popup to fade out.
        If this parameter is zero, fading is disabled.
    */
    void setFadeOutTime(float fadetime) { d_fadeOutTime = fadetime; }

    //! Find out if this popup menu is open or closed;
    bool isPopupMenuOpen() const { return d_isOpen; }

    /*!
    \brief
        Tells the popup menu to open.

    \param notify
        true if the parent menu item (if any) is to handle the opening. false if not.
    */
    void openPopupMenu(bool notify = true);

    /*!
    \brief
        Tells the popup menu to close.

    \param notify
        true if the parent menu item (if any) is to handle the closing. false if not.
    */
    void closePopupMenu(bool notify = true);

protected:

    void updateSelf(float elapsed) override;
    void layoutItemWidgets() override;
    Sizef getContentSize() const override;

    void onAlphaChanged(WindowEventArgs& e) override;
    void onDestructionStarted(WindowEventArgs& e) override;
    void onShown(WindowEventArgs& e) override;
    void onHidden(WindowEventArgs& e) override;

    float d_origAlpha = 1.f;   //!< The original alpha of this window.
    float d_fadeElapsed = 0.f; //!< The time in seconds this popup menu has been fading.
    float d_fadeOutTime = 0.f; //!< The time in seconds it takes for this popup menu to fade out.
    float d_fadeInTime = 0.f;  //!< The time in seconds it takes for this popup menu to fade in.
    bool d_fading = false;     //!< true if this popup menu is fading in/out. false if not
    bool d_fadingOut = false;  //!< true if this popup menu is fading out. false if fading in.
    bool d_isOpen = false;     //!< true if this popup menu is open. false if not.

private:

    void addPopupMenuProperties();
};

}
