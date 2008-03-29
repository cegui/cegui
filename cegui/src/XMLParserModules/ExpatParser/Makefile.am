INCLUDES = -I$(top_srcdir)/cegui/include \
           -I$(top_srcdir)/cegui/include/XMLParserModules/ExpatParser \
           $(expat_CFLAGS)

libCEGUIExpatParser_la_LIBADD =

if MINGW32_BUILD
libCEGUIExpatParser_la_CPPFLAGS = -DCEGUIEXPATPARSER_EXPORTS
libCEGUIExpatParser_la_LDFLAGS = -no-undefined -avoid-version
libCEGUIExpatParser_la_LIBADD += -L$(top_builddir)/src -lCEGUIBase
endif

libCEGUIExpatParser_la_LIBADD += $(expat_LIBS)


lib_LTLIBRARIES=libCEGUIExpatParser.la

libCEGUIExpatParser_la_SOURCES = CEGUIExpatParser.cpp \
                                 CEGUIExpatParserModule.cpp
