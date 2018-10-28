/***************************************************************************
 *   Copyright (C) 2018 Erik Ogenvik
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

#ifndef _CEGUIOgreMacros_h_
#define _CEGUIOgreMacros_h_

#include <OgrePrerequisites.h>

//New shared ptr API introduced in 1.10.1
#if OGRE_VERSION >= 0x10A01
#define OGRE_RESET(_sharedPtr) ((_sharedPtr).reset())
#define OGRE_ISNULL(_sharedPtr) (!(_sharedPtr))
#else
#define OGRE_RESET(_sharedPtr) ((_sharedPtr).setNull())
#define OGRE_ISNULL(_sharedPtr) ((_sharedPtr).isNull())
#endif

#endif //_CEGUIOgreMacros_h_
