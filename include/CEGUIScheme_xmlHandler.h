/************************************************************************
filename: 	CEGUIScheme_xmlHandler.h
created:	21/2/2004
author:		Paul D Turner

purpose:	Defines abstract base class for the GUI Scheme object.
*************************************************************************/
/*************************************************************************
Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
Copyright (C)2004 Paul D Turner (crayzed@users.sourceforge.net)

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
#ifndef _CEGUIScheme_xmlHandler_h_
#define _CEGUIScheme_xmlHandler_h_

#include "CEGUIScheme.h"

#include "xercesc/sax2/DefaultHandler.hpp"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Handler class used to parse the Scheme XML files using SAX2
*/
class Scheme_xmlHandler : public XERCES_CPP_NAMESPACE::DefaultHandler
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
	Scheme_xmlHandler(Scheme* scheme) : d_scheme(scheme) {}

	/*!
	\brief
		Destructor for Scheme::xmlHandler objects
	*/
	virtual ~Scheme_xmlHandler(void) {}

	/*************************************************************************
	SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
		document processing (only care about elements, schema validates format)
	*/
	virtual void	startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs);

	/*!
	\brief
		error processing
	*/
	virtual void  warning (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
	virtual void  error (const XERCES_CPP_NAMESPACE::SAXParseException &exc);
	virtual void  fatalError (const XERCES_CPP_NAMESPACE::SAXParseException &exc);

private:
	/*************************************************************************
	Implementation Constants
	*************************************************************************/
	// XML related strings
	static const utf8	GUISchemeElement[];				//!< Root GUIScheme element.
	static const utf8	ImagesetElement[];				//!< Element specifying an Imageset
	static const utf8	FontElement[];					//!< Element specifying a Font
	static const utf8	WindowSetElement[];				//!< Element specifying a module and set of WindowFactory elements.
	static const utf8	WindowFactoryElement[];			//!< Element specifying a WindowFactory type.
	static const char	NameAttribute[];				//!< Attribute specifying the name of some object.
	static const char	FilenameAttribute[];			//!< Attribute specifying the name of some file.


	/*************************************************************************
	Implementation Data
	*************************************************************************/
	Scheme* d_scheme;			//!< Scheme object that we are helping to build
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIScheme_xmlHandler_h_
