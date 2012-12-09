/***********************************************************************
    filename:   CEGUIFalWidgetComponent.h
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

#include "CEGUIFalDimensions.h"
#include "CEGUIFalPropertyInitialiser.h"
#include "../CEGUIWindow.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Class that encapsulates information regarding a sub-widget required for a widget.

    \todo
        This is not finished in the slightest!  There will be many changes here...
    */
    class CEGUIEXPORT WidgetComponent
    {
    public:
        WidgetComponent() {}
        WidgetComponent(const String& type, const String& look, const String& suffix, const String& renderer);

        /*!
        \brief
            Create an instance of this widget component adding it as a child to the given Window.
        */
        void create(Window& parent) const;

        const ComponentArea& getComponentArea() const;
        void setComponentArea(const ComponentArea& area);

        const String& getBaseWidgetType() const;
        void setBaseWidgetType(const String& type);

        const String& getWidgetLookName() const;
        void setWidgetLookName(const String& look);

        const String& getWidgetNameSuffix() const;
        void setWidgetNameSuffix(const String& suffix);

        const String& getWindowRendererType() const;
        void setWindowRendererType(const String& type);

        VerticalAlignment getVerticalWidgetAlignment() const;
        void setVerticalWidgetAlignment(VerticalAlignment alignment);

        HorizontalAlignment getHorizontalWidgetAlignment() const;
        void setHorizontalWidgetAlignment(HorizontalAlignment alignment);

        void addPropertyInitialiser(const PropertyInitialiser& initialiser);
        void clearPropertyInitialisers();

        void layout(const Window& owner) const;

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

    private:
        typedef std::vector<PropertyInitialiser> PropertiesList;

        ComponentArea   d_area;              //!< Destination area for the widget (relative to it's parent).
        String   d_baseType;                 //!< Type of widget to be created.
        String   d_imageryName;              //!< Name of a WidgetLookFeel to be used for the widget.
        String   d_nameSuffix;               //!< Suffix to apply to the parent Window name to create this widgets unique name.
        String   d_rendererType;             //!< Name of the window renderer type to assign to the widget.
        VerticalAlignment    d_vertAlign;    //!< Vertical alignment to be used for this widget.
        HorizontalAlignment  d_horzAlign;    //!< Horizontal alignment to be used for this widget.
        PropertiesList  d_properties;        //!< Collection of PropertyInitialisers to be applied the the widget upon creation.
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalWidgetComponent_h_
