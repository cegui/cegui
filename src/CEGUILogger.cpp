/************************************************************************
	filename: 	CEGUILogger.cpp
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Implementation of the Logger class
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
#include "CEGUILogger.h"
#include <ctime>
#include <iomanip>

// Start of CEGUI namespace section
namespace CEGUI
{
/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> Logger* Singleton<Logger>::ms_Singleton	= NULL;


/*************************************************************************
	Constructor
*************************************************************************/
Logger::Logger(const String& filename, bool append /* = false */) :
	d_level(Standard),
	d_ostream(filename.c_str(), std::ios_base::out | (append ? std::ios_base::app : std::ios_base::trunc))
{
	// initialise width for date & time alignment.
	d_ostream.width(2);

	// create log header
	logEvent((utf8*)"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+");
	logEvent((utf8*)"+                     Crazy Eddie's GUI System - Event log                    +");
	logEvent((utf8*)"+                     (http://www.cegui.org.uk)                    +");
	logEvent((utf8*)"+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	logEvent((utf8*)"CEGUI::Logger singleton created.");
}

/*************************************************************************
	Destructor
*************************************************************************/
Logger::~Logger(void)
{
	if (d_ostream.is_open())
	{
		logEvent((utf8*)"CEGUI::Logger singleton destroyed.");
		d_ostream.close();
	}

}

/*************************************************************************
	Logs an event
*************************************************************************/
void Logger::logEvent(const String& message, LoggingLevel level /* = Standard */)
{
	// only log the message if the current logging level is >= the level for the message
	if (d_level >= level)
	{
		using namespace std;

		time_t	et;
		time(&et);
		tm* etm = gmtime(&et);

		if (etm != NULL)
		{
			// write date
			d_ostream << setfill('0') << setw(2) << etm->tm_mday << '/' <<
				setfill('0') << setw(2) << 1 + etm->tm_mon << '/' <<
				setw(4) << (1900 + etm->tm_year) << ' ';

			// wite time
			d_ostream << setfill('0') << setw(2) << etm->tm_hour << ':' <<
				setfill('0') << setw(2) << etm->tm_min << ':' <<
				setfill('0') << setw(2) << etm->tm_sec << ' ';

			// write event type code
			switch(level)
			{
			case Errors:
				d_ostream << "(Error)\t";
				break;

			case Standard:
				d_ostream << "(InfL1)\t";
				break;

			case Informative:
				d_ostream << "(InfL2)\t";
				break;

			case Insane:
				d_ostream << "(InfL3)\t";
				break;

			default:
				d_ostream << "(Unkwn)\t";
				break;
			}

			// write message
			d_ostream << message << endl;
		}

		// ensure new event is written to the file, rather than just being buffered.
		d_ostream.flush();
	}
}


Logger&	Logger::getSingleton(void)
{
	return Singleton<Logger>::getSingleton();
}

} // End of  CEGUI namespace section
