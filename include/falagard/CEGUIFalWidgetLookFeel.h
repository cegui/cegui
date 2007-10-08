/***********************************************************************
    filename:   CEGUIFalWidgetLookFeel.h
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
#ifndef _CEGUIFalWidgetLookFeel_h_
#define _CEGUIFalWidgetLookFeel_h_

#include "falagard/CEGUIFalStateImagery.h"
#include "falagard/CEGUIFalWidgetComponent.h"
#include "falagard/CEGUIFalImagerySection.h"
#include "falagard/CEGUIFalPropertyInitialiser.h"
#include "falagard/CEGUIFalPropertyDefinition.h"
#include "falagard/CEGUIFalPropertyLinkDefinition.h"
#include "falagard/CEGUIFalNamedArea.h"
#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates look & feel information for a particular widget type.
    */
    class CEGUIEXPORT WidgetLookFeel
    {
    public:
        WidgetLookFeel(const String& name);
        WidgetLookFeel() {}

        /*!
        \brief
            Return a const reference to the StateImagery object for the specified state.

        \return
            StateImagery object for the requested state.
        */
        const StateImagery& getStateImagery(const CEGUI::String& state) const;

        /*!
        \brief
            Return a const reference to the ImagerySection object with the specified name.

        \return
            ImagerySection object with the specified name.
        */
        const ImagerySection& getImagerySection(const CEGUI::String& section) const;

        /*!
        \brief
            Return the name of the widget look.

        \return
            String object holding the name of the WidgetLookFeel.
        */
        const String& getName() const;

        /*!
        \brief
            Add an ImagerySection to the WidgetLookFeel.

        \param section
            ImagerySection object to be added.

        \return Nothing.
        */
        void addImagerySection(const ImagerySection& section);

        /*!
        \brief
            Add a WidgetComponent to the WidgetLookFeel.

        \param widget
            WidgetComponent object to be added.

        \return Nothing.
        */
        void addWidgetComponent(const WidgetComponent& widget);

        /*!
        \brief
            Add a state specification (StateImagery object) to the WidgetLookFeel.

        \param section
            StateImagery object to be added.

        \return Nothing.
        */
        void addStateSpecification(const StateImagery& state);

        /*!
        \brief
            Add a property initialiser to the WidgetLookFeel.

        \param initialiser
            PropertyInitialiser object to be added.

        \return Nothing.
        */
        void addPropertyInitialiser(const PropertyInitialiser& initialiser);

        /*!
        \brief
            Clear all ImagerySections from the WidgetLookFeel.

        \return
            Nothing.
        */
        void clearImagerySections();

        /*!
        \brief
            Clear all WidgetComponents from the WidgetLookFeel.

        \return
            Nothing.
        */
        void clearWidgetComponents();

        /*!
        \brief
            Clear all StateImagery objects from the WidgetLookFeel.

        \return
            Nothing.
        */
        void clearStateSpecifications();

        /*!
        \brief
            Clear all PropertyInitialiser objects from the WidgetLookFeel.

        \return
            Nothing.
        */
        void clearPropertyInitialisers();

        /*!
        \brief
            Initialise the given window using PropertyInitialsers and component widgets
            specified for this WidgetLookFeel.

        \param widget
            Window based object to be initialised.

        \return
            Nothing.
        */
        void initialiseWidget(Window& widget) const;

        /*!
        \brief
            Clean up the given window from all properties and component widgets created by this WidgetLookFeel

        \param widget
            Window based object to be cleaned up.

        \return
            Nothing.
        */
        void cleanUpWidget(Window& widget) const;

        /*!
        \brief
            Return whether imagery is defined for the given state.

        \param state
            String object containing name of state to look for.

        \return
            - true if imagery exists for the specified state,
            - false if no imagery exists for the specified state.
        */
        bool isStateImageryPresent(const String& state) const;

        /*!
        \brief
            Adds a named area to the WidgetLookFeel.

        \param area
            NamedArea to be added.

        \return
            Nothing.
        */
        void addNamedArea(const NamedArea& area);

        /*!
        \brief
            Clear all defined named areas from the WidgetLookFeel

        \return
            Nothing.
        */
        void clearNamedAreas();

        /*!
        \brief
            Return the NamedArea with the specified name.

        \param name
            String object holding the name of the NamedArea to be returned.

        \return
            The requested NamedArea object.
        */
        const NamedArea& getNamedArea(const String& name) const;

        /*!
        \brief
            return whether a NamedArea object with the specified name exists for this WidgetLookFeel.

        \param name
            String holding the name of the NamedArea to check for.

        \return
            - true if a named area with the requested name is defined for this WidgetLookFeel.
            - false if no such named area is defined for this WidgetLookFeel.
        */
        bool isNamedAreaDefined(const String& name) const;

        /*!
        \brief
            Layout the child widgets defined for this WidgetLookFeel which are attached to the given window.

        \param owner
            Window object that has the child widgets that require laying out.

        \return
            Nothing.
        */
        void layoutChildWidgets(const Window& owner) const;

        /*!
        \brief
            Adds a property definition to the WidgetLookFeel.

        \param propdef
            PropertyDefinition object to be added.

        \return
            Nothing.
        */
        void addPropertyDefinition(const PropertyDefinition& propdef);

        /*!
        \brief
            Adds a property link definition to the WidgetLookFeel.

        \param propdef
            PropertyLinkDefinition object to be added.

        \return
            Nothing.
        */
        void addPropertyLinkDefinition(const PropertyLinkDefinition& propdef);

        /*!
        \brief
            Clear all defined property definitions from the WidgetLookFeel

        \return
            Nothing.
        */
        void clearPropertyDefinitions();

        /*!
        \brief
            Clear all defined property link definitions from the WidgetLookFeel

        \return
            Nothing.
        */
        void clearPropertyLinkDefinitions();

        /*!
        \brief
            Writes an xml representation of this WidgetLookFeel to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

        /*!
        \brief
            Uses the WindowManager to rename the child windows that are
            created for this WidgetLookFeel.

        \param widget
            The target Window containing the child windows that are to be
            renamed.

        \param newBaseName
            String object holding the new base name that will be used when
            constructing new names for the child windows.
        */
        void renameChildren(const Window& widget, const String& newBaseName) const;

        /*!
        \brief
            Takes the name of a property and returns a pointer to the last
            PropertyInitialiser for this property or 0 if the is no
            PropertyInitialiser for this property in the WidgetLookFeel

        \param propertyName
            The name of the property to look for.
        */
        const PropertyInitialiser* findPropertyInitialiser(const String& propertyName) const;

        /*!
        \brief
            Takes the namesuffix for a widget component and returns a pointer to
            it if it exists or 0 if it does'nt.

        \param nameSuffix
            The name suffix of the Child component to look for.
        */
        const WidgetComponent* findWidgetComponent(const String& nameSuffix) const;

        /** Typedefs for property related lists. */
        typedef std::vector<PropertyInitialiser>  PropertyList;
        typedef std::vector<PropertyDefinition>   PropertyDefinitionList;
        typedef std::vector<PropertyLinkDefinition> PropertyLinkDefinitionList;

        /** Obtains list of properties definitions.
         * @access public 
         * @return CEGUI::WidgetLookFeel::PropertyDefinitionList List of properties definitions
         */
        const PropertyDefinitionList& getPropertyDefinitions() const { return d_propertyDefinitions; } 

        /** Obtains list of properties link definitions.
         * @access public 
         * @return CEGUI::WidgetLookFeel::PropertyLinkDefinitionList List of properties link definitions
         */
        const PropertyLinkDefinitionList& getPropertyLinkDefinitions() const { return d_propertyLinkDefinitions; } 

        /** Obtains list of properties.
         * @access public 
         * @return CEGUI::WidgetLookFeel::PropertyList List of properties
         */
        const PropertyList& getProperties() const { return d_properties; } 

    private:
        typedef std::map<String, StateImagery, String::FastLessCompare>    StateList;
        typedef std::map<String, ImagerySection, String::FastLessCompare>  ImageryList;
        typedef std::map<String, NamedArea, String::FastLessCompare>       NamedAreaList;
        typedef std::vector<WidgetComponent>      WidgetList;

        CEGUI::String   d_lookName;         //!< Name of this WidgetLookFeel.
        ImageryList     d_imagerySections;  //!< Collection of ImagerySection objects.
        WidgetList      d_childWidgets;     //!< Collection of WidgetComponent objects.
        StateList       d_stateImagery;     //!< Collection of StateImagery objects.
        PropertyList    d_properties;       //!< Collection of PropertyInitialser objects.
        NamedAreaList   d_namedAreas;       //!< Collection of NamedArea objects.
        mutable PropertyDefinitionList  d_propertyDefinitions;  //!< Collection of PropertyDefinition objects.
        mutable PropertyLinkDefinitionList d_propertyLinkDefinitions;  //!< Collection of PropertyLinkDefinition objects.
    };


} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalWidgetLookFeel_h_
