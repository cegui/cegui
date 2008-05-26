INCLUDES = -I$(top_srcdir)/include

if MINGW32_BUILD
libCEGUITinyXMLParser_la_CPPFLAGS = -DCEGUITINYXMLPARSER_EXPORTS
libCEGUITinyXMLParser_la_LDFLAGS = -no-undefined -avoid-version
endif

lib_LTLIBRARIES=libCEGUITinyXMLParser.la
libCEGUITinyXMLParser_la_LIBADD = $(top_builddir)/src/libCEGUIBase.la

libCEGUITinyXMLParser_la_SOURCES = CEGUITinyXMLParser.cpp \
                                   CEGUITinyXMLParserModule.cpp
if BUILD_USING_INTERNAL_TINYXML
libCEGUITinyXMLParser_la_SOURCES += ceguitinyxml/tinystr.cpp \
                                    ceguitinyxml/tinyxml.cpp \
                                    ceguitinyxml/tinyxmlerror.cpp \
                                    ceguitinyxml/tinyxmlparser.cpp
else
INCLUDES += $(tinyxml_CFLAGS)
libCEGUITinyXMLParser_la_LIBADD += $(tinyxml_LIBS)
endif

pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/TinyXMLParser
pkginclude_HEADERS = CEGUITinyXMLParser.h CEGUITinyXMLParserModule.h

noinst_HEADERS = ceguitinyxml/tinystr.h \
                 ceguitinyxml/tinyxml.h
