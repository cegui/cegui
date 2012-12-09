INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/RapidXMLParser \
		   $(rapidxml_CFLAGS)

libCEGUIRapidXMLParser@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIRapidXMLParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIRapidXMLParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUIRAPIDXMLPARSER_EXPORTS
endif
endif

lib_LTLIBRARIES=libCEGUIRapidXMLParser@cegui_bsfx@.la
libCEGUIRapidXMLParser@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

libCEGUIRapidXMLParser@cegui_bsfx@_la_SOURCES = CEGUIRapidXMLParser.cpp \
                                   CEGUIRapidXMLParserModule.cpp

