@echo off

set name=build-tool
set builddir=bin\debug
set tooldir=bin\tools
set srcdir=..\merge-source

if not exist "bin" mkdir "bin"
if not exist "%builddir%" mkdir "%builddir%"
if not exist "%tooldir%" mkdir "%tooldir%"
if not exist "pdb" mkdir "pdb"
if not exist "obj" mkdir "obj"

set includes=/I"src\\core" /I"src\\tools" /I"src\\platform"
set common_flags=/WL /nologo /Zi /Od /std:c11 %includes%
set common_linker=/DEBUG /INCREMENTAL:NO

set i=1
:loop
if "%1"=="" goto end
if "%1"=="rebuild" goto rebuild
shift
goto loop

:rebuild
set includes=/I"src\core" /I"src\tools" /I"src\platform"
set compiler=%common_flags% /MTd /Fo"obj\%name%.obj" /Fd"pdb\compiler_%name%.pdb" /DCFLAGS="\"%common_flags%\"" /DCLINKER="\"%common_linker%\""
set linker=%common_linker% /OUT:"%name%.exe" /PDB:"pdb\linker_%name%.pdb"
cl.exe %compiler% src\win32_build_tool.c /link %linker%
shift
goto loop
 
:end

.\%name%.exe
