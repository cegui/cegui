INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/LibxmlParser \
           $(libxml_CFLAGS)

if MINGW32_BUILD
libCEGUILibxmlParser_la_CPPFLAGS = -DCEGUILIBXMLPARSER_EXPORTS
libCEGUILibxmlParser_la_LDFLAGS = -no-undefined -avoid-version
else
libCEGUILibxmlParser_la_LDFLAGS = -release @PACKAGE_VERSION@
endif

libCEGUILibxmlParser_la_LIBADD = $(libxml_LIBS) \
                                 $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUILibxmlParser.la

libCEGUILibxmlParser_la_SOURCES = CEGUILibxmlParser.cpp \
                                  CEGUILibxmlParserModule.cpp
