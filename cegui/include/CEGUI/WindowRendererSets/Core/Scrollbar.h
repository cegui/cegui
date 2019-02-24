/***********************************************************************
    created:    Mon Jul 4 2005
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
#ifndef _FalScrollbar_h_
#define _FalScrollbar_h_

#include "CEGUI/WindowRendererSets/Core/Module.h"
#include "CEGUI/widgets/Scrollbar.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Scrollbar class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States:
            - Enabled
            - Disabled

        Named Areas:
            - ThumbTrackArea

        Child Widgets:
            Thumb based widget with name suffix "__auto_thumb__"
            PushButton based widget with name suffix "__auto_incbtn__"
            PushButton based widget with name suffix "__auto_decbtn__"

        Property initialiser definitions:
            - VerticalScrollbar - boolean property.
            Indicates whether this scrollbar will operate in the vertical or
            horizontal direction.  Default is for horizontal.  Optional.
    */
    class COREWRSET_API FalagardScrollbar : public ScrollbarWindowRenderer
    {
    public:
        static const String TypeName;     //! type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardScrollbar(const String& type);

        bool isVertical() const;
        void setVertical(bool setting);

        void createRenderGeometry() override;
        void performChildWindowLayout() override;

    protected:
        // data members
        bool    d_vertical;     //!< True if slider operates in vertical direction.

        // overridden from Scrollbar base class.
        void updateThumb(void) override;
        float getValueFromThumb(void) const override;
        float getAdjustDirectionFromPoint(const glm::vec2& pt) const override;
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalScrollbar_h_
