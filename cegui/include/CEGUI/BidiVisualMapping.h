/***********************************************************************
    created:    Wed Aug 5 2009
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
#ifndef _CEGUIBidiVisualMapping_h_
#define _CEGUIBidiVisualMapping_h_

#include "CEGUI/String.h"
#include <vector>

#if defined (_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{

//! Enumeration of possible character classes, used for bi-directional text.
enum BidiCharType : int
{
    RIGHT_TO_LEFT,
    LEFT_TO_RIGHT,
    NEUTRAL
};

//! Abstract class to wrap a Bidi visual mapping of a text string.
class CEGUIEXPORT BidiVisualMapping
{
public:

    virtual ~BidiVisualMapping() = default;

    /*!
    \brief
        Gets the Bidi char type of a char.

    \param charToCheck
        The char / char32_t character code that will be checked.

    \return
        One of the BidiCharType enumerated values indicating the Bidi char type.
    */
    virtual BidiCharType getBidiCharType(const char32_t char_to_check) const = 0;

    /*!
    \brief
        Reorder a string from a logical (type order) bidi string to a
        visual (the way it displayed) string.

    \param logical
        String object to be reordered.

    \param visual
        String object containing the result reordered string.

    \param l2vMapping
        List of integers that map the pos of each char from logical string in
        the visual string.

    \param v2lMapping
        List of integers that map the pos of each char from visual string in
        the logical string.

    \return
        - true if successful.
        - false if the operation failed.
    */
    virtual bool reorderFromLogicalToVisual(const String& logical,
                                            String& visual,
                                            std::vector<int>& l2v,
                                            std::vector<int>& v2l) const = 0;

    /*!
    \brief
        Use reorderFromLogicalToVisual to update the internal visual mapping
        data and visual string representation based upon the logical string
        \a logical.

    \param logical
        String object representing the logical text order.

    \return
        - true if the update was a success.
        - false if something went wrong.
    */
    bool updateVisual(const String& logical);

    const std::vector<int>& getL2vMapping() const { return d_l2vMapping; }
    const std::vector<int>& getV2lMapping() const { return d_v2lMapping; }
    const String& getTextVisual() const { return d_textVisual; }

protected:

    std::vector<int> d_l2vMapping;
    std::vector<int> d_v2lMapping;
    String d_textVisual;
};

}

#if defined (_MSC_VER)
#   pragma warning(pop)
#endif

#endif
