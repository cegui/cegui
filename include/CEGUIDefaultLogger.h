/************************************************************************
	filename: 	CEGUIDefaultLogger.h
	created:	25/1/2006
	author:		Andrew Zabolotny
	
	purpose:	Defines interface for the default Logger implementation
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _CEGUIDefaultLogger_h_
#define _CEGUIDefaultLogger_h_

#include "CEGUILogger.h"


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4275)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
	Default implementation for the Logger class.
    If you want to redirect CEGUI logs to some place other than a text file,
    implement your own Logger implementation and create a object of the
    Logger type before creating the CEGUI::System singleton.
*/
class CEGUIEXPORT DefaultLogger : public Logger
{
public:
	/*!
	\brief
		Constructor for DefaultLogger object.
	*/
	DefaultLogger(void);

	/*!
	\brief Destructor for DefaultLogger object.
	*/
	virtual ~DefaultLogger(void);

	
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
	virtual void logEvent(const String& message, LoggingLevel level = Standard);

    /*!
    \brief
        Set the name of the log file where all subsequent log entries should be written.

    \note
        When this is called, and the log file is created, any cached log entries are
        flushed to the log file.

    \param filename
        Name of the file to put log messages.

    \param append
        - true if events should be added to the end of the current file.
        - false if the current contents of the file should be discarded.
     */
    virtual void setLogFilename(const String& filename, bool append = false);

protected:
    /*************************************************************************
		Implementation Data
	*************************************************************************/
	std::ofstream	d_ostream;		//!< Stream used to implement the logger
    std::vector<std::pair<String, LoggingLevel> > d_cache;    //!< Used to cache log entries before log file is created.
    std::ostringstream d_workstream;//!< Used to build log entry strings. 
    bool d_caching;                 //!< true while log entries are beign cached (prior to logfile creation)
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIDefaultLogger_h_
