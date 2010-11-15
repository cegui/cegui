SUBDIRS = . ImageCodecModules RendererModules ScriptingModules WindowRendererSets XMLParserModules

INCLUDES = -I$(top_srcdir)/cegui/include $(freetype2_CFLAGS) $(pcre_CFLAGS) \
           $(fribidi_CFLAGS) $(ZLIB_CFLAGS)

libCEGUIBase@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@ \
                                      $(freetype2_LIBS) $(pcre_LIBS) \
                                      $(fribidi_LIBS) $(ZLIB_LIBS)

libCEGUIBase@cegui_bsfx@_la_LIBADD = $(cegui_pc_platform_libs)

if CEGUI_BUILD_STATIC
libCEGUIBase@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIBase@cegui_bsfx@_la_CPPFLAGS = -DCEGUIBASE_EXPORTS
endif
endif

lib_LTLIBRARIES=libCEGUIBase@cegui_bsfx@.la

libCEGUIBase@cegui_bsfx@_la_SOURCES = \
                      CEGUIAffector.cpp \
                      CEGUIAnimation.cpp \
                      CEGUIAnimationInstance.cpp \
                      CEGUIAnimationManager.cpp \
                      CEGUIAnimation_xmlHandler.cpp \
                      CEGUIBase.cpp \
                      CEGUIBasicInterpolators.cpp \
                      CEGUIBasicRenderedStringParser.cpp \
                      CEGUIBiDiVisualMapping.cpp \
                      CEGUIBoundSlot.cpp \
                      CEGUICentredRenderedString.cpp \
                      CEGUIChainedXMLHandler.cpp \
                      CEGUIColourRect.cpp \
                      CEGUIConfig_xmlHandler.cpp \
                      CEGUICoordConverter.cpp \
                      CEGUIDataContainer.cpp \
                      CEGUIDefaultRenderedStringParser.cpp \
                      CEGUIDefaultResourceProvider.cpp \
                      CEGUIDynamicModule.cpp \
                      CEGUIEvent.cpp \
                      CEGUIEventArgs.cpp \
                      CEGUIEventSet.cpp \
                      CEGUIEventSignal.cpp \
                      CEGUIEventSignalSet.cpp \
                      CEGUIExceptions.cpp \
                      CEGUIFactoryModule.cpp \
                      CEGUIFont.cpp \
                      CEGUIFontManager.cpp \
                      CEGUIFontProperties.cpp \
                      CEGUIFont_xmlHandler.cpp \
                      CEGUIFormattedRenderedString.cpp \
					  CEGUIGeometryBuffer.cpp \
                      CEGUIGlobalEventSet.cpp \
                      CEGUIGUILayout_xmlHandler.cpp \
                      CEGUIImage.cpp \
					  CEGUIImageCodec.cpp \
                      CEGUIImageset.cpp \
                      CEGUIImagesetManager.cpp \
                      CEGUIImageset_xmlHandler.cpp \
                      CEGUIJustifiedRenderedString.cpp \
                      CEGUIJustifiedRenderedStringWordWrapper.cpp \
                      CEGUILeftAlignedRenderedString.cpp \
                      CEGUIKeyFrame.cpp \
                      CEGUILogger.cpp \
                      CEGUIMouseCursor.cpp \
                      CEGUINamedXMLResourceManager.cpp \
                      CEGUIPixmapFont.cpp \
                      CEGUIProperty.cpp \
                      CEGUIPropertyHelper.cpp \
                      CEGUIPropertySet.cpp \
                      CEGUIRect.cpp \
                      CEGUIRenderedString.cpp \
                      CEGUIRenderedStringComponent.cpp \
                      CEGUIRenderedStringImageComponent.cpp \
                      CEGUIRenderedStringTextComponent.cpp \
                      CEGUIRenderedStringWidgetComponent.cpp \
                      CEGUIRenderingRoot.cpp \
                      CEGUIRenderingSurface.cpp \
                      CEGUIRenderingWindow.cpp \
					  CEGUIRenderEffectManager.cpp \
                      CEGUIRenderQueue.cpp \
                      CEGUIRightAlignedRenderedString.cpp \
                      CEGUIScheme.cpp \
                      CEGUISchemeManager.cpp \
                      CEGUIScheme_xmlHandler.cpp \
                      CEGUIScriptModule.cpp \
                      CEGUISize.cpp \
                      CEGUIString.cpp \
                      CEGUISubscriberSlot.cpp \
                      CEGUISystem.cpp \
                      CEGUITextUtils.cpp \
                      CEGUIWindow.cpp \
                      CEGUIWindowFactory.cpp \
                      CEGUIWindowFactoryManager.cpp \
                      CEGUIWindowManager.cpp \
                      CEGUIWindowProperties.cpp \
                      CEGUIWindowRenderer.cpp \
                      CEGUIWindowRendererManager.cpp \
                      CEGUIWindowRendererModule.cpp \
                      CEGUIWRFactoryRegisterer.cpp \
                      CEGUIXMLAttributes.cpp \
                      CEGUIXMLHandler.cpp \
                      CEGUIXMLParser.cpp \
                      CEGUIXMLSerializer.cpp \
                      CEGUIcolour.cpp \
                      elements/CEGUIButtonBase.cpp \
                      elements/CEGUICheckbox.cpp \
                      elements/CEGUIClippedContainer.cpp \
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
                      elements/CEGUIGridLayoutContainer.cpp \
                      elements/CEGUIGridLayoutContainerProperties.cpp \
                      elements/CEGUIGroupBox.cpp \
                      elements/CEGUIGUISheet.cpp \
                      elements/CEGUIHorizontalLayoutContainer.cpp \
                      elements/CEGUIItemEntry.cpp \
                      elements/CEGUIItemEntryProperties.cpp \
                      elements/CEGUIItemListBase.cpp \
                      elements/CEGUIItemListBaseProperties.cpp \
                      elements/CEGUIItemListbox.cpp \
                      elements/CEGUIItemListboxProperties.cpp \
                      elements/CEGUILayoutContainer.cpp \
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
                      elements/CEGUIMenuItemProperties.cpp \
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
                      elements/CEGUIScrolledItemListBase.cpp \
                      elements/CEGUIScrolledItemListBaseProperties.cpp \
					  elements/CEGUISequentialLayoutContainer.cpp \
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
                      elements/CEGUITree.cpp \
                      elements/CEGUITreeProperties.cpp \
                      elements/CEGUITreeItem.cpp \
                      elements/CEGUIVerticalLayoutContainer.cpp \
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
                      falagard/CEGUIFalagard_xmlHandler.cpp

if CEGUI_BUILD_FREETYPE_FONT
libCEGUIBase@cegui_bsfx@_la_SOURCES += CEGUIFreeTypeFont.cpp
endif

if CEGUI_BUILD_DEFAULT_LOGGER
libCEGUIBase@cegui_bsfx@_la_SOURCES += CEGUIDefaultLogger.cpp
endif

if CEGUI_BUILD_PCRE_REGEX
libCEGUIBase@cegui_bsfx@_la_SOURCES += CEGUIPCRERegexMatcher.cpp
endif

if CEGUI_BUILD_FRIBIDI_MAPPER
libCEGUIBase@cegui_bsfx@_la_SOURCES += CEGUIFribidiVisualMapping.cpp
endif

if CEGUI_BUILD_MINIBIDI_MAPPER
libCEGUIBase@cegui_bsfx@_la_SOURCES += CEGUIMinibidiVisualMapping.cpp
endif

if CEGUI_BUILD_MINIZIP_RESOURCE_PROVIDER
libCEGUIBase@cegui_bsfx@_la_SOURCES += CEGUIMinizipResourceProvider.cpp minizip/ioapi.cpp minizip/unzip.cpp
endif

if MACOSX_BUILD
INCLUDES += -I$(top_srcdir)/src/implementations/mac
libCEGUIBase@cegui_bsfx@_la_SOURCES += implementations/mac/macPlugins.cpp
endif

EXTRA_libCEGUIBase@cegui_bsfx@_la_SOURCES = minibidi.cpp
EXTRA_DIST = implementations/mac/macPlugins.h \
             minizip/ioapi.h minizip/iowin32.cpp minizip/iowin32.h minizip/unzip.h minizip/README

