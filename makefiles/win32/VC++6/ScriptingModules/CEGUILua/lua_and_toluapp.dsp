# Microsoft Developer Studio Project File - Name="lua_and_toluapp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=lua_and_toluapp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lua_and_toluapp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lua_and_toluapp.mak" CFG="lua_and_toluapp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "lua_and_toluapp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "lua_and_toluapp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lua_and_toluapp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../bin"
# PROP Intermediate_Dir "Output\release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LUA_AND_TOLUA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "LUA_AND_TOLUA_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /implib:"../../../../../lib/lua_and_tolua++.lib" /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\lua_and_tolua++.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "lua_and_toluapp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../bin"
# PROP Intermediate_Dir "Output\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LUA_AND_TOLUA_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "LUA_AND_TOLUA_EXPORTS" /D "_STLP_DEBUG" /YX /FD /GZ /c
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/lua_and_tolua++_d.dll" /implib:"../../../../../lib/lua_and_tolua++_d.lib" /pdbtype:sept /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\lua_and_tolua++_d.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "lua_and_toluapp - Win32 Release"
# Name "lua_and_toluapp - Win32 Debug"
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include\lauxlib.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include\lua.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include\lualib.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include\tolua++.h"
# End Source File
# End Group
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "tolua++"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\tolua++\tolua_event.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\tolua++\tolua_event.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\tolua++\tolua_is.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\tolua++\tolua_map.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\tolua++\tolua_push.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\tolua++\tolua_to.c"
# End Source File
# End Group
# Begin Group "lua"

# PROP Default_Filter ""
# Begin Group "lib"

# PROP Default_Filter ""
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\lauxlib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\lbaselib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\ldblib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\liolib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\lmathlib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\loadlib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\lstrlib.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lib\ltablib.c"
# End Source File
# End Group
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lapi.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lapi.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lcode.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lcode.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ldebug.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ldebug.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ldo.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ldo.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ldump.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lfunc.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lfunc.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lgc.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lgc.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\llex.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\llex.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\llimits.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lmem.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lmem.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lobject.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lobject.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lopcodes.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lopcodes.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lparser.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lparser.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lstate.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lstate.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lstring.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lstring.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ltable.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ltable.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ltests.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ltm.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\ltm.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lundump.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lundump.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lvm.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lvm.h"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lzio.c"
# End Source File
# Begin Source File

SOURCE="..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\src\lzio.h"
# End Source File
# End Group
# End Group
# End Target
# End Project
