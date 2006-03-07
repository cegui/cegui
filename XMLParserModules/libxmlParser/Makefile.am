INCLUDES = -I$(top_srcdir)/include $(libxml_CFLAGS)
lib_LTLIBRARIES=libCEGUILibxmlParser.la

libCEGUILibxmlParser_la_SOURCES = CEGUILibxmlParser.cpp \
                                  CEGUILibxmlParserModule.cpp

pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/LibxmlParser
pkginclude_HEADERS = CEGUILibxmlParser.h CEGUILibxmlParserModule.h

libCEGUILibxmlParser_la_LIBADD = $(libxml_LIBS)
