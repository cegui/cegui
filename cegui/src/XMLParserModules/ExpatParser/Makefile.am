INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/ExpatParser \
           $(expat_CFLAGS)

libCEGUIExpatParser@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIExpatParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIExpatParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUIEXPATPARSER_EXPORTS
endif
endif

libCEGUIExpatParser@cegui_bsfx@_la_LIBADD = $(expat_LIBS) \
                                $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la


lib_LTLIBRARIES=libCEGUIExpatParser@cegui_bsfx@.la

libCEGUIExpatParser@cegui_bsfx@_la_SOURCES = CEGUIExpatParser.cpp \
                                 CEGUIExpatParserModule.cpp
