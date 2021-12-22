/***********************************************************************
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
#ifndef _CEGUIFalStateImagery_h_
#define _CEGUIFalStateImagery_h_

#include "./LayerSpecification.h"
#include "../String.h"
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class LayerSpecification;

    /*!
    \brief
        Class the encapsulates imagery for a given widget state.
    */
    class CEGUIEXPORT StateImagery
    {
    public:
        //! Container for LayerSpecifications. This container needs to be sorted for priority every time a member is changed, added or removed.
        typedef std::vector<LayerSpecification> LayerSpecificationList;

        //! Container type for LayerSpecification pointers
        typedef std::vector<LayerSpecification*> LayerSpecificationPointerList;

        /*!
        \brief
            Constructor
        */
        StateImagery():
        	d_clipToDisplay(false)
        {}

        /*!
        \brief
            Constructor

        \param name
            Name of the state
        */
        StateImagery(const String& name);

        /*!
        \brief
            Render imagery for this state.

        \param srcWindow
            Window to use when convering BaseDim values to pixels.
        */
        void render(Window& srcWindow, const ColourRect* modcols = nullptr, const Rectf* clipper = nullptr) const;

        /*!
        \brief
            Render imagery for this state.

        \param srcWindow
            Window to use when convering BaseDim values to pixels.

        \param baseRect
            Rect to use when convering BaseDim values to pixels.
        */
        void render(Window& srcWindow, const Rectf& baseRect, const ColourRect* modcols = nullptr, const Rectf* clipper = nullptr) const;

        /*!
        \brief
            Add an imagery LayerSpecification to this state.

        \param layer
            LayerSpecification to be added to this state (will be copied)
        */
        void addLayer(const LayerSpecification& layer);

        /*!
        \brief
            Sorts the LayerSpecifications after their priority. Whenever a LayerSpecification, which has been added
            to this StateImagery, is changed or an element is added to or removed from the list, this sort function
            has to be called.
        */
        void sort();

        /*!
        \brief
            Removed all LayerSpecifications from this state.
        */
        void clearLayers();

        /*!
        \brief
            Return the name of this state.

        \return
            String object holding the name of the StateImagery object.
        */
        const String& getName() const;

        /*!
        \brief
            Set the name of this state.

        \return
            String object holding the name of the StateImagery object.
        */
        void setName(const String& name);

        /*!
        \brief
            Return whether this state imagery should be clipped to the display rather than the target window.

            Clipping to the display effectively implies that the imagery should be rendered unclipped.

        \return
            - true if the imagery will be clipped to the display area.
            - false if the imagery will be clipped to the target window area.
        */
        bool isClippedToDisplay() const;

        /*!
        \brief
            Set whether this state imagery should be clipped to the display rather than the target window.

            Clipping to the display effectively implies that the imagery should be rendered unclipped.

        \param setting
            - true if the imagery should be clipped to the display area.
            - false if the imagery should be clipped to the target window area.
        */
        void setClippedToDisplay(bool setting);

        /*!
        \brief
            Writes an xml representation of this StateImagery to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

        /*!
        \brief
            Returns a const reference to the list of LayerSpecifications that are currently added to this StateImagery.

         \return
            A const reference to the vector of LayerSpecifications that are currently added to this StateImagery
        */
        const LayerSpecificationList& getLayerSpecifications() const;

        /*!
        \brief
            Returns a vector of pointers to the LayerSpecifications that are currently added to this StateImagery.
            If a LayerSpecification is added or removed from this StateImagery, then the pointers in this vector are
            not valid anymore. The function should then be called again to retrieve valid pointers.

        \note
            Whenever a pointer from this list is changed in a way that the multiset needs to be resorted, the sort
            function of this class must be called.

        \return
            A vector of pointers to the LayerSpecifications that are currently added to this StateImagery
        */
        LayerSpecificationPointerList getLayerSpecificationPointers();

    private:
        CEGUI::String               d_stateName;    //!< Name of this state.
        LayerSpecificationList      d_layers;       //!< Collection of LayerSpecification objects to be drawn for this state.
        bool                        d_clipToDisplay; //!< true if Imagery for this state should be clipped to the display instead of winodw (effectively, not clipped).
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalStateImagery_h_
