INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/WindowRendererSets/Falagard

libCEGUIFalagardWRBase@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIFalagardWRBase@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIFalagardWRBase@cegui_bsfx@_la_CPPFLAGS = -DFALAGARDWRBASE_EXPORTS -DCEGUIWRMODULE_EXPORTS
endif
endif

libCEGUIFalagardWRBase@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIFalagardWRBase@cegui_bsfx@.la

libCEGUIFalagardWRBase@cegui_bsfx@_la_SOURCES = FalButton.cpp \
                                  FalToggleButton.cpp \
                                  FalDefault.cpp \
                                  FalEditbox.cpp \
                                  FalEditboxProperties.cpp \
                                  FalFrameWindow.cpp \
                                  FalItemEntry.cpp \
								  FalItemListbox.cpp \
                                  FalListHeader.cpp \
                                  FalListHeaderProperties.cpp \
                                  FalListHeaderSegment.cpp \
                                  FalListbox.cpp \
                                  FalMenubar.cpp \
                                  FalMenuItem.cpp \
                                  FalModule.cpp \
                                  FalMultiColumnList.cpp \
                                  FalMultiLineEditbox.cpp \
                                  FalMultiLineEditboxProperties.cpp \
                                  FalPopupMenu.cpp \
                                  FalProgressBar.cpp \
                                  FalProgressBarProperties.cpp \
                                  FalScrollablePane.cpp \
                                  FalScrollbar.cpp \
                                  FalScrollbarProperties.cpp \
                                  FalSlider.cpp \
                                  FalSliderProperties.cpp \
                                  FalStatic.cpp \
                                  FalStaticImage.cpp \
                                  FalStaticImageProperties.cpp \
                                  FalStaticProperties.cpp \
                                  FalStaticText.cpp \
                                  FalStaticTextProperties.cpp \
                                  FalSystemButton.cpp \
                                  FalTabButton.cpp \
                                  FalTabControl.cpp \
                                  FalTabControlProperties.cpp \
                                  FalTitlebar.cpp \
                                  FalTree.cpp \
                                  FalTooltip.cpp
