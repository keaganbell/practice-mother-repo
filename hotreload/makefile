cflags=-WL /nologo /Zi /Foobj\\
#inc=/I"C:\Program Files\Khronos Group"
common_linker_flags = /INCREMENTAL:NO /DEBUG
exe_linker_flags = $(common_linker_flags) user32.lib gdi32.lib kernel32.lib /PDB:"pdb\\windows_hotreload.pdb"
dll_linker_flags = $(common_linker_flags) /PDB:"pdb\\hotreload.pdb" /EXPORT:Initialize /EXPORT:UpdateAndRender

.PHONY: dll
dll: | obj pdb
	@cl $(cflags) $(inc) /Fd"pdb\\hotreload_debug.pdb" src/hotreload.cpp /LD /link $(dll_linker_flags) 

.PHONY: executable
executable: | obj pdb
	@cl $(cflags) $(inc) /Fd"pdb\\windows_hotreload_debug.pdb" src/windows_hotreload.cpp /link $(exe_linker_flags)

.PHONY: all
all: executable dll

obj:
	@mkdir obj

pdb:
	@mkdir pdb

.PHONY: clean
clean:
	@del /Q "pdb\\*"
	@del hotreload.*
	@del windows_hotreload.exe
