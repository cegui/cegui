/***********************************************************************
    created:  25/9/2014
    author:   David Reepmeyer

    purpose: holds the global android_app instance of an android app
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2011 Paul D Turner & The CEGUI Development Team
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
#ifndef _AndroidUtils_h_
#define _AndroidUtils_h_

#ifndef __ANDROID__
#   error "do not include AndroidUtils.h unless compiling for android"
#endif

#include <android/log.h>
#include <android_native_app_glue.h>


#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

namespace CEGUI
{
//----------------------------------------------------------------------------//

/*!
\brief
    All android applictions starts up with a global struct type
    of android_app that is passed in to the main method.  this
    is needed in various andriod functionality by CEGUI
*/
class AndroidUtils
{
public:
    /*!
    \brief
            Returns the android app struct

    \return
                android app struct
    */
    static android_app* getAndroidApp();

    /*!
    \brief
        sets the global android app struct.  this should
        be called once and used via CEGUI throughout
        the lifetime of the app

    \param androidapp
        global android app struct
    */
    static void setAndroidApp(android_app* androidapp);
private:
    static struct android_app* app;
};

}

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif


