/************************************************************************
    filename:   CEGUIFalStateImagery.h
    created:    Mon Jun 13 2005
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
#ifndef _CEGUIFalStateImagery_h_
#define _CEGUIFalStateImagery_h_

#include "falagard/CEGUIFalLayerSpecification.h"
#include "CEGUIWindow.h"
#include <set>

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class the encapsulates imagery for a given widget state.
    */
    class StateImagery
    {
    public:
        /*!
        \brief
            Constructor
        */
        StateImagery() {}

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

        \return
            Nothing.
        */
        void render(Window& srcWindow, const Rect* clipper = 0) const;

        /*!
        \brief
            Render imagery for this state.

        \param srcWindow
            Window to use when convering BaseDim values to pixels.

        \param baseRect
            Rect to use when convering BaseDim values to pixels.

        \return
            Nothing.
        */
        void render(Window& srcWindow, const Rect& baseRect, const Rect* clipper = 0) const;

        /*!
        \brief
            Add an imagery LayerSpecification to this state.

        \param layer
            LayerSpecification to be added to this state (will be copied)

        \return
            Nothing.
        */
        void addLayer(const LayerSpecification& layer);

        /*!
        \brief
            Removed all LayerSpecifications from this state.

        \return
            Nothing.
        */
        void clearLayers();

        /*!
        \brief
            Return the name of this state.

        \return
            String object holding the name of the StateImagery object.
        */
        const String& getName() const;

    private:
        typedef std::multiset<LayerSpecification> LayersList;

        CEGUI::String   d_stateName;    //!< Name of this state.
        LayersList      d_layers;       //!< Collection of LayerSpecification objects to be drawn for this state.
    };

} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalStateImagery_h_
