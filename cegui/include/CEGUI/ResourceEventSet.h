/***********************************************************************
    created:    4th July 2015
    author:     Lukas Meindl
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2015 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIResourceEventSet_h_
#define _CEGUIResourceEventSet_h_

#include "EventSet.h"

namespace CEGUI
{

//! EventSet for loading of resources
class CEGUIEXPORT ResourceEventSet : public EventSet
{
public:
    //! Namespace name for all resource managers
    static const String EventNamespace;
    /** Name of event fired when a resource is created by this manager.
        * Handlers are passed a const ResourceEventArgs reference with
        * ResourceEventArgs::resourceType String set to the type of resource that
        * the event is related to, and ResourceEventArgs::resourceName String set
        * to the name of the resource that the event is related to.
        */
    static const String EventResourceCreated;
    /** Name of event fired when a resource is destroyed by this manager.
        * Handlers are passed a const ResourceEventArgs reference with
        * ResourceEventArgs::resourceType String set to the type of resource that
        * the event is related to, and ResourceEventArgs::resourceName String set
        * to the name of the resource that the event is related to.
        */
    static const String EventResourceDestroyed;
    /** Name of event fired when a resource is replaced by this manager.
        * Handlers are passed a const ResourceEventArgs reference with
        * ResourceEventArgs::resourceType String set to the type of resource that
        * the event is related to, and ResourceEventArgs::resourceName String set
        * to the name of the resource that the event is related to.
        */
    static const String EventResourceReplaced;
};


}

#endif