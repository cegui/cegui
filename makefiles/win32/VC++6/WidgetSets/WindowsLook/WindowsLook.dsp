# Microsoft Developer Studio Project File - Name="WindowsLook" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=WindowsLook - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "WindowsLook.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "WindowsLook.mak" CFG="WindowsLook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "WindowsLook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "WindowsLook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "WindowsLook - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WINDOWSLOOK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\..\WidgetSets\WindowsLook\include" /I "..\..\..\..\..\include\\" /I "..\..\..\..\..\dependencies\include\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "WINDOWSLOOK_EXPORTS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 CEGUIBase.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../../../bin/CEGUIWindowsLook.dll" /implib:"../../../../../lib/CEGUIWindowsLook.lib" /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUIWindowsLook.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "WindowsLook - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../../../bin"
# PROP Intermediate_Dir "Output/debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WINDOWSLOOK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\WidgetSets\WindowsLook\include" /I "..\..\..\..\..\include\\" /I "..\..\..\..\..\dependencies\include\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "WINDOWSLOOK_EXPORTS" /D "_STLP_DEBUG" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 CEGUIBase_d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/CEGUIWindowsLook_d.dll" /implib:"../../../../../lib/CEGUIWindowsLook_d.lib" /pdbtype:sept /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUIWindowsLook_d.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "WindowsLook - Win32 Release"
# Name "WindowsLook - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLCheckbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLCloseButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLCombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLComboDropList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLFrameWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLHorzScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLHorzScrollbarThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLListbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLListHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLListHeaderSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLModule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLMultiColumnList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLMultiLineEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLScrollablePane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLSlider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLSliderThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLSpinner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLTabButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLTabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLTabPane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLTitlebar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLVertScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\src\WLVertScrollbarThumb.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLCheckbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLCloseButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLCombobox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLComboDropList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLFrameWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLHorzScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLHorzScrollbarThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLListbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLListHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLListHeaderSegment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLModule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLMultiColumnList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLMultiLineEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLRadioButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLScrollablePane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLSlider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLSliderThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLSpinner.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLStatic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLTabButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLTabPane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLTitlebar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLVertScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\WindowsLook\include\WLVertScrollbarThumb.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
