@echo off
set name=meta
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

set common_flags=/std:c++20 /WL /nologo /Zi /Od /DPROJDIR="\"%projdir%\""
set common_linker=/INCREMENTAL:NO /DEBUG


rem build and run preprocess ++++++++++++++++++++++++++++++++++++
set preprocess_name=simple_preprocess
set preprocess_flags=%common_flags% /Fo"obj\%preprocess_name%.obj" /Fd"pdb\compiler_%preprocess_name%.pdb"
set preprocess_link=%common_linker% /OUT:"%tooldir%\%preprocess_name%.exe" /PDB:"pdb\%preprocess_name%.pdb"

cl.exe %preprocess_flags% src\simple_preprocess.cpp /link %preprocess_link%


rem build program ++++++++++++++++++++++++++++++++++++++++++++++
set compile_flags=%common_flags% /Fo"obj\%name%.obj" /Fd"pdb\compiler_%name%.pdb"

set linker_output=/OUT:"%builddir%\%name%.exe" /PDB:"pdb\linker_%name%.pdb"
set libraries=kernel32.lib user32.lib gdi32.lib opengl32.lib
set linker_flags=%common_linker% %libraries% %linker_output%

rem cl.exe %compile_flags% src\main.cpp /link %linker_flags%
