INCLUDES = -I$(top_srcdir)/include $(expat_CFLAGS)

if MINGW32_BUILD
libCEGUIExpatParser_la_CPPFLAGS = -DCEGUIEXPATPARSER_EXPORTS
libCEGUIExpatParser_la_LDFLAGS = -no-undefined -avoid-version
endif

libCEGUIExpatParser_la_LIBADD = $(expat_LIBS) $(top_builddir)/src/libCEGUIBase.la


lib_LTLIBRARIES=libCEGUIExpatParser.la

libCEGUIExpatParser_la_SOURCES = CEGUIExpatParser.cpp \
                                 CEGUIExpatParserModule.cpp
pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/ExpatParser
pkginclude_HEADERS = CEGUIExpatParser.h CEGUIExpatParserModule.h
