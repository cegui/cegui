# Microsoft Developer Studio Project File - Name="TaharezLook" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=TaharezLook - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TaharezLook.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TaharezLook.mak" CFG="TaharezLook - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TaharezLook - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "TaharezLook - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TaharezLook - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAHAREZLOOK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\..\WidgetSets\TaharezLook\include" /I "..\..\..\..\..\include\\" /I "..\..\..\..\..\dependencies\include\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "TAHAREZLOOK_EXPORTS" /FD /c
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
# ADD LINK32 CEGUIBase.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../../../bin/CEGUITaharezLook.dll" /implib:"../../../../../lib/CEGUITaharezLook.lib" /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUITaharezLook.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "TaharezLook - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "TAHAREZLOOK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\WidgetSets\TaharezLook\include" /I "..\..\..\..\..\include\\" /I "..\..\..\..\..\dependencies\include\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "TAHAREZLOOK_EXPORTS" /D "_STLP_DEBUG" /FD /GZ /c
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
# ADD LINK32 CEGUIBase_d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/CEGUITaharezLook_d.dll" /implib:"../../../../../lib/CEGUITaharezLook_d.lib" /pdbtype:sept /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUITaharezLook_d.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "TaharezLook - Win32 Release"
# Name "TaharezLook - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLAlternateProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLCheckbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLCloseButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLCombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLComboDropList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLComboEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLFrameWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLListbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLListHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLListHeaderSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLMiniHorzScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLMiniHorzScrollbarThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLMiniVertScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLMiniVertScrollbarThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLModule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLMultiColumnList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLMultiLineEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLSlider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLSliderThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLSpinner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLTabButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLTabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLTabPane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLTitlebar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLVertScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLVertScrollbarThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\src\TLVUMeter.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLAlternateProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLCheckbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLCloseButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLCombobox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLComboDropList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLComboEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLFrameWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLListbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLListHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLListHeaderSegment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLMiniHorzScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLMiniHorzScrollbarThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLMiniVertScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLMiniVertScrollbarThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLModule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLMultiColumnList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLMultiLineEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLRadioButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLSlider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLSliderThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLSpinner.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLStatic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLTabButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLTabPane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLTitlebar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLVertScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLVertScrollbarThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\TaharezLook\include\TLVUMeter.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
