targetname := soundbyte
targetsrc := src\\windows_$(targetname).cpp
targetexe := $(targetname).exe

compile_flags := /WL /nologo /Foobj\\ /Zi /O2 /Fdpdb\\compiler_$(targetname).pdb

linker_flags := /INCREMENTAL:NO /DEBUG
linker_flags += kernel32.lib user32.lib gdi32.lib

builddir := bin\\debug
datadir := $(builddir)\\data
output := /OUT:$(builddir)\\$(targetexe) /PDB:pdb\\linker_$(targetname).pdb

.PHONY: $(targetname)
$(targetname): | obj pdb $(builddir)
	@cl $(compile_flags) $(defines) $(targetsrc) /link $(linker_flags) $(output)

pdb:
	@mkdir $@

obj:
	@mkdir $@

$(builddir): | bin
	@mkdir $@

bin:
	@mkdir $@

$(datadir): | $(builddir)
	@mkdir $@

.PHONY: clean
clean:
	@del /Q pdb\\*
	@del /Q obj\\*
	@rmdir /s bin
