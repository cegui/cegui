INCLUDES = -I$(top_srcdir)/cegui/include -I$(top_srcdir)/Samples/common/include

noinst_PROGRAMS = InventoryDemo@cegui_bsfx@
noinst_HEADERS = BoolArray2D.h \
                 InventoryBase.h \
                 InventoryItem.h \
                 InventoryItemRenderer.h \
                 InventoryReceiver.h \
                 Sample_InventoryDemo.h


if CEGUI_BUILD_STATIC
InventoryDemo@cegui_bsfx@_CPPFLAGS = -DCEGUI_STATIC
endif

InventoryDemo@cegui_bsfx@_SOURCES = BoolArray2D.cpp \
                                    InventoryBase.cpp \
                                    InventoryItem.cpp \
                                    InventoryItemRenderer.cpp \
                                    InventoryReceiver.cpp \
                                    Sample_InventoryDemo.cpp

InventoryDemo@cegui_bsfx@_LDADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la \
                    $(top_builddir)/Samples/common/src/libCEGUISampleHelper@cegui_bsfx@.la \
					$(SAMPLE_STATIC_LDFLAGS)

install-exec-local:
	$(LN_S) -f `pwd`/.libs/InventoryDemo@cegui_bsfx@$(EXEEXT) $(top_builddir)/bin/InventoryDemo@cegui_bsfx@$(EXEEXT)
