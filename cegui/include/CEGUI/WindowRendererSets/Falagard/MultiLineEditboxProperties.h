/***********************************************************************
    filename:   FalMultiLineEditboxProperties.h
    created:    Wed Jul 29 2009
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2009 Paul D Turner & The CEGUI Development Team
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
#ifndef _FalMultiLineEditboxProperties_h_
#define _FalMultiLineEditboxProperties_h_

#include "../../CEGUIProperty.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! Namespace containing the specialised Falagard properties for MultiLineEditbox.
namespace FalagardMultiLineEditboxProperties
{
/*!
\brief
    Property to access the setting that controls whether the caret will blink.

    \par Usage:
        - Name: BlinkCaret
        - Format: "[text]".

    \par Where [Text] is:
        - "True" to indicate the caret should blink when he editbox is active.
        - "False" to indicate the caret should not blink.
*/
class BlinkCaret : public Property
{
public:
    BlinkCaret() : Property(
        "BlinkCaret",
        "Property to get/set whether the Editbox caret should blink.  "
        "Value is either \"True\" or \"False\".",
        "False")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

/*!
\brief
    Property to access the setting that controls the speed at which the caret
    blinks when the caret blink option is enabled.

    \par Usage:
        - Name: BlinkCaretTimeout
        - Format: float (floating point value representing seconds).
*/
class BlinkCaretTimeout : public Property
{
public:
    BlinkCaretTimeout() : Property(
        "BlinkCaretTimeout",
        "Property to get/set the caret blink timeout / speed.  "
        "Value is a float value indicating the timeout in seconds.",
        "0.66")
    {}

    String  get(const PropertyReceiver* receiver) const;
    void    set(PropertyReceiver* receiver, const String& value);
};

} // End of FalagardMultiLineEditboxProperties namespace section


} // End of  CEGUI namespace section

#endif  // end of guard _FalMultiLineEditboxProperties_h_
