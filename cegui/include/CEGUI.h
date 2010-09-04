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
#include "CEGUIAffector.h"
#include "CEGUIAnimation.h"
#include "CEGUIAnimationInstance.h"
#include "CEGUIAnimationManager.h"
#include "CEGUIBasicInterpolators.h"
#include "CEGUIBasicRenderedStringParser.h"
#include "CEGUIBiDiVisualMapping.h"
#include "CEGUICentredRenderedString.h"
#include "CEGUIcolour.h"
#include "CEGUIColourRect.h"
#include "CEGUICoordConverter.h"
#include "CEGUIDataContainer.h"
#include "CEGUIDefaultLogger.h"
#include "CEGUIDefaultRenderedStringParser.h"
#include "CEGUIDefaultResourceProvider.h"
#include "CEGUIDynamicModule.h"
#include "CEGUIExceptions.h"
#include "CEGUIFactoryModule.h"
#include "CEGUIFontManager.h"
#include "CEGUIGeometryBuffer.h"
#include "CEGUIGlobalEventSet.h"
#include "CEGUIImageCodec.h"
#include "CEGUIImagesetManager.h"
#include "CEGUIInputEvent.h"
#include "CEGUIInterpolator.h"
#include "CEGUIJustifiedRenderedString.h"
#include "CEGUIKeyFrame.h"
#include "CEGUILeftAlignedRenderedString.h"
#include "CEGUIMouseCursor.h"
#include "CEGUIPropertyHelper.h"
#include "CEGUIPropertySet.h"
#include "CEGUIRect.h"
#include "CEGUIRegexMatcher.h"
#include "CEGUIRenderedString.h"
#include "CEGUIRenderedStringImageComponent.h"
#include "CEGUIRenderedStringParser.h"
#include "CEGUIRenderedStringTextComponent.h"
#include "CEGUIRenderedStringWidgetComponent.h"
#include "CEGUIRenderedStringWordWrapper.h"
#include "CEGUIRenderEffect.h"
#include "CEGUIRenderEffectManager.h"
#include "CEGUIRenderer.h"
#include "CEGUIRenderingContext.h"
#include "CEGUIRenderingRoot.h"
#include "CEGUIRenderingSurface.h"
#include "CEGUIRenderingWindow.h"
#include "CEGUIRenderQueue.h"
#include "CEGUIRenderTarget.h"
#include "CEGUIRightAlignedRenderedString.h"
#include "CEGUISchemeManager.h"
#include "CEGUIScriptModule.h"
#include "CEGUISize.h"
#include "CEGUISystem.h"
#include "CEGUITexture.h"
#include "CEGUITextureTarget.h"
#include "CEGUITextUtils.h"
#include "CEGUITplWindowFactory.h"
#include "CEGUITplWindowRendererFactory.h"
#include "CEGUITplWRFactoryRegisterer.h"
#include "CEGUIVector.h"
#include "CEGUIVersion.h"
#include "CEGUIVertex.h"
#include "CEGUIWidgetModule.h"
#include "CEGUIWindowFactory.h"
#include "CEGUIWindowFactoryManager.h"
#include "CEGUIWindow.h"
#include "CEGUIWindowManager.h"
#include "CEGUIWindowRendererManager.h"
#include "CEGUIWindowRendererModule.h"
#include "CEGUIWRFactoryRegisterer.h"
#include "CEGUIXMLAttributes.h"
#include "CEGUIXMLHandler.h"
#include "CEGUIXMLParser.h"
#include "CEGUIXMLSerializer.h"

// Falagard core system include
#include "falagard/CEGUIFalWidgetLookManager.h"

// gui elements
#include "elements/CEGUIAll.h"

#endif	// end of guard _CEGUI_h_

