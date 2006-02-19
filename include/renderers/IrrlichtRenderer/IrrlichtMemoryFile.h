/************************************************************************
    filename:   IrrlichtMemoryFile.h
    created:    Sun Feb 19 2006
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
#include "CEGUIBase.h"
#include "CEGUIString.h"
#include <irrlicht.h>

// Start of CEGUI namespace section
namespace CEGUI
{
/*!
\brief
    Class to wrap a file access interface around a memory buffer to enable us to
    pass data that has been loaded via the CEGUI::ResourceProvider to irrlicht,
    via it's IReadFile based interfaces.
*/
class IrrlichtMemoryFile : public irr::io::IReadFile
{
public:
    IrrlichtMemoryFile(const String& filename, const unsigned char* memory, uint32 size);
    virtual ~IrrlichtMemoryFile() {};
    virtual irr::s32 read(void* buffer, irr::s32 sizeToRead);
    virtual bool seek(irr::s32 finalPos, bool relativeMovement = false);
    virtual irr::s32 getSize();
    virtual irr::s32 getPos();
    virtual const irr::c8* getFileName();

protected:
    String d_filename;
    const unsigned char* d_buffer;
    uint32 d_size;
    uint32 d_position;
};

} // End of  CEGUI namespace section
