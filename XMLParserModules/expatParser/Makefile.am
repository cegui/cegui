INCLUDES = -I$(top_srcdir)/include $(expat_CFLAGS)
lib_LTLIBRARIES=libCEGUIExpatParser.la

libCEGUIExpatParser_la_SOURCES = CEGUIExpatParser.cpp \
                                 CEGUIExpatParserModule.cpp
pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/ExpatParser
pkginclude_HEADERS = CEGUIExpatParser.h CEGUIExpatParserModule.h

libCEGUIExpatParser_la_LIBADD = $(expat_LIBS)
