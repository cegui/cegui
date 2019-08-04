/***********************************************************************
    created:    Fri Jul 8 2005
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _FalMenuItem_h_
#define _FalMenuItem_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"
#include "CEGUI/widgets/ItemEntry.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    class NamedArea;

    /*!
    \brief
        MenuItem class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States (missing states will default to '***Normal'):
            - EnabledNormal
            - EnabledHover
            - EnabledPushed
            - EnabledPushedOff
            - EnabledPopupOpen
            - DisabledNormal
            - DisabledHover
            - DisabledPushed
            - DisabledPushedOff
            - DisabledPopupOpen
            - PopupClosedIcon   - Additional state drawn when item has a pop-up attached (in closed state)
            - PopupOpenIcon     - Additional state drawn when item has a pop-up attached (in open state)

        Named Areas:
            ContentSize
            HasPopupContentSize
    */
    class COREWRSET_API FalagardMenuItem : public ItemEntryWindowRenderer
    {
    public:
        static const String TypeName;       //!< type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardMenuItem(const String& type);

        void createRenderGeometry() override;
        Sizef getItemPixelSize(void) const override;

        // overridden from WindowRenderer
        bool handleFontRenderSizeChange(const Font* const font) override;

    protected:
        const NamedArea& getContentNamedArea() const;
        bool parentIsMenubar() const;
    };

} // End of  CEGUI namespace section


#endif  // end of guard _FalMenuItem_h_

