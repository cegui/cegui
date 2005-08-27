# Microsoft Developer Studio Project File - Name="CEGUILua" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CEGUILua - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CEGUILua.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CEGUILua.mak" CFG="CEGUILua - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CEGUILua - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CEGUILua - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CEGUILua - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../../../bin"
# PROP Intermediate_Dir "Output/release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CEGUILUA_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\..\include" /I "..\..\..\..\..\dependencies\include" /I "..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include" /I "..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CEGUILUA_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 CEGUIBase.lib lua_and_tolua++.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /implib:"../../../../../lib/CEGUILua.lib" /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUILua.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "CEGUILua - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "CEGUILua___Win32_Debug"
# PROP BASE Intermediate_Dir "CEGUILua___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../bin"
# PROP Intermediate_Dir "Output/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CEGUILUA_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\include" /I "..\..\..\..\..\dependencies\include" /I "..\..\..\..\..\ScriptingModules\CEGUILua\lua_and_tolua++\include" /I "..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "CEGUILUA_EXPORTS" /D "_STLP_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CEGUIBase_d.lib lua_and_tolua++_d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/CEGUILua_d.dll" /implib:"../../../../../lib/CEGUILua_d.lib" /pdbtype:sept /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUILua_d.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "CEGUILua - Win32 Release"
# Name "CEGUILua - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\src\CEGUILua.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\src\lua_CEGUI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\src\required.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\src\required.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\include\CEGUILua.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\include\lua_CEGUI.h
# End Source File
# End Group
# Begin Group "package"

# PROP Default_Filter ""
# Begin Group "elements"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ButtonBase.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Checkbox.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Combobox.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ComboDropList.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\DragContainer.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Editbox.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\FrameWindow.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\GUISheet.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ItemEntry.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ItemListBase.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Listbox.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ListboxItem.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ListboxTextItem.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ListHeader.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ListHeaderSegment.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Menubar.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\MenuBase.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\MenuItem.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\MultiColumnList.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\MultiLineEditbox.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\PopupMenu.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ProgressBar.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\PushButton.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\RadioButton.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ScrollablePane.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Scrollbar.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\ScrolledContainer.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Slider.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Spinner.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Static.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\StaticImage.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\StaticText.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\TabButton.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\TabControl.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\TabPane.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\TextItem.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Thumb.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Titlebar.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\elements\Tooltip.pkg
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Basic.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\CEGUI.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\EventSet.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Font.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\FontManager.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\HelperFunctions.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Image.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Imageset.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\ImagesetManager.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\InputEvent.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Key.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Logger.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Makefile.am
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\MouseCursor.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\PropertyHelper.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Renderer.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Scheme.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\SchemeManager.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\System.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Texture.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\UnifiedCoordSystem.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\Window.pkg
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\ScriptingModules\CEGUILua\LuaScriptModule\package\WindowManager.pkg
# End Source File
# End Group
# End Target
# End Project
