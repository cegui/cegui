/************************************************************************
	filename: 	CEGUIGroupBox.h
	created:	03/23/2007
	author:		Lars 'Levia' Wesselius (Content Pane based on Tomas Lindquist Olsen's code)

	purpose:	Interface for the GroupBox widget
*************************************************************************/
/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
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
#ifndef _CEGUIGroupBox_h_
#define _CEGUIGroupBox_h_

#include "../CEGUIBase.h"
#include "../CEGUIWindowFactory.h"
#include "CEGUIClippedContainer.h"
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
