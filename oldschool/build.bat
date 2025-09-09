@echo off

set name=oldschool
set builddir=bin\debug
set tooldir=bin\tools
set projdir=%~dp0
set projdir=%projdir:\=/%

rem setup directories
if not exist "bin" mkdir "bin"
if not exist "%builddir%" mkdir "%builddir%"
if not exist "%tooldir%" mkdir "%tooldir%"
if not exist "pdb" mkdir "pdb"
if not exist "obj" mkdir "obj"

rem compile the program
set common_flags=/WL /nologo /Zi /Od
set common_linker=/DEBUG /INCREMENTAL:NO /NODEFAULTLIB kernel32.lib

set flags=%common_flags% /Fo"obj\%name%.obj" /Fd"pdb\compiler_%name%.pdb"
set link=%common_linker% /SUBSYSTEM:console /OUT:"%builddir%\%name%.exe" /PDB:"pdb\linker_%name%.pdb"

cl.exe %flags% src\win32_main.c /link %link%
