/***********************************************************************
	created:	14/10/2010
	author:		Martin Preisler

	purpose:	Implements the default "dummy" allocator
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2010 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIMemoryStdAllocator_h_
#define _CEGUIMemoryStdAllocator_h_

#include <stdlib.h>
#include <limits>

namespace CEGUI
{

class CEGUIEXPORT StdAllocator
{
public:
	static inline void* allocateBytes(size_t count)
	{
		void* ptr = malloc(count);
#if defined(CEGUI_CUSTOM_ALLOCATORS_USAGE)
		if (ptr == NULL)
			return NULL;
		getAllocationSize(count);
#endif
		return ptr;
	}

	static inline void deallocateBytes(void* ptr)
	{
#if defined(CEGUI_CUSTOM_ALLOCATORS_USAGE)
		if (ptr == NULL)
			return;
		size_t count = 0;
#if defined(_MSC_VER)
		count = _msize(ptr);
#elif defined(__linux__)
		count = malloc_usable_size(ptr);
#elif defined(__APPLE__)
		count = malloc_size(ptr);
#endif
		getAllocationSize((size_t)0 - count);
#endif
		free(ptr);
	}

    // !!! IF YOU GET AN ERROR HERE:
    // that says something like: "You can't call allocateBytes with 4 arguments",
    // you are using StdAllocator and trying to enable CEGUI_CUSTOM_ALLOCATORS_DEBUG, you
    // have to provide your own custom memory allocator if you want memory debugging.

	/// Get the maximum size of a single allocation
	static inline size_t getMaxAllocationSize()
	{
		return std::numeric_limits<size_t>::max();
	}

	static inline size_t getAllocationSize(size_t adjust = 0)
	{
		static size_t allocationSize = 0;
		if (adjust == 0)
			return allocationSize;
#if defined(CEGUI_CUSTOM_ALLOCATORS_USAGE)
#if defined(_M_IX86)
		_InterlockedExchangeAdd(reinterpret_cast<long*>(&allocationSize), adjust);
#elif defined(_M_AMD64)
		_InterlockedExchangeAdd64(reinterpret_cast<long long*>(&allocationSize), adjust);
#elif defined(__GNUC__)
		__sync_add_and_fetch(&allocationSize, adjust);
#endif
#endif
		return allocationSize;
	}
};

CEGUI_SET_DEFAULT_ALLOCATOR(StdAllocator)

}

#endif	// end of guard _CEGUIMemoryStdAllocator_h_
