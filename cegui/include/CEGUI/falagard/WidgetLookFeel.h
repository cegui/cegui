/***********************************************************************
    created:    Mon Jun 13 2005
    author:     Paul D Turner <paul@cegui.org.uk>
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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

#include "./StateImagery.h"
#include "./WidgetComponent.h"
#include "./ImagerySection.h"
#include "./PropertyInitialiser.h"
#include "./PropertyDefinition.h"
#include "./PropertyLinkDefinition.h"
#include "./EventLinkDefinition.h"
#include "./NamedArea.h"
#include "./NamedDefinitionCollator.h"
#include <map>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class that encapsulates look & feel information for a particular widget
    type.
*/
class CEGUIEXPORT WidgetLookFeel :
    public AllocatedObject<WidgetLookFeel>
{
public:
    WidgetLookFeel(const String& name, const String& inherits);
    WidgetLookFeel() {}
    WidgetLookFeel(const WidgetLookFeel& other);
    WidgetLookFeel& operator=(const WidgetLookFeel& other);

    virtual ~WidgetLookFeel();
    /*!
    \brief
        Return a const reference to the StateImagery object for the specified
        state.

    \return
        StateImagery object for the requested state.
    */
    const StateImagery& getStateImagery(const CEGUI::String& state) const;

    /*!
    \brief
        Return a const reference to the ImagerySection object with the
        specified name.

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

    \return
        Nothing.
    */
    void addImagerySection(const ImagerySection& section);
    void renameImagerySection(const String& oldName, const String& newName);

    /*!
    \brief
        Add a WidgetComponent to the WidgetLookFeel.

    \param widget
        WidgetComponent object to be added.

    \return
        Nothing.
    */
    void addWidgetComponent(const WidgetComponent& widget);

    /*!
    \brief
        Add a state specification (StateImagery object) to the WidgetLookFeel.

    \param section
        StateImagery object to be added.

    \return
        Nothing.
    */
    void addStateSpecification(const StateImagery& state);

    /*!
    \brief
        Add a property initialiser to the WidgetLookFeel.

    \param initialiser
        PropertyInitialiser object to be added.

    \return
        Nothing.
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
        Initialise the given window using PropertyInitialsers and component
        widgets specified for this WidgetLookFeel.

    \param widget
        Window based object to be initialised.

    \return
        Nothing.
    */
    void initialiseWidget(Window& widget) const;

    /*!
    \brief
        Clean up the given window from all properties and component widgets
        created by this WidgetLookFeel

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

    void renameNamedArea(const String& oldName, const String& newName);
    /*!
    \brief
        return whether a NamedArea object with the specified name exists for
        this WidgetLookFeel.

    \param name
        String holding the name of the NamedArea to check for.

    \return
        - true if a named area with the requested name is defined for this
          WidgetLookFeel.
        - false if no such named area is defined for this WidgetLookFeel.
    */
    bool isNamedAreaDefined(const String& name) const;

    /*!
    \brief
        Layout the child widgets defined for this WidgetLookFeel which are
        attached to the given window.

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
    void addPropertyDefinition(PropertyDefinitionBase* propdef);

    /*!
    \brief
        Adds a property link definition to the WidgetLookFeel.

    \param propdef
        PropertyLinkDefinition object to be added.

    \return
        Nothing.
    */
    void addPropertyLinkDefinition(PropertyDefinitionBase* propdef);

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
        Add the name of an animation that is associated with the
        WidgetLookFeel.

    \param anim_name
        Reference to a String object that contains the name of the animation
        to be associated with this WidgetLookFeel.
    */
    void addAnimationName(const String& anim_name);

    //! adds an event link definition to the WidgetLookFeel.
    void addEventLinkDefinition(const EventLinkDefinition& evtdef); 

    //! clear all defined event link definitions from the WidgetLookFeel.
    void clearEventLinkDefinitions();

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
        Takes the name of a property and returns a pointer to the last
        PropertyInitialiser for this property or 0 if the is no
        PropertyInitialiser for this property in the WidgetLookFeel

    \param propertyName
        The name of the property to look for.
    */
    const PropertyInitialiser* findPropertyInitialiser(const String& propertyName) const;

    /*!
    \brief
        Takes the name for a widget component and returns a pointer to
        it if it exists or 0 if it does'nt.

    \param name
        The name of the Child component to look for.
    */
    const WidgetComponent* findWidgetComponent(const String& name) const;

    /** Typedefs for property related lists. */
    typedef std::vector<PropertyInitialiser
        CEGUI_VECTOR_ALLOC(PropertyInitialiser)> PropertyList;
    typedef std::vector<PropertyDefinitionBase*
        CEGUI_VECTOR_ALLOC(PropertyDefinitionBase*)> PropertyDefinitionList;
    typedef std::vector<PropertyDefinitionBase*
        CEGUI_VECTOR_ALLOC(PropertyDefinitionBase*)> PropertyLinkDefinitionList;

    /** Obtains list of properties definitions.
     * @access public
     * @return CEGUI::WidgetLookFeel::PropertyDefinitionList List of properties
     * definitions
     */
    const PropertyDefinitionList& getPropertyDefinitions() const
    {
        return d_propertyDefinitions;
    }

    /** Obtains list of properties link definitions.
     * @access public
     * @return CEGUI::WidgetLookFeel::PropertyLinkDefinitionList List of
     * properties link definitions
     */
    const PropertyLinkDefinitionList& getPropertyLinkDefinitions() const
    {
        return d_propertyLinkDefinitions;
    }

    /** Obtains list of properties.
     * @access public
     * @return CEGUI::WidgetLookFeel::PropertyList List of properties
     */
    const PropertyList& getProperties() const
    {
        return d_properties;
    }

    //! perform any processing required due to the given font having changed.
    bool handleFontRenderSizeChange(Window& window, const Font* font) const;

private:
    typedef std::map<String, StateImagery, StringFastLessCompare
        CEGUI_MAP_ALLOC(String, StateImagery)> StateList;
    typedef std::map<String, ImagerySection, StringFastLessCompare
        CEGUI_MAP_ALLOC(String, ImagerySection)> ImageryList;
    typedef std::map<String, NamedArea, StringFastLessCompare
        CEGUI_MAP_ALLOC(String, NamedArea)> NamedAreaList;
    typedef std::vector<WidgetComponent
        CEGUI_VECTOR_ALLOC(WidgetComponent)> WidgetList;
    typedef std::vector<String
        CEGUI_VECTOR_ALLOC(String)> AnimationList;
    typedef std::multimap<Window*, AnimationInstance*
        /*CEGUI_MULTIMAP_ALLOC(Window*, AnimationInstance*)*/> AnimationInstanceMap;
    typedef std::vector<EventLinkDefinition
        CEGUI_VECTOR_ALLOC(EventLinkDefinition)> EventLinkDefinitionList;

    //! Name of this WidgetLookFeel.
    CEGUI::String d_lookName;
    //! Name of a WidgetLookFeel inherited by this WidgetLookFeel.
    CEGUI::String d_inheritedLookName;
    //! Collection of ImagerySection objects.
    ImageryList d_imagerySections;
    //! Collection of WidgetComponent objects.
    WidgetList d_childWidgets;
    //! Collection of StateImagery objects.
    StateList d_stateImagery;
    //! Collection of PropertyInitialser objects.
    PropertyList d_properties;
    //! Collection of NamedArea objects.
    NamedAreaList d_namedAreas;
    //! Collection of PropertyDefinition objects.
    mutable PropertyDefinitionList  d_propertyDefinitions;
    //! Collection of PropertyLinkDefinition objects.
    mutable PropertyLinkDefinitionList d_propertyLinkDefinitions;
    //! Collection of animation names associated with this WidgetLookFeel.
    AnimationList d_animations;
    //! map of windows and their associated animation instances
    mutable AnimationInstanceMap d_animationInstances;
    //! Collection of EventLinkDefinition objects.
    EventLinkDefinitionList d_eventLinkDefinitions;

    // these are container types used when composing final collections of
    // objects that come via inheritence.
    typedef NamedDefinitionCollator<String, const WidgetComponent*> WidgetComponentCollator;
    typedef NamedDefinitionCollator<String, PropertyDefinitionBase*> PropertyDefinitionCollator;
    typedef NamedDefinitionCollator<String, PropertyDefinitionBase*> PropertyLinkDefinitionCollator;
    typedef NamedDefinitionCollator<String, const PropertyInitialiser*> PropertyInitialiserCollator;
    typedef NamedDefinitionCollator<String, const EventLinkDefinition*> EventLinkDefinitionCollator;
    typedef std::set<String, StringFastLessCompare
        CEGUI_SET_ALLOC(String)> AnimationNameSet;

    // functions to populate containers with collections of objects that we
    // gain through inheritence.
    void appendChildWidgetComponents(WidgetComponentCollator& col, bool inherits = true) const;
    void appendPropertyDefinitions(PropertyDefinitionCollator& col, bool inherits = true) const;
    void appendPropertyLinkDefinitions(PropertyLinkDefinitionCollator& col, bool inherits = true) const;
    void appendPropertyInitialisers(PropertyInitialiserCollator& col, bool inherits = true) const;
    void appendEventLinkDefinitions(EventLinkDefinitionCollator& col, bool inherits = true) const;
    void appendAnimationNames(AnimationNameSet& set, bool inherits = true) const;

    void swap(WidgetLookFeel& other);

public:
    /*************************************************************************
        Iterator stuff
    *************************************************************************/
    typedef std::set<String, StringFastLessCompare
            CEGUI_SET_ALLOC(String)> StringSet;

    typedef ConstMapIterator<StateList> StateIterator;
    typedef ConstMapIterator<ImageryList> ImageryIterator;
    typedef ConstMapIterator<NamedAreaList> NamedAreaIterator;
    typedef ConstVectorIterator<WidgetComponentCollator> WidgetComponentIterator;
    typedef ConstVectorIterator<PropertyDefinitionCollator> PropertyDefinitionIterator;
    typedef ConstVectorIterator<PropertyLinkDefinitionCollator> PropertyLinkDefinitionIterator;
    typedef ConstVectorIterator<PropertyInitialiserCollator> PropertyInitialiserIterator;
    typedef ConstVectorIterator<EventLinkDefinitionCollator> EventLinkDefinitionIterator;
    typedef ConstVectorIterator<AnimationNameSet> AnimationNameIterator;

    StringSet getStateNames(bool inherits = false) const;
    StringSet getImageryNames(bool inherits = false) const;
    StringSet getNamedAreaNames(bool inherits = false) const;

    StringSet getWidgetNames(bool inherits = false) const;
    StringSet getPropertyDefinitionNames(bool inherits = false) const;
    StringSet getPropertyLinkDefinitionNames(bool inherits = false) const;
    StringSet getPropertyInitialiserNames(bool inherits = false) const;
    StringSet getEventLinkDefinitionNames(bool inherits = false) const;
    StringSet getAnimationNames(bool inherits = false) const;

    StateIterator getStateIterator(bool inherits = false) const;
    ImageryIterator getImageryIterator(bool inherits = false) const;
    NamedAreaIterator getNamedAreaIterator(bool inherits = false) const;
    WidgetComponentIterator getWidgetComponentIterator(bool inherits = false) const;
    PropertyDefinitionIterator getPropertyDefinitionIterator(bool inherits = false) const;
    PropertyLinkDefinitionIterator getPropertyLinkDefinitionIterator(bool inherits = false) const;
    PropertyInitialiserIterator getPropertyInitialiserIterator(bool inherits = false) const;
    EventLinkDefinitionIterator getEventLinkDefinitionIterator(bool inherits = false) const;
    AnimationNameIterator getAnimationNameIterator(bool inherits = false) const;
};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalWidgetLookFeel_h_

