INCLUDES = -I$(top_srcdir)/include $(xerces_CFLAGS)
lib_LTLIBRARIES=libCEGUIXercesParser.la

libCEGUIXercesParser_la_SOURCES = CEGUIXercesParser.cpp \
                                  CEGUIXercesParserModule.cpp
pkgincludedir=$(prefix)/include/CEGUI/XMLParserModules/XercesParser
pkginclude_HEADERS = CEGUIXercesParser.h CEGUIXercesParserModule.h

libCEGUIXercesParser_la_LIBADD = $(xerces_LIBS)
