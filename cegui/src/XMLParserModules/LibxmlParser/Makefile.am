INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/LibxmlParser \
           $(libxml_CFLAGS)

libCEGUILibxmlParser@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUILibxmlParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUILibxmlParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUILIBXMLPARSER_EXPORTS
endif
endif

libCEGUILibxmlParser@cegui_bsfx@_la_LIBADD = $(libxml_LIBS) \
                                 $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUILibxmlParser@cegui_bsfx@.la

libCEGUILibxmlParser@cegui_bsfx@_la_SOURCES = CEGUILibxmlParser.cpp \
                                  CEGUILibxmlParserModule.cpp
