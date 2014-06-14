# Microsoft Developer Studio Generated NMAKE File, Based on StudManage.dsp
!IF "$(CFG)" == ""
CFG=StudManage - Win32 Debug
!MESSAGE No configuration specified. Defaulting to StudManage - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "StudManage - Win32 Release" && "$(CFG)" != "StudManage - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StudManage.mak" CFG="StudManage - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StudManage - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "StudManage - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StudManage - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\StudManage.exe"


CLEAN :
	-@erase "$(INTDIR)\CInfo.obj"
	-@erase "$(INTDIR)\CInfoManage.obj"
	-@erase "$(INTDIR)\CManage.obj"
	-@erase "$(INTDIR)\CMyFile.obj"
	-@erase "$(INTDIR)\CMyString.obj"
	-@erase "$(INTDIR)\CStudManage.obj"
	-@erase "$(INTDIR)\CTable.obj"
	-@erase "$(INTDIR)\DLinkList.obj"
	-@erase "$(INTDIR)\Exception.obj"
	-@erase "$(INTDIR)\Fit.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\MyBiTree_main.obj"
	-@erase "$(INTDIR)\MyCopy.obj"
	-@erase "$(INTDIR)\UI.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\StudManage.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\StudManage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\StudManage.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\StudManage.pdb" /machine:I386 /out:"$(OUTDIR)\StudManage.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DLinkList.obj" \
	"$(INTDIR)\MyBiTree_main.obj" \
	"$(INTDIR)\CTable.obj" \
	"$(INTDIR)\CMyFile.obj" \
	"$(INTDIR)\MyCopy.obj" \
	"$(INTDIR)\CMyString.obj" \
	"$(INTDIR)\UI.obj" \
	"$(INTDIR)\Fit.obj" \
	"$(INTDIR)\Exception.obj" \
	"$(INTDIR)\CInfo.obj" \
	"$(INTDIR)\CInfoManage.obj" \
	"$(INTDIR)\CStudManage.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\CManage.obj"

"$(OUTDIR)\StudManage.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\StudManage.exe" "$(OUTDIR)\StudManage.bsc"


CLEAN :
	-@erase "$(INTDIR)\CInfo.obj"
	-@erase "$(INTDIR)\CInfo.sbr"
	-@erase "$(INTDIR)\CInfoManage.obj"
	-@erase "$(INTDIR)\CInfoManage.sbr"
	-@erase "$(INTDIR)\CManage.obj"
	-@erase "$(INTDIR)\CManage.sbr"
	-@erase "$(INTDIR)\CMyFile.obj"
	-@erase "$(INTDIR)\CMyFile.sbr"
	-@erase "$(INTDIR)\CMyString.obj"
	-@erase "$(INTDIR)\CMyString.sbr"
	-@erase "$(INTDIR)\CStudManage.obj"
	-@erase "$(INTDIR)\CStudManage.sbr"
	-@erase "$(INTDIR)\CTable.obj"
	-@erase "$(INTDIR)\CTable.sbr"
	-@erase "$(INTDIR)\DLinkList.obj"
	-@erase "$(INTDIR)\DLinkList.sbr"
	-@erase "$(INTDIR)\Exception.obj"
	-@erase "$(INTDIR)\Exception.sbr"
	-@erase "$(INTDIR)\Fit.obj"
	-@erase "$(INTDIR)\Fit.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\MyBiTree_main.obj"
	-@erase "$(INTDIR)\MyBiTree_main.sbr"
	-@erase "$(INTDIR)\MyCopy.obj"
	-@erase "$(INTDIR)\MyCopy.sbr"
	-@erase "$(INTDIR)\UI.obj"
	-@erase "$(INTDIR)\UI.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\StudManage.bsc"
	-@erase "$(OUTDIR)\StudManage.exe"
	-@erase "$(OUTDIR)\StudManage.ilk"
	-@erase "$(OUTDIR)\StudManage.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\StudManage.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\StudManage.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DLinkList.sbr" \
	"$(INTDIR)\MyBiTree_main.sbr" \
	"$(INTDIR)\CTable.sbr" \
	"$(INTDIR)\CMyFile.sbr" \
	"$(INTDIR)\MyCopy.sbr" \
	"$(INTDIR)\CMyString.sbr" \
	"$(INTDIR)\UI.sbr" \
	"$(INTDIR)\Fit.sbr" \
	"$(INTDIR)\Exception.sbr" \
	"$(INTDIR)\CInfo.sbr" \
	"$(INTDIR)\CInfoManage.sbr" \
	"$(INTDIR)\CStudManage.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\CManage.sbr"

"$(OUTDIR)\StudManage.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\StudManage.pdb" /debug /machine:I386 /out:"$(OUTDIR)\StudManage.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DLinkList.obj" \
	"$(INTDIR)\MyBiTree_main.obj" \
	"$(INTDIR)\CTable.obj" \
	"$(INTDIR)\CMyFile.obj" \
	"$(INTDIR)\MyCopy.obj" \
	"$(INTDIR)\CMyString.obj" \
	"$(INTDIR)\UI.obj" \
	"$(INTDIR)\Fit.obj" \
	"$(INTDIR)\Exception.obj" \
	"$(INTDIR)\CInfo.obj" \
	"$(INTDIR)\CInfoManage.obj" \
	"$(INTDIR)\CStudManage.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\CManage.obj"

"$(OUTDIR)\StudManage.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("StudManage.dep")
!INCLUDE "StudManage.dep"
!ELSE 
!MESSAGE Warning: cannot find "StudManage.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "StudManage - Win32 Release" || "$(CFG)" == "StudManage - Win32 Debug"
SOURCE=.\DLinkList.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\DLinkList.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\DLinkList.obj"	"$(INTDIR)\DLinkList.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MyBiTree_main.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\MyBiTree_main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\MyBiTree_main.obj"	"$(INTDIR)\MyBiTree_main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CTable.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CTable.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CTable.obj"	"$(INTDIR)\CTable.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CMyFile.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CMyFile.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CMyFile.obj"	"$(INTDIR)\CMyFile.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MyCopy.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\MyCopy.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\MyCopy.obj"	"$(INTDIR)\MyCopy.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CMyString.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CMyString.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CMyString.obj"	"$(INTDIR)\CMyString.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\UI.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\UI.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\UI.obj"	"$(INTDIR)\UI.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Fit.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\Fit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\Fit.obj"	"$(INTDIR)\Fit.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\Exception.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\Exception.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\Exception.obj"	"$(INTDIR)\Exception.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CInfo.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CInfo.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CInfo.obj"	"$(INTDIR)\CInfo.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CInfoManage.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CInfoManage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CInfoManage.obj"	"$(INTDIR)\CInfoManage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CManage.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CManage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CManage.obj"	"$(INTDIR)\CManage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\CStudManage.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\CStudManage.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\CStudManage.obj"	"$(INTDIR)\CStudManage.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\main.cpp

!IF  "$(CFG)" == "StudManage - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "StudManage - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

