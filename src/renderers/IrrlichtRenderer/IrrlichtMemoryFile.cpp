/************************************************************************
    filename:   IrrlichtMemoryFile.cpp
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
#include "renderers/IrrlichtRenderer/IrrlichtMemoryFile.h"

// Start of CEGUI namespace section
namespace CEGUI
{
IrrlichtMemoryFile::IrrlichtMemoryFile(const String& filename, const unsigned char* memory, uint32 size)
 : d_filename(filename),
   d_buffer(memory),
   d_size(size),
   d_position(0)
{
}

irr::s32 IrrlichtMemoryFile::read(void* buffer, irr::s32 sizeToRead)
{
    uint32 realReadSize =
        ((d_position + sizeToRead) > d_size) ? d_size - d_position : sizeToRead;

    std::memcpy(buffer, d_buffer + d_position, realReadSize);
    d_position += realReadSize;

    return realReadSize;
}

bool IrrlichtMemoryFile::seek(irr::s32 finalPos, bool relativeMovement)
{
    uint32 targetPosition = relativeMovement ? d_position : 0;
    targetPosition += finalPos;

    if (targetPosition > d_size)
    {
        return false;
    }
    else
    {
        d_position = targetPosition;
        return true;
    }
}

irr::s32 IrrlichtMemoryFile::getSize()
{
    return d_size;
}

irr::s32 IrrlichtMemoryFile::getPos()
{
    return d_position;
}

const irr::c8* IrrlichtMemoryFile::getFileName()
{
    return d_filename.c_str();
}

} // End of  CEGUI namespace section
