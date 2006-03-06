/************************************************************************
    filename:   CEGUILibxmlParser.h
    created:    Mon Mar 6 2006
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUILibxmlParser_h_
#define _CEGUILibxmlParser_h_

#include "CEGUIXMLParser.h"

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Implementation of XMLParser using libxml
*/
class LibxmlParser : public XMLParser
{
public:
    LibxmlParser(void);
    ~LibxmlParser(void);

    // implementation of abstract members of base class
    void parseXMLFile(XMLHandler& handler, const String& filename, const String& schemaName, const String& resourceGroup);

protected:
    // implementation of abstract members of base class
    bool initialiseImpl(void);
    void cleanupImpl(void);
};

} // End of  CEGUI namespace section

#endif // end of guard _CEGUILibxmlParser_h_
