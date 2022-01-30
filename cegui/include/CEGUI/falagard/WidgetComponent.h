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
#ifndef _CEGUIFalWidgetComponent_h_
#define _CEGUIFalWidgetComponent_h_

#include "CEGUI/falagard/Dimensions.h"
#include "CEGUI/falagard/PropertyInitialiser.h"
#include "CEGUI/falagard/EventAction.h"
#include <vector>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates information regarding a child-widget that is used in a WidgetLookFeel.
    */
    class CEGUIEXPORT WidgetComponent
    {
    public:
        WidgetComponent() : d_autoWindow(false), d_vertAlign(VerticalAlignment::Top), d_horzAlign(HorizontalAlignment::Left) {}
        WidgetComponent(const String& targetType,
                        const String& suffix,
                        const String& renderer,
                        bool autoWindow);

        //! Default value for the HorzAlignment elements of the WidgetComponent
        static const HorizontalAlignment HorizontalAlignmentDefault;
        //! Default value for the VertAlignment elements of the WidgetComponent
        static const VerticalAlignment VerticalAlignmentDefault;

        //! List of PropertyInitialisers of this WidgetComponent
        typedef std::vector<PropertyInitialiser> PropertyInitialiserList;
        //! List of EventActions of this WidgetComponent
        typedef std::vector<EventAction> EventActionList;

        /*!
        \brief
            Create an instance of this widget component adding it as a child to
            the given Window.
        */
        void create(Window& parent) const;

        /*!
        \brief
            Cleanup from the given parent widget, the instance of the child
            created for this WidgetComponent.
        */
        void cleanup(Window& parent) const;

        const ComponentArea& getComponentArea() const;
        void setComponentArea(const ComponentArea& area);

        const String& getTargetType() const;
        void setTargetType(const String& type);

        const String& getWidgetName() const;
        void setWidgetName(const String& name);

        const String& getWindowRendererType() const;
        void setWindowRendererType(const String& type);

        VerticalAlignment getVerticalWidgetAlignment() const;
        void setVerticalWidgetAlignment(VerticalAlignment alignment);

        HorizontalAlignment getHorizontalWidgetAlignment() const;
        void setHorizontalWidgetAlignment(HorizontalAlignment alignment);

        void addPropertyInitialiser(const PropertyInitialiser& initialiser);
        void removePropertyInitialiser(const String& name);
        void clearPropertyInitialisers();

        void setAutoWindow(bool is_auto);
        bool isAutoWindow() const;

        void addEventAction(const EventAction& event_action);
        void clearEventActions();

        bool layout(const Window& owner) const;

        /*!
        \brief
            Writes an xml representation of this WidgetComponent to \a out_stream.

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

        //! perform any processing required due to the given font having changed.
        bool handleFontRenderSizeChange(Window& window, const Font* font) const;

        /*!
        \brief
            Returns the collection of PropertyInitialisers of this WidgetComponent

        \param propertyName
            A list of PropertyInitialiser instances owned by this WidgetComponent.
        */
        const PropertyInitialiserList& getPropertyInitialisers() const;

        /*!
        \brief
            Returns the collection of EventActionList of this WidgetComponent

        \return
            A list of EventAction instances owned by this WidgetComponent.
        */
        const EventActionList& getEventActions() const;

    private:
        ComponentArea   d_area;              //!< Destination area for the widget (relative to it's parent).
        String   d_targetType;               //!< The target type (e.g. falagard mapping or alias) mapping of the widget to be created.
        String   d_name;                     //!< Name to create this widget with.
        String   d_rendererType;             //!< Name of the window renderer type to assign to the widget.
        bool     d_autoWindow;               //!< Specifies whether to mark component as an auto-window.
        VerticalAlignment    d_vertAlign;    //!< Vertical alignment to be used for this widget.
        HorizontalAlignment  d_horzAlign;    //!< Horizontal alignment to be used for this widget.
        PropertyInitialiserList  d_propertyInitialisers;//!< Collection of PropertyInitialisers to be applied the the widget upon creation.
        EventActionList d_eventActions;      //!< EventActions added to the WidgetComponent
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalWidgetComponent_h_
