@echo off

set loc=%cd%
set me=%~dp0
set src=%me%src\

set buildpat=*%1*_main.c
if "%~1"=="" set buildpat=%src%*_main.c

set asmpat=*%2*.asm
if "%~2"=="" set asmpat=%src%*.asm

IF NOT EXIST build mkdir build
pushd build

for %%g in (%asmpat%) DO (
    where /q nasm && (
          call nasm -f win64 -o %%~ng.obj %%g
          call lib -nologo %%~ng.obj
    )
)

for %%g in (%buildpat%) DO (
    where /q cl && (
        call cl -arch:AVX2 -Zi -W4 -nologo %%g
    )
)

popd
