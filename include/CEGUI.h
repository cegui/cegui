/***********************************************************************
	filename: 	CEGUI.h
	created:	21/2/2004
	author:		Paul D Turner
	
	purpose:	Main system include for client code
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
#ifndef _CEGUI_h_
#define _CEGUI_h_

// base stuff
#include "CEGUIBase.h"
#include "CEGUIString.h"
#include "CEGUIRect.h"
#include "CEGUIExceptions.h"
#include "CEGUITexture.h"
#include "CEGUIRenderer.h"
#include "CEGUIImageset.h"
#include "CEGUIImagesetManager.h"
#include "CEGUILogger.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIFont.h"
#include "CEGUIFontManager.h"
#include "CEGUIEventArgs.h"
#include "CEGUIEvent.h"
#include "CEGUIEventSet.h"
#include "CEGUIGlobalEventSet.h"
#include "CEGUIProperty.h"
#include "CEGUIPropertySet.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindowRenderer.h"
#include "CEGUIWindowRendererManager.h"
#include "CEGUIScheme.h"
#include "CEGUISchemeManager.h"
#include "CEGUIScriptWindowHelper.h"
#include "CEGUISystem.h"
#include "CEGUIScriptModule.h"
#include "CEGUICoordConverter.h"
#include "CEGUIPropertyHelper.h"

// Falagard core system include
#include "falagard/CEGUIFalWidgetLookManager.h"

// gui elements
#include "elements/CEGUIButtonBase.h"
#include "elements/CEGUIPushButton.h"
#include "elements/CEGUICheckbox.h"
#include "elements/CEGUIRadioButton.h"
#include "elements/CEGUITitlebar.h"
#include "elements/CEGUIFrameWindow.h"
#include "elements/CEGUIProgressBar.h"
#include "elements/CEGUIEditbox.h"
#include "elements/CEGUIThumb.h"
#include "elements/CEGUISlider.h"
#include "elements/CEGUIScrollbar.h"
#include "elements/CEGUIListbox.h"
#include "elements/CEGUICombobox.h"
#include "elements/CEGUIListHeader.h"
#include "elements/CEGUIMultiColumnList.h"
#include "elements/CEGUIGUISheet.h"
#include "elements/CEGUIListboxTextItem.h"
#include "elements/CEGUIComboDropList.h"
#include "elements/CEGUIMultiLineEditbox.h"
#include "elements/CEGUITabButton.h"
#include "elements/CEGUITabControl.h"
#include "elements/CEGUISpinner.h"
#include "elements/CEGUIDragContainer.h"
#include "elements/CEGUIScrolledContainer.h"
#include "elements/CEGUIScrollablePane.h"
#include "elements/CEGUITooltip.h"
#include "elements/CEGUIItemEntry.h"
#include "elements/CEGUIMenuItem.h"
#include "elements/CEGUIItemListBase.h"
#include "elements/CEGUIMenuBase.h"
#include "elements/CEGUIMenubar.h"
#include "elements/CEGUIPopupMenu.h"
#include "elements/CEGUIScrolledItemListBase.h"
#include "elements/CEGUIItemListbox.h"
#include "elements/CEGUITree.h"
#include "elements/CEGUITreeItem.h"
#include "elements/CEGUIGroupBox.h"

#endif	// end of guard _CEGUI_h_
