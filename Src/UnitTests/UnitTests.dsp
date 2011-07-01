# Microsoft Developer Studio Project File - Name="UnitTests" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=UnitTests - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "UnitTests.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "UnitTests.mak" CFG="UnitTests - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "UnitTests - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "UnitTests - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Gamedesk/Cpp/UnitTests", IHAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "UnitTests - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\Tmp\UnitTests\Release"
# PROP Intermediate_Dir "..\Tmp\UnitTests\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNITTESTS_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GX /O2 /Ob2 /I "../Core/Common" /I "../Core/Win32" /I "../Engine/Common" /I "../Engine/Win32" /D UNITTESTS_API=__declspec(dllexport) /Yu"UnitTests.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "NDEBUG"
# ADD RSC /l 0xc0c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 Core.lib Engine.lib /nologo /dll /machine:I386 /out:"..\..\Bin\UnitTests.dll" /libpath:"../Lib"

!ELSEIF  "$(CFG)" == "UnitTests - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\Tmp\UnitTests\Debug"
# PROP Intermediate_Dir "..\Tmp\UnitTests\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "UNITTESTS_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GX /Zi /Od /I "../Core/Common" /I "../Core/Win32" /I "../Engine/Common" /I "../Engine/Win32" /D UNITTESTS_API=__declspec(dllexport) /D "GD_DEBUG" /Yu"UnitTests.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0c /d "_DEBUG"
# ADD RSC /l 0xc0c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Core_d.lib Engine_d.lib opengl32.lib glut32.lib /nologo /dll /debug /machine:I386 /out:"..\..\Bin\UnitTests_d.dll" /pdbtype:sept /libpath:"../Lib"

!ENDIF 

# Begin Target

# Name "UnitTests - Win32 Release"
# Name "UnitTests - Win32 Debug"
# Begin Group "Tests"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TestConfigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TestMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\TestString.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\UnitTests.cpp

!IF  "$(CFG)" == "UnitTests - Win32 Release"

# ADD CPP /Yc"UnitTests.h"

!ELSEIF  "$(CFG)" == "UnitTests - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\UnitTests.h
# End Source File
# End Target
# End Project
