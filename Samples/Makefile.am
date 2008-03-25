if CEGUI_BUILD_SAMPLES

if CEGUI_BUILD_LUA_MODULE
CEGUILUA_DEMOS=Demo8 InstancedWindows
endif

SUBDIRS = . \
          common \
          datafiles \
          Demo6 \
          Demo7 \
          FirstWindow \
          FalagardDemo1 \
          FontDemo \
          Minesweeper \
          ScrollablePaneDemo \
          TabControlDemo \
          TextDemo \
          TreeDemo \
          $(CEGUILUA_DEMOS)
endif

EXTRA_DIST = ReadMe.txt \
             bin/keepme

install-exec-local:
	if test ! -d $(top_builddir)/Samples/bin ; then \
		mkdir $(top_builddir)/Samples/bin ; \
	fi
