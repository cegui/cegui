SUBDIRS = . renderers

INCLUDES = -I$(top_srcdir)/include $(freetype2_CFLAGS)
lib_LTLIBRARIES=libCEGUIBase.la

libCEGUIBase_la_SOURCES = CEGUIBase.cpp \
		      CEGUIColourManipulator.cpp \
                      CEGUIColourRect.cpp \
		      CEGUIConfig_xmlHandler.cpp \
                      CEGUIEvent.cpp \
                      CEGUIEventArgs.cpp \
                      CEGUIEventSet.cpp \
                      CEGUIEventSignal.cpp \
                      CEGUIEventSignalSet.cpp \
                      CEGUIExceptions.cpp \
                      CEGUIFont.cpp \
                      CEGUIFontManager.cpp \
                      CEGUIFont_xmlHandler.cpp \
		      CEGUIGUILayout_xmlHandler.cpp \
                      CEGUIImage.cpp \
                      CEGUIImageset.cpp \
                      CEGUIImagesetManager.cpp \
                      CEGUIImageset_xmlHandler.cpp \
                      CEGUILogger.cpp \
                      CEGUIMouseCursor.cpp \
                      CEGUIProperty.cpp \
		      CEGUIPropertyHelper.cpp \
                      CEGUIPropertySet.cpp \
                      CEGUIRect.cpp \
                      CEGUIRenderableElement.cpp \
                      CEGUIRenderableFrame.cpp \
                      CEGUIRenderableImage.cpp \
                      CEGUIRenderer.cpp \
                      CEGUIScheme.cpp \
                      CEGUISchemeManager.cpp \
                      CEGUIScheme_xmlHandler.cpp \
		      CEGUIScriptModule.cpp \
                      CEGUISize.cpp \
                      CEGUIString.cpp \
                      CEGUISystem.cpp \
                      CEGUITextUtils.cpp \
                      CEGUITexture.cpp \
                      CEGUIVector.cpp \
                      CEGUIWindow.cpp \
                      CEGUIWindowFactory.cpp \
                      CEGUIWindowFactoryManager.cpp \
                      CEGUIWindowManager.cpp \
		      CEGUIWindowProperties.cpp \
		      CEGUIXmlHandlerHelper.cpp\
                      elements/CEGUIButtonBase.cpp \
		      elements/CEGUIButtonBaseProperties.cpp \
                      elements/CEGUICheckbox.cpp \
		      elements/CEGUICheckboxProperties.cpp \
                      elements/CEGUICombobox.cpp \
                      elements/CEGUIComboboxProperties.cpp \
                      elements/CEGUIComboDropList.cpp \
                      elements/CEGUIEditbox.cpp \
		      elements/CEGUIEditboxProperties.cpp \
                      elements/CEGUIFrameWindow.cpp \
                      elements/CEGUIFrameWindowProperties.cpp \
                      elements/CEGUIListbox.cpp \
                      elements/CEGUIListboxItem.cpp \
                      elements/CEGUIListboxProperties.cpp \
                      elements/CEGUIListboxTextItem.cpp \
                      elements/CEGUIListHeader.cpp \
                      elements/CEGUIListHeaderProperties.cpp \
                      elements/CEGUIListHeaderSegment.cpp \
                      elements/CEGUIListHeaderSegmentProperties.cpp \
                      elements/CEGUIMultiColumnList.cpp \
                      elements/CEGUIMultiColumnListProperties.cpp \
		      elements/CEGUIMultiLineEditbox.cpp \
		      elements/CEGUIMultiLineEditboxProperties.cpp \
                      elements/CEGUIProgressBar.cpp \
                      elements/CEGUIProgressBarProperties.cpp \
                      elements/CEGUIPushButton.cpp \
                      elements/CEGUIRadioButton.cpp \
                      elements/CEGUIRadioButtonProperties.cpp \
                      elements/CEGUIScrollbar.cpp \
                      elements/CEGUIScrollbarProperties.cpp \
                      elements/CEGUISlider.cpp \
                      elements/CEGUISliderProperties.cpp \
                      elements/CEGUIStatic.cpp \
                      elements/CEGUIStaticImage.cpp \
                      elements/CEGUIStaticImageProperties.cpp \
                      elements/CEGUIStaticProperties.cpp \
                      elements/CEGUIStaticText.cpp \
                      elements/CEGUIStaticTextProperties.cpp \
                      elements/CEGUIThumb.cpp \
                      elements/CEGUIThumbProperties.cpp \
                      elements/CEGUITitlebar.cpp \
                      elements/CEGUITitlebarProperties.cpp \
                      implementations/linux/CEGUIFactoryModule.cpp

libCEGUIBase_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@
libCEGUIBase_la_LIBADD = -lboost_regex -lboost_signals $(freetype2_LIBS)
