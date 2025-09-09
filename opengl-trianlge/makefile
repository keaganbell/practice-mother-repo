target=triangle
targetexe=windows_$(target).exe
targetdll=$(target).dll
exesrc="src/windows_$(target).cpp"
dllsrc="src/$(target).cpp"

cflags=-WL /nologo /Foobj\\
inc=/I"ext\\inc"
common_linker_flags = /INCREMENTAL:NO
exe_linker_flags = $(common_linker_flags) user32.lib gdi32.lib kernel32.lib opengl32.lib
dll_linker_flags = $(common_linker_flags) /EXPORT:Initialize /EXPORT:UpdateAndRender

exe_defines=-DAPP_DLL_NAME="\"$(targetdll)\""

ifeq ($(build), 1)
cflags += -DRELEASE_BUILD
else
cflags += /Zi
common_linker_flags += /DEBUG
exe_linker_flags += /PDB:"pdb\\windows_$(target).pdb"
dll_linker_flags += /PDB:"pdb\\$(target).pdb"
endif

ifeq ($(opt), 1)
# small size
cflags += /O1
else ifeq ($(opt), 2)
# speed
cflags += /O2
else ifeq ($(opt), x)
# efficiency (some speed and size)
cflags += /Ox
else
cflags += /Od
endif

.PHONY: executable
executable: | obj pdb
	@cl $(cflags) $(inc) $(exe_defines) /Fd"pdb\\debug.pdb" $(exesrc) /link $(exe_linker_flags)

.PHONY: all
all: dll executable

.PHONY: dll
dll: | obj pdb
	@cl $(cflags) $(inc) /Fd"pdb\\dll_debug.pdb" $(dllsrc) /LD /link $(dll_linker_flags) 

obj:
	@mkdir obj

pdb:
	@mkdir pdb

.PHONY: clean
clean:
	@del /Q "pdb\\*"
	@del /Q "obj\\*"
	@del $(targetexe)
	@del $(targetdll)
	@del temp_$(targetdll)
	@del $(target).exp
	@del $(target).lib
