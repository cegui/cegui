INCLUDES = -I$(top_srcdir)/include $(libxml_CFLAGS)

libCEGUILibxmlParser_la_LIBADD =

if MINGW32_BUILD
libCEGUILibxmlParser_la_CPPFLAGS = -DCEGUILIBXMLPARSER_EXPORTS
libCEGUILibxmlParser_la_LDFLAGS = -no-undefined -avoid-version
libCEGUILibxmlParser_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUILibxmlParser_la_LIBADD += $(libxml_LIBS)

lib_LTLIBRARIES=libCEGUILibxmlParser.la

libCEGUILibxmlParser_la_SOURCES = CEGUILibxmlParser.cpp \
                                  CEGUILibxmlParserModule.cpp

pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/LibxmlParser
pkginclude_HEADERS = CEGUILibxmlParser.h CEGUILibxmlParserModule.h
