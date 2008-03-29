/***********************************************************************
    filename:   CEGUIFalXMLEnumHelper.h
    created:    Mon Jul 18 2005
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
#ifndef _CEGUIFalXMLEnumHelper_h_
#define _CEGUIFalXMLEnumHelper_h_

#include "CEGUIString.h"
#include "CEGUIWindow.h"
#include "falagard/CEGUIFalEnums.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Utility helper class primarily intended for use by the falagard xml parser.
    */
    class CEGUIEXPORT FalagardXMLHelper
    {
    public:
        static VerticalFormatting stringToVertFormat(const String& str);
        static HorizontalFormatting stringToHorzFormat(const String& str);
        static VerticalAlignment stringToVertAlignment(const String& str);
        static HorizontalAlignment stringToHorzAlignment(const String& str);
        static DimensionType stringToDimensionType(const String& str);
        static VerticalTextFormatting stringToVertTextFormat(const String& str);
        static HorizontalTextFormatting stringToHorzTextFormat(const String& str);
        static FontMetricType stringToFontMetricType(const String& str);
        static DimensionOperator stringToDimensionOperator(const String& str);
        static FrameImageComponent stringToFrameImageComponent(const String& str);

        static String vertFormatToString(VerticalFormatting format);
        static String horzFormatToString(HorizontalFormatting format);
        static String vertAlignmentToString(VerticalAlignment alignment);
        static String horzAlignmentToString(HorizontalAlignment alignment);
        static String dimensionTypeToString(DimensionType dim);
        static String vertTextFormatToString(VerticalTextFormatting format);
        static String horzTextFormatToString(HorizontalTextFormatting format);
        static String fontMetricTypeToString(FontMetricType metric);
        static String dimensionOperatorToString(DimensionOperator op);
        static String frameImageComponentToString(FrameImageComponent imageComp);
    };
} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalXMLEnumHelper_h_
