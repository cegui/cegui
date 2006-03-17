/************************************************************************
    filename:   CEGUIExpatParser.h
    created:    Mon Mar 6 2006
    author:     Paul D Turner <paul@cegui.org.uk> (based on Dalfy's code)
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2006 Paul D Turner (paul@cegui.org.uk)

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
#ifndef _CEGUIExpatParser_h_
#define _CEGUIExpatParser_h_

#include "CEGUIXMLParser.h"

#if defined( __WIN32__ ) || defined( _WIN32 )
#   ifdef CEGUIEXPATPARSER_EXPORTS
#       define CEGUIEXPATPARSER_API __declspec(dllexport)
#   else
#       define CEGUIEXPATPARSER_API __declspec(dllimport)
#   endif
#else
#   define CEGUIEXPATPARSER_API
#endif

// Start of CEGUI namespace section
namespace CEGUI
{

/*!
\brief
    Implementation of XMLParser using Expat
*/
class CEGUIEXPATPARSER_API ExpatParser : public CEGUI::XMLParser
{
public:
    ExpatParser(void);
    ~ExpatParser(void);
    // Implementation of public abstract interface
    void parseXMLFile(CEGUI::XMLHandler& handler, const CEGUI::String& filename, const CEGUI::String& schemaName, const CEGUI::String& resourceGroup);

protected:
    // Implementation of protected abstract interface.
    bool initialiseImpl(void);
    // Implementation of protected abstract interface.
    void cleanupImpl(void);
    // C++ class methods name are not valide C function pointer. static solve this
    static void startElement(void* data, const char* element, const char**attr); // Expat handlers
    static void endElement(void* data, const char* element); // Expat handlers
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUIEXpatParser_h_
