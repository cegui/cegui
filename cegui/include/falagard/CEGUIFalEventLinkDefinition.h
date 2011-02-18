/***********************************************************************
    filename:   CEGUIFalEventLinkDefinition.h
    created:    Sun Nov 21 2010
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
#ifndef _CEGUIFalEventLinkDefinition_h_
#define _CEGUIFalEventLinkDefinition_h_

#include "../CEGUIString.h"

#include <vector>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
class CEGUIEXPORT EventLinkDefinition
{
public:
    EventLinkDefinition(const String& event_name);
    ~EventLinkDefinition();

    //! add a new link target to \a event on \a widget (name suffix).
    void addLinkTarget(const String& widget, const String& event);
    //! clear all link targets from this link definition.
    void clearLinkTargets();

    //! initialise \a window with an event link as specified here.
    void initialiseWidget(Window& window) const;

    //! clean this event from \a window.
    void cleanUpWidget(Window& window) const;

    //! return the name of the Event defined here.
    const String& getName() const;

protected:
    //! Return a pointer to the target window with the given suffix.
    Window* getTargetWindow(Window& start_wnd,
                            const String& name_suffix) const;

    //! Internal struct used to keep track of target events.
    struct Target
    {
        //! name suffix of the target widget.
        String d_widgetNameSuffix;
        //! the event to link with on the target widget.
        String d_targetEvent;
    };

    //! String holding the name of the event being defined
    String d_eventName;

    //! type used for the collection of target events.
    typedef std::vector<Target
        CEGUI_VECTOR_ALLOC(Target)> LinkTargetCollection;

    //! collection of targets for this EventLinkDefinition.
    LinkTargetCollection d_targets;
};

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif

#endif  // end of guard _CEGUIFalEventLinkDefinition_h_

