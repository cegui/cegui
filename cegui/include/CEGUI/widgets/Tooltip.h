/***********************************************************************
    created:    21/2/2005
    author:     Paul D Turner
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
#ifndef _CEGUITooltip_h_
#define _CEGUITooltip_h_

#include "../Window.h"
#include "../WindowRenderer.h"

namespace CEGUI
{

//! \brief Base class for Tooltip window renderer objects.
class CEGUIEXPORT TooltipWindowRenderer : public WindowRenderer
{
public:

    TooltipWindowRenderer(const String& name);

    /*!
    \brief
        Return the size of the area that will be occupied by the tooltip text, given
        any current formatting options.

    \return
        Size object describing the size of the rendered tooltip text in pixels.
    */
    virtual Sizef getTextSize() const = 0;
};

/*!
\brief
    Base class for Tooltip widgets.

    The Tooltip class shows a simple pop-up window around the cursor position
    with some text information.  The tool-tip fades in when the user hovers
    with the cursor over a window which has tool-tip text set, and then fades
    out after some pre-set time.

\note
    For Tooltip to work properly, you must specify a default tool-tip widget
    type via System::setTooltip, or by setting a custom tool-tip object for
    your Window(s).  Additionally, you need to ensure that time pulses are
    properly passed to the system via System::injectTimePulse.
*/
class CEGUIEXPORT Tooltip : public Window
{
public:

    static const String WidgetTypeName; //!< Window factory name

    Tooltip(const String& type, const String& name);

    //! \brief Causes the tooltip to position itself appropriately.
    void positionSelf();

    //! \brief Causes the tooltip to resize itself appropriately.
    void sizeSelf();

    /*!
    \brief
        Return the size of the area that will be occupied by the tooltip text, given
        any current formatting options.

    \return
        Size object describing the size of the rendered tooltip text in pixels.
    */
    Sizef getTextSize() const;

    /*!
    \brief
        Return the size of the area that will be occupied by the tooltip text, given
        any current formatting options.

    \return
        Size object describing the size of the rendered tooltip text in pixels.
    */
    virtual Sizef getTextSize_impl() const { return getRenderedString().getPixelSize(this); }

protected:

    bool validateWindowRenderer(const WindowRenderer* renderer) const override;
    void onCursorEnters(CursorInputEventArgs& e) override;
    void onTextChanged(WindowEventArgs& e) override;

    bool d_inPositionSelf = false;
};

}

#endif
