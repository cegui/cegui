/************************************************************************
	filename: 	CEGUIScheme.h
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
#ifndef _CEGUIScheme_h_
#define _CEGUIScheme_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUISchemeManager.h"

#include "xercesc/sax2/DefaultHandler.hpp"

#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	A class that groups a set of GUI elements and initialises the system to access those elements.

	A GUI Scheme is a high-level construct that loads and initialises various lower-level objects
	and registers them within the system for usage.  So, for example, a Scheme might create some
	Imageset objects, some Font objects, and register a collection of WindowFactory objects within
	the system which would then be in a state to serve those elements to client code.
*/
class CEGUIBASE_API Scheme
{
public:
	/*!
	\brief
		Loads all resources for this scheme.

	\return
		Nothing.
	*/
	void	loadResources(void);


	/*!
	\brief
		Unloads all resources for this scheme.  This should be used very carefully.

	\return
		Nothing.
	*/
	void	unloadResources(void);


	/*!
	\brief
		Return whether the resources for this Scheme are all loaded.

	\return
		true if all resources for the Scheme are loaded and available, or false of one or more resource is not currently loaded.
	*/
	bool	resourcesLoaded(void) const;


	/*!
	\brief
		Return the name of this Scheme.

	\return
		String object containing the name of this Scheme.
	*/
	const String& getName(void) const		{return d_name;}

private:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const char	GUISchemeSchemaName[];			//!< Filename of the XML schema used for validating GUIScheme files.

	/*************************************************************************
		Friends
	*************************************************************************/
	friend	Scheme* SchemeManager::loadScheme(const String& scheme_filename);
	friend	void	SchemeManager::unloadScheme(const String& scheme_name);


	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	/*!
	\brief
		Creates a scheme object from the data specified in the file \a filename

	\param filename
		String object holding the name of the file to use when creating this Scheme object.

	\return
		Nothing.
	*/
	Scheme(const String& filename);


	/*!
	\brief
		Destroys a Scheme object

	\return
		Nothing
	*/
	~Scheme(void);

	
	/*************************************************************************
		Structs used to hold scheme information
	*************************************************************************/
	struct LoadableUIElement
	{
		String	name;
		String	filename;
	};

	struct	UIElementFactory
	{
		String name;
	};

	struct	UIModule
	{
		String name;
		FactoryModule*	module;
		std::vector<UIElementFactory>	factories;
	};

	/*************************************************************************
		Implementation Classes
	*************************************************************************/
	/*!
	\brief
		Handler class used to parse the Scheme XML files using SAX2
	*/
	class xmlHandler : public XERCES_CPP_NAMESPACE::DefaultHandler
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
		xmlHandler(Scheme* scheme) : d_scheme(scheme) {}

		/*!
		\brief
			Destructor for Scheme::xmlHandler objects
		*/
		virtual ~xmlHandler(void) {}

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
		static const char	GUISchemeElement[];				//!< Root GUIScheme element.
		static const char	ImagesetElement[];				//!< Element specifying an Imageset
		static const char	FontElement[];					//!< Element specifying a Font
		static const char	WindowSetElement[];				//!< Element specifying a module and set of WindowFactory elements.
		static const char	WindowFactoryElement[];			//!< Element specifying a WindowFactory type.
		static const char	NameAttribute[];				//!< Attribute specifying the name of some object.
		static const char	FilenameAttribute[];			//!< Attribute specifying the name of some file.


		/*************************************************************************
			Implementation Data
		*************************************************************************/
		Scheme* d_scheme;			//!< Scheme object that we are helping to build
	};



	/*************************************************************************
		Implementation Data
	*************************************************************************/
	String	d_name;			//!< the name of this scheme.

	std::vector<LoadableUIElement>		d_imagesets;
	std::vector<LoadableUIElement>		d_fonts;
	std::vector<UIModule>				d_widgetModules;
};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIScheme_h_
