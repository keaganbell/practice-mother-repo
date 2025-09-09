target=extendo
targetexe=$(target).exe
targetdll=$(target).dll
exesrc="src/windows_$(target).cpp"
dllsrc="src/$(target).cpp"

cflags=-WL /nologo /Zi /Foobj\\
inc=/I"C:\Program Files\Khronos Group"
common_linker_flags = /INCREMENTAL:NO /DEBUG opengl32.lib
exe_linker_flags = $(common_linker_flags) user32.lib gdi32.lib kernel32.lib /PDB:"pdb\\windows_$(target).pdb"
dll_linker_flags = $(common_linker_flags) /PDB:"pdb\\$(target).pdb" /EXPORT:Initialize /EXPORT:UpdateAndRender

exe_defines=-DEXTENDO_DLL_NAME="\"$(targetdll)\""

.PHONY: dll
dll: | obj pdb
	@cl $(cflags) $(inc) /Fd"pdb\\dll_debug.pdb" $(dllsrc) /LD /link $(dll_linker_flags) 

.PHONY: executable
executable: | obj pdb
	@cl $(cflags) $(inc) $(exe_defines) /Fd"pdb\\debug.pdb" $(exesrc) /link $(exe_linker_flags)

.PHONY: all
all: executable dll

obj:
	@mkdir obj

pdb:
	@mkdir pdb

.PHONY: clean
clean:
	@del /Q "pdb\\*"
	@del $(targetexe)
	@del $(targetdll)
