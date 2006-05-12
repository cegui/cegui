/***********************************************************************
    filename:   CEGUIScheme_xmlHandler.h
    created:    21/2/2004
    author:     Paul D Turner
    
    purpose:    Defines abstract base class for the GUI Scheme object.
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
#ifndef _CEGUIScheme_xmlHandler_h_
#define _CEGUIScheme_xmlHandler_h_

#include "CEGUIScheme.h"
#include "CEGUIXMLHandler.h"

// Start of CEGUI namespace section
namespace CEGUI
{

    /*!
    \brief
        Handler class used to parse the Scheme XML files using SAX2
    */
    class Scheme_xmlHandler : public XMLHandler
    {
    public:
        /*************************************************************************
        Construction & Destruction
        *************************************************************************/
        /*!
        \brief
            Constructor for Scheme::xmlHandler objects

        \param scheme
            Pointer to the Scheme object creating this xmlHandler object
        */
        Scheme_xmlHandler(Scheme* scheme) : d_scheme(scheme)
        {}

        /*!
        \brief
            Destructor for Scheme::xmlHandler objects
        */
        virtual ~Scheme_xmlHandler(void)
        {}

        /*************************************************************************
        SAX2 Handler overrides
        *************************************************************************/
        /*!
        \brief
            document processing (only care about elements, schema validates format)
        */
        virtual void elementStart(const String& element, const XMLAttributes& attributes);
        virtual void elementEnd(const String& element);

    private:
        /*************************************************************************
        Implementation Constants
        *************************************************************************/
        // XML related strings
        static const String GUISchemeElement;               //!< Root GUIScheme element.
        static const String ImagesetElement;                //!< Element specifying an Imageset
        static const String ImagesetFromImageElement;       //!< Element specifying an Imageset to be created directly via an image file.
        static const String FontElement;                    //!< Element specifying a Font
        static const String WindowSetElement;               //!< Element specifying a module and set of WindowFactory elements.
        static const String WindowFactoryElement;           //!< Element specifying a WindowFactory type.
        static const String WindowAliasElement;             //!< Element specifying a WindowFactory type alias
        static const String FalagardMappingElement;         //!< Element specifying a Falagard window mapping
        static const String LookNFeelElement;               //!< Element specifying a LookNFeel.
        static const String NameAttribute;                  //!< Attribute specifying the name of some object.
        static const String FilenameAttribute;              //!< Attribute specifying the name of some file.
        static const String AliasAttribute;                 //!< Attribute specifying an alias name.
        static const String TargetAttribute;                //!< Attribute specifying target for an alias.
        static const String ResourceGroupAttribute;         //!< Attribute specifying resource group for some loadable resource.
        static const String WindowTypeAttribute;            //!< Attribute specifying the type of a window being created via a mapping.
        static const String TargetTypeAttribute;            //!< Attribute specifying the base target type of a falagard mapped window type.
        static const String LookNFeelAttribute;             //!< Attribute specifying the name of a LookNFeel for a falagard mapping..
        static const String WindowRendererSetElement;       //!< Element specifying a module and set of WindowRendererFactory elements.
        static const String WindowRendererFactoryElement;   //!< Element specifying a WindowRendererFactory type.
        static const String WindowRendererAttribute;        //!< Attribute specifying the type name of a window renderer.

        /*!
        \brief
            Method that handles the opening GUIScheme XML element.
        */
        void elementGUISchemeStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the Imageset XML element.
        */
        void elementImagesetStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the ImagesetFromImage XML element.
        */
        void elementImagesetFromImageStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the Font XML element.
        */
        void elementFontStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the WindowSet XML element.
        */
        void elementWindowSetStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the WindowFactory XML element.
        */
        void elementWindowFactoryStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the WindowRendererSet XML element.
        */
        void elementWindowRendererSetStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the WindowRendererFactory XML element.
        */
        void elementWindowRendererFactoryStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the WindowAlias XML element.
        */
        void elementWindowAliasStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the FalagardMapping XML element.
        */
        void elementFalagardMappingStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the LookNFeel XML element.
        */
        void elementLookNFeelStart(const XMLAttributes& attributes);

        /*!
        \brief
            Method that handles the closing GUIScheme XML element.
        */
        void elementGUISchemeEnd();

        /*************************************************************************
        Implementation Data
        *************************************************************************/
        Scheme* d_scheme;           //!< Scheme object that we are helping to build
    };

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIScheme_xmlHandler_h_
