if CEGUI_BUILD_SAMPLES

if CEGUI_BUILD_LUA_MODULE
CEGUILUA_DEMOS=Demo8
endif

SUBDIRS = . \
          common \
          Demo6 \
          Demo7 \
          DragDropDemo \
          FalagardDemo1 \
          FirstWindow \
          FontDemo \
		  InventoryDemo \
          Minesweeper \
          ScrollablePaneDemo \
          TabControlDemo \
          TextDemo \
          TreeDemo \
          $(CEGUILUA_DEMOS)
endif

EXTRA_DIST = ReadMe.txt

install-exec-local:
	if test ! -d $(top_builddir)/bin ; then \
		mkdir $(top_builddir)/bin ; \
	fi
