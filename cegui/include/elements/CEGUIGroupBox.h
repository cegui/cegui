/************************************************************************
	filename: 	CEGUIGroupBox.h
	created:	03/23/2007
	author:		Lars 'Levia' Wesselius (Content Pane based on Tomas Lindquist Olsen's code)

	purpose:	Interface for the GroupBox widget
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
#ifndef _CEGUIGroupBox_h_
#define _CEGUIGroupBox_h_

#include "CEGUIBase.h"
#include "CEGUIWindowFactory.h"
#include "elements/CEGUIClippedContainer.h"
#include <vector>


#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif


// Start of CEGUI namespace section
namespace CEGUI
{

	/*!
	\brief
		Base class for standard GroupBox widget.
	*/
	class CEGUIEXPORT GroupBox : public Window
	{
	protected:

	public:
		static const String EventNamespace;					//!< Namespace for global events


		/*************************************************************************
		Constants
		*************************************************************************/
		// temp
		static const String WidgetTypeName;

		static const String ContentPaneNameSuffix;


		/*************************************************************************
		Construction and Destruction
		*************************************************************************/
		/*!
		\brief
			Constructor for GroupBox class.
		*/
		GroupBox(const String& type, const String& name);


		/*!
		\brief
			Destructor for GroupBox class.
		*/
		virtual ~GroupBox();

		/*!
		\brief
			Draws the GroupBox around a widget. The size and position of the GroupBox are overriden.
			 Once the window that is drawn around resizes, you'll have to call the function again. FIXME
		*/
		bool drawAroundWidget(const CEGUI::Window * wnd);
		bool drawAroundWidget(const String& name);

		/*!
		\brief
			Return whether this window was inherited from the given class name at some point in the inheritance hierarchy.

		\param class_name
			The class name that is to be checked.

		\return
			true if this window was inherited from \a class_name. false if not.
		*/
		virtual bool testClassName_impl(const String& class_name) const
		{
			if (class_name=="GroupBox") return true;
			return Window::testClassName_impl(class_name);
		}

		/*!
		\brief
			Returns the content pane held by this GroupBox.

		\return
			Pointer to a Window instance.
		*/
		Window * getContentPane() const;
			

	protected:
		

		// Overridden from Window
		/*!
		\brief
			Initializes the components necessary.
		*/
		virtual void initialiseComponents();

		/*!
		\brief
			Add given window to child list at an appropriate position.
		*/
		virtual void addChild_impl(Window* wnd);

		/*!
		\brief
			Remove our child again when necessary.
		*/
		virtual void removeChild_impl(Window* wnd);


	};

} // End of  CEGUI namespace section


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _CEGUIGroupBox_h_
