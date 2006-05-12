/***********************************************************************
	filename: 	CEGUIDefaultLogger.h
	created:	25/1/2006
	author:		Andrew Zabolotny
	
	purpose:	Defines interface for the default Logger implementation
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
