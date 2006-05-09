/************************************************************************
    filename:   FalStatic.h
    created:    Sat Sep 17 2005
    author:     Tomas Lindquist Olsen (based on code by Paul D Turner)
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)
 
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
#ifndef _FalStatic_h_
#define _FalStatic_h_

#include "FalModule.h"
#include "FalStaticProperties.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

// Start of CEGUI namespace section
namespace CEGUI
{
    /*!
    \brief
        Static class for the FalagardBase module.

        This class requires LookNFeel to be assigned.  The LookNFeel should provide the following:

        States:
            - Enabled                     - basic rendering for enabled state.
            - Disabled                    - basic rendering for disabled state.
            - EnabledFrame                - frame rendering for enabled state
            - DisabledFrame               - frame rendering for disabled state.
            - WithFrameEnabledBackground  - backdrop rendering for enabled state with frame enabled.
            - WithFrameDisabledBackground - backdrop rendering for disabled state with frame enabled.
            - NoFrameEnabledBackground    - backdrop rendering for enabled state with frame disabled.
            - NoFrameDisabledBackground   - backdrop rendering for disabled state with frame disabled.
    */
    class FALAGARDBASE_API FalagardStatic : public WindowRenderer
    {
    public:
        static const utf8   TypeName[];     //! type name for this widget.

        /*!
        \brief
            Constructor
        */
        FalagardStatic(const String& type);

        /*!
        \brief
            Return whether the frame for this static widget is enabled or disabled.

        \return
            true if the frame is enabled and will be rendered.  false is the frame is disabled and will not be rendered.
        */
        bool    isFrameEnabled(void) const        {return d_frameEnabled;}

        /*!
        \brief
            Return whether the background for this static widget is enabled to disabled.

        \return
            true if the background is enabled and will be rendered.  false if the background is disabled and will not be rendered.
        */
        bool    isBackgroundEnabled(void) const        {return d_backgroundEnabled;}

        /*!
        \brief
            Enable or disable rendering of the frame for this static widget.

        \param setting
            true to enable rendering of a frame.  false to disable rendering of a frame.
        */
        void    setFrameEnabled(bool setting);

        /*!
        \brief
            Enable or disable rendering of the background for this static widget.

        \param setting
            true to enable rendering of the background.  false to disable rendering of the background.
        */
        void    setBackgroundEnabled(bool setting);

        virtual void render();

    protected:
        // static properties
        static FalagardStaticProperties::FrameEnabled       d_frameEnabledProperty;
        static FalagardStaticProperties::BackgroundEnabled  d_backgroundEnabledProperty;

        // implementation data
        bool d_frameEnabled;        //!< True when the frame is enabled.
        bool d_backgroundEnabled;   //!< true when the background is enabled.
    };

} // End of  CEGUI namespace section

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif  // end of guard _FalStatic_h_
