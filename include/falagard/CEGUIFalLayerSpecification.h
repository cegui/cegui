/***********************************************************************
    filename:   CEGUIFalLayerSpecification.h
    created:    Mon Jun 13 2005
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
#ifndef _CEGUIFalLayerSpecification_h_
#define _CEGUIFalLayerSpecification_h_

#include "falagard/CEGUIFalSectionSpecification.h"
#include "CEGUIWindow.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates a single layer of imagery.
    */
    class CEGUIEXPORT LayerSpecification
    {
    public:
        /*!
        \brief
            Constructor.

        \param priority
            Specifies the priority of the layer.  Layers with higher priorities will be drawn on top
            of layers with lower priorities.
        */
        LayerSpecification(uint priority);

        /*!
        \brief
            Render this layer.

        \param srcWindow
            Window to use when calculating pixel values from BaseDim values.

        \param base_z
            base level z value to use for all imagery in the layer.

        \return
            Nothing.
        */
        void render(Window& srcWindow, float base_z, const ColourRect* modcols = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Render this layer.

        \param srcWindow
            Window to use when calculating pixel values from BaseDim values.

        \param baseRect
            Rect to use when calculating pixel values from BaseDim values.

        \param base_z
            base level z value to use for all imagery in the layer.

        \return
            Nothing.
        */
        void render(Window& srcWindow, const Rect& baseRect, float base_z, const ColourRect* modcols = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Add a section specification to the layer.

            A section specification is a reference to a named ImagerySection within the WidgetLook.

        \param section
            SectionSpecification object descibing the section that should be added to this layer.

        \return
            Nothing,
        */
        void addSectionSpecification(const SectionSpecification& section);

        /*!
        \brief
            Clear all section specifications from this layer,

        \return
            Nothing.
        */
        void clearSectionSpecifications();

        /*!
        \brief
            Return the priority of this layer.

        \return
            uint value descibing the priority of this LayerSpecification.
        */
        uint getLayerPriority() const;

        // required to sort layers according to priority
        bool operator<(const LayerSpecification& other) const;

        /*!
        \brief
            Writes an xml representation of this Layer to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

    private:
        typedef std::vector<SectionSpecification> SectionList;

        SectionList d_sections;         //!< Collection of SectionSpecification objects descibing the sections to be drawn for this layer.
        uint        d_layerPriority;    //!< Priority of the layer.
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalLayerSpecification_h_
