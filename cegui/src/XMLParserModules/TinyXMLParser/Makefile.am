INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/TinyXMLParser

libCEGUITinyXMLParser@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUITinyXMLParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUITinyXMLParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUITINYXMLPARSER_EXPORTS
endif
endif

lib_LTLIBRARIES=libCEGUITinyXMLParser@cegui_bsfx@.la
libCEGUITinyXMLParser@cegui_bsfx@_la_LIBADD = $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

libCEGUITinyXMLParser@cegui_bsfx@_la_SOURCES = CEGUITinyXMLParser.cpp \
                                   CEGUITinyXMLParserModule.cpp
if BUILD_USING_INTERNAL_TINYXML
libCEGUITinyXMLParser@cegui_bsfx@_la_SOURCES += ceguitinyxml/tinystr.cpp \
                                    ceguitinyxml/tinyxml.cpp \
                                    ceguitinyxml/tinyxmlerror.cpp \
                                    ceguitinyxml/tinyxmlparser.cpp
else
INCLUDES += $(tinyxml_CFLAGS)
libCEGUITinyXMLParser@cegui_bsfx@_la_LIBADD += $(tinyxml_LIBS)
endif

noinst_HEADERS = ceguitinyxml/tinystr.h \
                 ceguitinyxml/tinyxml.h
