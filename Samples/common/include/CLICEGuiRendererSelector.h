/************************************************************************
    filename:   CLICEGuiRendererSelector.h
    created:    5/3/2005
    author:     Paul D Turner
*************************************************************************/
/*************************************************************************
    Crazy Eddie's GUI System (http://crayzedsgui.sourceforge.net)
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
#ifndef _CLICEGuiRendererSelector_h_
#define _CLICEGuiRendererSelector_h_

#include "CEGuiRendererSelector.h"


class CLICEGuiRendererSelector : public CEGuiRendererSelector
{
public:
    /*!
    \brief
        Constructor.
    */
    CLICEGuiRendererSelector();

    /*!
    \brief
        Destructor.
    */
    ~CLICEGuiRendererSelector();

    bool inkokeDialog();
};

#endif  // end of guard _CLICEGuiRendererSelector_h_
