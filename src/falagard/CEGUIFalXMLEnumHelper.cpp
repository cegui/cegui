/************************************************************************
    filename:   CEGUIFalXMLEnumHelper.cpp
    created:    Mon Jul 18 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.
 
    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/
#include "falagard/CEGUIFalXMLEnumHelper.h"

// Start of CEGUI namespace section
namespace CEGUI
{
    VerticalFormatting FalagardXMLHelper::stringToVertFormat(const String& str)
    {
        if (str == "CentreAligned")
        {
            return VF_CENTRE_ALIGNED;
        }
        else if (str == "BottomAligned")
        {
            return VF_BOTTOM_ALIGNED;
        }
        else if (str == "Tiled")
        {
            return VF_TILED;
        }
        else if (str == "Stretched")
        {
            return VF_STRETCHED;
        }
        else
        {
            return VF_TOP_ALIGNED;
        }
    }

    HorizontalFormatting FalagardXMLHelper::stringToHorzFormat(const String& str)
    {
        if (str == "CentreAligned")
        {
            return HF_CENTRE_ALIGNED;
        }
        else if (str == "RightAligned")
        {
            return HF_RIGHT_ALIGNED;
        }
        else if (str == "Tiled")
        {
            return HF_TILED;
        }
        else if (str == "Stretched")
        {
            return HF_STRETCHED;
        }
        else
        {
            return HF_LEFT_ALIGNED;
        }
    }

    VerticalAlignment FalagardXMLHelper::stringToVertAlignment(const String& str)
    {
        if (str == "CentreAligned")
        {
            return VA_CENTRE;
        }
        else if (str == "BottomAligned")
        {
            return VA_BOTTOM;
        }
        else
        {
            return VA_TOP;
        }
    }

    HorizontalAlignment FalagardXMLHelper::stringToHorzAlignment(const String& str)
    {
        if (str == "CentreAligned")
        {
            return HA_CENTRE;
        }
        else if (str == "RightAligned")
        {
            return HA_RIGHT;
        }
        else
        {
            return HA_LEFT;
        }
    }

    DimensionType FalagardXMLHelper::stringToDimensionType(const String& str)
    {
        if (str == "LeftEdge")
        {
            return DT_LEFT_EDGE;
        }
        else if (str == "XPosition")
        {
            return DT_X_POSITION;
        }
        else if (str == "TopEdge")
        {
            return DT_TOP_EDGE;
        }
        else if (str == "YPosition")
        {
            return DT_Y_POSITION;
        }
        else if (str == "RightEdge")
        {
            return DT_RIGHT_EDGE;
        }
        else if (str == "BottomEdge")
        {
            return DT_BOTTOM_EDGE;
        }
        else if (str == "Width")
        {
            return DT_WIDTH;
        }
        else if (str == "Height")
        {
            return DT_HEIGHT;
        }
        else if (str == "XOffset")
        {
            return DT_X_OFFSET;
        }
        else if (str == "YOffset")
        {
            return DT_Y_OFFSET;
        }
        else
        {
            return DT_INVALID;
        }
    }

    VerticalTextFormatting FalagardXMLHelper::stringToVertTextFormat(const String& str)
    {
        if (str == "CentreAligned")
        {
            return VTF_CENTRE_ALIGNED;
        }
        else if (str == "BottomAligned")
        {
            return VTF_BOTTOM_ALIGNED;
        }
        else
        {
            return VTF_TOP_ALIGNED;
        }
    }

    HorizontalTextFormatting FalagardXMLHelper::stringToHorzTextFormat(const String& str)
    {
        if (str == "CentreAligned")
        {
            return HTF_CENTRE_ALIGNED;
        }
        else if (str == "RightAligned")
        {
            return HTF_RIGHT_ALIGNED;
        }
        else if (str == "Justified")
        {
            return HTF_JUSTIFIED;
        }
        else if (str == "WordWrapLeftAligned")
        {
            return HTF_WORDWRAP_LEFT_ALIGNED;
        }
        else if (str == "WordWrapCentreAligned")
        {
            return HTF_WORDWRAP_CENTRE_ALIGNED;
        }
        else if (str == "WordWrapRightAligned")
        {
            return HTF_WORDWRAP_RIGHT_ALIGNED;
        }
        else if (str == "WordWrapJustified")
        {
            return HTF_WORDWRAP_JUSTIFIED;
        }
        else
        {
            return HTF_LEFT_ALIGNED;
        }
    }

    FontMetricType FalagardXMLHelper::stringToFontMetricType(const String& str)
    {
        if (str == "LineSpacing")
        {
            return FMT_LINE_SPACING;
        }
        else if (str == "Baseline")
        {
            return FMT_BASELINE;
        }
        else
        {
            return FMT_HORZ_EXTENT;
        }
    }

    DimensionOperator FalagardXMLHelper::stringToDimensionOperator(const String& str)
    {
        if (str == "Add")
        {
            return DOP_ADD;
        }
        else if (str == "Subtract")
        {
            return DOP_SUBTRACT;
        }
        else if (str == "Multiply")
        {
            return DOP_MULTIPLY;
        }
        else if (str == "Divide")
        {
            return DOP_DIVIDE;
        }
        else
        {
            return DOP_NOOP;
        }
    }

    FrameImageComponent FalagardXMLHelper::stringToFrameImageComponent(const String& str)
    {
        if (str == "TopLeftCorner")
        {
            return FIC_TOP_LEFT_CORNER;
        }
        if (str == "TopRightCorner")
        {
            return FIC_TOP_RIGHT_CORNER;
        }
        if (str == "BottomLeftCorner")
        {
            return FIC_BOTTOM_LEFT_CORNER;
        }
        if (str == "BottomRightCorner")
        {
            return FIC_BOTTOM_RIGHT_CORNER;
        }
        if (str == "LeftEdge")
        {
            return FIC_LEFT_EDGE;
        }
        if (str == "RightEdge")
        {
            return FIC_RIGHT_EDGE;
        }
        if (str == "TopEdge")
        {
            return FIC_TOP_EDGE;
        }
        if (str == "BottomEdge")
        {
            return FIC_BOTTOM_EDGE;
        }
        else
        {
            return FIC_BACKGROUND;
        }
    }

} // End of  CEGUI namespace section
