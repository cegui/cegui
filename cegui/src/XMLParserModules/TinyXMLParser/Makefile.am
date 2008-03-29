INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/TinyXMLParser

if MINGW32_BUILD
libCEGUITinyXMLParser_la_CPPFLAGS = -DCEGUITINYXMLPARSER_EXPORTS
libCEGUITinyXMLParser_la_LDFLAGS = -no-undefined -avoid-version
libCEGUITinyXMLParser_la_LIBADD = -L$(top_builddir)/src -lCEGUIBase
endif

lib_LTLIBRARIES=libCEGUITinyXMLParser.la

libCEGUITinyXMLParser_la_SOURCES = CEGUITinyXMLParser.cpp \
                                   CEGUITinyXMLParserModule.cpp \
                                   ceguitinyxml/tinystr.cpp \
                                   ceguitinyxml/tinyxml.cpp \
                                   ceguitinyxml/tinyxmlerror.cpp \
                                   ceguitinyxml/tinyxmlparser.cpp

noinst_HEADERS = ceguitinyxml/tinystr.h \
                 ceguitinyxml/tinyxml.h
