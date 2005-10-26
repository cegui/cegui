SRC=tolua.c toluabind.c
INCLUDE=-I../lua_and_tolua++/include
LIB=-L../lua_and_tolua++/src/.libs
LINK=-lCEGUIluatoluapp
PROG=tolua++cegui

all:
	gcc $(SRC) -o $(PROG) $(INCLUDE) $(LIB) $(LINK)
