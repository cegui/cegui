/***********************************************************************
    filename:   IrrlichtMemoryFile.cpp
    created:    Sun Feb 19 2006
    author:     Paul D Turner <paul@cegui.org.uk>
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
#include "IrrlichtMemoryFile.h"
#include <memory.h>

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

#if CEGUI_IRRLICHT_ABOVE_1_3 == 1
irr::s32 IrrlichtMemoryFile::read(void* buffer, irr::u32 sizeToRead)
#else
irr::s32 IrrlichtMemoryFile::read(void* buffer, irr::s32 sizeToRead)
#endif
{
    uint32 realReadSize =
        ((d_position + sizeToRead) > d_size) ? d_size - d_position : sizeToRead;

    memcpy(buffer, d_buffer + d_position, realReadSize);
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
