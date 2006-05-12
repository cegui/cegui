/***********************************************************************
	filename: 	CEGUIConfig_xmlHandler.h
	created:	17/7/2004
	author:		Paul D Turner
	
	purpose:	Interface to configuration file parser
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
#ifndef _CEGUIConfig_xmlHandler_h_
#define _CEGUIConfig_xmlHandler_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUILogger.h"
#include "CEGUIXMLHandler.h"

#include <vector>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
	Handler class used to parse the Configuration XML file.
*/
class Config_xmlHandler : public XMLHandler
{
public:
	/*************************************************************************
		Construction & Destruction
	*************************************************************************/
	/*!
	\brief
		Constructor for GUILayout_xmlHandler objects
	*/
	Config_xmlHandler(void) {}

	/*!
	\brief
		Destructor for GUILayout_xmlHandler objects
	*/
	virtual ~Config_xmlHandler(void) {}

	/*************************************************************************
		SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
		document processing (only care about elements, schema validates format)
	*/
    virtual void elementStart(const String& element, const XMLAttributes& attributes);

	/*************************************************************************
		Functions used by our implementation
	*************************************************************************/
	/*!
	\brief
		Return log filename
	*/
	const String&	getLogFilename(void) const				{return d_logFilename;}


	/*!
	\brief
		Return initial scheme filename to load
	*/
	const String&	getSchemeFilename(void) const			{return d_schemeFilename;}


	/*!
	\brief
		Return initial layout filename to load and set as the GUI sheet.
	*/
	const String&	getLayoutFilename(void) const			{return d_layoutFilename;}


	/*!
	\brief
		Return the name of the initialisation script to run
	*/
	const String&	getInitScriptFilename(void) const		{return d_initScriptFilename;}


	/*!
	\brief
		Return the name of the termination script to run
	*/
	const String&	getTermScriptFilename(void) const		{return d_termScriptFilename;}


	/*!
	\brief
		Return name of font to use as default.
	*/
	const String&	getDefaultFontName(void) const		{return d_defaultFontName;}


	/*!
	\brief
		Return name of default resource group.
	*/
	const String&	getDefaultResourceGroup(void) const		{return d_defaultResourceGroup;}

    /*!
    \brief
        Return logging level which was read from the config file.
    */
    LoggingLevel getLoggingLevel(void) const     {return d_logLevel;}


private:
	/*************************************************************************
		Implementation Constants
	*************************************************************************/
	static const String CEGUIConfigElement;				//!< Tag name for CEGUIConfig elements.
	static const char	ConfigLogfileAttribute[];			//!< Attribute name that stores the filename to use for the log.
	static const char	ConfigSchemeAttribute[];			//!< Attribute name that stores the filename of a scheme to load.
	static const char	ConfigLayoutAttribute[];			//!< Attribute name that stores the filename of a layout to load.
	static const char	ConfigDefaultFontAttribute[];		//!< Attribute name that stores the name of the default font to set (as loaded by scheme)
	static const char	ConfigInitScriptAttribute[];		//!< Attribute name that stores the filename of an initialisation script to run.
	static const char	ConfigTerminateScriptAttribute[];	//!< Attribute name that stores the filename of a termination script to run.
    static const char   ConfigDefaultResourceGroupAttribute[]; //!< Attribute name that stores the default resource group (also used when loading config resources).
    static const char   ConfigLoggingLevelAttribute[];      //!< Attribute name that stores the logging level to be used.

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	String		d_logFilename;			//!< filename for the log.
	String		d_schemeFilename;		//!< filename for the scheme to auto-load.
	String		d_layoutFilename;		//!< filename for the layout to auto-load.
	String		d_initScriptFilename;	//!< filename for the script to run after system init.
	String		d_termScriptFilename;	//!< filename for the script to run before system shutdown.
	String		d_defaultFontName;		//!< Holds name of default font to set.
    String      d_defaultResourceGroup; //!< Holds default resource group name.
    LoggingLevel    d_logLevel;         //!< Holds the logging level read from the config.
};


} // End of  CEGUI namespace section

#endif	// end of guard _CEGUIConfig_xmlHandler_h_
