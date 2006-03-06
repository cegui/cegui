INCLUDES = -I$(top_srcdir)/include $(xerces_CFLAGS)
lib_LTLIBRARIES=libCEGUIXercesParser.la

libCEGUIXercesParser_la_SOURCES = CEGUIXercesParser.cpp
pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/XercesParser
pkginclude_HEADERS = CEGUIXercesParser.h

libCEGUIXercesParser_la_LIBADD = $(xerces_LIBS)
