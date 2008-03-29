INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/XercesParser \
           $(xerces_CFLAGS)

libCEGUIXercesParser_la_LIBADD =

if MINGW32_BUILD
libCEGUIXercesParser_la_CPPFLAGS = -DCEGUIXERCESPARSER_EXPORTS
libCEGUIXercesParser_la_LDFLAGS = -no-undefined -avoid-version
libCEGUIXercesParser_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIXercesParser_la_LIBADD += $(xerces_LIBS)

lib_LTLIBRARIES=libCEGUIXercesParser.la

libCEGUIXercesParser_la_SOURCES = CEGUIXercesParser.cpp \
                                  CEGUIXercesParserModule.cpp
