INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/XercesParser \
           $(xerces_CFLAGS)

if MINGW32_BUILD
libCEGUIXercesParser_la_CPPFLAGS = -DCEGUIXERCESPARSER_EXPORTS
libCEGUIXercesParser_la_LDFLAGS = -no-undefined -avoid-version
endif

libCEGUIXercesParser_la_LIBADD = $(xerces_LIBS) \
                                 $(top_builddir)/cegui/src/libCEGUIBase.la

lib_LTLIBRARIES=libCEGUIXercesParser.la

libCEGUIXercesParser_la_SOURCES = CEGUIXercesParser.cpp \
                                  CEGUIXercesParserModule.cpp
