SUBDIRS = . renderers

INCLUDES = -I$(top_srcdir)/include $(freetype2_CFLAGS)
lib_LTLIBRARIES=libCEGUIBase.la

libCEGUIBase_la_SOURCES = CEGUIBase.cpp \
                      CEGUIColourRect.cpp \
                      CEGUIConfig_xmlHandler.cpp \
                      CEGUICoordConverter.cpp \
                      CEGUIDefaultResourceProvider.cpp \
                      CEGUIEvent.cpp \
                      CEGUIEventArgs.cpp \
                      CEGUIEventSet.cpp \
                      CEGUIEventSignal.cpp \
                      CEGUIEventSignalSet.cpp \
                      CEGUIExceptions.cpp \
                      CEGUIFactoryModule.cpp \
                      CEGUIFont.cpp \
                      CEGUIFontManager.cpp \
                      CEGUIFont_xmlHandler.cpp \
                      CEGUIGlobalEventSet.cpp \
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
                      CEGUIRefPtr.cpp \
                      CEGUIRenderCache.cpp \
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
                      CEGUIXMLAttributes.cpp \
                      CEGUIXMLHandler.cpp \
                      CEGUIXMLParser.cpp \
                      CEGUIcolour.cpp \
                      elements/CEGUIButtonBase.cpp \
                      elements/CEGUICheckbox.cpp \
                      elements/CEGUICheckboxProperties.cpp \
                      elements/CEGUICombobox.cpp \
                      elements/CEGUIComboboxProperties.cpp \
                      elements/CEGUIComboDropList.cpp \
                      elements/CEGUIDragContainer.cpp \
                      elements/CEGUIDragContainerProperties.cpp \
                      elements/CEGUIEditbox.cpp \
                      elements/CEGUIEditboxProperties.cpp \
                      elements/CEGUIFrameWindow.cpp \
                      elements/CEGUIFrameWindowProperties.cpp \
                      elements/CEGUIGUISheet.cpp \
                      elements/CEGUIItemEntry.cpp \
                      elements/CEGUIItemListBase.cpp \
                      elements/CEGUIItemListBaseProperties.cpp \
                      elements/CEGUIListbox.cpp \
                      elements/CEGUIListboxItem.cpp \
                      elements/CEGUIListboxProperties.cpp \
                      elements/CEGUIListboxTextItem.cpp \
                      elements/CEGUIListHeader.cpp \
                      elements/CEGUIListHeaderProperties.cpp \
                      elements/CEGUIListHeaderSegment.cpp \
                      elements/CEGUIListHeaderSegmentProperties.cpp \
                      elements/CEGUIMenubar.cpp \
                      elements/CEGUIMenuBase.cpp \
                      elements/CEGUIMenuBaseProperties.cpp \
                      elements/CEGUIMenuItem.cpp \
                      elements/CEGUIMultiColumnList.cpp \
                      elements/CEGUIMultiColumnListProperties.cpp \
                      elements/CEGUIMultiLineEditbox.cpp \
                      elements/CEGUIMultiLineEditboxProperties.cpp \
                      elements/CEGUIPopupMenu.cpp \
                      elements/CEGUIPopupMenuProperties.cpp \
                      elements/CEGUIProgressBar.cpp \
                      elements/CEGUIProgressBarProperties.cpp \
                      elements/CEGUIPushButton.cpp \
                      elements/CEGUIRadioButton.cpp \
                      elements/CEGUIRadioButtonProperties.cpp \
                      elements/CEGUIScrollablePane.cpp \
                      elements/CEGUIScrollablePaneProperties.cpp \
                      elements/CEGUIScrollbar.cpp \
                      elements/CEGUIScrollbarProperties.cpp \
                      elements/CEGUIScrolledContainer.cpp \
                      elements/CEGUIScrolledContainerProperties.cpp \
                      elements/CEGUISlider.cpp \
                      elements/CEGUISliderProperties.cpp \
                      elements/CEGUISpinner.cpp \
                      elements/CEGUISpinnerProperties.cpp \
                      elements/CEGUITabButton.cpp \
                      elements/CEGUITabControl.cpp \
                      elements/CEGUITabControlProperties.cpp \
                      elements/CEGUIThumb.cpp \
                      elements/CEGUIThumbProperties.cpp \
                      elements/CEGUITitlebar.cpp \
                      elements/CEGUITitlebarProperties.cpp \
                      elements/CEGUITooltip.cpp \
                      elements/CEGUITooltipProperties.cpp \
                      falagard/CEGUIFalComponentBase.cpp \
                      falagard/CEGUIFalDimensions.cpp \
                      falagard/CEGUIFalFrameComponent.cpp \
                      falagard/CEGUIFalImageryComponent.cpp \
                      falagard/CEGUIFalImagerySection.cpp \
                      falagard/CEGUIFalLayerSpecification.cpp \
                      falagard/CEGUIFalNamedArea.cpp \
                      falagard/CEGUIFalPropertyDefinition.cpp \
                      falagard/CEGUIFalPropertyDefinitionBase.cpp \
                      falagard/CEGUIFalPropertyLinkDefinition.cpp \
                      falagard/CEGUIFalPropertyInitialiser.cpp \
                      falagard/CEGUIFalSectionSpecification.cpp \
                      falagard/CEGUIFalStateImagery.cpp \
                      falagard/CEGUIFalTextComponent.cpp \
                      falagard/CEGUIFalWidgetComponent.cpp \
                      falagard/CEGUIFalWidgetLookFeel.cpp \
                      falagard/CEGUIFalWidgetLookManager.cpp \
                      falagard/CEGUIFalXMLEnumHelper.cpp \
                      falagard/CEGUIFalagard_xmlHandler.cpp \
                      pcre/chartables.c \
                      pcre/get.c \
                      pcre/maketables.c \
                      pcre/pcre.c \
                      pcre/pcreposix.c \
                      pcre/study.c

noinst_HEADERS =      pcre/internal.h \
                      pcre/ucpinternal.h \
                      pcre/pcre.h \
                      pcre/pcreposix.h \
                      pcre/ucp.h \
                      pcre/pcreconfig.h

if CEGUI_USING_TINYXML
libCEGUIBase_la_SOURCES += CEGUITinyXMLParser.cpp \
                      tinyxml/tinystr.cpp \
                      tinyxml/tinyxml.cpp \
                      tinyxml/tinyxmlerror.cpp \
                      tinyxml/tinyxmlparser.cpp

noinst_HEADERS += tinyxml/tinystr.h \
                  tinyxml/tinyxml.h
endif

if CEGUI_USING_XERCES
libCEGUIBase_la_SOURCES += CEGUIXercesParser.cpp
endif

libCEGUIBase_la_LDFLAGS = -version-info @CEGUI_VERSION_INFO@
libCEGUIBase_la_LIBADD = $(freetype2_LIBS)
