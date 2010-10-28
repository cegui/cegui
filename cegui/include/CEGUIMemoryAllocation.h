/***********************************************************************
	filename: 	CEGUIMemoryAllocation.h
	created:	14/10/2010
	author:		Martin Preisler (inspired by Ogre3D)

	purpose:	Allows custom memory allocators to be used within CEGUI
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
#ifndef _CEGUIMemoryAllocation_h_
#define _CEGUIMemoryAllocation_h_

#ifndef _CEGUIBase_h_
#   error Don't include this directly! Include CEGUIBase.h instead.
#endif

// this should really go to config.h, now it's there for testing since
// I am a premake noob...
#define CEGUI_CUSTOM_ALLOCATORS

#include "CEGUIMemoryAllocatedObject.h"
#include "CEGUIMemorySTLWrapper.h"

// convenience macro, sets all allocator types to given allocator
// use if you don't want to maintain lots of typedefs
#define CEGUI_SET_ONE_ALLOCATOR(Allocator)\
    typedef Allocator AnimationAllocator;\
    typedef Allocator AnimationInstanceAllocator;\
    typedef Allocator BiDiVisualMappingAllocator;\
    typedef Allocator EventAllocator;\
    typedef Allocator EventArgsAllocator;\
    typedef Allocator FactoryAllocator;\
    typedef Allocator FontAllocator;\
    typedef Allocator FormattedRenderedStringAllocator;\
    typedef Allocator GeometryBufferAllocator;\
    typedef Allocator ImageAllocator;\
    typedef Allocator ImageCodecAllocator;\
    typedef Allocator ImagesetAllocator;\
    typedef Allocator InterpolatorAllocator;\
    typedef Allocator LoggerAllocator;\
    typedef Allocator ModuleAllocator;\
    typedef Allocator PropertyAllocator;\
    typedef Allocator RawDataContainerAllocator;\
    typedef Allocator RegexMatcherAllocator;\
    typedef Allocator RenderedStringAllocator;\
    typedef Allocator RendererAllocator;\
    typedef Allocator RenderEffectAllocator;\
    typedef Allocator RenderingSurfaceAllocator;\
    typedef Allocator RenderQueueAllocator;\
    typedef Allocator RenderTargetAllocator;\
    typedef Allocator ResourceProviderAllocator;\
    typedef Allocator SchemeAllocator;\
    typedef Allocator SingletonAllocator;\
    typedef Allocator StringAllocator;\
    typedef Allocator STLAllocator;\
    typedef Allocator SubscriberAllocator;\
    typedef Allocator TextureAllocator;\
    typedef Allocator TrivialAllocator;\
    typedef Allocator WindowAllocator;\
    typedef Allocator WindowRendererAllocator;\
    typedef Allocator XMLAttributesAllocator;\
    typedef Allocator XMLHandlerAllocator;\
    typedef Allocator XMLParserAllocator;\
    typedef Allocator XMLSerializerAllocator;

// convenience macro - more just for clarity really
#define CEGUI_SET_ALLOCATOR(Category, Allocator)\
    typedef Allocator Category##Allocator;

#ifdef CEGUI_CUSTOM_ALLOCATORS

namespace CEGUI
{

// borrowed from Ogre, used to construct arrays
template<typename T>
T* constructN(T* basePtr, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		new ((void*)(basePtr+i)) T();
	}
	return basePtr;
}

} // CEGUI namespace

#ifndef CEGUI_MEMORY_DEBUG
#   define CEGUI_NEW_AO new
#   define CEGUI_DELETE_AO delete
// for primitive types, types not inherited from AllocatedObject
#   define CEGUI_NEW_PT(T, Allocator) new (Allocator::allocateBytes(sizeof(T))) T
#   define CEGUI_NEW_ARRAY_PT(T, count, Allocator) ::CEGUI::constructN(static_cast<T*>(Allocator::allocateBytes(sizeof(T)*(count))), count)
#   define CEGUI_DELETE_PT(ptr, T, Allocator) do{if(ptr){(ptr)->~T(); Allocator::deallocateBytes((void*)ptr);}}while(0)
#   define CEGUI_DELETE_ARRAY_PT(ptr, T, count, Allocator) do{if(ptr){for (size_t b = count; b-- > 0;){ (ptr)[b].~T();} Allocator::deallocateBytes((void*)ptr);}}while(0)
#else
#   define CEGUI_NEW_AO new(__FILE__, __LINE__, __FUNCTION__)
#   define CEGUI_DELETE_AO delete
// for primitive types, types not inherited from AllocatedObject
#   define CEGUI_NEW_PT(T, Allocator) new (Allocator::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T
#   define CEGUI_NEW_ARRAY_PT(T, count, Allocator) ::CEGUI::constructN(static_cast<T*>(Allocator::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count)
#   define CEGUI_DELETE_PT(ptr, T, Allocator) do{if(ptr){(ptr)->~T(); Allocator::deallocateBytes((void*)ptr);}}while(0)
#   define CEGUI_DELETE_ARRAY_PT(ptr, T, count, Allocator) do{if(ptr){for (size_t b = count; b-- > 0;){ (ptr)[b].~T();} Allocator::deallocateBytes((void*)ptr);}}while(0)
#endif

//#define CEGUI_CUSTOM_ALLOCATORS_INCLUDE "CEGUIMemoryOgreAllocator.h"

#ifndef CEGUI_CUSTOM_ALLOCATORS_INCLUDE
#   define CEGUI_CUSTOM_ALLOCATORS_INCLUDE "CEGUIMemoryStdAllocator.h"
#endif

// all the wrappers have been declared, now we include the chosen memory allocator file
#include CEGUI_CUSTOM_ALLOCATORS_INCLUDE

#else

// dummy macros
#define CEGUI_NEW_AO new
#define CEGUI_DELETE_AO delete
// for primitive types, types not inherited from AllocatedObject
#define CEGUI_NEW_PT(T, Allocator) new T
#define CEGUI_NEW_ARRAY_PT(T, count, Allocator) new T[count]
#define CEGUI_DELETE_PT(ptr, T, Allocator) delete ptr
#define CEGUI_DELETE_ARRAY_PT(ptr, T, count, Allocator) delete [] ptr

namespace CEGUI
{

// use int as allocator for everything, it's not going to be used anyways
CEGUI_SET_ONE_ALLOCATOR(int)

}

#endif

#endif	// end of guard _CEGUIMemoryAllocation_h_
