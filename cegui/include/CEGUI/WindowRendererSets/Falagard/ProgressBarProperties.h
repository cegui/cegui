/***********************************************************************
    filename:   FalProgressBarProperties.h
    created:    Sat Jul 2 2005
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
#ifndef _FalProgressBarProperties_h_
#define _FalProgressBarProperties_h_

#include "../../CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Namespace containing the specialised properties interface for the Progress Bar under Falagard class
*/
namespace FalagardProgressBarProperties
{
    /*!
    \brief
        Property to access the setting that controls whether the progress bar is horizontal or vertical.

        \par Usage:
            - Name: VerticalProgress
            - Format: "[text]".

        \par Where [Text] is:
            - "True" to indicate the progress bar's operates in the vertical direction.
            - "False" to indicate the progress bar's operates in the horizontal direction.
    */
    class VerticalProgress : public Property
    {
    public:
        VerticalProgress() : Property(
            "VerticalProgress",
            "Property to get/set whether the ProgressBar operates in the vertical direction.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

    /*!
    \brief
        Property to access the setting that controls the direction that progress 'grows' towards

        \par Usage:
            - Name: ReversedProgress
            - Format: "[text]".

        \par Where [Text] is:
            - "True" to indicate the progress grows towards the left or bottom edge.
            - "False" to indicate the progress grows towards the right or top edge.
    */
    class ReversedProgress : public Property
    {
    public:
        ReversedProgress() : Property(
            "ReversedProgress",
            "Property to get/set whether the ProgressBar operates in reversed direction.  Value is either \"True\" or \"False\".",
            "False")
        {}

        String	get(const PropertyReceiver* receiver) const;
        void	set(PropertyReceiver* receiver, const String& value);
    };

}

} // End of  CEGUI namespace section


#endif  // end of guard _FalProgressBarProperties_h_
