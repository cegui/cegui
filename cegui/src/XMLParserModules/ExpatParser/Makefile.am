INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/ExpatParser \
           $(expat_CFLAGS)

if MINGW32_BUILD
libCEGUIExpatParser_la_CPPFLAGS = -DCEGUIEXPATPARSER_EXPORTS
libCEGUIExpatParser_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUIExpatParser_la_LDFLAGS = -release @PACKAGE_VERSION@
endif

libCEGUIExpatParser_la_LIBADD = $(expat_LIBS) \
                                $(top_builddir)/cegui/src/libCEGUIBase.la


lib_LTLIBRARIES=libCEGUIExpatParser.la

libCEGUIExpatParser_la_SOURCES = CEGUIExpatParser.cpp \
                                 CEGUIExpatParserModule.cpp
