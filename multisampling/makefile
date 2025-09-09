targetname := multisample
targetsrc := src\\windows_$(targetname).cpp
targetexe := $(targetname).exe

compile_flags := /WL /nologo /Foobj\\ /Zi /Od /Fdpdb\\compiler_$(targetname).pdb

linker_flags := /INCREMENTAL:NO /DEBUG
linker_flags += kernel32.lib user32.lib gdi32.lib opengl32.lib

builddir := bin\\debug
output := /OUT:$(builddir)\\$(targetexe) /PDB:pdb\\linker_$(targetname).pdb

.PHONY: $(targetname)
$(targetname): | obj pdb $(builddir)
	@cl $(compile_flags) $(targetsrc) /link $(linker_flags) $(output)

pdb:
	@mkdir $@

obj:
	@mkdir $@

$(builddir): | bin
	@mkdir $@

bin:
	@mkdir $@

.PHONY: clean
clean:
	@del /Q pdb\\*
	@del /Q obj\\*
	@del /Q $(builddir)\\*
