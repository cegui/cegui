SUBDIRS = . renderers

INCLUDES = -I$(top_srcdir)/include $(freetype2_CFLAGS)
lib_LTLIBRARIES=libCEGUIBase.la

libCEGUIBase_la_SOURCES = CEGUIBase.cpp \
                      CEGUIColourRect.cpp \
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
                      elements/CEGUICheckbox.cpp \
                      elements/CEGUICombobox.cpp \
                      elements/CEGUIComboDropList.cpp \
                      elements/CEGUIEditbox.cpp \
                      elements/CEGUIFrameWindow.cpp \
                      elements/CEGUIListbox.cpp \
                      elements/CEGUIListboxItem.cpp \
                      elements/CEGUIListboxTextItem.cpp \
                      elements/CEGUIListHeader.cpp \
                      elements/CEGUIListHeaderSegment.cpp \
                      elements/CEGUIMultiColumnList.cpp \
		      elements/CEGUIMultiLineEditbox.cpp \
                      elements/CEGUIProgressBar.cpp \
                      elements/CEGUIPushButton.cpp \
                      elements/CEGUIRadioButton.cpp \
                      elements/CEGUIScrollbar.cpp \
                      elements/CEGUISlider.cpp \
                      elements/CEGUIStatic.cpp \
                      elements/CEGUIStaticImage.cpp \
                      elements/CEGUIStaticText.cpp \
                      elements/CEGUIThumb.cpp \
                      elements/CEGUITitlebar.cpp \
                      implementations/linux/CEGUIFactoryModule.cpp

libCEGUIBase_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@
libCEGUIBase_la_LIBADD = -lboost_regex -lboost_signals $(freetype2_LIBS)
