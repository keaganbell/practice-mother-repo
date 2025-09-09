@echo off

set name=compute
set builddir=bin\debug


rem setup directories
if not exist "bin" mkdir "bin"
if not exist "%builddir%" mkdir "%builddir%"
if not exist "pdb" mkdir "pdb"
if not exist "obj" mkdir "obj"


rem build the program
set compile_flags=/WL /nologo /Foobj\%name%.obj /Zi /Od /Fdpdb\compiler_%name%.pdb

set linker_output=/OUT:"%builddir%\%name%.exe" /PDB:"pdb\linker_%name%.pdb"
set libraries=kernel32.lib user32.lib gdi32.lib opengl32.lib
set linker_flags=/INCREMENTAL:NO /DEBUG %libraries% %linker_output%

cl.exe %compile_flags% src\windows_compute.cpp /link %linker_flags%
