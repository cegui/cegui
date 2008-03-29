/***********************************************************************
    filename:   CEGUIFalWidgetLookManager.h
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
#ifndef _CEGUIFalWidgetLookManager_h_
#define _CEGUIFalWidgetLookManager_h_

#include "CEGUISingleton.h"
#include "CEGUIString.h"
#include "CEGUIExceptions.h"
#include "falagard/CEGUIFalWidgetLookFeel.h"
#include <map>

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#	pragma warning(disable : 4275)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Manager class that gives top-level access to widget data based "look and feel" specifications loaded into the system.
    */
    class CEGUIEXPORT WidgetLookManager : public Singleton<WidgetLookManager>
    {
    public:
        /*!
        \brief
            Constructor.
        */
        WidgetLookManager();

        /*!
        \brief
            Destructor
        */
        ~WidgetLookManager();

       	/*!
	      \brief
		        Return singleton WidgetLookManager object

	      \return
		        Singleton WidgetLookManager object
	      */
	      static	WidgetLookManager&	getSingleton(void);


	      /*!
	      \brief
		        Return pointer to singleton WindowFactoryManager object

	      \return
		        Pointer to singleton WindowFactoryManager object
	      */
	      static	WidgetLookManager*	getSingletonPtr(void);


        /*!
        \brief
            Parses a file containing window look & feel specifications (in the form of XML).

        \note
            If the new file contains specifications for widget types that are already specified, it is not an error;
            the previous definitions are overwritten by the new data.  An entry will appear in the log each time any
            look & feel component is overwritten.

        \param filename
            String object containing the filename of a file containing the widget look & feel data

        \param resourceGroup
            Resource group identifier to pass to the resource provider when loading the file.

        \return
            Nothing.

        \exception	FileIOException				thrown if there was some problem accessing or parsing the file \a filename
        \exception	InvalidRequestException		thrown if an invalid filename was provided.
        */
        void parseLookNFeelSpecification(const String& filename, const String& resourceGroup = "");


        /*!
        \brief
            Return whether a WidgetLookFeel has been created with the specified name.

        \param widget
            String object holding the name of a widget look to test for.

        \return
            - true if a WindowLookFeel named \a widget is available.
            - false if so such WindowLookFeel is currently available.
        */
        bool isWidgetLookAvailable(const String& widget) const;


        /*!
        \brief
            Return a const reference to a WidgetLookFeel object which has the specified name.

        \param widget
            String object holding the name of a widget look that is to be returned.

        \return
            const reference to the requested WidgetLookFeel object.

        \exception UnknownObjectException   thrown if no WindowLookFeel is available with the requested name.
        */
        const WidgetLookFeel& getWidgetLook(const String& widget) const;


        /*!
        \brief
            Erase the WidgetLookFeel that has the specified name.

        \param widget
            String object holding the name of a widget look to be erased.  If no such WindowLookFeel exists, nothing
            happens.

        \return
            Nothing.
        */
        void eraseWidgetLook(const String& widget);


        /*!
        \brief
            Add the given WidgetLookFeel.

        \note
            If the WidgetLookFeel specification uses a name that already exists within the system, it is not an error;
            the previous definition is overwritten by the new data.  An entry will appear in the log each time any
            look & feel component is overwritten.

        \param look
            WidgetLookFeel object to be added to the system.  NB: The WidgetLookFeel is copied, no change of ownership of the
            input object occurrs.

        \return
            Nothing.
        */
        void addWidgetLook(const WidgetLookFeel& look);


        /*!
        \brief
            Writes a complete Widge Look to a stream.  Note that xml file header and
            falagard opening/closing tags will also be written.

        \param name
            String holding the name of the widget look to be output to the stream.

        \param out_stream
            OutStream where XML data should be sent.
        */
        void writeWidgetLookToStream(const String& name, OutStream& out_stream) const;

        /*!
        \brief
            Writes a series of complete Widge Look objects to a stream.  Note that xml file header and
            falagard opening/closing tags will also be written.

            The \a prefix specifies a name prefix common to all widget looks to be written, you could
            specify this as "TaharezLook/" and then any defined widget look starting with that prefix, such
            as "TaharezLook/Button" and "TaharezLook/Listbox" will be written to the stream.

        \param prefix
            String holding the widget look name prefix, which will be used when searching for the widget looks
            to be output to the stream.

        \param out_stream
            OutStream where XML data should be sent.
        */
        void writeWidgetLookSeriesToStream(const String& prefix, OutStream& out_stream) const;

        /*!
        \brief
            Returns the default resource group currently set for LookNFeels.

        \return
            String describing the default resource group identifier that will be
            used when loading LookNFeel data.
        */
        static const String& getDefaultResourceGroup()
            { return d_defaultResourceGroup; }

        /*!
        \brief
            Sets the default resource group to be used when loading LookNFeel data

        \param resourceGroup
            String describing the default resource group identifier to be used.

        \return
            Nothing.
        */
        static void setDefaultResourceGroup(const String& resourceGroup)
            { d_defaultResourceGroup = resourceGroup; }


    private:
        static const String FalagardSchemaName;     //!< Name of schema file used for XML validation.

        typedef std::map<String, WidgetLookFeel, String::FastLessCompare> WidgetLookList;
        WidgetLookList  d_widgetLooks;

        static String d_defaultResourceGroup;   //!< holds default resource group
    };

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalWidgetLookManager_h_
