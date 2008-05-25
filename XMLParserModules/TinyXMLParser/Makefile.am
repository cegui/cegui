INCLUDES = -I$(top_srcdir)/include

if MINGW32_BUILD
libCEGUITinyXMLParser_la_CPPFLAGS = -DCEGUITINYXMLPARSER_EXPORTS
libCEGUITinyXMLParser_la_LDFLAGS = -no-undefined -avoid-version
endif

lib_LTLIBRARIES=libCEGUITinyXMLParser.la

libCEGUITinyXMLParser_la_SOURCES = CEGUITinyXMLParser.cpp \
                                   CEGUITinyXMLParserModule.cpp \
                                   ceguitinyxml/tinystr.cpp \
                                   ceguitinyxml/tinyxml.cpp \
                                   ceguitinyxml/tinyxmlerror.cpp \
                                   ceguitinyxml/tinyxmlparser.cpp
libCEGUITinyXMLParser_la_LIBADD = $(top_builddir)/src/libCEGUIBase.la

pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/TinyXMLParser
pkginclude_HEADERS = CEGUITinyXMLParser.h CEGUITinyXMLParserModule.h

noinst_HEADERS = ceguitinyxml/tinystr.h \
                 ceguitinyxml/tinyxml.h
