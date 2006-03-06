INCLUDES = -I$(top_srcdir)/include $(expat_CFLAGS)
lib_LTLIBRARIES=libCEGUIExpatParser.la

libCEGUIExpatParser_la_SOURCES = CEGUIExpatParser.cpp
pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/ExpatParser
pkginclude_HEADERS = CEGUIExpatParser.h

libCEGUIExpatParser_la_LIBADD = $(expat_LIBS)
