/***********************************************************************
    filename:   CEGUIFalSectionSpecification.h
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
#ifndef _CEGUIFalSectionSpecification_h_
#define _CEGUIFalSectionSpecification_h_

#include "CEGUIWindow.h"


// Start of CEGUI namespace section
namespace CEGUI
{
    // forward refs
    class WidgetLookFeel;

    /*!
    \brief
        Class that represents a simple 'link' to an ImagerySection.

        This class enables sections to be easily re-used, by different states and/or layers, by allowing
        sections to be specified by name rather than having mutiple copies of the same thing all over the place.
    */
    class CEGUIEXPORT SectionSpecification
    {
    public:
        /*!
        \brief
            Constructor

        \param owner
            String holding the name of the WidgetLookFeel object that contains the target section.

        \param sectionName
            String holding the name of the target section.

        \param controlPropertySource
            String holding the name of a boolean property that will control if
            the rendering for this secion will actually occur or not.
        */
        SectionSpecification(const String& owner, const String& sectionName, const String& controlPropertySource);

        /*!
        \brief
            Constructor

        \param owner
            String holding the name of the WidgetLookFeel object that contains the target section.

        \param sectionName
            String holding the name of the target section.

        \param controlPropertySource
            String holding the name of a boolean property that will control if
            the rendering for this secion will actually occur or not.

        \param cols
            Override colours to be used (modulates sections master colours).
        */
        SectionSpecification(const String& owner, const String& sectionName, const String& controlPropertySource, const ColourRect& cols);

        /*!
        \brief
            Render the section specified by this SectionSpecification.

        \param srcWindow
            Window object to be used when calculating pixel values from BaseDim values.

        \param base_z
            base z co-ordinate to use for all imagery in the linked section.

        \return
            Nothing.
        */
        void render(Window& srcWindow, float base_z, const ColourRect* modcols = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Render the section specified by this SectionSpecification.

        \param srcWindow
            Window object to be used when calculating pixel values from BaseDim values.

        \param baseRect
            Rect object to be used when calculating pixel values from BaseDim values.

        \param base_z
            base z co-ordinate to use for all imagery in the linked section.

        \return
            Nothing.
        */
        void render(Window& srcWindow, const Rect& baseRect, float base_z, const ColourRect* modcols = 0, const Rect* clipper = 0, bool clipToDisplay = false) const;

        /*!
        \brief
            Return the name of the WidgetLookFeel object containing the target section.

        \return
            String object holding the name of the WidgetLookFeel that contains the target ImagerySection.
        */
        const String& getOwnerWidgetLookFeel() const;

        /*!
        \brief
            Return the name of the target ImagerySection.

        \return
            String object holding the name of the target ImagerySection.
        */
        const String& getSectionName() const;

        /*!
        \brief
            Return the current override colours.

        \return
            ColourRect holding the colours that will be modulated with the sections master colours if
            colour override is enabled on this SectionSpecification.
        */
        const ColourRect& getOverrideColours() const;

        /*!
        \brief
            Set the override colours to be used by this SectionSpecification.

        \param cols
            ColourRect describing the override colours to set for this SectionSpecification.

        \return
            Nothing.
        */
        void setOverrideColours(const ColourRect& cols);

        /*!
        \brief
            return whether the use of override colours is enabled on this SectionSpecification.

        \return
            - true if override colours will be used for this SectionSpecification.
            - false if override colours will not be used for this SectionSpecification.
        */
        bool isUsingOverrideColours() const;

        /*!
        \brief
            Enable or disable the use of override colours for this section.

        \param setting
            - true if override colours should be used for this SectionSpecification.
            - false if override colours should not be used for this SectionSpecification.

        \return
            Nothing.
        */
        void setUsingOverrideColours(bool setting = true);

        /*!
        \brief
            Set the name of the property where override colour values can be obtained.

        \param property
            String containing the name of the property.

        \return
            Nothing.
        */
        void setOverrideColoursPropertySource(const String& property);

        /*!
        \brief
            Set whether the override colours property source represents a full ColourRect.

        \param setting
            - true if the override colours property will access a ColourRect object.
            - false if the override colours property will access a colour object.

        \return
            Nothing.
        */
        void setOverrideColoursPropertyIsColourRect(bool setting = true);

        /*!
        \brief
            Set the name of the property that controls whether to actually
            render this section.

        \param property
            String containing the name of the property.

        \return
            Nothing.
        */
        void setRenderControlPropertySource(const String& property);

        /*!
        \brief
            Writes an xml representation of this SectionSpecification to \a out_stream.

        \param xml_stream
            Stream where xml data should be output.


        \return
            Nothing.
        */
        void writeXMLToStream(XMLSerializer& xml_stream) const;

    protected:
        /*!
        \brief
            Helper method to initialise a ColourRect with appropriate values according to the way the
            section sepcification is set up.

            This will try and get values from multiple places:
                - a property attached to \a wnd
                - the integral d_coloursOverride values.
                - or default to colour(1,1,1,1);
        */
        void initColourRectForOverride(const Window& wnd, ColourRect& cr) const;

    private:
        String          d_owner;                //!< Name of the WidgetLookFeel containing the required section.
        String          d_sectionName;          //!< Name of the required section within the specified WidgetLookFeel.
        ColourRect      d_coloursOverride;      //!< Colours to use when override is enabled.
        bool            d_usingColourOverride;  //!< true if colour override is enabled.
        String          d_colourPropertyName;   //!< name of property to fetch colours from.
        bool            d_colourProperyIsRect;  //!< true if the colour property will fetch a full ColourRect.
        String          d_renderControlProperty;    //!< Name of a 'boolean' property that controls whether to actually draw this section.
    };


} // End of  CEGUI namespace section


#endif  // end of guard _CEGUIFalSectionSpecification_h_
