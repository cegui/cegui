/***********************************************************************
    filename:   CEGUIAnimation_xmlHandler.h
    created:    Wed Aug 11 2010
    author:     Paul D Turner <paul@cegui.org.uk>
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
#ifndef _CEGUIAnimation_xmlHandler_h_
#define _CEGUIAnimation_xmlHandler_h_

#include "CEGUIChainedXMLHandler.h"
#include "CEGUIString.h"

// Start of CEGUI namespace section
namespace CEGUI
{
//! Class used to parse stand alone Animation XML files.
class CEGUIEXPORT Animation_xmlHandler : public ChainedXMLHandler
{
public:
    Animation_xmlHandler();
    ~Animation_xmlHandler();

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);
};

//----------------------------------------------------------------------------//
//! Chained sub-handler for AnimationDefinition XML elements
class CEGUIEXPORT AnimationDefinitionHandler : public ChainedXMLHandler
{
public:
    //! String holding the element handled by this class.
    static const String ElementName;

    AnimationDefinitionHandler(const XMLAttributes& attributes,
                               const String& name_prefix);
    ~AnimationDefinitionHandler();

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);

    //! Pointer to the Animation created by this handler.
    Animation* d_anim;
};

//----------------------------------------------------------------------------//
//! Chained sub-handler for Affector XML elements
class CEGUIEXPORT AnimationAffectorHandler : public ChainedXMLHandler
{
public:
    //! String holding the element handled by this class.
    static const String ElementName;

    AnimationAffectorHandler(const XMLAttributes& attributes,
                             Animation& anim);
    ~AnimationAffectorHandler();

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);

    //! Affector created by this handler.
    Affector* d_affector;
};

//----------------------------------------------------------------------------//
//! Chained sub-handler for KeyFrame XML elements.
class CEGUIEXPORT AnimationKeyFrameHandler : public ChainedXMLHandler
{
public:
    //! String holding the element handled by this class.
    static const String ElementName;

    AnimationKeyFrameHandler(const XMLAttributes& attributes,
                             Affector& affector);
    ~AnimationKeyFrameHandler();

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);
};

//----------------------------------------------------------------------------//
//! Chained sub-handler for Subscription XML elements.
class CEGUIEXPORT AnimationSubscriptionHandler : public ChainedXMLHandler
{
public:
    //! String holding the element handled by this class.
    static const String ElementName;

    AnimationSubscriptionHandler(const XMLAttributes& attributes,
                                 Animation& anim);
    ~AnimationSubscriptionHandler();

protected:
    // implement ChainedXMLHandler interface.
    void elementStartLocal(const String& element,
                           const XMLAttributes& attributes);
    void elementEndLocal(const String& element);
};

} // End of  CEGUI namespace section

#endif  // end of guard _CEGUIAnimation_xmlHandler_h_

