# Microsoft Developer Studio Project File - Name="FalagardBase" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FalagardBase - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Falagard.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Falagard.mak" CFG="FalagardBase - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FalagardBase - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FalagardBase - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FalagardBase - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FALAGARDBASE_EXPORTS" /D "CEGUIWIDGETMODULE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\..\WidgetSets\Falagard\include" /I "..\..\..\..\..\include\\" /I "..\..\..\..\..\dependencies\include\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FALAGARDBASE_EXPORTS" /D "CEGUIWIDGETMODULE_EXPORTS" /FD /c
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
# ADD LINK32 CEGUIBase.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../../../../bin/CEGUIFalagardBase.dll" /implib:"../../../../../lib/CEGUIFalagardBase.lib" /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUIFalagardBase.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ELSEIF  "$(CFG)" == "FalagardBase - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FALAGARDBASE_EXPORTS" /D "CEGUIWIDGETMODULE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\..\..\..\WidgetSets\Falagard\include" /I "..\..\..\..\..\include\\" /I "..\..\..\..\..\dependencies\include\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FALAGARDBASE_EXPORTS" /D "CEGUIWIDGETMODULE_EXPORTS" /D "_STLP_DEBUG" /FD /GZ /c
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
# ADD LINK32 CEGUIBase_d.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../../../../bin/CEGUIFalagardBase_d.dll" /implib:"../../../../../lib/CEGUIFalagardBase_d.lib" /pdbtype:sept /libpath:"..\..\..\..\..\lib" /libpath:"..\..\..\..\..\dependencies\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy "..\..\..\..\..\bin\CEGUIFalagardBase_d.dll" "..\..\..\..\..\Samples\bin"
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "FalagardBase - Win32 Release"
# Name "FalagardBase - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalCheckbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalComboDropList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalCombobox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalFrameWindow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalListHeader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalListHeaderProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalListHeaderSegment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalListHeaderSegmentProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalListbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalMenuBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalMenuItem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalModule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalMultiColumnList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalMultiLineEditbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalMultiLineEditboxProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalProgressBarProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalScrollablePane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalScrollbarProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalSlider.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalSliderProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalSpinner.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalStatic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalStaticImage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalStaticImageProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalStaticProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalStaticText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalStaticTextProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalSystemButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalTabButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalTabControl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalTabControlProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalTabPane.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalThumb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalTitlebar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\src\FalTooltip.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalCheckbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalComboDropList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalCombobox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalFrameWindow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalListHeader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalListHeaderProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalListHeaderSegment.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalListHeaderSegmentProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalListbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalMenuBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalMenuItem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalModule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalMultiColumnList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalMultiLineEditbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalMultiLineEditboxProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalPopupMenu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalProgressBar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalProgressBarProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalRadioButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalScrollablePane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalScrollbarProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalSlider.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalSliderProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalSpinner.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalStatic.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalStaticImage.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalStaticImageProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalStaticProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalStaticText.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalStaticTextProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalSystemButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalTabButton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalTabControl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalTabControlProperties.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalTabPane.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalThumb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalTitlebar.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\..\WidgetSets\Falagard\include\FalTooltip.h
# End Source File
# End Group
# End Target
# End Project
