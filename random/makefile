cflags = -WL /Zi /Od /nologo /Foobj\\
linker = /INCREMENTAL:NO /DEBUG /PDB:"pdb\\random.pdb"

.PHONY: exe
exe: | obj pdb
	cl $(cflags) /Fd"pdb\\debug.pdb" src/random.cpp /link 
obj:
	@mkdir obj

pdb:
	@mkdir pdb

.PHONY: clean
clean:
	@del /Q "pdb\\*"
	@del /Q "obj\\*"
	@del random.exe
