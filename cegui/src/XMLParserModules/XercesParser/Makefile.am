INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/XercesParser \
           $(xerces_CFLAGS)

libCEGUIXercesParser@cegui_bsfx@_la_LDFLAGS = @CEGUI_LIB_LINK_FLAGS@

if CEGUI_BUILD_STATIC
libCEGUIXercesParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUI_STATIC
else
if MINGW32_BUILD
libCEGUIXercesParser@cegui_bsfx@_la_CPPFLAGS = -DCEGUIXERCESPARSER_EXPORTS
endif
endif

libCEGUIXercesParser@cegui_bsfx@_la_LIBADD = $(xerces_LIBS) \
                                 $(top_builddir)/cegui/src/libCEGUIBase@cegui_bsfx@.la

lib_LTLIBRARIES=libCEGUIXercesParser@cegui_bsfx@.la

libCEGUIXercesParser@cegui_bsfx@_la_SOURCES = CEGUIXercesParser.cpp \
                                  CEGUIXercesParserModule.cpp \
                                  CEGUIXercesParserProperties.cpp
