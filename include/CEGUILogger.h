/************************************************************************
	filename: 	CEGUILogger.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Defines interface for the Logger class
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
#ifndef _CEGUILogger_h_
#define _CEGUILogger_h_

#include "CEGUIBase.h"
#include "CEGUIString.h"
#include <fstream>

#include "CEGUISingleton.h"

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Enumeration of logging levels
*/
enum LoggingLevel
{
	Errors,			//!< Only actual error conditions will be logged.
	Standard,		//!< Basic events will be logged (default level).
	Informative,	//!< Useful tracing (object creations etc) information will be logged.
	Insane,			//!< Mostly everything gets logged (use for heavy tracing only, log WILL be big).
};

/*!
\brief
	Class that implements logging for the GUI system
*/
class CEGUIBASE_API Logger : public Singleton <Logger>
{
public:
	/*!
	\brief
		Constructor for Logger object

	\param filename
		Name of the file to put log messages.

	\param append
		- If true, events will be added to the end of the current file.
		- If false, the current contents of the file will be discarded.

	\return
		Nothing.
	*/
	Logger(const String& filename, bool append = false);

	/*!
	\brief Destructor for Logger object.
	*/
	virtual ~Logger(void);

	
	/*!
	\brief
		Return the singleton Logger object

	\return
		Reference to the one and only Logger object
	*/
	static Logger&	getSingleton(void)			{return Singleton<Logger>::getSingleton();}


	/*!
	\brief
		Set the level of logging information that will get out to the log file

	\param level
		One of the LoggingLevel enumerated values that specified the level of logging information required.

	\return
		Nothing
	*/
	void	setLoggingLevel(LoggingLevel level)		{d_level = level;}


	/*!
	\brief
		return the current logging level setting

	\return
		One of the LoggingLevel enumerated values specifying the current level of logging
	*/
	LoggingLevel	getLoggingLevel(void) const		{return d_level;}


	/*!
	\brief
		Add an event to the log.

	\param message
		String object containing the message to be added to the event log.

	\param level
		LoggingLevel for this message.  If \a level is greater than the current set logging level, the message is not logged.

	\return
		Nothing
	*/
	void	logEvent(const String& message, LoggingLevel level = Standard);

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	LoggingLevel	d_level;		//!< Holds current logging level
	std::ofstream	d_ostream;		//!< Stream used to implement the logger

private:
	/*************************************************************************
		Copy constructor and assignment usage is denied.
	*************************************************************************/
	Logger(const Logger& logger) {}
	Logger& operator=(const Logger& logger) {return *this;}

};

} // End of  CEGUI namespace section

#endif	// end of guard _CEGUILogger_h_
